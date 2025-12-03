#include <sstream>
#include <iomanip>
#include "Fine.h" // include the Fine class header

using namespace std; 

// Constructor that sets the fine amount
Fine::Fine(double amount_)
    : amount(amount_) // store the amount in the object
{}

// Returns the fine amount
double Fine::getAmount() const { 
    return amount; 
}

// Adds two Fine objects together and returns a new Fine object
Fine Fine::operator+(const Fine& other) const {
    return Fine(amount + other.amount); // add amounts from both fines
}

// Converts the fine amount to a formatted string (example: "5.00")
string Fine::toString() const {
    ostringstream oss; // create a stream to build the string
    oss << fixed << setprecision(2) << amount; // keep 2 decimal places
    return oss.str(); // return string version of amount
}

// Emma Das
