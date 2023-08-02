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

std::unordered_map<std::string, std::string> HTTPHeaders;

struct HTTPRequest 
{
    int Method;
    char *URI;
    float HTTPVersion;
};

struct HTTPRequest http_request_constructor(char *request_string);

/*
A-IM
Accept 
Accept-Charset 
Accept-Encoding
Accept-Language
Accept-Datetime
Access-Control-Request-Method  
Access-Control-Request-Headers 
Authorization  
Cache-Control  
Connection 
Content-Length 
Content-Type   
Cookie 
Date   
Expect 
Forwarded  
From   
Host   
If-Match   
If-Modified-Since  
If-None-Match  
If-Range   
If-Unmodified-Since
Max-Forwards   
Origin 
Pragma 
Proxy-Authorization
Range  
Referer
TE 
User-Agent 
Upgrade
Via
Warning
Dnt
X-Requested-With   
X-CSRF-Token   
*/
