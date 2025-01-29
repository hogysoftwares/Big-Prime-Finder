#include <iostream>
#include <gmpxx.h>
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

// Function to perform Fermat primality test with base a=3
bool fermatTest(const mpz_class& M_p, unsigned long long a = 3) {
    if (M_p == 1)
        return false;
    mpz_class result;
    mpz_powm_ui(result.get_mpz_t(), mpz_class(a).get_mpz_t(), M_p.get_ui() - 1, M_p.get_mpz_t());
    return result == 1;
}

// Lucas-Lehmer test to check to see if the found Fermat pseudoprimes are Mersenne primes
bool lucasLehmerTest(unsigned long long p) {
    if (p == 2)
        return true;
    mpz_class M_p = (mpz_class(1) << p) - 1;
    mpz_class s = 4;
    for (unsigned long long i = 0; i < p - 2; i++) {
        s = (s * s - 2) % M_p;
    }
    return s == 0;
}

int main() {
    vector<mpz_class> mersennePrimes;
    unsigned long long last_p = 2; // Start from p=2 but can be changed to any known prime number

    cout<< "Searching for Mersenne primes..." <<endl;

    while (mersennePrimes.size() < 10) {
        // Find candidate exponents greater than last_p
        unsigned long long p = last_p + 1;
        while (true) {
            if (!isPrime(p)) {
                p++;
                continue;
            }

            // Compute M_p = 2^p - 1
            mpz_class M_p = (mpz_class(1) << p) - 1;

            //Fermat Primality test with base a=3
            if (fermatTest(M_p, 3)) {
                // Do Lucas-Lehmer test
                if (lucasLehmerTest(p)) {
                    mersennePrimes.push_back(M_p);
                    cout << "Mersenne Prime Found: M_" << p << " = " << M_p.get_str() << endl;
                    last_p = p;
                    break; // Go back to search for next Mersenne prime greater than last_p
                }
            }

            p++;
        }
    }

    cout << "\nList of 10 Mersenne Primes found:" << endl;
    for (int i = 0; i < mersennePrimes.size(); i++) {
        cout << "M_" << last_p << " = " << mersennePrimes[i].get_str() << endl;
    }

    return 0;
}
