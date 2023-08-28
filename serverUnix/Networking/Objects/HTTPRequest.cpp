#include "HTTPRequest.hpp"

using namespace std;

std::unordered_map<std::string, HTTPMethods> methodMap = {
    {"GET", GET},
    {"POST", POST},
    {"PUT", PUT},
    {"HEAD", HEAD},
    {"PATCH", PATCH},
    {"DELETE", DELETE},
    {"CONNECT", CONNECT},
    {"OPTIONS", OPTIONS},
    {"TRACE", TRACE}
};

int process_http_method(string method) 
{
    // UPPERCASE CONVERSION
    method = to_uppercase(method);

    // SEARCH METHOD, RETURN methodMap.end() on failure
    auto it = methodMap.find(method);

    if (it != methodMap.end()) {
        HTTPMethods httpMethod = it->second;
        return httpMethod;
    } else {
        perror("HTTPMethod is not a legal method");
        exit(-1);
    }
};

string process_http_uri(string URI)
{
    return URI;
};

float process_http_version(string HTTPVersion)
{   
    HTTPVersion = HTTPVersion.substr(5);

    float httpversion_float = stof(HTTPVersion);

    return httpversion_float;
}

std::unordered_map<string, string> process_http_headers(string headers) 
{  
    unordered_map<string, string> HTTPHeaders;
    istringstream iss(headers);
    string line;

    while (std::getline(iss, line))
    {
        size_t pos = line.find(": ");
        if (pos != line.npos) 
        {
            string field = line.substr(0, pos);
            string value = line.substr(pos + 2);
            HTTPHeaders.insert_or_assign(field, value);
        }
    }

    return HTTPHeaders;
}

struct HTTPRequest http_request_constructor(char *request_string)
{
    struct HTTPRequest request;
    string body(request_string);
  
    string request_line = tokUntil(body, "\n");
    string headers = tokUntil(body, "\n\n");

    // cut everything but body from processed_string
    body.erase(0, headers.length());
    request.body = body;

    // cut request_line from headers-string
    headers.erase(0, request_line.length());

    // MARK METHOD
    int pos = request_line.find(" ");
    string method = request_line.substr(0, pos);
    request_line = request_line.erase(0, pos + 1);
    request.Method = process_http_method(method);

    // URI
    pos = request_line.find(" ");
    string URI = request_line.substr(0, pos);
    request_line.erase(0, pos + 1);
    request.URI = process_http_uri(URI);
    
    // HTTPVersion
    string HTTPVersion = request_line;
    request.HTTPVersion = process_http_version(HTTPVersion);

    // headers
    request.HTTPHeaders = process_http_headers(headers);

    return request;
}