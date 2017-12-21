#include "UpdateKey.h"
#include "DxLib.h"
#include <string.h>

UpdateKey::UpdateKey( ) {
	initialize( );
}

UpdateKey::~UpdateKey( ) {
}

void UpdateKey::initialize( ) {
	memset( _key, 0, sizeof( int ) * KEY_MAX );
}

void UpdateKey::update( ) {
	inputKeyState( );
}

void UpdateKey::inputKeyState( ) {
	char tmp_key[ KEY_MAX ];
	GetHitKeyStateAll( tmp_key );

	for ( int i = 0; i < KEY_MAX; i++ ) {
		if ( tmp_key[ i ] == 1 ) {
			_key[ i ]++;
		} else {
			_key[ i ] = 0;
		}
	}
}

int UpdateKey::getKeyState( int num ) const {
	return _key[ num ];
}