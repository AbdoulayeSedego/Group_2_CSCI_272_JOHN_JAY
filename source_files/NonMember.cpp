#include "NonMember.h"
#include "Library.h"
#include <iostream>
#include <limits>

// Constructor: initialize NonMember with basic user info and associated Library
NonMember::NonMember(int id, const std::string& name, const std::string& email,
                     const std::string& membershipDate, Library* lib)
    : User(id, name, email, "NonMember", membershipDate),
      lib(lib)
{}

// Display basic information about the non-member
void NonMember::display() const {
    std::cout << "**** NON-MEMBER ****\n";
    std::cout << "ID: " << userID << " | Name: " << name << "\n";
}

// NonMember menu: allows searching books, viewing inventory, renting a book, or exiting
void NonMember::menu() {
    while (true) {
        std::cout << "\n=== NonMember Menu ($3 per rental) ===\n"
             << "1) Search Books\n2) View Inventory\n3) Rent a Book (charge $3)\n4) Exit\n"
             << "Choose: ";

        int opt;
        // Input validation: clear buffer if input is invalid
        if (!(std::cin >> opt)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (opt == 1) {
            // Search books by keyword in title, author, or ISBN
            std::cout << "Enter keyword (title/author/ISBN): ";
            std::string kw;
            std::getline(std::cin >> std::ws, kw); // ws consumes leading whitespace

            // Use getter methods to access private Book members (encapsulation)
            auto results = Library::instance().searchBooks([&](const Book& b) {
                return b.getTitle().find(kw) != std::string::npos
                       || b.getAuthor().find(kw) != std::string::npos
                       || b.getIsbn().find(kw) != std::string::npos;
            });

            if (results.empty())
                std::cout << "No matches found.\n";
            else
                // Access Book data through public getters
                for (auto p : results)
                    std::cout << p->getBookId() << ": " << p->getTitle() << " | " << p->getAuthor()
                         << " | avail: " << p->getAvailableCopies() << "\n";

        } else if (opt == 2) {
            // View all books in the library using public getters
            for (const auto& b : Library::instance().getAllBooks()) {
                std::cout << b.getBookId() << ": " << b.getTitle() << " | " << b.getAuthor()
                     << " | available: " << b.getAvailableCopies() << "\n";
            }

        } else if (opt == 3) {
            // Rent a book 
            try {
                int bookId = 0;
                std::string checkoutDate, dueDate;

                std::cout << "Enter book ID: ";
                if (!(std::cin >> bookId)) throw std::runtime_error("Invalid book ID");

                std::cout << "Enter checkout date (YYYY-MM-DD): ";
                std::cin >> checkoutDate;

                std::cout << "Enter due date (YYYY-MM-DD): ";
                std::cin >> dueDate;

                std::cout << "Charging $3.00 (simulated)\n";

                // Perform checkout in library
                int transId = Library::instance().checkoutBook(userID, bookId, checkoutDate, dueDate);
                std::cout << "Transaction created: " << transId << "\n";

            } catch (const std::exception& ex) {
                std::cout << "Error: " << ex.what() << "\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }

        } else if (opt == 4) {
            // Exit the menu
            return;

        } else {
            std::cout << "Invalid option\n"; // Handle invalid input
        }
    }
}

// Emma Das
