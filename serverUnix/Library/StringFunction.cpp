#include "StringFunction.hpp"

// Stringtoken Funktion, welche entweder den ganzen oder den Substring zurückgibt. 
std::string tokUntil(std::string str, std::string delimiter) {
    auto pos = str.find(delimiter);
    // Falls es gefunden wurde, entweder den Substring oder den ganzen String zurückgeben
    return pos == std::string::npos ? str : str.substr(0, pos);
}

std::string to_uppercase(std::string str)
{
    for (char& c : str) {
        c = std::toupper(c);
    }
    return str;
};