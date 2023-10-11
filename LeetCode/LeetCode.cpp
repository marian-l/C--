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
        if(!strs[i].starts_with(word))
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
// basecase: empty list, only one digit
    std::vector<std::string> cartesianProduct = {""};

    if (digits.empty()) {
        return {}; // Clang-Tidy: Avoid repeating the return type from the declaration; use a braced initializer list instead
    }

    std::vector<std::string> buttonLetters = {"", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};

    for(char c: digits) {
        std::vector<std::string> tempProduct;
        for (char d: buttonLetters[c - '0']) {
            for (auto s: cartesianProduct) {
                tempProduct.push_back(s + d);
            }
        }
        cartesianProduct.swap(tempProduct);
    }
    return cartesianProduct;
}

int LeetCode::search(std::vector<int>& nums, int target) {
    // pivot point is the index of the largest number or the index of the lowest number - 1
    // from start to pivot point the value of numbers is ascending
    // from pivot-point+1 to n-1 the value of numbers is also ascending

    int index = 0;

    if(nums.empty()) {
        return -1;
    } else if(nums.size() <= 3) {
        for (int i: nums) {
            if(i == target) {
                return index;
            }
            index++;
        }
        return -1;
    }

    int right = nums.size() - 1; int left = 0; int middle = (right + left) / 2;

    // array is still sorted
    if(nums[left] < nums[middle] && nums[middle] < nums[right]) {
        // binary search for target
        while(left <= right) {

            if(target == nums[middle]) {
                return middle;
            }

            if(target < nums[middle]) {
                right = middle - 1;
            } else {
                left = middle + 1;
            }
            middle = (right + left) / 2;
        }
    // finding the pivot point in rotated array
    } else {
        // binary search pivot element
        while(left <= right) {

            // the pivot is in the middle
            if(nums[middle] > nums[middle + 1]) {
                index = middle;
                break;

                // the pivot is one left of the middle
            }
            // else if(nums[middle - 1] > nums[middle]) {
            //     index = middle - 1;
            //     break;
            // }

            if(nums[left] > nums[middle]) {
                // pivot point is to the left
                right = middle - 1;
            } else {
                // pivot point is to the right
                left = middle + 1;
            }

            middle = (right + left) / 2;
        }

        // now perform binary search on one of the subarrays
        if(target >= nums[0]) {
            // refresh the binary search parameters
            right = index; left = 0; middle = (right + left) / 2;

            // binary search on the left array because the left array contains the bigger numbers
            while(left <= right) {
                if(target == nums[middle]) {
                    return middle;
                }

                if(target < nums[middle]) {
                    right = middle - 1;
                } else {
                    left = middle + 1;
                }
                middle = (right + left) / 2;
            }
        } else {
            // refresh the binary search parameters
            right = nums.size() - 1; left = index+1; middle = (right + left) / 2;

            // binary search on the right array because the right array contains the smaller numbers
            while(left <= right) {
                if(target == nums[middle]) {
                    return middle;
                }

                if(target < nums[middle]) {
                    right = middle - 1;
                } else {
                    left = middle + 1;
                }
                middle = (right + left) / 2;
            }
        }
    }

    // in case the value is not found
    return -1;
}

/*
 * Programm nutzt Binärsuche, um bei einem sortierten Array das Target zu finden, solange das Array größer als 3 Elemente ist (seltsame BugCases).
 * Die Binärsuche geht so: Die linke Grenze liegt am Anfang, die rechte Grenze am Ende des Arrays. Die Mitte wird gebildet.
 * Es muss geprüft werden, ob die Mitte das Element ist, das gesucht wird und gegebenenfalls dieses Element zurückgegeben werden.
 * Wenn das Element nicht in der Mitte liegt, können wir überprüfen, ob das Element in der Mitte größer oder kleiner als das Target ist und dementsprechend in einem
 * sortierten Array die linke oder rechte Hälfte durchsuchen. Die linke bzw rechte Grenze wird dann angepasst auf Mitte+-1, da wir die Mitte ja bereits geprüft haben.
 * Die Mitte wird neu berechnet. Dann wird das ganze wiederholt.
 *
 * Haben wir den Fall, dass das Array rotiert worden ist, suchen wir zuerst das Pivotelement, also das Element, dessen Nachfolger kleiner ist als es selbst
 * (das Array wurde ja vorher sortiert). Das Pivotelement ist entweder in der Mitte, oder eben nicht, das zeigen die Testcases. Mit dem Pivotelement können wir jetzt
 * beurteilen, welches der beiden Subarrays wir durchsuchen. Wir überprüfen, ob das linke Element des ganzen Arrays kleiner-gleich dem Target ist, was ja bedeuten
 * würde, dass die kleineren Zahlen des rechten Subarrays alle zu klein wären.
 *
 * Die Edge Cases habe ich lazy abgedeckt, wenn dass Array klein ist, dann kann man es auch händisch durchsuchen. Leere Arrays und welche,
 * die das Element nicht enthalten, sind auch abgedeckt.
 *
 * left < middle && middle < right
 * [0,1,2,4,5,6,7]
 *
 * left < middle && right < middle
 * [1,2,4,5,6,7,0]
 * [2,4,5,6,7,0,1]
 * [4,5,6,7,0,1,2]
 *
 * left > middle && right > middle
 * [5,6,7,0,1,2,4]
 * [6,7,0,1,2,4,5]
 * [7,0,1,2,4,5,6]
 *
 * */



