#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include <string>
#include <iostream>

std::string compute_get_request(const std::string &host, const std::string &url, 
                                const std::string &query_params, 
                                const std::vector<std::string> &cookies,
                                const std::string &web_token) {
    std::string message;
    std::string line;

    // Step 1: write the method name, URL, request params (if any), and protocol type
    if (!query_params.empty()) {
        line = "GET " + url + "?" + query_params + " HTTP/1.1";
    } else {
        line = "GET " + url + " HTTP/1.1";
    }

    compute_message(message, line);

    // Step 2: add the host
    line = "Host: " + host;
    compute_message(message, line);

    // Step 3 (optional): add headers and/or cookies, according to the protocol format
    if (web_token != "") {
        line = "Authorization: Bearer " + web_token;
        compute_message(message, line);
    }

    if (!cookies.empty()) {
        line = "Cookie: ";
        for (size_t i = 0; i < cookies.size() - 1; ++i) {
            line += cookies[i] + "; ";
        }
        line += cookies[cookies.size() - 1];
        compute_message(message, line);
    }

    // Step 4: add final new line
    compute_message(message, "");

    return message;
}

std::string compute_post_request(const std::string &host, const std::string &url, 
                                 const std::string &content_type, 
                                 const std::vector<std::string> &body_data, 
                                 const std::vector<std::string> &cookies,
                                 const std::string &web_token) {
    std::string message;
    std::string line;
    std::string body_data_buffer;

    // Step 1: write the method name, URL and protocol type
    line = "POST " + url + " HTTP/1.1";
    compute_message(message, line);

    // Step 2: add the host
    line = "Host: " + host;
    compute_message(message, line);

    // Step 3: add necessary headers (Content-Type and Content-Length are mandatory)
    line = "Content-Type: " + content_type;
    compute_message(message, line);

    // Concatenate body data fields
    for (size_t i = 0; i < body_data.size(); ++i) {
        body_data_buffer += body_data[i];
        if (i < body_data.size() - 1) {
            body_data_buffer += "&";
        }
    }

    line = "Content-Length: " + std::to_string(body_data_buffer.size());
    compute_message(message, line);

    // Step 4 (optional): add cookies
    if (web_token != "") {
        line = "Authorization: Bearer " + web_token;
        compute_message(message, line);
    }

    if (!cookies.empty()) {
        line = "Cookie: ";
        for (size_t i = 0; i < cookies.size() - 1; ++i) {
            line += cookies[i] + "; ";
        }
        line += cookies[cookies.size() - 1];
        compute_message(message, line);
    }

    // Step 5: add new line at end of header
    compute_message(message, "");

    // Step 6: add the actual payload data
    message += body_data_buffer;

    return message;
}

std::string compute_delete_request(const std::string &host, const std::string &url, 
                                   const std::vector<std::string> &cookies,
                                   const std::string &web_token) {
    std::string message;
    std::string line;
    std::string body_data_buffer;

    // Step 1: write the method name, URL and protocol type
    line = "DELETE " + url + " HTTP/1.1";
    compute_message(message, line);

    // Step 2: add the host
    line = "Host: " + host;
    compute_message(message, line);

    line = "Content-Length: " + std::to_string(body_data_buffer.size());
    compute_message(message, line);

    // Step 3 (optional): add cookies
    if (web_token != "") {
        line = "Authorization: Bearer " + web_token;
        compute_message(message, line);
    }

    if (!cookies.empty()) {
        line = "Cookie: ";
        for (size_t i = 0; i < cookies.size() - 1; ++i) {
            line += cookies[i] + "; ";
        }
        line += cookies[cookies.size() - 1];
        compute_message(message, line);
    }

    // Step 5: add new line at end of header
    compute_message(message, "");

    // Step 6: add the actual payload data
    message += body_data_buffer;

    return message;
}