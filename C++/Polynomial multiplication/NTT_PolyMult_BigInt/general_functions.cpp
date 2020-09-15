#include <iostream>
#include <vector>
#include <string>
#include "BigIntLibrary/BigIntegerLibrary.hh"
#include <fstream>
#include <iomanip>

using namespace std;

//////////////////////////////////////////////////////////////////////////////
// Returns product of values in the vector
//
///////////////////////////////////////////////////////////////////////////////
BigUnsigned product(vector<BigUnsigned> vec) {
    BigUnsigned t = 1;
    for (int i = 0; i < vec.size(); i++) {
        t *= vec[i];
    }
    return t;
}

//////////////////////////////////////////////////////////////////////////////
// Compare vectors
// Returns true if vectors are equal
///////////////////////////////////////////////////////////////////////////////
bool vectorsAreEqual(vector<BigUnsigned> a, vector<BigUnsigned> b) {
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
vector<BigUnsigned> hadamard_product(vector<BigUnsigned> a, vector<BigUnsigned> b, BigUnsigned moduli) {
    vector<BigUnsigned> Z;

    for (int i = 0; i < a.size(); i++) {
        Z.push_back((a[i] * b[i]) % moduli);  //RNS_mod_multiply(a[i],b[i],moduli)
    }

    return Z;
}

//////////////////////////////////////////////////////////////////////////////
// RNS Modular Hadamard Product with reduction
// pointwise modular multiplication between vectors (to use RNS_mult function later)
///////////////////////////////////////////////////////////////////////////////
vector<BigUnsigned> rns_hadamard_product(vector<BigUnsigned> a, vector<BigUnsigned> b, BigUnsigned moduli) {
    vector<BigUnsigned> Z;

    for (int i = 0; i < a.size(); i++) {
        Z.push_back((a[i] * b[i]) % moduli);  //RNS_mod_multiply(a[i],b[i],moduli)
    }

    return Z;
}

///////////////////////////////////////////////////////////////////////////////
// Sample Polynomial
// Returns uniformly random polynomial of length N in ring q
///////////////////////////////////////////////////////////////////////////////
// rand % q needs fixed
vector<BigUnsigned> sample_polynomial(BigUnsigned N, BigUnsigned q) {
    vector<BigUnsigned> Z;

    // use modulus q only if <= 32 bits
    unsigned long mod;
    if (q.bitLength() <= 32) {
        mod = q.toUnsignedLong();
    }
    else
        mod = unsigned int(pow(2, 31));

    //sample
    for (BigUnsigned i = 0; i < N; i++) {
        Z.push_back(rand() % mod);
    }
    return Z;
}

///////////////////////////////////////////////////////////////
// Bit Reverse
// Rearrange vector based on bit reversal of the indices
///////////////////////////////////////////////////////////////
vector<BigUnsigned> bitReverse(vector<BigUnsigned> A) {
    vector<BigUnsigned> Z = A; //return vector
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
// Factorize & isPrime
// Returns a vector of prime factors of integer 
///////////////////////////////////////////////////////////////
vector<BigUnsigned> factorize(BigUnsigned n) {
    vector<BigUnsigned> factors;

    //find number of 2s
    while (n % 2 == 0) {
        factors.push_back(2);
        n = n / 2;
    }

    //now n is odd
    for (BigUnsigned i = 3; i <= n/2; i += 2) {  //sqrt(n) instead of n/2
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

bool isPrime(BigUnsigned A) {
    return(factorize(A).size() == 1);
}

///////////////////////////////////////////////////////////////
// Modular inverse   (stupid brute force way)
// Given integer A and modulus mod, return 'ans' where
// (A * ans) mod 'mod' = 1

// is now replaced by BigUnsigned's library:
// BigUnsigned modinv(const BigInteger &x, const BigUnsigned &n)
///////////////////////////////////////////////////////////////
BigUnsigned mod_inverse(BigUnsigned A, BigUnsigned mod) {
    return modinv(A, mod);
    /*
    for (int ans = 0; ans < mod; ans++) {
        if ((ans * A) % mod == 1)
            return ans;
    }
    //error
    cout << "modular inverse error" << endl;
    return 0;
    */
}

///////////////////////////////////////////////////////////////
// Modular exponentiation (from wikipedia)

// Can be replaced by:
// modexp(const BigInteger& base, const BigUnsigned& exponent,const BigUnsigned& modulus)
///////////////////////////////////////////////////////////////
BigUnsigned pow_mod(BigUnsigned base, BigUnsigned ex, BigUnsigned mod) {
   
    return modexp(base, ex, mod);

    /*
    BigUnsigned result = 1;
    BigUnsigned ex_prime = 0;

    while (ex_prime < ex) {
        ex_prime++;
        result = (base * result) % mod;
    }

    return result;
    */
    }

/////////////////////////////////////////////////////////////////
// Multiply vector by powers of val
///////////////////////////////////////////////////////////////
vector<BigUnsigned> mult_by_power(vector<BigUnsigned> vec, BigUnsigned val, BigUnsigned modulus) {
    for (int i = 0; i < vec.size(); i++) {
        vec[i] = (vec[i] * pow_mod(val, i, modulus)) % modulus;
    }

    return vec;
}

///////////////////////////////////////////////////////////////
// Modular square root (stupid way) (Can do Tonelli - Shanks algorithm)
///////////////////////////////////////////////////////////////
BigUnsigned sqrt_mod(BigUnsigned A, BigUnsigned mod) {
    for (BigUnsigned i = 0; i < mod; i++) {
        if (pow_mod(i, 2, mod) == A)
            return i;
    }
    //not found for (1331, 12289)
    // error
    cout << "No modular square root found. INCORRECT NUMBER RETURNED (-1), CAUTION." << endl;
    return -1;
}

///////////////////////////////////////////////////////////////
// Print Vector
// prints contents of an integer only vector
///////////////////////////////////////////////////////////////
void printVector(vector<BigUnsigned> list, string name, bool printFullVector) {
    cout << name;
    int len = list.size();

    //print vector ends
    if (!printFullVector && (len > 6)) {
        cout << list[0] << ' ' << list[1] << ' ' << list[2] << " ... ";
        cout << list[len - 3] << ' ' << list[len - 2] << ' ' << list[len-1] << endl;
    }

    //print whole vector 
    else {
        for (int i = 0; i < list.size(); i++) {
            cout << list[i] << ' ';
        }
        cout << endl;
    }
}

///////////////////////////////////////////////////////////////
// Save vector to text file
// 
///////////////////////////////////////////////////////////////
void saveValToTextfile(BigUnsigned val, string savename) {
    ofstream file;
    file.open(savename);
    file << hex << setfill('0') << val << "\n";
    file.close();
}

void saveVectorToTextfile(vector<BigUnsigned> vec, string savename, bool WriteOnOneLine, int val_width) {
    ofstream file;
    file.open(savename);
    
    if (WriteOnOneLine) {
        for (int i = 0; i < vec.size(); i++) {
            file << hex << setfill('0') << setw(val_width) << vec[i] << "\n";
        }
    }

    else {
        for (int i = 0; i < vec.size(); i++) {
            file << hex << setfill('0') << vec[i] << "\n";
        }
    }

    file.close();
}

void saveVectorVectorToTextfile(vector<vector<BigUnsigned>> vec, string savename) {
    ofstream file;
    file.open(savename);

    for (int i = 0; i < vec.size(); i++) {
        for (int j = 0; j < vec[i].size(); j++) {
            file << hex << setfill('0') << vec[i][j] << "\n";
        }
    }
    file.close();
}

///////////////////////////////////////////////////////////////
// Coprime tests
// for variables and vectors
///////////////////////////////////////////////////////////////
/*
BigUnsigned gcd(BigUnsigned A, BigUnsigned B) {
    if (B == 0)
        return A;
    return gcd(B, A % B);
}
*/
bool areCoprimes(BigUnsigned A, BigUnsigned B) {
    return (gcd(A, B) == 1);
}

bool isCoprimeToVector(BigUnsigned A, vector<BigUnsigned> B) {
    for (int i = 0; i < B.size(); i++) {
        if (!areCoprimes(A, B[i]))
            return false;
    }

    return true;
}

///////////////////////////////////////////////////////////////
// Zero pad input vector
///////////////////////////////////////////////////////////////
vector<BigUnsigned> zero_pad(vector<BigUnsigned> A) {
    int size = A.size();

    for (int i = 0; i < size; i++) {
        A.push_back(0);
    }
    return A;
}
