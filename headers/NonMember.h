#ifndef NONMEMBER_H
#define NONMEMBER_H

#include "User.h"
using namespace std;

class Library;

// NonMember pays per rental (simulated).
class NonMember : public User {
private:
    Library* lib = nullptr;

public:
    NonMember() = default;
    NonMember(int id, const string& name, const string& email,
              const string& membershipDate, Library* lib);

    void display() const override;
    void menu() override;
};

#endif // NONMEMBER_H

// Emma Das
