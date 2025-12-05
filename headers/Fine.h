#ifndef FINE_H
#define FINE_H
// this class represents a monetary fine in the system
// it basically charges a penalty of $.50 per each day (late) they take to return the book.

#include <string>   // for string return type
#include <sstream>  // for converting amount to string
#include <iomanip>  // for formatting decimal places


// Represents a monetary fine in the system
class Fine {
    double amount = 0.0; // stores the fine amount

public:
    Fine() = default; // default constructor when there is $0.00 

//Marked "explicit" to prevent implicit conversions
//from double to Fine because if it does it would not compile.
    explicit Fine(double amount_); // create fine with a set amount

    double getAmount() const; // returns fine amount
    Fine operator+(const Fine& other) const; // allows adding fines together
    std:: string toString() const; // returns fine amount as a formatted string
};

#endif // FINE_H

//Gisneiry 



