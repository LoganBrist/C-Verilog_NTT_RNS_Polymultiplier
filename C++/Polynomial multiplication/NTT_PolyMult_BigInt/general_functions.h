#pragma once
#ifndef __genfunc__   // if general functions hasn't been included yet...
#define __genfunc__

#include <vector>
#include <string>
#include "BigIntLibrary/BigIntegerLibrary.hh"

bool vectorsAreEqual(std::vector<BigUnsigned> a, std::vector<BigUnsigned> b);

BigUnsigned product(std::vector<BigUnsigned> vec);

std::vector<BigUnsigned> hadamard_product(std::vector<BigUnsigned> a, std::vector<BigUnsigned> b, BigUnsigned moduli);

std::vector<BigUnsigned> sample_polynomial(BigUnsigned N, BigUnsigned q);

std::vector<BigUnsigned> bitReverse(std::vector<BigUnsigned> A);

std::vector<BigUnsigned> factorize(BigUnsigned n);

std::vector<BigUnsigned> mult_by_power(std::vector<BigUnsigned> vec, BigUnsigned val, BigUnsigned modulus);

BigUnsigned mod_inverse(BigUnsigned A, BigUnsigned mod);

BigUnsigned pow_mod(BigUnsigned base, BigUnsigned ex, BigUnsigned mod);

BigUnsigned sqrt_mod(BigUnsigned A, BigUnsigned mod);

void printVector(std::vector<BigUnsigned> list, std::string name = "", bool printFullVector = false);

BigUnsigned gcd(BigUnsigned A, BigUnsigned B);

bool isCoprimeToVector(BigUnsigned A, std::vector<BigUnsigned> B);

bool areCoprimes(BigUnsigned A, BigUnsigned B);

bool isPrime(BigUnsigned A);

std::vector<BigUnsigned> zero_pad(std::vector<BigUnsigned> A);

#endif