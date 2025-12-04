#ifndef LIBRARY_H
#define LIBRARY_H

#include <vector>
#include <string>
#include "Book.h"
#include "Transaction.h"
#include "Fine.h"

// -----------------------------------------------------------------------------
// Library (Singleton)
// -----------------------------------------------------------------------------
// Responsibilities:
// - Manage all books, transactions, and fines
// - Process checkout and returns
// - File persistence (CSV)
// - Provide search functionality
//
// NOTE:
// The Library has no public constructor. Use Library::instance() to access it.
// -----------------------------------------------------------------------------

class Library {
private:
    // -----------------------
    // Internal Data Storage
    // -----------------------
    std::vector<Book> books;
    std::vector<Transaction> transactions;
    std::vector<Fine> fines;

    int nextBookId = 1;
    int nextTransactionId = 1;

    // Private constructor (Singleton)
    Library();

public:
    // Delete copy operations (Singleton enforcement)
    Library(const Library&) = delete;
    Library& operator=(const Library&) = delete;

    // Global access point
    static Library& instance();

    // -----------------------
    // Book Management
    // -----------------------

    // Find book by ID (returns nullptr if not found)
    Book* findBookById(int id);

    // Add book — returns the created book ID
    int addBook(const std::string& title,
                const std::string& author,
                const std::string& isbn,
                int copies);

    // Remove book — returns true if removed
    bool removeBook(int id);

    // Get reference to all books
    std::vector<Book>& getAllBooks();

    // -----------------------
    // Book Search (Generic)
    // -----------------------
    template <typename Predicate>
    std::vector<Book*> searchBooks(Predicate pred) {
        std::vector<Book*> results;
        for (auto& b : books) {
            if (pred(b)) results.push_back(&b);
        }
        return results;
    }

    // -----------------------
    // Checkout / Return
    // -----------------------

    // Checkout a book and create a transaction
    int checkoutBook(int userId,
                     int bookId,
                     const std::string& checkoutDate,
                     const std::string& dueDate);

    // Process return and compute any fine
    Fine processReturn(int transactionId,
                       const std::string& returnDate);

    // -----------------------
    // Date Utility
    // -----------------------
    static int daysBetween(const std::string& d1,
                           const std::string& d2);

    // -----------------------
    // File Persistence (CSV)
    // -----------------------
    void loadFromCSV(const std::string& booksFile = "books.csv",
                     const std::string& transFile = "transactions.csv");

    void saveToCSV(const std::string& booksFile = "books.csv",
                   const std::string& transFile = "transactions.csv");

    // -----------------------
    // Logs
    // -----------------------
    std::vector<Transaction>& getTransactions();
    std::vector<Fine>& getFines();
};

#endif // LIBRARY_H

// Emma Das

