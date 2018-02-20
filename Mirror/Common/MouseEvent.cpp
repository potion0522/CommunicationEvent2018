#include "MouseEvent.h"
#include "DxLib.h"
#include <limits.h>

MouseEvent::MouseEvent( ) {
	initialize( );
}

MouseEvent::~MouseEvent( ) {
}

void MouseEvent::initialize( ) {
	_mouse = NULL;
	_click_left = 0;
	_click_right = 0;
	memset( &_point, 0, sizeof( Point ) );
}

void MouseEvent::update( ) {
	_mouse = GetMouseInput( );
	calcLeftClick( );
	calcRightClick( );
	GetMousePoint( &_point.x, &_point.y );
}

void MouseEvent::calcLeftClick( ) {
	if ( _mouse & MOUSE_INPUT_LEFT ) {
		_click_left = ( _click_left + 1 ) % INT_MAX;
		return;
	}
	_click_left = 0;
}

void MouseEvent::calcRightClick( ) {
	if ( _mouse & MOUSE_INPUT_RIGHT ) {
		_click_right = ( _click_right + 1 ) % INT_MAX;
		return;
	}
	_click_right = 0;
}

int MouseEvent::getPointX( ) const {
	return _point.x;
}

int MouseEvent::getPointY( ) const {
	return _point.y;
}

bool MouseEvent::getClickLeft( ) const {
	if ( _click_left == 1 ) {
		return true;
	}
	return false;
}

bool MouseEvent::getClickRight( ) const {
	if ( _click_right == 1 ) {
		return true;
	}
	return false;
}

int MouseEvent::getClickingLeft( ) const {
	return _click_left;
}

int MouseEvent::getClickingRight( ) const {
	return _click_right;
}