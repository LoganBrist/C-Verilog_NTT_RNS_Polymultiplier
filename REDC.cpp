#include "REDC.h"
#include "general_functions.h"
#include <iostream>

using namespace std;

//////////////////////////////////////////////////////////////
// Reduction function
// returns (val * r^-1) mod n
//////////////////////////////////////////////////////////////
int REDC::reduce(int val,int mod) {
	//small overflow multiplication
	int s = (val * K) & (R-1); /* (val*K) mod R. R is a power of 2.
							   Mod 2^n is the same as keeping the lowest 
							   n bits, which is the same as bitwise AND
							   with 2^n-1 (i.e. num & R-1). I think this
							   is essentially a log2(R) x log2(R) multiplication
							   between size n numbers but with overflow 
							   allowed. 
							   */

	int t = val + s * mod;

	int u = t / R;

	if (u < mod)
		return u;
	else
		return u - mod;
}

//////////////////////////////////////////////////////////////
// Outer function
//
// R is an initialization parameter. It is a power of 2 to allow
// for bitshifts.
//////////////////////////////////////////////////////////////
int REDC::modmult(int A, int B, int mod) {
	// Convert to montgomery form
	int a = (A << R_bits) % mod;   //efficient multiply A by R
	int b = (B << R_bits) % mod;   //efficient multiply B by R
	 
	//main efficient montgomery reduction
	int c = reduce(a * b, mod); 

	// Convert to standard form
	int ans = (c * R_inv) % mod;

	return ans;
}

//////////////////////////////////////////////////////////////
// Test all multiplications up to mod x mod
//////////////////////////////////////////////////////////////
bool REDC::test() {
	bool error = false;
	for (int i = 0; i < modulus; i++) {
		for (int j = 0; j < modulus; j++) {
			if (modmult(i, j, modulus) != (i * j % modulus)) {
				cout << i << " x " << j << " != " << modmult(i, j, modulus) << " = " << (i * j % modulus) << endl;
				error = true;
			}
		}
	}

	if (error)
		cout << "REDC: Errors with modulus " << modulus << "." << endl;
	else
		cout << "REDC: No errors with modulus " << modulus << "." << endl;
	return error;
}


//////////////////////////////////////////////////////////////
// Initialization parameters
//////////////////////////////////////////////////////////////
int REDC::findR(int mod) {
	int R = 1;
	// R should be greater, coprime with mod, and power of two 
	while (not (areCoprimes(R, mod) && R > mod)) {
		R *= 2; 
	}
	return R;
}

int REDC::findK(int R, int R_inv, int mod) {
	int K = (R * R_inv - 1) / mod;
	return K;
}

// Coprime test
int REDC::gcd(int A, int B) {
	if (B == 0)
		return A;
	return gcd(B, A % B);
}

bool REDC::areCoprimes(int A, int B) {
	return (gcd(A, B) == 1);
}

//////////////////////////////////////////////////////////////
// Error
//////////////////////////////////////////////////////////////
void isPrimeException(int val) {
	if (factorize(val).size() != 1) {
		cout << "REDC Error: Modulus " << val << " is not prime." << endl;
		throw;
	}
}

//////////////////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////////////////
REDC::REDC(int mod) {
	isPrimeException(mod);
	modulus = mod;
	R       = findR(mod);
	R_inv   = mod_inverse(R, mod);
	R_bits  = log2(R);
	K       = findK(R, R_inv, mod);
	mod     = mod;
}
