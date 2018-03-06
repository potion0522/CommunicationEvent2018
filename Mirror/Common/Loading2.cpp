#include "Loading2.h"
#include "DxLib.h"
#include "const.h"

const short int STRING_SIZE = 50;
const float MAX_LENGTH = WIDTH * 0.35f;

Loading2::Loading2( std::string message ) :
_message( message ) {
	_fin = false;
	_length = 0;
	_max = 0;
	_cnt = 0;
	_font_handle = CreateFontToHandle( "ƒƒCƒŠƒI", STRING_SIZE, -1, DX_FONTTYPE_NORMAL );
}

Loading2::~Loading2( ) {
}

void Loading2::update( ) {
	WaitVSync( 30 );
	while ( !_fin ) {
		ClearDrawScreen( );

		DrawFormatStringToHandle( WIDTH / 2 - STRING_SIZE * ( ( int )_message.length( ) / 4 ), ( int )( HEIGHT * 0.3 ), 0xffffff, _font_handle, _message.c_str( ) );
		DrawFormatStringToHandle( WIDTH / 2 - STRING_SIZE * ( ( int )_message.length( ) / 4 ), ( int )( HEIGHT * 0.4 ), 0xff0000, _font_handle, "%.1f%%", _length * 100 );

		if ( _length >= 1 ) {
			_fin = true;
		}

		ScreenFlip( );
	}
}

bool Loading2::isFinish( ) const {
	return _fin;
}

void Loading2::add( float add ) {
	_length = add;
}

void Loading2::setMaxLength( float max ) {
	_max = max;
}