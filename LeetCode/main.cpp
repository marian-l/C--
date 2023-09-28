//
// Created by maria on 18.09.2023.
//
#include <iostream>
#include "LeetCode.h"

int main() {
    LeetCode leetCode = LeetCode();

    std::vector nums = {4,0,5,-5,3,3,0,-4,-5};

    std::cout << leetCode.ClosestThreeSum(nums, -2);

    return 0;
}