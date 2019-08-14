/*******************************************************************************
 * File:        main.cpp
 * Author:      eazy
 *              easa.elsirgany@oit.edu
 *
 * Description: Program used to implement and crack the Vigenere cipher
 ******************************************************************************/
#include <string>
#include <iostream>
#include <cstring>
#include <cctype>
#include <cerrno>
#include <map>
#include <functional>

#include "encrypt.hpp"

using namespace std;


/**
 * Global variables
 */
static string operation;  // What is the user trying to achieve
static map<string, function<int(void)>> ops = {
	{"encrypt"s, [] () { return encryptFileToFile(); } },
	{"decrypt"s, [] () { return decryptFileToFile(); } },
	{"crack"s, [] () { return crack(); } }
};

/**
 * Functions
 */
// Print help and exit
[[noreturn]] void printHelp(int exVal = 1)
{
	clog << "Usage: vigenere [-h] <operation>" << endl;
	clog << "\t-h:        Print this help menu" << endl;
	clog << "\toperation: The following operations are available: " << endl;

	for(auto it : ops) {
		clog << "\t\t" << it.first << endl;
	}

	exit(exVal);
}

// Parse the command line and ensure all variables are set
void parse(int cnt, char** vect)
{
	for(auto i{1}; i < cnt; i++) {
		if(0 == strcmp("-h", vect[i])) {
			printHelp(0);
		} else { // Assume this is the operation
			operation = vect[i];
		}
	}
}

int main(int argc, char** argv)
{
	auto retVal{0};
	parse(argc, argv);

	try {
		auto f = ops.at(operation);
		retVal = f();
	} catch(...) {
		printHelp();
	}

	return retVal;
}
