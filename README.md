# Crypto Magic Encryption/Decryption Software

Crypto Magic is a basic encryptor and decryptor software that uses File I/O and string logic. 

## Inputs

There are two subcategories of inputs for this program: required arguments and optional arguments.

### Required Inputs
- File path of the file to be encrypted or decrypted

### Optional Inputs
- Encrypt (-E): specifies to the program that it will encrypt the file (By default this option is set to true if neither decrypt or encrypt is specified).

- Decrypt (-D): specifies to the program that it will decrypt the contents of the provided file.

- Keep original file (-KEEP-ORIG): specifies to the program to keep the initial file before encryption/decryption after all operations are complete (By default the program will delete the initial file).

- Alternate output path (-O): specifies to the program that the proceeding argument is an alternate path to output to for the current execution (default output directory is \output\\). 

## Outputs

There is only one output for the program which is the resulting encrypted/decrypted file, at either the default location or an alternate one specified with -O