#include "Drawer.h"
#include "DxLib.h"
#include "Color.h"

const int SIZE_SMALL		= 10;
const int SIZE_NORMAL		= 18;
const int SIZE_LITTLE_BIG	= 25;
const int SIZE_BIG			= 35;

Drawer::Drawer( ) {
	setFlag( 1 );
	_color = ColorPtr( new Color( ) );
	int idx = 0;
	_size[ idx ] = SIZE_SMALL;
	idx++;
	_size[ idx ] = SIZE_NORMAL;
	idx++;
	_size[ idx ] = SIZE_LITTLE_BIG;
	idx++;
	_size[ idx ] = SIZE_BIG;
	idx++;
	initialize( );
}

Drawer::~Drawer( ) {
	for ( int i = 0; i < FONTHANDLE_MAX; i++ ) {
		if ( _handle_font[ i ] < 0 ) {
			continue;
		}
		DeleteFontToHandle( _handle_font[ i ] );
	}
}

std::string Drawer::getTag( ) {
	return "DRAWER";
}

void Drawer::initialize( ) {
	for ( int i = 0; i < FONTHANDLE_MAX; i++ ) {
		_handle_font[ i ] = -1;
		switch ( ( FONTSIZE_TYPE )i ) {
		case SMALL		: _handle_font[ i ] = CreateFontToHandle( "MS –¾’©", _size[ i ], -1, DX_FONTTYPE_NORMAL ); break;
		case NORMAL		: _handle_font[ i ] = CreateFontToHandle( "MS –¾’©", _size[ i ], -1, DX_FONTTYPE_NORMAL ); break;
		case LITTLE_BIG	: _handle_font[ i ] = CreateFontToHandle( "MS –¾’©", _size[ i ], -1, DX_FONTTYPE_NORMAL ); break;
		case BIG		: _handle_font[ i ] = CreateFontToHandle( "MS –¾’©", _size[ i ], -1, DX_FONTTYPE_NORMAL ); break;
		}
	}
}

void Drawer::update( ) {
	drawImage( );
	drawString( );
	drawLine( );
	reset( );
}

void Drawer::drawImage( ) {
	std::list< ImageProperty >::iterator ite;
	ite = _images.begin( );
	for ( ite; ite != _images.end( ); ite++ ) {
		bool brend = false;
		if ( ite->brt > 0 ) {
			brend = true;
		}

		if ( brend ) {
			SetDrawBlendMode( DX_BLENDMODE_ALPHA, ite->brt );
		}

		DrawRotaGraphF( ( float )ite->cx, ( float )ite->cy, ite->size, ite->angle, ite->png, TRUE );

		if ( brend ) {
			SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
		}
	}
}

void Drawer::drawString( ) {
	std::list< StringProperty >::iterator ite;
	ite = _strings.begin( );

	for ( ite; ite != _strings.end( ); ite++ ) {
		bool brend = false;
		if ( ite->brt < 255 ) {
			brend = true;
		}

		if ( brend ) {
			SetDrawBlendMode( DX_BLENDMODE_ALPHA, ite->brt );
		}

		DrawFormatStringFToHandle( ite->x, ite->y, _color->getColor( ite->col ), ite->handle, "%s", ite->str.c_str( ) );

		if ( brend ) {
			SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
		}
	}
}

void Drawer::drawLine( ) {
	std::list< LineProperty >::iterator ite;
	ite = _lines.begin( );
	if ( _lines.size( ) < 1 ) {
		return;
	}

	SetDrawMode( DX_DRAWMODE_BILINEAR );
	for ( ite; ite != _lines.end( ); ite++ ) {
		DrawLineAA( ite->sx, ite->sy, ite->ex, ite->ey, _color->getColor( ite->col ) );
	}
	SetDrawMode( DX_DRAWMODE_NEAREST );
}

void Drawer::setImage( ImageProperty png ) {
	_images.push_back( png );
}

void Drawer::setString( bool flag, double x, double y, COLOR col, std::string str, Drawer::FONTSIZE_TYPE type, int brt ) {
	//“n‚³‚ê‚½ x, y ‚ª•`‰æ‚·‚é‚Æ‚«‚É•¶Žš—ñ‚Ì’†‰›‚É‚È‚é‚æ‚¤‚É‚·‚é
	if ( flag ) {
		int len = ( int )str.length( );
		double w = GetDrawFormatStringWidthToHandle( _handle_font[ type ], str.c_str( ) );
		x -= w / 2;
	}
	double h = GetDrawFormatStringWidthToHandle( _handle_font[ type ], "‚ " );
	y -= h / 2;
	StringProperty tmp = { ( float )x, ( float )y, col, str, brt, _handle_font[ type ] };
	_strings.push_back( tmp );
}

void Drawer::setLine( double sx, double sy, double ex, double ey, COLOR col ) {
	LineProperty line = { ( float )sx, ( float )sy, ( float )ex, ( float )ey, col };
	_lines.push_back( line );
}

int Drawer::getStringW( FONTSIZE_TYPE type, std::string str ) const {
	return GetDrawFormatStringWidthToHandle( _handle_font[ type ], str.c_str( ) );
}

int Drawer::getStringH( FONTSIZE_TYPE type ) const {
	return GetDrawFormatStringWidthToHandle( _handle_font[ type ], "‚ " );
}

void Drawer::reset( ) {
	int size = 0;
	size = ( int )_images.size( );
	if ( size > 0 ) {
		std::list< ImageProperty >( ).swap( _images );
	}
	size = ( int )_strings.size( );
	if ( size > 0 ) {
		std::list< StringProperty >( ).swap( _strings );
	}
	size = ( int )_lines.size( );
	if ( size > 0 ) {
		std::list< LineProperty >( ).swap( _lines );
	}
}
