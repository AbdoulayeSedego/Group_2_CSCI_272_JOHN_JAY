/**
 *Librarian.h
 *The 'Librarian' class is a derived class of 'User'.
 *          Librarians are staff members who handle daily library operations:
 *          - Processing book checkouts
 *          - Processing book returns
 *          - Calculating late fees/fines
 *          - Viewing transaction history
 * Author: Abdoulaye Sedego
 **/

#ifndef LIBRARIAN_H
#define LIBRARIAN_H

#include "User.h"
#include "Book.h"
#include "Transaction.h"
#include <vector>
#include <string>

class Librarian : public User
{

private:
    // FINE_PER_DAY - The late fee charged per day overdue $0.50 per day

    static constexpr double FINE_PER_DAY = 0.50;

    static constexpr int DEFAULT_LOAN_PERIOD = 14;

public:
    // ==================== CONSTRUCTORS ====================

    // Default Constructor
    // Creates a Librarian with default/empty values

    Librarian();

    /**
     * Parameterized Constructor
     * Creates a fully initialized Librarian
     *
     * @param userID         - Unique identifier for this librarian
     * @param name           - Full name of the librarian
     * @param email          - Email address
     * @param userType       - Should be "Librarian"
     * @param membershipDate - Date the librarian joined the library staff
     */
    Librarian(int userID, const std::string &name, const std::string &email,
              const std::string &userType, const std::string &membershipDate);

    // display() - Displays librarian information
    // Shows librarian-specific information

    void display() const override;

    /**
     * menu() - Displays and handles the librarian's menu options
     * Overrides User::menu()
     * Provides options for:
     * - Processing checkouts
     * - Processing returns
     * - Viewing all transactions
     * - Generating reports
     */
    void menu() override;

    // ==================== LIBRARIAN-SPECIFIC FUNCTIONS =========
    /**
     * processCheckout - Handles a book checkout operation
     * 1. Get user ID and book ID from input
     * 2. Verify book is available
     * 3. Create new transaction record
     * 4. Update book's available copies
     *
     * Edge Cases Handled:
     * - Book not found
     * - No copies available
     * - User not found
     * - Invalid input
     */
    void processCheckout();

    /**
     * processReturn - Handles a book return operation
     * 1. Get transaction ID from input
     * 2. Find the transaction
     * 3. Mark as returned with current date
     * 4. Calculate late fee if applicable
     * 5. Update book's available copies
     *
     * Edge Cases Handled:
     * - Transaction not found
     * - Already returned
     * - Invalid input
     */
    void processReturn();

    /**
     * calculateFine - Calculates late fee for overdue books
     * Formula: daysLate * FINE_PER_DAY ($0.50)
     * Edge Cases:
     * - Negative days (shouldn't happen) -> returns 0
     * - Zero days -> returns 0
     * Examples:
     *   calculateFine(0) = $0.00
     *   calculateFine(5) = $2.50
     *   calculateFine(18) = $9.00
     */
    double calculateFine(int daysLate) const;

    /**
     * viewAllTransactions - Displays all transaction records
     * Shows both active and completed transactions
     */
    void viewAllTransactions() const;

    /**
     * generateReport - Generates a summary report
     * Shows statistics like:
     * - Total checkouts
     * - Active rentals
     * - Overdue books
     * - Total fines collected
     */
    void generateReport() const;

    // ==================== DESTRUCTOR ====================
    ~Librarian() override = default;
};

#endif
