//
// Created by maria on 18.09.2023.
//
#include <iostream>
#include "LeetCode.h"

int main() {
    LeetCode leetCode = LeetCode();

    std::vector nums = {-1, 0, 1, 2, -1, -4};
    std::vector<std::vector<int>> result = leetCode.threeSum(nums);

    // std::vector<std::string> strs;    strs.push_back("flower");    strs.push_back("flow");    strs.push_back("flight");
    // int result = leetCode.RomanToInt("MCMXCIV");

    for (auto i : result) {
        std::cout << "[ ";
        for (auto j: i) {
            std::cout << j << " ";
        }
        std::cout << "]\n";
    }

    return 0;
}