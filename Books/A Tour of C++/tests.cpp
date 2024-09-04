//
// Created by maria on 26.08.2024.
//
#include <iostream>

bool accept()

{
    std::cout << "Do you want to proceed (y or n)?\n";

    char answer = 0;

    std::cin >> answer;

    if (answer == 'y') {
        // accept();
        return true;
    }
    return false;
}