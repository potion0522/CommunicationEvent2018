#include "ResultServer.h"
#include "GlobalData.h"
#include "GameMaster.h"
#include "Log.h"
#include "Drawer.h"

ResultServer::ResultServer( GlobalDataPtr data, GameMasterPtr game, LogPtr log ) :
_data( data ),
_game( game ),
_log( log ) {
	_drawer = _data->getDrawerPtr( );
	setFlag( 1 );
}

ResultServer::~ResultServer( ) {
}

std::string ResultServer::getTag( )  { 
	return "RESULTSERVER";
}

void ResultServer::initialize( ) {
	_win = _game->getWinner( );
	_cnt = 60 * 3;
}

void ResultServer::update( ) {
	_drawer->setFrontString( false, WIDTH / 2, HEIGHT / 2, RED, std::to_string( ( _cnt / 60 ) + 1 ), Drawer::SUPER_BIG );

	if ( _cnt <= 0 ) {
		_data->setInitFlag( );
		_log->reset( );
		_log->add( "Game Reset" );
		return;
	}
	_cnt--;
}
