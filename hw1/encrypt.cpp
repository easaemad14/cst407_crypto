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
#include <vector>
#include <cmath>

#include "encrypt.hpp"

using namespace std;


/**
 * Global variables
 */
static constexpr auto alpha_size{26}; // Size of our alphabet
constexpr auto upper_offset{65}; // Man ascii
constexpr auto lower_offset{97};
constexpr float fa[alpha_size] = {8.167, 1.492, 2.782, 4.253, 12.702, 2.228,
	2.015, 6.094, 6.966, .153, .772, 4.025, 2.406, 6.749, 7.507, 1.929, .095,
	5.987, 6.327, 9.056, 2.758, .978, 2.36, .15, 1.974, .074};

/**
 * Functions
 */
// Ensure the key offset is valid
constexpr void safeKeyOffset(int key)
{
	// Increment until our key is positive
	while(key < 0) {
		key += alpha_size;
	}

	// Ensure our key is in the range from 0 - alpha_size
	key = key % alpha_size;
}

// Encrypt ifile to ofile using key offset
// Returns zero on success, else errno
int encryptFile(const string& ifile, const string& ofile, const int key)
{
	// Open our files
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
		for(auto& c : buf) {
			// Ensure we are only operating on alphabetic characters
			if(not isalpha(c)) {
				continue;
			}

			// Magic goes here
			c = toupper(c);
			c = ((c - upper_offset) + key) % alpha_size + upper_offset;
		}

		// Write the block to the cipher text file
		ofs << buf;
	}

	// Close our files and return success
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
		for(auto& c : buf) {
			if(not isalpha(c)) {
				continue;
			}

			c = tolower(c);
			c = ((c - lower_offset) - key + alpha_size) % alpha_size + lower_offset;
		}

		ofs << buf;
	}

	ifs.close(); ofs.close();
	return 0;
}

// Attempt to crack ifile, writing the best match to ofile
int crackFile(const string& ifile, const string& ofile)
{
	ifstream ifs(ifile);
	if(not ifs.is_open()) {
		cerr << "Could not open cipher text file!" << endl;
		return -ENOENT;
	}

	// Count the characters from our ciphertext and close the file
	vector<float> chars(alpha_size);
	int cnt{0};
	string buf;

	while(getline(ifs, buf)) {
		for(auto& c : buf) {
			if(not isalpha(c)) {
				continue;
			}

			// Increment our cnt array
			chars[tolower(c) - lower_offset]++;
			cnt++; // Keep track of alphabetic characters
		}
	}
	ifs.close();

	// Now, we need to get percentage of each character read
	for(auto i{0u}; i < chars.size(); i++) {
		chars[i] /= cnt;
#if 0 // Output FA for testing purposes
		clog << to_string(i) << ": " << to_string(chars[i]) << endl;
#endif
	}

	// Calculate error for each
	auto index{0}; // Keep track of our best guess of the offset
	auto chi2{-1.}; // Keep track of the value there
	for(auto i{0u}; i < chars.size(); i++) {
		auto tmp = calcChi2(chars, i);
#if 0 // Output all data for testing purposes
		clog << to_string(i) << ": " << to_string(tmp) << endl;
#endif
		if((chi2 < 0) or (tmp < chi2)) {
			index = i;
			chi2 = tmp;
		}
	}

	clog << "Guessing offset of " << to_string(index) << endl;
	decryptFile(ifile, ofile, index);

	return 0;
}

// Calculate the Chi-squared value of a vector of floats with some offset
float calcChi2(const vector<float>& vect, int offset)
{
	auto retVal{0.};
	for(auto i{0u}; i < vect.size(); i++) {
		auto observed = vect[i];
		auto expected = fa[(i - offset + alpha_size) % alpha_size];
		auto error = observed - expected;
		retVal += error * error / expected;
	}

	return retVal;
}
