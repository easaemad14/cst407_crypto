#pragma once
/*******************************************************************************
 * File:        rsa.hpp
 * Author:      sirganye
 *              easa.elsirgany@3dsystems.com
 *
 * Description: RSA (helper) functions
 ******************************************************************************/

bool isProbablyPrime(const unsigned long);
bool isDefinitelyPrime(const unsigned long);
bool factorNumber(const unsigned long, unsigned long&, unsigned long&);
unsigned long gcd(const unsigned long, const unsigned long);
unsigned long igcd(const unsigned long, const unsigned long);

unsigned long genPrime(const unsigned long);
unsigned long genCoprime(const unsigned long, const unsigned long, const unsigned long);
