﻿#pragma once
#include "Corps.h"

class Sword:
	public Corps
{
public:
	Sword( int playerID, int corpsID, Position position ):Corps( playerID, corpsID, position ) {};
	virtual ~Sword( );

private:
	Sword( );
	Sword( const Sword& );
	Sword& operator=( const Sword& );

};

