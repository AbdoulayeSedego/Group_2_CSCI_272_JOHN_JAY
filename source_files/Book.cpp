/*
 Book.cpp
Implementation file for the Book class.
Contains all method definitions for book operations.

 Author: Abdoulaye Sedego
*/

#include "Book.h"
#include <iostream>
#include <stdexcept> // For exception classes (runtime_error, invalid_argument)

// ==================== CONSTRUCTORS ====================
// Default Constructor
Book::Book()
    : bookId(0),
      title(""),
      author(""),
      isbn(""),
      totalCopies(0),
      availableCopies(0)
{}

/**
 * Parameterized Constructor
 * @param id        - Unique book identifier
 * @param t         - Book title
 * @param a         - Author name
 * @param isbnNum   - ISBN number
 * @param total     - Total copies owned by library
 * @param available - Currently available copies
 */
Book::Book(int id,
           const std::string& t,
           const std::string& a,
           const std::string& isbnNum,
           int total,
           int available)
    : bookId(id),
      title(t),
      author(a),
      isbn(isbnNum),
      totalCopies(total),
      availableCopies(available)
{
    if (id < 0)
        throw std::invalid_argument("Book ID cannot be negative.");

    if (total < 0)
        throw std::invalid_argument("Total copies cannot be negative.");

    if (available < 0)
        throw std::invalid_argument("Available copies cannot be negative.");

    if (available > total)
        throw std::invalid_argument("Available copies cannot exceed total copies.");
}

// ==================== SETTERS WITH VALIDATION ====================

/**
 * setBookId - Sets the book's unique identifier
 */
void Book::setBookId(int id) {
    // ===== EDGE CASE: Empty ID =====
    if (id < 0)
        throw std::invalid_argument("Book ID cannot be negative.");
    bookId = id;
}

/**
 setTitle - Sets the book's title
 Empty titles are allowed (some books may have no title initially)
 */
void Book::setTitle(const std::string& t) {
    title = t;
}

/**
 * setAuthor - Sets the book's author
   Empty authors are allowed (anonymous authors)
 */
void Book::setAuthor(const std::string& a) {
    author = a;
}

/**
 * setIsbn - Sets the book's ISBN
   Empty ISBN allowed (some older books don't have ISBN)
 */
void Book::setIsbn(const std::string& i) {
    isbn = i;
}

/**
 * setTotalCopies - Sets the total number of copies with validation
 *
 * Edge Cases Handled:
 * Negative total -> throws invalid_argument
 * New total < current available -> throws invalid_argument
 *    (Cannot reduce total below what's currently in the library)
 */
void Book::setTotalCopies(int total) {
    // ===== EDGE CASE: Negative total =====
    if (total < 0)
        throw std::invalid_argument("Total copies cannot be negative.");

    // ===== EDGE CASE: Total less than available =====
    // Example: If 3 copies are available, cannot set total to 2
    // This would create an impossible state
    if (total < availableCopies)
        throw std::invalid_argument(
            "Total copies cannot be less than currently available copies.");

    totalCopies = total;
}

/**
 * setAvailableCopies - Sets available copies with validation
 *
 * Edge Cases Handled:
 * 1. Negative available -> throws invalid_argument
 * 2. Available > total -> throws invalid_argument
 */
void Book::setAvailableCopies(int available) {
    // ===== EDGE CASE: Negative available =====
    if (available < 0)
        throw std::invalid_argument("Available copies cannot be negative.");

    // ===== EDGE CASE: Available exceeds total =====
    // Cannot have more books available than the library owns
    if (available > totalCopies)
        throw std::invalid_argument(
            "Available copies cannot exceed total copies.");

    availableCopies = available;
}

// ==================== CORE FUNCTIONALITY ====================

/**
 * checkout() - Process a book checkout (user borrows the book)
 *
 * What it does:
 * - Decreases availableCopies by 1
 * - Simulates a user taking a book from the shelf
 *
 * Edge Cases Handled:
 * No copies available (availableCopies == 0)
 *    - Cannot checkout what doesn't exist
 *    - Throws runtime_error with descriptive message
 *
 * Example:
 *   Book book("B001", "Clean Code", "Robert Martin", "123", 5, 2);
 *   book.checkout();  // availableCopies becomes 1
 *   book.checkout();  // availableCopies becomes 0
 *   book.checkout();  // THROWS runtime_error!
 */
void Book::checkout() {
    // ===== EDGE CASE: No copies available =====
    // Check BEFORE decrementing to prevent going negative
    if (availableCopies <= 0)
        throw std::runtime_error(
            "No copies available for checkout.");

    // Safe to decrement - we have at least 1 copy available
    availableCopies--;
}

/**
 * returnBook() - Process a book return (when a user returns a book)
 *
 * - Increases availableCopies by 1
 * - Simulates a user returning a book to the shelf
 *
 * Edge Cases Handled:
 * All copies already in library (availableCopies == totalCopies)
 *    - Cannot return more books than were checked out
 *    - This would indicate a data integrity issue
 *    - Throws runtime_error with descriptive message
 *
 * Example:
 *   Book book("B001", "Clean Code", "Robert Martin", "123", 3, 3);
 *   book.returnBook();  // THROWS! All 3 copies are already here
 */
void Book::returnBook() {
    // ===== EDGE CASE: All copies already returned =====
    // If available == total, all books are on the shelf
    // Returning another would create more copies than we own!
    if (availableCopies >= totalCopies)
        throw std::runtime_error(
            "All copies are already in the library.");

    // Safe to increment - at least we have 1 copy that is checked out
    availableCopies++;
}

// display() - Prints formatted book information to console

void Book::display() const {
    std::cout << "=============================\n";
    std::cout << "Book ID: " << bookId << "\n";
    std::cout << "Title:   " << title << "\n";
    std::cout << "Author:  " << author << "\n";
    std::cout << "ISBN:    " << isbn << "\n";
    std::cout << "Available: " << availableCopies
              << "/" << totalCopies << "\n";
    std::cout << "Status: "
              << (isAvailable() ? "AVAILABLE" : "OUT OF STOCK")
              << "\n";
    std::cout << "=============================\n";
}

/**
 * toCSV() - Converts book data to CSV format for file storage
 *
 Used by FileManager to save books to books.csv
 * Format: bookId,title,author,isbn,totalCopies,availableCopies
 *
 * Example output: "B001,Clean Code,Robert Martin,978-0132350884,5,3"
 *
 * Edge Cases Handled:
 * - Titles/authors containireturn CSV formatted string representation of the book
 */
std::string Book::toCSV() const
{
    // Build CSV string with comma separators
    return std::to_string(bookId) + "," +
            title + "," +
            author + "," +
            isbn + "," +
            std::to_string(totalCopies) + "," +
            std::to_string(availableCopies);
}
