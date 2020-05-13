#pragma once

#include <vector>
#include "REDC.h"


///////////////////////////////////////////////////////////////////////////////
/*
RNS class
    creates an RNS system for low bitwidth arithmetic

parameter: 
    vector<int> moduli

main functions:
    int RNS.add(int A, int B)
    int RNS.sub(int A, int B)
    int RNS.mult(int A, int B)


ex.
    vector<int> moduli = {2,3,5,7};
    RNS rns_system(moduli);

    int A = 5;
    int B = 10;

    int Z = rns_system.add(A,B);

*/

///////////////////////////////////////////////////////////////////////////////
// Non-class functions
std::vector<int> generateModuli(int size, int mod);

///////////////////////////////////////////////////////////////////////////////
// class
class RNS
{
    private:
        // Variables

        
        // Functions
        int getDynamicRange();

        std::vector<int> getSingleResidues();


    public:
        // Variables
        std::vector<int> moduli;    //base of coprimes
        std::vector<int> weights;   //int values of 1|0|0, 0|1|0, etc.
        std::vector<REDC> redc;     //montgomery reduction function for each moduli
        int dR;                     //dynamic range 
        int n_moduli;               //number of coprimes 

        //Functions
        RNS(std::vector<int> mod = {2,3,5,7});  //constructor

        //std::vector<int> optimizeModuli(int size, int mod);    // No longer in class 
        std::vector<int> forwardConverter(int num);
        int reverseConverter(std::vector<int> num_RNS);

        std::vector<int> add_RNS(std::vector<int> A, std::vector<int> B);
        std::vector<int> sub_RNS(std::vector<int> A, std::vector<int> B);
        std::vector<int> mult_RNS(std::vector<int> A, std::vector<int> B);

        int add(int A, int B);
        int sub(int A, int B);
        int mult(int A, int B);

};

