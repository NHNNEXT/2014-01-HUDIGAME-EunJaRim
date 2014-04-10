﻿#include "stdafx.h"
#include "Exception.h"
#include <DbgHelp.h>


LONG WINAPI ExceptionFilter( EXCEPTION_POINTERS* exceptionInfo )
{
	// 디버깅 되고 있다면
	// 예외 처리 (except) 를 상위 구문으로 찾도록 요청 신호 반환
	if ( IsDebuggerPresent() )
	{
		return EXCEPTION_CONTINUE_SEARCH;
	}

	std::string fileName = "YaMangServer_minidump-" __DATE__ "-" __TIME__".dmp";
	std::string::size_type offset = 0;
	for ( int i = 0; i < fileName.length(); ++i )
	{
		offset = fileName.find( " ", offset );

		if ( std::string::npos == offset )
		{
			break;
		}
		else
		{
			fileName.replace( offset, 1, "_" );
		}
	}

	offset = 0;
	for ( int i = 0; i < fileName.length(); ++i )
	{
		offset = fileName.find( ":", offset );

		if ( std::string::npos == offset )
		{
			break;
		}
		else
		{
			fileName.replace( offset, 1, "_" );
		}
	}

	/// dump file 남기자.
	// 디버깅 중이 아닐 때 생긴 Crash에 대해서 dmp 파일을 남기는 부분
	HANDLE hFile = CreateFileA( fileName.c_str(), GENERIC_READ | GENERIC_WRITE,
								0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	if ( ( hFile != NULL ) && ( hFile != INVALID_HANDLE_VALUE ) )
	{
		MINIDUMP_EXCEPTION_INFORMATION mdei;

		mdei.ThreadId = GetCurrentThreadId();
		mdei.ExceptionPointers = exceptionInfo;
		mdei.ClientPointers = FALSE;


		MINIDUMP_TYPE mdt = (MINIDUMP_TYPE)( MiniDumpWithPrivateReadWriteMemory |
											 MiniDumpWithDataSegs | MiniDumpWithHandleData | MiniDumpWithFullMemoryInfo |
											 MiniDumpWithThreadInfo | MiniDumpWithUnloadedModules );

		MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(),
						   hFile, mdt, ( exceptionInfo != 0 ) ? &mdei : 0, 0, NULL );

		CloseHandle( hFile );

	}
	else
	{
		printf( "CreateFile failed. Error: %u \n", GetLastError() );
	}

	// 예외처리 종료 신호 반환
	return EXCEPTION_EXECUTE_HANDLER;

}