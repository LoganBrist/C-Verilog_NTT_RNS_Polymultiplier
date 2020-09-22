#pragma once
#include <vector>
#include "RNS.h"

class NTT
{
	public:

		BigUnsigned min_mod;     // Minimum modulus. Possibly recomputed to be prime and saved as "modulus"
		BigUnsigned modulus;     // Used modulus.
		BigUnsigned w_n;         // nth root of unity when reduced.
		BigUnsigned w_n_inv;     // modular inverse of w_n.
		BigUnsigned phi;         // phi^2 = w_n
		BigUnsigned phi_inv;

		RNS rns;

		BigUnsigned vec_length;  //  Size of the transform and "n" in the nth root of unity.
		std::vector<BigUnsigned> phi_table; //bit reversed powers of phi
		
		NTT(BigUnsigned vector_length, BigUnsigned minimum_modulus, RNS RNS_system, bool modulusIsPrimeIPromise = false);   //constructor
		
		static BigUnsigned new_modulus(BigUnsigned vec_length, BigUnsigned min_modulus);
		std::vector<BigUnsigned> calculate(std::vector<BigUnsigned> A, bool inverse = false);
		std::vector<std::vector<BigUnsigned>> calculate_rns(std::vector<std::vector<BigUnsigned>> A, bool inverse = false);
		std::vector<BigUnsigned> stupidcalculate(std::vector<BigUnsigned> A, bool inverse = false);
		static BigUnsigned find_root_of_unity2(BigUnsigned vec_length, BigUnsigned modulus);
		void NTT_test(int n_tests);
		std::vector<BigUnsigned> static solveParameters(BigUnsigned vector_length, BigUnsigned mod, bool modulusIsPrimeIPromse = false);
		void printParameters();

	private:
		static bool is_generator(BigUnsigned val, BigUnsigned totient, BigUnsigned mod);
		static BigUnsigned find_generator(BigUnsigned totient, BigUnsigned mod);
		BigUnsigned find_root_of_unity(BigUnsigned vec_length, BigUnsigned modulus);
		std::vector<BigUnsigned> constant_vector(BigUnsigned length, BigUnsigned val);
		std::vector<BigUnsigned> mult_by_power(std::vector<BigUnsigned> in, BigUnsigned val, BigUnsigned modulus);
		std::vector<BigUnsigned> static generate_phi_table(BigUnsigned n, BigUnsigned w_n, BigUnsigned modulus);
		void NTT::butterfly(BigUnsigned* left_val, BigUnsigned* right_val);
		
};

void save_twiddle_table(char* savename, BigUnsigned NTT_size, BigUnsigned w_n, BigUnsigned mod);

//void massNTT(int n_tests, BigUnsigned length, BigUnsigned minimum_modulus, std::vector<BigUnsigned> rns_moduli);
//void massSolveParameters(std::vector<BigUnsigned> vector_lengths, std::vector<BigUnsigned> moduli);
//void massPolynomialMultiply(int n_tests, BigUnsigned length, BigUnsigned minimum_modulus, std::vector<BigUnsigned> rns_moduli);

