#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_map>
#include<windows.h>
#pragma execution_character_set( "utf-8" )
struct Book {
    std::string title;
    std::string author;
    std::string genre;
    bool issued = false;
};
struct Reader {
    std::string name;
    std::vector<Book> issuedBooks;
};
bool authenticate(const std::string& username, const std::string& password) {
    return username == "admin" && password == "admin";
}
bool isAuthorized(const std::string& username, const std::string& operation) {
    return operation == "view";
}

int main() {
    SetConsoleOutputCP(65001);
    std::vector<Book> library;
    std::unordered_map<std::string, Reader> readers;
    std::ifstream file("db.txt");
    std::string line;
    while (std::getline(file, line)) {
        if (line.substr(0, 5) == "Book:") {
            Book book;
            book.title = line.substr(5, line.find(',', 5) - 5);
            book.author = line.substr(line.find(',', 5) + 2, line.find(',', line.find(',', 5) + 1) - (line.find(',', 5) + 2));
            book.genre = line.substr(line.find(',', line.find(',', 5) + 1) + 2);
            library.push_back(book);
        }
        else if (line.substr(0, 7) == "Reader:") {
            Reader reader;
            reader.name = line.substr(7);
            readers[reader.name] = reader;
        }
    }
    file.close();

    std::string username, password;
    std::cout << "Enter username: ";
    std::cin >> username;
    std::cout << "Enter password: ";
    std::cin >> password;

    if (authenticate(username, password)) {
        int choice;
        do {
            std::cout << "1. View library\n";
            std::cout << "2. Issue book\n";
            std::cout << "3. Return book\n";
            std::cout << "4. View readers\n";
            std::cout << "5. Generate report\n";
            std::cout << "0. Exit\n";
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            switch (choice) {
            case 1: {
                if (isAuthorized(username, "view")) {
                    std::cout << "Library:\n";
                    for (const auto& book : library) {
                        std::cout << "Title: " << book.title << ", Author: " << book.author << ", Genre: " << book.genre << (book.issued ? " (Issued)" : "") << "\n";
                    }
                }
                else {
                    std::cout << "Access denied\n";
                }
                break;
            }
            case 2: {
                std::string readerName, bookTitle;
                std::cout << "Enter reader name: ";
                std::cin >> readerName;
                std::cout << "Enter book title: ";
                std::cin >> bookTitle;

                auto readerIt = readers.find(readerName);
                if (readerIt != readers.end()) {
                    auto bookIt = std::find_if(library.begin(), library.end(), [&](const Book& book) {
                        return book.title == bookTitle && !book.issued;
                        });
                    if (bookIt != library.end()) {
                        bookIt->issued = true;
                        readerIt->second.issuedBooks.push_back(*bookIt);
                        std::cout << "Book issued successfully\n";
                    }
                    else {
                        std::cout << "Book not found or already issued\n";
                    }
                }
                else {
                    std::cout << "Reader not found\n";
                }
                break;
            }
            case 3: {
                std::string readerName, bookTitle;
                std::cout << "Enter reader name: ";
                std::cin >> readerName;
                std::cout << "Enter book title: ";
                std::cin >> bookTitle;

                auto readerIt = readers.find(readerName);
                if (readerIt != readers.end()) {
                    auto bookIt = std::find_if(readerIt->second.issuedBooks.begin(), readerIt->second.issuedBooks.end(), [&](const Book& book) {
                        return book.title == bookTitle;
                        });
                    if (bookIt != readerIt->second.issuedBooks.end()) {
                        auto libraryBookIt = std::find_if(library.begin(), library.end(), [&](const Book& book) {
                            return book.title == bookTitle;
                            });
                        libraryBookIt->issued = false;
                        readerIt->second.issuedBooks.erase(bookIt);
                        std::cout << "Book returned successfully\n";
                    }
                    else {
                        std::cout << "Book not found in reader's issued books\n";
                    }
                }
                else {
                    std::cout << "Reader not found\n";
                }
                break;
            }
            case 4: {
                if (isAuthorized(username, "view")) {
                    std::cout << "Readers:\n";
                    for (const auto& reader : readers) {
                        std::cout << "Name: " << reader.first << "\n";
                        std::cout << "Issued books:\n";
                        for (const auto& book : reader.second.issuedBooks) {
                            std::cout << "  Title: " << book.title << ", Author: " << book.author << ", Genre: " << book.genre << "\n";
                        }
                    }
                }
                else {
                    std::cout << "Access denied\n";
                }
                break;
            }
            case 5: {
                break;
            }
            case 0:
                break;
            default:
                std::cout << "Invalid choice\n";
            }
        } while (choice != 0);
    }
    else {
        std::cout << "Invalid username or password\n";
    }
    std::ofstream ofile("db.txt");
    for (const auto& book : library) {
        ofile << "Book:" << book.title << "," << book.author << "," << book.genre << "\n";
    }
    for (const auto& reader : readers) {
        ofile << "Reader:" << reader.first << "\n";
    }
    file.close();
    return 0;
}