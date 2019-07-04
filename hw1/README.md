# Caesar Cipher
This is the first homework assignment for cst407 cryptography.

The *caesar* executable provides functionality for encrypting and decrypting
files using the Caesar Cipher. If key value is unknown, it will attempt to guess
the plaintext using frequency analysis. Therefore, the longer the ciphertext,
the more accurate this program will be.

## Usage
*caesar* can be used without any options, at which point you will be prompted
for the needed information at runtime.

*caesar* comes with a built-in help menu if you use the following command:
`./caesar -h`

The default operation of *caesar* is encryption, but you can use the **-d** option
for decryption mode. If you do not specify, you will be prompted to ensure that
encryption is the option that you want.

The key value (offset) can be specified with the **-k** value. This can be a
numerical offset, or a character (which should be the translation from 'a'). If
a key is not provided when running decryption, *caesar* will attempt to crack
the encryption. If a key is not provided when encrypting, this is a fault and
the program will exit with an error.

You can also specify the plain-text and cipher-text file names (full paths are
okay) with the **-p** and **-c** options. From the homework specification (the
included .doc file), all file names are dependent upon the user, so both of
these values will need to be set from either options or at runtime.

## Specifications
*caesar* implements the [Caesar
cipher](https://en.wikipedia.org/wiki/Caesar_cipher).

### Key Values and Offset
Since the alphabet in a Caesar Cipher is merely shifted by some offset, the
ciphertext is assumed to have the same ordering. Therefore, if you pass a key
value of 'n' as your offset, plain-text 'a' will become 'n', 'b' will become
'o', etc.

Encryption uses the following algorithm: `c = (p + k) % a`, where 'c' is the
cipher-text character, 'p' is the plain-text character, 'k' is the key or
offset, and 'a' is the size of the alphabet. Decryption uses the following
algorithm: `p = (c - k) % a`.

Since the key offset is expected to be in range of the alphabet size, the offset
is normalized. For example, if you use an offset of -1 the key value will be 25,
and if you use a key value of 53 the key value will be 1.

### Cracking Caesar
If *caesar* is run in decrypt mode without a key value, it will attempt to crack
the cipher using the Chi-squared test on the known frequency analysis values of
the English language.
