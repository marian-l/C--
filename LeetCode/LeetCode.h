//
// Created by maria on 18.09.2023.
//

#ifndef C_ROMANTOINT_H
#define C_ROMANTOINT_H

#include <vector>
#include <string>

class LeetCode {
public:
    LeetCode();
    static int RomanToInt (std::string s);

    static std::string longestCommonPrefix(std::vector<std::string>& strs);
    static std::string _longestCommonPrefix(std::vector<std::string>& strs);
    static std::string _longestCommonPrefix_(std::vector<std::string>& strs);
    static bool isCommonPrefix(std::vector<std::string> &vector, int middle);

    std::vector<std::vector<int>> threeSum(std::vector<int>& nums);

    std::vector<std::string> CartesianProduct(std::string digits);
    std::vector<std::string> _CartesianProduct(std::string digits);
};


#endif //C_ROMANTOINT_H
