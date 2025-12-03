#ifndef MEMBER_H
#define MEMBER_H

#include "User.h"
using namespace std;

// Forward declaration of Library class
class Library;

// Member class inherits from User
// Represents a registered library member who can search and view books
class Member : public User {
private:
    Library* lib = nullptr; // Pointer to the Library instance the member interacts with

public:
    // Default constructor
    Member() = default;

    // Parameterized constructor
    // Initializes a Member with id, name, email, membership date, and associated Library
    Member(int id, const string& name, const string& email,
           const string& membershipDate, Library* lib);

    // Display member information (ID and Name)
    void display() const override;

    // Display member menu and handle interactions
    // Options include searching books, viewing inventory, or exiting
    void menu() override;
};

#endif // MEMBER_H

// Emma Das
