#ifndef MEMBER_H
#define MEMBER_H

#include "User.h"

// Forward declaration
class Library;

class Member : public User {
private:
    Library* lib = nullptr; // Currently unused but kept for compatibility

public:
    // Default constructor
    Member() = default;

    // Parameterized constructor
    Member(int id, const std::string& name, const std::string& email,
           const std::string& membershipDate, Library* lib);

    // Display member info
    void display() const override;

    // Main menu for the member
    void menu() override;

    // New features:
    void borrowBook();
    void returnBook();
    void viewMyBorrowedBooks() const;
};

#endif //MEMBER_H
