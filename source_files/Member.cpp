#include "Member.h"
#include "Library.h"
#include <iostream>
#include <limits>

using namespace std;

// Constructor: initialize Member with basic user info and associated Library
Member::Member(int id, const string& name, const string& email,
               const string& membershipDate, Library* lib)
    : User(id, name, email, "Member", membershipDate),
      lib(lib)
{}

// Display basic information about the member
void Member::display() const {
    cout << "**** MEMBER ****\n";
    cout << "ID: " << userID << " | Name: " << name << "\n";
}

// Member menu: allows searching books, viewing inventory, or exiting
void Member::menu() {
    while (true) {
        cout << "\n=== Member Menu ===\n"
             << "1) Search Books\n2) View Inventory\n3) Exit\n"
             << "Choose: ";

        int opt;
        // Input validation: if invalid, clear and ignore input buffer
        if (!(cin >> opt)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (opt == 1) {
            // Search books by keyword in title, author, or ISBN
            cout << "Enter keyword (title/author/ISBN): ";
            string kw;
            getline(cin >> ws, kw); // ws consumes any leading whitespace

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
            // Exit the member menu
            return;

        } else {
            cout << "Invalid option\n"; // Handle invalid input
        }
    }
}

// Emma Das