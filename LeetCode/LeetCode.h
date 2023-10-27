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

    // Binary Search
    static std::string longestCommonPrefix(std::vector<std::string>& strs);
    static std::string _longestCommonPrefix(std::vector<std::string>& strs);
    static std::string _longestCommonPrefix_(std::vector<std::string>& strs);
    static bool isCommonPrefix(std::vector<std::string> &vector, int middle);

    int search(std::vector<int>& nums, int target); // problem 33
    std::vector<int> searchRange(std::vector<int> &nums, int target); // problem 34 (Leetcode reports overflow which I cant reproduce)
    int searchInsert(std::vector<int>& nums, int target); // problem 35
    int mySqrt(int x); // problem 69
    bool searchMatrix(std::vector<std::vector<int>>& matrix, int target);
    bool searchWithDuplicates(std::vector<int>& nums, int target); // problem 81

    //
    std::vector<std::vector<int>> threeSum(std::vector<int>& nums);

    std::vector<std::string> CartesianProduct(std::string digits);

};


#endif //C_ROMANTOINT_H
