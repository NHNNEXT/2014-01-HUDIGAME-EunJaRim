﻿#pragma once
#include <map>
#include "MacroSet.h"

__interface EventHandler;
struct PacketHeader;
class CircularBuffer;

class HandlerMap : public Singleton<HandlerMap>
{
public:
	HandlerMap();
	~HandlerMap();

	bool	HandleEvent( short* packetType, PacketHeader* header, CircularBuffer* buffer );

private:
	std::map<short, EventHandler*> mPacketHandler;

};

