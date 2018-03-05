#include "ResultClient.h"
#include "GlobalData.h"
#include "Game.h"
#include "Drawer.h"
#include "KeyBoard.h"

ResultClient::ResultClient( GlobalDataPtr data, GamePtr game ) :
_data( data ),
_game( game ) {
	_drawer = _data->getDrawerPtr( );
	_keyboard = KeyBoardPtr( new KeyBoard( ) );
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

	_drawer->setFrontString( true, WIDTH / 2, HEIGHT / 5 * 3, YELLOW, "Press \"ENTER\" Key", Drawer::BIG );

	if ( _data->getKeyState( _keyboard->getKeyCode( ENTER_KEY ) ) == 1 ) {
		_data->setInitFlag( );
	}
}
