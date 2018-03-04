#pragma once
#include "Base.h"
#include "smart_ptr.h"
#include "const.h"

PTR( GlobalData );
PTR( UpdateKey );
PTR( MouseEvent );
PTR( Image );
PTR( Sound );
PTR( Debug );
PTR( Server );
PTR( Client );
PTR( Drawer );
PTR( Soundplayer );
PTR( Field );

class GlobalData : public Base {
public:
	GlobalData( MACHINE_TYPE type );
	virtual ~GlobalData( );

public:
	std::string getTag( );
	void initialize( );
	void update( );

public:
	void setScene( SCENE scene );
	int getKeyState( int key ) const;
	int getMouseX( ) const;
	int getMouseY( ) const;
	bool getClickLeft( ) const;
	bool getClickRight( ) const;
	int getClickingLeft( ) const;
	int getClickingRight( ) const;
	int getRandom( int min, int max ) const;
	int getCount( ) const;
	SCENE getScene( ) const;
	MACHINE_TYPE getMachineType( ) const;
	bool getCommandFlag( ) const;
	bool getInitFlag( ) const;

public:
	void setCommandFlag( bool flag );
	void setInitFlag( );
	void foldInitFlag( );
	void setPtr( DebugPtr ptr );
	void setPtr( ServerPtr ptr );
	void setPtr( ClientPtr ptr );
	void setPtr( FieldPtr ptr );

public:
	ImagePtr getImagePtr( ) const;
	SoundPtr getSoundPtr( ) const;
	DebugPtr getDebugPtr( ) const;
	ServerPtr getServerPtr( ) const;
	ClientPtr getClientPtr( ) const;
	DrawerPtr getDrawerPtr( ) const;
	SoundplayerPtr getSoundplayerPtr( ) const;
	FieldPtr getFieldPtr( ) const;

private:
	SCENE _scene;
	MACHINE_TYPE _type;
	bool _command_flag;
	bool _init;
	int _count;

	UpdateKeyPtr _key;
	MouseEventPtr _mouse;
	ImagePtr _image;
	SoundPtr _sound;
	DebugPtr _debug;
	ServerPtr _server;
	ClientPtr _client;
	DrawerPtr _drawer;
	SoundplayerPtr _soundplayer;
	FieldPtr _field;
};

