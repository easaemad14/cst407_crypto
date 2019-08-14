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
#include <array>
#include <cstring>
#include <algorithm>

#include "encrypt.hpp"

using namespace std;


/**
 * Global variables
 */
// English is assumed for now
static constexpr auto alpha_size{26}; // Size of our alphabet
constexpr auto upper_offset{65}; // Man ascii
constexpr auto lower_offset{97};
constexpr float fa_vals[alpha_size] = {8.167, 1.492, 2.782, 4.253, 12.702,
	2.228, 2.015, 6.094, 6.966, .153, .772, 4.025, 2.406, 6.749, 7.507, 1.929,
	.095, 5.987, 6.327, 9.056, 2.758, .978, 2.36, .15, 1.974, .074};


/**
 * Inline Functions
 */
// Encode plaintext character p with some key character k, to upper
constexpr char encChar(char p, char k)
{
	auto pVal = toupper(p) - upper_offset;
	auto kVal = toupper(k) - upper_offset;
	return (pVal + kVal) % alpha_size + upper_offset;
}

// Decode ciphertext character c with some key character k, to lower
constexpr char decChar(char c, char k)
{
	auto cVal = tolower(c) - lower_offset;
	auto kVal = tolower(k) - lower_offset;
	return (cVal - kVal + alpha_size) % alpha_size + lower_offset;
}


/**
 * Functions
 */
// Encrypt one file to another
// Returns zero on success, else appropriate errno
int encryptFileToFile()
{
	auto pt = getStringFromUser("Please provide plaintext file: ");
	auto ct = getStringFromUser("Please provide ciphertext file: ");
	auto key = getStringFromUser("Please provide key: ");

	// Open our files
	ifstream ifs(pt);
	if(not ifs.is_open()) {
		cerr << "Could not open plain text file!" << endl;
		return -ENOENT;
	}
	ofstream ofs(ct);
	if(not ofs.is_open()) {
		cerr << "Could not open cipher text file!" << endl;
		return -ENOENT;
	}

	// Check validity of key
	if(end(key) not_eq (find_if(begin(key), end(key), [] (int c) {
				return not isalpha(c); }))) {
		cerr << "Invalid key: " << key << endl;
		return -EINVAL;
	}
	clog << "Encrypting characters from : " << pt << " to " << ct << endl;

	// Read the file and do a little magic
	char p;
	auto i{0};
	while(ifs.get(p)) {
		// Ensure we are only operating on alphabetic characters
		if(not isalpha(p)) {
			continue;
		}

		ofs << encChar(p, key.at(i++ % key.length()));
	}

	// Close our files and return success
	ifs.close(); ofs.close();
	clog << "Encryption complete" << endl;
	return 0;
}

// Decrypt one file to another; run from main with unknown values
// Returns zero on success, else appropriate errno
int decryptFileToFile()
{
	auto ct = getStringFromUser("Please provide ciphertext file: ");
	auto pt = getStringFromUser("Please provide plaintext file: ");
	auto key = getStringFromUser("Please provide key: ");

	return decryptFileToFile(ct, pt, key);
}

// Overloaded function to run with known values
int decryptFileToFile(const string& ct, const string& pt, const string& key)
{
	ifstream ifs(ct);
	if(not ifs.is_open()) {
		cerr << "Could not open cipher text file!" << endl;
		return -ENOENT;
	}
	ofstream ofs(pt);
	if(not ofs.is_open()) {
		cerr << "Could not open plain text file!" << endl;
		return -ENOENT;
	}
	if(end(key) not_eq (find_if(begin(key), end(key), [] (int c) {
				return not isalpha(c); }))) {
		cerr << "Invalid key: " << key << endl;
		return -EINVAL;
	}
	clog << "Decrypting characters from " << ct << " to " << pt << endl;

	string big_buf; // Ciphertext is assumed to be only characters
	getline(ifs, big_buf); // Really big lines
	ofs << decryptStringToString(big_buf, key);

	ifs.close(); ofs.close();
	clog << "Decryption complete" << endl;
	return 0;
}

// Attempt to crack ciphertext, writing the best match to disk
// Returns zero on success, else appropriate errno
int crack()
{
	auto ct = getStringFromUser("Please provide ciphertext file: ");
	auto pt = getStringFromUser("Please provide plaintext file: ");
	auto retVal = getKeyLength(ct);

	if(retVal == 0) {
		clog << "NOTE: IC for English is approximately 1.73" << endl;
		auto length = getValueFromUser<int>("Please provide key length: ");

		clog << "Attempting to crack cipher..." << endl;
		string key; // Keep track of best key match
		vector<float> fa(alpha_size); // FA calculation of each decryption
		auto chi2{-1.}; // Keep track of best chi^2 value

		// Loop through all possible combinations (no optimization)
		for(string i(length, 'a'); ; incrementKeyString(i)) {
			auto ptStr = decryptFileToString(ct, i); // Get possible plaintext
			auto count = calculateFA(ptStr, fa); (void)count;
			auto chi = calcChi2(fa);
#if 0
			clog << "Chi2 value of key " << i << ": " << to_string(chi) << endl;
#endif

			if((chi2 < 0) or (chi < chi2)) {
				chi2 = chi;
				key = i;
			}

			// Check terminating case after checking
			string end(length, 'z');
			if(i.compare(end) == 0) {
				break;
			}
		}

		clog << "Guessing key used: " << key << endl;
		decryptFileToFile(ct, pt, key);
	}

	return retVal;
}

// Return decrypted string ct using key
string decryptStringToString(const string& ct, const string& key)
{
	string retVal;
	for(auto i{0u}; i < ct.length(); i++) {
		retVal.push_back(decChar(ct.at(i), key.at(i % key.length())));
	}

	return retVal;
}

// Return decrypted file using key to string
string decryptFileToString(const string& file, const string& key)
{
	ifstream ifs(file);
	if(not ifs.is_open()) {
		cerr << "Could not open cipher text file!" << endl;
		return "";
	}
	if(end(key) not_eq (find_if(begin(key), end(key), [] (int c) {
				return not isalpha(c); }))) {
		cerr << "Invalid key!" << endl;
		return "";
	}

	// Read the ciphertext from file and close
	string big_buf;
	getline(ifs, big_buf);
	ifs.close();

	// Return decrypted string
	return decryptStringToString(big_buf, key);
}

// Rolling increment of string
void incrementKeyString(string& str)
{
	for(auto it = str.rbegin(); it not_eq str.rend(); it++) {
		if(*it == 'z') {
			*it = 'a';
		} else {
			*it += 1;
			break; // Only continue to next character if roll over
		}
	}
}

// Calculate the Chi-squared value of a vector with FA data against English
float calcChi2(const vector<float>& vect)
{
	auto retVal{0.};
	for(auto i{0u}; i < vect.size(); i++) {
		auto error = vect[i] - fa_vals[i];
		retVal += error * error / fa_vals[i];
	}

	return retVal;
}

// Provide IC values based off of varying key lengths
// Returns zero on success, else appropriate errno
int getKeyLength(const string& ct)
{
	auto depth = getValueFromUser<int>("Please provide depth of analysis: ");

	ifstream ifs(ct);
	if(not ifs.is_open()) {
		cerr << "Could not open plain text file!" << endl;
		return -ENOENT;
	}
	if(depth < 1) {
		cerr << "Depth not valid!" << endl;
		return -EINVAL;
	}

	// Check ifile for possible key lengths
	for(auto i{1}; i < depth; i++) {
		ifs.seekg(0); // Rewind to beginning of file

		// Generate two dimensional map of strings based off of potential key length
		vector<string> ic_map;
		string tmp;
		getline(ifs, tmp); // Really big lines
		for(auto j{0u}; j < tmp.length(); j += i) {
			ic_map.push_back(tmp.substr(j, i));
		}

		// Output data for user
		clog << "IC of key length " << to_string(i) << ": " <<
			to_string(calcICFromLength(ic_map, i)) << endl;
	}

	ifs.close();
	return 0;
}

// Return calculated IC from two dimensional mapping of strings
float calcICFromLength(const vector<string>& ics, int len)
{
	auto retVal{0.};

	for(auto i{0}; i < len; i++) {
		string tmp;
		for(auto j{0u}; j < ics.size(); j++) {
			try { // The last string may not be full (out_of_bounds)
				tmp.push_back(ics.at(j).at(i));
			} catch(...) {
				break;
			}
		}

		// Calculate IC values for every column
		retVal += calcIC(tmp) / len;
	}

	return retVal;
}

// Returns calculated IC of string
// https://en.wikipedia.org/wiki/Index_of_coincidence#Calculation
float calcIC(const string& str)
{
	auto retVal{0.};
	int counts[alpha_size];
	memset(counts, 0, sizeof(counts));

	if(str.length() < 2) {
		return 1; // Normalize towards one
	}

	// Count all characters in our string
	for(auto c : str) {
		++counts[toupper(c) - upper_offset];
	}

	// Calculate IC from all counts
	for(auto i{0u}; i < alpha_size; i++) {
		retVal += static_cast<float>(alpha_size) * counts[i] * (counts[i] - 1) /
			str.length() / (str.length() - 1);
	}

	return retVal;
}

// Store calculated Frequency Analysis of string str in vector vect
// Returns number of characters in string
int calculateFA(const string& str, vector<float>& vect)
{
	auto retVal{0};

	// Ensure all values are zeroed before calculation
	for(auto& i : vect) {
		i = 0;
	}

	// Count all characters in string
	for(auto c : str) {
		vect.at(tolower(c) - lower_offset) += 1;
		++retVal;
	}

	// Now, we need to get percentage of each character read
	for(auto i{0u}; i < vect.size(); i++) {
		vect.at(i) /= retVal;
	}

	return retVal;
}

// Provide prompt to user and return input as a string
string getStringFromUser(const string& prompt)
{
	string retVal;
	clog << prompt;
	getline(cin, retVal);
	return retVal;
}
