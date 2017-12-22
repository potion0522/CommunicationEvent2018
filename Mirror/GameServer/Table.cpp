#include "Table.h"
#include "const.h"
#include "DxLib.h"
#include "Color.h"

const int CHARA_SIZE = 20;

Table::Table( ) {
	initialize( );
}

Table::~Table( ) {
}

void Table::initialize( ) {
	_add = SERVER_CONSOLE_LOG_MAX - 1;
}

void Table::update( ) {
	isLogPring( );
}

void Table::isLogPring( ) {
	const int LOG_X = WIDTH - CHARA_SIZE * 15;
	int size = ( int )_log.size( );
	if ( size < 0 ) {
		return;
	}
	ColorPtr color( new Color( ) );

	for ( int i = 0; i < size; i++ ) {
		int y = HEIGHT - 20 * i;
		DrawFormatString( LOG_X, y, color->getColor( WHITE ), "%s", _log[ i ].c_str( ) );
	}
}

void Table::add( std::string log ) {
	_log[ _add ] = log;
	_add = ( _add + ( SERVER_CONSOLE_LOG_MAX - 1 ) ) % SERVER_CONSOLE_LOG_MAX;
}