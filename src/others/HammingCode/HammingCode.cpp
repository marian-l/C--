//
// Created by maria on 01.09.2024.
//
#include <list>
#include <iostream>
#include <array>
#include <algorithm>

// 1 1 0 0      ? ? ? 0
// 1 1 1 1      ? 1 1 1
// 1 0 1 0      ? 0 1 0
// 0 1 1 0      0 1 1 0

// input must be split into parts of 11

// 1st bit: parity bit of all bits
// 2nd bit: all the odds
// 3rd bit: right sided
// 4th bit: row 2 and 4
// 8th bit: row 3 and 4

// 16th bit
// 32th bit

class Hammingcode {
    using block = std::array<int, 16>;

    std::list<block> codeBlocks;

private:
    void split_every_16_array(const std::array<int, 16>& array) {
        int numberOfBlocks = array.size() / 16;
        block code;

        for (int i = 0; i < numberOfBlocks; ++i) {
            std::fill(code.begin(), code.end(), 2);

            // base case of full blocks
            int splitLength = 16;

            // for the remainder
            if (i*16 > array.size()) {
                splitLength = array.size() % 16;
            }

            for (int j = 0; j < splitLength; ++j) {
                code[j] = array[(i*16) + j]; // i.e. 0 * 16 + 0 = 0; 0 * 16 + 1 = 0;
            }

            codeBlocks.insert(codeBlocks.end(), 1, code);
        }
    }

    std::array<int, 5> parity_check(block block) {
            // loop over the parity bits
            // for (int i = 1; i <= 8; i*=2) { // way too complicated
            // }

            std::array<int, 5> result{};
            
            int expectedParity = 0;

            // first parity bit
            for (int i = 1; i < 16; i+=2) {
                if (block[i] == 1) expectedParity +=1;
            }
            expectedParity = expectedParity % 2;
            
            if (block[1] == expectedParity) {
                std::cout << "Wrong parity bit in position 1" << "\n";
                result[1] = 1;
                // result.insert(result.end(), 1);
            } else {
                result[1] = 0;
                // result.insert(result.end(), 0);
            }
            
            // second parity bit
            std::list<int> secondParityList = {2, 3, 6, 7, 10, 11, 14, 15};
            expectedParity = 0;

            for (int i : secondParityList) {
                if (block[i] == 1) expectedParity +=1;
            }

            expectedParity = expectedParity % 2;

            if (block[1] == expectedParity) {
                std::cout << "Wrong parity bit in position 2" << "\n";
                result[2] = 1;
                // result.insert(result.end(), 1);
            } else {
                result[2] = 0;
                // result.insert(result.end(), 0);
            }

            // 4th parity bit
            std::list<int> fourthParityList = {4, 5, 6, 7, 12, 13, 14, 15};
            expectedParity = 0;

            for (int i : secondParityList) {
                if (block[i] == 1) expectedParity +=1;
            }

            expectedParity = expectedParity % 2;

            if (block[1] == expectedParity) {
                std::cout << "Wrong parity bit in position 4" << "\n";
                result[3] = 1;
                // result.insert(result.end(), 1);
            } else {
                result[3] = 0;
                // result.insert(result.end(), 0);
            }

            // 8th parity bit
            expectedParity = 0;

            for (int i = 8; i <= 15 ; ++i) {
                if (block[i] == 1) expectedParity +=1;
            }

            expectedParity = expectedParity % 2;

            if (block[1] == expectedParity) {
                std::cout << "Wrong parity bit in position 8" << "\n";
                result[4] = 1;
                // result.insert(result.end(), 1);
            } else {
                result[4] = 0;
                // result.insert(result.end(), 0);
            }

            // parity bit of whole block
            expectedParity = 0;

            for (int i : block) {
                if (block[i] == 1) expectedParity += 1;
            }

            expectedParity = expectedParity % 2;

            if (block[0] == expectedParity) {
                std::cout << "Wrong parity bit in position 0" << "\n";
                result[0] = 1;
                // result.insert(result.end(), 1);
            } else {
                result[0] = 0;
                // result.insert(result.end(), 0);
            }

            return result;

    }

public:
    std::list<int> find_faulty_bits(block faultyBlock, std::array<int, 5> positions) {
        for (int i = 0; i < 5; ++i) {
            if (positions[i] == 1) {

            }
        }
        return std::list<int>{};
    }

    Hammingcode(std::array<int, 16>& message) {

        split_every_16_array(message);

        for (auto a: codeBlocks) {
            for (auto v: a) {
                std::cout << v;
            }
            std::cout << "\n";
        }

        // check all the 16bit blocks
        for (block _block : codeBlocks) {
            std::array<int, 5> result = parity_check(_block);
            if (std::find(result.begin(), result.end(),1) != result.end()) {
                std::list<int> faultyBitPositions = find_faulty_bits(_block, result);
            }
        }
    }
};