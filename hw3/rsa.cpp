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


// Compute a^b mod n with very large numbers
unsigned long powMod(const ul a, const ul b, const ul n)
{
	auto retVal{1ul};
	for(auto i{0ul}; i < b; i++) {
		retVal = retVal * a % n;
	}

	return retVal;
}

// Uses Fermat's little theorem to guess primality
bool isProbablyPrime(const ul n)
{
	// a^(p-1) cong 1 mod p; using 2 for a
	// Three will fail due to 3-1==2, but 3 is prime
	return (powMod(2, n - 1, n) == 1ul ? true : false);
}

// Check that n is definitely prime
bool isDefinitelyPrime(const ul n)
{
	auto retVal{true};
	for(auto i = static_cast<ul>(sqrtl(n)); i > 1; i--) {
		if(n % i == 0) {
			retVal = false;
			break;
		}
	}

	return retVal;
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

// Calculate Least Common Multiple of a and b
ul lcm(const ul a, const ul b)
{
	return (a * b / gcd(a, b));
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

// Generate modular multiplicative inverse of a, with respect to m
// Min and max values are inclusive
ul igcd(const ul a, const ul m, const ul min, const ul max)
{
	auto retVal{0ul};
	for(auto i{min}; i <= max; i++) {
		if(a*i % m == 1ul) {
			retVal = i;
			break;
		}
	}

	return retVal;
}

// Generate a prime, starting at start
ul genPrime(const ul start)
{
	auto retVal{0ul};
	for(auto i{start}; i not_eq 0; i++) {
		if(isDefinitelyPrime(i)) {
			retVal = i;
			break;
		}
	}

	return retVal;
}

// Return coprime of val within min and max (exclusively)
ul genCoprime(const ul val, const ul min, const ul max)
{
	auto retVal{0ul};
	for(auto i{min + 1}; i < max; i++) {
		if(gcd(i, val) == 1ul) {
			retVal = i;
			break;
		}
	}

	return retVal;
}

// Get Euler's Totient value from product of two primes (n)
ul getTotient(const ul n)
{
	auto p{0ul}, q{0ul}, retVal{0ul};
	if(factorNumber(n, p, q)) {
		retVal = lcm((p-1),(q-1));
		//retVal = (p-1)*(q-1);
	}

	return retVal;
}
