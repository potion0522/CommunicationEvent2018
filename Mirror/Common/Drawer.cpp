#include "Drawer.h"
#include "DxLib.h"
#include "Color.h"

Drawer::Drawer( ) {
	setFlag( 1 );
	_color = ColorPtr( new Color( ) );
}

Drawer::~Drawer( ) {
}

std::string Drawer::getTag( ) {
	return "DRAWER";
}

void Drawer::update( ) {
	drawImage( );
	drawString( );
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
		if ( ite->brt > 0 ) {
			brend = true;
		}

		if ( brend ) {
			SetDrawBlendMode( DX_BLENDMODE_ALPHA, ite->brt );
		}

		DrawFormatStringF( ite->x, ite->y, _color->getColor( ite->col ), "%s", ite->str.c_str( ) );

		if ( brend ) {
			SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
		}
	}
}

void Drawer::setImage( ImageProperty png ) {
	_images.push_back( png );
}

void Drawer::setString( double x, double y, COLOR col, std::string str, int brt ) {
	StringProperty tmp = { ( float )x, ( float )y, col, str, brt };
	_strings.push_back( tmp );
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
}
