﻿#pragma once

enum class KeyStatus
{
	KEY_NONE,
	KEY_DOWN,
	KEY_PRESSED,
	KEY_UP,
};

const unsigned char VK_A = 0x41;
const unsigned char VK_B = 0x42;
const unsigned char VK_C = 0x43;
const unsigned char VK_D = 0x44;
const unsigned char VK_E = 0x45;
const unsigned char VK_F = 0x46;
const unsigned char VK_G = 0x47;
const unsigned char VK_H = 0x48;
const unsigned char VK_I = 0x49;
const unsigned char VK_J = 0x4A;
const unsigned char VK_K = 0x4B;
const unsigned char VK_L = 0x4C;
const unsigned char VK_M = 0x4D;
const unsigned char VK_N = 0x4E;
const unsigned char VK_O = 0x4F;
const unsigned char VK_P = 0x50;
const unsigned char VK_Q = 0x51;
const unsigned char VK_R = 0x52;
const unsigned char VK_S = 0x53;
const unsigned char VK_T = 0x54;
const unsigned char VK_U = 0x55;
const unsigned char VK_V = 0x56;
const unsigned char VK_W = 0x57;
const unsigned char VK_X = 0x58;
const unsigned char VK_Y = 0x59;
const unsigned char VK_Z = 0x5A;

const int MAX_KEY = 256;

class KeyInput
{
public:
	KeyInput();
	~KeyInput();

	unsigned char	GetKeyValue() const { return m_KeyValue; }
	void			SetKeyValue( unsigned char key ) { m_KeyValue = key; }
	KeyStatus		GetKeyStatus() const { return m_KeyStatus; }
	void			SetKeyStatus( KeyStatus keyStatus ) { m_KeyStatus = keyStatus; }

private:
	unsigned char	m_KeyValue = 0;
	KeyStatus		m_KeyStatus = KeyStatus::KEY_NONE;
};
