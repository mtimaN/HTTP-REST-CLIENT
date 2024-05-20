#pragma once

#include <string>

void register_user();

std::string login_user();

std::string enter_library(std::string &session_cookie);

void get_books(std::string &session_cookie, std::string &jwt_token);

void get_book(std::string &session_cookie, std::string &jwt_token);

void add_book(std::string &session_cookie, std::string &jwt_token);

void delete_book(std::string &session_cookie, std::string &jwt_token);

void logout_user(std::string &session_cookie);