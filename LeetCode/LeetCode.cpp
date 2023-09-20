//
// Created by maria on 18.09.2023.
//

#include "LeetCode.h"

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
    int shortestWordSize = 128;
    std::string shortestWord = "";
    for(std::string word : strs) {
        if(word.size() < shortestWordSize) {
            shortestWordSize = word.size();
            shortestWord = word;
        }
    }

    // kürzestes Wort in der Mitte teilen, linke Hälfte als Präfix zum Vergleich benutzen
    // Wort für Wort abgleichen
        // wenn ein Wort nicht den Präfix enthält:
            // die Mitte um 1 verkleinern,
            // neuen Präfix extrahieren
            // von vorne beginnen
        // wenn jedes Wort den Präfix enthält:
            // Präfix speichern
            // Mitte um 1 erhöhen
            // Flag setzen

    int prefixLength = shortestWordSize / 2;
    std::string prefix = shortestWord.substr(0, prefixLength);
    bool _continue = true;
    bool setContinue = false;

        for (std::string word : strs) {
            if (word.substr(0, prefixLength) != prefix) {
                prefixLength -= 1;
                prefix = shortestWord.substr(0, prefixLength);
            } else {
                prefixLength += 1;
                prefix = shortestWord.substr(0, prefixLength);
            }
        }

    return prefix;
}
