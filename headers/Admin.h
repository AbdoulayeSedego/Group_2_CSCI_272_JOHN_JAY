/**
 * Admin.h & Admin.cpp
 *
 * Purpose: 'Admin' is a subclass (child class) of 'User', when declared, this
 *          user has extended permissions that are not available to other users.
 *          This user can: add books, remove books, manage inventory, etc...
 *
 **/

#include "User.h"
// #include "Book.h"
// #include "Library.h"

class Admin : public User {
public:
    // Constructors (Default & Parametrized)
    Admin();

    Admin(int userID, const std::string &name, const std::string &email, const std::string &userType,
          const std::string &membershipDate);

    // Overridden Functions
    // These have been declared in User.h and must be overridden
    void display() const override;
    void menu() override;

    // Declare functions specific to this user
    void addBook();
    void removeBook();
    void manageInventory();
};
