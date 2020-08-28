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
        // Functions
        BigUnsigned getDynamicRange();

        std::vector<BigUnsigned> getSingleResidues(std::vector<BigUnsigned> mod);


    public:
        // Variables
        std::vector<BigUnsigned> moduli;    //base of coprimes
        std::vector<BigUnsigned> weights;   //int values of 1|0|0, 0|1|0, etc.
        std::vector<REDC> redc;             //montgomery reduction function for each moduli
        BigUnsigned dR;                     //dynamic range 
        int n_moduli;                       //number of coprimes 

        //Functions
        RNS(std::vector<BigUnsigned> mod = {3,5,7,11});  //constructor

        std::vector<BigUnsigned> forwardConverter(BigUnsigned num);
        BigUnsigned reverseConverter(std::vector<BigUnsigned> num_RNS);


        std::vector<BigUnsigned> add_RNS(std::vector<BigUnsigned> A, std::vector<BigUnsigned> B);
        std::vector<BigUnsigned> sub_RNS(std::vector<BigUnsigned> A, std::vector<BigUnsigned> B);
        std::vector<BigUnsigned> mult_RNS(std::vector<BigUnsigned> A, std::vector<BigUnsigned> B);


        BigUnsigned add(BigUnsigned A, BigUnsigned B);
        BigUnsigned sub(BigUnsigned A, BigUnsigned B);
        BigUnsigned mult(BigUnsigned A, BigUnsigned B);

        //montgomery reduction variables
        int                                   n_base1, n_base2, total_bases;
        std::vector<BigUnsigned>                   base1, base2, bases;
        BigUnsigned                           M,M_inv,D1, D1_inv, D2, m_r;
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


        //montgomery reduction functions 
         //sets up parameters for rns REDC
        std::vector<BigUnsigned> weights_extendedbase;   //int values of 1|0|0, 0|1|0, etc.
        void initializeREDCParameters(int n_moduli, BigUnsigned modulus, std::vector<BigUnsigned> bases);
        std::vector<BigUnsigned> forwardConverter_extendedbase(BigUnsigned num);
        BigUnsigned reverseConverter_extendedbase(std::vector<BigUnsigned> num_RNS);
        std::vector<BigUnsigned> baseExtension1(std::vector<BigUnsigned> num_RNS, std::vector<BigUnsigned> base, std::vector<BigUnsigned> newbase);
        std::vector<BigUnsigned> baseExtension2(std::vector<BigUnsigned> num_RNS, std::vector<BigUnsigned> base, std::vector<BigUnsigned> newbase);
        
        std::vector<BigUnsigned> modmult_RNS_bajard(std::vector<BigUnsigned> A, std::vector<BigUnsigned> B);
        std::vector<BigUnsigned> modmult_RNS_split(std::vector<BigUnsigned> A, std::vector<BigUnsigned> B);
        std::vector<BigUnsigned> modmult_RNS(std::vector<BigUnsigned> A, std::vector<BigUnsigned> B);
        bool RNSmodmultTest(int n_tests);
        bool baseExtensionTest(int n_tests);
        bool shenoyTest(int n_tests);
        bool bajardTest(int n_tests);

        // Non-object dependent functions
        static std::vector<BigUnsigned> determineRNSmoduli(int totalBits, int n_moduli);
        static std::vector<BigUnsigned> determineRNSmoduli2(int totalBits, int n_moduli, bool generate_redundant_base);
        static void printModuliResults(int totalBits, std::vector<BigUnsigned> moduli);
        static void RNS_test(RNS rns, bool mult);
};
