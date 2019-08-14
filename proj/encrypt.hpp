#pragma once
/*******************************************************************************
 * File:        encrypt.hpp
 * Author:      eazy
 *              easa.elsirgany@oit.com
 *
 * Description: Encryption functions and tools
 ******************************************************************************/
#include <string>
#include <vector>
#include <iostream>

// User operations
int encryptFileToFile();
int decryptFileToFile();
int decryptFileToFile(const std::string&, const std::string&, const std::string&);
int crack();

// Helper functions
std::string decryptStringToString(const std::string&, const std::string&);
std::string decryptFileToString(const std::string&, const std::string&);
void incrementKeyString(std::string&);
float calcChi2(const std::vector<float>&);
int getKeyLength(const std::string&);
float calcICFromLength(const std::vector<std::string>&, int);
float calcIC(const std::string&);
int calculateFA(const std::string&, std::vector<float>&);
std::string getStringFromUser(const std::string&);

// Templates
template<class T>
T getValueFromUser(const std::string& prompt)
{
	auto value{0ull};
	std::string buf;
	std::clog << prompt;
	std::getline(std::cin, buf);

	try {
		value = std::stoll(buf);
	} catch(...) {
		std::cerr << "Invalid value: " << buf << std::endl;
	}

	return static_cast<T>(value);
}
