#include "Console.h"
#include "GlobalData.h"
#include "Color.h"
#include "Drawer.h"

Console::Console( GlobalDataPtr data ) :
_data( data ) {
}

Console::~Console( ) {
	if ( _client->getFlag( ) ) {
		_client->disConnect( );
	}
}

std::string Console::getTag( ) {
	return "CONSOLE";
}

void Console::initialize( ) {
	setFlag( 1 );
	_client = _data->getClientPtr( );
	_drawer = _data->getDrawerPtr( );
	memset( &_recv_data_tcp, 0, sizeof( Client::NetWorkData ) );
	memset( &_recv_data_udp, 0, sizeof( Client::NetWorkData ) );
	memset( &_send_data, 0, sizeof( Client::NetWorkData ) );
}

void Console::update( ) {
	std::string phase = _client->getPhase( );

	if ( phase == "READY" ) {
		ready( );
	}
	if ( phase == "CONNECTING" ) {
		connecting( );
	}

	if ( _data->getKeyState( KEY_INPUT_X ) == 1 ) {
		_client->disConnect( );
		_data->setScene( TITLE );
	}
}

void Console::ready( ) {
	std::string finding = "- Connect Server -";
	std::string ip = _client->getSeverIP( );
	int size_rate = _drawer->getStringH( Drawer::BIG );
	_drawer->setString( WIDTH / 2, HEIGHT / 2 - size_rate, YELLOW, finding, Drawer::BIG );
	_drawer->setString( WIDTH / 2, HEIGHT / 2, WHITE, ip );
}

void Console::connecting( ) {
	_client->sendTcp( _send_data );

	if ( !_client->isRecvingTcp( ) ) {
		std::string matching = "- Matching -";
		_drawer->setString(  WIDTH / 2, HEIGHT / 2 - 60, YELLOW, matching );
	}
	_recv_data_tcp = _client->getDataTcp( );
}