#pragma once
/*******************************************************************************
 * File:        rsa.hpp
 * Author:      sirganye
 *              easa.elsirgany@3dsystems.com
 *
 * Description: RSA (helper) functions
 ******************************************************************************/

bool isProbablyPrime(unsigned long);
bool factorNumber(unsigned long, unsigned long&, unsigned long&);
unsigned long gcd(unsigned long, unsigned long);
unsigned long igcd(unsigned long, unsigned long);
