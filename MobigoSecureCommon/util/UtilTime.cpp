////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "utiltime.h"
#include "Util2.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// static member
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// constructor / destructor
////////////////////////////////////////////////////////////////////////////////


CUtilTime::CUtilTime(void)
{
}

CUtilTime::~CUtilTime(void)
{
}

////////////////////////////////////////////////////////////////////////////////
// public
////////////////////////////////////////////////////////////////////////////////

// COleDateTime의 "-" 연산자의 bug를 Fix하기위해 사용된다.
//  잘못된 연산의 예 
//		   2004-07-02 00-00-00
//      -  2004-07-01 00-00-00
//      ----------------------
//                  0 00-00-00
//
//  bug를 fix하여   1 00-00-00
COleDateTimeSpan CUtilTime::DiffTime( COleDateTime time1, COleDateTime time2 )
{
//	CDebug::Debug( "-----------------------------------------------------" );
//	CDebug::Debug( "time 1 : " + CUtil2::Time2Str( time1 ));
//	CDebug::Debug( "time 2 : " + CUtil2::Time2Str( time2 ));

	COleDateTimeSpan spanDiff = time1 - time2;
//	CDebug::Debug( "time 1 - time 2 : " + CUtil2::TimeSpan2Str( spanDiff ));
	

	if( // spanDiff.GetDays()    == 0
		   spanDiff.GetHours()   == 0
		&& spanDiff.GetMinutes() == 0
		&& spanDiff.GetSeconds() == 0 ) {
		if( time1 != time2 ) {

			COleDateTime timeTemp1 = COleDateTime( time1.GetYear(), time1.GetMonth(), time1.GetDay(), 1, 1, 1 );
			COleDateTime timeTemp2 = COleDateTime( time2.GetYear(), time2.GetMonth(), time2.GetDay(), 1, 1, 1 );

			spanDiff = timeTemp1 - timeTemp2;
		}
	}

//	CDebug::Debug( "time 1 - time 2 : " + CUtil2::TimeSpan2Str( spanDiff ));

	return spanDiff;
}

// return value : 15:15:22
CString CUtilTime::GetLocalTime()
{
	CTime time;
	time = CTime::GetCurrentTime();

	// hour
	CString strHour; 
	int nHour  = time.GetHour();
	strHour.Format("%d", nHour );

	// nMinute
	CString strMinute; 
	int nMinute = time.GetMinute();
	
	if( nMinute < 10 ) strMinute.Format("0%d", nMinute );
	else			   strMinute.Format("%d",  nMinute );

	// nSecond
	CString strSecond; 
	int nSecond = time.GetSecond();

	if( nSecond < 10 ) strSecond.Format("0%d", nSecond );
	else			   strSecond.Format("%d",  nSecond );

	return strHour + ":" + strMinute + ":" + strSecond;
}

// return value : 2004-06-11
CString CUtilTime::GetLocalDate()
{
	CTime time;
	time = CTime::GetCurrentTime();

	// year
	CString strYear; 
	int nYear  = time.GetYear();
	strYear.Format("%d", nYear );

	// nMonth
	CString strMonth; 
	int nMonth = time.GetMonth();
	
	if( nMonth < 10 ) strMonth.Format("0%d", nMonth );
	else			  strMonth.Format("%d", nMonth );

	// nDay
	CString strDay; 
	int nDay = time.GetDay();

	if( nDay < 10 ) strDay.Format("0%d", nDay );
	else			strDay.Format("%d", nDay );

	return strYear + "-" + strMonth + "-" + strDay;
}

// return value : 20040611
CString CUtilTime::GetLocalNow()
{
	CTime time;
	time = CTime::GetCurrentTime();

	// year
	CString strYear; 
	int nYear  = time.GetYear();
	strYear.Format("%d", nYear );

	// nMonth
	CString strMonth; 
	int nMonth = time.GetMonth();
	strMonth.Format("%02d", nMonth);

	// nDay
	CString strDay; 
	int nDay = time.GetDay();
	strDay.Format("%02d", nDay);

	return strYear + strMonth + strDay;
}

// return 20040720_202411
CString CUtilTime::GetLocalDateTime()
{
	CTime time;
	time = CTime::GetCurrentTime();

	// year
	CString strYear; 
	int nYear  = time.GetYear();
	strYear.Format("%d", nYear );

	// nMonth
	CString strMonth; 
	int nMonth = time.GetMonth();
	strMonth.Format("%02d", nMonth);

	// nDay
	CString strDay; 
	int nDay = time.GetDay();
	strDay.Format("%02d", nDay);

	// hour
	CString strHour; 
	int nHour  = time.GetHour();
	strHour.Format("%02d", nHour);

	// nMinute
	CString strMinute; 
	int nMinute = time.GetMinute();
	strMinute.Format("%02d", nMinute);

	// nSecond
	CString strSecond; 
	int nSecond = time.GetSecond();
	strSecond.Format("%02d", nSecond);

	// 20040720_202411
	return strYear + strMonth + strDay + "_" + strHour + strMinute + strSecond;
}

COleDateTime CUtilTime::GetMaxTime()
{
	return COleDateTime( 2100, 1, 1, 0, 0, 0 );
}

CString CUtilTime::Time2Str( COleDateTime datetime )
{
	CString strTime;
	strTime = datetime.Format(_T("%Y-%m-%d %H:%M:%S"));

	return strTime;
}

CString	CUtilTime::TimeSpan2Str( COleDateTimeSpan datetimeSpan )
{
	CString strTimeSpan;
	strTimeSpan = datetimeSpan.Format(_T("%D %H:%M:%S"));

	return strTimeSpan;
}

// yyyy-mm-dd hh:mi:ss -> COleDateTime
// yyyymmdd_hhmiss     -> COleDateTime
COleDateTime CUtilTime::Str2Time(CString strTime )
{
	COleDateTime oleTime;
	int nYear	= 0;
	int nMonth	= 0;
	int nDay	= 0;
	int nHour	= 0;
	int nMinute	= 0;
	int nSecond	= 0;
    
    // yyyy-mm-dd hh:mi:ss
    if( strTime.GetLength() == 10 || strTime.GetLength() == 19 ) {
	    // Year(  yyyy-mm-dd hh:mi:ss )
	    //	      0123456789012345678     
	    nYear = CUtil2::Str2Int(strTime.Mid( 0, 4 ) );		

	    // Month(  yyyy-mm-dd hh:mi:ss )
	    //	       0123456789012345678     
	    nMonth = CUtil2::Str2Int(strTime.Mid( 5, 2 ) );

	    // Day(    yyyy-mm-dd hh:mi:ss )
	    //	       0123456789012345678     
	    nDay = CUtil2::Str2Int(strTime.Mid(8, 2 ) );		

	    if( 10 < strTime.GetLength() ) {
		    // Hour(   yyyy-mm-dd hh:mi:ss )
	        //	       0123456789012345678     
		    nHour = CUtil2::Str2Int(strTime.Mid(11, 2 ) );		

		    // Minute( yyyy-mm-dd hh:mi:ss )
	        //	       0123456789012345678     
		    nMinute = CUtil2::Str2Int(strTime.Mid( 14, 2 ) );		

		    // Second( yyyy-mm-dd hh:mi:ss )
	        //	       0123456789012345678     
		    nSecond = CUtil2::Str2Int(strTime.Mid( 17, 2 ) );		
	    }
    // yyyymmdd_hhmiss
    } else if( strTime.GetLength() == 8 || strTime.GetLength() == 15 ) {
	    // Year(  yyyymmdd_hhmiss )
	    //	      012345678901234
	    nYear = CUtil2::Str2Int(strTime.Mid( 0, 4 ) );		

	    // Month(  yyyymmdd_hhmiss )
	    //	       012345678901234
	    nMonth = CUtil2::Str2Int(strTime.Mid( 4, 2 ) );

	    // Day(    yyyymmdd_hhmiss )
	    //	       012345678901234
	    nDay = CUtil2::Str2Int(strTime.Mid(6, 2 ) );		

	    if( 8 < strTime.GetLength() ) {
		    // Hour(   yyyymmdd_hhmiss )
	        //	       012345678901234
		    nHour = CUtil2::Str2Int(strTime.Mid(9, 2 ) );		

		    // Minute( yyyymmdd_hhmiss )
	        //	       012345678901234     
		    nMinute = CUtil2::Str2Int(strTime.Mid( 11, 2 ) );		

		    // Second( yyyymmdd_hhmiss )
	        //	       012345678901234     
		    nSecond = CUtil2::Str2Int(strTime.Mid( 13, 2 ) );		
	    }
    }

	oleTime.SetDateTime( nYear, nMonth, nDay, nHour, nMinute, nSecond );	

	return oleTime;
}

// dd hh:mi:ss -> COleDateTimeSpan
COleDateTimeSpan CUtilTime::Str2TimeSpan(CString strTimeSpan )
{
	COleDateTimeSpan oleTimeSpan;
	int nDay	= 0;
	int nHour	= 0;
	int nMinute	= 0;
	int nSecond	= 0;

	// Day( dd hh:mi:ss )
	//	    01234567890     
	nDay = CUtil2::Str2Int(strTimeSpan.Mid(0, 2 ) );		

	// Hour(   dd hh:mi:ss )
    //	       01234567890     
	nHour = CUtil2::Str2Int(strTimeSpan.Mid(3, 2 ) );		

	// Minute( dd hh:mi:ss )
    //	       01234567890     
	nMinute = CUtil2::Str2Int(strTimeSpan.Mid( 6, 2 ) );		

	// Second( dd hh:mi:ss )
    //	       01234567890     
	nSecond = CUtil2::Str2Int(strTimeSpan.Mid( 9, 2 ) );		

	oleTimeSpan.SetDateTimeSpan( nDay, nHour, nMinute, nSecond );	

	return oleTimeSpan;
}

int CUtilTime::Month2Day(int nMonth )
{
	switch( nMonth ) {
		case 1 :	return	31;
		case 2 :	return	28;
		case 3 :	return	31;
		case 4 :	return	30;
		case 5 :	return	31;
		case 6 :	return	30;
		case 7 :	return	31;
		case 8 :	return	31;
		case 9 :	return	30;
		case 10 :	return	31;
		case 11 :	return	30;
		case 12 :	return	31;
	}
	
	return -1;
}
