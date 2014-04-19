﻿#include "stdafx.h"
#include "Guard.h"


Guard::Guard()
{
	m_MeshKey = MESH_KEY_UNIT_GUARD;
	// 배트맨

	m_EyePoint = { 4.0f, -1.2f, -20.0f };
	m_LookAtPoint = { 4.3f, -1.2f, -21.0f };
	m_UpVector = { 0.0f, 1.0f, 0.0f };
	m_Scale = { 2.0f, 2.0f, 2.0f };
}


Guard::~Guard()
{
}
