#include "RNS.h"
#include <vector>
#include "general_functions.h"
#include <iostream>
#include "REDC.h"

using namespace std;


///////////////////////////////////////////////////////////////////////////////
// Determine optimal moduli by power of 2 rule //leave outside of class
///////////////////////////////////////////////////////////////////////////////
vector<int> generateModuli(int size, int mod) {
    vector<int> A;
    return A;
}

///////////////////////////////////////////////////////////////////////////////
// RNS dynamic range given moduli
///////////////////////////////////////////////////////////////////////////////
int RNS::getDynamicRange() {
    int val = 1;
    for (int i = 0; i < n_moduli; i++) {
        val *= moduli[i];
    }
    return val;
}

///////////////////////////////////////////////////////////////////////////////
// Returns values where the RNS sytem is 1|0|0, 0|1|0, 0|0|1, etc.
///////////////////////////////////////////////////////////////////////////////
vector<int> RNS::getSingleResidues() {
    vector<int> vals;
    for (int i = 0; i < n_moduli; i++) {
        int interval = int(dR / moduli[i]);

        for (int num = interval; num <= dR; num += interval) {
            if (num % moduli[i] == 1) {
                vals.push_back(num);
            }
        }
    }
    return vals;
}

///////////////////////////////////////////////////////////////////////////////
// Converts integer to RNS representation
///////////////////////////////////////////////////////////////////////////////
vector<int> RNS::forwardConverter(int num) {
    vector<int> num_RNS;
    for (int i = 0; i < n_moduli; i++) {
        num_RNS.push_back(num % moduli[i]);
    }
    return num_RNS;
}

///////////////////////////////////////////////////////////////////////////////
// Converts RNS to integer representation
///////////////////////////////////////////////////////////////////////////////
int RNS::reverseConverter(vector<int> num_RNS) {
    int ret_val         = 0;
    for (int i = 0; i < n_moduli; i++) {
        ret_val += weights[i] * num_RNS[i];
        ret_val %= dR;  
    }

    return int(ret_val);
}

///////////////////////////////////////////////////////////////////////////////
// Returns addition of RNS vectors 
///////////////////////////////////////////////////////////////////////////////
vector<int> RNS::add_RNS(vector<int> A, vector<int> B) {
    vector<int> ret_val;
    for (int i = 0; i < n_moduli; i++) {
        ret_val.push_back((A[i] + B[i]) % moduli[i]);
    }
    return ret_val;
}

///////////////////////////////////////////////////////////////////////////////
// Returns subtraction of RNS vectors 
///////////////////////////////////////////////////////////////////////////////
vector<int> RNS::sub_RNS(vector<int> A, vector<int> B) {
    vector<int> ret_val;
    for (int i = 0; i < n_moduli; i++) {
        ret_val.push_back((A[i] - B[i] + moduli[i]) % moduli[i]);
    }
    return ret_val;
}

///////////////////////////////////////////////////////////////////////////////
// Returns modular multiplication of RNS vectors (no overflow protection)
///////////////////////////////////////////////////////////////////////////////
vector<int> RNS::mult_RNS(vector<int> A, vector<int> B) {
    vector<int> ret_val;
    for (int i = 0; i < n_moduli; i++) {
        //cout << A[i] << " * " << B[i] << " mod " << moduli[i] << " = " << (A[i] * B[i]) % moduli[i] << endl;
        
        //ret_val.push_back((A[i] * B[i]) % moduli[i]); //standard reduction
        ret_val.push_back(redc[i].modmult(A[i], B[i])); //montgomery reduction
    }

    return ret_val;
}

///////////////////////////////////////////////////////////////////////////////
// Returns addition of integers converting to and from RNS
///////////////////////////////////////////////////////////////////////////////
int RNS::add(int A, int B) {
    return reverseConverter(add_RNS(forwardConverter(A), forwardConverter(B)));
}

///////////////////////////////////////////////////////////////////////////////
// Returns subtraction of integers converting to and from RNS
///////////////////////////////////////////////////////////////////////////////
int RNS::sub(int A, int B) {
    return reverseConverter(sub_RNS(forwardConverter(A), forwardConverter(B)));
}

///////////////////////////////////////////////////////////////////////////////
// Returns multiplication of integers converting to and from RNS
///////////////////////////////////////////////////////////////////////////////
int RNS::mult(int A, int B) {
    return reverseConverter(mult_RNS(forwardConverter(A), forwardConverter(B)));
}

///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////

RNS::RNS(vector<int> mod) {
    moduli   = mod;
    n_moduli = mod.size();
    dR = getDynamicRange(); 
    weights = getSingleResidues();

    // create montgomery reduction module for each moduli
    for (int i = 0; i < mod.size(); i++) {
        REDC redc_module(mod[i]);
        redc.push_back(redc_module);
    }
}