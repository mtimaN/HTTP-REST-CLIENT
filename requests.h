#ifndef _REQUESTS_
#define _REQUESTS_

#include <string>
#include <vector>

// computes and returns a GET request string
std::string compute_get_request(const std::string &host, const std::string &url, 
                                const std::string &query_params, 
                                const std::vector<std::string> &cookies,
                                const std::string &web_token);

// computes and returns a POST request string
std::string compute_post_request(const std::string &host, const std::string &url, 
                                 const std::string &content_type, 
                                 const std::vector<std::string> &body_data, 
                                 const std::vector<std::string> &cookies,
                                 const std::string &web_token);

// computes and returns a DELETE request string
std::string compute_delete_request(const std::string &host, const std::string &url, 
                                   const std::vector<std::string> &cookies,
                                   const std::string &web_token);

#endif
