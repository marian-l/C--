#ifndef HTTP_Request_Validator_hpp
#define HTTP_Request_Validator_hpp

#include <string>
#include <iostream>
#include <stdio.h>
#include <unordered_map>
#include <sstream>
#include <algorithm>
#include <vector>
#include "./../../Library/StringFunction.hpp"
#include "HTTPRequest.hpp"

#endif

bool validateHeaders(HTTPRequest httprequest);
bool validateBody(HTTPRequest httprequest);

bool http_request_validator(HTTPRequest httprequest);

bool checkHTTPSmuggle(HTTPRequest httprequest);