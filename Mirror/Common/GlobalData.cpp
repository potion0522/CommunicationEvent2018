#include "GlobalData.h"
#include "UpdateKey.h"
#include "MouseEvent.h"
#include "Image.h"
#include "Debug.h"

GlobalData::GlobalData( ) {
	initialize( );
}

GlobalData::~GlobalData( ) {
}

std::string GlobalData::getTag( ) {
	return "GLOBALDATA";
}

void GlobalData::initialize( ) {
	setFlag( 1 );
	_scene = NONE;

	_key =   UpdateKeyPtr( new UpdateKey( ) );
	_mouse = MouseEventPtr( new MouseEvent( ) );
	_image = ImagePtr( new Image( ) );
	_debug = NULL;
}

void GlobalData::update( ) {
	if ( _debug == NULL ) {
		DebugPtr debug( new Debug( NULL ) );
		debug->error( "GlobalData.cpp->update : クラスポインタがNULLのものがあります。" );
	}

	_key->update( );
	_mouse->update( );
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

void GlobalData::setPtr( DebugPtr ptr ) {
	_debug = ptr;
}

void GlobalData::setPtr( ServerPtr ptr ) {
	_server = ptr;
}

void GlobalData::setPtr( ClientPtr ptr ) {
	_client = ptr;
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