#pragma once
/*******************************************************************************
 * File:        encrypt.hpp
 * Author:      eazy
 *              easa.elsirgany@oit.com
 *
 * Description: Encryption functions and tools
 ******************************************************************************/
#include <string>

int encryptFile(const std::string&, const int);
int decryptFile(const std::string&, const int);
int crackFile(const std::string&);
