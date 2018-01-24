#include "Color.h"
#include "DxLib.h"

Color::Color( ) {
	initialize( );
}

Color::~Color( ) {
}

void Color::initialize( ) {
	_col = GetColor( 255, 255, 255 );
}

int Color::getColor( COLOR col ) {
	switch ( col ) {
	case WHITE	: setWhite( )	;	break;
	case BLACK	: setBlack( )	;	break;
	case RED	: setRed( )		;	break;
	case BLUE	: setBlue( )	;	break;
	case WATER	: setWater( )	;	break;
	case YELLOW	: setYellow( )	;	break;
	case PURPLE	: setPurple( )	;	break;
	default		: setWhite( )	;	break;
	}

	return _col;
}

void Color::setWhite( ) {
	_col = GetColor( 255, 255, 255 );
}

void Color::setBlack( ) {
	_col = GetColor( 0, 0, 0 );
}

void Color::setRed( ) {
	_col = GetColor( 255, 0, 0 );
}

void Color::setBlue( ) {
	_col = GetColor( 0, 0, 255 );
}

void Color::setWater( ) {
	_col = GetColor( 3, 222, 241 );
}

void Color::setYellow( ) {
	_col = GetColor( 255, 255, 0 );
}

void Color::setPurple( ) {
	_col = GetColor( 255, 0, 255 );
}