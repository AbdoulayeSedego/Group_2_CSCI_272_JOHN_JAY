
         #include "Member.h"
#include "Library.h"
#include <iostream>
#include <limits>

// Constructor: initialize Member with basic user info and associated Library
Member::Member(int id, const std::string& name, const std::string& email,
               const std::string& membershipDate, Library* lib)
    : User(id, name, email, "Member", membershipDate),
      lib(lib)
{}

// Display basic information about the member
void Member::display() const {
    std::cout << "**** MEMBER ****\n";
    std::cout << "ID: " << userID << " | Name: " << name << "\n";
}

// Member menu: allows searching books, viewing inventory, or exiting
void Member::menu() {
    while (true) {
        std::cout << "\n=== Member Menu ===\n"
             << "1) Search Books\n2) View Inventory\n3) Exit\n"
             << "Choose: ";

        int opt;
        // Input validation: if invalid, clear and ignore input buffer
        if (!(std::cin >> opt)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (opt == 1) {
            // Search books by keyword in title, author, or ISBN
            std::cout << "Enter keyword (title/author/ISBN): ";
            std::string kw;
            std::getline(std::cin >> std::ws, kw); // ws consumes any leading whitespace

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
            // Exit the member menu
            return;

        } else {
            std::cout << "Invalid option\n"; // Handle invalid input
        }
    }
}

// Emma Das
