#pragma once

////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include <tchar.h>
#include <wininet.h>

// use stl
#include <vector>

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////


// PRE-DEFINED CONSTANTS
#define __DEFAULT_AGENT_NAME			"MERONG(0.9/;p)"

// PRE-DEFINED BUFFER SIZE
#define	__SIZE_HTTP_ARGUMENT_NAME		256
#define __SIZE_HTTP_ARGUMENT_VALUE		1024

#define __HTTP_VERB_GET					"GET"
#define __HTTP_VERB_POST				"POST"
#define __HTTP_ACCEPT_TYPE				"*/*"
#define __HTTP_ACCEPT					"Accept: */*\r\n"
#define __SIZE_HTTP_BUFFER				100000
#define __SIZE_HTTP_RESPONSE_BUFFER		100000
#define __SIZE_HTTP_HEAD_LINE			2048

#define __SIZE_BUFFER					2048
#define __SIZE_SMALL_BUFFER				256

////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// class declare
////////////////////////////////////////////////////////////////////////////////

class AFX_EXT_CLASS CUtilHttpPost
{
public:					
	typedef struct __GENERIC_HTTP_ARGUMENT{							// ARGUMENTS STRUCTURE
		TCHAR	szName[__SIZE_HTTP_ARGUMENT_NAME];
		TCHAR	szValue[__SIZE_HTTP_ARGUMENT_VALUE];
		DWORD	dwType;
		BOOL    operator==(const __GENERIC_HTTP_ARGUMENT &argV)
		{
			return !_tcscmp(szName, argV.szName) && !_tcscmp(szValue, argV.szValue);
		}
	} GenericHTTPArgument;

	enum RequestMethod{															// REQUEST METHOD
		RequestUnknown=0,
		RequestGetMethod=1,
		RequestPostMethod=2,
		RequestPostMethodMultiPartsFormData=3
	};

	enum TypePostArgument{													// POST TYPE 
		TypeUnknown=0,
		TypeNormal=1,
		TypeBinary=2
	};

	// CONSTRUCTOR & DESTRUCTOR
	CUtilHttpPost();
	virtual ~CUtilHttpPost();

	static CUtilHttpPost::RequestMethod GetMethod(int nMethod);
	static CUtilHttpPost::TypePostArgument GetPostArgumentType(int nType);

	// Connection handler	
	BOOL Connect(LPCTSTR szAddress, LPCTSTR szAgent = __DEFAULT_AGENT_NAME, unsigned short nPort = INTERNET_DEFAULT_HTTP_PORT, LPCTSTR szUserAccount = NULL, LPCTSTR szPassword = NULL);
	BOOL Close();
	VOID InitilizePostArguments();

	// HTTP Arguments handler	
	VOID AddPostArguments(LPCTSTR szName, DWORD nValue);
	VOID AddPostArguments(LPCTSTR szName, LPCTSTR szValue, BOOL bBinary = FALSE);

	// HTTP Method handler 
	BOOL Request( LPCTSTR szURL, int nMethod = CUtilHttpPost::RequestGetMethod, LPCTSTR szAgent = __DEFAULT_AGENT_NAME);
	BOOL RequestOfURI(LPCTSTR szURI, int nMethod = CUtilHttpPost::RequestGetMethod);
	BOOL Response(PBYTE pHeaderBuffer, DWORD dwHeaderBufferLength, PBYTE pBuffer, DWORD dwBufferLength, DWORD &dwResultSize);	
	LPCTSTR QueryHTTPResponse();
	LPCTSTR QueryHTTPResponseHeader();	

	// General Handler
	DWORD GetLastError();
	LPCTSTR GetContentType(LPCTSTR szName);
	VOID ParseURL(LPCTSTR szURL, LPTSTR szProtocol, LPTSTR szAddress, DWORD &dwPort, LPTSTR szURI);

	
protected:				
	std::vector<GenericHTTPArgument> _vArguments;				// POST ARGUMENTS VECTOR

	TCHAR		_szHTTPResponseHTML[__SIZE_HTTP_BUFFER];		// RECEIVE HTTP BODY
	TCHAR		_szHTTPResponseHeader[__SIZE_HTTP_BUFFER];	// RECEIVE HTTP HEADR

	HINTERNET	_hHTTPOpen;				// internet open handle
	HINTERNET	_hHTTPConnection;		// internet connection hadle
	HINTERNET	_hHTTPRequest;		// internet request hadle

	DWORD		_dwError;					// LAST ERROR CODE
	LPCTSTR		_szHost;					 //	 HOST NAME
	DWORD		_dwPort;					//  PORT

	// HTTP Method handler
	DWORD	ResponseOfBytes(PBYTE pBuffer, DWORD dwSize);
	DWORD	GetPostArguments(LPTSTR szArguments, DWORD dwLength);
	BOOL	RequestPost(LPCTSTR szURI);
	BOOL	RequestPostMultiPartsFormData(LPCTSTR szURI);
	BOOL	RequestGet(LPCTSTR szURI);
	DWORD	AllocMultiPartsFormData(PBYTE &pInBuffer, LPCTSTR szBoundary = "--MULTI-PARTS-FORM-DATA-BOUNDARY-");
	VOID	FreeMultiPartsFormData(PBYTE &pBuffer);
	DWORD	GetMultiPartsFormDataLength();
private:
	TCHAR			m_szPostArguments[__SIZE_BUFFER];
};
