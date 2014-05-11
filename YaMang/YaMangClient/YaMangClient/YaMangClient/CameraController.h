﻿#pragma once

#include "MacroSet.h"
#include <d3dx9math.h>

const FLOAT NEAREST_Y = 30.0f;
const FLOAT DEGREE_1_Y = 50.0f;
const FLOAT DEGREE_2_Y = 100.0f;
const FLOAT DEGREE_3_Y = 150.0f;
const FLOAT FARTHEST_Y = 250.0f;

const FLOAT NEAREST_VIEW_Y = 0.03f;
const FLOAT DEGREE_1_VIEW_Y = 0.15f;
const FLOAT DEGREE_2_VIEW_Y = 0.45f;
const FLOAT DEGREE_3_VIEW_Y = 0.60f;
const FLOAT FARTHEST_VIEW_Y = 0.9999f;

enum ZoomStatusType
{
	ZOOM_STATUS_NONE,
	ZOOM_STATUS_NEAREST,
	
	ZOOM_STATUS_DEGREE_1,
	ZOOM_STATUS_DEGREE_2,
	ZOOM_STATUS_DEGREE_3,
	
	ZOOM_STATUS_FARTHEST,
	ZOOM_STATUS_MAX
};

enum ZoomDirectionType
{
	ZOOM_DIRECTION_NONE,
	
	ZOOM_DIRECTION_FOWARD,
	ZOOM_DIRECTION_BACK,

	ZOOM_DIRECTION_MAX
};

class CameraController : public Singleton<CameraController>
{
public:
	CameraController();
	~CameraController();

	void	Init();

	void	MoveForward( float speed, bool zoom = false );
	void	MoveSide( float speed );
	void	MoveElevate( float speed );
	void	RotateUp( float angle );
	void	RotateSide( float angle );

	void	Update();

	void	ChangeMouseZoomStatus( short zoom );

private:
	FLOAT			m_Axis = 0;
	FLOAT			m_Height = 0;

	D3DXVECTOR3		m_EyePoint = { 0, 0, 0 };
	D3DXVECTOR3		m_LookAtPoint = { 0, -0.01f, 1.0f };
	D3DXVECTOR3		m_UpVector = { 0, 1, 0 };
	
	D3DXVECTOR3		m_NowZoomPoint = { 0, 0, 0 };
	D3DXVECTOR3		m_PrevZoomPoint = { 0, 0, 0 };

	ZoomStatusType	m_ZoomStatus = ZoomStatusType::ZOOM_STATUS_FARTHEST;
	int				m_ZoomDegree = 0;
	
	ZoomDirectionType	m_ZoomDirection = ZoomDirectionType::ZOOM_DIRECTION_NONE;
};