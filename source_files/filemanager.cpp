#include "FileManager.h"
#include "Library.h"
#include <fstream>
#include <iostream>

FileManager::FileManager(const std::string& books,
                         const std::string& trans)
    : booksfile(books), transfile(trans) {}

bool FileManager::exists(const std::string& file) {
    std::ifstream f(file);
    return f.good();
}

void FileManager::loaddata() {

    // check files exist
    if (!exists(booksfile)) {
        std::cout << "books file not found\n";
        return;
    }
    if (!exists(transfile)) {
        std::cout << "transactions file not found\n";
        return;
    }

    // load using library functions
    Library::instance().loadFromCSV(booksfile, transfile);

    std::cout << "data loaded\n";
}

void FileManager::savedata() {

    // save using library functions
    Library::instance().saveToCSV(booksfile, transfile);

    std::cout << "data saved\n";
}

