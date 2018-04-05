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
}

void ResultServer::finalize( ) {
}

void ResultServer::update( ) {
	_data->setInitFlag( );
	_log->reset( );
	_log->add( "Game Reset" );
}
