#pragma once

enum class UnitType: int
{
	UNIT_NONE = 0,

	UNIT_ARROW = 10,
	UNIT_GUARD = 20,
	UNIT_KNIGHT = 30,
	UNIT_PIKE = 40,
	UNIT_SWORD = 50,

	UNIT_MAX = 255
};

enum class FormationType: int
{
	FORMATION_NONE = 0,

	FORMATION_DEFENSE = 10,
	FORMATION_DESTROY = 20,
	FORMATION_RUSH = 30,

	FORMATION_MAX = 255
};

enum SceneType
{
	SCENE_NONE,

	SCENE_LOAD,
	SCENE_PLAY,
	SCENE_RESULT,

	SCENE_MAX
};

enum MeshKeyType
{
	MESH_KEY_NONE,
	
	MESH_KEY_UNIT_SWORD,
	MESH_KEY_UNIT_PIKE,
	MESH_KEY_UNIT_ARROW,
	MESH_KEY_UNIT_KNIGHT,
	MESH_KEY_UNIT_GUARD,

	MESH_KEY_MAX
};

enum MapKeyType
{
	MAP_KEY_NONE,

	MAP_KEY_KOREA,

	MAP_KEY_MAX
};

enum ResourceType
{
	RESOURCE_NONE,

	RESOURCE_MESH,
	RESOURCE_SOUNDE,

	RESOURCE_MAX
};

enum ActionStatusType
{
	ACTION_STATUS_NONE,

	ACTION_STATUS_ROTATE,
	ACTION_STATUS_GOFOWARD,
	ACTION_STATUS_ATTACK,
	
	ACTION_STATUS_RUSH,
	ACTION_STATUS_KNOCKBACK,

	ACTION_STATUS_MAX
};