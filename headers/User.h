#ifndef USER_H
#define USER_H

/**
 * User.h & User.cpp
 *
 * Purpose: The 'User' class is the base class for Admin, Member, NonMember, and Librarian
 *          Essentially the parent class to the ones mentioned.
 **/

#include <string>

class User {
protected:
    int userID{};
    std::string name; // First and Last name of the user
    std::string email; // email of said user (must be in proper format!)
    std::string userType; // (i.e. 'Admin', 'Member', 'NonMember', 'Librarian')
    std::string membershipDate; // Date the user joined
public:
    // Default Constructor
    User() = default;

    // Parametrized constructor
    User(int userID, std::string name,
       std::string email,
       std::string userType,
       std::string membershipDate);

    // Setters & Getters
    void setUserID(int userID) {
        this->userID = userID;
    }
    [[nodiscard]] int getUserID() const {
        return this->userID;
    }

    void setName(std::string name) {
        this->name = std::move(name);
    }
    [[nodiscard]] std::string getName() const {
        return this->name;
    }

    void setEmail(std::string email) {
        this->email = std::move(email);
    }
    [[nodiscard]] std::string getEmail() const {
        return this->email;
    }

    void setUserType(std::string userType) {
        this->userType = std::move(userType);
    }
    [[nodiscard]] std::string getUserType() const {
        return this->userType;
    }

    void setMembershipDate(std::string membershipDate) {
        this->membershipDate = std::move(membershipDate);
    }
    [[nodiscard]] std::string getMembershipDate() const {
        return this->membershipDate;
    }

    // Menu function
    // Purpose: Direct users to their proper menus
    virtual void menu() = 0;

    // display function
    virtual void display() const = 0;

    // Destructor
    virtual ~User() = default;

};

#endif
