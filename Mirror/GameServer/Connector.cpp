#include "Connector.h"
#include "GlobalData.h"
#include "Table.h"
#include <string.h>

Connector::Connector( GlobalDataPtr data ) :
_data( data ) {
}

Connector::~Connector( ) {
	if ( _server->getFlag( ) ) {
		_server->disConnect( );
	}
}

std::string Connector::getTag( ) {
	return "CONNECTOR";
}

void Connector::initialize( ) {
	setFlag( 1 );
	_server = _data->getServerPtr( );
	memset( _client_data, 0, sizeof( Client::NetWorkData ) * MACHINE_MAX );	
	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		_connect_state[ i ] = NOT_CONNECTING;
	}
	_matching = false;

	_table = TablePtr( new Table( _data ) );
	_table->add( "Connection Waiting" );
}

void Connector::update( ) {
	updateConnectState( );

	//クライアントにマッチング相手が見つかったことを教える
	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		_matching = false;
		if ( _connect_state[ i ] != CONNECTING ) {
			break;
		}
		//シーンがCONNECTのときのみ
		if ( _data->getScene( ) != CONNECT ) {
			break;
		}
		Client::NetWorkData call = Client::NetWorkData( );
		_server->sendDataTcp( i, call );
		_matching = true;
	}

	if ( _matching ) {
		connecting( );
	}

	_table->update( );
}

void Connector::updateConnectState( ) {
	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		std::string log = "";
		if ( !_server->isConnecting( i ) ) {
			if ( _connect_state[ i ] == CONNECTING ) {
				_connect_state[ i ] = NOT_CONNECTING;
				log = "DisConnect Machine [ " + std::to_string( i ) + " ]";
				_table->add( log );
			}
			continue;
		}
		if ( _connect_state[ i ] != NOT_CONNECTING ) {
			continue;
		}

		log = "Connect " + _server->getMachineIpStr( i );
		_connect_state[ i ] = CONNECTING;
		_table->add( log );

	}
}

void Connector::notConnecting( ) {

}

void Connector::connecting( ) {
	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		Client::NetWorkData send = Client::NetWorkData( );
		_server->sendDataUdp( send );
	}
}