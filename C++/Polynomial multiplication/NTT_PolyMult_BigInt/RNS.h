#pragma once

#include <vector>
#include "REDC.h"
#include "BigIntLibrary/BigIntegerLibrary.hh"

///////////////////////////////////////////////////////////////////////////////
/*
RNS class 
creates an RNS system for low bitwidth arithmetic

input parameter: 
    vector<int> moduli

main accesssed functions:
    int RNS.add(int A, int B)
    int RNS.sub(int A, int B)
    int RNS.mult(int A, int B)

ex.
    vector<int> moduli = {2,3,5,7};
    RNS rns_system(moduli);

    int A = 5;
    int B = 10;
    int Z = rns_system.add(A,B);

Z will hold the RNS addition of (A,B). 

NOTE: int is now replaced with BigUnsigned 
      to allow for arbitrarily large bitwidths.

*/

///////////////////////////////////////////////////////////////////////////////
// class 

class RNS
{
    private:



    public:

        // Functions

        // Variables
        int                                      n_base1, n_base2, n_base2_with_mr, total_bases;   //number in each RNS base
        std::vector<BigUnsigned>                   base1,   base2, base2_with_mr,  bases;   //Bases of coprimes
   
        std::vector<BigUnsigned> weights_base1, weights_base2, weights_base2_with_mr, weights_bases;   //int values of 1|0|0, 0|1|0, etc.
        std::vector<REDC> redc;             //montgomery reduction function for each moduli

        //Initialization
        RNS();  
        void initializeParameters(std::vector<BigUnsigned> moduli, BigUnsigned montgomery_reduction_modulus);
        void savetotextParameters();

        //Functions
        BigUnsigned getDynamicRange(std::vector<BigUnsigned> base);
        std::vector<BigUnsigned> forwardConverter(BigUnsigned num, std::vector<BigUnsigned> base);
        BigUnsigned reverseConverter(std::vector<BigUnsigned> num_RNS, std::vector<BigUnsigned> base);
        std::vector<BigUnsigned> getConversionWeights(std::vector<BigUnsigned> base);
        std::vector<std::vector<BigUnsigned>> RNS::forwardConverter_polynomial(std::vector<BigUnsigned> polynomial, std::vector<BigUnsigned> base);
        std::vector<BigUnsigned> RNS::reverseConverter_polynomial(std::vector<std::vector<BigUnsigned>> polynomial_rns, std::vector<BigUnsigned> base);

        BigUnsigned MOD_ADD(BigUnsigned A, BigUnsigned B, BigUnsigned MOD);
        BigUnsigned MOD_SUB(BigUnsigned A, BigUnsigned B, BigUnsigned MOD);
        BigUnsigned MOD_MULT(BigUnsigned A, BigUnsigned B, BigUnsigned MOD);
        std::vector<BigUnsigned> add_RNS(std::vector<BigUnsigned> A, std::vector<BigUnsigned> B, std::vector<BigUnsigned> base);
        std::vector<BigUnsigned> sub_RNS(std::vector<BigUnsigned> A, std::vector<BigUnsigned> B, std::vector<BigUnsigned> base);
        std::vector<BigUnsigned> mult_RNS(std::vector<BigUnsigned> A, std::vector<BigUnsigned> B, std::vector<BigUnsigned> base);


        BigUnsigned add(BigUnsigned A, BigUnsigned B, std::vector<BigUnsigned> base);
        BigUnsigned sub(BigUnsigned A, BigUnsigned B, std::vector<BigUnsigned> base);
        BigUnsigned mult(BigUnsigned A, BigUnsigned B, std::vector<BigUnsigned> base);

        //montgomery reduction variables
        BigUnsigned                           M,M_inv,D1, D1_inv, D2, m_r;
        std::vector<BigUnsigned>              M_rns, D1_rns;
        std::vector<BigUnsigned>                   D1_i, D2_j;

        BigUnsigned                           D1_inv_red_r;
        BigUnsigned                           two_inv_red_r;
        std::vector<BigUnsigned>              M_inv_red_i;
        std::vector<BigUnsigned>              M_red_j;

        std::vector<BigUnsigned>              D1_i_inv_red_i;
        std::vector<std::vector<BigUnsigned>> D1_i_red_j;
        std::vector<std::vector<BigUnsigned>> D2_j_red_i;
        std::vector<BigUnsigned>              D2_j_inv_red_j;

        std::vector<BigUnsigned>              D2_j_red_r;
        std::vector<BigUnsigned>              D2_red_i;
        std::vector<BigUnsigned>              D1_inv_red_j;
        BigUnsigned                           D2_inv_red_r;

        bool MULTIPLY_MODMULT_INPUT_BY_D  = true;   // Decides whether to multiply input to modmult by correction factor or not
        bool CORRECT_MODMULT_OUTPUT       = false;  // Decides whether to reduce output entirely or to leave offset
        bool CORRECT_BF_SUBTRACTION_INPUT = true;   // Decides whether to reduce input to butterfly subtraction (as that is the overflow problem)
        //montgomery reduction functions 
         //sets up parameters for rns REDC
        std::vector<BigUnsigned> weights_extendedbase;   //int values of 1|0|0, 0|1|0, etc.

        std::vector<BigUnsigned> baseExtension1(std::vector<BigUnsigned> num_RNS, std::vector<BigUnsigned> base, std::vector<BigUnsigned> newbase);
        std::vector<BigUnsigned> baseExtension2(std::vector<BigUnsigned> num_RNS, std::vector<BigUnsigned> base, std::vector<BigUnsigned> newbase);
        
        std::vector<BigUnsigned> modmult_RNS(std::vector<BigUnsigned> A, std::vector<BigUnsigned> B);

        void arithmetic_test(int n_tests);
        bool RNSmodmultTest(int n_tests);
        bool baseExtensionTest(int n_tests);
        bool shenoyTest(int n_tests);
        bool bajardTest(int n_tests);
        void converterTest(int n_tests);
        void butterflyRNStest(int n_tests);

        void baseExtension1_UnitTest(int n_tests, bool SET_CONSTS_TO_ZERO);
        void baseExtension2_UnitTest(int n_tests, bool SET_CONSTS_TO_ZERO);

        // Non-object dependent functions
        void printRNSval(std::vector<BigUnsigned> val_rns, std::vector<BigUnsigned> base = {}, bool printInIntform = false, std::string name = "");
        void printRNSvector(std::vector<std::vector<BigUnsigned>> list, std::string name = "",  std::vector<BigUnsigned> base = {}, bool printInIntform = true, bool printFullVector = false);
        std::vector<std::vector<BigUnsigned>> hadamard_product_RNS(std::vector<std::vector<BigUnsigned>> A, std::vector<std::vector<BigUnsigned>> B);
        std::vector<std::vector<BigUnsigned>> constant_vector_RNS(BigUnsigned length, BigUnsigned val, std::vector<BigUnsigned> base);
        static std::vector<BigUnsigned> determineRNSmoduli(int totalBits, int n_moduli);
        static std::vector<BigUnsigned> determineRNSmoduli2(int totalBits, int n_moduli, bool generate_redundant_base);
        static void printModuliResults(int totalBits, std::vector<BigUnsigned> moduli, int n_moduli = 4);
        std::vector<BigUnsigned> butterfly(BigUnsigned left, BigUnsigned right, BigUnsigned twiddlefactor, BigUnsigned modulus);
        std::vector<std::vector<BigUnsigned>> butterfly_rns(std::vector<BigUnsigned> left, std::vector<BigUnsigned> right, std::vector<BigUnsigned> twiddlefactor);

};
