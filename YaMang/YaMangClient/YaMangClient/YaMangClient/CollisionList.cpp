#include "stdafx.h"
#include "CollisionList.h"
#include "Collision.h"

CollisionList::CollisionList()
{
}


CollisionList::~CollisionList()
{
}

void CollisionList::AddCollision( Collision* collision )
{
	if ( collision )
	{
		m_CollisionList.push_back( collision );
	}
}

void CollisionList::RemoveColiision( Collision* collision )
{
	if ( collision )
	{
		for ( auto iter = m_CollisionList.begin(); iter != m_CollisionList.end(); )
		{
			if ( collision == *iter )
			{
				iter = m_CollisionList.erase( iter );
			}
			else
			{
				++iter;
			}
		}
	}
}

void CollisionList::CheckCollision( Collision* collision ) const
{
	for ( auto iter : m_CollisionList )
	{
		if ( collision == iter )
		{
			continue;
		}

		if ( collision->IsCollideWith( *iter ) )
		{
			collision->SetCollide();
			break;
		}
	}
}
