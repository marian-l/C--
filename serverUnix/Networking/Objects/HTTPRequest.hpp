#ifndef HTTPRequest_h
#define HTTPRequest_h

#include <stdio.h>
#include <unordered_map>

#endif

enum HTTPMethods
{
    GET,
    POST,
    PUT,
    HEAD,
    PATCH,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE
};

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

struct HTTPRequest 
{
    int Method;
    char *URI;
    float HTTPVersion;
};

struct HTTPRequest http_request_constructor(char *request_string);
