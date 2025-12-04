/**
 * Admin.h & Admin.cpp
 *
 * Purpose: 'Admin' is a subclass (child class) of 'User', when declared, this
 *          user has extended permissions that are not available to other users.
 *          This user can: add books, remove books, manage inventory, etc...
 *
 **/

#include "User.h"
#include "Library.h"

// Admin class:
// - Has full access to Library operations
// - Can add/remove books
// - Can manage inventory
class Admin : public User {
public:
    // Constructors
    Admin();
    Admin(int userID,
          const std::string& name,
          const std::string& email);

    // Display admin info
    void display() const override;

    // Main admin menu loop
    void menu() override;

private:
    // Admin actions
    void addBook();
    void removeBook();
    void manageInventory();
};

