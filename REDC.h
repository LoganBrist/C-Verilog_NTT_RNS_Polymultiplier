#pragma once
class REDC
{

public:
	int R;
	int R_inv;
	int R_bits;
	int K;
	int modulus;

	bool test();
	int reduce(int val, int mod);
	int modmult(int A, int B, int mod);
	int findR(int mod);
	int findK(int R, int R_inv, int mod);
	int gcd(int A, int B);
	bool areCoprimes(int A, int B);
	REDC(int mod);

private:

};

