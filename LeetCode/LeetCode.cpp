//
// Created by maria on 18.09.2023.
//

#include "LeetCode.h"
#include <climits>

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
    }

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
        if(!strs[i].starts_with(word))
            return false;
    }
    return true;
}
