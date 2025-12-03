#ifndef FINE_H
#define FINE_H

#include <string>   // for string return type
#include <sstream>  // for converting amount to string
#include <iomanip>  // for formatting decimal places
using namespace std;

// Represents a monetary fine in the system
class Fine {
    double amount = 0.0; // stores the fine amount

public:
    Fine() = default; // default constructor (fine = 0)
    explicit Fine(double amount_); // create fine with a set amount

    double getAmount() const; // returns fine amount
    Fine operator+(const Fine& other) const; // allows adding fines together
    string toString() const; // returns fine amount as a formatted string
};

#endif // FINE_H

// Emma Das
