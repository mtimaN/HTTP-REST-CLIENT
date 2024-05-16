#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

int main(int argc, char *argv[])
{
    char *message;
    char *response;
    int sockfd;

        
    // Ex 1.1: GET dummy from main server
    sockfd = open_connection("54.170.241.232", 8080, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request("54.170.241.232", "/api/v1/dummy", NULL, NULL, 0);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s\n", response);
    close_connection(sockfd);
    free(message);
    free(response);

    // Ex 1.2: POST dummy and print response from main server
    sockfd = open_connection("54.170.241.232", 8080, AF_INET, SOCK_STREAM, 0);
    char *payload = "data=hello";
    message = compute_post_request("54.170.241.232", "/api/v1/dummy", "application/x-www-form-urlencoded",
                                    &payload, 1, NULL, 0);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s\n", response);
    close_connection(sockfd);
    free(message);
    free(response);
    // Ex 2: Login into main server
    char *body[2];
    body[0] = strdup("username=student");
    body[1] = strdup("password=student");
    sockfd = open_connection("54.170.241.232", 8080, AF_INET, SOCK_STREAM, 0);
    message = compute_post_request("54.170.241.232", "/api/v1/auth/login", "application/x-www-form-urlencoded",
                                        body, 2, NULL, 0);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s\n", response);
    close_connection(sockfd);
    free(message);
    free(body[0]);
    free(body[1]);
    
    // Ex 3: GET weather key from main server
    char *cookies[3];
    cookies[0] = strdup("connect.sid=s%3AusQLCn6UfJ4l1Pf4q3MY-8SimCoyYJMX.b15PVd6wroRW%2Fi2IGAj4%2FHxoNj%2Fqe%2FB1dHnpV%2F69fnE");
    cookies[1] = strdup("Path=/");
    cookies[2] = strdup("HttpOnly");
    sockfd = open_connection("54.170.241.232", 8080, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request("54.170.241.232", "/api/v1/weather/key", NULL, cookies, 3);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s\n", response);
    close_connection(sockfd);
    free(cookies[0]);
    free(cookies[1]);
    free(cookies[2]);
    free(message);

    // Ex 4: GET weather data from OpenWeather API

    // Ex 5: POST weather data for verification to main server
    // Ex 6: Logout from main server

    // BONUS: make the main server return "Already logged in!"

    // free the allocated data at the end!

    return 0;
}
