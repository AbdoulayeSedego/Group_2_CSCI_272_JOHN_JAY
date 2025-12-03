#include "NonMember.h"
#include "Library.h"
#include <iostream>
#include <limits>

using namespace std;

// Constructor: initialize NonMember with basic user info and associated Library
NonMember::NonMember(int id, const string& name, const string& email,
                     const string& membershipDate, Library* lib)
    : User(id, name, email, "NonMember", membershipDate),
      lib(lib)
{}

// Display basic information about the non-member
void NonMember::display() const {
    cout << "**** NON-MEMBER ****\n";
    cout << "ID: " << userID << " | Name: " << name << "\n";
}

// NonMember menu: allows searching books, viewing inventory, renting a book, or exiting
void NonMember::menu() {
    while (true) {
        cout << "\n=== NonMember Menu ($3 per rental) ===\n"
             << "1) Search Books\n2) View Inventory\n3) Rent a Book (charge $3)\n4) Exit\n"
             << "Choose: ";

        int opt;
        // Input validation: clear buffer if input is invalid
        if (!(cin >> opt)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (opt == 1) {
            // Search books by keyword in title, author, or ISBN
            cout << "Enter keyword (title/author/ISBN): ";
            string kw;
            getline(cin >> ws, kw); // ws consumes leading whitespace

            auto results = Library::instance().searchBooks([&](const Book& b) {
                return b.title.find(kw) != string::npos
                       || b.author.find(kw) != string::npos
                       || b.isbn.find(kw) != string::npos;
            });

            if (results.empty())
                cout << "No matches found.\n";
            else
                for (auto p : results)
                    cout << p->bookId << ": " << p->title << " | " << p->author
                         << " | avail: " << p->availableCopies << "\n";

        } else if (opt == 2) {
            // View all books in the library
            for (const auto& b : Library::instance().getAllBooks()) {
                cout << b.bookId << ": " << b.title << " | " << b.author
                     << " | available: " << b.availableCopies << "\n";
            }

        } else if (opt == 3) {
            // Rent a book (simulated $3 charge)
            try {
                int bookId = 0;
                string checkoutDate, dueDate;

                cout << "Enter book ID: ";
                if (!(cin >> bookId)) throw runtime_error("Invalid book ID");

                cout << "Enter checkout date (YYYY-MM-DD): ";
                cin >> checkoutDate;

                cout << "Enter due date (YYYY-MM-DD): ";
                cin >> dueDate;

                cout << "Charging $3.00 (simulated)\n";

                // Perform checkout in library
                int transId = Library::instance().checkoutBook(userID, bookId, checkoutDate, dueDate);
                cout << "Transaction created: " << transId << "\n";

            } catch (const exception& ex) {
                cout << "Error: " << ex.what() << "\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

        } else if (opt == 4) {
            // Exit the menu
            return;

        } else {
            cout << "Invalid option\n"; // Handle invalid input
        }
    }
}

// Emma Das