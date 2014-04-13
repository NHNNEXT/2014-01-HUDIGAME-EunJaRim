#pragma  once

#pragma warning(disable: 4316)
#include <cassert>

template <class T>
class Singleton
{
protected:
	Singleton( void ) {}
	virtual ~Singleton( void ) {}
public:
	static T* GetInstance( void )
	{
		if ( !m_Instance )
		{
			m_Instance = new T;
		}
		return m_Instance;
	}
	
	static void Release( void )
	{
		SafeDelete( m_Instance );
	}

private:
	static T*	m_Instance;
};
template <typename T>
T* Singleton<T>::m_Instance = nullptr;


template <typename T>
inline void SafeRelease( T* &p )
{
	if ( p != nullptr )
	{
		p->Release();
		p = nullptr;
	}
}

template <typename T>
inline void SafeDelete( T* &p )
{
	if ( p != nullptr )
	{
		delete p;
		p = nullptr;
	}
}

template <typename ... Types >
inline void Log( const Types& ...args )
{
#ifdef _DEBUG
	printf_s( args... );
#endif // _DEBUG
}