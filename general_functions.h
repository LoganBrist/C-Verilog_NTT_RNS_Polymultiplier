#pragma once
#ifndef __genfunc__   // if general functions hasn't been included yet...
#define __genfunc__

#include <vector>
#include <string>

bool compareVectors(std::vector<int> a, std::vector<int> b);

std::vector<int> hadamard_product(std::vector<int> a, std::vector<int> b, int moduli);

std::vector<int> sample_polynomial(int N, int q);

std::vector<int> bitReverse(std::vector<int> A);

std::vector<int> factorize(int n);

int mod_inverse(int A, int mod);

int pow_mod(int base, int ex, int mod);

int sqrt_mod(int A, int mod);

void printVector(std::vector<int> list, std::string name = "");


#endif