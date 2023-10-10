//
// Created by maria on 18.09.2023.
//
#include <iostream>
#include "LeetCode.h"

int main() {
    LeetCode leetCode = LeetCode();
    std::string digits = "23";
    std::vector<std::string> v1 = leetCode.CartesianProduct(digits);

    for(std::string s: v1) {
        std::cout << s << "; ";
    }

        return 0;
}