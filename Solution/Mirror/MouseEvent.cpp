#include "MouseEvent.h"
#include "DxLib.h"

MouseEvent::MouseEvent( ) {
	initialize( );
}

MouseEvent::~MouseEvent( ) {
}

void MouseEvent::initialize( ) {
	_mouse = NULL;
	_click_left = 0;
	memset( &_point, 0, sizeof( Point ) );
}

void MouseEvent::update( ) {
	_mouse = GetMouseInput( );
	calcLeftClick( );
	GetMousePoint( &_point.x, &_point.y );
}

void MouseEvent::calcLeftClick( ) {
	if ( _mouse & MOUSE_INPUT_LEFT ) {
		_click_left++;
		return;
	}
	_click_left = 0;
}

int MouseEvent::getPointX( ) const {
	return _point.x;
}

int MouseEvent::getPointY( ) const {
	return _point.y;
}

bool MouseEvent::getLeftClick( ) const {
	if ( _click_left == 1 ) {
		return true;
	}
	return false;
}