#include "ResultClient.h"
#include "GlobalData.h"
#include "Game.h"
#include "Drawer.h"

ResultClient::ResultClient( GlobalDataPtr data, GamePtr game ) :
_data( data ),
_game( game ) {
	setFlag( 1 );
	_drawer = _data->getDrawerPtr( );
}

ResultClient::~ResultClient( ) {
}

std::string ResultClient::getTag( ) {
	return "RESULTCLIENT";
}

void ResultClient::initialize( ) {
	_win = _game->isWin( );
	_cnt = FRAME * 5;
}

void ResultClient::finalize( ) {
}

void ResultClient::update( ) {
	if ( _win ) {
		//Ÿ—˜
		_drawer->setFrontString( true, WIDTH / 2, HEIGHT / 3, RED, "‚ ‚È‚½‚ÌŸ‚¿", Drawer::SUPER_BIG );
	} else {
		//”s–k
		_drawer->setFrontString( true, WIDTH / 2, HEIGHT / 3, RED, "‚ ‚È‚½‚Ì•‰‚¯", Drawer::SUPER_BIG );
	}

	_drawer->setFrontString( true, WIDTH / 2, HEIGHT / 5 * 3, YELLOW, "Return Title ....", Drawer::BIG );
	_drawer->setFrontString( true, WIDTH / 2, HEIGHT / 5 * 3, RED,    "                  " + std::to_string( _cnt / FRAME ), Drawer::BIG );

	_cnt--;

	if ( _cnt <= 0 ) {
		_data->setInitFlag( );
	}
}
