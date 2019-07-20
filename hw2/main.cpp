/*******************************************************************************
 * File:        main.cpp
 * Author:      sirganye
 *              easa.elsirgany@3dsystems.com
 *
 * Description: Main for Simple Data Encryption Standard (sDES) written like a
 *              hacker in order to match the mathematical structure of the doc.
 ******************************************************************************/
#include <iostream>
#include <string>
#include <cerrno>

using namespace std;


/**
 * Global variables
 */
string inout, key, k1, k2;
bool enc;
constexpr int num_offset = 48;

const int sbox0[4][4] = {
	{1, 0, 3, 2},
	{3, 2, 1, 0},
	{0, 2, 1, 3},
	{3, 1, 3, 2}
};

const int sbox1[4][4] = {
	{0, 1, 2, 3},
	{2, 0, 1, 3},
	{3, 0, 1, 0},
	{2, 1, 0, 3}
};


/**
 * Encryption functions
 */
// Permute 10
string p10(const string& k)
{
	// See S-DES Key Generation from handout
	string retVal;
	retVal.push_back(k.at(2));
	retVal.push_back(k.at(4));
	retVal.push_back(k.at(1));
	retVal.push_back(k.at(6));
	retVal.push_back(k.at(3));
	retVal.push_back(k.at(9));
	retVal.push_back(k.at(0));
	retVal.push_back(k.at(8));
	retVal.push_back(k.at(7));
	retVal.push_back(k.at(5));

	return retVal;
}

// Permute 8
string p8(const string& k)
{
	string retVal;
	retVal.push_back(k.at(5));
	retVal.push_back(k.at(2));
	retVal.push_back(k.at(6));
	retVal.push_back(k.at(3));
	retVal.push_back(k.at(7));
	retVal.push_back(k.at(4));
	retVal.push_back(k.at(9));
	retVal.push_back(k.at(8));

	return retVal;
}

string p4(const string& k)
{
	string retVal;
	retVal.push_back(k.at(1));
	retVal.push_back(k.at(3));
	retVal.push_back(k.at(2));
	retVal.push_back(k.at(0));

	return retVal;
}

// Circular left shift on string k
string cls(const string& k, int num)
{
	string retVal;
	for(auto i{0}; i < num; i++) {
		retVal = k.substr(1);
		retVal.push_back(k.at(0));
	}

	return retVal;
}

// Circular left shift on each half of input
string shift(const string& k, int num = 1)
{
	return cls(k.substr(0, 5), num) + cls(k.substr(5), num);
}

// Generate key values
void genKey1()
{
	k1 = p8(shift(p10(key)));
}

void genKey2()
{
	k2 = p8(shift(shift(p10(key)), 2));
}

// Initial permutation
string ip(const string& k)
{
	string retVal;
	retVal.push_back(k.at(1));
	retVal.push_back(k.at(5));
	retVal.push_back(k.at(2));
	retVal.push_back(k.at(0));
	retVal.push_back(k.at(3));
	retVal.push_back(k.at(7));
	retVal.push_back(k.at(4));
	retVal.push_back(k.at(6));

	return retVal;
}

// Final permutation
string fp(const string& k)
{
	string retVal;
	retVal.push_back(k.at(3));
	retVal.push_back(k.at(0));
	retVal.push_back(k.at(2));
	retVal.push_back(k.at(4));
	retVal.push_back(k.at(6));
	retVal.push_back(k.at(1));
	retVal.push_back(k.at(7));
	retVal.push_back(k.at(5));

	return retVal;
}

// Expand/permute
string ep(const string& text)
{
	string retVal;
	retVal.push_back(text.at(3));
	retVal.push_back(text.at(0));
	retVal.push_back(text.at(1));
	retVal.push_back(text.at(2));
	retVal.push_back(text.at(1));
	retVal.push_back(text.at(2));
	retVal.push_back(text.at(3));
	retVal.push_back(text.at(0));

	return retVal;
}

// xor elements withing strings
string xorStr(const string& a, const string& b)
{
	string retVal;
	for(auto i{0u}; i < a.length(); i++) {
		retVal += to_string((a.at(i) - num_offset) xor (b.at(i) - num_offset));
	}

	return retVal;
}

// S-box functions
string sb(const string& text, const int box[][4])
{
	auto row = 2 * (text.at(0) - num_offset) + (text.at(3) - num_offset);
	auto col = 2 * (text.at(1) - num_offset) + (text.at(2) - num_offset);
	return to_string(box[row][col] / 2) + to_string(box[row][col] % 2);
}

// Function Fk
string fk(const string& text, const string& key)
{
	// Create mutable halves of text
	auto msb = text.substr(0, 4), lsb = text.substr(4);

	// Generate subkey
	auto sk = xorStr(key, ep(lsb));
	sk = p4(sb(sk.substr(0, 4), sbox0) + sb(sk.substr(4), sbox1));

	return xorStr(msb, sk) + lsb;
}

// Simplification of functions to match equations
string fk1(const string& text)
{
	return fk(text, k1);
}

string fk2(const string& text)
{
	return fk(text, k2);
}

// Switch top and bottom halves of 8-bit text
string sw(const string& text)
{
	return text.substr(4) + text.substr(0, 4);
}

// Encrypt input
void des()
{
	inout = (enc) ?
		fp(fk2(sw(fk1(ip(inout))))) :
		fp(fk1(sw(fk2(ip(inout)))));
}

// Validate input and expected length
bool isValidInput(string& input, unsigned int length)
{
	auto retVal{false};

	// If length matches and input only contains 1s and 0s, valid
	if((input.length() == length) and
			(input.find_first_not_of("10") == string::npos)) {
		retVal = true;
	}
	return retVal;
}

int main(int, char**)
{
	auto retVal{0};
	string buf;

	// Prompt user for encrypt/decrypt, binary text, and key
	clog << "Would you like to encrypt [Y/n]: ";
	getline(cin, buf);
	enc = (buf[0] == 'n' or buf[0] == 'N') ? false : true;

	// Prompt user for plaintext or ciphertext and check validity
	clog << "8-bit binary " << (enc ? "plaintext: " : "ciphertext: ");
	getline(cin, inout);
	if(not isValidInput(inout, 8)) {
		cerr << "Input must be 8 bits and only contains 1s and 0s!" << endl;
		retVal = -EINVAL;
		goto exit;
	}

	// Prompt user for key and check validity
	clog << "10-bit binary key: ";
	getline(cin, key);
	if(not isValidInput(key, 10)) {
		cerr << "Input must be 10 bits and only contains 1s and 0s!" << endl;
		retVal = -EINVAL;
		goto exit;
	}

	// Perform operations
	genKey1();
	genKey2();
	des();

	// Log key values and output from sDES
	clog << "Key1: " << k1 << endl;
	clog << "Key2: " << k2 << endl;
	clog << (enc ? "Ciphertext: " : "Plaintext: ") << inout << endl;

exit:
	// Nothing to clean up; just exit
	return retVal;
}
