#pragma once
class REDC
{

public:
	int R       = 0;
	int R_inv   = 0;
	int R_bits  = 0;
	int K       = 0;
	int modulus = 0;
	int modIsEven = false;

	bool test();
	int modmult(int A, int B);
	REDC(int mod);

private:
	int findR(int mod);
	int findK(int R, int R_inv, int mod);
	int gcd(int A, int B);
	bool areCoprimes(int A, int B);
	int reduce(int val, int mod);
};

