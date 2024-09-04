//
// Created by maria on 26.08.2024.
//
#include <iostream>
#include <vector>
// import std;

// the '=' operator can lead to conversion as seen in i1 and i2

int i1 = 7.8;

// int i2 {7.8}; // narrowing conversion and not possible with this syntax

double d {8.9};

// generic programming
auto a2 {'s'};
auto a1 {1};

double sqrt(double var) {
    return var*var;
}

// constants
constexpr int dmv = 17; // constant
const double sqv = sqrt(dmv);  // constant, possibly with runtime computation

// constexpr double sqv2 = sqrt(dmv); // sqrt(dmv) is not a constant expression
//      call to non-'constexpr' function 'double sqrt(double)' 27 | constexpr double sqv2 = sqrt(dmv); // sqrt(dmv) is not a constant expression

const int const_int_var = 17;
int int_var = 17;

constexpr double square(double x) { return x*x; } // function must be defined as constexpr

constexpr double max1 = 1.4*square(17);       // OK: 1.4*square(17) is a constant expression

constexpr double max2 = 1.4*square(const_int_var);      // error: var is not a constant, so square(var) is not a constant
const double max3 = 1.4*square(const_int_var);             // OK: may be evaluated at run time

// constexpr double max4 = 1.4*square(int_var);      // error: var is not a constant, so square(var) is not a constant
//      error: the value of 'int_var' is not usable in a constant expression constexpr double max4 = 1.4*square(int_var);

const double max5 = 1.4*square(int_var);             // OK: may be evaluated at run time

// arrays, pointer
char char_array_6[6] = {'p', '2', '3', '4'};

char* p = &char_array_6[3];

// in expression:
// * means contents of
char x = *p;

char& y = *p;

int main() {
    std::cout << &char_array_6[3] << '\n';
    std::cout << char_array_6[3] << '\n';
    std::cout << x << '\n';
    std::cout << y << '\n';
}

// modify argument by using the pointer &
void sort(std::vector<double>& v);

// not modify an argument but not copy
double sum(const std::vector<double>&);

// T a[n]       // a is an array of T's
// T f(A)       // f is a function taking type A and returning type T

// nullpointer:
// - notion of "no object available"
// - no object to point to
double* prt = nullptr



