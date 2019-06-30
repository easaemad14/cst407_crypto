/*******************************************************************************
 * File:        encrypt.cpp
 * Author:      eazy
 *              easa.elsirgany@oit.com
 *
 * Description: Implementation of encryption functions and tools
 ******************************************************************************/
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <cerrno>

#include "encrypt.hpp"

using namespace std;


/**
 * Global variables
 */
static constexpr auto alpha_size{26}; // Size of our alphabet
constexpr auto upper_offset{65}; // Man ascii
constexpr auto lower_offset{97};

/**
 * Functions
 */
// Encrypt a file with the key value offset
// Ciphertext is written to file.ct
// Returns zero on success, else errno
int encryptFile(const string& file, const int key)
{
	ifstream ifs(file);
	ofstream ofs(file + ".ct"s);

	if(not ifs.is_open() or not ofs.is_open()) {
		cerr << "Could not open files!" << endl;
		return -ENOENT;
	}

	// Read the file and do a little magic
	string buf; // Create a mutable buffer
	while(getline(ifs, buf)) {
		for(auto c = buf.begin(); c not_eq buf.end(); c++) {
			// Ensure we are only operating on alphabetic characters
			if(not isalpha(*c)) {
				continue;
			}

			// Magic goes here
			*c = toupper(*c);
			*c = ((*c - upper_offset) + key) % alpha_size + upper_offset;
		}

		// Write the block to the cipher text file
		ofs << buf;
	}

	ifs.close(); ofs.close();
	return 0;
}

// Decrypt a file with the key value offset
// Plaintext is written to file.pt
int decryptFile(const string& file, const int key)
{
	ifstream ifs(file);
	ofstream ofs(file + ".pt"s);

	if(not ifs.is_open() or not ofs.is_open()) {
		cerr << "Could not open files!" << endl;
		return -ENOENT;
	}

	string buf;
	while(getline(ifs, buf)) {
		for(auto c = buf.begin(); c not_eq buf.end(); c++) {
			if(not isalpha(*c)) {
				continue;
			}

			*c = tolower(*c);
			*c = ((*c - lower_offset) - key + alpha_size) % alpha_size + lower_offset;
		}

		ofs << buf;
	}

	ifs.close(); ofs.close();
	return 0;
}

// Attempt to crack a file, writing the best match to file.crack
// file.dat will contain the frequency analysis data
int crackFile(const string& /* file */)
{
	clog << "Cracking is not available yet.... Come back soon" << endl;
	return 0;
}
