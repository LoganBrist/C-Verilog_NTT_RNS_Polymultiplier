/*
#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "general_functions.h"
#include "RNS.h"
#include "NTT.h"
#include "REDC.h"
#include <map>

class processor
{
	private:
		RNS rns_system;
		NTT ntt_system;
		std::vector<REDC> redc_modules;   

	public:
		int min_modulus;
		std::vector<int> rns_moduli;
		int mod_multiply(int A, int B);
		processor(int minimum_modulus, vector<int> rns_moduli, int polynomial_length);

	//std::vector<int>

};

*/