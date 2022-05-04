#pragma once

#include <msxml.h>

class AFX_EXT_CLASS CUtil2
{
public:
	CUtil2(void);
	~CUtil2(void);

	static CString			Int2Str(			int	    n   );
	static CString			Float2Str(			float   f   );
	static CString			Float2DecimalStr(	float	f	);
	static CString			Long2Str(			long	l   );
	static CString			Bool2Str(			BOOL    b   );
	static CString			Bool2Option(	  	BOOL    b   );
	static BOOL			    Option2Bool(	  	CString str );
	static int				Str2Int(			CString str );
	static long				Str2Long(			CString str );
	static float			Str2Float(			CString str );
	static BOOL				Str2Bool(			CString str );

	static CRgn* 			Rect2Rgn(			CRect &rect);
	static CString 			Rect2Str(			CRect &rect);

	static CString				Int2K(				DWORD dw );
	static CString				Int2M(				DWORD dw );
	static CString				Int2G(				DWORD dw );
	static CString			Int2Money(			int     n );
	static CString			Int2Money(			CString strInt );

	static long				Time2Second(		COleDateTimeSpan span );
	static CString			Str2Star(           CString str );
	static CStringArray*	Str2StrArray(       CString str );

	static CString			Point2Str(			CPoint point );
	static int				COMStr2COMNum(		CString strCOM );
	static void				StrIP2ByteIP(       CString strIP, BYTE btAyIP[4] );
	static BOOL   is_CallDigit_STR( char* str );  // [KTFT-SMPARK] 2005-05-09 - Call Digit Check
};
