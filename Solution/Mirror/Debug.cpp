#include "Debug.h"
#include "DxLib.h"
#include "GlobalData.h"
#include <time.h>

const int INIT_X = 20;
const int INIT_Y = 0;

Debug::Debug( ) {
}

Debug::~Debug( ) {
}

void Debug::initialize( ) {
}

void Debug::error( char *err ) {
	if ( fopen_s( &_fp, "error.txt", "w" ) != 0 ) {
		printfDx( "Error：ファイルオープンに失敗しました。" );
		exit( 0 );
	}
	time_t now = time( NULL );
	const int SIZE = 128;
	char buf[ SIZE ] = { };
	ctime_s( buf, SIZE, &now );
	//改行を消す
	int len = ( int )strlen( buf );
	buf[ len - 1 ] = '\0';

	fprintf_s( _fp, "[ %s ] %s", buf, err );
	fclose( _fp );

	DxLib_End( );
	exit( 0 );
}

void Debug::update( ) {
}