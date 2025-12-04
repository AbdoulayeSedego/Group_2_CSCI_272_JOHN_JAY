/**
Book.h
The 'Book' class represents a book entity in the library system.
          It tracks book information and inventory (total copies vs available copies).
          This is an entity class - it holds data and provides operations on that data.

 Author: Abdoulaye Sedego
**/

#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <stdexcept> // For runtime_error exception handling

class Book
{
private:
    int bookId;             // Unique integer ID
    std::string title;
    std::string author;
    std::string isbn;
    int totalCopies;
    int availableCopies;

public:
    // ==================== CONSTRUCTORS ====================

    // Default Constructor

    Book();

    /**
     * Parameterized Constructor
     * @param id        - Unique book identifier
     * @param t         - Book title
     * @param a         - Author name
     * @param isbnNum   - ISBN number
     * @param total     - Total copies owned by library
     * @param available - Currently available copies
     */
    Book(int id,
         const std::string& t,
         const std::string& a,
         const std::string& isbnNum,
         int total,
         int available);

    // ==================== GETTERS ====================

    int getBookId() const { return bookId; }
    const std::string& getTitle() const { return title; }
    const std::string& getAuthor() const { return author; }
    const std::string& getIsbn() const { return isbn; }
    int getTotalCopies() const { return totalCopies; }
    int getAvailableCopies() const { return availableCopies; }

    // ==================== SETTERS ====================

    void setBookId(int id);
    void setTitle(const std::string& t);
    void setAuthor(const std::string& a);
    void setIsbn(const std::string& i);

    // Sets total copies with validation
    void setTotalCopies(int total);

    // Sets available copies with validation
    void setAvailableCopies(int available);

    // ==================== CORE FUNCTIONALITY ====================

    // checkout() - Decreases available copies by 1
    // Called when a user borrows this book
    void checkout();

    // returnBook() - Increases available copies by 1
    // Called when a user returns this book
    void returnBook();

    // isAvailable() - Checks if at least one copy is available

    bool isAvailable() const { return availableCopies > 0; }

    // display() - Prints book information to console
    // Formats output for user friendly display

    void display() const;

    /**
     * toCSV() - Converts book data to CSV format string
     * Used for file I/O operations (saving to books.csv)
     * Format: bookId,title,author,isbn,totalCopies,availableCopies
     * @return CSV formatted string representation of the book
     */
    std::string toCSV() const;
};

#endif
