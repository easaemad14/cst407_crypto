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
// Ensure the key offset is valid
constexpr void safeKeyOffset(int key)
{
	while(key < 0) {
		key += alpha_size;
	}
	key = key % alpha_size;
}

// Encrypt ifile to ofile using key offset
// Returns zero on success, else errno
int encryptFile(const string& ifile, const string& ofile, const int key)
{
	ifstream ifs(ifile);
	if(not ifs.is_open()) {
		cerr << "Could not open plain text file!" << endl;
		return -ENOENT;
	}

	ofstream ofs(ofile);
	if(not ofs.is_open()) {
		cerr << "Could not open cipher text file!" << endl;
		return -ENOENT;
	}

	// Read the file and do a little magic
	string buf; // Create a mutable buffer
	safeKeyOffset(key); // Ensure the key is valid offset
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

// Decrypt ifile to ofile with key offset
int decryptFile(const string& ifile, const string& ofile, const int key)
{
	ifstream ifs(ifile);
	if(not ifs.is_open()) {
		cerr << "Could not open cipher text file!" << endl;
		return -ENOENT;
	}

	ofstream ofs(ofile);
	if(not ofs.is_open()) {
		cerr << "Could not open plain text file!" << endl;
		return -ENOENT;
	}

	string buf;
	safeKeyOffset(key);
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

// Attempt to crack ifile, writing the best match to ofile
// file.dat will contain the frequency analysis data
int crackFile(const string&, const string&)
{
	clog << "Cracking is not available yet.... Come back soon" << endl;
	return 0;
}
