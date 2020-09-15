#include "RNS.h"
#include <vector>
#include "general_functions.h"
#include <iostream>
#include "REDC.h"
#include "BigIntLibrary/BigIntegerLibrary.hh"

using namespace std;


///////////////////////////////////////////////////////////////////////////////
// RNS dynamic range given moduli
///////////////////////////////////////////////////////////////////////////////
BigUnsigned RNS::getDynamicRange() {
    return product(moduli);
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
vector<BigUnsigned> RNS::forwardConverter(BigUnsigned num) {
    vector<BigUnsigned> num_RNS;
    for (int i = 0; i < n_base1; i++) { //n_moduli
        num_RNS.push_back(num % base1[i]);
    }
    return num_RNS;
}

///////////////////////////////////////////////////////////////////////////////
// Converts RNS to integer representation
///////////////////////////////////////////////////////////////////////////////
BigUnsigned RNS::reverseConverter(vector<BigUnsigned> num_RNS) {
    BigUnsigned ret_val         = 0;
    for (int i = 0; i < n_base1; i++) {  
        ret_val += weights[i] * num_RNS[i];
        ret_val %= D1;  
    }

    return BigUnsigned(ret_val);
}

///////////////////////////////////////////////////////////////////////////////
// Convert BigUnsigned to RNS (of extended and m_r)
///////////////////////////////////////////////////////////////////////////////
vector<BigUnsigned> RNS::forwardConverter_extendedbase(BigUnsigned num) {
    vector<BigUnsigned> num_RNS;

    for (int i = 0; i < n_base2; i++) {
        num_RNS.push_back(num % base2[i]);
    }

    return num_RNS;
}

///////////////////////////////////////////////////////////////////////////////
// Converts RNS to BigUnsigned representation for extended base
///////////////////////////////////////////////////////////////////////////////
BigUnsigned RNS::reverseConverter_extendedbase(vector<BigUnsigned> num_RNS) {
    BigUnsigned ret_val = 0;
    for (int i = 0; i < n_base2; i++) {
        ret_val += weights_extendedbase[i] * num_RNS[i];
        ret_val %= (D2 * m_r);    // the extension dynamic range is calculated excluding the redundant base, but the forward converter uses it so it needs included. 
    }

    return BigUnsigned(ret_val);
}

///////////////////////////////////////////////////////////////////////////////
// RNS montgomery reduction parameters

/*
Variable naming convention:

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

// bases holds the regular moduli (N), extended base (N), and redundant modulus (1). All coprime
// needs called in main before using modmult()
///////////////////////////////////////////////////////////////////////////////
void RNS::initializeREDCParameters(int n_moduli, BigUnsigned modulus, vector<BigUnsigned> all_bases) {

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
    n_base1     = n_moduli;
    n_base2     = n_moduli + 1;  //holds redundant moduli
    total_bases = n_base1 + n_base2;  
    
    //save list of all bases 
    bases = all_bases;

    // Original RNS moduli (vector BigUnsigned)
    for (int i = 0; i < n_base1; i++) {
        base1.push_back(bases[i]);
    }

    // Extended base + redundant modulus (vector BigUnsigned)
    for (int j = n_base1; j < total_bases; j++) {
        base2.push_back(bases[j]);
    }

    // Redundant moduli (BigUnsigned)
    m_r = bases[total_bases-1];

/*----- Intermediate parameters ------*/

    // Reduction modulus (BigUnsigned)
    M     = modulus; 

    // Dynamic range of base1- also montgomery number  (BigUnsigned)
    D1 = product(base1);
    D1_inv = modinv(D1, modulus); //inverse with respect to the reduction modulus 

    // Dynamic range of base2  excluding m_r (BigUnsigned)
    D2 = product(base2) / m_r;

    //Modulus inverse with respect to dynamic range
    M_inv = modinv(M, D1);   //may not be right

    //check if input conditions are correct (k = n channels, M = reduction modulus, D =  dynamic range 1, D' = dynamic range 2)
    int k = n_moduli;

    bool cond1 = (M * (k + 2) * (k + 2)) < D1;
    bool cond2 = (M * (k + 2)          ) < D2;
    bool cond3 = (M * (base1[k-1] * (k + 2))) >= D1;

    if (cond1 && cond2 && cond3) 
        cout << "Bajard conditions met." << endl;
    else 
        cout << "Bajard conditions not met." << endl;


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

    // 4: M reduced by the jth modulus (vector BigUnsigned) 
    for (int j = 0; j < n_base2; j++) {
        M_red_j.push_back(M % base2[j]);
    }

    // 5: Inverse of D1_i and the ith modulus, reduced by the ith modulus (vector BigUnsigned) 
    for (int i = 0; i < n_base1; i++) {
        D1_i_inv_red_i.push_back(modinv(D1_i[i], base1[i]));
    }

    // 6: Each D1_i reduced by each jth modulus (vector vector BigUnsigned)
    for (int i = 0; i < n_base1; i++) {
        vector<BigUnsigned> j_elements;
        for (int j = 0; j < n_base2; j++) {
            j_elements.push_back(D1_i[i] % base2[j]);
        }
        D1_i_red_j.push_back(j_elements);  //accessed in form D1_i_red_j[i][j]
    }

    // 7: Each D2_j reduced by each ith modulus (vector vector BigUnsigned)
    for (int j = 0; j < n_base2; j++) {
        vector<BigUnsigned> i_elements;
        for (int i = 0; i < n_base1; i++) {
            i_elements.push_back(D2_j[j] % base1[i]); 
        }
        D2_j_red_i.push_back(i_elements); //accessed in form D2_j_red_i[j][i]
    }

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

    // 11: Inverse of D1 and the jth modulus, reduced by the jth modulus (vector BigUnsigned)
    for (int j = 0; j < n_base2; j++) {
        D1_inv_red_j.push_back(modinv(D1, base2[j])); 
    }
  
    // 12: Inverse of D2 and the r modulus, reduced by the r modulus (BigUnsigned)
    D2_inv_red_r = modinv(D2, m_r); 

    // Base 1 conversion weights
    for (int i = 0; i < n_base1; i++) {
        weights.push_back((D1_i[i] * D1_i_inv_red_i[i]) % D1);     //int values of 1|0|0, 0|1|0, etc.
    }

    // Base 2 conversion weights
    for (int j = 0; j < n_base2; j++) {
        BigUnsigned a = (D2 * m_r) / base2[j];
        BigUnsigned b = modinv(a, base2[j]);
        weights_extendedbase.push_back((a * b) % (D2 * m_r)); 

        /*IMPORTANT: This needs to be written this way, instead of being a copy of the base 1 weight code. The values here that depend on D2 are recalculated to include
        m_r in the dynamic range (because all other code besides the forward/reverse converter do not include m_r as one of the bases). */
    }
}

///////////////////////////////////////////////////////////////////////////////
// prints REDC parameters into text files

///////////////////////////////////////////////////////////////////////////////
void RNS::savetotextREDCParameters() {

    // Original RNS moduli (vector BigUnsigned)
    saveVectorToTextfile(base1,"Parameters/base1.txt");

    // Extended base + redundant modulus (vector BigUnsigned)
    saveVectorToTextfile(base2,"Parameters/base2.txt");

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

    /*
    //zero pad j vectors - may want to move this to outer function so output isnt padded 
    for (int i = 0; i < n_base; i++) {
        num_RNS_new.push_back(1);
    }
    */

    // Calculate sigmas for each channel of the first base  
    for (int i = 0; i < n_base_in; i++) {                    //to be in parallel
        sigma.push_back((num_RNS[i] * D1_i_inv_red_i[i]) % base_in[i]);
    }

    //Compute each channel j of the new base + redundant channel (step 4)
    for (int j = 0; j < n_base_out; j++) { //in parallel
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
vector<BigUnsigned> RNS::baseExtension2(vector<BigUnsigned> num_RNS, vector<BigUnsigned> base_in, vector<BigUnsigned> base_out) {
    vector<BigUnsigned> E_j, new_num_RNS;
    BigUnsigned beta;

    int n_base_in = base_in.size(), n_base_out = base_out.size();

    //step 1
    for (int j = 0; j < n_base_out; j++) {
        E_j.push_back((num_RNS[j] * D2_j_inv_red_j[j]) % base_in[j]);
    }

    //step 2-4 (find t for m_r)
    BigUnsigned t = 0;
    for (int j = 0; j < n_base_out; j++) {
        t = (t + (E_j[j] * D2_j_red_r[j])); //% m_r) % m_r;
    }

    t %= m_r;
    // t should be 0

    //step 5 - only place (input mod m_r) is used! All loops dont use it
    beta = (D2_inv_red_r * (t + m_r - num_RNS[n_base_in-1]) % m_r) % m_r;

    // beta should be 1

    //step 6-8 (find t for all j)
    vector<BigUnsigned> t_i;
    cout << "M_j_red_i: ";
    for (int i = 0; i < n_base_out; i++) {      //performed in parallel
        t = 0;
        for (int j = 0; j < n_base_out; j++) {
            t = (t + E_j[j] * D2_j_red_i[j][i]);
        }

        t %= base_out[i];

        t_i.push_back(t);
    }

    //step 9
    for (int i = 0; i < n_base_out; i++) { //in parallel
        new_num_RNS.push_back((t_i[i] + base_out[i] - (beta * D2_red_i[i]) % base_out[i]) % base_out[i]);
    }

    return new_num_RNS;
}

///////////////////////////////////////////////////////////////////////////////
/* Returns montgomery modular multiplication of RNS vectors
Following "a full RNS implementation of RSA"

Bajard algorithm
*/
///////////////////////////////////////////////////////////////////////////////
vector<BigUnsigned> RNS::modmult_RNS_bajard(vector<BigUnsigned> A, vector<BigUnsigned> B) {
    vector<BigUnsigned> X, ans, Q_i, Q_j, out_j, out_i;

    // step 1
    for (int i = 0; i < total_bases; i++) {
        X.push_back((A[i] * B[i]) % bases[i]);
    }

    // step 2 
    for (int i = 0; i < n_base1; i++) {
        Q_i.push_back((X[i] * M_inv_red_i[i]) % base1[i]); //M_inv_red_i should be negative
    }

    // step 3 
    Q_j = baseExtension1(Q_i, base1, base2);
    
    //step 4
    for (int j = 0; j < n_base2; j++) {
        out_j.push_back(((X[j + n_base1] + Q_j[j] * M_red_j[j]) * D1_inv_red_j[j]) % base2[j]);
    }

    //step 5
    out_i = baseExtension2(out_j, base2, base1);
    
    return out_i;
}
///////////////////////////////////////////////////////////////////////////////
/* Returns montgomery modular multiplication of RNS vectors
Following "VHDL Implementation of 4096-bit RNS Montgomery
Modular Exponentiation for RSA Encryption" 

masters algorithm

Base extensions are split
*/
///////////////////////////////////////////////////////////////////////////////
vector<BigUnsigned> RNS::modmult_RNS_split(vector<BigUnsigned> A, vector<BigUnsigned> B) {
    vector<BigUnsigned> ans, Q_i, Q_j, E_j, R_j;
    BigUnsigned beta;

    //step 2
    for (int i = 0; i < n_base1; i++) {
        Q_i.push_back((A[i] * B[i] * M_inv_red_i[i]) % base1[i]);
    }

    //step 3-7
    Q_j = baseExtension1(Q_i, base1, base2);

    for (int j = 0; j < n_base2; j++) {
        R_j.push_back( ((A[j + n_base1] * B[j + n_base1] + Q_j[j] * M_red_j[j]) * D1_inv_red_j[j]) % base2[j]);
    }

    ans = baseExtension2(R_j, base2, base1);
  
    return ans;
}

///////////////////////////////////////////////////////////////////////////////
// Returns montgomery modular multiplication of RNS vectors (no overflow protection)

// PhD algorithm
// inputs A and B are in base 1,2, and m_r. They have length 2N + 1
///////////////////////////////////////////////////////////////////////////////
vector<BigUnsigned> RNS::modmult_RNS(vector<BigUnsigned> A, vector<BigUnsigned> B) {
    vector<BigUnsigned> X, Q_i, Q_j, Z_j, Z_i;
    BigUnsigned Beta;

    //step 1: (correct)
    for (int i = 0; i < total_bases; i++) {
        X.push_back((A[i] * B[i]) % bases[i]);
    }

    //step 2:(correct)
    for (int i = 0; i < n_base1; i++) {
        Q_i.push_back((X[i] * M_inv_red_i[i]) % base1[i]);
    }

    //step 3: extension (correct)
    for (int j = 0; j < n_base2; j++) {
        BigUnsigned num = 0;
        for (int i = 0; i < n_base1; i++) {
            num += D1_i_red_j[i][j] * ((D1_i_inv_red_i[i] * Q_i[i]) % base1[i]);
        }
        Q_j.push_back(num % base2[j]);
    }

    // printVector(Q_j, "Q_j: ",true);

     //step 4: (correct)
    for (int j = 0; j < n_base2; j++) {
        Z_j.push_back((((X[j] + Q_j[j] * M_red_j[j]) % base2[j]) * D1_inv_red_j[j]) % base2[j]);
    }

    //printVector(Z_j, "Z_j: ",true);

    //steps 5 & 6 where the sums do not include the subtraction
    BigUnsigned result = 0;
    for (int j = n_base1; j < n_base1 + n_base2; j++) {
        result += ((D2_inv_red_r * D2_j_red_r[j]) * ((D2_j_inv_red_j[j] * Z_j[j]) % base2[j]) % m_r) % m_r;
    }
    Beta = ((result + m_r - (D2_inv_red_r * Z_j[total_bases - 1]) % m_r) % m_r);

    //cout << "Beta: " << Beta << endl;

    //step 6: extend Z_j to Z_i
    for (int i = 0; i < n_base1; i++) {
        BigUnsigned sum = 0;
        for (int j = n_base1; j < n_base1 + n_base2; j++) {
            sum += (D2_j_red_i[j][i] * (D2_j_inv_red_j[j] * Z_j[j]) % base2[j]);
        }

        Z_i[i] = ((sum + base1[i] - (Beta * D2_red_i[i]) % base1[i]) % base1[i]);
    }

    // printVector(Z_i, "Z_i: ",true);

    return Z_i;
}



///////////////////////////////////////////////////////////////////////////////
// Tests forward and reverse converter
///////////////////////////////////////////////////////////////////////////////
void RNS::converterTest(int n_tests){

    for (int i = 0; i < n_tests; i++) {
        cout << "Converter test: " << endl << endl;

        BigUnsigned num = rand() % D1.toUnsignedInt();

        vector<BigUnsigned> rns1 = forwardConverter(num);
        vector<BigUnsigned> rns2 = forwardConverter_extendedbase(num);

        BigUnsigned res1 = reverseConverter(rns1);
        BigUnsigned res2 = reverseConverter_extendedbase(rns2);

        cout << "Integer: " << num << endl;
        printVector(rns1, "Forward RNS - base 1: ");
        printVector(rns2, "Forward RNS - base 2: ");

        cout << "Reverse Integer - base 1: " << res1 << endl;
        cout << "Reverse Integer - base 2: " << res2 << endl;

        if ((res1 == num) && (res2 == num))
            cout << "Correct." << endl;
        else
            cout << "Wrong." << endl;

        /*for (int i = 0; i < n_base1; i++) {
            BigUnsigned ans = num % base1[i];
            BigUnsigned res = rns1[i];
            if (ans != res) {
                cout << num << " % " << base1[i] << " should = " << ans << ", not " << res << "." << endl;
            }
        }
        cout << "Integer: " << num << endl;
        */
    }
 }

///////////////////////////////////////////////////////////////////////////////
// RNS mod mult test
///////////////////////////////////////////////////////////////////////////////
bool RNS::RNSmodmultTest(int n_tests) {
    cout << "----------- RNS montgomery multiplication test ----------------" << endl;
    bool all_correct = true;
    BigUnsigned cap = M / 2;   //half of modulus

    for (int i = 0; i < n_tests; i++) {

        BigUnsigned A, B, C1, C2, C3, ans;
        vector<BigUnsigned> A_rns, B_rns, C1_rns, C2_rns, C3_rns, ans_rns;
        
        
        A = rand() % cap.toUnsignedInt();
        B = rand() % cap.toUnsignedInt();

        BigUnsigned R = D1;        // 1 ... D1 to try to get true montgomery result
        A_rns = forwardConverter_extendedbase(A * 1);
        B_rns = forwardConverter_extendedbase(B * R);

        
        // test answer
        //C1_rns  = modmult_RNS(A_rns, B_rns);      //RNS answer for PhD
        //C2_rns = modmult_RNS_split(A_rns, B_rns); //RNS answer for Masters
        C3_rns = modmult_RNS_bajard(A_rns, B_rns);//RNS answer from Bajard
        //C1 = reverseConverter(C1_rns) % M;            //int answer for PhD
        //C2 = reverseConverter(C2_rns) % M;            //int answer for Masters
        C3 = reverseConverter(C3_rns) % M;            //int answer from Bajard

        // correct answer
        ans = (A * B * D1_inv) % M;               //int true answer
        ans_rns = forwardConverter(ans);          //RNS true answer

    
        //test if answers are equal
        cout << A << " * " << B << " % " << M << " = " << ans << endl;
        
        /*
        cout << " PhD result: " << C1 << " Masters result: " << C2 <<  " Bajard result: " << C3 << endl;

        if (ans == C1) {
            cout << "PhD Correct, ";
        }
        else {
            cout << "PhD incorrect, ";
        }
        if (ans == C2) {
            cout << "Masters Correct, ";
        }
        else {
            cout << "Masters incorrect, ";
        }
        */
        if (ans == C3) {
            cout << "Bajard Correct, ";
        }
        else {
            cout << "Bajard incorrect, " << endl;
        }

    }

    return true;
}
///////////////////////////////////////////////////////////////////////////////
// Base extension tests
///////////////////////////////////////////////////////////////////////////////
bool RNS::baseExtensionTest(int n_tests) {
    cout << "Starting base extension test." << endl;
    printVector(base1, "From base: ", true);
    printVector(base2, "To base: ", true); //new base needs to have a redundant modulus appended at input.
                                       //Treat the last channel as m_r
   
    int n_correct = n_tests;
 
    for (int i = 0; i < n_tests; i++) {
        //generate RNS number 
        BigUnsigned              num = rand() % M.toUnsignedInt();
        cout << "test integer:  " << num << endl;
        vector<BigUnsigned> num_rns = forwardConverter(num);

        printVector(num_rns, "Base 1 representation: ",true);
        

        //extend using Bajard
        vector<BigUnsigned> extension1 = RNS::baseExtension1(num_rns, base1, base2);
        printVector(extension1, "Bajard representation: ",true);

        //extend using Shenoy
        vector<BigUnsigned> extension2 = RNS::baseExtension2(extension1, base2, base1);
        printVector(extension2, "Shenoy re-conversion: ",true);

        //check
        BigUnsigned result = reverseConverter(extension2);
        cout << "Result: " << result << endl;

        if (result == num)
            cout << "Correct." << endl << endl;
        else if (result % M == num)
            cout << "Reduced result correct." << endl << endl;
        else {
            cout << "Incorrect." << endl << endl;
            n_correct -= 1;
        }

    }

    cout << endl << "Test finished. " << n_correct << "/" << n_tests << " correct." <<  endl;

    return true;
}

///////////////////////////////////////////////////////////////////////////////
// Tests shenoy complete base extension
///////////////////////////////////////////////////////////////////////////////
bool RNS::shenoyTest(int n_tests) {
    cout << "Starting Shenoy base extension test." << endl << endl;
    printVector(base2, "From base: ", true);
    printVector(base1, "To base: ", true); //Initial base needs to have a redundant modulus appended at input.
                                       //Treat the last channel as m_r

    int n_correct = n_tests;

    for (int i = 0; i < n_tests; i++) {
        //generate RNS number 
        BigUnsigned              num = rand() % M.toUnsignedInt();
        cout << "test integer:  " << num << endl;
        vector<BigUnsigned> num_rns = forwardConverter_extendedbase(num);

        printVector(num_rns, "Original RNS representation: ", true);

        //extend using Shenoy
        vector<BigUnsigned> shenoy_result = RNS::baseExtension2(num_rns, base2, base1);
        printVector(shenoy_result, "Shenoy extension: ", true);
        cout << "Integer result: " << reverseConverter(shenoy_result) % M << endl;

        //check
        vector<BigUnsigned> answer = forwardConverter(num);
        printVector(answer, "Correct representation: ", true);

        if (vectorsAreEqual(shenoy_result, answer))
            cout << "Correct." << endl << endl;
        else {
            cout << "Incorrect." << endl << endl;
            n_correct -= 1;
        }
    }

    cout << endl << "Test finished. " << n_correct << "/" << n_tests << " correct." << endl;

    return true;
}

///////////////////////////////////////////////////////////////////////////////
// Tests bajard incomplete base extension
///////////////////////////////////////////////////////////////////////////////
bool RNS::bajardTest(int n_tests) {
    cout << "Starting Bajard base extension test." << endl << endl;
    printVector(base1, "From base: ", true);
    printVector(base2, "To base: ", true); //Final base needs to have a redundant modulus appended at input.
                                          //Treat the last channel as m_r
    
    int n_correct = n_tests;

    for (int i = 0; i < n_tests; i++) {
        //generate RNS number 
        BigUnsigned              num = rand() % M.toUnsignedInt();
        cout << "test integer:  " << num << endl;
        vector<BigUnsigned> num_rns = forwardConverter(num);

        printVector(num_rns, "Original RNS representation: ", true);

        //extend using bajard
        vector<BigUnsigned> bajard_result = RNS::baseExtension1(num_rns, base1, base2);
        printVector(bajard_result, "Bajard extension: ", true);

        BigUnsigned integer_result = reverseConverter_extendedbase(bajard_result);
        cout << "Integer result: " << integer_result << endl;

        //check
        if (integer_result == num)
            cout << "Correct." << endl << endl;
        else if (integer_result % M == num)                             //need to determine alpha offset to test
            cout << "Reduced version correct." << endl << endl;
        else {
            cout << "Incorrect." << endl << endl;
            n_correct -= 1;
        }
    }

    cout << endl << "Test finished. " << n_correct << "/" << n_tests << " correct." << endl;

    return true;
}
///////////////////////////////////////////////////////////////////////////////
// Returns addition of RNS vectors 
///////////////////////////////////////////////////////////////////////////////
vector<BigUnsigned> RNS::add_RNS(vector<BigUnsigned> A, vector<BigUnsigned> B) {
    vector<BigUnsigned> ret_val;
    for (int i = 0; i < n_moduli; i++) {
        ret_val.push_back((A[i] + B[i]) % moduli[i]);
    }
    return ret_val;
}

///////////////////////////////////////////////////////////////////////////////
// Returns subtraction of RNS vectors 
///////////////////////////////////////////////////////////////////////////////
vector<BigUnsigned> RNS::sub_RNS(vector<BigUnsigned> A, vector<BigUnsigned> B) {
    vector<BigUnsigned> ret_val;
    for (int i = 0; i < n_moduli; i++) {
        ret_val.push_back((A[i] - B[i] + moduli[i]) % moduli[i]);
    }
    return ret_val;
}

///////////////////////////////////////////////////////////////////////////////
// Returns multiplication of RNS vectors (no reduction or overflow protection)
///////////////////////////////////////////////////////////////////////////////
vector<BigUnsigned> RNS::mult_RNS(vector<BigUnsigned> A, vector<BigUnsigned> B) {
    vector<BigUnsigned> ret_val;

    for (int i = 0; i < n_moduli; i++) {      
        ret_val.push_back((A[i] * B[i]) % moduli[i]); //standard RNS multiplication
    }

    return ret_val;
}


///////////////////////////////////////////////////////////////////////////////
// Returns addition of integers converting to and from RNS
///////////////////////////////////////////////////////////////////////////////
BigUnsigned RNS::add(BigUnsigned A, BigUnsigned B) {
    return reverseConverter(add_RNS(forwardConverter(A), forwardConverter(B)));
}

///////////////////////////////////////////////////////////////////////////////
// Returns subtraction of integers converting to and from RNS
///////////////////////////////////////////////////////////////////////////////
BigUnsigned RNS::sub(BigUnsigned A, BigUnsigned B) {
    return reverseConverter(sub_RNS(forwardConverter(A), forwardConverter(B)));
}

///////////////////////////////////////////////////////////////////////////////
// Returns multiplication of integers converting to and from RNS
///////////////////////////////////////////////////////////////////////////////
BigUnsigned RNS::mult(BigUnsigned A, BigUnsigned B) {
    return reverseConverter(mult_RNS(forwardConverter(A), forwardConverter(B)));
}

/////////////////////////////////////////////////////////////////////////////
// Prints results for the determine moduli functions
/////////////////////////////////////////////////////////////////////////////
void RNS::printModuliResults(int totalBits, vector<BigUnsigned> moduli) {
    //print results
    printVector(moduli, "Chosen RNS moduli: ");

    cout << "RNS moduli bits: ";
    BigUnsigned sum = 0;
    for (int i = 0; i < moduli.size(); i++) {
        BigUnsigned val = moduli[i].bitLength();
        cout << val << " ";
        sum += val;
    }
    cout << "= " << sum << " bits" << endl;
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

    //return twice as many moduli if needing a redundant base (plus redundant channel m_r)
    if (generate_redundant_base) n_moduli = n_moduli * 2 + 1;         

    for (mod = start; mod < end; mod += 2) {
        if (isCoprimeToVector(mod, moduli)) {
            moduli.push_back(mod);
        }
        if (moduli.size() == n_moduli) {
            printModuliResults(totalBits, moduli);
            return moduli;
        }
    }

    cout << "Could not find " << n_moduli << " " << bitsPer << "-bit coprime numbers." << endl;
    printModuliResults(totalBits, moduli);
    return moduli;
}

/////////////////////////////////////////////////////////////////////////////
// Tests all RNS mult/addition
/////////////////////////////////////////////////////////////////////////////
void RNS::RNS_test(RNS rns, bool mult) {
    BigUnsigned val, ans;
    BigUnsigned lim = rns.D1;

    for (BigUnsigned i = 0; i < lim; i++) {
        for (BigUnsigned j = 0; j < i; j++) {

            if (mult) {
                val = rns.mult(i, j);
                ans = i * j;
                if (val != ans) {
                    cout << i << " * " << j << " = " << val << " != " << ans << endl;
                    cout << "All values multiplying to " << ans - 1 << " with a dynamic range of " << rns.D1 << " calculated correctly." << endl;
                    return;
                }
            }

            else {
                val = rns.add(i, j);
                ans = i + j;
                if (val != ans) {
                    cout << i << " + " << j << " = " << val << " != " << ans << endl;
                    cout << "All values summing to " << ans - 1 << " with a dynamic range of " << rns.D1 << " calculated correctly." << endl;
                    return;
                }
            }
        }
    }

    cout << "All values up to " << lim << " with a dynamic range of " << rns.D1 << " calculated correctly." << endl;
}


///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////

RNS::RNS(vector<BigUnsigned> mod) {
    
    moduli   = mod;
    n_moduli = mod.size();

    // create montgomery reduction module for each moduli
    for (int i = 0; i < mod.size(); i++) {
        REDC redc_module(mod[i]);
        redc.push_back(redc_module);
    }
}
