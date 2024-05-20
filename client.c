#include "helpers.h"
#include "requests.h"

#include <string>
#include <iostream>

#include "library_api.hpp"

using namespace std;

int main() {
    string command;
    string session_cookie;
    string jwt_token;
    while (cin >> command) {
        if (command == "register") {
            register_user();
        } else if (command == "login") {
            session_cookie = login_user();
        } else if (command == "enter_library") {
            jwt_token = enter_library(session_cookie);
        } else if (command == "get_books") {
            get_books(session_cookie, jwt_token);
        } else if (command == "get_book") {
            get_book(session_cookie, jwt_token);
        } else if (command == "add_book") {
            add_book(session_cookie, jwt_token);
        } else if (command == "delete_book") {
            delete_book(session_cookie, jwt_token);
        } else if (command == "logout") {
            logout_user(session_cookie);
            jwt_token = "";
        } else if (command == "exit") {
            break;
        }
    }

    return 0;
}
