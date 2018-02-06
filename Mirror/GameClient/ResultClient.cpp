#include "ResultClient.h"
#include "GlobalData.h"
#include "Game.h"
#include "Drawer.h"

ResultClient::ResultClient( GlobalDataPtr data, GamePtr game ) :
_data( data ),
_game( game ) {
	_drawer = _data->getDrawerPtr( );
	setFlag( 1 );
}

ResultClient::~ResultClient( ) {
}

std::string ResultClient::getTag( ) {
	return "RESULTCLIENT";
}

void ResultClient::initialize( ) {
	_win = _game->isWin( );
}

void ResultClient::update( ) {
	if ( _win ) {
		//Ÿ—˜
		_drawer->setString( true, WIDTH / 2, HEIGHT / 2, RED, "‚ ‚È‚½‚ÌŸ‚¿", Drawer::BIG );
	} else {
		//”s–k
		_drawer->setString( true, WIDTH / 2, HEIGHT / 2, RED, "‚ ‚È‚½‚Ì•‰‚¯", Drawer::BIG );
	}

	if ( _data->getKeyState( KEY_INPUT_RETURN ) == 1 ) {
		_data->setScene( TITLE );
	}
}
