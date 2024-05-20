#include "helpers.h"
#include "requests.h"

#include <string>
#include <iostream>
#include <functional>
#include <map>

#include "library_api.hpp"

using std::cin;
using std::string;
using std::map;
using std::cout;

int main() {
    string command;
    string session_cookie;
    string jwt_token;

    map<std::string, std::function<void()>> command_map = {
        {"register", [&]() { register_user(); }},
        {"login", [&]() { session_cookie = login_user(); }},
        {"enter_library", [&]() { jwt_token = enter_library(session_cookie); }},
        {"get_books", [&]() { get_books(session_cookie, jwt_token); }},
        {"get_book", [&]() { get_book(session_cookie, jwt_token); }},
        {"add_book", [&]() { add_book(session_cookie, jwt_token); }},
        {"delete_book", [&]() { delete_book(session_cookie, jwt_token); }},
        {"logout", [&]() { logout_user(session_cookie); jwt_token = ""; }},
        {"exit", [&]() { std::exit(0); }}
    };

    while (cin >> command) {
        auto it = command_map.find(command);
        if (it != command_map.end()) {
            it->second();
        } else {
            cout << "Unknown command: " << command << "\n";
        }
    }

    return 0;
}
