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

using namespace std;

void RNS_test(RNS rns, bool mult) {
    int val, ans;
    int lim = rns.dR;

    for (int i = 0; i < lim; i++) {
        for (int j = 0; j < i; j++) {

            if (mult) {
                val = rns.mult(i, j);
                ans = i * j;
                if (val != ans) {
                    cout << i << " * " << j << " = " << val << " != " << ans << endl;
                    cout << "All values multiplying to " << ans - 1 << " with a dynamic range of " << rns.dR << " calculated correctly." << endl;
                    return;
                }
            }

            else {
                val = rns.add(i, j);
                ans = i + j;
                if (val != ans) {
                    cout << i << " + " << j << " = " << val << " != " << ans << endl;
                    cout << "All values summing to " << ans - 1 << " with a dynamic range of " << rns.dR << " calculated correctly." << endl;
                    return;
                }
            }
        }
    }
    
    cout << "All values up to " << lim << " with a dynamic range of " << rns.dR << " calculated correctly." << endl;
}

void NTT_test(vector<int> A, NTT ntt) {
    vector<int> Z;

    cout << endl << endl << "NTT TESTS:" << endl;
    printVector(A, "input:");
    cout << endl;

   //reference NTT
   Z = ntt.stupidcalculate(A);
   printVector(Z,"Stupid NTT:");
   Z = ntt.stupidcalculate(Z, true);
   printVector(Z,"result:");
   cout << endl;
   /*
   // efficient NTT 1
   Z = ntt.calculate(A);
   printVector(Z, "Paper1 NTT:");
   Z = ntt.calculate(Z, true);
   printVector(Z, "result:");
   cout << endl;

   // efficient NTT 2
   Z = ntt.calculate2(A);
   printVector(Z, "Paper2 NTT:");
   Z = ntt.calculate2(Z, true);
   printVector(Z, "result:");
   cout << endl;

   // efficient NTT 3
   Z = ntt.calculate3(A);
   printVector(Z, "Poppelmann thesis NTT:");
   Z = ntt.calculate3(Z, true);
   printVector(Z, "result:");
   cout << endl;

   // efficient NTT 4
   Z = ntt.calculate4(A);
   printVector(Z, "Nayuki NTT:");
   Z = ntt.calculate4(Z, true);
   printVector(Z, "result:");
   cout << endl;
   */
   // efficient NTT 5
   Z = ntt.calculate5(A);
   printVector(Z, "Nayuki NTT with RNS:");
   Z = ntt.calculate4(Z, true);
   printVector(Z, "result:");
   cout << endl;
}

vector<int> polynomial_multiply(vector<int> A, vector<int> B, NTT ntt_system) {
   
    cout << endl << "POLYNOMIAL MULTIPLICATION:" << endl;
    vector<int> C1, A_ntt1, B_ntt1, C_ntt1, C2, A_ntt2, B_ntt2, C_ntt2;

    //Stupid NTT
    A_ntt1 = ntt_system.stupidcalculate(A);
    B_ntt1 = ntt_system.stupidcalculate(B);
    C_ntt1 = hadamard_product(A_ntt1, B_ntt1, ntt_system.modulus);
    C1     = ntt_system.stupidcalculate(C_ntt1, true);

    // Print results
    cout << endl << "STUPID NTT:" << endl;
    printVector(A, "input A:");
    printVector(B, "input B:");
    printVector(A_ntt1, "A NTT:");
    printVector(B_ntt1, "B NTT:");
    printVector(C_ntt1, "NTT Product:");
    cout << endl;
    printVector(C1, "Result:");

    //Butterfly NTT with RNS
    A_ntt2 = ntt_system.calculate5(A);
    B_ntt2 = ntt_system.calculate5(B);
    C_ntt2 = hadamard_product(A_ntt2, B_ntt2, ntt_system.modulus);
    C2     = ntt_system.calculate5(C_ntt2, true);

    // Print results
    cout << endl << "Butterfly NTT with RNS:" << endl;
    printVector(A, "input A:");
    printVector(B, "input B:");
    printVector(A_ntt2, "A NTT:");
    printVector(B_ntt2, "B NTT:");
    printVector(C_ntt2, "NTT Product:");
    cout << endl;
    printVector(C2, "result:");

    // Compare vectors
    cout << boolalpha;
    cout << "A NTTs equal: " << compareVectors(A_ntt1, A_ntt2) << endl;
    cout << "B NTTs equal: " << compareVectors(B_ntt1, B_ntt2) << endl;
    cout << "NTT products equal: " << compareVectors(C_ntt1, C_ntt2) << endl << endl;
    cout << "Result equal: " << compareVectors(C1, C2) << endl;

    return C2;
}

/////////////////////////////////////////////////////////////////////////
int main()
{
    //Parameters
    int length = 256;                          /* 256 length of polynomial. Needs to be power of 2. It is consistently
                                                4096 (2^12) in papers.*/

    int minimum_modulus = 649;               /* The polynomial modulus. Has to be greater than the polynomial
                                                length and greater than each polynomial vector value. The value 
                                                is possibly increased in ntt.new_modulus() to become prime. The
                                                modulus 12,289 (13.6 bits) is commonly used in lattice-based 
                                                cryptography. It is in the order of 60-180-372 bits for FHE schemes.*/

    int n_bits = log2(minimum_modulus);      /* Polynomial coefficient bit depth. See for standard depths: 
                                                "FPGA-based High-Performance Parallel Architecture for Homomorphic Computing on Encrypted Data"
                                                "HEAX: An Architecture for Computing on Encrypted Data"
                                             */
    
    
    int mod   = 673;
    REDC redc = REDC(mod);
    redc.test();

    return 0;

    // Create test polynomials
 
    vector<int> polynomial_A, polynomial_B, polynomial_C, rns_moduli;
    polynomial_A = sample_polynomial(length, minimum_modulus);   
    polynomial_B = sample_polynomial(length, minimum_modulus);    
   // rns_moduli   = generateModuli(length, minimum_modulus);      
    rns_moduli     = {128,127,63,31};     
    
                                                                     /* 
                                                                     Must have RNS dynamic range > modulus^2 to allow for multiplication. 
                                                                     The dR is also multiplied by (w_n)^i in RNS.reverseconverter(), so caution with 
                                                                     overflow. Must have minimum_modulus = modulus to generate the moduli 
                                                                     correctly so this function should probably be changed to be called
                                                                     after ntt.findmodulus().
                                                                     */
   
    //if example polynomial is used
    /*
    polynomial_A = {4,1,4,2,1,3,5,6};
    polynomial_B = {6,1,8,0,3,3,9,8};
    length     = polynomial_A.size();                              
    rns_moduli = {256,255,127,31};  // Has dR of 28 bits. Doesn't work because of reverseConverter multiplication overflow.
    rns_moduli = {128,127,63,31};   // Has dR of 25 bits.
    */

    // Create NTT & RNS systems
    RNS rns_system(rns_moduli);
    NTT ntt_system(length, minimum_modulus, rns_system);
    ntt_system.printParameters();

    // Multiply using butterfly NTT with RNS
    polynomial_C = polynomial_multiply(polynomial_A, polynomial_B, ntt_system);
   
    return 0;
}
