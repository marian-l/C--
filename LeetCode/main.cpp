//
// Created by maria on 18.09.2023.
//
#include <iostream>
#include "LeetCode.h"

int main() {
    LeetCode leetCode = LeetCode();

    // int result = leetCode.RomanToInt("MCMXCIV");

    std::vector<std::string> strs;
    strs.push_back("flower");
    strs.push_back("flow");
    strs.push_back("flight");

    std::string result = LeetCode::longestCommonPrefix(strs);
    std::cout << result;

    return 0;
}