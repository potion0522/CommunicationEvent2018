#include "Debug.h"
#include "DxLib.h"
#include "const.h"
#include "Color.h"
#include "GlobalData.h"
#include <time.h>

const int INIT_X = 20;
const int INIT_Y = 0;

Debug::Debug( std::shared_ptr< GlobalData > data ) :
_data( data ){
	initialize( );
}

Debug::~Debug( ) {
}

void Debug::initialize( ) {
	_color = std::shared_ptr< Color >( new Color( ) );
}

void Debug::error( std::string err ) {
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

	fprintf_s( _fp, "[ %s ] %s", buf, err.c_str( ) );
	fclose( _fp );

	DxLib_End( );
	exit( 0 );
}

void Debug::update( ) {
	printLog( );
	initLog( );
}

void Debug::printLog( ) {
	int size = ( int )_log.size( );

	for ( int i = 0; i < size; i++ ) {
		int y = ( i % ( HEIGHT / 20 ) ) * 20;
		DrawFormatString( 0, y, _color->getColor( WHITE ), "%s", _log[ i ].c_str( ) );
	}
}

void Debug::initLog( ) {
	std::vector< std::string >( ).swap( _log );
}

void Debug::addLog( std::string add ) {
	_log.push_back( add );
}