#include "GlobalData.h"

GlobalData::GlobalData( ) {
	initialize( );
}

GlobalData::~GlobalData( ) {
}

void GlobalData::initialize( ) {
	setFlag( 0 );
	_key = std::shared_ptr< UpdateKey >( new UpdateKey( ) );
	_mouse = std::shared_ptr< MouseEvent >( new MouseEvent( ) );
}

void GlobalData::update( ) {
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