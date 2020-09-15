/*
#include "processor.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "general_functions.h"
#include "RNS.h"
#include "NTT.h"
#include "REDC.h"
#include <map>

using namespace std;
int processor::mod_multiply(int A, int B) {
	return 0;
}

processor::processor(int minimum_modulus, vector<int> rns_moduli, int polynomial_length) {
	RNS rns(rns_moduli);
	NTT ntt(polynomial_length, minimum_modulus, rns);
	rns_system = rns;
	ntt_system = ntt;
	min_modulus = minimum_modulus;
}
*/