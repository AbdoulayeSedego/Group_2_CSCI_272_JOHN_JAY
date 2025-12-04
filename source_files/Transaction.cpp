/*
 * Transaction.cpp
 *Implementation file for the Transaction class.
 *Contains method definitions for rental record operations.
 * Author: Abdoulaye Sedego
 * Key Features:
 * - Tracks book checkouts and returns
 * - Calculates late days for fine computation
 * - Supports CSV serialization for file I/O
 */

#include "Transaction.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

// ==================== PRIVATE HELPER METHODS ====================

/**
 * parseDateToComponents - Parses a date string into numeric components
 * @param dateStr  - Input date string
 * @return true if parsing successful, false if format invalid
 *
 * Edge Cases Handled:
 * 1. Empty string -> returns false
 * 2. Wrong format (missing dashes) -> returns false
 * 3. Non-numeric values -> may cause parsing issues
 */
bool Transaction::parseDateToComponents(const std::string &dateStr,
                                        int &year, int &month, int &day)
{
    // ===== EDGE CASE: Empty date string =====
    if (dateStr.length() < 10) return false;

    // ===== EDGE CASE: Invalid length =====
    // "YYYY-MM-DD" should be exactly 10 characters
    if (dateStr.length() < 10)
    {
        return false;
    }

    // ===== EDGE CASE: Missing dashes =====
    // Check for dashes at positions 4 and 7
    if (dateStr[4] != '-' || dateStr[7] != '-') return false;

    // Use stringstream for parsing
    // stringstream allows us to parse formatted input easily
    std::stringstream ss(dateStr);
    char dash1, dash2; // To capture the dash characters

    // Parse format: YYYY-MM-DD
    // ss >> year reads until non-numeric character
    // ss >> dash1 reads the dash
    // Continue for month and day
    ss >> year >> dash1 >> month >> dash2 >> day;

    // ===== EDGE CASE: Parsing failure =====
    // Check if stringstream encountered errors
    if (ss.fail()) return false;

    // ===== EDGE CASE: Invalid month range =====
    if (month < 1 || month > 12) return false;
    // ===== EDGE CASE: Invalid day range =====
    if (day < 1 || day > 31) return false;

    return true; // Parsing successful
}

/**
 * dateToDays - Converts a date to approximate total days since year 0
 *
 * Purpose: Allows comparing two dates by converting to a single number
 * Note: This is a SIMPLIFIED calculation for educational purposes
 *       It doesn't perfectly handle leap years or varying month lengths
 *       But it's accurate enough for calculating rental period differences
 *
 * @param year  - Year
 * @param month - Month
 * @param day   - Day
 * @return Approximate total days since year 0
 *
 * Formula: year * 365 + month * 30 + day
 * This gives us a comparable number for date differences
 *
 * Example:
 *   dateToDays(2024, 11, 15) = 2024*365 + 11*30 + 15 = 739095
 *   dateToDays(2024, 11, 20) = 2024*365 + 11*30 + 20 = 739100
 *   Difference = 5 days (correct!)
 */
int Transaction::dateToDays(int year, int month, int day)
{
    return year * 365 + month * 30 + day;
}

// ==================== CONSTRUCTORS ====================

// Default Constructor

Transaction::Transaction()
    : transactionId(0), userId(0), bookId(0),
      checkoutDate(""), dueDate(""), returnDate(""),
      status("Active")
{
}

/**
 * Parameterized Constructor - For new checkouts
 * Used when a user checks out a book
 *
 * @param tid      - Transaction ID (must be unique)
 * @param uid      - User ID (who is borrowing)
 * @param bid      - Book ID (what is being borrowed)
 * @param checkout - Checkout date
 * @param due      - Due date (14 days after checkout)
 *
 * Edge Cases:
 * - All IDs must be non-empty (throws if empty)
 */
Transaction::Transaction(int tid, int uid, int bid,
                         std::string checkout, std::string due)
    : transactionId(tid), userId(uid), bookId(bid),
      checkoutDate(std::move(checkout)),
      dueDate(std::move(due)), returnDate(""),
      status("Active")
{
    // ===== EDGE CASE: Empty Transaction ID =====
    if (tid <= 0) throw std::invalid_argument("Transaction ID must be > 0");

    // ===== EDGE CASE: Empty User ID =====
    if (uid <= 0) throw std::invalid_argument("User ID must be > 0");

    // ===== EDGE CASE: Empty Book ID =====
    if (bid <= 0) throw std::invalid_argument("Book ID must be > 0");

    // ===== EDGE CASE: Empty checkout date =====
    if (checkoutDate.empty())
    {
        throw std::invalid_argument("Checkout date cannot be empty");
    }

    // ===== EDGE CASE: Empty due date =====
    if (dueDate.empty())
    {
        throw std::invalid_argument("Due date cannot be empty");
    }
}

/*
  Full Constructor - For loading from file
 Creates a complete transaction including return information
 Used when loading existing transactions from transactions.csv
 */
Transaction::Transaction(int tid, int uid, int bid,
                         std::string checkout, std::string due,
                         std::string returned, std::string stat)
    : transactionId(tid), userId(uid), bookId(bid),
      checkoutDate(std::move(checkout)),
      dueDate(std::move(due)), returnDate(std::move(returned)),
      status(std::move(stat))
{
    // Validation for required fields
    if (tid <= 0) throw std::invalid_argument("Transaction ID must be > 0");
    if (uid <= 0) throw std::invalid_argument("User ID must be > 0");
    if (bid <= 0) throw std::invalid_argument("Book ID must be > 0");

    // ===== EDGE CASE: Invalid status =====
    // Status should be one of: "Active", "Returned", "Returned-Late"
    if (status != "Active" &&
        status != "Returned" &&
        status != "Returned-Late")
    {
        status = "Active";
    }
}

// ==================== SETTERS ====================

void Transaction::setTransactionId(int tid) {
    if (tid <= 0) throw std::invalid_argument("Transaction ID must be > 0");
    transactionId = tid;
}

void Transaction::setUserId(int uid) {
    if (uid <= 0) throw std::invalid_argument("User ID must be > 0");
    userId = uid;
}

void Transaction::setBookId(int bid) {
    if (bid <= 0) throw std::invalid_argument("Book ID must be > 0");
    bookId = bid;
}

void Transaction::setCheckoutDate(const std::string& date) {
    checkoutDate = date;
}

void Transaction::setDueDate(const std::string& date) {
    dueDate = date;
}

void Transaction::setReturnDate(const std::string& date) {
    returnDate = date;
}

void Transaction::setStatus(const std::string& s) {
    // ===== EDGE CASE: Invalid status value =====
    // Only accept valid status values
    if (s != "Active" &&
        s != "Returned" &&
        s != "Returned-Late")
    {
        throw std::invalid_argument("Invalid status value.");
    }
    status = s;
}

// ==================== CORE FUNCTIONALITY ====================

/**
 * calculateDaysLate - Calculate how many days overdue the return is
 *
 * This is a KEY function for the library system:
 * - Used by Librarian to calculate fines
 * - Fine = daysLate * $0.50 per day
 *
 * Logic:
 * 1. If book not returned (returnDate empty) -> return 0
 * 2. Parse both dueDate and returnDate
 * 3. Calculate difference: returnDate - dueDate
 * 4. If difference <= 0, book was on time -> return 0
 * 5. If difference > 0, book was late -> return days late
 *
 * Edge Cases:
 * 1. returnDate empty (still active) -> returns 0
 * 2. returnDate == dueDate (on time) -> returns 0
 * 3. returnDate < dueDate (early) -> returns 0
 * 4. Invalid date formats -> returns 0 (safe default)
 *
 * @return Number of days late (0 if on time or still active)
 */

int Transaction::calculateDaysLate() const
{
    // ===== EDGE CASE: Book not returned yet =====
    // If returnDate is empty, the book hasn't been returned
    // No late fee until it's actually returned
    if (returnDate.empty()) return 0;

    // Parse due date into components
    int dueYear, dueMonth, dueDay;
    bool dueParsed = parseDateToComponents(dueDate, dueYear, dueMonth, dueDay);

    // Parse return date into components
    int retYear, retMonth, retDay;
    bool retParsed = parseDateToComponents(returnDate, retYear, retMonth, retDay);

    // ===== EDGE CASE: Invalid date format =====
    // If we can't parse the dates, we can't calculate lateness
    // Return 0 as a safe default
    if (!dueParsed || !retParsed)
    {
        std::cerr << "[WARNING] Could not parse dates for transaction "
                  << transactionId << ". Due: " << dueDate
                  << ", Return: " << returnDate << "\n";
        return 0;
    }

    // Convert both dates to "total days" for easy comparison
    int dueDays = dateToDays(dueYear, dueMonth, dueDay);
    int returnDays = dateToDays(retYear, retMonth, retDay);

    // Calculate the difference
    int daysLate = returnDays - dueDays;

    // ===== EDGE CASE: Book returned on time or early =====
    // If daysLate is 0 or negative, the book was not late
    if (daysLate <= 0)
    {
        return 0;
    }

    // Book was late - return the number of days
    return daysLate;
}

/**
 * completeReturn - Mark the transaction as returned
 *
 * Called when a user returns a book:
 * 1. Sets the return date
 * 2. Calculates if the return was late
 * 3. Updates status to "Returned" or "Returned-Late"
 *
 * @param returnDateStr - The date of return (format: "YYYY-MM-DD")
 *
 * Edge Cases:
 * - Empty return date -> throws invalid_argument
 * - Already returned -> throws runtime_error
 */
void Transaction::completeReturn(const std::string& returnDateStr)
{
    // ===== EDGE CASE: Empty return date =====
    if (returnDateStr.empty())
        throw std::invalid_argument("Return date cannot be empty.");

    // ===== EDGE CASE: Already returned =====
    // Cannot return a book that's already been returned
    if (status != "Active")
        throw std::runtime_error("Transaction already completed.");

    // Set the return date
    returnDate = returnDateStr;

    // Calculate if it's late and set appropriate status
    int daysLate = calculateDaysLate();

    status = (daysLate > 0 ? "Returned-Late" : "Returned");
}

/*
  display - Print transaction details to console
  Shows all transaction information in a formatted layout
 */
void Transaction::display() const
{
    std::cout << "========================================\n";
    std::cout << "Transaction ID: " << transactionId << "\n";
    std::cout << "User ID: " << userId << "\n";
    std::cout << "Book ID: " << bookId << "\n";
    std::cout << "Checkout Date: " << checkoutDate << "\n";
    std::cout << "Due Date: " << dueDate << "\n";

    // Show return date only if book has been returned
    if (returnDate.empty()) {
        std::cout << "Return Date: (Not returned)\n";
    } else {
        std::cout << "Return Date: " << returnDate << "\n";
        int late = calculateDaysLate();
        if (late > 0)
            std::cout << "Days Late: " << late << "\n";
    }

    std::cout << "Status: " << status << "\n";
    std::cout << "========================================\n";
}

/**
 toCSV - Convert transaction to CSV format for file storage
 * @return CSV formatted string
 */
std::string Transaction::toCSV() const
{
    return std::to_string(transactionId) + "," +
           std::to_string(userId) + "," +
           std::to_string(bookId) + "," +
           checkoutDate + "," +
           dueDate + "," +
           returnDate + "," +
           status;
}
