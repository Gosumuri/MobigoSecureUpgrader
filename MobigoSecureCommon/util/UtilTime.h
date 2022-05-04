#pragma once

class AFX_EXT_CLASS CUtilTime
{
public:
	CUtilTime(void);
	~CUtilTime(void);

// Time
public :
	static CString			GetLocalDate(); // local system
	static CString			GetLocalTime(); // local system	
	static CString			GetLocalNow();		// 20040720
	static CString			GetLocalDateTime();	// 20040720_202411
	static COleDateTime		GetMaxTime();	
	static COleDateTimeSpan DiffTime( COleDateTime, COleDateTime ); 

	static CString			Time2Str(			COleDateTime     datetime     );
	static CString			TimeSpan2Str(		COleDateTimeSpan datetimeSpan );
	static COleDateTime		Str2Time(			CString strTime );
	static COleDateTimeSpan Str2TimeSpan(		CString strTimeSpan );

	static int				Month2Day(			int nMonth );

};


