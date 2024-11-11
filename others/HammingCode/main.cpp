//
// Created by maria on 04.09.2024.
//

#include "lib/HammingCode.cpp"

int main() {

    std::array<int, 16> myArray = {0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0};

    Hammingcode hc = Hammingcode(myArray);

    return 0;
}