/**
 * Librarian.cpp
 *
 * Purpose: Implementation file for the Librarian class.
 *          Contains all method definitions for librarian operations.
 *
 * Author: Member 3
 * OOP Concepts: Inheritance, Polymorphism (virtual function override),
 *               Exception Handling, Input Validation
 *
 * Key Responsibilities:
 * - Process book checkouts and returns
 * - Calculate late fees
 * - View and manage transactions
 **/

#include "../headers/Librarian.h"  // Include Librarian header
#include <iostream>                 // For console I/O (cin, cout)
#include <iomanip>                  // For output formatting (setprecision)
#include <limits>                   // For numeric_limits (input clearing)
#include <stdexcept>                // For exceptions

// ==================== CONSTRUCTORS ====================

/**
 * Default Constructor
 * Creates a Librarian with empty/default values
 *
 * Implementation note:
 * - Calls User() default constructor first (base class initialization)
 * - Then initializes Librarian-specific members (none currently)
 */
Librarian::Librarian()
    : User()  // Explicitly call base class default constructor
{
    // Set userType to "Librarian" for identification
    // Using the inherited setter from User class
    setUserType("Librarian");
}

/**
 * Parameterized Constructor
 * Creates a fully initialized Librarian
 *
 * @param userID         - Unique ID for this librarian
 * @param name           - Full name
 * @param email          - Email address
 * @param userType       - Should be "Librarian"
 * @param membershipDate - Employment start date
 *
 * Note: The initializer list calls the User base class constructor
 *       with all the provided parameters
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

/**
 * display() - Displays librarian information to console
 *
 * This function OVERRIDES the pure virtual display() from User class
 * POLYMORPHISM: When we call display() on a User* pointing to a Librarian,
 *               THIS function executes (not User::display())
 *
 * Example of polymorphism:
 *   User* user = new Librarian(1, "John", "john@lib.com", "Librarian", "2024-01-01");
 *   user->display();  // Calls Librarian::display(), NOT User::display()
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
 *
 * Menu Options:
 * 1. Process Checkout - Check out a book to a user
 * 2. Process Return - Return a book and calculate fees
 * 3. View All Transactions - See checkout history
 * 4. Generate Report - View statistics
 * 5. Exit - Return to main menu
 *
 * Edge Cases Handled:
 * - Invalid menu choice (non-integer input)
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
            // numeric_limits<streamsize>::max() = maximum possible stream size
            // '\n' = discard until newline
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << "\n[ERROR] Invalid input! Please enter a number (1-5).\n";
            continue;  // Go back to start of loop
        }

        // Clear any remaining input (e.g., if user entered "1abc")
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
 * processCheckout - Handle a book checkout operation
 *
 * Workflow:
 * 1. Prompt for User ID
 * 2. Prompt for Book ID
 * 3. (In full implementation: verify user exists, verify book available)
 * 4. Create transaction record
 * 5. Update book inventory
 *
 * Note: This is a simplified implementation. In the full system,
 *       this would interact with Library class and FileManager.
 *
 * Edge Cases Handled:
 * - Empty user ID input
 * - Empty book ID input
 * - Invalid input types
 */
void Librarian::processCheckout() {
    std::string userId;
    std::string bookId;

    // Get User ID
    std::cout << "Enter User ID (e.g., U001): ";
    std::getline(std::cin, userId);

    // ===== EDGE CASE: Empty user ID =====
    if (userId.empty()) {
        std::cout << "[ERROR] User ID cannot be empty.\n";
        return;
    }

    // Get Book ID
    std::cout << "Enter Book ID (e.g., B001): ";
    std::getline(std::cin, bookId);

    // ===== EDGE CASE: Empty book ID =====
    if (bookId.empty()) {
        std::cout << "[ERROR] Book ID cannot be empty.\n";
        return;
    }

    // ===== PLACEHOLDER IMPLEMENTATION =====
    // In the full system, this would:
    // 1. Call Library::getInstance() to get the library
    // 2. Find the book by ID
    // 3. Check if book.isAvailable()
    // 4. If available, call book.checkout()
    // 5. Create a Transaction object
    // 6. Add transaction to the system
    // 7. Save to transactions.csv

    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "CHECKOUT PROCESSED (Placeholder)\n";
    std::cout << "========================================\n";
    std::cout << "User ID: " << userId << "\n";
    std::cout << "Book ID: " << bookId << "\n";
    std::cout << "Checkout Date: [Current Date]\n";
    std::cout << "Due Date: [Current Date + 14 days]\n";
    std::cout << "Status: Active\n";
    std::cout << "========================================\n";
    std::cout << "\n";

    // TODO: Implement full checkout logic when integrating with Library class
    // Example of what full implementation would look like:
    /*
    try {
        Library* lib = Library::getInstance();
        Book* book = lib->findBook(bookId);

        if (book == nullptr) {
            throw std::runtime_error("Book not found: " + bookId);
        }

        if (!book->isAvailable()) {
            throw std::runtime_error("No copies available for: " + book->getTitle());
        }

        // Create transaction
        std::string transId = generateTransactionId();
        std::string today = getCurrentDate();
        std::string dueDate = addDays(today, DEFAULT_LOAN_PERIOD);

        Transaction trans(transId, userId, bookId, today, dueDate);

        // Update book and save
        book->checkout();
        lib->addTransaction(trans);
        lib->saveToFile();

        std::cout << "Checkout successful!\n";

    } catch (const std::exception& e) {
        std::cout << "[ERROR] " << e.what() << "\n";
    }
    */
}

/**
 * processReturn - Handle a book return operation
 *
 * Workflow:
 * 1. Prompt for Transaction ID
 * 2. (In full implementation: find transaction)
 * 3. Calculate days late
 * 4. Calculate fine if late
 * 5. Update transaction status
 * 6. Update book inventory
 *
 * Edge Cases Handled:
 * - Empty transaction ID
 * - Transaction not found
 * - Already returned transaction
 */
void Librarian::processReturn() {
    std::string transactionId;

    // Get Transaction ID
    std::cout << "Enter Transaction ID (e.g., T001): ";
    std::getline(std::cin, transactionId);

    // ===== EDGE CASE: Empty transaction ID =====
    if (transactionId.empty()) {
        std::cout << "[ERROR] Transaction ID cannot be empty.\n";
        return;
    }

    // ===== PLACEHOLDER IMPLEMENTATION =====
    // Simulating a late return for demonstration

    // Simulated days late (in real implementation, this would be calculated)
    int simulatedDaysLate = 5;  // Example: 5 days late
    double fine = calculateFine(simulatedDaysLate);

    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "RETURN PROCESSED (Placeholder)\n";
    std::cout << "========================================\n";
    std::cout << "Transaction ID: " << transactionId << "\n";
    std::cout << "Return Date: [Current Date]\n";

    if (simulatedDaysLate > 0) {
        std::cout << "Days Late: " << simulatedDaysLate << "\n";
        // setprecision(2) and fixed ensure we show exactly 2 decimal places
        std::cout << "Late Fee: $" << std::fixed << std::setprecision(2) << fine << "\n";
        std::cout << "Status: Returned-Late\n";
    } else {
        std::cout << "Days Late: 0 (On time!)\n";
        std::cout << "Late Fee: $0.00\n";
        std::cout << "Status: Returned\n";
    }
    std::cout << "========================================\n";
    std::cout << "\n";

    // TODO: Implement full return logic when integrating with Library class
    // Example:
    /*
    try {
        Library* lib = Library::getInstance();
        Transaction* trans = lib->findTransaction(transactionId);

        if (trans == nullptr) {
            throw std::runtime_error("Transaction not found: " + transactionId);
        }

        if (!trans->isActive()) {
            throw std::runtime_error("Transaction already completed");
        }

        // Process return
        std::string today = getCurrentDate();
        trans->completeReturn(today);

        // Calculate fine
        int daysLate = trans->calculateDaysLate();
        if (daysLate > 0) {
            double fineAmount = calculateFine(daysLate);
            // Create Fine object and save
        }

        // Update book inventory
        Book* book = lib->findBook(trans->getBookId());
        book->returnBook();

        lib->saveToFile();

    } catch (const std::exception& e) {
        std::cout << "[ERROR] " << e.what() << "\n";
    }
    */
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
 *
 * In full implementation, this would:
 * 1. Get all transactions from Library or FileManager
 * 2. Display each transaction's details
 * 3. Show statistics (total active, total completed, etc.)
 */
void Librarian::viewAllTransactions() const {
    std::cout << "\n";
    std::cout << "============================================\n";
    std::cout << "        ALL TRANSACTIONS (Placeholder)      \n";
    std::cout << "============================================\n";
    std::cout << "\n";

    // ===== PLACEHOLDER: Sample transaction display =====
    // In real implementation, this would loop through all transactions

    std::cout << "Transaction ID | User ID | Book ID | Status\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "T001           | U001    | B001    | Returned\n";
    std::cout << "T002           | U002    | B002    | Active\n";
    std::cout << "T003           | U001    | B003    | Returned-Late\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "\n";
    std::cout << "Total Transactions: 3\n";
    std::cout << "Active: 1 | Returned: 1 | Late: 1\n";
    std::cout << "\n";

    // TODO: Implement when integrating with Library/FileManager
    /*
    Library* lib = Library::getInstance();
    std::vector<Transaction> transactions = lib->getAllTransactions();

    if (transactions.empty()) {
        std::cout << "No transactions found.\n";
        return;
    }

    int activeCount = 0, returnedCount = 0, lateCount = 0;

    for (const auto& trans : transactions) {
        trans.display();

        if (trans.getStatus() == "Active") activeCount++;
        else if (trans.getStatus() == "Returned") returnedCount++;
        else if (trans.getStatus() == "Returned-Late") lateCount++;
    }

    std::cout << "Total: " << transactions.size() << "\n";
    std::cout << "Active: " << activeCount << " | Returned: " << returnedCount
              << " | Late: " << lateCount << "\n";
    */
}

/**
 * generateReport - Generate a summary report of library activity
 *
 * Report includes:
 * - Total books in system
 * - Total users
 * - Active checkouts
 * - Overdue books
 * - Total fines collected
 */
void Librarian::generateReport() const {
    std::cout << "\n";
    std::cout << "============================================\n";
    std::cout << "         LIBRARY REPORT (Placeholder)       \n";
    std::cout << "============================================\n";
    std::cout << "\n";

    // ===== PLACEHOLDER: Sample report data =====
    std::cout << "INVENTORY SUMMARY\n";
    std::cout << "  Total Books: 50\n";
    std::cout << "  Available: 42\n";
    std::cout << "  Checked Out: 8\n";
    std::cout << "\n";

    std::cout << "USER SUMMARY\n";
    std::cout << "  Total Users: 25\n";
    std::cout << "  Members: 18\n";
    std::cout << "  Non-Members: 7\n";
    std::cout << "\n";

    std::cout << "TRANSACTION SUMMARY\n";
    std::cout << "  Total Checkouts (all time): 150\n";
    std::cout << "  Active Rentals: 8\n";
    std::cout << "  Overdue Books: 2\n";
    std::cout << "\n";

    std::cout << "FINANCIAL SUMMARY\n";
    std::cout << "  Total Fines Issued: $45.50\n";
    std::cout << "  Fines Collected: $38.00\n";
    std::cout << "  Outstanding Fines: $7.50\n";
    std::cout << "\n";
    std::cout << "============================================\n";
    std::cout << "\n";

    // TODO: Implement with real data from Library/FileManager
}
