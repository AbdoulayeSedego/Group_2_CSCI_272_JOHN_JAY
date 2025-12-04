/**
 * Librarian.cpp
 * Author: Abdoulaye Sedego
 * Key Responsibilities:
 * - Process book checkouts and returns
 * - Calculate late fees
 * - View and manage transactions
 **/

#include "../headers/Librarian.h"
#include "../headers/Library.h"     // For Library singleton access
#include <iostream>
#include <iomanip>
#include <limits>
#include <stdexcept>                // For exceptions
#include <ctime>                    // For getting current date

// ==================== CONSTRUCTORS ====================


 // Default Constructor

Librarian::Librarian()
    : User()  // Explicitly call base class default constructor
{
    // Set userType to "Librarian" for identification
    // Using the inherited setter from User class
    setUserType("Librarian");
}

/** Documention Only
 * Parameterized Constructor
 * Creates a fully initialized Librarian
 *
 * @param userID         - Unique ID for this librarian
 * @param name           - Full name
 * @param email          - Email address
 * @param userType       - Should be "Librarian"
 * @param membershipDate - Employment start date
 */
Librarian::Librarian(int userID, const std::string& name,
                     const std::string& email, const std::string& userType,
                     const std::string& membershipDate)
    : User(userID, name, email, userType, membershipDate)  // Base class constructor
{
    // ===== EDGE CASE: Wrong userType provided =====
    // If someone creates a Librarian but passes "Admin" as userType,
    // we should correct it to maintain data integrity
    if (getUserType() != "Librarian") {
        setUserType("Librarian");
    }
}

// ==================== OVERRIDDEN VIRTUAL FUNCTIONS ====================

/*
 * display() - Displays librarian information to console
 */
void Librarian::display() const {
    std::cout << "\n";
    std::cout << "********** LIBRARIAN INFORMATION **********\n";
    std::cout << "User ID:    " << getUserID() << "\n";
    std::cout << "Name:       " << getName() << "\n";
    std::cout << "Email:      " << getEmail() << "\n";
    std::cout << "User Type:  " << getUserType() << "\n";
    std::cout << "Start Date: " << getMembershipDate() << "\n";
    std::cout << "*******************************************\n";
    std::cout << "\n";
}

/**
 * menu() - Displays and handles the librarian menu
 *
 * This function OVERRIDES the pure virtual menu() from User class
 * Menu Options:
 * 1. Process Checkout - Check out a book to a user
 * 2. Process Return - Return a book and calculate fees
 * 3. View All Transactions - See checkout history
 * 4. Generate Report - View statistics
 * 5. Exit - Return to main menu
 *
 * Edge Cases Handled:
 * - Invalid menu choice
 * - Out of range menu choice
 * - Empty input
 */
void Librarian::menu() {
    int choice;  // User's menu selection

    // Menu loop - continues until user chooses to exit
    while (true) {
        // Display menu options
        std::cout << "\n";
        std::cout << "============================================\n";
        std::cout << "           LIBRARIAN MENU                   \n";
        std::cout << "============================================\n";
        std::cout << "  [1] Process Checkout\n";
        std::cout << "  [2] Process Return\n";
        std::cout << "  [3] View All Transactions\n";
        std::cout << "  [4] Generate Report\n";
        std::cout << "  [5] Exit to Main Menu\n";
        std::cout << "============================================\n";
        std::cout << "Enter your choice (1-5): ";

        // ===== INPUT HANDLING WITH EDGE CASES =====
        std::cin >> choice;

        // ===== EDGE CASE: Non-integer input =====
        // If user enters "abc" instead of a number, cin fails
        if (std::cin.fail()) {
            // Clear the error state
            std::cin.clear();
            // Discard invalid input from the buffer
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << "\n[ERROR] Invalid input! Please enter a number (1-5).\n";
            continue;  // Go back to start of loop
        }

        // Clear any remaining input
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // Process the menu choice using switch statement
        switch (choice) {
            case 1:
                // Process a book checkout
                std::cout << "\n--- Processing Checkout ---\n";
                processCheckout();
                break;

            case 2:
                // Process a book return
                std::cout << "\n--- Processing Return ---\n";
                processReturn();
                break;

            case 3:
                // View all transactions
                std::cout << "\n--- All Transactions ---\n";
                viewAllTransactions();
                break;

            case 4:
                // Generate report
                std::cout << "\n--- Library Report ---\n";
                generateReport();
                break;

            case 5:
                // Exit menu
                std::cout << "\nReturning to main menu...\n";
                return;  // Exit the menu() function

            default:
                // ===== EDGE CASE: Out of range choice =====
                // User entered a number, but not 1-5
                std::cout << "\n[ERROR] Invalid choice! Please select 1-5.\n";
                break;
        }
    }
}

// ==================== LIBRARIAN-SPECIFIC FUNCTIONS ====================

/**
 * Helper function to get current date as YYYY-MM-DD string
 */
static std::string getCurrentDate() {
    std::time_t now = std::time(nullptr);
    std::tm* tm = std::localtime(&now);
    char buffer[11];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", tm);
    return std::string(buffer);
}

/**
 * Helper function to add days to a date string (YYYY-MM-DD)
 * Returns new date as YYYY-MM-DD string
 */
static std::string addDays(const std::string& dateStr, int days) {
    int y, m, d;
    sscanf(dateStr.c_str(), "%d-%d-%d", &y, &m, &d);

    std::tm tm = {};
    tm.tm_year = y - 1900;
    tm.tm_mon = m - 1;
    tm.tm_mday = d + days;

    std::mktime(&tm);  // Normalizes the date

    char buffer[11];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", &tm);
    return std::string(buffer);
}

/**
 * processCheckout - Handle a book checkout operation
 *
 * Workflow:
 * 1. Prompt for User ID (integer)
 * 2. Prompt for Book ID (integer)
 * 3. Verify book exists and is available
 * 4. Create transaction record via Library::checkoutBook()
 * 5. Display confirmation
 *
 * Edge Cases Handled:
 * - Invalid user ID input
 * - Invalid book ID input
 * - Book not found
 * - No copies available
 */
void Librarian::processCheckout() {
    int userId = 0;
    int bookId = 0;

    // Get User ID (integer)
    std::cout << "Enter User ID (integer): ";
    if (!(std::cin >> userId)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "[ERROR] Invalid User ID. Must be an integer.\n";
        return;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Get Book ID (integer)
    std::cout << "Enter Book ID (integer): ";
    if (!(std::cin >> bookId)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "[ERROR] Invalid Book ID. Must be an integer.\n";
        return;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Get checkout and due dates
    std::string checkoutDate = getCurrentDate();
    std::string dueDate = addDays(checkoutDate, DEFAULT_LOAN_PERIOD);

    try {
        // Find the book first to display its title
        Book* book = Library::instance().findBookById(bookId);
        if (!book) {
            throw std::runtime_error("Book not found with ID: " + std::to_string(bookId));
        }

        if (!book->isAvailable()) {
            throw std::runtime_error("No copies available for: " + book->getTitle());
        }

        // Create transaction via Library (handles book checkout internally)
        int transId = Library::instance().checkoutBook(userId, bookId, checkoutDate, dueDate);

        // Display success message
        std::cout << "\n";
        std::cout << "========================================\n";
        std::cout << "        CHECKOUT SUCCESSFUL             \n";
        std::cout << "========================================\n";
        std::cout << "Transaction ID: " << transId << "\n";
        std::cout << "User ID: " << userId << "\n";
        std::cout << "Book: " << book->getTitle() << " (ID: " << bookId << ")\n";
        std::cout << "Checkout Date: " << checkoutDate << "\n";
        std::cout << "Due Date: " << dueDate << "\n";
        std::cout << "Status: Active\n";
        std::cout << "========================================\n";

    } catch (const std::exception& e) {
        std::cout << "[ERROR] Checkout failed: " << e.what() << "\n";
    }
}

/**
 * processReturn - Handle a book return operation
 *
 * Workflow:
 * 1. Prompt for Transaction ID (integer)
 * 2. Find transaction in Library
 * 3. Process return via Library::processReturn()
 * 4. Display fine information if applicable
 *
 * Edge Cases Handled:
 * - Invalid transaction ID input
 * - Transaction not found
 * - Transaction already completed
 */
void Librarian::processReturn() {
    int transactionId = 0;

    // Get Transaction ID (integer)
    std::cout << "Enter Transaction ID (integer): ";
    if (!(std::cin >> transactionId)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "[ERROR] Invalid Transaction ID. Must be an integer.\n";
        return;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Get return date (current date)
    std::string returnDate = getCurrentDate();

    try {
        // Process return via Library (handles transaction update and book return)
        Fine fine = Library::instance().processReturn(transactionId, returnDate);

        // Get the transaction to display details
        auto& transactions = Library::instance().getTransactions();
        Transaction* trans = nullptr;
        for (auto& t : transactions) {
            if (t.getTransactionId() == transactionId) {
                trans = &t;
                break;
            }
        }

        // Calculate days late for display
        int daysLate = trans ? trans->calculateDaysLate() : 0;
        double fineAmount = fine.getAmount();

        // Display success message
        std::cout << "\n";
        std::cout << "========================================\n";
        std::cout << "        RETURN PROCESSED                \n";
        std::cout << "========================================\n";
        std::cout << "Transaction ID: " << transactionId << "\n";
        std::cout << "Return Date: " << returnDate << "\n";

        if (daysLate > 0) {
            std::cout << "Days Late: " << daysLate << "\n";
            std::cout << "Late Fee: $" << std::fixed << std::setprecision(2) << fineAmount << "\n";
            std::cout << "Status: Returned-Late\n";
        } else {
            std::cout << "Days Late: 0 (On time!)\n";
            std::cout << "Late Fee: $0.00\n";
            std::cout << "Status: Returned\n";
        }
        std::cout << "========================================\n";

    } catch (const std::exception& e) {
        std::cout << "[ERROR] Return failed: " << e.what() << "\n";
    }
}

/**
 * calculateFine - Calculate late fee based on days overdue
 *
 * Formula: daysLate * FINE_PER_DAY
 * With FINE_PER_DAY = $0.50
 *
 * @param daysLate - Number of days the book is overdue
 * @return Fine amount in dollars
 *
 * Examples:
 *   calculateFine(0) returns 0.00
 *   calculateFine(1) returns 0.50
 *   calculateFine(10) returns 5.00
 *   calculateFine(18) returns 9.00 (matches project example)
 *
 * Edge Cases:
 * - Negative days: Returns 0 (shouldn't charge negative fine)
 * - Zero days: Returns 0 (no fine for on-time return)
 */
double Librarian::calculateFine(int daysLate) const {
    // ===== EDGE CASE: Negative or zero days late =====
    // Cannot have negative fine, and 0 days late means no fine
    if (daysLate <= 0) {
        return 0.0;
    }

    // Calculate fine using the constant rate
    // FINE_PER_DAY is defined in the header as 0.50
    double fineAmount = daysLate * FINE_PER_DAY;

    return fineAmount;
}

/**
 * viewAllTransactions - Display all transaction records
 * 1. Get all transactions from Library
 * 2. Display each transaction's details in a table format
 * 3. Show statistics (total active, total completed, etc.)
 */
void Librarian::viewAllTransactions() const {
    // Get transactions from Library singleton
    auto& transactions = Library::instance().getTransactions();

    std::cout << "\n";
    std::cout << "============================================\n";
    std::cout << "           ALL TRANSACTIONS                 \n";
    std::cout << "============================================\n";
    std::cout << "\n";

    if (transactions.empty()) {
        std::cout << "No transactions found.\n";
        std::cout << "============================================\n";
        return;
    }

    // Display header
    std::cout << std::left
              << std::setw(8) << "Trans ID"
              << std::setw(10) << "User ID"
              << std::setw(10) << "Book ID"
              << std::setw(14) << "Checkout"
              << std::setw(14) << "Due Date"
              << std::setw(14) << "Returned"
              << "Status\n";
    std::cout << "--------------------------------------------"
              << "--------------------------------------------\n";

    // Count statistics
    int activeCount = 0, returnedCount = 0, lateCount = 0;

    // Display each transaction
    for (const auto& trans : transactions) {
        std::cout << std::left
                  << std::setw(8) << trans.getTransactionId()
                  << std::setw(10) << trans.getUserId()
                  << std::setw(10) << trans.getBookId()
                  << std::setw(14) << trans.getCheckoutDate()
                  << std::setw(14) << trans.getDueDate()
                  << std::setw(14) << (trans.getReturnDate().empty() ? "-" : trans.getReturnDate())
                  << trans.getStatus() << "\n";

        // Count by status
        if (trans.getStatus() == "Active") activeCount++;
        else if (trans.getStatus() == "Returned") returnedCount++;
        else if (trans.getStatus() == "Returned-Late") lateCount++;
    }

    std::cout << "--------------------------------------------"
              << "--------------------------------------------\n";
    std::cout << "\n";
    std::cout << "Total Transactions: " << transactions.size() << "\n";
    std::cout << "Active: " << activeCount
              << " | Returned: " << returnedCount
              << " | Late: " << lateCount << "\n";
    std::cout << "============================================\n";
}

/**
 * generateReport - Generate a summary report of library activity
 *
 * Report includes:
 * - Total books in system (total titles, total copies, available copies)
 * - Active checkouts
 * - Overdue books
 * - Total fines collected
 */
void Librarian::generateReport() const {
    // Get data from Library singleton
    auto& books = Library::instance().getAllBooks();
    auto& transactions = Library::instance().getTransactions();
    auto& fines = Library::instance().getFines();

    // Calculate book statistics
    int totalTitles = static_cast<int>(books.size());
    int totalCopies = 0;
    int availableCopies = 0;

    for (const auto& book : books) {
        totalCopies += book.getTotalCopies();
        availableCopies += book.getAvailableCopies();
    }

    int checkedOutCopies = totalCopies - availableCopies;

    // Calculate transaction statistics
    int totalTransactions = static_cast<int>(transactions.size());
    int activeCount = 0;
    int returnedCount = 0;
    int lateCount = 0;

    std::string today = getCurrentDate();

    for (const auto& trans : transactions) {
        if (trans.getStatus() == "Active") {
            activeCount++;
        } else if (trans.getStatus() == "Returned") {
            returnedCount++;
        } else if (trans.getStatus() == "Returned-Late") {
            lateCount++;
        }
    }

    // Calculate fine statistics
    double totalFinesAmount = 0.0;
    for (const auto& fine : fines) {
        totalFinesAmount += fine.getAmount();
    }

    // Display report
    std::cout << "\n";
    std::cout << "============================================\n";
    std::cout << "           LIBRARY REPORT                   \n";
    std::cout << "============================================\n";
    std::cout << "\n";

    std::cout << "INVENTORY SUMMARY\n";
    std::cout << "  Total Book Titles: " << totalTitles << "\n";
    std::cout << "  Total Copies: " << totalCopies << "\n";
    std::cout << "  Available Copies: " << availableCopies << "\n";
    std::cout << "  Checked Out: " << checkedOutCopies << "\n";
    std::cout << "\n";

    std::cout << "TRANSACTION SUMMARY\n";
    std::cout << "  Total Transactions: " << totalTransactions << "\n";
    std::cout << "  Active Rentals: " << activeCount << "\n";
    std::cout << "  Completed Returns: " << returnedCount << "\n";
    std::cout << "  Late Returns: " << lateCount << "\n";
    std::cout << "\n";

    std::cout << "FINANCIAL SUMMARY\n";
    std::cout << "  Total Fines Collected: $" << std::fixed << std::setprecision(2)
              << totalFinesAmount << "\n";
    std::cout << "  Number of Fines: " << fines.size() << "\n";
    std::cout << "\n";
    std::cout << "============================================\n";
}
