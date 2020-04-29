#pragma once
#include <vector>
#include "RNS.h"

class NTT
{
	public:

		int min_mod;     // Minimum modulus. Possibly recomputed to be prime and saved as "modulus"
		int modulus;     // Used modulus.
 		int w_n;         // nth root of unity when reduced.
		int w_n_inv;     // modular inverse of w_n.
		int phi;         // phi^2 = w_n
		int phi_inv;

		RNS rns;

		int vec_length;  //  Size of the transform and "n" in the nth root of unity.
		std::vector<int> phi_table; //bit reversed powers of phi
		
		NTT(int vector_length, int minimum_modulus, RNS RNS_system);   //constructor
		
		int new_modulus(int vec_length, int min_modulus);
		std::vector<int> calculate(std::vector<int> A, bool inverse = false);
		std::vector<int> calculate2(std::vector<int> A, bool inverse = false);
		std::vector<int> calculate3(std::vector<int> A, bool inverse = false);
		std::vector<int> calculate4(std::vector<int> A, bool inverse = false);
		std::vector<int> calculate5(std::vector<int> A, bool inverse = false);
		std::vector<int> stupidcalculate(std::vector<int> A, bool inverse = false);
		int find_root_of_unity2(int vec_length, int modulus);
		void printParameters();

	private:
		std::vector<int> factorize(int n);
		int pow_mod(int base, int ex, int mod);
		int sqrt_mod(int num, int mod);
		int mod_inverse(int w_n, int M);
		bool is_generator(int val, int totient, int mod);
		int find_generator(int totient, int mod);
		int find_root_of_unity(int vec_length, int modulus);
		//int find_root_of_unity2(int vec_length, int modulus);
		std::vector<int> bitReverse(std::vector<int> A);
		std::vector<int> hadamard_product(std::vector<int> a, std::vector<int> b, int moduli);
		std::vector<int> constant_vector(int length, int val);
		std::vector<int> mult_by_power(std::vector<int> in, int val, int modulus);
		std::vector<int> generate_phi_table(int n, int w_n, int modulus);
		
};

