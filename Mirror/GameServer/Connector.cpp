#include "Connector.h"
#include "GlobalData.h"
#include "Table.h"
#include <string.h>

Connector::Connector( GlobalDataPtr data ) :
_data( data ) {
	initialize( );
}

Connector::~Connector( ) {
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

	_table = TablePtr( new Table( ) );
	_table->add( "Connection Waiting" );
}

void Connector::update( ) {
	updateConnectState( );

	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		switch ( _connect_state[ i ] ) {
		case NOT_CONNECTING	: NotConnecting( ); break;
		case CONNECTING		: Connecting( )	; break;
		}
	}

	_table->update( );
}

void Connector::updateConnectState( ) {
	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		if ( _server->isConnecting( i ) ) {
			if ( _connect_state[ i ] != NOT_CONNECTING ) {
				continue;
			}
			_connect_state[ i ] = CONNECTING;
			std::string log = "Machine " + std::to_string( i ) + "is Connected";
			_table->add( log );
			continue;
		}		
		_connect_state[ i ] = NOT_CONNECTING;
	}
}

void Connector::NotConnecting( ) {

}

void Connector::Connecting( ) {
	//ëóêM
	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		if ( _connect_state[ i ] != CONNECTING ) {
			continue;
		}
		Client::NetWorkData send_data;
		send_data.test = 1;
		_server->sendDataTcp( i, send_data );
	}

	//if ( _data->getKeyState( KEY_INPUT_RETURN ) == 1 ) {
	//	Client::NetWorkData send_data;
	//	send_data.test = 2;
	//	_server->sendDataUdp( send_data );
	//	_table->add( "send udp" );
	//}

	//éÛêM
	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		if ( _connect_state[ i ] != CONNECTING ) {
			continue;
		}
		if ( !_server->isRecving( i ) ) {
			continue;
		}
		Client::NetWorkData recv_data;
		recv_data = _server->getData( i );
		std::string log = "Recv from " + std::to_string( i ) + 
			"Machine" + "\"" + std::to_string( recv_data.test ) + "\"";

		_table->add( log );
	}
}