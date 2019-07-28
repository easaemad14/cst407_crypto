/*******************************************************************************
 * File:        rsa.cpp
 * Author:      sirganye
 *              easa.elsirgany@3dsystems.com
 *
 * Description: RSA (helper) functions
 ******************************************************************************/

#include "rsa.hpp"

using namespace std;
using ul = unsigned long;


// Uses Fermat's little theorem to guess primality
bool isProbablyPrime(ul n)
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
bool factorNumber(ul, ul&, ul&)
{
	return false;
}

ul gcd(ul, ul)
{
	return 0ul;
}

ul igcd(ul, ul)
{
	return 0ul;
}
