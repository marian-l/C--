//
// Created by maria on 18.09.2023.
//

#include "LeetCode.h"
#include <climits>
#include <algorithm>
#include <list>

int LeetCode::RomanToInt(std::string s) {
    int result = 0;
    int index = 0;

    for (char c : s) {
        if (c == 'I') {
            result += 1;

        } else if (c == 'V') {
            result += 5;
            if (index > 0 && s[index - 1] == 'I') {
                result -= 2;
            }

        } else if (c == 'X') {
            result += 10;
            if (index > 0 && s[index - 1] == 'I') {
                result -= 2; }

        } else if (c == 'L') {
            result += 50;
            if (index > 0 && s[index - 1] == 'X') {
                result -= 20; }

        } else if (c == 'C') {
            result += 100;
            if (index > 0 && s[index - 1] == 'X') {
                result -= 20;
            }}

        else if (c == 'D') {
            result += 500;
            if (index > 0 && s[index - 1] == 'C') {
                result -= 200;
            }}

        else if (c == 'M') {
            result += 1000;
            if (index > 0 && s[index - 1] == 'C') {
                result -= 200;
            }
        }
        index += 1;
    }

    return result;
}

LeetCode::LeetCode() = default;

// first try
std::string LeetCode::_longestCommonPrefix(std::vector<std::string> &strs) {
std::string shortestWord = "";
int shortestWordLength = 128;

for (std::string s : strs) {
    if (s.size() < shortestWordLength) {
        shortestWordLength = s.size();
        shortestWord = s;
    }
}

int prefixLength = 0;
int tempPrefixLength = 128;

for (std::string s : strs) {
    for (int i = 0; i < s.size(); ++i) {
        if (s[i] == shortestWord[i]) {
            prefixLength += 1;
            continue;
        } else {
            break;
        }
    }
    if (prefixLength < tempPrefixLength) {
        tempPrefixLength = prefixLength;
    }
    prefixLength = 0;
}

std::string prefix = strs[0].substr(0, tempPrefixLength);
//    return std::string();
//     Clang-Tidy: Avoid repeating the return type from the declaration; use a braced initializer list instead#
    return prefix;
}

// second try
std::string LeetCode::_longestCommonPrefix_(std::vector<std::string> &strs) {
int shortestWordSize = 128;
int shortestWordIndex = 0;
    for(int i = 0; i != strs.size(); i++) {
        if(strs[i].size() < shortestWordSize) {
            shortestWordSize = strs[i].size();
            shortestWordIndex = i;
    }
}

std::string prefix = "";

for(int i = 0; i != shortestWordSize; i++) {
    char currentPrefixChar = strs[shortestWordIndex][i];
    for (auto & str : strs) {
        if (str[i] != currentPrefixChar) {
            return prefix;
        }
    }
    prefix.push_back(currentPrefixChar);
}
 return prefix;
}

// binary search
std::string LeetCode::longestCommonPrefix(std::vector<std::string> &strs) {
    if(strs.size() == 0) {
        return "";
    }
    int minLen = INT_MAX;
    for (std::string word : strs) {
        minLen = std::min(minLen, static_cast<int>(word.size()));
    };

    int low = 1;
    int high = minLen;

    while (low <= high) {
        int middle = (low + high) / 2;
        if (isCommonPrefix(strs, middle)) {
            low = middle + 1;
        } else {
            high = middle - 1;
        }
    }
    return strs[0].substr(0, (low + high) / 2);
}

bool LeetCode::isCommonPrefix(std::vector<std::string> &strs, int len) {
    std::string word = strs[0].substr(0, len);
    for (int i = 1; i < strs.size(); i++) {
//        if(!strs[i].starts_with(word))
            return false;
    }
    return true;
}

std::vector<std::vector<int>> LeetCode::threeSum(std::vector<int> &nums) {
    sort(nums.begin(), nums.end());

    std::vector<std::vector<int>> resultVector;
    int numsSize = nums.size();

    // only use one for loop and operate with the two-pointer technique
    for (int i = 0; i < numsSize - 2; ++i) {
        // filter out duplicate operations on the go
        if (i > 0 && nums[i] == nums[i - 1])
            continue;

        //
        int left = i + 1;
        int right = numsSize - 1;

        while (left < right) {

            // TODO: if present, include one [0,0,0] vector. currently loops over the [0, 0, 0] portion.
            //  might try to execute on the end of the loop and in-/decrement left or right twice in return.

            // if((nums[left] == 0) and (nums[right] == 0) and (nums[i] == 0)) { resultVector.push_back({nums[i], nums[left], nums[right]}); }
            // // check for duplicates in left and right
            // if (nums[left] == nums[left + 1]) {
            //     ++left;
            // } else if (nums[right] == nums[right - 1]) {
            //     --right;
            // }

            int sum = nums[i] + nums[left] + nums[right];

            if (sum == 0) {
                resultVector.push_back({nums[i], nums[left], nums[right]});
                while (left < right && nums[left] == nums[left + 1])
                    ++left;
                while (left < right && nums[right] == nums[right - 1])
                    --right;
                ++left;
                --right;
            } else if (sum < 0) {
                ++left;
            } else {
                --right;
            }
        }
    }

    // delete [0,0,0] elements (wont be needed because we actually dont include the 0,0,0 element.)
    // resultVector.erase(std::remove_if(resultVector.begin(), resultVector.end(), [](const std::vector<int>& vec) {
    //     return (vec == std::vector<int>{0,0,0});
    // }), resultVector.end());

    return resultVector;
}

std::vector<std::string> LeetCode::CartesianProduct(std::string digits) {
    // TODO: correct current structures to make usage of the recursion in one more layer if possible
    std::vector<int> convertedDigits; 
    for (char c: digits) {
        int d = c - '0';
        convertedDigits.push_back(d);
    }

    std::vector<std::string> cartesianProduct;
    std::vector<std::string> buttonLetters = {"", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};

    while(convertedDigits.size() != 0) { // vector
        std::string letters = buttonLetters[convertedDigits.size()-1];
        std::vector<char> vectorLetters(letters.begin(), letters.end());

        cartesianProduct = recursiveCartesian(vectorLetters, cartesianProduct);
        convertedDigits.pop_back();
    }

    return cartesianProduct;
}

std::vector<std::string> recursiveCartesian(std::vector<char> vTmp, std::vector<std::string> vRst) {
    std::vector<std::string> result;
        while(vTmp.size() != 0) {
            int vTmpIndex = vTmp.size()-1;
            for(std::string string: vRst) {
                string.push_back(vTmp[vTmpIndex]);
            }
            vTmp.pop_back();
        }
        return vRst; // better not use a pointer because that would mean the value should not be changed but rather used as a singleton?
    }
