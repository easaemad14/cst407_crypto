/*******************************************************************************
 * File:        main.cpp
 * Author:      sirganye
 *              easa.elsirgany@3dsystems.com
 *
 * Description: Simple RSA implementation
 ******************************************************************************/
#include <iostream>
#include <string>

#include "rsa.hpp"

using namespace std;


unsigned long getValueFromUser(string message)
{
	unsigned long retVal;
	string buf;

	clog << message;
	getline(cin, buf);
	try {
		retVal = stoll(buf);
	} catch(...) {
		cerr << "Invalid value: " << buf << endl;
		retVal = 0ull;
	}

	return retVal;
}

int main(int, char**)
{
	//auto enc{true};
	unsigned long n, e; //, p, q, e, val;
	string buf;

	// Are we encrypting?
	//clog << "Would you like to encrypt [Y/n]: ";
	//getline(cin, buf);
	//if(buf[0] == 'N' or buf[0] == 'n') enc = false;

	// Get values from user
	n = getValueFromUser("Please provide n value: "s);
	e = getValueFromUser("Please provide e value: "s);
	//val = getValueFromUser("Please enter "s +
	//		(enc ? "plaintext"s : "ciphertext"s) + ": "s);

	if((n > 0) and (e > 0)) {
#if 1
		clog << "n is " << (isProbablyPrime(n) ? "probably" : "not") <<
			" prime" << endl;
		clog << "e is " << (isProbablyPrime(e) ? "probably" : "not") <<
			" prime" << endl;
#endif
	}

	return 0;
}
