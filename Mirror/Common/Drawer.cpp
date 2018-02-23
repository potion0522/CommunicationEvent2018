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
		case SMALL      : _handle_font[ i ] = CreateFontToHandle( "MS –¾’©", _size[ i ], -1, DX_FONTTYPE_NORMAL ); break;
		case NORMAL     : _handle_font[ i ] = CreateFontToHandle( "MS –¾’©", _size[ i ], -1, DX_FONTTYPE_NORMAL ); break;
		case LITTLE_BIG : _handle_font[ i ] = CreateFontToHandle( "MS –¾’©", _size[ i ], -1, DX_FONTTYPE_NORMAL ); break;
		case BIG        : _handle_font[ i ] = CreateFontToHandle( "MS –¾’©", _size[ i ], -1, DX_FONTTYPE_NORMAL ); break;
		case SUPER_BIG  : _handle_font[ i ] = CreateFontToHandle( "MS –¾’©", _size[ i ], -1, DX_FONTTYPE_NORMAL ); break;
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
	drawBackGroundImage( );
	drawBackImage( );
	drawBackString( );
	drawCircle( );
	drawBackBox( );
	drawLine( );
	drawFrontImage( );
	drawFrontString( );
	drawFrontBox( );
	reset( );
}

void Drawer::drawBackImage( ) {
	std::list< ExtendImageProperty >::iterator ite;
	ite = _back_images.begin( );
	for ( ite; ite != _front_images.end( ); ite++ ) {
		ImageProperty back_image = ite->base;
		errno_t front_image_handle = back_image.png;
		if ( back_image.png < 1 ) {
			DebugPtr debug( new Debug( ) );
			debug->error( "drawer->drawImage : ‰æ‘œƒnƒ“ƒhƒ‹‚ª‚ ‚è‚Ü‚¹‚ñ" );
		}
		assert( front_image_handle != -1 );

		bool brend = false;
		if ( back_image.brt < 255 ) {
			brend = true;
		}

		if ( brend ) {
			SetDrawBlendMode( DX_BLENDMODE_ALPHA, back_image.brt );
		}

		//‰æ‘œŠg‘å—¦(•‚¾‚¯A‚‚³‚¾‚¯)•Ï‚¦‚éê‡
		if ( ite->extend ) {
			//•ÏŒ`‚µ‚Ä•`‰æ
			SetDrawMode( DX_DRAWMODE_BILINEAR );
			DrawRotaGraph3F( ( float )back_image.cx, ( float )back_image.cy, ( float )ite->base.cx, ( float )ite->base.cy, ite->extend_width, ite->extend_height, back_image.angle, back_image.png, TRUE, FALSE );
			SetDrawMode( DX_DRAWMODE_NEAREST );
		} else {
			//’Êí•`‰æ
			if ( ite->base.size != 1 ) {
				SetDrawMode( DX_DRAWMODE_BILINEAR );
			}
			DrawRotaGraphF( ( float )back_image.cx, ( float )back_image.cy, back_image.size, back_image.angle, back_image.png, TRUE );
			if ( ite->base.size != 1 ) {
				SetDrawMode( DX_DRAWMODE_NEAREST );
			}
		}

		if ( brend ) {
			SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
		}
	}
}

void Drawer::drawFrontImage( ) {
	std::list< ExtendImageProperty >::iterator ite;
	ite = _front_images.begin( );
	for ( ite; ite != _front_images.end( ); ite++ ) {
		ImageProperty front_image = ite->base;
		errno_t front_image_handle = front_image.png;
		if ( front_image.png < 1 ) {
			DebugPtr debug( new Debug( ) );
			debug->error( "drawer->drawImage : ‰æ‘œƒnƒ“ƒhƒ‹‚ª‚ ‚è‚Ü‚¹‚ñ" );
		}
		assert( front_image_handle != -1 );

		bool brend = false;
		if ( front_image.brt < 255 ) {
			brend = true;
		}

		if ( brend ) {
			SetDrawBlendMode( DX_BLENDMODE_ALPHA, front_image.brt );
		}

		//‰æ‘œŠg‘å—¦(•‚¾‚¯A‚‚³‚¾‚¯)•Ï‚¦‚éê‡
		if ( ite->extend ) {
			//•ÏŒ`‚µ‚Ä•`‰æ
			SetDrawMode( DX_DRAWMODE_BILINEAR );
			DrawRotaGraph3F( ( float )front_image.cx, ( float )front_image.cy, ite->base.cx, ite->base.cy, ite->extend_width, ite->extend_height, front_image.angle, front_image.png, TRUE, FALSE );
			SetDrawMode( DX_DRAWMODE_NEAREST );
		} else {
			//’Êí•`‰æ
			if ( ite->base.size != 1 ) {
				SetDrawMode( DX_DRAWMODE_BILINEAR );
			}
			DrawRotaGraphF( ( float )front_image.cx, ( float )front_image.cy, front_image.size, front_image.angle, front_image.png, TRUE );
			if ( ite->base.size != 1 ) {
				SetDrawMode( DX_DRAWMODE_NEAREST );
			}
		}

		if ( brend ) {
			SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
		}
	}
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
void Drawer::drawBackGroundImage( ) {
	if ( _background_image.png < 0 ) {
		return;
	}

	bool brend = false;
	if ( _background_image.brt < 255 ) {
		brend = true;
	}

	if ( brend ) {
		SetDrawBlendMode( DX_BLENDMODE_ALPHA, _background_image.brt );
	}

	DrawRotaGraphF( ( float )_background_image.cx, ( float )_background_image.cy, _background_image.size, _background_image.angle, _background_image.png, TRUE );

	if ( brend ) {
		SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
	}
}

void Drawer::setBackGroundImage( ImageProperty png ) {
	_background_image = png;
}

void Drawer::setBackGroundImage( LightImageProperty png ) {
	ImageProperty image = ImageProperty( );
	image.cx = png.cx;
	image.cy = png.cy;
	image.png = png.png;
	_background_image = image;
}

void Drawer::setFrontImage( ImageProperty png ) {
	ExtendImageProperty image = ExtendImageProperty( );
	image.base = png;
	_front_images.push_back( image );
}

void Drawer::setFrontImage( LightImageProperty png ) {
	ExtendImageProperty image = ExtendImageProperty( );
	image.base.cx = png.cx;
	image.base.cy = png.cy;
	image.base.png = png.png;
	_front_images.push_back( image );
}


void Drawer::setBackImage( ImageProperty png ) {
	ExtendImageProperty image = ExtendImageProperty( );
	image.base = png;
	_back_images.push_back( image );
}

void Drawer::setBackImage( LightImageProperty png ) {
	ExtendImageProperty image = ExtendImageProperty( );
	image.base.cx = png.cx;
	image.base.cy = png.cy;
	image.base.png = png.png;
	_back_images.push_back( image );
}

void Drawer::setFrontString( bool flag, double x, double y, COLOR col, std::string str, Drawer::FONTSIZE_TYPE type, int brt ) {
	//“n‚³‚ê‚½ x, y ‚ª•`‰æ‚·‚é‚Æ‚«‚É•¶Žš—ñ‚Ì’†‰›‚É‚È‚é‚æ‚¤‚É‚·‚é
	if ( flag ) {
		int len = ( int )str.length( );
		double w = GetDrawFormatStringWidthToHandle( _handle_font[ type ], str.c_str( ) );
		x -= w / 2;
	}
	double h = GetDrawFormatStringWidthToHandle( _handle_font[ type ], "‚ " );
	y -= h / 2;
	StringProperty tmp = { ( float )x, ( float )y, col, str, brt, _handle_font[ type ] };
	_front_strings.push_back( tmp );
}

void Drawer::setBackString( bool flag, double x, double y, COLOR col, std::string str, Drawer::FONTSIZE_TYPE type, int brt ) {
	//“n‚³‚ê‚½ x, y ‚ª•`‰æ‚·‚é‚Æ‚«‚É•¶Žš—ñ‚Ì’†‰›‚É‚È‚é‚æ‚¤‚É‚·‚é
	if ( flag ) {
		int len = ( int )str.length( );
		double w = GetDrawFormatStringWidthToHandle( _handle_font[ type ], str.c_str( ) );
		x -= w / 2;
	}
	double h = GetDrawFormatStringWidthToHandle( _handle_font[ type ], "‚ " );
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

void Drawer::setExtendFrontImage( ImageProperty base, float image_cx, float image_cy, double extend_width, double extend_height ) {
	ExtendImageProperty image = ExtendImageProperty( );
	image.extend = true;
	image.base = base;
	image.image_cx = image_cx;
	image.image_cy = image_cy;
	image.extend_width = extend_width;
	image.extend_height = extend_height;
	_front_images.push_back( image );
}

void Drawer::setExtendFrontImage( LightImageProperty base, float image_cx, float image_cy, double extend_width, double extend_height ) {
	ExtendImageProperty extend = ExtendImageProperty( );
	ImageProperty image = ImageProperty( );
	image.cx = base.cx;
	image.cy = base.cy;
	image.png = base.png;

	extend.extend = true;
	extend.base = image;
	extend.image_cx = image_cx;
	extend.image_cy = image_cy;
	extend.extend_width = extend_width;
	extend.extend_height = extend_height;
	_front_images.push_back( extend );
}

void Drawer::setExtendBackImage( ImageProperty base, float image_cx, float image_cy, double extend_width, double extend_height ) {
	ExtendImageProperty image = ExtendImageProperty( );
	image.extend = true;
	image.base = base;
	image.image_cx = image_cx;
	image.image_cy = image_cy;
	image.extend_width = extend_width;
	image.extend_height = extend_height;
	_back_images.push_back( image );
}

void Drawer::setExtendBackImage( LightImageProperty base, float image_cx, float image_cy, double extend_width, double extend_height ) {
	ExtendImageProperty extend = ExtendImageProperty( );
	ImageProperty image = ImageProperty( );
	image.cx = base.cx;
	image.cy = base.cy;
	image.png = base.png;

	extend.extend = true;
	extend.base = image;
	extend.image_cx = image_cx;
	extend.image_cy = image_cy;
	extend.extend_width = extend_width;
	extend.extend_height = extend_height;
	_back_images.push_back( extend );
}

int Drawer::getStringW( FONTSIZE_TYPE type, std::string str ) const {
	return GetDrawFormatStringWidthToHandle( _handle_font[ type ], str.c_str( ) );
}

int Drawer::getStringH( FONTSIZE_TYPE type ) const {
	return GetDrawFormatStringWidthToHandle( _handle_font[ type ], "‚ " );
}

void Drawer::reset( ) {
	_background_image = ImageProperty( );

	int size = 0;
	size = ( int )_front_images.size( );
	if ( size > 0 ) {
		std::list< ExtendImageProperty >( ).swap( _front_images );
	}
	size = ( int )_back_images.size( );
	if ( size > 0 ) {
		std::list< ExtendImageProperty >( ).swap( _back_images );
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
