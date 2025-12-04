#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>

class FileManager {
private:
    std::string booksfile;
    std::string transfile;

public:
    FileManager(const std::string& books = "books.csv",
                const std::string& trans = "transactions.csv");

    // load all data
    void loaddata();

    // save all data
    void savedata();

    // check if file exists
    static bool exists(const std::string& file);
};

#endif
