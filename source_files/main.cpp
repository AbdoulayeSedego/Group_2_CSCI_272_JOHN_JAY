// Include header files (use -I headers flag when compiling)
#include "Library.h"
#include "Admin.h"
#include "Librarian.h"
#include "Member.h"
#include "NonMember.h"
#include <iostream>

using namespace std;

int main() {
    // Load persisted data (books and transactions) from CSV
    Library::instance().loadFromCSV();

    // Create example users for demonstration purposes
    // Constructor params: (userID, name, email, userType, membershipDate)
    Admin admin(1, "Alice Admin", "alice@lib.org", "Admin", "2023-01-01");
    Librarian librarian(2, "Bob Librarian", "bob@lib.org", "Librarian", "2023-02-01");
    Member member(3, "Carol Member", "carol@lib.org", "2023-03-01", &Library::instance());
    NonMember guest(4, "Dave Guest", "dave@guest.org", "2023-04-01", &Library::instance());

    // Seed demo books 
    if (Library::instance().getAllBooks().empty()) {
        Library::instance().addBook("The C++ Programming Language", "Bjarne Stroustrup", "9780321563842", 3);
        Library::instance().addBook("Clean Code", "Robert C. Martin", "9780132350884", 2);
        Library::instance().addBook("Design Patterns", "Gamma et al.", "9780201633610", 1);
    }

    // Top-level menu loop
    while (true) {
        cout << "\n=== Library System ===\n"
             << "1) Admin\n2) Librarian\n3) Member\n4) NonMember\n5) Exit\n"
             << "Choose: ";

        int opt;
        // Validate input; if invalid, clear input buffer and continue
        if (!(cin >> opt)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        // Handle user choice
        switch (opt) {
            case 1: admin.menu(); break;       // Admin menu
            case 2: librarian.menu(); break;   // Librarian menu
            case 3: member.menu(); break;      // Member menu
            case 4: guest.menu(); break;       // Non-member menu
            case 5:                           // Exit option
                Library::instance().saveToCSV(); // Save current data to CSV
                cout << "Goodbye\n";
                return 0;
            default:
                cout << "Invalid option\n";  // Handle invalid input
        }
    }
}
// Emma Das
