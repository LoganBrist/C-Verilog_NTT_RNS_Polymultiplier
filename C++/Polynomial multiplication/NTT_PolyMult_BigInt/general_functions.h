#pragma once
#ifndef __genfunc__   // if general functions hasn't been included yet...
#define __genfunc__

#include <vector>
#include <string>
#include "BigIntLibrary/BigIntegerLibrary.hh"

bool vectorsAreEqual(std::vector<BigUnsigned> a, std::vector<BigUnsigned> b);

BigUnsigned product(std::vector<BigUnsigned> vec);

BigUnsigned getRandomBigUnsigned(BigUnsigned range);

std::vector<BigUnsigned> hadamard_product(std::vector<BigUnsigned> a, std::vector<BigUnsigned> b, BigUnsigned moduli);

std::vector<BigUnsigned> sample_polynomial(BigUnsigned length, BigUnsigned range);
std::vector<std::vector<BigUnsigned>> sample_RNS_polynomial(int length, std::vector<BigUnsigned> moduli, BigUnsigned range);
std::vector<BigUnsigned> get_random_RNS_val(std::vector<BigUnsigned> moduli, BigUnsigned range);

std::vector<BigUnsigned> bitReverse(std::vector<BigUnsigned> A);
std::vector<std::vector<BigUnsigned>> bitReverse_rns(std::vector<std::vector<BigUnsigned>> A);

std::vector<BigUnsigned> factorize(BigUnsigned n);

std::vector<BigUnsigned> mult_by_power(std::vector<BigUnsigned> vec, BigUnsigned val, BigUnsigned modulus);

BigUnsigned mod_inverse(BigUnsigned A, BigUnsigned mod);

BigUnsigned pow_mod(BigUnsigned base, BigUnsigned ex, BigUnsigned mod);

BigUnsigned sqrt_mod(BigUnsigned A, BigUnsigned mod);



void printVector(std::vector<BigUnsigned> list, std::string name = "", bool printFullVector = false);

void saveValToTextfile(BigUnsigned val, std::string savename = "");
void saveVectorToTextfile(std::vector<BigUnsigned> vec, std::string savename = "",bool writeOnOneLine = false, int val_width = 5);
void saveVectorVectorToTextfile(std::vector<std::vector<BigUnsigned>> vec, std::string savename = "");

BigUnsigned gcd(BigUnsigned A, BigUnsigned B);

bool isCoprimeToVector(BigUnsigned A, std::vector<BigUnsigned> B);

bool areCoprimes(BigUnsigned A, BigUnsigned B);

bool isPrime(BigUnsigned A);

std::vector<BigUnsigned> zero_pad(std::vector<BigUnsigned> A);

#endif
