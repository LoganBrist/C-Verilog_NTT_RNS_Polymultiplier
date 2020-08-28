#pragma once
#include "BigIntLibrary/BigIntegerLibrary.hh"

class REDC
{

public:
	BigUnsigned R       = 0;
	BigUnsigned R_inv   = 0;
	int R_bits  = 0;
	BigUnsigned K       = 0;
	BigUnsigned modulus = 0;
	bool modIsEven = false;

	bool test();
	BigUnsigned modmult(BigUnsigned A, BigUnsigned B);
	REDC(BigUnsigned mod);

private:
	BigUnsigned findR(BigUnsigned mod);
	BigUnsigned findK(BigUnsigned R, BigUnsigned R_inv, BigUnsigned mod);
	BigUnsigned gcd(BigUnsigned A, BigUnsigned B);
	bool areCoprimes(BigUnsigned A, BigUnsigned B);
	BigUnsigned reduce(BigUnsigned val, BigUnsigned mod);
};

