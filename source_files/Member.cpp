#include "Member.h"
#include "Library.h"
#include <iostream>
#include <limits>

// Constructor
Member::Member(int id, const std::string& name, const std::string& email,
               const std::string& membershipDate, Library* lib)
    : User(id, name, email, "Member", membershipDate),
      lib(lib)
{}

// Display basic information
void Member::display() const {
    std::cout << "**** MEMBER ****\n";
    std::cout << "ID: " << userID << " | Name: " << name << "\n";
}

// -------------------------------
// Borrow a Book
// -------------------------------
void Member::borrowBook() {
    int bookId;
    std::cout << "Enter Book ID to borrow: ";

    if (!(std::cin >> bookId)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input.\n";
        return;
    }

    std::string checkoutDate, dueDate;
    std::cout << "Enter checkout date (YYYY-MM-DD): ";
    std::cin >> checkoutDate;
    std::cout << "Enter due date (YYYY-MM-DD): ";
    std::cin >> dueDate;

    try {
        int transactionId = Library::instance().checkoutBook(
            userID, bookId, checkoutDate, dueDate
        );
        std::cout << "Book borrowed successfully! Transaction ID: "
                  << transactionId << "\n";
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}

// -------------------------------
// Return a Book
// -------------------------------
void Member::returnBook() {
    int transId;
    std::cout << "Enter Transaction ID to return book: ";

    if (!(std::cin >> transId)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input.\n";
        return;
    }

    std::string returnDate;
    std::cout << "Enter return date (YYYY-MM-DD): ";
    std::cin >> returnDate;

    try {
        Fine fine = Library::instance().processReturn(transId, returnDate);
        std::cout << "Book returned successfully.\n";
        std::cout << "Fine amount: $" << fine.getAmount() << "\n";
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}

// -------------------------------
// View All Borrowed Books
// -------------------------------
void Member::viewMyBorrowedBooks() const {
    auto& trans = Library::instance().getTransactions();
    bool found = false;

    std::cout << "\n=== My Borrowed Books ===\n";

    for (const auto& t : trans) {
        if (t.getUserId() == userID && t.isActive()) {
            found = true;

            Book* b = Library::instance().findBookById(t.getBookId());
            if (b) {
                std::cout << "Transaction ID: " << t.getTransactionId()
                          << " | Book: " << b->getTitle()
                          << " | Due: " << t.getDueDate() << "\n";
            }
        }
    }

    if (!found)
        std::cout << "You have no borrowed books.\n";
}

// -------------------------------
// Member Menu
// -------------------------------
void Member::menu() {
    while (true) {
        std::cout << "\n=== Member Menu ===\n"
                  << "1) Search Books\n"
                  << "2) View Inventory\n"
                  << "3) Borrow Book\n"
                  << "4) Return Book\n"
                  << "5) View My Borrowed Books\n"
                  << "6) Exit\n"
                  << "Choose: ";

        int opt;
        if (!(std::cin >> opt)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (opt) {
            case 1: {
                std::cout << "Enter keyword (title/author/ISBN): ";
                std::string kw;
                std::getline(std::cin >> std::ws, kw);

                auto results = Library::instance().searchBooks([&](const Book& b) {
                    return b.getTitle().find(kw) != std::string::npos
                        || b.getAuthor().find(kw) != std::string::npos
                        || b.getIsbn().find(kw) != std::string::npos;
                });

                if (results.empty())
                    std::cout << "No matches found.\n";
                else
                    for (auto p : results)
                        std::cout << p->getBookId() << ": "
                                  << p->getTitle() << " | "
                                  << p->getAuthor()
                                  << " | avail: "
                                  << p->getAvailableCopies()
                                  << "\n";
                break;
            }

            case 2:
                for (const auto& b : Library::instance().getAllBooks()) {
                    std::cout << b.getBookId() << ": "
                              << b.getTitle() << " | "
                              << b.getAuthor()
                              << " | available: "
                              << b.getAvailableCopies() << "\n";
                }
                break;

            case 3:
                borrowBook();
                break;

            case 4:
                returnBook();
                break;

            case 5:
                viewMyBorrowedBooks();
                break;

            case 6:
                return;

            default:
                std::cout << "Invalid option\n";
                break;
        }
    }
}
