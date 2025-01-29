#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

// function to check if 'p' in M = 2^p-1 is prime (Mersenne Prime Condition)
bool isPrime(unsigned long long n) {
    if (n < 2)
        return false;
    if (n == 2)
        return true;
    if (n % 2 == 0)
        return false;
    unsigned long long sqrt_n = static_cast<unsigned long long>(sqrt(n));
    for (unsigned long long i = 3; i <= sqrt_n; i += 2) {
        if (n % i == 0)
            return false;
    }
    return true;
}

// scale too big must use modular exponentiation
unsigned long long powMod(unsigned long long base, unsigned long long exp, unsigned long long mod) {
    unsigned long long result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp & 1)
            result = (__int128(result) * base) % mod; //__int128 to prevent overflow
        base = (__int128(base) * base) % mod;
        exp >>= 1;
    }
    return result;
}

// Fermat primality test with base a=3
bool fermatTest(unsigned long long M_p, unsigned long long a = 3) {
    if (M_p == 1)
        return false;
    return powMod(a, M_p - 1, M_p) == 1;
}

// Lucas-Lehmer test to check to see if the found Fermat Pseudoprimes are Mersenne primes
bool lucasLehmerTest(unsigned long long p) {
    if (p == 2)
        return true;
    unsigned long long M_p = (1ULL << p) - 1;
    unsigned long long s = 4;
    for (unsigned long long i = 0; i < p - 2; i++) {
        __int128 temp = (__int128(s) * s) - 2;
        s = temp % M_p;
    }
    return s == 0;
}

int main() {
    vector<unsigned long long> mersennePrimes;
    unsigned long long last_p = 2; // Start from p=2 but can be changed to another known prime number

    while (mersennePrimes.size() < 10) {
        // Find candidate exponents greater than last_p
        for (unsigned long long p = last_p + 1; ; p++) {
            if (!isPrime(p))
                continue;

            unsigned long long M_p;
            if (p < 64)
                M_p = (1ULL << p) - 1;
            else {
                // Skip if it exceeds 63 to prevent overflow in 64-bit (limit of unsigned long long, need GMP to bypass this problem)
                continue;
            }

            // Do Fermat Primality test with base a=3
            if (fermatTest(M_p, 3)) {
                // add to candidates and apply Lucas-Lehmer test
                if (lucasLehmerTest(p)) {
                    mersennePrimes.push_back(M_p);
                    cout << "Mersenne Prime Found: M_" << p << " = " << M_p << endl;
                    last_p = p;
                    break; // Go back to search for next Mersenne prime greater than last_p
                }
            }
        }
    }

    cout << "\nList of 10 Mersenne Primes found:" << endl;
    for (int i = 0; i < mersennePrimes.size(); i++) {
        cout << "M_" << last_p << " = " << mersennePrimes[i].get_str() << endl;
    }

    return 0;
}
