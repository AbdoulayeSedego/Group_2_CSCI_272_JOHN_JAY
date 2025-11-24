#include "User.h"

#include <utility>

// Constructor
User::User(int userID, std::string name,
       std::string email,
       std::string userType,
       std::string membershipDate)
: userID(userID), name(std::move(name)), email(std::move(email)),
  userType(std::move(userType)), membershipDate(std::move(membershipDate)) {}
