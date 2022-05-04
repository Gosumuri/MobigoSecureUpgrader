////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Util.h"
#include "UtilFile.h"
#include "UtilApp.h"
#include "UtilTime.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

#define SHORT_TIME          2 // ( 1√ )

////////////////////////////////////////////////////////////////////////////////
// static member
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// constructor / destructor
////////////////////////////////////////////////////////////////////////////////

CUtilApp::CUtilApp(void)
{
}

CUtilApp::~CUtilApp(void)
{
}

////////////////////////////////////////////////////////////////////////////////
// public
////////////////////////////////////////////////////////////////////////////////

BOOL CUtilApp::IsDualRunDuringShortTime( CString strExePath )
{
    CString strPath          = CUtilFile::ExtractPath(               strExePath );
    CString strFileName      = CUtilFile::ExtractFileNameWithoutExt( strExePath );
    CString strTimestampPath = strPath + "\\" + strFileName + "_timestamp.txt";
    if( CUtilFile::IsExistPath( strTimestampPath ) == TRUE ) {
        CString strTimestamp = CUtilFile::ReadString( strTimestampPath );
        strTimestamp = strTimestamp.Trim();
        if( strTimestamp.IsEmpty() == FALSE && strTimestamp.GetLength() == 15 /** 15 is size of "20040720_202411"*/ ) {
            COleDateTime     curTimestamp  = COleDateTime::GetCurrentTime();
            COleDateTime     lastTimestamp = CUtilTime::Str2Time( strTimestamp );
            COleDateTimeSpan span          = CUtilTime::DiffTime( curTimestamp, lastTimestamp );

            int nElapseTime = span.GetSeconds();

            if( nElapseTime < SHORT_TIME ) {
                CUtilFile::WriteString( strTimestampPath, CUtilTime::GetLocalDateTime() );
                return TRUE;
            }
        }
    }
    CUtilFile::WriteString( strTimestampPath, CUtilTime::GetLocalDateTime() );
	return FALSE;
}
