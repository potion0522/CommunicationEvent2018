#include "Drawer.h"
#include "DxLib.h"
#include "Color.h"
#include "Debug.h"
#include <math.h>
#include <assert.h>
#include <errno.h>

const int SIZE_SMALL        = 10;
const int SIZE_NORMAL       = 18;
const int SIZE_LITTLE_BIG   = 25;
const int SIZE_BIG          = 35;
const int SIZE_SUPER_BIG    = 100;

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
	_size[ idx ] = SIZE_SUPER_BIG;
	idx++;
	for ( int i = 0; i < FONT_TYPE_MAX; i++ ) {
		_handle_font[ i ] = -1;
		switch ( ( FONTSIZE_TYPE )i ) {
		case SMALL      : _handle_font[ i ] = CreateFontToHandle( "MS ñæí©", _size[ i ], -1, DX_FONTTYPE_NORMAL ); break;
		case NORMAL     : _handle_font[ i ] = CreateFontToHandle( "MS ñæí©", _size[ i ], -1, DX_FONTTYPE_NORMAL ); break;
		case LITTLE_BIG : _handle_font[ i ] = CreateFontToHandle( "MS ñæí©", _size[ i ], -1, DX_FONTTYPE_NORMAL ); break;
		case BIG        : _handle_font[ i ] = CreateFontToHandle( "MS ñæí©", _size[ i ], -1, DX_FONTTYPE_NORMAL ); break;
		case SUPER_BIG  : _handle_font[ i ] = CreateFontToHandle( "MS ñæí©", _size[ i ], -1, DX_FONTTYPE_NORMAL ); break;
		}
	}
}

Drawer::~Drawer( ) {
	for ( int i = 0; i < FONT_TYPE_MAX; i++ ) {
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
}

void Drawer::update( ) {
	drawBackImage( );
	drawBackString( );
	drawCircle( );
	drawBackBox( );
	drawLine( );
	drawImage( );
	drawFrontString( );
	drawFrontBox( );
	reset( );
}

void Drawer::drawImage( ) {
	std::list< ExtendImageProperty >::iterator ite;
	ite = _images.begin( );
	SetDrawMode( DX_DRAWMODE_BILINEAR );
	for ( ite; ite != _images.end( ); ite++ ) {
		ImageProperty image = ite->base;
		errno_t image_handle = image.png;
		if ( image.png < 1 ) {
			DebugPtr debug( new Debug( ) );
			debug->error( "drawer->drawImage : âÊëúÉnÉìÉhÉãÇ™Ç†ÇËÇ‹ÇπÇÒ" );
		}
		assert( image_handle != -1 );

		bool brend = false;
		if ( image.brt < 255 ) {
			brend = true;
		}

		if ( brend ) {
			SetDrawBlendMode( DX_BLENDMODE_ALPHA, image.brt );
		}

		//âÊëúägëÂó¶(ïùÇæÇØÅAçÇÇ≥ÇæÇØ)ïœÇ¶ÇÈèÍçá
		if ( ite->extend ) {
			//ïœå`ÇµÇƒï`âÊ
			DrawRotaGraph3F( ( float )image.cx, ( float )image.cy, ite->image_cx, ite->image_cy, ite->extend_width, ite->extend_height, image.angle, image.png, TRUE, FALSE );
		} else {
			//í èÌï`âÊ
			DrawRotaGraphF( ( float )image.cx, ( float )image.cy, image.size, image.angle, image.png, TRUE );
		}

		if ( brend ) {
			SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
		}
	}
	SetDrawMode( DX_DRAWMODE_NEAREST );
}

void Drawer::drawFrontString( ) {
	std::list< StringProperty >::iterator ite;
	ite = _front_strings.begin( );

	for ( ite; ite != _front_strings.end( ); ite++ ) {
		if ( ite->brt < 255 ) {
			SetDrawBlendMode( DX_BLENDMODE_ALPHA, ite->brt );
		}

		DrawFormatStringFToHandle( ite->x, ite->y, _color->getColor( ite->col ), ite->handle, "%s", ite->str.c_str( ) );

		if ( ite->brt < 255 ) {
			SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
		}
	}
}

void Drawer::drawBackString( ) {
	std::list< StringProperty >::iterator ite;
	ite = _back_strings.begin( );

	for ( ite; ite != _back_strings.end( ); ite++ ) {
		if ( ite->brt < 255 ) {
			SetDrawBlendMode( DX_BLENDMODE_ALPHA, ite->brt );
		}

		DrawFormatStringFToHandle( ite->x, ite->y, _color->getColor( ite->col ), ite->handle, "%s", ite->str.c_str( ) );

		if ( ite->brt < 255 ) {
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
		if ( ite->brt < 255 ) {
			SetDrawBlendMode( DX_BLENDMODE_ALPHA, ite->brt );
		}

		DrawLineAA( ite->sx, ite->sy, ite->ex, ite->ey, _color->getColor( ite->col ) );

		if ( ite->brt < 255 ) {
			SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
		}
	}
	SetDrawMode( DX_DRAWMODE_NEAREST );

}

void Drawer::drawCircle( ) {
	std::list< CircleProperty >::iterator ite;
	ite = _circles.begin( );
	if( _circles.size( ) < 1 ) {
		return;
	}

	SetDrawMode(  DX_DRAWMODE_BILINEAR );
	for( ite; ite != _circles.end( ); ite++ ) {

		if ( ite->brt < 255 ) {
			SetDrawBlendMode( DX_BLENDMODE_ALPHA, ite->brt );
		}
		DrawCircleAA( ite->cx, ite->cy, ite->r, 32, _color->getColor( ite->col ), ite->isFill );

		if ( ite->brt < 255 ) {
			SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
		}
	}
	SetDrawMode( DX_DRAWMODE_NEAREST );
}

void Drawer::drawFrontBox( ) {
	std::list< BoxProperty >::iterator ite;
	ite = _front_boxes.begin( );
	if( _front_boxes.size( ) < 1 ) {
		return;
	}

	for( ite; ite != _front_boxes.end( ); ite++ ) {
		DrawBoxAA( ite->lx, ite->ly, ite->rx, ite->ry, _color->getColor( ite->col ), FALSE );
	}
}

void Drawer::drawBackBox( ) {
	std::list< BoxProperty >::iterator ite;
	ite = _back_boxes.begin( );
	if( _back_boxes.size( ) < 1 ) {
		return;
	}

	for( ite; ite != _back_boxes.end( ); ite++ ) {
		DrawBoxAA( ite->lx, ite->ly, ite->rx, ite->ry, _color->getColor( ite->col ), FALSE );
	}
}
void Drawer::drawBackImage( ) {
	if ( _back_image.png < 0 ) {
		return;
	}

	bool brend = false;
	if ( _back_image.brt < 255 ) {
		brend = true;
	}

	if ( brend ) {
		SetDrawBlendMode( DX_BLENDMODE_ALPHA, _back_image.brt );
	}

	DrawRotaGraphF( ( float )_back_image.cx, ( float )_back_image.cy, _back_image.size, _back_image.angle, _back_image.png, TRUE );

	if ( brend ) {
		SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
	}
}

void Drawer::setBackImage( ImageProperty png ) {
	_back_image = png;
}

void Drawer::setBackImage( LightImageProperty png ) {
	ImageProperty image = ImageProperty( );
	image.cx = png.cx;
	image.cy = png.cy;
	image.png = png.png;
	_back_image = image;
}

void Drawer::setImage( ImageProperty png ) {
	ExtendImageProperty image = ExtendImageProperty( );
	image.base = png;
	_images.push_back( image );
}

void Drawer::setImage( LightImageProperty png ) {
	ExtendImageProperty image = ExtendImageProperty( );
	image.base.cx = png.cx;
	image.base.cy = png.cy;
	image.base.png = png.png;
	_images.push_back( image );
}

void Drawer::setFrontString( bool flag, double x, double y, COLOR col, std::string str, Drawer::FONTSIZE_TYPE type, int brt ) {
	//ìnÇ≥ÇÍÇΩ x, y Ç™ï`âÊÇ∑ÇÈÇ∆Ç´Ç…ï∂éöóÒÇÃíÜâõÇ…Ç»ÇÈÇÊÇ§Ç…Ç∑ÇÈ
	if ( flag ) {
		int len = ( int )str.length( );
		double w = GetDrawFormatStringWidthToHandle( _handle_font[ type ], str.c_str( ) );
		x -= w / 2;
	}
	double h = GetDrawFormatStringWidthToHandle( _handle_font[ type ], "Ç†" );
	y -= h / 2;
	StringProperty tmp = { ( float )x, ( float )y, col, str, brt, _handle_font[ type ] };
	_front_strings.push_back( tmp );
}

void Drawer::setBackString( bool flag, double x, double y, COLOR col, std::string str, Drawer::FONTSIZE_TYPE type, int brt ) {
	//ìnÇ≥ÇÍÇΩ x, y Ç™ï`âÊÇ∑ÇÈÇ∆Ç´Ç…ï∂éöóÒÇÃíÜâõÇ…Ç»ÇÈÇÊÇ§Ç…Ç∑ÇÈ
	if ( flag ) {
		int len = ( int )str.length( );
		double w = GetDrawFormatStringWidthToHandle( _handle_font[ type ], str.c_str( ) );
		x -= w / 2;
	}
	double h = GetDrawFormatStringWidthToHandle( _handle_font[ type ], "Ç†" );
	y -= h / 2;
	StringProperty tmp = { ( float )x, ( float )y, col, str, brt, _handle_font[ type ] };
	_back_strings.push_back( tmp );
}

void Drawer::setLine( double sx, double sy, double ex, double ey, COLOR col, int brt ) {
	LineProperty line = { ( float )sx, ( float )sy, ( float )ex, ( float )ey, col, brt };
	_lines.push_back( line );
}

void Drawer::setCircle( double x, double y, double r, COLOR col, int brt, bool isfill ) {
	CircleProperty circle = { ( float )x, ( float )y, ( float )r, col, brt, isfill };
	_circles.push_back( circle );
}

void Drawer::setFrontBox( double lx, double ly, double rx, double ry, COLOR col ){
	BoxProperty box = { ( float )lx, ( float )ly, ( float )rx, ( float )ry, col };
	_front_boxes.push_back( box );
}

void Drawer::setBackBox( double lx, double ly, double rx, double ry, COLOR col ){
	BoxProperty box = { ( float )lx, ( float )ly, ( float )rx, ( float )ry, col };
	_back_boxes.push_back( box );
}

void Drawer::setExtendImage( ImageProperty base, float image_cx, float image_cy, double extend_width, double extend_height ) {
	ExtendImageProperty image = ExtendImageProperty( );
	image.extend = true;
	image.base = base;
	image.image_cx = image_cx;
	image.image_cy = image_cy;
	image.extend_width = extend_width;
	image.extend_height = extend_height;
	_images.push_back( image );
}

int Drawer::getStringW( FONTSIZE_TYPE type, std::string str ) const {
	return GetDrawFormatStringWidthToHandle( _handle_font[ type ], str.c_str( ) );
}

int Drawer::getStringH( FONTSIZE_TYPE type ) const {
	return GetDrawFormatStringWidthToHandle( _handle_font[ type ], "Ç†" );
}

void Drawer::reset( ) {
	_back_image = ImageProperty( );

	int size = 0;
	size = ( int )_images.size( );
	if ( size > 0 ) {
		std::list< ExtendImageProperty >( ).swap( _images );
	}
	size = ( int )_front_strings.size( );
	if ( size > 0 ) {
		std::list< StringProperty >( ).swap( _front_strings );
	}
	size = ( int )_back_strings.size( );
	if ( size > 0 ) {
		std::list< StringProperty >( ).swap( _back_strings );
	}
	size = ( int )_lines.size( );
	if ( size > 0 ) {
		std::list< LineProperty >( ).swap( _lines );
	}
	size = ( int )_circles.size( );
	if ( size > 0 ) {
		std::list< CircleProperty >( ).swap( _circles );
	}
	size = ( int )_front_boxes.size( );
	if ( size > 0 ) {
		std::list< BoxProperty >( ).swap( _front_boxes );
	}
	size = ( int )_back_boxes.size( );
	if ( size > 0 ) {
		std::list< BoxProperty >( ).swap( _back_boxes );
	}
}
