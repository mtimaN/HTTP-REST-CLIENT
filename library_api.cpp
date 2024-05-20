#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>

#include "nlohmann/json.hpp"

#include "helpers.h"
#include "requests.h"

#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::string;
using std::vector;

using json = nlohmann::json;

#define HOST (char *)"34.246.184.49"
#define PORT 8080

#define JSON_APP "application/json"

using namespace std;

void register_user() {
    string name, password;
    cin.ignore();
    cout << "username=";
    getline(cin, name);
    cout << "password=";
    getline(cin, password);

    if (name.find(' ') != string::npos || password.find(' ') != string::npos) {
        cout << "Error: invalid name or password\n";
        return;
    }

    json json_payload = {
        {"username", name},
        {"password", password},
    };

    vector<string> payload(1);
    payload[0] = json_payload.dump();
    string message = compute_post_request(HOST, "/api/v1/tema/auth/register", JSON_APP, payload, {}, {});

    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message.data());
    string response(receive_from_server(sockfd));
    close_connection(sockfd);

    size_t payload_start = response.find('{');
    if (payload_start != string::npos) {
        json json_response = json::parse(response.substr(payload_start));
        string error = json_response["error"];
        cout << "Error: " << error << '\n';
    } else {
        cout << "Successfully registered user\n";
    }
}

string login_user() {
    string name, password;
    cin.ignore();
    cout << "username=";
    getline(cin, name);
    cout << "password=";
    getline(cin, password);

    if (name.find(' ') != string::npos || password.find(' ') != string::npos) {
        cout << "Error: invalid name or password\n";
        return "";
    }

    json json_payload = {
        {"username", name},
        {"password", password},
    };

    vector<string> payload(1);
    payload[0] = json_payload.dump();
    string message = compute_post_request(HOST, "/api/v1/tema/auth/login", JSON_APP, payload, {}, {});

    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message.data());
    string response(receive_from_server(sockfd));
    close_connection(sockfd);

    const size_t payload_start = response.find('{');
    if (payload_start != string::npos) {
        json json_response = json::parse(response.substr(payload_start));
        string error = json_response["error"];
        cout << "Error: " << error << '\n';
    } else {
        size_t cookie_pos = response.find("connect.sid=");
        if (cookie_pos == string::npos) {
            cout << "Error: session cookie not found\n";
            return "";
        }
        string cookie_line = response.substr(cookie_pos);
        size_t end_pos = cookie_line.find(';');
        cout << "Successfully logged in\n";
        return cookie_line.substr(0, end_pos);
    }

    return "";
}

string enter_library(string &session_cookie) {
    vector<string> cookies = {session_cookie};
    string message = compute_get_request(HOST, "/api/v1/tema/library/access", {}, cookies, "");

    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message.data());
    string response(receive_from_server(sockfd));
    close_connection(sockfd);

    size_t payload_start = response.find('{');
    if (payload_start != string::npos) {
        json json_response = json::parse(response.substr(payload_start));
        if (json_response.contains("error")) {
            string error = json_response["error"];
            cout << "Error: " << error << '\n';
        } else if (json_response.contains("token")) {
            cout << "Successfully received token\n";
            return json_response["token"];
        }
    } else {
        cout << "Error: token not received\n";
    }
    return "";
}

void get_books(string &session_cookie, string &jwt_token) {
    vector<string> cookies = {session_cookie};
    string message = compute_get_request(HOST, "/api/v1/tema/library/books", {}, cookies, jwt_token);

    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message.data());
    string response(receive_from_server(sockfd));
    close_connection(sockfd);
    
    size_t payload_start = response.rfind("{\"error");
    if (payload_start != string::npos) {
        json json_response = json::parse(response.substr(payload_start));
        if (json_response.contains("error")) {
            string error = json_response["error"];
            cout << "Error: " << error << '\n';
        }
    } else {
        payload_start = response.rfind('[');
        cout << response.substr(payload_start) << '\n';
    }
}


void get_book(string &session_cookie, string &jwt_token) {
    int id;
    cout << "id=";
    cin >> id;
    if (cin.fail()) {
        cout << "Error: Invalid id\n";
        return;
    }

    vector<string> cookies = {session_cookie};
    string message = compute_get_request(HOST, "/api/v1/tema/library/books/" + to_string(id), {}, cookies, jwt_token);

    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message.data());
    string response(receive_from_server(sockfd));
    close_connection(sockfd);

    size_t payload_start = response.rfind("{\"error");
    if (payload_start != string::npos) {
        json json_response = json::parse(response.substr(payload_start));
        if (json_response.contains("error")) {
            string error = json_response["error"];
            cout << "Error: " << error << '\n';
        }
    } else {
        payload_start = response.rfind('{');
        cout << response.substr(payload_start) << '\n';
    }
}


void add_book(string &session_cookie, string &jwt_token) {
    vector<string> cookies = {session_cookie};
    string message;

    string title, author, genre, publisher;
    string page_count_string;
    int page_count;

    cin.ignore();
    cout << "title=";
    getline(cin, title);
    
    cout << "author=";
    getline(cin, author);

    cout << "genre=";
    getline(cin, genre);

    cout << "publisher=";
    getline(cin, publisher);
    cout << "page_count=";
    getline(cin, page_count_string);


    if (title == "" || author == "" || genre == "" || publisher == "" ||
            page_count_string == "") {
        cout << "Error: Invalid book data\n";
        return;
    }

    try {
        page_count = std::stoi(page_count_string);
    } catch (std::invalid_argument const& ex) {
        cout << "Error: invalid page count\n";
        return;
    } catch (std::out_of_range const& ex) {
        cout << "Error: invalid page count\n";
        return;
    }

    json json_payload = {
        {"title", title},
        {"author", author},
        {"genre", genre},
        {"publisher", publisher},
        {"page_count", page_count},
    };

    vector<string> payload(1);
    payload[0] = json_payload.dump();
    message = compute_post_request(HOST, "/api/v1/tema/library/books", JSON_APP, payload, cookies, jwt_token);

    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message.data());
    string response(receive_from_server(sockfd));
    close_connection(sockfd);

    size_t payload_start = response.rfind("{\"error");
    if (payload_start != string::npos) {
        json json_response = json::parse(response.substr(payload_start));
        if (json_response.contains("error")) {
            string error = json_response["error"];
            cout << "Error: " << error << '\n';
        }
    } else {
        cout << "Book successfully added\n";
    }
}

void delete_book(string &session_cookie, string &jwt_token) {
    int id;
    cout << "id=";
    cin >> id;
    if (cin.fail()) {
        cout << "Error: Invalid id\n";
        return;
    }

    vector<string> cookies = {session_cookie};
    string message = compute_delete_request(HOST, "/api/v1/tema/library/books/" + to_string(id), cookies, jwt_token);

    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message.data());
    string response(receive_from_server(sockfd));
    close_connection(sockfd);

    size_t payload_start = response.rfind("{\"error");
    if (payload_start != string::npos) {
        json json_response = json::parse(response.substr(payload_start));
        if (json_response.contains("error")) {
            string error = json_response["error"];
            cout << "Error: " << error << '\n';
        }
    } else {
        cout << "Successfully deleted book\n";
    }
}

void logout_user(string &session_cookie) {
    vector<string> cookies = {session_cookie};
    string message = compute_get_request(HOST, "/api/v1/tema/auth/logout", "", cookies, "");

    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message.data());
    string response(receive_from_server(sockfd));
    close_connection(sockfd);

    size_t payload_start = response.rfind("{\"error");
    if (payload_start != string::npos) {
        json json_response = json::parse(response.substr(payload_start));
        if (json_response.contains("error")) {
            string error = json_response["error"];
            cout << "Error: " << error << '\n';
        }
    } else {
        cout << "Successfully logged out\n";
    }
}