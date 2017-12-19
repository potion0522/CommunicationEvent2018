#include "GlobalData.h"
#include "UpdateKey.h"
#include "MouseEvent.h"
#include "Image.h"
#include "Debug.h"
#include "Title.h"

GlobalData::GlobalData( ) {
	initialize( );
}

GlobalData::~GlobalData( ) {
}

void GlobalData::initialize( ) {
	setFlag( 1 );
	_scene = NONE;

	_key = std::shared_ptr< UpdateKey >( new UpdateKey( ) );
	_mouse = std::shared_ptr< MouseEvent >( new MouseEvent( ) );
	_image = std::shared_ptr< Image >( new Image( ) );
	_debug = NULL;
	_title = NULL;
}

void GlobalData::update( ) {
	if ( _debug == NULL ||
		 _title == NULL
		) {
		std::shared_ptr< Debug > debug( new Debug( NULL ) );
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

void GlobalData::setPtr( std::shared_ptr< Debug > ptr ) {
	_debug = ptr;
}

void GlobalData::setPtr( std::shared_ptr< Title > ptr ) {
	_title = ptr;
}

std::shared_ptr< Image > GlobalData::getImagePtr( ) const {
	return _image;
}

std::shared_ptr< Debug > GlobalData::getDebugPtr( ) const {
	return _debug;
}

std::shared_ptr< Title > GlobalData::getTitlePtr( ) const {
	return _title;
}