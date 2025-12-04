#include "Library.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <ctime>

// ==================== CONSTRUCTOR ====================

Library::Library() = default;

// ==================== SINGLETON ====================

Library& Library::instance() {
    static Library lib;
    return lib;
}

// ==================== BOOK LOOKUP ====================

Book* Library::findBookById(int id) {
    for (auto& b : books) {
        if (b.getBookId() == id)
            return &b;
    }
    return nullptr;
}

// ==================== ADD BOOK ====================

int Library::addBook(const std::string& title,
                     const std::string& author,
                     const std::string& isbn,
                     int copies)
{
    if (copies < 0)
        throw std::invalid_argument("Copies cannot be negative");

    int newId = nextBookId++;

    // totalCopies = copies, availableCopies = copies at creation
    Book bk(newId, title, author, isbn, copies, copies);

    books.push_back(bk);

    return newId;
}

// ==================== REMOVE BOOK ====================

bool Library::removeBook(int id) {
    auto it = std::remove_if(books.begin(), books.end(),
        [&](const Book& b) {
            return b.getBookId() == id;
        });

    if (it == books.end()) return false;

    books.erase(it, books.end());
    return true;
}

// ==================== GET ALL BOOKS ====================

std::vector<Book>& Library::getAllBooks() {
    return books;
}

// ==================== CHECKOUT BOOK ====================

int Library::checkoutBook(int userId,
                          int bookId,
                          const std::string& checkoutDate,
                          const std::string& dueDate)
{
    Book* b = findBookById(bookId);

    if (!b)
        throw std::runtime_error("Book not found.");

    b->checkout();  // uses Book::checkout() validation

    int tId = nextTransactionId++;

    Transaction t(tId, userId, bookId, checkoutDate, dueDate);

    transactions.push_back(t);

    return tId;
}

// ==================== PROCESS RETURN ====================

Fine Library::processReturn(int transactionId,
                            const std::string& returnDate)
{
    auto it = std::find_if(transactions.begin(),
                           transactions.end(),
                           [&](const Transaction& tr) {
                               return tr.getTransactionId() == transactionId;
                           });

    if (it == transactions.end())
        throw std::runtime_error("Transaction not found.");

    if (!it->isActive())
        throw std::runtime_error("Transaction already completed.");

    // Mark the transaction as returned
    it->completeReturn(returnDate);

    // Restore the book copy
    Book* b = findBookById(it->getBookId());
    if (b)
        b->returnBook();

    // Calculate fine
    int daysLate = it->calculateDaysLate();
    double amount = daysLate * 0.5;

    Fine fine(amount);
    fines.push_back(fine);

    return fine;
}

// ==================== DATE DIFFERENCE ====================

int Library::daysBetween(const std::string& d1,
                         const std::string& d2)
{
    auto to_tm = [](const std::string& s) {
        std::tm tm{};
        int y, m, d;
        if (sscanf(s.c_str(), "%d-%d-%d", &y, &m, &d) != 3)
            return tm;

        tm.tm_year = y - 1900;
        tm.tm_mon = m - 1;
        tm.tm_mday = d;
        return tm;
    };

    std::tm t1 = to_tm(d1);
    std::tm t2 = to_tm(d2);

    time_t tt1 = mktime(&t1);
    time_t tt2 = mktime(&t2);

    if (tt1 == -1 || tt2 == -1)
        return 0;

    double seconds = difftime(tt2, tt1);

    return static_cast<int>(seconds / (60 * 60 * 24));
}

// ==================== LOAD FROM CSV ====================

void Library::loadFromCSV(const std::string& booksFile,
                          const std::string& transFile)
{
    // -------- Load Books --------

    std::ifstream inb(booksFile);
    if (inb) {
        std::string line;

        while (std::getline(inb, line)) {
            if (line.empty()) continue;

            std::stringstream ss(line);
            std::string token;

            int id, total, avail;
            std::string title, author, isbn;

            std::getline(ss, token, ','); id = std::stoi(token);
            std::getline(ss, title, ',');
            std::getline(ss, author, ',');
            std::getline(ss, isbn, ',');
            std::getline(ss, token, ','); total = std::stoi(token);
            std::getline(ss, token, ','); avail = std::stoi(token);

            Book b(id, title, author, isbn, total, avail);
            books.push_back(b);

            nextBookId = std::max(nextBookId, id + 1);
        }
    }

    // -------- Load Transactions --------

    std::ifstream intf(transFile);
    if (intf) {
        std::string line;

        while (std::getline(intf, line)) {
            if (line.empty()) continue;

            std::stringstream ss(line);
            std::string token;

            int tid, uid, bid;
            std::string checkout, due, returned, status;

            std::getline(ss, token, ','); tid = std::stoi(token);
            std::getline(ss, token, ','); uid = std::stoi(token);
            std::getline(ss, token, ','); bid = std::stoi(token);
            std::getline(ss, checkout, ',');
            std::getline(ss, due, ',');
            std::getline(ss, returned, ',');
            std::getline(ss, status, ',');

            Transaction t(tid, uid, bid, checkout, due, returned, status);
            transactions.push_back(t);

            nextTransactionId = std::max(nextTransactionId, tid + 1);
        }
    }
}

// ==================== SAVE TO CSV ====================

void Library::saveToCSV(const std::string& booksFile,
                        const std::string& transFile)
{
    // Save books
    std::ofstream outb(booksFile);

    for (const auto& b : books)
        outb << b.toCSV() << "\n";

    // Save transactions
    std::ofstream outt(transFile);

    for (const auto& t : transactions)
        outt << t.toCSV() << "\n";
}

// ==================== ACCESSORS ====================

std::vector<Transaction>& Library::getTransactions() {
    return transactions;
}

std::vector<Fine>& Library::getFines() {
    return fines;
}
            //Emma Das
