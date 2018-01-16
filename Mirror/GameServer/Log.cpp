#include "Log.h"
#include "const.h"
#include "DxLib.h"
#include "Color.h"
#include "GlobalData.h"
#include "Drawer.h"
#include <time.h>

const int CHARA_SIZE = 20;

Log::Log( GlobalDataPtr data ) :
_data( data ) {
	initialize( );
}

Log::~Log( ) {
}

void Log::initialize( ) {
	_drawer = _data->getDrawerPtr( );
}

void Log::update( ) {
	isLogPring( );
}

void Log::isLogPring( ) {
	const int LOG_X = WIDTH - 250;
	int size = 0;
	for ( int i = 0; i < SERVER_CONSOLE_LOG_MAX; i++ ) {
		if ( _log[ i ].empty( ) ) {
			break;
		}
		size = i + 1;
	}

	ColorPtr color( new Color( ) );
	for ( int i = 0; i < size; i++ ) {
		int time_x = LOG_X - _drawer->getStringW( Drawer::NORMAL, _log_time[ i ] );
		int y = HEIGHT - 20 * ( i + 1 );
		COLOR col = WHITE;
		if ( i == 0 ) {
			col = YELLOW;
		}
		_drawer->setString( false,  LOG_X, y, col, _log[ i ] );
		_drawer->setString( false, time_x, y, col, _log_time[ i ] );
	}
}

void Log::add( std::string log ) {
	time_t now = time( NULL );
	const int SIZE = 128;
	char buf[ SIZE ] = { };
	ctime_s( buf, SIZE, &now );
	std::string log_time = buf;
	log_time.insert( 0, "[ " );
	log_time.insert( log_time.length( ) - 1, " ] : " );

	for ( int i = SERVER_CONSOLE_LOG_MAX - 1; i > 0; i-- ) {
		if ( _log[ i - 1 ].empty( ) ) {
			continue;
		}
		_log_time[ i ] = _log_time[ i - 1 ];
		_log[ i ] = _log[ i - 1 ];
	}
	_log_time[ 0 ] = log_time;
	_log[ 0 ] = log;
}