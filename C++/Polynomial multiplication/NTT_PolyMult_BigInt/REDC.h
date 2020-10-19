#pragma once
#include "BigIntLibrary/BigIntegerLibrary.hh"

class REDC
{

public:
	// Montgomery
	BigUnsigned R       = 0;
	BigUnsigned R_inv   = 0;
	int R_bits  = 0;
	//BigUnsigned K       = 0;
	BigUnsigned modulus = 0;
	bool modIsEven = false;

	// Barrett
	BigUnsigned M = 0;
	int K = 0;

	bool test();
	BigUnsigned modmult(BigUnsigned A, BigUnsigned B);
	BigUnsigned modmult_barrett(BigUnsigned A, BigUnsigned B);
	void getBarrettConstants(BigUnsigned mod);
	void modmultTest_barrett(int n_tests);
	REDC(BigUnsigned mod);

private:
	BigUnsigned findR(BigUnsigned mod);
	BigUnsigned findK(BigUnsigned R, BigUnsigned R_inv, BigUnsigned mod);
	BigUnsigned gcd(BigUnsigned A, BigUnsigned B);
	bool areCoprimes(BigUnsigned A, BigUnsigned B);
	BigUnsigned reduce(BigUnsigned val, BigUnsigned mod);
};

