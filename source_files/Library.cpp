#include "Library.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <ctime>

using namespace std;

// Default constructor
Library::Library() = default;

// Singleton pattern: ensures only one Library instance exists
Library& Library::instance() {
    static Library lib; // static local ensures single instance
    return lib;
}

// Find a book by its ID. Returns pointer to book or nullptr if not found
Book* Library::findBookById(int id) {
    for (auto& b : books) {
        if (b.bookId == id) return &b;
    }
    return nullptr;
}

// Add a new book to the library and return its unique book ID
int Library::addBook(const string& title, const string& author, const string& isbn, int copies) {
    Book bk(nextBookId++, title, author, isbn, copies);
    books.push_back(bk);
    return bk.bookId;
}

// Remove a book by ID. Returns true if removed, false if not found
bool Library::removeBook(int id) {
    auto it = remove_if(books.begin(), books.end(), [&](const Book& b) { return b.bookId == id; });
    if (it == books.end()) return false; // book not found
    books.erase(it, books.end()); // erase the removed book(s)
    return true;
}

// Get all books in the library
vector<Book>& Library::getAllBooks() {
    return books;
}

// Checkout a book to a user, creating a transaction and decreasing available copies
int Library::checkoutBook(int userId, int bookId, const string& checkoutDate, const string& dueDate) {
    Book* b = findBookById(bookId);
    if (!b) throw runtime_error("Book not found");
    if (b->availableCopies <= 0) throw runtime_error("No available copies");

    b->availableCopies--; // decrease available copies

    // Create transaction
    Transaction t;
    t.transactionId = nextTransactionId++;
    t.userId = userId;
    t.bookId = bookId;
    t.checkoutDate = checkoutDate;
    t.dueDate = dueDate;
    t.returnDate = "";
    t.status = "OUT";
    transactions.push_back(t);

    return t.transactionId;
}

// Process the return of a book, calculate fine if late, and update book availability
Fine Library::processReturn(int transactionId, const string& returnDate) {
    auto it = find_if(transactions.begin(), transactions.end(),
                      [&](const Transaction& tr) { return tr.transactionId == transactionId; });
    if (it == transactions.end()) throw runtime_error("Transaction not found");
    if (it->status == "RETURNED") throw runtime_error("Already returned");

    it->returnDate = returnDate;
    it->status = "RETURNED";

    Book* b = findBookById(it->bookId);
    if (b) b->availableCopies++; // increment available copies

    // Calculate fine based on days late
    int daysLate = max(0, daysBetween(it->dueDate, returnDate));
    double amount = daysLate * 0.5; // $0.50 per day
    Fine f(amount);
    fines.push_back(f);

    return f;
}

// Helper function: calculates number of days between two dates (YYYY-MM-DD format)
int Library::daysBetween(const string& d1, const string& d2) {
    auto to_tm = [](const string& s) {
        tm tm{}; 
        int y=0,m=0,day=0;
        sscanf(s.c_str(), "%d-%d-%d", &y, &m, &day);
        tm.tm_year = y - 1900;
        tm.tm_mon = m - 1;
        tm.tm_mday = day;
        return tm;
    };
    tm t1 = to_tm(d1);
    tm t2 = to_tm(d2);
    time_t tt1 = mktime(&t1);
    time_t tt2 = mktime(&t2);
    if (tt1 == -1 || tt2 == -1) return 0;
    return static_cast<int>(difftime(tt2, tt1) / (60*60*24));
}

// Load books and transactions from CSV files
void Library::loadFromCSV(const string& booksFile, const string& transFile) {
    // Load books
    ifstream inb(booksFile);
    if (inb) {
        string line;
        while (getline(inb, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string token;
            int id = 0, total = 0, avail = 0;
            Book b;
            getline(ss, token, ','); id = stoi(token);
            getline(ss, b.title, ',');
            getline(ss, b.author, ',');
            getline(ss, b.isbn, ',');
            getline(ss, token, ','); total = stoi(token);
            getline(ss, token, ','); avail = stoi(token);
            b.bookId = id;
            b.totalCopies = total;
            b.availableCopies = avail;
            books.push_back(b);
            nextBookId = max(nextBookId, id + 1); // ensure next ID is unique
        }
    }

    // Load transactions
    ifstream intf(transFile);
    if (intf) {
        string line;
        while (getline(intf, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            Transaction t;
            string token;
            getline(ss, token, ','); t.transactionId = stoi(token);
            getline(ss, token, ','); t.userId = stoi(token);
            getline(ss, token, ','); t.bookId = stoi(token);
            getline(ss, t.checkoutDate, ',');
            getline(ss, t.dueDate, ',');
            getline(ss, t.returnDate, ',');
            getline(ss, t.status, ',');
            transactions.push_back(t);

            //Emma Das