#include "stdafx.h"
#include "GameMapManager.h"
#include "MacroSet.h"
#include "tinyxml.h"
#include "xpath_static.h"

GameMapManager::GameMapManager()
{
}


GameMapManager::~GameMapManager()
{
	for ( int i = 0; i < m_BattleMap.size(); ++i )
	{
		m_BattleMap.at( i ).clear();
	}
	m_BattleMap.clear();
}



bool GameMapManager::Initialize()
{
	TiXmlDocument document = TiXmlDocument( "../../SharedPreference/ServerConfig.xml" );
	bool xmlLoadSuccess = document.LoadFile();
	
	if ( xmlLoadSuccess )
	{
		std::string mapFilePath = TinyXPath::S_xpath_string( document.RootElement(), "/server/mapFilePath/text()" ).c_str();
		Log( "Map Path Loaded! :%s \n", mapFilePath.c_str() );
		if ( ReadMapFile( mapFilePath.c_str() ) )
		{
			return true;
		}
	}
	return false;
}


bool GameMapManager::ReadMapFile( const char* filename )
{
	FILE* f = NULL;
	fopen_s( &f, filename, "rb" );

	if ( f == NULL )
	{
		return false;
	}

	unsigned char info[54];
	fread( info, sizeof( unsigned char ), 54, f ); // read the 54-byte header

	// extract image height and width from header
	int width = *(int*)&info[18];
	int height = *(int*)&info[22];

	m_BattleMap.reserve( height );

	int row_padded = ( width * 3 + 3 ) & ( ~3 );
	unsigned char* data = new unsigned char[row_padded];


	unsigned char tmp;
	for ( int i = 0; i < height; i++ )
	{
		std::vector<Tile> row;
		row.reserve( width );
		fread( data, sizeof( unsigned char ), row_padded, f );
		for ( int j = 0; j < width * 3; j += 3 )
		{
			// Convert (B, G, R) to (R, G, B)
			tmp = data[j];
			data[j] = data[j + 2];
			data[j + 2] = tmp;

			//Tile tile = { (int)data[j], (int)data[j + 1], (int)data[j + 2] };
			Tile tile = { data[j], data[j + 1], data[j + 2] };

			// ���� �������� ���� �� ����!!
			row.push_back( tile );
		}
		m_BattleMap.push_back( row );
	}
	Log( "[%d][%d] Map Loaded! \n", m_BattleMap.size(), m_BattleMap.at( 0 ).size() );

	fclose( f );

	return true;
}