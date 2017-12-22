#pragma once
#include "Base.h"
#include "smart_ptr.h"
#include "const.h"

PTR( GlobalData );
PTR( UpdateKey );
PTR( MouseEvent );
PTR( Image );
PTR( Debug );
PTR( Server );
PTR( Client );

class GlobalData : public Base {
public:
	GlobalData( );
	virtual ~GlobalData( );

public:
	std::string getTag( );
	void initialize( MACHINE_TYPE type );
	void update( );

public:
	int getKeyState( int key ) const;
	int getMouseX( ) const;
	int getMouseY( ) const;
	bool getClickLeft( ) const;
	SCENE getScene( ) const;

public:
	void setPtr( DebugPtr ptr );
	void setPtr( ServerPtr ptr );
	void setPtr( ClientPtr ptr );

	ImagePtr getImagePtr( ) const;
	DebugPtr getDebugPtr( ) const;
	ServerPtr getServerPtr( ) const;
	ClientPtr getClientPtr( ) const;

private:
	SCENE _scene;

	UpdateKeyPtr _key;
	MouseEventPtr _mouse;
	ImagePtr _image;
	DebugPtr _debug;
	ServerPtr _server;
	ClientPtr _client;
};

