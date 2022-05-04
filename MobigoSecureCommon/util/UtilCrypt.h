#pragma once
#include <string>

class AFX_EXT_CLASS CCrypt
{
public:
	CCrypt(void);
	~CCrypt(void);

public:
	static void	DataForCipher(std::string CryptText, std::string &ReturnValue, BOOL bCipher = TRUE, BOOL bSHA1 = FALSE);
    static void	CharToByte(char* chars, byte* bytes);
    static void	ByteToChar(byte* bytes, char* chars, unsigned int count);
	//////////////////////////////////////////////////////////////////////////
    static std::string	randomStrGen(int length);
    static std::string	RSAEncryptString(std::string publickeyfile, std::string message);
    static std::string	RSADecryptString(std::string privatekeyfile, std::string ciphertext);
    static void	EncryptFile_CryptoPP(std::string publickeyfile, std::string filetoencrypt, std::string encryptedfile);
    static void	DecryptFile_CryptoPP(std::string privatekeyfile, std::string filetodecrypt, std::string decryptedfile);
    static void	GenerateRSAKey(unsigned int keyLength, std::string privFilename, std::string pubFilename);
    static std::string AESDecryptString(const char *instr, const char *passPhrase);
};