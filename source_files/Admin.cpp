#include "Admin.h"
#include <iostream>

// Constructors
Admin::Admin() : User() {}

Admin::Admin(int userID, const std::string& name,
             const std::string& email, const std::string& userType,
             const std::string& membershipDate)
    : User(userID, name, email, userType, membershipDate) {}

// Display Admin info
void Admin::display() const {
    std::cout << "**** ADMIN USER ****\n";
    std::cout << "User ID: " << userID << "\n";
    std::cout << "Name: " << name << "\n";
    std::cout << "Email: " << email << "\n";
}

// Admin menu options
void Admin::menu() {
    while (true) {
        std::cout << "\n=== Admin Menu ===\n";
        std::cout << "1. Add Book\n";
        std::cout << "2. Remove Book\n";
        std::cout << "3. Manage Inventory\n";
        std::cout << "4. Exit\n";

        int option;
        std::cin >> option;

        switch (option) {
            case 1:
                addBook();
                break;
            case 2:
                removeBook();
                break;
            case 3:
                manageInventory();
                break;
            case 4:
                return; // exit the menu
            default:
                std::cout << "Invalid option.\n";
        }
    }
}

// Placeholder methods
void Admin::addBook() {
    std::cout << "Placeholder!\n";
}

void Admin::removeBook() {
    std::cout << "Placeholder!\n";
}

void Admin::manageInventory() {
    std::cout << "Placeholder!\n";
}
