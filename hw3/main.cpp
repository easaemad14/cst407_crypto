/*******************************************************************************
 * File:        main.cpp
 * Author:      sirganye
 *              easa.elsirgany@3dsystems.com
 *
 * Description: Simple RSA implementation
 ******************************************************************************/
#include <iostream>
#include <string>
#include <string.h>
#include <random>

#include "rsa.hpp"

using namespace std;


auto gen_keys{false}, encrypt{false}, decrypt{false};
auto value{0ul}, seed{0ul};
mt19937 rng;


[[noreturn]] void printHelp(int exVal = 1)
{
	clog << "Usage: rsa [-h] [-g seed] [-e val] [-d val]" << endl;
	clog << "\t-h: Print this help menu" << endl;
	clog << "\t-g: Generate RSA keys using seed value" << endl;
	clog << "\t\tIf no seed provided, time will be used" << endl;
	clog << "\t-e: Encrypt value val" << endl;
	clog << "\t-d: Decrypt value val" << endl;

	exit(exVal);
}

// Parse the command line and ensure all variables are set
void parse(int cnt, char** vect)
{
	for(auto i{1}; i < cnt; i++) {
		if(0 == strcmp("-h", vect[i])) {
			printHelp(0);
		} else if(0 == strcmp("-g", vect[i])) {
			gen_keys = true;
			if((i + 1 < cnt) and (vect[i + 1][0] not_eq '-')) {
				try {
					seed = stol(vect[++i]);
				} catch(...) {
					cerr << "Invalid seed value " << vect[i] << endl;
					printHelp();
				}
			}
		} else if(0 == strcmp("-e", vect[i])) {
			if(i + 1 == cnt) {
				printHelp();
			}
			try {
				value = stol(vect[++i]);
				encrypt = true;
			} catch(...) {
				cerr << "Invalid encryption value: " << vect[i] << endl;
				printHelp();
			}
		} else if(0 == strcmp("-d", vect[i])) {
			if(i + 1 == cnt) {
				printHelp();
			}
			try {
				value = stol(vect[++i]);
				decrypt = true;
			} catch(...) {
				cerr << "Invalid decryption value: " << vect[i] << endl;
				printHelp();
			}
		} else {
			cerr << "Invalid option!" << endl;
			printHelp();
		}
	}

	// We need at least one operation
	if(not gen_keys and not encrypt and not decrypt) {
		printHelp();
	}
}

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

// Generate n number of keys
void genKeyValues(unsigned int n)
{
	for(auto i{0u}; i < n; i++) {
		auto p = genPrime(rng() % 90000 + 10000ul);
		auto q = genPrime(rng() % 90000 + 10000ul);
		auto t = (p - 1) * (q - 1);
		auto e = genCoprime(t, rng() % 90000 + 10000ul, t);
		clog << "n: " << to_string(p*q) << ", e: " << to_string(e) << endl;
	}
}

int main(int argc, char** argv)
{
	auto retVal{0};
	parse(argc, argv);

	// Seed our RNG
	if(not seed) {
		seed = time(nullptr);
	}
	clog << "Using seed value: " << to_string(seed) << endl;
	rng.seed(seed);

	if(gen_keys) {
		clog << "Generating keys. . ." << endl;
		genKeyValues(3);
	}

	if(encrypt) {
		clog << "Encrypting..." << endl;
		auto n = getValueFromUser("Please provide n value: "s);
		auto e = getValueFromUser("Please provide e value: "s);

		if((n > 0) and (e > 0)) {
			clog << "Factoring... this may take a while" << endl;
			auto p{0ul}, q{0ul};
			if(factorNumber(n, p, q)) {
				clog << "n value: " << to_string(n) << endl;
				clog << "p value: " << to_string(p) << endl;
				clog << "q value: " << to_string(q) << endl;
			}
		}
	}

	if(decrypt) {
		clog << "Decrypting..." << endl;
	}

	return retVal;
}
