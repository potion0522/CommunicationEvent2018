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
PTR( Drawer );
PTR( Field );

class GlobalData : public Base {
public:
	GlobalData( );
	virtual ~GlobalData( );

public:
	std::string getTag( );
	void initialize( MACHINE_TYPE type );
	void update( );

public:
	void setScene( SCENE scene );
	int getKeyState( int key ) const;
	int getMouseX( ) const;
	int getMouseY( ) const;
	bool getClickLeft( ) const;
	SCENE getScene( ) const;
	bool getCommandFlag( ) const;

public:
	void setCommandFlag( bool flag );
	void setPtr( DebugPtr ptr );
	void setPtr( ServerPtr ptr );
	void setPtr( ClientPtr ptr );
	void setPtr( FieldPtr ptr );

	ImagePtr getImagePtr( ) const;
	DebugPtr getDebugPtr( ) const;
	ServerPtr getServerPtr( ) const;
	ClientPtr getClientPtr( ) const;
	DrawerPtr getDrawerPtr( ) const;
	FieldPtr getFieldPtr( ) const;

private:
	SCENE _scene;
	bool _command_flag;

	UpdateKeyPtr _key;
	MouseEventPtr _mouse;
	ImagePtr _image;
	DebugPtr _debug;
	ServerPtr _server;
	ClientPtr _client;
	DrawerPtr _drawer;
	FieldPtr _field;
};

