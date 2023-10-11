//
// Created by maria on 18.09.2023.
//
#include <iostream>
#include "LeetCode.h"

int main() {
    LeetCode leetCode = LeetCode();

    std::vector<int> nums = {5,1,2,3,4};
    int result = leetCode.search(nums, 1);

    std::cout << result;

        return 0;
}