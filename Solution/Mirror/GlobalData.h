#pragma once
#include "Base.h"
#include "UpdateKey.h"
#include "MouseEvent.h"
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

private:
	std::shared_ptr< UpdateKey > _key;
	std::shared_ptr< MouseEvent > _mouse;
};

