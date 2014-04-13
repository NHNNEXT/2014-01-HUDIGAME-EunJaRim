#pragma once

class ClientSession;
class ClientManager;

class RoomManager
{
public:
	RoomManager();
	~RoomManager();

	void	AddRoom(); // �� ����
	void	DeleteRoom(); // �� ����
	void	ChangeRoom(); // Ŭ���̾�Ʈ�� �ٸ� ������ �̵� (Ŭ�� �Ŵ��� �ƴ�)

	ClientSession* CreateClient( SOCKET sock );
	void FlushClientSend( );
	void OnPeriodWork( );

private:
	typedef std::map<int, ClientManager*> RoomList;
	RoomList	m_RoomList;
};

extern RoomManager* g_RoomManager;

