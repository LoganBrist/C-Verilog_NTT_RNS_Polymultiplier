#include "NTT.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include "general_functions.h"
#include "RNS.h"

using namespace std;

///////////////////////////////////////////////////////////////
// Print Parameters
// prints NTT parameters
///////////////////////////////////////////////////////////////

void NTT::printParameters() {
    cout << "Entered modulus:" << min_mod << endl; 
    cout << "Used modulus:" << modulus << endl;
    cout << "n:" << vec_length << endl;
    cout << "nth root of unity:" << w_n << endl;
    cout << "nth root inverse:" << w_n_inv << endl << endl;


}
///////////////////////////////////////////////////////////////
// Factorize
// Returns a vector of prime factors of integer 
///////////////////////////////////////////////////////////////
vector<int> NTT::factorize(int n) {
    vector<int> factors;

    //find number of 2s
while (n % 2 == 0) {
    factors.push_back(2);
    n = n / 2;
}
//now n is odd
for (int i = 3; i <= sqrt(n); i += 2) {
    while (n % i == 0) {
        factors.push_back(i);
        n = n / i;
    }
}
//if n is prime
if (n > 2)
factors.push_back(n);

return factors;
}

///////////////////////////////////////////////////////////////
// Bit Reverse
// Rearrange vector based on bit reversal of the indices
///////////////////////////////////////////////////////////////
vector<int> NTT::bitReverse(vector<int> A) {
    vector<int> Z = A; //return vector
    int N = Z.size();
    int n_bits = ceil(log2(N));

    for (int i = 0; i < N; i++) {
        int val = 0;
        int idx = 0;
        for (int j = 0; j < n_bits; j++) {
            val = i & (1 << j);
            if (val)
                idx |= 1 << ((n_bits - 1) - j);
        }
        Z[i] = A[idx];
    }
    return Z;
}

///////////////////////////////////////////////////////////////
// Modular exponentiation (from wikipedia)
///////////////////////////////////////////////////////////////
int NTT::pow_mod(int base, int ex, int mod) {
    int result = 1;
    int ex_prime = 0;

    while (ex_prime < ex) {
        ex_prime++;
        result = (base * result) % mod;
    }
    return result;
}

///////////////////////////////////////////////////////////////
// Modular square root (stupid way) (Can do Tonelli - Shanks algorithm)
///////////////////////////////////////////////////////////////
int NTT::sqrt_mod(int num, int mod) {
    for (int i = 0; i < mod; i++) {
        if (pow_mod(i, 2, mod) == num)
            return i;
    }

    // error
    return -1;
}
///////////////////////////////////////////////////////////////////////////////
//Create constant to vector
///////////////////////////////////////////////////////////////////////////////
vector<int> NTT::constant_vector(int length, int val) {
    vector<int> Z;
    for (int i = 0; i < length; i++) {
        Z.push_back(val);
    }
    return Z;
}

//////////////////////////////////////////////////////////////////////////////
// Modular Hadamard Product
// pointwise modular multiplication between vectors 
///////////////////////////////////////////////////////////////////////////////
vector<int> NTT::hadamard_product(vector<int> a, vector<int> b, int moduli) {
    vector<int> Z;

    for (int i = 0; i < a.size(); i++) {
        Z.push_back((a[i] * b[i]) % moduli);  
    }

    return Z;
}

/////////////////////////////////////////////////////////////////
// Finds nth root of unity given vector of length n and modulus M
///////////////////////////////////////////////////////////////
int NTT::mod_inverse(int w_n, int M) {
    for (int a = 0; a < M; a++) {
        if ((a * w_n) % M == 1)
            return a;
    }

    //error
    return 0;
}

/////////////////////////////////////////////////////////////////
// Multiply vector by powers of val
///////////////////////////////////////////////////////////////
vector<int> NTT::mult_by_power(vector<int> vec, int val, int modulus) {
    for (int i = 0; i < vec.size(); i++) {
        vec[i] = (vec[i] * pow_mod(val, i, modulus)) % modulus;
    }

    return vec;
}
/////////////////////////////////////////////////////////////////
// generate table holding powers of phi
///////////////////////////////////////////////////////////////
vector<int> NTT::generate_phi_table(int n, int phi, int modulus) {
    vector<int> table;

    for (int i = 0; i < n; i++) {
        table.push_back(pow_mod(phi, i, modulus));
    }

    return table;//bitReverse(table);
}
////////////////////////////////////////////////////////////////////////////////
// Checks and possibly readjusts modulus given min modulus M and vector length n
////////////////////////////////////////////////////////////////////////////////
int NTT::new_modulus(int vec_length, int min_modulus) {

    int k = floor((min_modulus - 1) / vec_length);  //starting value to start the new_modulus off close to >= min_modulus
    int new_modulus = k * vec_length + 1;

    //increment k until modulus is prime and greater than the minimum modulus
    while ((new_modulus < min_modulus) || (factorize(new_modulus).size() > 1)) {
        k++;
        new_modulus = k * vec_length + 1;
    }

    return new_modulus;
}

///////////////////////////////////////////////////////////////
// Tests to find the nth root of unity
///////////////////////////////////////////////////////////////
int NTT::find_root_of_unity(int vec_length, int modulus) {
    //Get factors of n (vec_length)
    vector<int> factors = factorize(vec_length);

    //remove reaccuring factors
    sort(factors.begin(), factors.end());
    factors.erase(unique(factors.begin(), factors.end()), factors.end());


    for (int a = 1; a <= modulus; a++) {
        //test 1
        bool flag1 = (pow_mod(a, vec_length, modulus) == 1);

        //test 2: try for each prime factor of n
        bool flag2 = all_of(factors.begin(), factors.end(), [&](int prime) {return pow_mod(a, vec_length / prime, modulus) != 1; });

        if (flag2 && flag1)
            return a;
    }
    /*
    // 'a' is the candidate root
    for (int a = 1; a <= modulus; a++) {
        //test 1: try for n
        bool flag1 = (pow_mod(a, vec_length, modulus) == 1); // a^n mod N = 1?

        //test 2: try for each prime factor of n
        int flag2 = 0;
        for (int p = 0; p < factors.size(); p++) {
            flag2 += (pow_mod(a, vec_length / factors[p], modulus) != 1); // a^(n/p) mod N != 1?
        }

        //tests passed
        if ((flag2 == factors.size()) && flag1) {
            return a;
        }
    }
    */
    //error
    return -1;
}

///////////////////////////////////////////////////////////////
// Find generator
///////////////////////////////////////////////////////////////
bool NTT::is_generator(int val, int totient, int mod) {
    vector<int> factors = factorize(totient);

    //test 1
    bool flag1 = (pow_mod(val, totient, mod) == 1);

    //test 2: try for each prime factor of n
    bool flag2 = all_of(factors.begin(), factors.end(), [&](int prime) {return pow_mod(val, totient / prime, mod) != 1; });
  
    return (flag1 && flag2);
}

int NTT::find_generator(int totient, int mod) {
    for (int i = 1; i < mod; i++) {
        if (is_generator(i, totient, mod))
            return i;
    }
}
///////////////////////////////////////////////////////////////
// Tests to find the nth root of unity with generator
///////////////////////////////////////////////////////////////
int NTT::find_root_of_unity2(int degree, int mod) {
    int totient = mod - 1;
    int generator = find_generator(totient, mod);
    int root = pow_mod(generator, totient / degree, mod);
    return root;
}
///////////////////////////////////////////////////////////////////////////////
// Reference NTT (inefficient)
///////////////////////////////////////////////////////////////////////////////
vector<int> NTT::stupidcalculate(vector<int> A, bool inverse) {;
    vector<int> Z; //return polynomial
    
    int vec_len = A.size();  //This should be same as NTT.vec_length but just in case
    
    int omeg = w_n;

    if (inverse) {
        omeg = w_n_inv;
    }

    for (int i = 0; i < vec_len; i++) {
        int val = 0;
        for (int j = 0; j < vec_len; j++) {
            val += A[j] * pow_mod(omeg, i * j, modulus);
        }
        Z.push_back(val % modulus);
        cout << i * 100 / vec_len << "% of studid NTT done.\r";
    }
    cout << "100% of studid NTT done." << endl;

    if (inverse) {
        Z = hadamard_product(Z, constant_vector(vec_len, mod_inverse(vec_len,modulus)), modulus);
    }

    return Z;
}


///////////////////////////////////////////////////////////////////////////////
// NTT1  (not working)
// 
// Has to be power of 2
///////////////////////////////////////////////////////////////////////////////
vector<int> NTT::calculate(vector<int> A, bool inverse) {
    vector<int> Z; //return polynomial

    int vec_len = A.size();  //This should be same as NTT.vec_length but just in case

    Z = bitReverse(A);

    int omeg = w_n;
    if (inverse)
        omeg = w_n_inv;
    for (int m = 2; m <= vec_len; m *= 2) {
        int w_m = pow_mod(omeg, vec_len / m, modulus);
        int w = 1;

        for (int j = 0; j < m / 2; j++) {
            for (int k = 0; k < vec_len; k += m) {
                //butterfly
                int V = (w * Z[k + j + m / 2]) % modulus;
                int U = Z[k + j];
                Z[k + j] = (U + V) % modulus;
                Z[k + j + m / 2] = (U - V + modulus) % modulus;
            }
            w *= (w_m % modulus);
        }
    }

    if(inverse)
        Z = hadamard_product(Z, constant_vector(vec_len, mod_inverse(vec_len, modulus)), modulus);

    return Z;
}

///////////////////////////////////////////////////////////////////////////////
// NTT2 (not working)
// Based on "Speeding up the Number Theoretic Transform
// for Faster Ideal Lattice - Based Cryptography"
//
// Has to be power of 2
///////////////////////////////////////////////////////////////////////////////
vector<int> NTT::calculate2(vector<int> A, bool inverse) {
    vector<int> Z = A; //return polynomial

    int vec_len = A.size();  //This should be same as NTT.vec_length but just in case
    int spread = vec_len;

    for (int m = 1; m < vec_len; m *= 2) {
        spread /= 2;
        for (int i = 0; i < m; i++) {
            int j1 = 2 * i * spread;
            int j2 = j1 + spread - 1;
            int S  = phi_table[m + i];

            for (int j = j1; j <= j2; j++) {
                //butterfly
                int U = A[j];
                int V = (A[j + spread] * S) % modulus;

                Z[j] = (U + V) % modulus;
                Z[j + spread] = (U - V + modulus) % modulus;
            }
        }
    }

    if (inverse)
        Z = hadamard_product(Z, constant_vector(vec_len, mod_inverse(vec_len, modulus)), modulus);

    //Z = bitReverse(Z);
    return Z;
}

///////////////////////////////////////////////////////////////////////////////
// NTT3 
// Based on Popelmann thesis
///////////////////////////////////////////////////////////////////////////////
vector<int> NTT::calculate3(vector<int> A, bool inverse) {
    int n = A.size();
    A = bitReverse(A);
    int N = n;
    int m = 2;
    int omeg = w_n;
    if (inverse)
        omeg = w_n_inv;

    while (m <= N) {
        int s = 0;
        while (s < N) {
            for (int i = 0; i < m / 2; i++) {
                N = i * n / m;
                int a = s + i;
                int b = s + i + m / 2;
                int c = A[a];
                int d = A[b];
                int twid = pow_mod(omeg, N, modulus);
                A[a] = (c + (twid * d)) % modulus;
                A[b] = (c - (twid * d) + modulus) % modulus;
            }
            s = s * m;
        }
        m = m * 2;
    }

    //scale by n^-1
    if(inverse)
        A = hadamard_product(A, constant_vector(n, mod_inverse(n, modulus)), modulus);
    return A;
}

///////////////////////////////////////////////////////////////////////////////
// NTT4 (working)
// Based on Nayuki radix2 
///////////////////////////////////////////////////////////////////////////////
vector<int> NTT::calculate4(vector<int> A, bool inverse) { 
    int n      = A.size();
    int levels = log2(n);

    vector<int> powtable;

    int temp = 1;
    int omeg = w_n;
    if (inverse)
        omeg = w_n_inv;

    for (int i = 0; i < n / 2; i++) {
        powtable.push_back(temp);
        temp = temp * omeg % modulus;
    }
    
    A = bitReverse(A);

    int size = 2;
    while (size <= n) {
        int halfsize = size / 2;
        int tablestep = n / size;

        for (int i = 0; i < n; i += size) {
            int  k = 0;
            for (int j = i; j < i + halfsize; j++) {
                
                int l = j + halfsize;
                int left = A[j];
                int right = (A[l] * powtable[k]) % modulus;
                A[j] = (left + right) % modulus;
                A[l] = (left - right + modulus) % modulus;
                k += tablestep; 
            }
        }
        size *= 2; 
    }

    if (inverse)
        A = hadamard_product(A, constant_vector(n, mod_inverse(n, modulus)), modulus);
    return A;
}

///////////////////////////////////////////////////////////////////////////////
// NTT5 
// Based on Nayuki radix2, with RNS 
///////////////////////////////////////////////////////////////////////////////
vector<int> NTT::calculate5(vector<int> A, bool inverse) {
    int n = A.size();
    int levels = log2(n);

    vector<int> powtable;

    int temp = 1;
    int omeg = w_n;
    if (inverse)
        omeg = w_n_inv;

    for (int i = 0; i < n / 2; i++) {
        powtable.push_back(temp);
        temp = rns.mult(temp, omeg) % modulus; //temp * omeg % modulus;
    }

    A = bitReverse(A);

    int size = 2;
    int count = 0;
    while (size <= n) {
        int halfsize = size / 2;
        int tablestep = n / size;

        for (int i = 0; i < n; i += size) {
            int  k = 0;
            for (int j = i; j < i + halfsize; j++) {

                int l = j + halfsize;
                int left = A[j];
                int right = rns.mult(A[l], powtable[k]) % modulus; //(A[l] * powtable[k]) % modulus;
                A[j] = (left + right) % modulus;
                A[l] = (left - right + modulus) % modulus;
                k += tablestep;
            }
            count++;
         cout << count*100/n << "% of butterfly NTT done.\r";
        }
        
        size += size;  // size = size * 2
    }
    cout << "100% of butterfly NTT done." << endl;
    if (inverse)
        A = hadamard_product(A, constant_vector(n, mod_inverse(n, modulus)), modulus);
    return A;
}

///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////
NTT::NTT(int vector_length, int minimum_modulus, RNS rns_system){
    vec_length = vector_length;
    min_mod    = minimum_modulus;
    rns        = rns_system;

    modulus    = new_modulus(vector_length,minimum_modulus);     // Used modulus
    w_n        = find_root_of_unity2(vec_length, modulus);        // nth root of unity 
    w_n_inv    = mod_inverse(w_n, modulus);                      // root inverse
    phi        = sqrt_mod(w_n, modulus);
    phi_inv    = mod_inverse(phi, modulus);
    phi_table  = generate_phi_table(vector_length, phi, modulus);// bit reversed powers of phi
 

}