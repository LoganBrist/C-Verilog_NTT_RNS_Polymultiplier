#include <iostream>
#include <vector>
#include <string>

using namespace std;

//////////////////////////////////////////////////////////////////////////////
// Compare vectors
// Returns true if vectors are equal
///////////////////////////////////////////////////////////////////////////////
bool compareVectors(vector<int> a, vector<int> b) {
    for (int i = 0; i < a.size(); i++) {
        if (a[i] != b[i])
            return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////////
// Modular Hadamard Product
// pointwise modular multiplication between vectors (to use RNS_mult function later)
///////////////////////////////////////////////////////////////////////////////
vector<int> hadamard_product(vector<int> a, vector<int> b, int moduli) {
    vector<int> Z;

    for (int i = 0; i < a.size(); i++) {
        Z.push_back((a[i] * b[i]) % moduli);  //RNS_mod_multiply(a[i],b[i],moduli)
    }

    return Z;
}

///////////////////////////////////////////////////////////////////////////////
// Sample Polynomial
// Returns uniformly random polynomial of length N in ring q
///////////////////////////////////////////////////////////////////////////////
vector<int> sample_polynomial(int N, int q) {
    vector<int> Z;
    for (int i = 0; i < N; i++) {
        Z.push_back(rand() % q);
    }
    return Z;
}

///////////////////////////////////////////////////////////////
// Bit Reverse
// Rearrange vector based on bit reversal of the indices
///////////////////////////////////////////////////////////////
vector<int> bitReverse(vector<int> A) {
    vector<int> Z = A; //return vector
    int N = Z.size();
    int n_bits = ceil(log2(N));

    for (int i = 0; i < N; i++) {
        int val = 0;
        int idx = 0;
        for (int j = 0; j < n_bits; j++) {
            val = i & (1 << j); //A[i] --> i
            if (val)
                idx |= 1 << ((n_bits - 1) - j);
        }
        Z[i] = A[idx];
    }
    return Z;
}

///////////////////////////////////////////////////////////////
// Factorize
// Returns a vector of prime factors of integer 
///////////////////////////////////////////////////////////////
vector<int> factorize(int n) {
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
// Modular inverse   (stupid brute force way)
// Given integer A and modulus mod, return 'ans' where
// (A * ans) mod 'mod' = 1
///////////////////////////////////////////////////////////////
int mod_inverse(int A, int mod) {
    for (int ans = 0; ans < mod; ans++) {
        if ((ans * A) % mod == 1)
            return ans;
    }
    //error
    cout << "modular inverse error" << endl;
    return 0;
}

///////////////////////////////////////////////////////////////
// Modular exponentiation (from wikipedia)
///////////////////////////////////////////////////////////////
int pow_mod(int base, int ex, int mod) {
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
int sqrt_mod(int A, int mod) {
    for (int i = 0; i < mod; i++) {
        if (pow_mod(i, 2, mod) == A)
            return i;
    }

    // error
    return -1;
}

///////////////////////////////////////////////////////////////
// Print Vector
// prints contents of an integer only vector
///////////////////////////////////////////////////////////////
void printVector(vector<int> list, string name) {
    cout << name;
    for (int i = 0; i < list.size(); i++) {
        cout << list[i] << ' ';
    }
    cout << endl;
}