#include "FileManager.h"
#include "Library.h"
#include <fstream>
#include <iostream>

using namespace std;

FileManager::FileManager(const string& books,
                         const string& trans)
    : booksfile(books), transfile(trans) {}

bool FileManager::exists(const string& file) {
    ifstream f(file);
    return f.good();
}

void FileManager::loaddata() {

    // check files exist
    if (!exists(booksfile)) {
        cout << "books file not found\n";
        return;
    }
    if (!exists(transfile)) {
        cout << "transactions file not found\n";
        return;
    }

    // load using library functions
    Library::instance().loadFromCSV(booksfile, transfile);

    cout << "data loaded\n";
}

void FileManager::savedata() {

    // save using library functions
    Library::instance().saveToCSV(booksfile, transfile);

    cout << "data saved\n";
}
