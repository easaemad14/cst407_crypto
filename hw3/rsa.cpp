/*******************************************************************************
 * File:        rsa.cpp
 * Author:      sirganye
 *              easa.elsirgany@3dsystems.com
 *
 * Description: RSA (helper) functions
 ******************************************************************************/
#include <cmath>

#include "rsa.hpp"

using namespace std;
using ul = unsigned long;


// Uses Fermat's little theorem to guess primality
bool isProbablyPrime(const ul n)
{
	// a^(p-1) cong 1 mod p; using 2 for a
	// Three will fail due to 3-1==2, but 3 is prime
	auto rem{1ul};
	for(auto i{0ul}; i < n - 1; i++) {
		rem = rem * 2ul % n;
	}

	return (rem == 1ul ? true : false);
}

// Factor n from two primes p and q
bool factorNumber(const ul n, ul& p, ul& q)
{
	bool retVal{false};

	/**
	 * n is produced by multiplying p and q, therefore, taking the square root of
	 * n will give us the largest possible value of the smaller of the two.
	 */
	for(auto i = static_cast<ul>(sqrtl(n)); i > 0; i--) {
		if(n % i == 0) { // We found a possible match
			p = i;
			q = n / p;
			if(isProbablyPrime(p) and isProbablyPrime(q)) {
				retVal = true;
				break;
			}
		}
	}

	return retVal;
}

// Calculate Greatest Common Divisor (GCD) of two values a and b
// This is not a very efficient algorithm. . .
ul gcd(const ul a, const ul b)
{
	auto retVal{1ul};
	auto tmp1 = a, tmp2 = b; // Create mutable values
	for(auto i{2ul}; (i <= tmp1) and (i <= tmp2); ) {
		if((tmp1 % i == 0) and (tmp2 % i == 0)) { // We found a match
			retVal *= i;
			tmp1 /= i; tmp2 /= i;
		} else {
			++i;
		}
	}

	return retVal;
}

ul igcd(const ul, const ul)
{
	return 0ul;
}
