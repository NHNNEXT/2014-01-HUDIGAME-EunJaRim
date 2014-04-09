﻿#pragma once
#include "MacroSet.h"
#include "CircularBuffer.h"

class NetworkManager:public Singleton<NetworkManager>
{
public:
	NetworkManager();
	~NetworkManager();

	bool	Init();
	void	Destroy();
	void	ProcessPacket();
	bool	Connect();
	

private:
	SOCKET			m_Socket;
	CircularBuffer	m_RecvBuffer;
	CircularBuffer	m_SendBuffer;
};