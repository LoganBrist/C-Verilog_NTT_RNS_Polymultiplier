#include "RNS.h"
#include <vector>
#include "general_functions.h"
#include <iostream>
#include "REDC.h"
#include "BigIntLibrary/BigIntegerLibrary.hh"
#include <string>

using namespace std;


///////////////////////////////////////////////////////////////////////////////
// RNS dynamic range given moduli
///////////////////////////////////////////////////////////////////////////////
BigUnsigned RNS::getDynamicRange(vector<BigUnsigned> base) {
    return product(base);
}

///////////////////////////////////////////////////////////////
// Print RNS Value or Vector
// prints contents of an RNS vector in RNS or vector form
///////////////////////////////////////////////////////////////
void RNS::printRNSval(vector<BigUnsigned> val_rns, vector<BigUnsigned> base, bool printInIntform, string name) {
    cout << name;
    if (printInIntform == false) {
        for (int i = 0; i < val_rns.size(); i++) {
            cout << val_rns[i];
            if (i != val_rns.size() - 1)
                cout << "|";
        }
    }
    else {
        cout << reverseConverter(val_rns, base) << ' ';
    }
}
void RNS::printRNSvector(vector<vector<BigUnsigned>> list_rns, string name, vector<BigUnsigned> base, bool printInIntform, bool printFullVector)
{
    //If printing vector in integer form
    if (printInIntform == true) {
        vector<BigUnsigned> list = reverseConverter_polynomial(list_rns, base);
        printVector(list, name, printFullVector);
    }

    //If printing vector in RNS form
    if (printInIntform == false) {
        cout << name;

        int len = list_rns.size();

        //If printing RNS vector ends
        if (!printFullVector && (len > 6)) {
            printRNSval(list_rns[0]);
            cout << '|';
            printRNSval(list_rns[1]);
            cout << '|';
            printRNSval(list_rns[2]);
            cout << " ... ";
            printRNSval(list_rns[len - 3]);
            cout << '|';
            printRNSval(list_rns[len - 2]);
            cout << '|';
            printRNSval(list_rns[len - 1]);
            cout << endl;
        }

        //If printing whole RNS vector 
        else {
            for (int i = 0; i < list_rns.size(); i++) {
                printRNSval(list_rns[i]);
                cout << ' ';
            }
            cout << endl;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
//Create constant vector of RNS values
///////////////////////////////////////////////////////////////////////////////
vector<vector<BigUnsigned>> RNS::constant_vector_RNS(BigUnsigned length, BigUnsigned val, vector<BigUnsigned> base) {
    vector<BigUnsigned> val_rns = forwardConverter(val, base);
    vector<vector<BigUnsigned>> Z;
    for (BigUnsigned i = 0; i < length; i++) {
        Z.push_back(val_rns);
    }
    return Z;
}

//////////////////////////////////////////////////////////////////////////////
// Modular Hadamard Product

// pointwise modular multiplication between vectors (to use RNS_mult function here)
// The modulus is whatever is assigned at initialization 
///////////////////////////////////////////////////////////////////////////////
vector<vector<BigUnsigned>> RNS::hadamard_product_RNS(vector<vector<BigUnsigned>> A, vector<vector<BigUnsigned>> B) {
    vector<vector<BigUnsigned>> Z;

    for (int i = 0; i < A.size(); i++) {
        Z.push_back(modmult_RNS(A[i], B[i]));
    }

    return Z;
}
///////////////////////////////////////////////////////////////////////////////
// Returns values where the RNS sytem is 1|0|0, 0|1|0, 0|0|1, etc.

// This is the same problem as following the general unconversion algorithm but for specific values of {1|0|0}, {0|1|0}, {0|0|1}...
///////////////////////////////////////////////////////////////////////////////
/*
vector<BigUnsigned> RNS::getSingleResidues(vector<BigUnsigned> mod) {
    vector<BigUnsigned> vals;
    BigUnsigned dr = product(mod);
    BigUnsigned dR_bitLength = dr.bitLength();

    for (int i = 0; i < mod.size(); i++) {

        BigUnsigned interval = BigUnsigned(dr / mod[i]);  // search in increments of dR/mod. It's the product of all the other moduli (so their residue is 0)

        for (BigUnsigned num = interval; num <= dr; num += interval) {
            cout << "getSingleResidues() currently searching bits " << num.bitLength() << " out of " << dR_bitLength << " for channel " << i << ".\r";
            if (num % mod[i] == 1) {                                   //needs to be made more efficient for large dR. Is efficient for num up to 280 bits.
                vals.push_back(num);
                cout << num << endl;
                break;
            }
        }

    }
    cout << "                                                                                           " << endl;
    return vals;
}
*/
///////////////////////////////////////////////////////////////////////////////
// Converts integer to RNS representation
///////////////////////////////////////////////////////////////////////////////
vector<BigUnsigned> RNS::forwardConverter(BigUnsigned num, vector<BigUnsigned> base) {
    vector<BigUnsigned> num_RNS;

    for (int i = 0; i < base.size(); i++) { //n_moduli
        num_RNS.push_back(num % base[i]);
    }

    return num_RNS;
}

///////////////////////////////////////////////////////////////////////////////
// Converts RNS to integer representation
//
// The converter uses whichever weights are associated with the base, if they exist.
// Otherwise the weights are calculated on the spot and is very time consuming. Function always
// looks at the starting indices of num_RNS so you can pass a very long RNS number and
// only get its reconversion for base1 if, for example, it is the concatenation of base1
// and base2 out of the modmult module. However, you cannot get base2 conversion without first 
// separaing it into another vector where base2 are the first results in the input vector.
///////////////////////////////////////////////////////////////////////////////
BigUnsigned RNS::reverseConverter(vector<BigUnsigned> num_RNS, vector<BigUnsigned> base) {
    //select weights to use
    vector<BigUnsigned> weights;
    if (vectorsAreEqual(base, bases))
        weights = weights_bases;
    else if (vectorsAreEqual(base, base1))
        weights = weights_base1;
    else if (vectorsAreEqual(base, base2))
        weights = weights_base2;
    else if (vectorsAreEqual(base, base2_with_mr))
        weights = weights_base2_with_mr;
    else
        weights = getConversionWeights(base);

    // Resolve conversion
    BigUnsigned ret_val = 0;
    for (int i = 0; i < base.size(); i++) {
        ret_val += weights[i] * num_RNS[i];
        ret_val %= getDynamicRange(base);
    }

    return BigUnsigned(ret_val);
}

///////////////////////////////////////////////////////////////////////////////
// Gets weights for the reverse RNS conversion
///////////////////////////////////////////////////////////////////////////////
vector<BigUnsigned> RNS::getConversionWeights(vector<BigUnsigned> base) {
    vector<BigUnsigned> weights;
    BigUnsigned D = getDynamicRange(base);

    for (int i = 0; i < base.size(); i++) {
        BigUnsigned D_i = D / base[i];
        BigUnsigned D_i_inv_red_i = modinv(D_i, base[i]);
        weights.push_back((D_i * D_i_inv_red_i) % D);     //int values of 1|0|0, 0|1|0, etc.
    }
    return weights;
}

///////////////////////////////////////////////////////////////////////////////
// radix-2 Butterfly
///////////////////////////////////////////////////////////////////////////////
vector<BigUnsigned> RNS::butterfly(BigUnsigned left, BigUnsigned right, BigUnsigned twiddlefactor, BigUnsigned modulus) {

    BigUnsigned product, left_out, right_out;

    product = (right * twiddlefactor) % modulus;

    left_out = (left + product) % modulus;
    right_out = (left + modulus - product) % modulus;

    return vector<BigUnsigned> {left_out, right_out};
}
///////////////////////////////////////////////////////////////////////////////
// radix-2 RNS Butterfly
///////////////////////////////////////////////////////////////////////////////
vector<vector<BigUnsigned>> RNS::butterfly_rns(vector<BigUnsigned> left, vector<BigUnsigned> right, vector<BigUnsigned> twiddlefactor) {

    vector<BigUnsigned> product, left_out, right_out;

    product = modmult_RNS(right, twiddlefactor);

    left_out = add_RNS(left, product, bases);

    //PROBLEM: left and MM_product are of variable size due to MM in range [(2+alpha)M,(3+alpha)M] and left having similar range from previous cycle.
    // Subtraction needs to be guarenteed to not be negative. Add parity check after subtractions in both directions, or magnitude check before subtraction.

    // add_RNS will be between 
    /*
    for (int i = 0; i < bases.size() + 3; i++) {
        left = add_RNS(left, M_rns, bases);
    }
    */

    //magnitude check replacement
    if (CORRECT_BF_SUBTRACTION_INPUT) {
        BigUnsigned L = reverseConverter(left, bases);
        BigUnsigned R = reverseConverter(product, bases);
        L = (L % M) + M;
        R = R % M;

        left = forwardConverter(L, bases);
        product = forwardConverter(R, bases);
    }

    right_out = sub_RNS(left, product, bases);

    return vector<vector<BigUnsigned>> {left_out, right_out};
}

///////////////////////////////////////////////////////////////////////////////
// RNS montgomery reduction parameters

/*
Variable naming convention:f

 base1      - Moduli in the original base (size N)
 base2      - Moduli in the extended base (size N)
 m_r        - redundant channel modulus   (size 1)

 M          - The modulus being reduced by in the montgomery reduction unit
 D1         - The dynamic range of the first base. (product(base1))
 D2         - The dynamic range of the second base. (product(base2))
 D_i        - Is D1 or D2 divided by the ith modulus.     (D / moduli[i])

 _inv       - Is the modular inverse with respect to the ith modulus (not used with out _red_i)
 _red_i     - Means the value(s) are reduced by each ith modulus ( % base1[i])
 _red_j     - Means the value(s) are reduced by each jth modulus ( % base2[j])
 _red_r     - Means the value(s) are reduced by the r modulus    ( % m_r     )
 _inv_red_i - Modular inverse of D_i and the ith modulus, then reduced by ith modulus



Numbered based on alg 4.4 in "Modular Multiplication in the Residue Number System"
starting with "pre-computed values".

 Translation from algorithm 4.4:

 1.  <D^-1>_mr    -  D1_inv_red_r
 2.  <2^-1>_mr    -  two_inv_red_r
 3.  <M^-1>_mi    -  M_inv_red_i
 4.  <M>_mj       -  M_red_j
 5.  <D_i^-1>_mi  -  D1_i_inv_red_i
 6.  <D_i>_mj     -  D1_i_red_j
 7.  <D'_j>_mi    -  D2_j_red_i
 8.  <D'_j^-1>_mj -  D2_j_inv_red_j
 9.  <D'_j>_mr    -  D2_j_red_r
 10. <D'>_mi      -  D2_red_i
 11. <D^-1>_mj    -  D_inv_red_j
 12. <D'^-1>_mr   -  D2_inv_red_r

 Scheme kind of sucks but it is what it is.

 */
 ///////////////////////////////////////////////////////////////////////////////
 // calculates constants and parameters for RNS montgomery reduction

 // all_bases holds the regular moduli (N), extended base (N), and redundant modulus (1). All coprime
 // needs called in main before using modmult()
 ///////////////////////////////////////////////////////////////////////////////
void RNS::initializeParameters(vector<BigUnsigned> moduli, BigUnsigned montgomery_reduction_modulus) {

    /*
    all parameters are generated by knowing:
       n_moduli  - class variable, length of the core RNS base
       all_bases - Concatenated vector of moduli, extended moduli, and r
       modulus   - NTT modulus

    As states on page 8-9 of "Modular Multiplication in the Residue Number System" -Kong09,
    D_i = D / m_i, D_i_inv_red_m_i = modinv(D_i,m_i).


     The j's need to start at zero because some algorithms use j = 0,...,k,
     however indexes of i are generally only needed from 0 to n_base1 and indexes on j
     are only needed from n_base1 to total_bases.

     I set all tables to start at zero. The only one to go until i + j + r is "bases". All others need j - n_base1 as an index
     */


     /*----- RNS bases and sizes ------*/

        // Size of base1, base2, and total
    int n_moduli = (moduli.size() - 1) / 2;      //gets size of one base, given two bases and m_r.
    n_base1 = n_moduli;
    n_base2 = n_moduli;
    n_base2_with_mr = n_moduli + 1;
    total_bases = n_base1 + n_base2 + 1;

    //save list of all bases 
    bases = moduli;

    // Original RNS moduli (vector BigUnsigned)
    for (int i = 0; i < n_base1; i++) {
        base1.push_back(bases[i]);
    }

    // Extended base (vector BigUnsigned) 
    for (int j = n_base1; j < n_base1 + n_base2; j++) {
        base2.push_back(bases[j]);
    }

    // Extended base with m_r (vector BigUnsigned) 
    for (int j = n_base1; j < total_bases; j++) {
        base2_with_mr.push_back(bases[j]);
    }
    // Redundant moduli (BigUnsigned)
    m_r = bases[total_bases - 1];

    // create montgomery or barrett reduction module for each channel
    for (int i = 0; i < total_bases; i++) {
        REDC redc_module(bases[i]);
        redc.push_back(redc_module);
    }

    /*----- Intermediate parameters ------*/

        // Reduction modulus (BigUnsigned)
    M     = montgomery_reduction_modulus;
    M_rns = forwardConverter(M, bases);

    // Dynamic range of base1- also montgomery number  (BigUnsigned)
    D1 = product(base1);
    D1_inv = modinv(D1, M); //inverse with respect to the reduction modulus 
    D1_rns = forwardConverter(D1, bases);
    // Dynamic range of base2 (BigUnsigned)
    D2 = product(base2);

    //Modulus inverse with respect to dynamic range
    M_inv = modinv(M, D1);

    //check if input conditions are correct (k = n channels, M = reduction modulus, D =  dynamic range 1, D' = dynamic range 2)
    int k = n_moduli;

    BigUnsigned res1 = (M * (k + 2) * (k + 2));
    BigUnsigned res2 = (M * (k + 2));
    BigUnsigned res3 = (M*(k + 2)) * (M*(k + 2)); // (M * (base1[k - 1] * (k + 2)));

    bool cond1 = res1 < D1;
    bool cond2 = res2 < D2;
    bool cond3 = res3 < D1; //res3 >= D1;

    if (cond1 && cond2 && cond3)
        cout << "Bajard conditions met." << endl;
    if (!cond1) {
        cout << "Bajard condition 1 not met: (M * (k + 2) * (k + 2)) < D1" << endl;
        cout << "M,k, result < D1: " << M << " " << k << " " << res1 << " !< " << D1 << endl << endl;
    }
    if (!cond2) {
        cout << "Bajard condition 2 not met: (M * (k + 2)) < D2" << endl;
        cout << "M,k, result < D1: " << M << " " << k << " " << res2 << " !< " << D1 << endl << endl;
    }
    if (!cond3) {
        cout << "Bajard condition 3 not met: (M * (base1[k-1] * (k + 2))) >= D1" << endl;
        cout << "M,base1[k-1],k, result >= D1: " << M << " " << base1[k - 1] << " " << k << " " << res3 << " !>= " << D1 << endl << endl;
    }

    // D1 divided by the ith modulus (vector BigUnsigned)
    for (int i = 0; i < n_base1; i++) {
        D1_i.push_back(D1 / base1[i]);
    }

    // D2 divided by the jth modulus (vector BigUnsigned) 
    for (int j = 0; j < n_base2; j++) {
        D2_j.push_back(D2 / base2[j]);
    }

    /*----- Montgomery Reduction parameters  ------*/

        // 1: Inverse of D1 and m_r, reduced by m_r (BigUnsigned) 
    D1_inv_red_r = modinv(D1, m_r);

    // 2: Inverse of 2 and m_r, reduced by m_r (BigUnsigned) 
    two_inv_red_r = 1; //modinv(2, m_r);

    // 3: Inverse of M and the ith modulus, reduced by the ith modulus (vector BigUnsigned) 
    for (int i = 0; i < n_base1; i++) {
        M_inv_red_i.push_back(modinv(M, base1[i]));
    }

    // 4: M reduced by the jth modulus and m_r (vector BigUnsigned) - (In between base extensions)
    for (int j = 0; j < n_base2_with_mr; j++) {
        M_red_j.push_back(M % base2_with_mr[j]);
    }

    // 5: Inverse of D1_i and the ith modulus, reduced by the ith modulus (vector BigUnsigned) 
    for (int i = 0; i < n_base1; i++) {
        D1_i_inv_red_i.push_back(modinv(D1_i[i], base1[i]));
    }
    
    // 6: Each D1_i reduced by each jth modulus (vector vector BigUnsigned) (Used in Bajard)
    for (int i = 0; i < n_base1; i++) {
        vector<BigUnsigned> j_elements, j_elements2;
        for (int j = 0; j < n_base2_with_mr; j++) {
            j_elements.push_back(D1_i[i] % base2_with_mr[j]);
        }
        D1_i_red_j.push_back(j_elements);  //accessed in form D1_i_red_j[i][j]
    }

    // 7: Each D2_j reduced by each ith modulus (vector vector BigUnsigned)    
    // Correct version
    for (int j = 0; j < n_base2; j++) {
        vector<BigUnsigned> i_elements;
        for (int i = 0; i < n_base1; i++) {
            i_elements.push_back(D2_j[j] % base1[i]);  
        }
        D2_j_red_i.push_back(i_elements); //accessed in form D2_j_red_i[j][i]
    }
        /* alternate correct
     for (int j = 0; j < n_base2; j++) {
         vector<BigUnsigned> i_elements;
         for (int i = 0; i < n_base1; i++) {
             i_elements.push_back(D2_j[j] % base1[i]);
         }
         D2_j_red_i.push_back(i_elements); //accessed in form D2_j_red_i[i][j]
     }
     */

    
   

    // 8: inverse of D2_j and the jth modulus, reduced by each jth modulus (vector BigUnsigned)
    for (int j = 0; j < n_base2; j++) {
        D2_j_inv_red_j.push_back(modinv(D2_j[j], base2[j]));
    }

    // 9: Each D2_j reduced by the r modulus (vector BigUnsigned)
    for (int j = 0; j < n_base2; j++) {
        D2_j_red_r.push_back(D2_j[j] % m_r);
    }

    // 10: D2 reduced by each ith modulus (vector BigUnsigned)
    for (int i = 0; i < n_base1; i++) {
        D2_red_i.push_back(D2 % base1[i]);
    }

    // 11: Inverse of D1 and the jth modulus, reduced by the jth modulus and m_r (vector BigUnsigned) - (used in between base extensions)
    for (int j = 0; j < n_base2_with_mr; j++) {
        D1_inv_red_j.push_back(modinv(D1, base2_with_mr[j]));
    }

    // 12: Inverse of D2 and the r modulus, reduced by the r modulus (BigUnsigned)
    D2_inv_red_r = modinv(D2, m_r);

    // Base 1 conversion weights
    /*for (int i = 0; i < n_base1; i++) {
        weights.push_back((D1_i[i] * D1_i_inv_red_i[i]) % D1);     //int values of 1|0|0, 0|1|0, etc.
    }
    */
    // Base conversion weights (CANNOT combine into a single list of weights. Each of these are different.)
    weights_bases = getConversionWeights(bases);
    weights_base1 = getConversionWeights(base1);
    weights_base2 = getConversionWeights(base2);
    weights_base2_with_mr = getConversionWeights(base2_with_mr);

    /*
    // Base 2 conversion weights
    for (int j = 0; j < n_base2; j++) {
        BigUnsigned a = (D2 * m_r) / base2[j];
        BigUnsigned b = modinv(a, base2[j]);
        weights_extendedbase.push_back((a * b) % (D2 * m_r));
        */
        /*IMPORTANT: This needs to be written this way, instead of being a copy of the base 1 weight code. The values here that depend on D2 are recalculated to include
        m_r in the dynamic range (because all other code besides the forward/reverse converter do not include m_r as one of the bases). */
        //}
}

///////////////////////////////////////////////////////////////////////////////
// prints REDC parameters into text files

///////////////////////////////////////////////////////////////////////////////
void RNS::savetotextParameters() {

    // Original RNS moduli (vector BigUnsigned)
    saveVectorToTextfile(base1, "Parameters/base1.txt");

    // Extended base (vector BigUnsigned)
    saveVectorToTextfile(base2, "Parameters/base2.txt");

    // Extended base + redundant modulus (vector BigUnsigned)
    saveVectorToTextfile(base2_with_mr, "Parameters/base2_with_mr.txt");

    // Base 2 conversion weights
    saveVectorToTextfile(weights_extendedbase, "Parameters/weights_extendedbase.txt");

    // Redundant moduli (BigUnsigned)
    saveValToTextfile(m_r, "Parameters/m_r.txt");

    // Reduction modulus (BigUnsigned)
    saveValToTextfile(M, "Parameters/M.txt");
    saveValToTextfile(M_inv, "Parameters/M_inv.txt");

    // Dynamic range of base1 and base2 (BigUnsigned)
    saveValToTextfile(D1, "Parameters/D1.txt");
    saveValToTextfile(D1_inv, "Parameters/D1_inv.txt");
    saveValToTextfile(D2, "Parameters/D2.txt");




     // D1 divided by the ith modulus (vector BigUnsigned)
     saveVectorToTextfile(D1_i, "Parameters/D1_i.txt");


     // D2 divided by the jth modulus (vector BigUnsigned)
     saveVectorToTextfile(D2_j, "Parameters/D2_j.txt");


    /*----- Montgomery Reduction parameters  ------*/

        // 1: Inverse of D1 and m_r, reduced by m_r (BigUnsigned) 
    saveValToTextfile(D1_inv_red_r, "Parameters/D1_inv_red_r.txt");

    // 2: Inverse of 2 and m_r, reduced by m_r (BigUnsigned) 
    saveValToTextfile(two_inv_red_r, "Parameters/two_inv_red_r.txt");

    // 3: Inverse of M and the ith modulus, reduced by the ith modulus (vector BigUnsigned) 
    saveVectorToTextfile(M_inv_red_i, "Parameters/M_inv_red_i.txt");

    // 4: M reduced by the jth modulus (vector BigUnsigned) 
    saveVectorToTextfile(M_red_j, "Parameters/M_red_j.txt");

    // 5: Inverse of D1_i and the ith modulus, reduced by the ith modulus (vector BigUnsigned) 
    saveVectorToTextfile(D1_i_inv_red_i, "Parameters/D1_i_inv_red_i.txt");

    // 6: Each D1_i reduced by each jth modulus (vector vector BigUnsigned)
    saveVectorVectorToTextfile(D1_i_red_j, "Parameters/D1_i_red_j.txt");

    // 7: Each D2_j reduced by each ith modulus (vector vector BigUnsigned)
    saveVectorVectorToTextfile(D2_j_red_i, "Parameters/D2_j_red_i.txt");

    // 8: inverse of D2_j and the jth modulus, reduced by each jth modulus (vector BigUnsigned)
    saveVectorToTextfile(D2_j_inv_red_j, "Parameters/D2_j_inv_red_j.txt");

    // 9: Each D2_j reduced by the r modulus (vector BigUnsigned)
    saveVectorToTextfile(D2_j_red_r, "Parameters/D2_j_red_r.txt");

    // 10: D2 reduced by each ith modulus (vector BigUnsigned)
    saveVectorToTextfile(D2_red_i, "Parameters/D2_red_i.txt");

    // 11: Inverse of D1 and the jth modulus, reduced by the jth modulus (vector BigUnsigned)
    saveVectorToTextfile(D1_inv_red_j, "Parameters/D1_inv_red_j.txt");

    // 12: Inverse of D2 and the r modulus, reduced by the r modulus (BigUnsigned)
    saveValToTextfile(D2_inv_red_r, "Parameters/D2_inv_red_r.txt");

}

///////////////////////////////////////////////////////////////////////////////
// Base extension 1 (Bajard base extension)

/*
 D       - The dynamic range of the first base. (product(moduli))
 D_i     - Is D divided by the ith modulus.     (D / moduli[i])
 D_i_inv - Is the modular inverse of D_i and the ith modulus (modinv(D[i],moduli[i])
 */
 ///////////////////////////////////////////////////////////////////////////////
vector<BigUnsigned> RNS::baseExtension1(vector<BigUnsigned> num_RNS, vector<BigUnsigned> base_in, vector<BigUnsigned> base_out) {
    vector<BigUnsigned> sigma, num_RNS_new;

    int n_base_in = base_in.size(), n_base_out = base_out.size();


    // Calculate sigmas for each channel of the first base  
    for (int i = 0; i < n_base_in; i++) {                    //to be in parallel
        sigma.push_back((num_RNS[i] * D1_i_inv_red_i[i]) % base_in[i]);
    }

    //Compute each channel j of the new base + redundant channel (step 4)
    for (int j = 0; j < n_base_out; j++) { //in parallel //ONLY CONSTANT THAT IS N_CHANNELS + 1, for M_R OUTPUT
        BigUnsigned t = 0;
        //accumulator, to occur on each channel
        for (int i = 0; i < n_base_in; i++) {
            t = (t + sigma[i] * D1_i_red_j[i][j]);
        }

        t %= base_out[j];

        //push accumulator result back and repeat for next channel
        num_RNS_new.push_back(t);
    }

    return num_RNS_new;
}

void RNS::baseExtension1_UnitTest(int n_tests, bool SET_CONSTS_TO_ZERO) {
    cout << "BAJARD UNIT INPUTS TEST" << endl << endl;

    // Temporarily set constants in Bajard to 1
    vector<BigUnsigned>         Const1_hold;
    vector<vector<BigUnsigned>> Const2_hold;

    if (SET_CONSTS_TO_ZERO) {
        Const1_hold = D1_i_inv_red_i;
        Const2_hold = D1_i_red_j;
        cout << "Temporarily adjusting Bajard constants to 1..." << endl << endl;
        for (int i = 0; i < n_base1; i++) {
            D1_i_inv_red_i[i] = 1;
        }
        for (int j = 0; j < n_base2_with_mr; j++) {
            for (int i = 0; i < n_base1; i++) {
                D1_i_red_j[i][j] = 1;
            }
        }
    }

    // Begin test
    
    for (int i = 0; i < n_tests; i++) {
        
        vector<BigUnsigned> A;
        /*
        for (int j = 0; j < n_base1; j++){
            A.push_back(i);
        }
        */
        BigUnsigned a = 4294967087;
        a += 4294967087 + i;
        A = forwardConverter(a, base1);

        vector<BigUnsigned> ans = baseExtension1(A, base1, base2_with_mr);
        printVector(A, "input: ",true,true,32);
        printVector(ans, "output:  ",true,true,32);
        cout << endl;
    }

    // Reset constants 
    if (SET_CONSTS_TO_ZERO) {
        D1_i_inv_red_i = Const1_hold;
        D1_i_red_j     = Const2_hold;
    }
}
///////////////////////////////////////////////////////////////////////////////
// Base extension 2 (Shenoy) - From the extra base back to the original

/*
Input is size n_moduli + 1.
Output is size n_moduli.

 M       - The modulus being reduced by in the montgomery reduction unit
 D       - The dynamic range of the first base. (product(moduli))
 D_i     - Is D divided by the ith modulus.     (D / moduli[i])
 D_i_inv - Is the modular inverse of D_i and the ith modulus (modinv(D[i],moduli[i])

 _inv    - Is the modular inverse with respect to whatever modulus is
 _red_i  - Means the value or vector is reduced by the ith modulus ( % moduli[i])

 M_inv   - The modular inverse with respect to whatever modulus its reduced by.
 or        example: <M^-1>_mi is the modular inverse of M and the ith modulus, reduced
 D_inv     by the ith modulus

 */
 ///////////////////////////////////////////////////////////////////////////////

 // Shenoy base extension
vector<BigUnsigned> RNS::baseExtension2(vector<BigUnsigned> A, vector<BigUnsigned> base_in, vector<BigUnsigned> base_out) {
    vector<BigUnsigned> E_j, Z;
    BigUnsigned beta;

    int n_base_in = base_in.size(), n_base_out = base_out.size();

    //step 1
    for (int j = 0; j < n_base_out; j++) {
        E_j.push_back((A[j] * D2_j_inv_red_j[j]) % base_in[j]); 
    }

    //step 2-4 (find t for m_r)
    BigUnsigned t = 0;

   // cout << "D2_J_RED_R: ";
    for (int j = 0; j < n_base_out; j++) {
        t = (t + (E_j[j] * D2_j_red_r[j])); 
    }
    t %= m_r;
    
    //step 5 - only place (input mod m_r) is used! All loops dont use it
    beta = (D2_inv_red_r * (t + m_r - A[n_base_in - 1]) % m_r) % m_r;  //in algorithm, r is the intermediate MM value and original input to shenoy
    
    //step 6-8 (find t for all j)
    vector<BigUnsigned> t_i;

    for (int i = 0; i < n_base_out; i++) {      //performed in parallel
        t = 0;
        for (int j = 0; j < n_base_out; j++) {
            t = (t + E_j[j] * D2_j_red_i[j][i]);     //BOTH ARE LENGTH N_CHANNELS, NOT INCLUDE M_R

           // printVal(D2_j_red_i[j][i], " ", false, true, 32);
        }
        t %= base_out[i];

        t_i.push_back(t);
    }

    cout << endl;

    //need in form: 
    //{5-long j val (i = 0), 5 - long j val (val i = 1), 5 - long j val ( i = 2), 5 - long j val (i = 3)}
    //{D2_j_red_i[j][0], D2_j_red_i[j][1], D2_j_red_i[j][2], D2_j_red_i[j][3]}

    //step 9
    for (int i = 0; i < n_base_out; i++) { //in parallel
        Z.push_back((t_i[i] + base_out[i] - (beta * D2_red_i[i]) % base_out[i]) % base_out[i]);
    }

    //printVector(D2_red_i, "D2_RED_I: ", true, true, 32);

    return Z;
}

void RNS::baseExtension2_UnitTest(int n_tests, bool SET_CONSTS_TO_ZERO) {
    cout << "SHENOY UNIT INPUTS TEST" << endl << endl;

    // Temporarily set constants in Bajard to 1
    BigUnsigned Const1_hold;
    vector<BigUnsigned>         Const2_hold, Const3_hold, Const4_hold;
    vector<vector<BigUnsigned>> Const5_hold;

    if (SET_CONSTS_TO_ZERO) {
        Const1_hold = D2_inv_red_r;
        Const2_hold = D2_red_i;
        Const3_hold = D2_j_inv_red_j;
        Const4_hold = D2_j_red_r;
        Const5_hold = D2_j_red_i;
           
        cout << "Temporarily adjusting Shenoy constants to 1..." << endl << endl;

        D2_inv_red_r = 1;

        for (int i = 0; i < n_base1; i++) {
            D2_red_i[i] = 1;
        }
        for (int j = 0; j < n_base2; j++) {
            D2_j_inv_red_j[j] = 1;
            D2_j_red_r[j] = 1;
        }

        for (int i = 0; i < n_base1; i++) {
           for (int j = 0; j < n_base2_with_mr; j++) {
              D2_j_red_i[j][i] = 1;
            }
        }
    }

    // Begin test
    for (int i = 0; i < n_tests; i++) {
        vector<BigUnsigned> A;
        for (int j = 0; j < n_base2_with_mr; j++) {
            A.push_back(i);
        }
        vector<BigUnsigned> ans = baseExtension2(A, base2_with_mr, base1);
        printVector(A, "input: ", true, true, 32);
        printVector(ans, "output:  ", true, true, 32);
        cout << endl;
    }

    // Reset constants 
    if (SET_CONSTS_TO_ZERO) {
        D2_inv_red_r   = Const1_hold;
        D2_red_i       = Const2_hold;
        D2_j_inv_red_j = Const3_hold;
        D2_j_red_r     = Const4_hold;
        D2_j_red_i     = Const5_hold;
    }
}
///////////////////////////////////////////////////////////////////////////////
/* Returns montgomery modular multiplication of RNS vectors
Following "a full RNS implementation of RSA" by bajard

IMPORTANT: Requires inputs to be represented in both bases. They need to be the same length as total_bases.
*/
///////////////////////////////////////////////////////////////////////////////
vector<BigUnsigned> RNS::modmult_RNS(vector<BigUnsigned> A, vector<BigUnsigned> B) {
    vector<BigUnsigned> X, ans, Q_i, Q_j, Z_j, Z_i;

    //Operating conditions
       // 2*modulus less than dynamic range
       // A * B < M * D1

    if ((A.size() != total_bases) || (B.size() != total_bases)) {
        cout << "ERROR: RNS::modmult_RNS requires both inputs to be represented in both bases. (length != total_bases)" << endl;
    }

    // step 0  - get rid of montgomery factor if flag is set
    if (MULTIPLY_MODMULT_INPUT_BY_D) {
        A = mult_RNS(A, D1_rns, bases); //This doesn't make sense because D1 cannot be represented in base 1. But it results in correct answers.
    }

    // step 1
    for (int i = 0; i < total_bases; i++) {
        X.push_back((A[i] * B[i]) % bases[i]);
    }

    // step 2 
    for (int i = 0; i < n_base1; i++) {                                            // IMPORTANT:
        Q_i.push_back((base1[i] - (X[i] * M_inv_red_i[i]) % base1[i]) % base1[i]); // M_inv_red_i should be negative. This is the same as reducing the multiplication
                                                                                   // and subtracting it from from base1[i] then reducing again.
    }

    // step 3 
    Q_j = baseExtension1(Q_i, base1, base2_with_mr);

    //step 4
    for (int j = 0; j < n_base2_with_mr; j++) {
        Z_j.push_back(((X[j + n_base1] + Q_j[j] * M_red_j[j]) * D1_inv_red_j[j]) % base2_with_mr[j]); // ONLY MMULT LOOP TO NEED M_R
    }

    //step 5
    Z_i = baseExtension2(Z_j, base2_with_mr, base1);

    
    //convert to fully reduce output 
    if (CORRECT_MODMULT_OUTPUT) {
        BigUnsigned i = reverseConverter(Z_i, base1);
        BigUnsigned j = reverseConverter(Z_j, base2_with_mr);
        i %= M;
        j %= M;
        Z_i = forwardConverter(i, base1);
        Z_j = forwardConverter(j, base2_with_mr);
    }
    // NOTICE: Output without this is +1,2,3 alpha * M


    //edit to return concatenated both base results
    Z_i.insert(Z_i.end(), Z_j.begin(), Z_j.end());

    return Z_i;
}

///////////////////////////////////////////////////////////////////////////////
// Forward convert a whole polynomial
//
// Currently converts into ALL bases so the size will be larger
///////////////////////////////////////////////////////////////////////////////
vector<vector<BigUnsigned>> RNS::forwardConverter_polynomial(vector<BigUnsigned> A, vector<BigUnsigned> base) {
    vector<vector<BigUnsigned>> A_rns;

    // forward convert each polynomial element
    for (int i = 0; i < A.size(); i++) {
        A_rns.push_back(forwardConverter(A[i], base));
    }

    return A_rns;
}

///////////////////////////////////////////////////////////////////////////////
// Reverse convert a whole polynomial
///////////////////////////////////////////////////////////////////////////////
vector<BigUnsigned> RNS::reverseConverter_polynomial(vector<vector<BigUnsigned>> A_rns, vector<BigUnsigned> base) {
    vector<BigUnsigned> A;

    // forward convert each polynomial element
    for (int i = 0; i < A_rns.size(); i++) {
        A.push_back(reverseConverter(A_rns[i], base));
    }

    return A;
}

///////////////////////////////////////////////////////////////////////////////
// Tests forward and reverse converter

// Generates random integers in the dynamic range of base 1. Forward convert to
// RNS and reverse converts to integer form and compares to original integer. 
///////////////////////////////////////////////////////////////////////////////
void RNS::converterTest(int n_tests) {
    int n_correct = 0;

    for (int i = 0; i < n_tests; i++) {
        cout << endl << endl << "Converter test: " << endl;


        BigUnsigned ans = getRandomBigUnsigned(D1);

        vector<BigUnsigned> rns1 = forwardConverter(ans, base1);
        vector<BigUnsigned> rns2 = forwardConverter(ans, base2_with_mr);

        BigUnsigned res1 = reverseConverter(rns1, base1);
        BigUnsigned res2 = reverseConverter(rns2, base2_with_mr);

        cout << "Integer: " << ans << endl;
        printVector(rns1, "Forward RNS - base 1: ");
        printVector(rns2, "Forward RNS - base 2: ");

        cout << "Reverse Integer - base 1: " << res1 << endl;
        cout << "Reverse Integer - base 2: " << res2 << endl;

        if ((res1 == ans) && (res2 == ans)) {
            cout << "Correct." << endl;
            n_correct++;
        }
        else
            cout << "Incorrect." << endl;
    }

    cout << endl << n_correct << "/" << n_tests << " tests correct." << endl << endl;
}

///////////////////////////////////////////////////////////////////////////////
// RNS mod mult test

// Generates random numbers in the range of modulus/2. modmult_RNS requires 
// representation in both bases and returns a vector holding the independent
// rns values in base1 and base2 + m_r. They are separated and converted independently,
// then expected to both be equal to the directly computed montgomery reduction.
///////////////////////////////////////////////////////////////////////////////
bool RNS::RNSmodmultTest(int n_tests) {
    
    cout << "----------- RNS montgomery multiplication test ----------------" << endl;
    int n_correct = 0;
    int n_reduced_correct1 = 0;
    int n_reduced_correct2 = 0;

    vector<BigUnsigned> alpha;

    for (int i = 0; i < n_tests; i++) {
        // Variables
        BigUnsigned A, B, C_base1, C_base2, ans;
        vector<BigUnsigned> A_rns, B_rns, C_rns;

        // Random integers
        A = getRandomBigUnsigned(M);    //half of modulus
        B = getRandomBigUnsigned(M);

        // Convert to RNS
        A_rns = forwardConverter(A, bases);  //must be represented in both bases for modmult to work 
        B_rns = forwardConverter(B, bases);

        //test result
        C_rns = modmult_RNS(A_rns, B_rns);

        vector<BigUnsigned> C_rns_base2;
        for (int i = n_base1; i < total_bases; i++) {
            C_rns_base2.push_back(C_rns[i]);
        }

        C_base1 = reverseConverter(C_rns, base1);
        C_base2 = reverseConverter(C_rns, base2_with_mr);

        // correct answer
         if (MULTIPLY_MODMULT_INPUT_BY_D) {
            ans = (A * B) % M;
            cout << "OUTPUT RESULTS ARE COMPARED TO EXPECTED FORM A*B % M (SET BY rns.MULTIPLY_MODMULT_INPUT_BY_D flag)" << endl;
        }
         else {    
             ans = (A * B * modinv(D1, M)) % M;
             cout << "OUTPUT RESULTS ARE COMPARED TO EXPECTED FORM A*B*D^-1 % M (SET BY rns.MULTIPLY_MODMULT_INPUT_BY_D flag)" << endl;
         }

        //Compare if answers are equal
        cout << "Result: " << A << " * " << B << " % " << M << " = (base1: " << C_base1 << " base2: " << C_base2 << ")" << endl;
        cout << "Answer: " << A << " * " << B << " % " << M << " = " << ans << endl;

        if (ans == C_base1) {
            cout << "Base1 correct." << endl;
        }
        else if (ans == C_base1 % M) {
            cout << "Base1 incorrect unless reduced." << endl;
            alpha.push_back((C_base1 - ans) / M);
            n_reduced_correct1++;
        }
        else {
            cout << "Base1 incorrect." << endl;
        }

        if (ans == C_base2) {
            cout << "Base2 correct." << endl << endl;
        }
        else if (ans == C_base2 % M) {
            cout << "Base2 incorrect unless reduced." << endl << endl;
            n_reduced_correct2++;
        }
        else {
            cout << "Base2 incorrect." << endl << endl;
        }
        if (ans == C_base1 && ans == C_base2) {
            n_correct++;
            n_reduced_correct1++;
            n_reduced_correct2++;
        }
    }

    printVector(alpha, "Moduli offset for each non-reduced result: ", true);
    // The offset should always be less than (n_moduli-2).

    cout << n_reduced_correct1 << "/" << n_tests << " base1 correct if reduced." << endl;
    cout << n_reduced_correct2 << "/" << n_tests << " base2 correct if reduced." << endl;
    cout << n_correct << "/" << n_tests << " tests correct." << endl << endl;
    return true;
}

///////////////////////////////////////////////////////////////////////////////
// RNS Butterfly Test 

// Tests random pairing of left and right inputs, comparing RNS arithmetic 
// version and regular computation.

// Caution: results are not reduced by M, unlike the actual butterfly. It is reduced
// here but the true results will need reduced one final time.
///////////////////////////////////////////////////////////////////////////////
void RNS::butterflyRNStest(int n_tests) {
    int n_correct = 0;
    int l_correct = 0;
    int r_correct = 0;

    for (int i = 0; i < n_tests; i++) {
        cout << endl << endl << "RNS butterfly test: " << endl;

        BigUnsigned left  = getRandomBigUnsigned(M);
        BigUnsigned right = getRandomBigUnsigned(M);
        BigUnsigned twid  = getRandomBigUnsigned(M);

        vector<BigUnsigned> left_rns  = forwardConverter(left, bases);
        vector<BigUnsigned> right_rns = forwardConverter(right, bases);
        vector<BigUnsigned> twid_rns  = forwardConverter(twid, bases);

        vector<vector<BigUnsigned>> C_rns = butterfly_rns(left_rns, right_rns, twid_rns);
        vector<BigUnsigned>         ans   = butterfly(left, right, twid, M);

        BigUnsigned ans_left  = ans[0];
        BigUnsigned ans_right = ans[1];

        BigUnsigned C_left  = reverseConverter(C_rns[0], base1) % M;
        BigUnsigned C_right = reverseConverter(C_rns[1], base1) % M;

        cout << "Left, Right, twiddle values: " << left << " " << right << " " << twid << endl;

        cout << "result: " << C_left << " " << C_right << endl;
        cout << "answer: " << ans_left << " " << ans_right << endl;


        if ((C_left == ans_left) && (C_right == ans_right)) {
            cout << "Correct." << endl;
            n_correct++;
            l_correct++;
            r_correct++;
        }
        else if (C_left == ans_left) {
            cout << "Left correct. Right incorrect." << endl << endl;
            l_correct++;
        }
        else if (C_right == ans_right) {
            cout << "Left incorrect. Right correct." << endl << endl;
            r_correct++;
        }
        else
            cout << "Incorrect." << endl << endl;
    }

    cout << endl << l_correct << "/" << n_tests << " left tests correct." << endl;
    cout << endl << r_correct << "/" << n_tests << " right tests correct." << endl;
    cout << endl << n_correct << "/" << n_tests << " tests correct." << endl << endl;
}

///////////////////////////////////////////////////////////////////////////////
// Base extension tests

// A random number in the range of base1 dynamic range is generated. It is converted to 
// base2 using bajard, converted back to base1 using shenoy, and compared to the original correct 
// integer (for both the value and reduced value). Partial error here is fixed when using 
// base extension in combination with modmult. 75% accuracy here is replaced with 100% accuracy in 
// modmultTest(), despite these same base extensions being used there. This is because the bajard extension
// results in a multiply of D1, which is canceled in the in-between multiplication
///////////////////////////////////////////////////////////////////////////////
bool RNS::baseExtensionTest(int n_tests) {
    cout << "Starting base extension test." << endl;
    printVector(base1, "From base: ", true);
    printVector(base2_with_mr, "To base: ", true);

    int n_correct = 0;
    int n_correct2 = 0;

    for (int i = 0; i < n_tests; i++) {
        //generate base1 RNS number 
        BigUnsigned              num = getRandomBigUnsigned(D1/2); //In the range of Modulus dynamic range
        cout << "test integer:  " << num << endl;
        vector<BigUnsigned> num_rns = forwardConverter(num, base1);

        printVector(num_rns, "Base 1 representation: ", true);


        //extend to base2 using Bajard
        vector<BigUnsigned> extension1 = RNS::baseExtension1(num_rns, base1, base2_with_mr);
        printVector(extension1, "Bajard representation: ", true);



        //extend back to base1 using Shenoy
        vector<BigUnsigned> extension2 = RNS::baseExtension2(extension1, base2_with_mr, base1);
        printVector(extension2, "Shenoy re-conversion: ", true);

        //check
        BigUnsigned result = reverseConverter(extension2, base1);
        cout << "Result: " << result << endl;

        if (result == num) {
            cout << "Correct." << endl << endl;
            n_correct++;
        }
        if (result % M == num) {
            cout << "Correct when reduced by M." << endl << endl;   //All reducable results are being counted as correct.
            n_correct2++;
        }
        else {
            cout << "Incorrect." << endl << endl;
        }

    }

    cout << endl << "Test finished. " << n_correct << "/" << n_tests << " correct. " << n_correct2 << "/" << n_tests << " correct when reduced by M." << endl;

    return true;
}

///////////////////////////////////////////////////////////////////////////////
// Tests shenoy complete base extension
///////////////////////////////////////////////////////////////////////////////
bool RNS::shenoyTest(int n_tests) {
    cout << "Starting Shenoy base extension test." << endl << endl;
    printVector(base2_with_mr, "From base: ", true);
    printVector(base1, "To base: ", true); //Initial base needs to have a redundant modulus appended at input.
                                       //Treat the last channel as m_r

    int n_correct = 0;
    int n_partially_correct = 0;

    for (int i = 0; i < n_tests; i++) {
        //generate RNS number 
        BigUnsigned              ans = getRandomBigUnsigned(D1/2);
        cout << "test integer:  " << ans << endl;
        vector<BigUnsigned> num_rns = forwardConverter(ans, base2_with_mr);

        printVector(num_rns, "Original RNS representation: ", true);

        //extend using Shenoy
        vector<BigUnsigned> shenoy_result = RNS::baseExtension2(num_rns, base2_with_mr, base1);
        printVector(shenoy_result, "Shenoy extension: ", true);

        //check
        BigUnsigned result = reverseConverter(shenoy_result, base1);
        cout << "Integer result: " << result << endl;

        if (result == ans) {
            cout << "Correct." << endl << endl;
            n_correct++;
        }
        if (result == ans % M) {
            cout << "Correct if reduced." << endl << endl;
            n_partially_correct++;
        }
        else {
            cout << "Incorrect." << endl << endl;            
        }
    }

    cout << endl << "Test finished. " << n_correct << "/" << n_tests << " correct. " << n_partially_correct << "/" << n_tests << " correct if reduced." << endl;

    return true;
}

///////////////////////////////////////////////////////////////////////////////
// Tests bajard incomplete base extension
///////////////////////////////////////////////////////////////////////////////
bool RNS::bajardTest(int n_tests) {
    cout << "Starting Bajard base extension test." << endl << endl;
    printVector(base1, "From base: ", true);
    printVector(base2_with_mr, "To base: ", true); //Final base needs to have a redundant modulus appended at input.
                                          //Treat the last channel as m_r

    int n_correct = 0;
    int n_partially_correct = 0;

    for (int i = 0; i < n_tests; i++) {
        //generate RNS number 
        BigUnsigned              num = getRandomBigUnsigned(D1/2);
        cout << "test integer:  " << num << endl;
        
        vector<BigUnsigned> num_rns = forwardConverter(num, base1);

        printVector(num_rns, "Original RNS representation: ", true);

        //extend using bajard
        vector<BigUnsigned> bajard_result = RNS::baseExtension1(num_rns, base1, base2_with_mr);
        printVector(bajard_result, "Bajard extension: ", true);

        BigUnsigned integer_result = reverseConverter(bajard_result, base2_with_mr);
        cout << "Integer result: " << integer_result << endl;

        //check
        if (integer_result == num) {
            cout << "Exactly correct." << endl << endl;
            n_correct++;
        }
        if (integer_result % D1 == num) {                            //Bajard should be Q' = Q + alpha * D1 
            cout << "Correct when reduced by D1." << endl << endl;
            n_partially_correct++;
        }
        //else {
        //    cout << "Incorrect." << endl << endl;
        //}
    }

    cout << endl << "Test finished. " << n_correct << "/" << n_tests << " correct. " << n_partially_correct << "/" << n_tests << " correct if reduced." << endl;

    return true;
}

///////////////////////////////////////////////////////////////////////////////
// Modular arithmetic functions
///////////////////////////////////////////////////////////////////////////////
BigUnsigned RNS::MOD_ADD(BigUnsigned A, BigUnsigned B, BigUnsigned MOD) {
    return (A + B) % MOD;
}

BigUnsigned RNS::MOD_SUB(BigUnsigned A, BigUnsigned B, BigUnsigned MOD) {
    return (A + MOD - B) % MOD;
}

BigUnsigned RNS::MOD_MULT(BigUnsigned A, BigUnsigned B, BigUnsigned MOD) {
    return (A * B) % MOD;
}
///////////////////////////////////////////////////////////////////////////////
// Returns addition of RNS vectors 
///////////////////////////////////////////////////////////////////////////////
vector<BigUnsigned> RNS::add_RNS(vector<BigUnsigned> A, vector<BigUnsigned> B, vector<BigUnsigned> base) {
    vector<BigUnsigned> ret_val;
    for (int i = 0; i < base.size(); i++) {
        ret_val.push_back(MOD_ADD(A[i], B[i], base[i])); 
    }
    return ret_val;
}

///////////////////////////////////////////////////////////////////////////////
// Returns subtraction of RNS vectors 
///////////////////////////////////////////////////////////////////////////////
vector<BigUnsigned> RNS::sub_RNS(vector<BigUnsigned> A, vector<BigUnsigned> B, vector<BigUnsigned> base) {
    vector<BigUnsigned> ret_val;

    for (int i = 0; i < base.size(); i++) {
        ret_val.push_back(MOD_SUB(A[i], B[i], base[i]));
    }

    return ret_val;
}


///////////////////////////////////////////////////////////////////////////////
// Returns multiplication of RNS vectors (no reduction or overflow protection)
///////////////////////////////////////////////////////////////////////////////
vector<BigUnsigned> RNS::mult_RNS(vector<BigUnsigned> A, vector<BigUnsigned> B, vector<BigUnsigned> base) {
    vector<BigUnsigned> ret_val;

    for (int i = 0; i < base.size(); i++) {
        ret_val.push_back(MOD_MULT(A[i],B[i],base[i])); //standard RNS multiplication. Can be replaced with Barrett
    }

    return ret_val;
}


///////////////////////////////////////////////////////////////////////////////
// Returns addition of integers converting to and from RNS
///////////////////////////////////////////////////////////////////////////////
BigUnsigned RNS::add(BigUnsigned A, BigUnsigned B, vector<BigUnsigned> base) {
    return reverseConverter(add_RNS(forwardConverter(A, base), forwardConverter(B, base), base), base);
}

///////////////////////////////////////////////////////////////////////////////
// Returns subtraction of integers converting to and from RNS
///////////////////////////////////////////////////////////////////////////////
BigUnsigned RNS::sub(BigUnsigned A, BigUnsigned B, vector<BigUnsigned> base) {
    return reverseConverter(sub_RNS(forwardConverter(A, base), forwardConverter(B, base), base), base);
}

///////////////////////////////////////////////////////////////////////////////
// Returns multiplication of integers converting to and from RNS
///////////////////////////////////////////////////////////////////////////////
BigUnsigned RNS::mult(BigUnsigned A, BigUnsigned B, vector<BigUnsigned> base) {
    return reverseConverter(mult_RNS(forwardConverter(A, base), forwardConverter(B, base), base), base);
}

/////////////////////////////////////////////////////////////////////////////
// Prints results for the determine moduli functions
/////////////////////////////////////////////////////////////////////////////
void RNS::printModuliResults(int totalBits, vector<BigUnsigned> moduli, int n_moduli) {
    //print results
    printVector(moduli, "Chosen RNS moduli: ");

    cout << "RNS moduli bits: ";
    BigUnsigned sum = 0;
    for (int i = 0; i < moduli.size(); i++) {
        BigUnsigned val = moduli[i].bitLength();
        cout << val << " ";
        if (i < n_moduli) {
            sum += val;
        }
    }
    cout << "= " << sum << " bits in base1." << endl;
    cout << "Modulus bits: " << totalBits / 2 << " bits" << endl;
    cout << "Necessary bits to prevent overflow: " << totalBits << " bits" << endl << endl;
}

/////////////////////////////////////////////////////////////////////////////
// Auto-generate RNS moduli  
//
//Get's coprime moduli in the form 2^k, 2^(k-1), 2^(k2-1), 2^(k3-1)
//All are close to powers of 2. Downside of later moduli being many more bits
/////////////////////////////////////////////////////////////////////////////

vector<BigUnsigned> RNS::determineRNSmoduli(int totalBits, int n_moduli) {
    vector<BigUnsigned> moduli;

    int bitsPer = ceil(totalBits / n_moduli);
    int i = 0;

    while (moduli.size() < n_moduli) {
        BigUnsigned mod = 1;
        if (i == 0) {
            mod = mod << bitsPer;
            moduli.push_back(mod);
            cout << moduli.size() << "moduli found...\r";
        }
        else {
            mod = mod << (bitsPer + i - 1);
            mod -= 1;
        }
        if (isCoprimeToVector(mod, moduli)) {
            moduli.push_back(mod);
            cout << moduli.size() << "moduli found...\r";
        }
        i++;
    }

    printModuliResults(totalBits, moduli);
    return moduli;
}

/////////////////////////////////////////////////////////////////////////////
// Auto-generate RNS moduli 2 
//
//Get's coprime moduli in the form 2^k, 2^(k-1), and remaining all equal to 
// the equally distributed bitdepth.
/////////////////////////////////////////////////////////////////////////////

vector<BigUnsigned> RNS::determineRNSmoduli2(int totalBits, int n_moduli, bool generate_redundant_base) {
    vector<BigUnsigned> moduli;

    int bitsPer = ceil(totalBits / n_moduli) + 1;

    BigUnsigned mod = 1;

    //First added modulus
    mod = mod << bitsPer;
    moduli.push_back(mod);

    //search all odd "bitsPer"-bit numbers
    BigUnsigned start = mod + 1;             //2^(bitsPer) + 1
    BigUnsigned end = mod << 1;            //2^(bitsPer+1) 

    int n_mod = n_moduli;
    //return twice as many moduli if needing a redundant base (plus redundant channel m_r)
    if (generate_redundant_base) n_mod = n_moduli * 2 + 1;

    for (mod = start; mod < end; mod += 2) {
        if (isCoprimeToVector(mod, moduli)) {
            moduli.push_back(mod);
        }
        if (moduli.size() == n_mod) {
            printModuliResults(totalBits, moduli, n_moduli);
            return moduli;
        }
    }

    cout << "Could not find " << n_moduli << " " << bitsPer << "-bit coprime numbers." << endl;
    printModuliResults(totalBits, moduli, n_moduli);
    return moduli;
}

/////////////////////////////////////////////////////////////////////////////
// Tests all RNS mult/addition/subtraction.

// RNS addition is operational as long as modulus M.

// In the RNS butterfly, the general desire is to have all values wrapped to M
// (which is done after the montgomery multiplication). The addition and 
// multiplication results are raw and correct, however the 
// subtraction result is only correct if reduced by M because sub_RNS adds 
// M to the result to keep it positive (assuming two input values in [0,M])
/////////////////////////////////////////////////////////////////////////////
void RNS::arithmetic_test(int n_tests) {
    int n_correct = 0; //overall
    int a_correct = 0; //addition
    int s_correct = 0; //subtraction
    int m_correct = 0; //multiplication

    for (int i = 0; i < n_tests; i++) {
        cout << endl << "RNS arithmetic test " << i << ": " << endl;


        BigUnsigned left = getRandomBigUnsigned(M);
        BigUnsigned right = getRandomBigUnsigned(M);

        vector<BigUnsigned> left_rns = forwardConverter(left, base1);
        vector<BigUnsigned> right_rns = forwardConverter(right, base1);

        vector<BigUnsigned> a_res_rns = add_RNS(left_rns, right_rns, base1);
        vector<BigUnsigned> s_res_rns = sub_RNS(left_rns, right_rns, base1);
        vector<BigUnsigned> m_res_rns = mult_RNS(left_rns, right_rns, base1);

        BigUnsigned a_res = reverseConverter(a_res_rns, base1);
        BigUnsigned s_res = reverseConverter(s_res_rns, base1);
        BigUnsigned m_res = reverseConverter(m_res_rns, base1);

        BigUnsigned a_ans = left + right;
        BigUnsigned s_ans = left + M - right;
        BigUnsigned m_ans = left * right;


        if (a_ans == a_res) {
            cout << "Add correct. ";
            a_correct++;
        }
        else {
            cout << "Add Incorrect. ";
        }

        if (s_ans == s_res) {
            cout << "Subtraction correct. ";
            s_correct++;
        }
        else {
            cout << "Subtraction Incorrect. ";
        }
        if (m_ans == m_res) {
            cout << "Multiplication correct. " << endl << endl;
            m_correct++;
        }
        else {
            cout << "Multiplication Incorrect. " << endl << endl;
        }

        if (a_ans == a_res && s_ans == s_res && m_ans == m_res)
            n_correct++;
    }


    cout << a_correct << "/" << n_tests << " add tests correct." << endl;
    cout << s_correct << "/" << n_tests << " sub tests correct." << endl;
    cout << m_correct << "/" << n_tests << " mult tests correct." << endl;
    cout << endl << n_correct << "/" << n_tests << " tests correct." << endl << endl;
}


///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////

RNS::RNS() {

}