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
	memset( &_recv_data_udp, 0, sizeof( Client::NetWorkData ) );
	memset( &_send_data, 0, sizeof( Client::NetWorkData ) );
	_recving_udp = false;
}

void Console::update( ) {
	if ( _data->getScene( ) != BATTLE ) {
		if ( _data->getKeyState( KEY_INPUT_X ) == 1 && !_data->getCommandFlag( ) ) {
			_data->setScene( TITLE );
			_client->disConnect( );
			_client->initialize( );
			return;
		}
	}

	std::string phase = _client->getPhase( );
	if ( phase == "READY" ) {
		ready( );
	}
	if ( phase == "CONNECTING" ) {
		SCENE scene = _data->getScene( );
		switch ( scene ) {
		case CONNECT: matching( );
		case BATTLE	: battle( );
		}
	}
}

void Console::ready( ) {
	if ( _data->getScene( ) != CONNECT ) {
		_data->setScene( CONNECT );
		return;
	}

	std::string finding = "- Connect Server -";
	std::string ip = _client->getSeverIP( );
	int size_rate = _drawer->getStringH( Drawer::BIG );
	_drawer->setString( true, WIDTH / 2, HEIGHT / 2 - size_rate, YELLOW, finding, Drawer::BIG );
	_drawer->setString( true, WIDTH / 2, HEIGHT / 2, WHITE, ip );
}

void Console::matching( ) {
	std::string matching = "- Matching -";
	_drawer->setString( true, WIDTH / 2, HEIGHT / 2 - 60, YELLOW, matching, Drawer::BIG );

	checkRecvTcp( );
}

void Console::battle( ) {
	if ( checkRecvTcp( ) ) {
		return;
	}
	_recving_udp = false;
	if ( _client->isRecvingUdp( ) ) {
		_recv_data_udp = _client->getDataUdp( );
		_recving_udp = true;
	}
}

bool Console::checkRecvTcp( ) {
	if ( !_client->isRecvingTcp( ) ) {
		return false;
	}
	SCENE scene = _client->getDataTcp( );
	_data->setScene( scene );
	return true;
}

bool Console::isRecvingData( ) const {
	return _recving_udp;
}

Client::NetWorkData Console::getData( ) {
	return _recv_data_udp;
}