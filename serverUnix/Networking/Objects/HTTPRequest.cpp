#include "HTTPRequest.hpp"

#include <string.h>
#include <cstdlib>
#include <iostream>

#include <stdio.h>
#include <unordered_map>

// Helper function to convert a string to uppercase
std::string to_uppercase(std::string str)
{
    for (char& c : str) {
        c = std::toupper(c);
    }
    return str;
};

int process_http_method(char *method) 
{
    // UPPERCASE CONVERSION
    std::string upperMethod = to_uppercase(method);

    // SEARCH METHOD, RETURN methodMap.end() on failure
    auto it = methodMap.find(upperMethod);

    if (it != methodMap.end()) {
        HTTPMethods httpMethod = it->second;
        free(method);
        return httpMethod;
    } else {
        perror("HTTPMethod is not a legal method");
        exit(-1);
    }
};

char * process_http_uri(char *URI)
{

};

char * process_http_version(char *HTTPVersion)
{
    HTTPVersion = strtok(HTTPVersion, "/"); 

    // In subsequent calls, the function expects a null pointer and uses the position right after the end of the last token as the new starting location for scanning.
    HTTPVersion = strtok(NULL, "/");
}

char * process_http_headers(char *headers) 
{
    while (headers)
    {
        char *header = strtok(headers, "\n");
        char *field = strtok(header, ": ");
        char *value = strtok(NULL, ": ");

        HTTPHeaders[field] = value;        
    }
}

struct HTTPRequest http_request_constructor(const char *request_string)
{
    struct HTTPRequest request;
    char* processed_string = strdup(request_string);

    char *token = strtok(processed_string, "\n");
    char *headers = strtok(NULL, "\n\n");
    char *body = strtok(NULL, "\n\n");
    
    // request line
    char *method = strtok(token, " ");
    request.Method = process_http_method(method);

    char *URI = strtok(NULL, " ");
    process_http_uri(URI);

    char *HTTPVersion = strtok(NULL, "\n");
    process_http_version(HTTPVersion);
    request.HTTPVersion = (float)atof(HTTPVersion);

    // headers
    process_http_headers(headers);
    free(processed_string);


}
/*
GET /80 HTTP/1.1
Host: 172.27.34.56
Connection: keep-alive
DNT: 1
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/115.0.0.0 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,/*;q=0.8,application/signed-exchange;v=b3;q=0.7
cp-extension-installed: Yes
Accept-Encoding: gzip, deflate
Accept-Language: de-DE,de;q=0.9,en-US;q=0.8,en;q=0.7
Range: bytes=85-85
If-Range: Wed, 19 Jul 2023 19:15:56 GMT

GET /20 HTTP/1.1
Host: 172.27.34.56
Connection: keep-alive
DNT: 1
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/115.0.0.0 Safari/537.36
Sec-Purpose: prefetch;prerender
Purpose: prefetch
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,/*;q=0.8,application/signed-exchange;v=b3;q=0.7
cp-extension-installed: Yes
Accept-Encoding: gzip, deflate
Accept-Language: de-DE,de;q=0.9,en-US;q=0.8,en;q=0.7

*/