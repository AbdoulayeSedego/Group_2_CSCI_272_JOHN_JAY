#include "Admin.h"
#include <iostream>
#include <limits>

// ============================
// Constructors
// ============================

Admin::Admin()
    : User()  // default User constructor
{
    setUserType("Admin");
}

Admin::Admin(int userID,
             const std::string& name,
             const std::string& email)
    : User(userID, name, email, "Admin") // explicitly set type = Admin
{
}

// ============================
// Display admin information
// ============================

void Admin::display() const {
    std::cout << "===== ADMIN USER =====\n";
    std::cout << "ID: " << getUserId() << "\n";
    std::cout << "Name: " << getName() << "\n";
    std::cout << "Email: " << getEmail() << "\n";
}

// ============================
// Admin Menu
// ============================

void Admin::menu() {
    while (true) {
        std::cout << "\n=== ADMIN MENU ===\n";
        std::cout << "1) Add Book\n";
        std::cout << "2) Remove Book\n";
        std::cout << "3) Manage Inventory\n";
        std::cout << "4) Exit\n";
        std::cout << "Choose: ";

        int opt;
        if (!(std::cin >> opt)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (opt) {
            case 1: addBook(); break;
            case 2: removeBook(); break;
            case 3: manageInventory(); break;
            case 4: return;
            default:
                std::cout << "Invalid option.\n";
        }
    }
}

// ============================
// Add Book
// ============================

void Admin::addBook() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string title, author, isbn;
    int copies;

    std::cout << "Enter title: ";
    std::getline(std::cin, title);

    std::cout << "Enter author: ";
    std::getline(std::cin, author);

    std::cout << "Enter ISBN: ";
    std::getline(std::cin, isbn);

    std::cout << "Enter number of copies: ";
    std::cin >> copies;

    try {
        int newId = Library::instance().addBook(title, author, isbn, copies);
        std::cout << "Book added successfully! ID = " << newId << "\n";
    }
    catch (const std::exception& e) {
        std::cout << "Error adding book: " << e.what() << "\n";
    }
}

// ============================
// Remove Book
// ============================

void Admin::removeBook() {
    int id;
    std::cout << "Enter Book ID to remove: ";
    std::cin >> id;

    if (Library::instance().removeBook(id)) {
        std::cout << "Book removed.\n";
    } else {
        std::cout << "Book not found.\n";
    }
}

// ============================
// Manage Inventory
// ============================

void Admin::manageInventory() {
    auto& books = Library::instance().getAllBooks();

    if (books.empty()) {
        std::cout << "No books in library.\n";
        return;
    }

    // Display all books
    std::cout << "\n=== Current Inventory ===\n";
    for (const auto& b : books) {
        b.display();
    }

    // Edit a book’s quantity
    int id;
    std::cout << "\nEnter Book ID to change quantity (0 to cancel): ";
    std::cin >> id;

    if (id == 0) return;

    Book* b = Library::instance().findBookById(id);

    if (!b) {
        std::cout << "Book not found.\n";
        return;
    }

    int newTotal;
    std::cout << "Enter new TOTAL copies for this book: ";
    std::cin >> newTotal;

    try {
        b->setTotalCopies(newTotal);

        // Adjust availableCopies if needed
        if (b->getAvailableCopies() > newTotal) {
            b->setAvailableCopies(newTotal);
        }

        std::cout << "Inventory updated.\n";
    }
    catch (const std::exception& e) {
        std::cout << "Error updating inventory: " << e.what() << "\n";
    }
}
