#include "Debug.h"
#include "DxLib.h"
#include "const.h"
#include "Color.h"
#include "GlobalData.h"
#include <time.h>

const std::string SOLUTION_DIR = "../";
const int ACTIVE_CLASS_X = 20;
const int LOG_X = 120;

Debug::Debug( GlobalDataPtr data ) :
_data( data ){
	initialize( );
}

Debug::~Debug( ) {
}

void Debug::initialize( ) {
	setFlag( 0 );
	_color = ColorPtr( new Color( ) );
}

std::string Debug::getTag( ) {
	return "DEBUG";
}

void Debug::error( std::string err ) {
	if ( fopen_s( &_fp, ( SOLUTION_DIR + "error.txt" ).c_str( ), "w" ) != 0 ) {
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
	printActiveClass( );
	initLog( );
	//initActiveClass( );
}

int Debug::calcLogYpos( int num ) {
	return ( num % ( HEIGHT / 20 ) ) * 20;
}

void Debug::printLog( ) {
	int size = ( int )_log.size( );

	for ( int i = 0; i < size; i++ ) {
		int y = calcLogYpos( i );
		DrawFormatString( LOG_X, y, _color->getColor( WHITE ), "%s", _log[ i ].c_str( ) );
	}
}

void Debug::printActiveClass( ) {
	int size = ( int )_active_class.size( );

	if ( size < 1 ) {
		return;
	}

	for ( int i = 0; i < size; i++ ) {
		int y = calcLogYpos( i );
		DrawFormatString( ACTIVE_CLASS_X, y, _color->getColor( RED ), "%s", _active_class[ i ].c_str( ) );
	}
}

void Debug::initLog( ) {
	std::vector< std::string >( ).swap( _log );
}

void Debug::addLog( std::string add ) {
	if ( getFlag( ) < 1 ) {
		return;
	}
	_log.push_back( add );
}

void Debug::initActiveClass( ) {
	std::vector< std::string >( ).swap( _log );
}

void Debug::setActiveClass( std::string tag ) {
	int size = ( int )_active_class.size( );
	for ( int i = 0; i < size; i++ ) {
		if ( _active_class[ i ].find( tag ) != std::string::npos ) {
			return;
		}
	}
	_active_class.push_back( tag );
}