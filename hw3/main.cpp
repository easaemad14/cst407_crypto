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
auto seed{0ul};
mt19937 rng;


[[noreturn]] void printHelp(int exVal = 1)
{
	clog << "Usage: rsa [-h] [-g seed] [-e val] [-d val]" << endl;
	clog << "\t-h: Print this help menu" << endl;
	clog << "\t-g: Generate RSA keys using seed value" << endl;
	clog << "\t\tIf no seed provided, time will be used" << endl;
	clog << "\t-e: Encrypt mode" << endl;
	clog << "\t-d: Decrypt mode" << endl;

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
			encrypt = true;
		} else if(0 == strcmp("-d", vect[i])) {
			decrypt = true;
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

	if(gen_keys) {
		// Seed our RNG
		if(not seed) {
			seed = time(nullptr);
		}
		clog << "Using seed value: " << to_string(seed) << endl;
		rng.seed(seed);

		clog << "Generating keys. . ." << endl;
		genKeyValues(3);
	}

	if(encrypt) {
		clog << "Encrypting..." << endl;
		auto n = getValueFromUser("Please provide n value: "s);
		auto e = getValueFromUser("Please provide e value: "s);
		auto p = getValueFromUser("Please enter plaintext: "s);

		if((n > 0) and (e > 0) and (p > 0) and (p < n)) {
			auto t = getTotient(n);
			if((t > 0ul) and (gcd(e, t) == 1ul)) {
				clog << "Ciphertext: " << powMod(p, e, n) << endl;
			}
		}
	}

	if(decrypt) {
		clog << "Decrypting..." << endl;
		auto n = getValueFromUser("Please provide n value: "s);
		auto e = getValueFromUser("Please provide e value: "s);
		auto c = getValueFromUser("Please enter ciphertext: "s);

		if((n > 0) and (e > 0) and (c > 0) and (c < n)) {
			auto t = getTotient(n);
			if((t > 0ul) and (gcd(e, t) == 1ul)) {
				auto d = igcd(e, t, 0, n);
				if(d > 0ul) {
					clog << "Plaintext: " << powMod(c, d, n) << endl;
				}
			}
		}
	}

	return retVal;
}
