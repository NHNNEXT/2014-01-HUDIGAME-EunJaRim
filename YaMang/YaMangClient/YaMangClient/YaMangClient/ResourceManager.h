﻿#pragma once
#include "MacroSet.h"
#include "EnumSet.h"

struct MESHOBJECT;

struct Resource
{
	Resource(): m_Type( RESOURCE_NONE ) {}
	ResourceType m_Type;
};

struct ResourceMesh: public Resource
{
	ResourceMesh()
	{
		m_Type = RESOURCE_MESH;
	}
	MESHOBJECT*	m_MeshObject = nullptr;
};

struct ResourceMap: public Resource
{
// 	LPCTSTR	m_HeightMap = nullptr;
// 	LPCTSTR m_TextureMap = nullptr;
	std::wstring	m_HeightMap;
	std::wstring	m_TextureMap;
};

class ResourceManager: public Singleton<ResourceManager>
{
public:
	ResourceManager();
	~ResourceManager();

	void	Init();
	void	Destroy();
	bool	IsMapReady() { return m_IsMapReady; }
	
	ResourceMesh*	GetMeshByKey( MeshKeyType key ) { return m_MeshArray[key]; }

private:
	void	AddMap( LPCTSTR& heightMapFileName, LPCTSTR& textureFileName, MapKeyType key );
	bool	CreateMap( MapKeyType key );
	void	DeleteMap();

	bool	AddMesh( LPCTSTR& fileName, MeshKeyType key );
	void	CreateMesh( LPCTSTR& fileName, MESHOBJECT* mesh );
	void	DeleteMesh( MESHOBJECT* mesh );

	std::array<ResourceMesh*, MESH_KEY_MAX>	m_MeshArray;
	std::array<ResourceMap*, MAP_KEY_MAX> m_HeightMapArray;
	
	bool	m_IsMapReady = false;
};