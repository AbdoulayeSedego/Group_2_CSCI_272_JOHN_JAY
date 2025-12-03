#ifndef LIBRARY_H
#define LIBRARY_H

#include <vector>
#include <string>
#include "Book.h"
#include "Transaction.h"
#include "Fine.h"

using namespace std;

// Singleton library class: manages books, transactions, fines, and file persistence
class Library {
private:
    vector<Book> books;               // Collection of all books in the library
    vector<Transaction> transactions; // All checkout/return transactions
    vector<Fine> fines;               // Fines issued for late returns

    int nextBookId = 1;        // ID to assign to the next book added
    int nextTransactionId = 1; // ID to assign to the next transaction

    // Private constructor for singleton pattern
    Library();

public:
    // Delete copy constructor and assignment operator to enforce singleton
    Library(const Library&) = delete;
    Library& operator=(const Library&) = delete;

    // Get the single instance of the Library
    static Library& instance();

    // Find a book by its unique ID. Returns nullptr if not found
    Book* findBookById(int id);

    // Generic search using a predicate (lambda or function)
    // Example: searchBooks([](Book& b){ return b.title == "Book"; });
    template <typename Predicate>
    vector<Book*> searchBooks(Predicate pred) {
        vector<Book*> results;
        for (auto& b : books) {
            if (pred(b)) results.push_back(&b);
        }
        return results;
    }

    // Inventory management
    int addBook(const string& title, const string& author, const string& isbn, int copies); // Add new book
    bool removeBook(int id);                                                                    // Remove book by ID
    vector<Book>& getAllBooks();                                                                // Get reference to all books

    // Rental operations
    int checkoutBook(int userId, int bookId, const string& checkoutDate, const string& dueDate); // Checkout book
    Fine processReturn(int transactionId, const string& returnDate);                               // Process book return and calculate fine

    // Utility function: calculate days between two YYYY-MM-DD dates
    static int daysBetween(const string& d1, const string& d2);

    // Persistence: load/save books and transactions to CSV files
    void loadFromCSV(const string& booksFile = "books.csv", const string& transFile = "transactions.csv");
    void saveToCSV(const string& booksFile = "books.csv", const string& transFile = "transactions.csv");

    // Accessors for transaction and fine logs
    vector<Transaction>& getTransactions();
    vector<Fine>& getFines();
};

#endif // LIBRARY_H

// Emma Das
