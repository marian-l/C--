#include "HTTPRequest.hpp"

#include <string.h>
#include <cstdlib>
#include <iostream>

// Helper function to convert a string to uppercase
std::string to_uppercase(std::string str)
{
    for (char& c : str) {
        c = std::toupper(c);
    }
    return str;
};

char * process_http_method(char *method) 
{
    std::string upperMethod = to_uppercase(method);
    auto it = methodMap.find(upperMethod);
    if (it != methodMap.end()) {
        HTTPMethods httpMethod = it->second;
        // Now you have the corresponding HTTPMethods enum value (e.g., GET, POST, etc.)
        // Use it as needed in your code
        std::cout << "HTTP Method: " << httpMethod << std::endl;
    } else {
        // Handle the case when the method string is not recognized
        std::cout << "Invalid HTTP Method" << std::endl;
    }
};

struct HTTPRequest http_request_constructor(const char *request_string)
{
    char* processed_string = strdup(request_string);
    
    for (int i = 0; i < strlen(request_string) - 1; i++)
    {
        if (request_string[i] == '\n' && request_string[i + 1] == '\n')
        {
            processed_string[i + 1] = '|';
        }
    }

    char *token = strtok(processed_string, "\n");
    char *headers = strtok(NULL, "\n\n");
    char *body = strtok(NULL, "\n\n");
    char *method = strtok(token, " ");
    char *URI;
    float HTTPVersion;
    struct HTTPRequest request;

    free(processed_string);
    process_http_method(method);
    process_http_uri(uri);


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