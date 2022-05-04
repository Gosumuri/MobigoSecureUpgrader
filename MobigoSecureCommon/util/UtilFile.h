#pragma once

class AFX_EXT_CLASS CUtilFile
{
public:
	CUtilFile(void);
	~CUtilFile(void);

	static void			DeleteDir(			CString strDirPath );
	static void			DeleteFile(			CString strFilePath );
	static void			WriteString(		CString strFilePath, CString str );
	static CString		ReadString(			CString strFilePath );
	static BOOL			IsExistPath(		CString strFilePath );
	static BOOL			IsDirectory(		CString strFilePath );	
	static TCHAR*		GetTypeName(		CString strFilePath	);
	static CString		ExtractFileName(	CString strFilePath );
	static CString		ExtractFileExt(		CString strFilePath );
	static CString		ExtractPath(				CString strFilePath );	
	static CString		ExtractFileNameWithoutExt(	CString strFilePath );	
	static int			GetSize(			CString strPath     );
	static int			GetFileSize(		CString strFilePath );
	static int			GetDirectorySize(	CString strDirectoryPath );
	static CString		GetNewFilePath(		CString strFilePath );
	static void			MakeDir(			CString strDirPath  );
private :
	static void			DeleteFolder(		LPTSTR  lpFolder);
};
