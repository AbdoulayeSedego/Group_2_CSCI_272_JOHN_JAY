/**
 Transaction.h
 The 'Transaction' class represents a rental/checkout record in the library.
 It tracks when a user checks out a book, when it's due, and when it's returned.
 This class is essential for calculating late fees and maintaining rental history.
 Author: Abdoulaye Sedego

 **/

#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <stdexcept>

class Transaction {
private:
    std::string transactionId;   
    std::string userId;          
    std::string bookId;          
    std::string checkoutDate; 
    std::string dueDate;  
    std::string returnDate;   
    std::string status;  

    /**
     * parseDateToComponents - Breaks a date string into year, month, day
     * @param dateStr  - Date string
     * @return true if parsing successful, false otherwise
     */
    static bool parseDateToComponents(const std::string& dateStr,
                                      int& year, int& month, int& day);
    static int dateToDays(int year, int month, int day);

public:
    // ==================== CONSTRUCTORS ====================
     //Default Constructor
    Transaction();
    /**
     * Parameterized Constructor - For new checkouts
     * Creates a transaction when a user checks out a book
     * @param tid      - Transaction ID
     * @param uid      - User ID who is borrowing
     * @param bid      - Book ID being borrowed
     * @param checkout - Checkout date
     * @param due      - Due date
     */
    Transaction(std::string tid, std::string uid, std::string bid,
                std::string checkout, std::string due);

    /**
     * Full Constructor, For loading from file
     * Creates a complete transaction including return info
     * Used when loading existing transactions from CSV
     *
     * @param tid      - Transaction ID
     * @param uid      - User ID
     * @param bid      - Book ID
     * @param checkout - Checkout date
     * @param due      - Due date
     * @param returned - Return date
     * @param stat     - Status ("Active", "Returned", "Returned-Late")
     */
    Transaction(std::string tid, std::string uid, std::string bid,
                std::string checkout, std::string due,
                std::string returned, std::string stat);

    // ==================== GETTERS ====================

    std::string getTransactionId() const { return transactionId; }
    std::string getUserId() const { return userId; }
    std::string getBookId() const { return bookId; }
    std::string getCheckoutDate() const { return checkoutDate; }
    std::string getDueDate() const { return dueDate; }
    std::string getReturnDate() const { return returnDate; }
    std::string getStatus() const { return status; }

    // ==================== SETTERS ====================

    void setTransactionId(const std::string& tid);
    void setUserId(const std::string& uid);
    void setBookId(const std::string& bid);
    void setCheckoutDate(const std::string& date);
    void setDueDate(const std::string& date);
    void setReturnDate(const std::string& date);
    void setStatus(const std::string& s);

    // ==================== CORE FUNCTIONALITY ====================

    /*
     * calculateDaysLate - Calculates how many days overdue the return is
     * Edge Cases Handled:
     * 1. Return date empty -> returns 0 (still active, no late fee yet)
     * 2. Return date <= due date -> returns 0 (on time)
     * 3. Invalid date format -> returns 0 with warning
     * return Number of days late (0 if on time or still active)
     */
    int calculateDaysLate() const;

    /*
     * isActive - Checks if the transaction is still active (book not returned)
     * return true if status is "Active", false otherwise
     */
    bool isActive() const { return status == "Active"; }

    /*
     * isLate - Checks if the book was returned late
     * return true if status is "Returned-Late", false otherwise
     */
    bool isLate() const { return status == "Returned-Late"; }

    /**
     * completeReturn - Marks the transaction as complete
     * Updates the return date and status based on whether it's late
     *
     * @param returnDateStr - The date the book was returned
     
    Edge Cases:
      -Empty return date -> throws invalid_argument
     -Transaction already completed -> throws runtime_error
     */
    void completeReturn(const std::string& returnDateStr);

    /*
     display - Prints transaction information to console
     Formats output
     */
    void display() const;

    //toCSV - Converts transaction to CSV format for file storage

    std::string toCSV() const;
};
#endif
