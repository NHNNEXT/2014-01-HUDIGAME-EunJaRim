﻿#include "stdafx.h"
#include "ClientSession.h"
#include "PacketType.h"
#include "MacroSet.h"


// EasyServer.cpp 에서 클라이언트 매니저에서 CreateClient 한 후에
// 소켓 객체로부터 getpeername()를 이용해 주소 값을 뽑아 와서 OnConnect() 호출
// EasyServer.cpp의 ClientHandlingThread()에서 호출 한,
// 클라이언트 매니저의 CreateClient() 함수에서 new 해서 동적 할당 한,
// 접속자(클라이언트) 하나 당 할당 된 객체 하나이다
// 
// 해당 객체에, 위의 인자로부터(EasyServer.cpp의 클라이언트 핸들링 스레드에서)
// 주소 값을 받아와서 멤버 변수로 저장한다.

bool ClientSession::OnConnect( SOCKADDR_IN* addr )
{
	memcpy( &m_ClientAddr, addr, sizeof( SOCKADDR_IN ) );

	/// 소켓을 넌블러킹으로 바꾸고
	u_long arg = 1;
	ioctlsocket( m_Socket, FIONBIO, &arg );

	/// nagle 알고리즘 끄기
	int opt = 1;
	setsockopt( m_Socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt, sizeof( int ) );

	printf( "[DEBUG] Client Connected: IP=%s, PORT=%d\n", inet_ntoa( m_ClientAddr.sin_addr ), ntohs( m_ClientAddr.sin_port ) );

	m_Connected = true;

	return PostRecv();
}

bool ClientSession::PostRecv()
{
	if ( !IsConnected() )
	{
		return false;
	}

	DWORD recvbytes = 0;
	DWORD flags = 0;
	WSABUF buf;
	buf.len = (ULONG)m_RecvBuffer.GetFreeSpaceSize();
	buf.buf = (char*)m_RecvBuffer.GetBuffer();

	memset( &m_OverlappedRecv, 0, sizeof( OverlappedIO ) );

	// 비동기 입력 호출이 완료 되었을 때 콜백 함수가 실행 되면
	// 어느 호출이 부른 것인지를 식별하기 위해
	// ClientSession* 를 담고 있는 멤버변수 mObject에 this (현재 객체) 주소 대입
	m_OverlappedRecv.m_Object = this;

	/// 비동기 입출력 시작
	if ( SOCKET_ERROR == WSARecv( m_Socket, &buf, 1, &recvbytes, &flags, &m_OverlappedRecv, RecvCompletion ) )
	{
		if ( WSAGetLastError() != WSA_IO_PENDING )
		{
			return false;
		}
	}

	// Overlapped IO 요청 했음. 카운트 증가
	IncOverlappedRequest();

	return true;
}


// 비동기 입력 완료 후 RecvCompletion 콜백 발생하면
// 받은 데이터 사이즈를 인자로 넘겨서 OnRead 실행
void ClientSession::OnRead( size_t len )
{
	// 버퍼 쓰기 영역 크기 증가
	m_RecvBuffer.Commit( len );

	/// 패킷 파싱하고 처리
	while ( true )
	{
		/// 패킷 헤더 크기 만큼 읽어와보기
		PacketHeader header;
		if ( false == m_RecvBuffer.Peek( (char*)&header, sizeof( PacketHeader ) ) )
		{
			return;
		}

		/// 패킷 완성이 되는가? 
		if ( m_RecvBuffer.GetStoredSize() < header.m_Size )
		{
			return;
		}

		// 헤더에 담겨 있는 mSize는 패킷 전체 사이즈
		// 패킷 사이즈 = 패킷 헤더 + 페이로드(payload = 실제 데이터)
		if ( header.m_Type >= PKT_MAX || header.m_Type <= PKT_NONE )
		{
			Log( "[Disconnected from:]ClientSession::OnRead( size_t len ) \n" );
			Disconnect();
			return;
		}

		// 이 위의 상단들에서 return 하게 되면 하단의 RecvCompletion 에서
		// 다시 PostRecv() 함수를 호출하게 되어 통신이 재개 되어 다시 받아오고
		// 결국 요건을 충족 시키게 되면 아래로 진입 하게 되어 패킷 처리 하게 됨
		/// packet dispatch...
		HandlerTable[header.m_Type]( this, header );
	}
}

bool ClientSession::SendRequest( PacketHeader* pkt )
{
	if ( !IsConnected() )
	{
		return false;
	}

	/// Send 요청은 버퍼에 쌓아놨다가 한번에 보낸다.
	if ( false == m_SendBuffer.Write( (char*)pkt, pkt->m_Size ) )
	{
		/// 버퍼 용량 부족인 경우는 끊어버림
		Log( "[Disconnected from:]ClientSession::SendRequest \n" );
		Disconnect();
		return false;
	}

	return true;

}

bool ClientSession::SendFlush()
{
	if ( !IsConnected() )
	{
		return false;
	}

	/// 보낼 데이터가 없으면 그냥 리턴
	if ( m_SendBuffer.GetContiguiousBytes() == 0 )
	{
		return true;
	}

	DWORD sendbytes = 0;
	DWORD flags = 0;

	WSABUF buf;
	buf.len = (ULONG)m_SendBuffer.GetContiguiousBytes();
	buf.buf = (char*)m_SendBuffer.GetBufferStart();

	memset( &m_OverlappedSend, 0, sizeof( OverlappedIO ) );
	m_OverlappedSend.m_Object = this;

	// 비동기 입출력 시작
	if ( SOCKET_ERROR == WSASend( m_Socket, &buf, 1, &sendbytes, flags, &m_OverlappedSend, SendCompletion ) )
	{
		if ( WSAGetLastError() != WSA_IO_PENDING )
		{
			return false;
		}
	}

	// Overlapped IO 요청 했음. 카운트 증가
	IncOverlappedRequest();

	//assert(buf.len == sendbytes);

	return true;
}

void ClientSession::OnWriteComplete( size_t len )
{
	/// 보내기 완료한 데이터는 버퍼에서 제거
	m_SendBuffer.Remove( len );
}



///////////////////////////////////////////////////////////
// 비동기 입력 WSARecv()에 의해서, 입력이 완료 되면 콜백으로 RecvCompletion 실행
void CALLBACK RecvCompletion( DWORD dwError, DWORD cbTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags )
{

	// lpOverlapped 인자를 OverlappedIO 형태로 형변환 하면
	// 해당 구조체의 멤버 변수 mObject => ClientSession*
	// 바로 이 포인터가 비동기 입력 WSARecv로 보낸 ClientSession 객체의 주소값
	// PostRecv 멤소드에서 mOverlappedRecv.mObject = this ; 이 부분 참조
	ClientSession* fromClient = static_cast<OverlappedIO*>( lpOverlapped )->m_Object;

	// Overlapped IO 완료 했음. 카운트 감소
	fromClient->DecOverlappedRequest();

	if ( !fromClient->IsConnected() )
		return;

	/// 에러 발생시 해당 세션 종료
	if ( dwError || cbTransferred == 0 )
	{
		Log( "[Disconnected from:]ClientSession::RecvCompletion dwError \n" );
		fromClient->Disconnect();
		return;
	}

	/// 받은 데이터 처리
	fromClient->OnRead( cbTransferred );

	/// 다시 받기
	if ( false == fromClient->PostRecv() )
	{
		Log( "[Disconnected from:]ClientSession::RecvCompletion PostRecv \n" );
		fromClient->Disconnect();
		return;
	}
}

// 비동기 출력 WSASend()에 의해서 출력이 완료 되면 콜백으로 SendCompletion 실행
void CALLBACK SendCompletion( DWORD dwError, DWORD cbTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags )
{
	ClientSession* fromClient = static_cast<OverlappedIO*>( lpOverlapped )->m_Object;

	// Overlapped IO 완료 했음. 카운트 감소
	fromClient->DecOverlappedRequest();

	if ( !fromClient->IsConnected() )
	{
		return;
	}

	/// 에러 발생시 해당 세션 종료
	if ( dwError || cbTransferred == 0 )
	{
		Log( "[Disconnected from:]ClientSession::SendCompletion dwError \n" );
		fromClient->Disconnect();
		return;
	}

	/// 보내기 완료한 데이터를 버퍼에서 제거
	fromClient->OnWriteComplete( cbTransferred );

}