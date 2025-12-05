
#ifndef NONMEMBER_H
#define NONMEMBER_H

#include "User.h"

class Library;

// NonMember pays per rental (simulated).
class NonMember : public User {
private:
    Library* lib = nullptr;

public:
    NonMember() = default;
    NonMember(int id, const std::string& name, const std::string& email,
              const std::string& membershipDate, Library* lib);

    void display() const override;
    void menu() override;
};

#endif // NONMEMBER_H

// Emma Das
