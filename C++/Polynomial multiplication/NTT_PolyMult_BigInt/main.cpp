// NTT_PolyMult.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "general_functions.h"
#include "RNS.h"
#include "NTT.h"
#include "REDC.h"
#include "BigIntLibrary/BigIntegerLibrary.hh"

using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Comments
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
Big integer problem:
   It is difficult to load other bigint libraries and it is not conductive to knowing how to implement in hardware. An
   unsigned long long is 64 bits. With 32 bit multipliers in an RNS system, there will be no overflow. You only have to worry
   about calculating RNS dynamic range (32bits * n_residues) and calculating modular multiplication reduced using the largest
   modulus.

   edit: The need to use big ints carries down and it is hard to separate, using BigUnsigned library


Where to implement montgomery reduction with regards to RNS:
    The RNS system automatically wraps its multiplication results so it was a thought to use it without additional
    reduction. However, it  wraps with respect to the dynamic range. Given that the desired modulus is different & prime, I think
    it is impossible to match the dR and modulus. Montgomery reduction still needs implemented for each of the RNS channels' different
    moduli (likely 30-60 bit each). It also needs done for the large modulus reduction when doing the hadamard.
    additionally needs implemented for



if example polynomial is used:
    polynomial_A = {4,1,4,2,1,3,5,6};
    polynomial_B = {6,1,8,0,3,3,9,8};
    length     = polynomial_A.size();
    rns_moduli = {256,255,127,31};             // Has dR of 28 bits.
    rns_moduli = {128,127,63,31};              // Has dR of 25 bits.

Using Big Integer Library by:
    https://mattmccutchen.net/bigint/index.html#resources

*/



/////////////////////////////////////////////////////////////////////////////
// Code to multiply two polynomials
/////////////////////////////////////////////////////////////////////////////

vector<BigUnsigned> polynomial_multiply(vector<BigUnsigned> A, vector<BigUnsigned> B, NTT ntt_system, bool doStupidNTT = false, bool printFullVector = true) {
   
    cout << endl << "POLYNOMIAL MULTIPLICATION:" << endl;
    vector<BigUnsigned> C1, A_ntt1, B_ntt1, C_ntt1, C2, A_ntt2, B_ntt2, C_ntt2;

    if (doStupidNTT) {
        //Stupid NTT
        A_ntt1 = ntt_system.stupidcalculate(A);
        B_ntt1 = ntt_system.stupidcalculate(B);
        C_ntt1 = hadamard_product(A_ntt1, B_ntt1, ntt_system.modulus);
        C1 = ntt_system.stupidcalculate(C_ntt1, true);

        // Print results
        cout << endl << "STUPID NTT:" << endl;
        printVector(A, "input A:", printFullVector);
        printVector(B, "input B:", printFullVector);
        printVector(A_ntt1, "A NTT:", printFullVector);
        printVector(B_ntt1, "B NTT:", printFullVector);
        printVector(C_ntt1, "NTT Product:", printFullVector);
        cout << endl;
        printVector(C1, "Result:", printFullVector);
        cout << endl;
    }

    //Butterfly NTT with RNS
    A_ntt2 = ntt_system.calculate(A);
    B_ntt2 = ntt_system.calculate(B);
    C_ntt2 = hadamard_product(A_ntt2, B_ntt2, ntt_system.modulus);
    C2     = ntt_system.calculate(C_ntt2, true);

    // Print results
    cout << endl << "Butterfly NTT with RNS:" << endl;
    printVector(A, "input A:", printFullVector);
    printVector(B, "input B:", printFullVector);
    printVector(A_ntt2, "A NTT:", printFullVector);
    printVector(B_ntt2, "B NTT:", printFullVector);
    printVector(C_ntt2, "NTT Product:", printFullVector);
    cout << endl;
    printVector(C2, "result:", printFullVector);
    cout << endl;
    
    
    if (doStupidNTT) {
        // Compare vectors
        cout << boolalpha; //change output 0/1 to true/false
        cout << "A NTTs equal: " << vectorsAreEqual(A_ntt1, A_ntt2) << endl;
        cout << "B NTTs equal: " << vectorsAreEqual(B_ntt1, B_ntt2) << endl;
        cout << "NTT products equal: " << vectorsAreEqual(C_ntt1, C_ntt2) << endl << endl;
        cout << "Result equal: " << vectorsAreEqual(C1, C2) << endl;
    }
    return C2;
}

/////////////////////////////////////////////////////////////////////////////
// Code to multiply two polynomials
/////////////////////////////////////////////////////////////////////////////

vector<BigUnsigned> negative_wrapped_convolution(vector<BigUnsigned> A, vector<BigUnsigned> B, NTT ntt_system, bool doStupidNTT = false, bool printFullVector = true) {

    cout << endl << "NEGATIVE WRAPPED CONVOLUTION:" << endl;
    vector<BigUnsigned> A_prime, B_prime, C1, C1_prime, A_ntt1, B_ntt1, C_ntt1, C2, C2_prime, A_ntt2, B_ntt2, C_ntt2;

    //multiply inputs by powers of phi
    A_prime = mult_by_power(A, ntt_system.phi, ntt_system.modulus);
    B_prime = mult_by_power(B, ntt_system.phi, ntt_system.modulus);

    if (doStupidNTT) {
        //Stupid NTT
        A_ntt1 = ntt_system.stupidcalculate(A_prime);
        B_ntt1 = ntt_system.stupidcalculate(B_prime);
        C_ntt1 = hadamard_product(A_ntt1, B_ntt1, ntt_system.modulus);
        C1_prime = ntt_system.stupidcalculate(C_ntt1, true);

        //negative wrapped inverse
        C1 = mult_by_power(C1_prime, ntt_system.phi_inv, ntt_system.modulus);

        // Print results
        cout << endl << "STUPID NTT:" << endl;
        printVector(A, "input A:", printFullVector);
        printVector(B, "input B:", printFullVector);
        printVector(A_prime, "input A scaled by phi:", printFullVector);
        printVector(B_prime, "input B scaled by phi:", printFullVector);
        printVector(A_ntt1, "A NTT:", printFullVector);
        printVector(B_ntt1, "B NTT:", printFullVector);
        printVector(C_ntt1, "NTT Product:", printFullVector);
        cout << endl;
        printVector(C1_prime, "Result unscaled:", printFullVector);
        printVector(C1, "Result:", printFullVector);
        cout << endl;
    }

    //Butterfly NTT with RNS
    A_ntt2 = ntt_system.calculate(A_prime);
    B_ntt2 = ntt_system.calculate(B_prime);
    C_ntt2 = hadamard_product(A_ntt2, B_ntt2, ntt_system.modulus);
    C2_prime = ntt_system.calculate(C_ntt2, true);

    //negative wrapped inverse
    C2 = mult_by_power(C2_prime, ntt_system.phi_inv, ntt_system.modulus);

    // Print results
    cout << endl << "Butterfly NTT with RNS:" << endl;
    printVector(A, "input A:", printFullVector);
    printVector(B, "input B:", printFullVector);
    printVector(A_prime, "input A scaled by phi:", printFullVector);
    printVector(B_prime, "input B scaled by phi:", printFullVector);
    printVector(A_ntt2, "A NTT:", printFullVector);
    printVector(B_ntt2, "B NTT:", printFullVector);
    printVector(C_ntt2, "NTT Product:", printFullVector);
    cout << endl;
    printVector(C2_prime, "Result unscaled:", printFullVector);
    printVector(C2, "Result:", printFullVector);
    cout << endl;


    if (doStupidNTT) {
        // Compare vectors
        cout << boolalpha; //change output 0/1 to true/false
        cout << "A NTTs equal: " << vectorsAreEqual(A_ntt1, A_ntt2) << endl;
        cout << "B NTTs equal: " << vectorsAreEqual(B_ntt1, B_ntt2) << endl;
        cout << "NTT products equal: " << vectorsAreEqual(C_ntt1, C_ntt2) << endl << endl;
        cout << "Result equal: " << vectorsAreEqual(C1, C2) << endl;
    }
    return C2;
}
/////////////////////////////////////////////////////////////////////////////
// MAIN
/////////////////////////////////////////////////////////////////////////////

int main()
{
    //Input parameters
    BigUnsigned length = 8;                     /*4096 Length of polynomial. Needs to be power of 2 and needs to be
                                                compatible with the chosen modulus otherwise errors occur. It is 
                                                consistently 128,256,512 for moduli of 3329,7681,12289 in lattice
                                                crptography papers and around 4096 (2^12) in FHE papers. */

    //BigUnsigned base = 2;
    //BigUnsigned exp = 64;
    //BigUnsigned off = 59;
    //BigUnsigned minimum_modulus = pow(base,exp) - off; 
    
    BigUnsigned minimum_modulus = 673;           /*673The polynomial modulus. Has to be greater than the polynomial
                                                length and greater than each polynomial vector value. The value 
                                                is possibly increased in ntt.new_modulus() to become prime. The
                                                modulus 12289 (13.6 bits) is commonly used in lattice-based 
                                                cryptography. It is in the order of 60-180-372 bits for FHE schemes.
                                                For known large primes, see https://primes.utm.edu/lists/2small/.

                                                getSingleResidues() caps at about 15 bits per moduli. Should have min_mod/n_mod <= 15;

                                                ex. BigUnsigned m = 2;
                                                    m = (m << 54) - 33;         //a known 182 bit "near power of 2" prime 
                                                */
    

    int n_moduli = 4;                        /*// Number of RNS channels. Increasing this number increases parallelism and reduces an N by N-bit
                                             multiplication (N = n_bits) into k smaller N/k x N/k multiplications (k = n_moduli). This is only true
                                             if each channel takes on an equal number of bits as in determineRNSmoduli2(dR_bits, n_moduli). The other
                                             moduli generating function determineRNSmoduli(dR_bits, n_moduli) looks for powers of 2 and will resultingly have
                                             unequal bitwidths per channel as the trade off of easier hardware. A benefit of parallelism is that smaller
                                             multiplication can often be done by table look up if in the range of 8 bits for example.
                                             */

    // Derived parameters
    int n_bits = minimum_modulus.bitLength();     /* Polynomial coefficient bit depth. This is only used to determine the minimum
                                                  necessary span of the RNS base. If the minimum modulus happens to be recalculated,
                                                  n_bits will be underrepresented and RNS overflow may occur. The BigUnsigned accessor 
                                                  bitLength() returns how many bits a number requires to represent it, however the number of 
                                                  bits it needs to be represented in an RNS system will be somewhere between n_bits and n_bits - 1.
                                                  n_bits is therefore a safe cieling value. See for standard bit depths: 
                                                  - "FPGA-based High-Performance Parallel Architecture for Homomorphic Computing on Encrypted Data"
                                                  - "HEAX: An Architecture for Computing on Encrypted Data"
                                                */
    
    int dR_bits = n_bits * 2;                   /* Number of bits needed covered by RNS operation. An RNS arithmetic operation needs to be able to multiply 
                                                modulus x modulus without overflow, therefore the bit depth needs to be twice that of the minimum modulus. 
                                                */
    //REDC barrett(111110509);
    //barrett.modmultTest_barrett(100); 
    //return 0;

    
    //Calculates rns moduli, extended base, and m_r
    vector<BigUnsigned> bases;
    bases = {4294967291,4294967279,4294967231,4294967197,4294967189,4294967161,4294967143,4294967111, 4294967087}; //32 bit primes
    
    //bases = RNS::determineRNSmoduli2(dR_bits, n_moduli, true);
     
    RNS rns;                                         
    rns.initializeParameters(bases,minimum_modulus); /*Instantiation creates an rns system that allow basic functions such as add/sub/mult, provided
                                                     a base with function calls. To generate internal variables and to use montgomery reduction,
                                                     the initializeParameters function needs called. It requires base1 (size n), base2 (size n), and
                                                     m_r (size 1) in a single vector. The moduli can be autogenerated using determineRNSmoduli2() or it 
                                                     can be taken from a prime number list. The second parameter is the reducion modulus for the NTT. 
                                                     */

    //rns.savetotextParameters();
    //printVector(rns.D1_i_red_j[0], "const: ", true, true, 32);
    
    //rns.baseExtension1_UnitTest(10, false);
    //rns.baseExtension2_UnitTest(10, false);
    
    //printVector(rns.D2_j_inv_red_j, "Const: ", true, true, 32);
    //printVector(rns.D2_j_red_r , "Const: ", true, true, 32);
    //printVal(rns.D2_inv_red_r, "Const: ", true, true, 32);
    //printVector(rns.D2_red_i, "Const: ", true, true, 32);
    //saveVectorVectorToTextfile(rns.D2_j_red_i,"D2_J_RED_I.txt", 32);

    //return 0;

    //rns.converterTest(100);     //convert to and from RNS:       100% accuracy
    //rns.shenoyTest(100);
    //rns.bajardTest(100);
    //rns.baseExtensionTest(100); //base extends to base2 and back: 75% accuracy because result of bajard is a multiple of d1 (fixed when used in mod mult)
    //rns.arithmetic_test(100);   // Tests +/-/*:                  100% accuracy (IF subtraction is reduced at some juncture) 
    
    rns.MULTIPLY_MODMULT_INPUT_BY_D  = true;   // Multiply modmult input by correction factor to get A*B. or let result be A*B*D^-1 if false.
    rns.CORRECT_MODMULT_OUTPUT       = false;  // Reduce modmult output again to get fully reduced result  
    rns.CORRECT_BF_SUBTRACTION_INPUT = true;   // Decides whether to reduce input to butterfly subtraction (as that is the overflow problem)

    //rns.RNSmodmultTest(100);    //RNS montgomery reduction outputs answer + (k*n_moduli+2)M. Test for A*B or A*B*D^-1 depeneding on MULTIPLY_MODMULT_INPUT_BY_D flag.  
    rns.butterflyRNStest(100);    //Tests RNS butterfly:           100% accuracy if modmult is corrected.
    return 0;
                                                                     /* 
                                                                     Must have RNS dynamic range > modulus^2 for RNS multiplication. 
                                                                     The dR is multiplied by (w_n)^i in RNS.reverseconverter(), so caution with 
                                                                     overflow. Must have minimum_modulus = modulus to generate the moduli 
                                                                     correctly so this function should probably be changed to be called
                                                                     after ntt.findmodulus() if already known prime modulus are not used.
                                                                     */


    // Create NTT & RNS systems
    NTT ntt(length, minimum_modulus, rns, true); //it is important to use a prime as the minimum modulus if it is large. It will search for a new prime O.W.
    //ntt.printParameters();

    //ntt.CORRECT_LAST_NTT_RUN = true;// true; //enables CORRECT_MODMULT_OUTPUT on last stage of NTT
    //ntt.NTT_test(100);         //RNS NTT has 100% accuracy when compared to all other NTTs.
    
    return 0;

    // Multiply using butterfly NTT with RNS
    //bool doStupidNTT     = false;
    //bool printFullVector = true;

    //NTT ntt_system_padded(length * 2, minimum_modulus, rns_system);
    //polynomial_A_padded = zero_pad(polynomial_A);
    //polynomial_B_padded = zero_pad(polynomial_B);
 
    //polynomial_C1 = polynomial_multiply(polynomial_A, polynomial_B, ntt_system, doStupidNTT, printFullVector);
    //uses w = 2n root of unity (double length)

    //multiply using negative wrapped convolution
    // polynomial_C2 = negative_wrapped_convolution(polynomial_A, polynomial_B, ntt_system, doStupidNTT, printFullVector);
   
     //polynomial_C2 = ntt_system.calculate(polynomial_A);
   
   //used w = n root of unity
   // printVector(polynomial_C1, "Polynomial multiply padded: ");
   
    //printVector(polynomial_C2, "Negative wrapped conv     : ");
   // return 0;
}

