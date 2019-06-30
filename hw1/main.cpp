/*******************************************************************************
 * File:        main.cpp
 * Author:      eazy
 *              easa.elsirgany@oit.edu
 *
 * Description: Program used to implement and crack mono-alphabetic ciphers,
 *              namely the Caesar Cipher.
 *
 *              See the document description and the specification sheet for
 *              full details.
 ******************************************************************************/
#include <string>
#include <iostream>
#include <cstring>

#include "encrypt.hpp"

using namespace std;


/**
 * Global variables
 */
static auto enc{true};                // Encryption is used by default
static auto key_val{0};               // The offset used by the algorithm
static string pt_file;                // Path to the plain text file
static string ct_file;                // Path to the cipher text file


/**
 * Functions
 */
// Print help and exit
[[noreturn]] void printHelp(int exVal = 1)
{
	clog << "Usage: caesar [-h] [-d] [-k val] [-p file] [-c file]\n";
	clog << "\t-h: Print this help menu\n";
	clog << "\t-d: Switch to decryption mode; encryption is used by default\n";
	clog << "\t-k: Use the key value val for encryption/decryption\n";
	clog << "\t-p: Path to the plain text file (if applicable)\n";
	clog << "\t-c: Path to the cipher text file (if applicable)\n";
	clog << "\nIf variables are missing, you will be prompted for input\n";

	exit(exVal);
}

// Parse the command line and ensure all variables are set
void parse(int cnt, char** vect)
{
	for(auto i{1}; i < cnt; i++) {
		if(0 == strcmp("-h", vect[i])) {
			printHelp(0);
		} else if(0 == strcmp("-d", vect[i])) {
			enc = false;
		} else if(0 == strcmp("-k", vect[i])) {
			if(i + 1 == cnt) {
				printHelp();
			}
			try {
				key_val = stoi(vect[++i]);
			} catch(...) {
				cerr << "Invalid key value: " << vect[i] << endl;
				printHelp();
			}
		} else if(0 == strcmp("-p", vect[i])) {
			if(i + 1 == cnt) {
				printHelp();
			}
			pt_file = vect[++i];
		} else if(0 == strcmp("-c", vect[i])) {
			if(i + 1 == cnt) {
				printHelp();
			}
			ct_file = vect[++i];
		} else {
			cerr << "Invalid option!" << endl;
			printHelp();
		}
	}

	// If we are encrypting, a key value is needed
	while(enc and not key_val) {
		string buf;
		clog << "Please input key value for encryption: ";
		getline(cin, buf);
		try {
			key_val = stoi(buf);
		} catch(...) {
			clog << "Invalid key value: " << buf << endl;
		}
	}

	if(enc and pt_file.empty()) {
		clog << "Please enter path to plain text file: ";
		getline(cin, pt_file);
	}

	if(not enc and ct_file.empty()) {
		clog << "Please enter cipher text file: ";
		getline(cin, ct_file);
	}
}

int main(int argc, char** argv)
{
	auto retVal{0};
	parse(argc, argv);

	if(enc) {                        // Just encrypt the file
		retVal = encryptFile(pt_file, key_val);
	} else if(not enc and key_val) { // Decrypting with known key
		retVal = decryptFile(ct_file, key_val);
	} else {                         // Need to crack the cipher
		retVal = crackFile(ct_file);
	}

	return retVal;
}
