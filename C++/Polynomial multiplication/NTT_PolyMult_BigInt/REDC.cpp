#include "REDC.h"
#include "general_functions.h"
#include <iostream>
#include "BigIntLibrary/BigIntegerLibrary.hh"

using namespace std;

//////////////////////////////////////////////////////////////
// Reduction function
// returns (val * r^-1) mod n
//////////////////////////////////////////////////////////////
BigUnsigned REDC::reduce(BigUnsigned val, BigUnsigned mod) {
	//small overflow multiplication
	BigUnsigned s = (val * K) & (R-1); /* (val*K) mod R. R is a power of 2.
							   Mod 2^n is the same as keeping the lowest 
							   n bits, which is the same as bitwise AND
							   with 2^n-1 (i.e. num & R-1). I think this
							   is essentially a log2(R) x log2(R) multiplication
							   between size n numbers but with overflow 
							   allowed. 
							   */

	BigUnsigned t = val + s * mod;

	BigUnsigned u = t / R;

	if (u < mod)
		return u;
	else
		return u - mod;
}

//////////////////////////////////////////////////////////////
// Outer function\
//
// R is an initialization parameter. It is a power of 2 to allow
// for bitshifts.

// Montgomery reduction is only better when the same modulus is reused
// 
//////////////////////////////////////////////////////////////
BigUnsigned REDC::modmult(BigUnsigned A, BigUnsigned B) {
	// Do not perform montgomery reduction
	if (modIsEven)
		return (A * B) % modulus;   //In hardware use barret reduction. Or simple bitshift for powers of 2

	// Convert to montgomery form
	BigUnsigned a = (A << R_bits) % modulus;   //efficient bitshift multiply A by R
	BigUnsigned b = (B << R_bits) % modulus;   //efficient bitshift multiply B by R
	 
	//main efficient montgomery reduction
	BigUnsigned c = reduce(a * b, modulus);

	// Convert to standard form
	BigUnsigned ans = (c * R_inv) % modulus;

	return ans;
}

//////////////////////////////////////////////////////////////
// Barrett Multiplication
//
//

// 
//////////////////////////////////////////////////////////////
BigUnsigned REDC::modmult_barrett(BigUnsigned A, BigUnsigned B) {
	BigUnsigned Z = A * B;
	BigUnsigned T = (Z * M) >> K;
	Z = Z - (T * modulus);

	if (Z >= modulus)
		Z = Z - modulus;
	return Z;
}

void REDC::getBarrettConstants(BigUnsigned mod) {
	BigUnsigned M_temp = 0;
	int K_temp = 0;

	BigUnsigned Q = mod;
	BigUnsigned P = pow(BigUnsigned(2), BigUnsigned(K_temp));


	while (P * Q - P / Q * pow(Q, 2) >= P) {
		K_temp++;
		P = pow(BigUnsigned(2), BigUnsigned(K_temp));
	}

	M_temp = pow(BigUnsigned(2), BigUnsigned(K_temp)) / Q;
	cout << "K: " << K_temp << " M: " << M_temp << endl;

	M = M_temp;
	K = K_temp;
}

void REDC::modmultTest_barrett(int n_tests) {
	int n_correct = 0;

	cout << endl << endl << "Barrett Multiplication Test: " << endl;
	getBarrettConstants(modulus);

	for (int i = 0; i < n_tests; i++) {
		BigUnsigned A = getRandomBigUnsigned(modulus);
		BigUnsigned B = getRandomBigUnsigned(modulus);

		BigUnsigned res = modmult_barrett(A,B);

		BigUnsigned ans = (A * B) % modulus;

		if (res == ans) {
			//cout << "Correct." << endl;
			n_correct++;
		}
		else
			cout << "Incorrect." << endl;
	}

	cout << endl << n_correct << "/" << n_tests << " tests correct." << endl << endl;
}
//////////////////////////////////////////////////////////////
// Test all multiplications up to mod x mod
//////////////////////////////////////////////////////////////
bool REDC::test() {
	bool error = false;
	for (BigUnsigned i = 0; i < modulus; i++) {
		for (BigUnsigned j = 0; j < modulus; j++) {
			if (modmult(i, j) != (i * j % modulus)) {
				cout << i << " x " << j << " != " << modmult(i, j) << " = " << (i * j % modulus) << endl;
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
BigUnsigned REDC::findR(BigUnsigned mod) {
	BigUnsigned R = 1;
	// R should be greater, coprime with mod, and power of two 
	while ( !(areCoprimes(R, mod) && R > mod)) {
		R *= 2; 
	}
	return R;
}

BigUnsigned REDC::findK(BigUnsigned R, BigUnsigned R_inv, BigUnsigned mod) {
	BigUnsigned K = (R * R_inv - 1) / mod;
	return K;
}

// Coprime test
BigUnsigned REDC::gcd(BigUnsigned A, BigUnsigned B) {
	if (B == 0)
		return A;
	return gcd(B, A % B);
}

bool REDC::areCoprimes(BigUnsigned A, BigUnsigned B) {
	return (gcd(A, B) == 1);
}

//////////////////////////////////////////////////////////////
// Error
//////////////////////////////////////////////////////////////
bool isOdd(BigUnsigned val) {
	return (val % 2 != 0); 
	}

//////////////////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////////////////
REDC::REDC(BigUnsigned mod) {
	// Mod must be odd to use montgomery reduction.
	//(It allows R and mod to be coprime while R is a power of 2)
	if (isOdd(mod)) {
		R      = findR(mod);
		R_inv  = mod_inverse(R, mod);
		R_bits = R.bitLength() - 1;
		//K      = findK(R, R_inv, mod);
	}
	else {
		R         = mod;
		modIsEven = true;
		cout << "Montgomery reduction not used for even modulus " << mod << "." << endl;
	}

	modulus = mod;
}
