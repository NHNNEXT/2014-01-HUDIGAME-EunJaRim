#pragma once
#include "SharedDefine.h"

struct Tile
{
	unsigned char R; // �� �������� R�� ���� � �Ӽ����� ������ �������� ���� �����ʰ� ���� �ʿ�
	unsigned char G;
	unsigned char B; // �Ƹ��� height���� �� ����
};

class GameMapManager
{
public:
	GameMapManager();
	~GameMapManager();


	bool		Initialize();
	bool		ReadMapFile( const char* filename );

	const std::vector<PositionInfo>&	GetKingPositionList() { return m_KingPositionList; }
	const std::vector<PositionInfo>&	GetGuardPositionList() { return m_GuardPositionList; }
	const PositionInfo&					GetGuardPositionInfo( int index ) const { return m_GuardPositionList.at( index ); }

private:
	std::vector<std::vector<Tile>>	m_BattleMap;

	std::vector<PositionInfo> m_KingPositionList;
	std::vector<PositionInfo> m_GuardPositionList;
};

