#pragma once
#include "Base.h"
#include "UpdateKey.h"
#include "MouseEvent.h"
#include "Debug.h"
#include <memory>

class GlobalData : public Base {
public:
	GlobalData( );
	virtual ~GlobalData( );

public:
	void initialize( );
	void update( );

public:
	int getKeyState( int key ) const;
	int getMouseX( ) const;
	int getMouseY( ) const;
	bool getClickLeft( ) const;
	SCENE getScene( ) const;

public:
	void setPtr( std::shared_ptr< class Debug > ptr );

	std::shared_ptr< Debug > getDebugPtr( ) const;

private:
	SCENE _scene;

	std::shared_ptr< class UpdateKey > _key;
	std::shared_ptr< class MouseEvent > _mouse;
	std::shared_ptr< class Debug > _debug;
};

