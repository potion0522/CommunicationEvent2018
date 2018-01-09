#include "GlobalData.h"
#include "UpdateKey.h"
#include "MouseEvent.h"
#include "Image.h"
#include "Debug.h"
#include "Server.h"
#include "Client.h"
#include "Drawer.h"

GlobalData::GlobalData( ) {
}

GlobalData::~GlobalData( ) {
}

std::string GlobalData::getTag( ) {
	return "GLOBALDATA";
}

void GlobalData::initialize( MACHINE_TYPE type ) {
	setFlag( 1 );
	_scene = TITLE;
	_command_flag = false;

	_key =   UpdateKeyPtr( new UpdateKey( ) );
	_mouse = MouseEventPtr( new MouseEvent( ) );
	_image = ImagePtr( new Image( ) );
	_drawer = DrawerPtr( new Drawer( ) );
	_debug = NULL;

	switch ( type ) {
	case SERVER	: _server = ServerPtr( new Server( ) )		; break;
	case CLIENT	: _client = ClientPtr( new Client( ) )		; break;
	case TEST	: _debug  = DebugPtr ( new Debug( NULL ) )	; break;
	}
}

void GlobalData::update( ) {
	if ( _debug == NULL ) {
		DebugPtr debug( new Debug( NULL ) );
		debug->error( "GlobalData.cpp->update : クラスポインタがNULLのものがあります。" );
	}

	_key->update( );
	_mouse->update( );
}

void GlobalData::setScene( SCENE scene ) {
	_scene = scene;
}

int GlobalData::getKeyState( int key ) const {
	return _key->getKeyState( key );
}

int GlobalData::getMouseX( ) const {
	return _mouse->getPointX( );
}

int GlobalData::getMouseY( ) const {
	return _mouse->getPointY( );
}

bool GlobalData::getClickLeft( ) const {
	return _mouse->getLeftClick( );
}

SCENE GlobalData::getScene( ) const {
	return _scene;
}

bool GlobalData::getCommandFlag( ) const {
	return _command_flag;
}

void GlobalData::setCommandFlag( bool flag ) {
	_command_flag = flag;
}

void GlobalData::setPtr( DebugPtr ptr ) {
	_debug = ptr;
}

void GlobalData::setPtr( ServerPtr ptr ) {
	_server = ptr;
}

void GlobalData::setPtr( ClientPtr ptr ) {
	_client = ptr;
}

void GlobalData::setPtr( FieldPtr ptr ) {
	_field = ptr;
}

ImagePtr GlobalData::getImagePtr( ) const {
	return _image;
}

DebugPtr GlobalData::getDebugPtr( ) const {
	return _debug;
}

ServerPtr GlobalData::getServerPtr( ) const {
	return _server;
}

ClientPtr GlobalData::getClientPtr( ) const {
	return _client;
}

DrawerPtr GlobalData::getDrawerPtr( ) const {
	return _drawer;
}

FieldPtr GlobalData::getFieldPtr( ) const {
	return _field;
}