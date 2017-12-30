#include "Console.h"
#include "GlobalData.h"
#include "Color.h"
#include "Drawer.h"

Console::Console( GlobalDataPtr data ) :
_data( data ) {
}

Console::~Console( ) {
	_client->disConnect( );
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
	std::string finding = "finding server";
	std::string ip = _client->getSeverIP( );
	int size_find = ( int )finding.length( );
	int size_ip = ( int )ip.length( );
	_drawer->setString( WIDTH / 2 - size_find * 10 / 2, HEIGHT / 2 - 60, YELLOW, finding );
	_drawer->setString( WIDTH / 2 - size_ip * 10 / 2, HEIGHT / 2, WHITE, ip );
}

void Console::connecting( ) {
	_client->sendTcp( _send_data );

	if ( _client->isRecvingTcp( ) ) {
		_recv_data_tcp = _client->getDataTcp( );
	}
	if ( _client->isRecvingUdp( ) ) {
		_recv_data_udp = _client->getDataUdp( );
	}
	_drawer->setString( WIDTH - 200, HEIGHT - 200, WHITE, "recv_udp : " + std::to_string( _recv_data_udp.test ) );
}