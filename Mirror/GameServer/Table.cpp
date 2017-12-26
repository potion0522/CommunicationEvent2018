#include "Table.h"
#include "const.h"
#include "DxLib.h"
#include "Color.h"
#include "GlobalData.h"
#include "Drawer.h"

const int CHARA_SIZE = 20;

Table::Table( GlobalDataPtr data ) :
_data( data ) {
	initialize( );
}

Table::~Table( ) {
}

void Table::initialize( ) {
	_add = SERVER_CONSOLE_LOG_MAX - 1;
	_drawer = _data->getDrawerPtr( );
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
		_drawer->setString( LOG_X, y, WHITE, _log[ i ] );
	}
}

void Table::add( std::string log ) {
	_log[ _add ] = log;
	_add = ( _add + ( SERVER_CONSOLE_LOG_MAX - 1 ) ) % SERVER_CONSOLE_LOG_MAX;
}