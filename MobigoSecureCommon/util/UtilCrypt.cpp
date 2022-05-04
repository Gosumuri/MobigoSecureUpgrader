#include "stdafx.h"
#include "UtilCrypt.h"

// CryptoPP 라이브러리 내부에 붙여서 사용
// 기존 crypt.lib파일은 제거됨
// 사용헤더 값만 추가
#include "hex.h"
#include "aes.h"
#include "rsa.h"
#include "sha.h"
#include "osrng.h"
#include "modes.h"
#include "files.h"
#include "base64.h"
#include "filters.h"
#include "default.h"
#include "randpool.h"
#include "validate.h"

#pragma comment(lib, "cryptlib")  

using namespace CryptoPP;

static OFB_Mode<AES>::Encryption s_globalRNG;

RandomNumberGenerator & GlobalRNG()
{
	return s_globalRNG;
}

CCrypt::CCrypt(void)
{
}

CCrypt::~CCrypt(void)
{
}

/*************************************************************************
	암호화처리 - 암호화 처리 실 함수
	*************************************************************************/
void CCrypt::DataForCipher(std::string CryptText, std::string &ReturnValue, BOOL bCipher, BOOL bSHA1)
{
	// PHP에서 Padding을 ZEROS_PADDING만 사용한다.
	// key, iv(고정값) AES128
	// 로그인시 전달값 - 암호화 및 Base64 Encoding [생성시간, ID정보, 메뉴주소 전달]
	// 형태: http://aaa.bbb.ccc/login.php&data1=currenttime&data2=id&data3=community
	// 암호화시 시간값도 같이 구해 버린다.

	//////////////////////////////////////////////////////////////////////////
#include <time.h>
	time_t rawtime;
	struct tm* timeinfo;
	char currentTime[32] = { 0 };

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(currentTime, sizeof(currentTime), "%y/%m/%d %H:%M:%S", timeinfo);
	//////////////////////////////////////////////////////////////////////////

	TCHAR *rawKey = _T("ahqlrhahqlrh!@#");
	TCHAR *rawIv = _T("http://mobigo.co");

	byte key[AES::DEFAULT_KEYLENGTH] = { 0 };
	byte iv[AES::BLOCKSIZE] = { 0 };
	CharToByte(rawKey, key);
	CharToByte(rawIv, iv);

	std::string base64encodedciphertext = ""; // 암호화된 text를 Encoding
	std::string ciphertext = ""; // 암호화된 text

	std::string base64decryptedciphertext = ""; // 암호화된 text를 Decoding
	std::string decryptedtext = ""; // 복호화된 text

	SHA1 sha1;
	std::string hashvalue = "";

	if (bCipher)
	{
		if (bSHA1 == FALSE)
		{
			//////////////////////////////////////////////////////////////////////////
			// AES Encrypt
			AES::Encryption aesEncryption(key, AES::DEFAULT_KEYLENGTH);
			CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, iv);

			StreamTransformationFilter stfEncryptor(cbcEncryption, new StringSink(ciphertext), StreamTransformationFilter::ZEROS_PADDING);
			stfEncryptor.Put(reinterpret_cast<const unsigned char*>(CryptText.c_str()), CryptText.size());
			stfEncryptor.MessageEnd();

			// Base64 Encoding - ciphertext
			StringSource(ciphertext, true, new Base64Encoder(new StringSink(base64encodedciphertext)));
			// 결과 - base64encodedciphertext (AES + BASE64)
			ReturnValue = base64encodedciphertext;
		}
		else
		{
			// hash SHA1
			StringSource(CryptText, true, new HashFilter(sha1, new HexEncoder(new StringSink(hashvalue), false)));
			ReturnValue = hashvalue;
		}
	}
	else
	{
		//////////////////////////////////////////////////////////////////////////
		// 디코딩 부분 주석 처리
		// Base64 Decoding
        StringSource(CryptText, true, new Base64Decoder(new StringSink(base64decryptedciphertext)));

		//////////////////////////////////////////////////////////////////////////
		// AES Decrypt
		AES::Decryption aesDecryption(key, AES::DEFAULT_KEYLENGTH);
		CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, iv);

		StreamTransformationFilter stfDecryptor(cbcDecryption, new StringSink(decryptedtext), StreamTransformationFilter::ZEROS_PADDING);
		stfDecryptor.Put(reinterpret_cast<const unsigned char*>(base64decryptedciphertext.c_str()), base64decryptedciphertext.size());
		stfDecryptor.MessageEnd();
		// 결과 - decryptedtext (BASE64 + AES)
		ReturnValue = decryptedtext;
	}
}

/*************************************************************************
	암호화처리 - 암호화 처리 내부에서 변수값 변경을 위해 구현
	*************************************************************************/
void CCrypt::CharToByte(char* chars, byte* bytes)
{
	for (unsigned int i = 0; i < strlen(chars); i++)
		bytes[i] = (byte)chars[i];
}

void CCrypt::ByteToChar(byte* bytes, char* chars, unsigned int count)
{
	for (unsigned int i = 0; i < count; i++)
		chars[i] = (char)bytes[i];
}

//################################################################################################################################################

// Helper function
std::string CCrypt::randomStrGen(int length)
{
	static std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
	std::string result;
	result.resize(length);
	for (int i = 0; i < length; i++) {
		result[i] = charset[rand() % charset.length()];
	}
	return result;
}

void CCrypt::GenerateRSAKey(unsigned int keyLength, std::string privFilename, std::string pubFilename)
{
	std::string seed = randomStrGen(50);

	RandomPool randPool;
	randPool.Put((byte *)seed.c_str(), strlen(seed.c_str()));
	RSAES_OAEP_SHA_Decryptor priv(randPool, keyLength);

	HexEncoder privFile(new FileSink(privFilename.c_str()));
	priv.DEREncode(privFile);
	privFile.MessageEnd();

	RSAES_OAEP_SHA_Encryptor pub(priv);
	HexEncoder pubFile(new FileSink(pubFilename.c_str()));
	pub.DEREncode(pubFile);
	pubFile.MessageEnd();
}

// Encrypt string with RSA
std::string CCrypt::RSAEncryptString(std::string publickeyfile, std::string message)
{
	std::string result;
	std::string seed = randomStrGen(50);

	FileSource pubFile(publickeyfile.c_str(), true, new HexDecoder);
	RSAES_OAEP_SHA_Encryptor pub(pubFile);

	RandomPool randPool;
	randPool.IncorporateEntropy((byte *)seed.c_str(), strlen(seed.c_str()));

	StringSource(message.c_str(), true, new PK_EncryptorFilter(randPool, pub, new HexEncoder(new StringSink(result))));
	return result;
}

// Decrypt string with RSA
std::string CCrypt::RSADecryptString(std::string privatekeyfile, std::string ciphertext)
{
	std::string seed2 = IntToString(time(NULL));
	seed2.resize(16);
	s_globalRNG.SetKeyWithIV((byte *)seed2.c_str(), 16, (byte *)seed2.c_str());

	std::string result;

	FileSource privFile(privatekeyfile.c_str(), true, new HexDecoder);
	RSAES_OAEP_SHA_Decryptor priv(privFile);

	StringSource(ciphertext.c_str(), true, new HexDecoder(new PK_DecryptorFilter(GlobalRNG(), priv, new StringSink(result))));
	return result;
}

// Encrypt file in hybrid mode
void CCrypt::EncryptFile_CryptoPP(std::string publickeyfile, std::string filetoencrypt, std::string encryptedfile)
{
	try
	{
		// Generate passphrase
		std::string passphrase = randomStrGen(50);
		std::string encrypted_passphrase = RSAEncryptString(publickeyfile, passphrase);

		// Encrypt the file and save encrypted data to the string
		std::string data;
		FileSource f(filetoencrypt.c_str(), true, new DefaultEncryptorWithMAC(passphrase.c_str(), new HexEncoder(new StringSink(data))));

		// Append encrypted passphrase to the beginning of the string
		data = encrypted_passphrase + data;

		// Save string to the file
		StringSource ss(data.c_str(), true, new FileSink(encryptedfile.c_str()));
	}
	catch (Exception& e)
	{
		TRACE(e.what());
	}
}

// Decrypt file in hybrid mode
void CCrypt::DecryptFile_CryptoPP(std::string privatekeyfile, std::string filetodecrypt, std::string decryptedfile)
{
	try
	{
		// Read string from file
		std::string data;
		FileSource ff(filetodecrypt.c_str(), true, new StringSink(data));

		// Grab passphrase from the string
		std::string encrypted_passphrase = data.substr(0, 512);
		std::string passphrase = RSADecryptString(privatekeyfile, encrypted_passphrase);

		// Grab cipher from the string
		std::string cipher = data.substr(512, data.length());

		// Decrypt string and save to file
		StringSource s(cipher.c_str(), true, new HexDecoder(new DefaultDecryptorWithMAC(passphrase.c_str(), new FileSink(decryptedfile.c_str()))));
	}
	catch (Exception& e)
	{
		TRACE(e.what());
	}
}

////////////////////////////////////////////////////////////////////////////
//// AES
//string AESEncryptString(const char *instr, const char *passPhrase) // AES
//{
//	std::string outstr;
//	byte iv[AES::BLOCKSIZE]="123456";
//	AES::Encryption aesEncryption((byte *)passPhrase, AES::DEFAULT_KEYLENGTH);
//	CFB_Mode_ExternalCipher::Encryption cfbEncryption(aesEncryption, iv);
//	StreamTransformationFilter cfbEncryptor(cfbEncryption, new HexEncoder(new StringSink(outstr)));
//	cfbEncryptor.Put((byte *)instr, strlen(instr));
//	cfbEncryptor.MessageEnd();
//	return outstr;
//}
//
std::string CCrypt::AESDecryptString(const char *instr, const char *passPhrase)// AES
{
	std::string outstr;
	//byte iv[AES::BLOCKSIZE]="123456";
    TCHAR *rawKey = _T("ahqlrhahqlrh!@#");
    TCHAR *rawIv = _T("http://mobigo.co");

    byte key[AES::DEFAULT_KEYLENGTH] = { 0 };
    byte iv[AES::BLOCKSIZE] = { 0 };
    
    CharToByte(rawKey, key);
    CharToByte(rawIv, iv);

	CFB_Mode<AES >::Decryption cfbDecryption((byte *)passPhrase, AES::DEFAULT_KEYLENGTH, iv);
	HexDecoder decryptor(new StreamTransformationFilter(cfbDecryption, new StringSink(outstr)));
	decryptor.Put((byte *)instr, strlen(instr));
	decryptor.MessageEnd();
	return outstr;
}
//
//void AESEncryptFile(const char *in, const char *out, const char *passPhrase)//
//{
//	byte iv[AES::BLOCKSIZE]="123456";
//
//	AES::Encryption aesEncryption((byte *)passPhrase, AES::DEFAULT_KEYLENGTH);
//
//	CFB_Mode_ExternalCipher::Encryption cfbEncryption(aesEncryption, iv);
//
//	FileSource f(in, true, new StreamTransformationFilter(cfbEncryption, new FileSink(out)));
//}
//
//void AESDecryptFile(const char *in, const char *out, const char *passPhrase)//
//{
//	byte iv[AES::BLOCKSIZE]="123456";
//
//	CFB_Mode<AES >::Decryption cfbDecryption((byte *)passPhrase, 16, iv);
//
//	FileSource f(in, true, new StreamTransformationFilter(cfbDecryption, new FileSink(out)));
//}
//
////////////////////////////////////////////////////////////////////////////
//// DES-EDE
//string EncryptString(const char *instr, const char *passPhrase)
//{
//	string outstr;
//
//	DefaultEncryptorWithMAC encryptor(passPhrase, new HexEncoder(new StringSink(outstr)));
//	encryptor.Put((byte *)instr, strlen(instr));
//	encryptor.MessageEnd();
//
//	return outstr;
//}
//
//string DecryptString(const char *instr, const char *passPhrase)
//{
//	string outstr;
//
//	HexDecoder decryptor(new DefaultDecryptorWithMAC(passPhrase, new StringSink(outstr)));
//	decryptor.Put((byte *)instr, strlen(instr));
//	decryptor.MessageEnd();
//
//	return outstr;
//}