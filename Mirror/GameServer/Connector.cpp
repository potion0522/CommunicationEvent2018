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
	_server_scene = CONNECT;
	_table = TablePtr( new Table( _data ) );
	_table->add( "Connection Waiting" );
}

void Connector::update( ) {
	if ( _data->getScene( ) == BATTLE ) {
		calcData( );
	}

	updateConnectState( );
	updateMatchingState( );
	sendScene( );

	_table->update( );
}

void Connector::updateConnectState( ) {
	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		std::string log = "";
		if ( !_server->isConnecting( i ) ) {
			//ロストしたとき
			if ( _connect_state[ i ] == CONNECTING ) {
				log = "DisConnect Machine [ " + std::to_string( i ) + " ]";
				_table->add( log );
			}
			_connect_state[ i ] = NOT_CONNECTING;
			continue;
		}

		if ( _connect_state[ i ] == CONNECTING ) {
			continue;
		}

		//初回接続のみ
		log = "Connect " + _server->getMachineIpStr( i );
		_connect_state[ i ] = CONNECTING;
		_table->add( log );
	}
}

void Connector::updateMatchingState( ) {
	//クライアントにマッチング相手が見つかったことを教える
	int check = 0;
	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		if ( _connect_state[ i ] == CONNECTING ) {
			check++;
			continue;
		}
	}

	if ( check == 0 ) {
		_data->setScene( CONNECT );
		return;
	}

	if ( check < MACHINE_MAX ) {
		_data->setScene( CONNECT );
		return;
	}

	if ( _data->getScene( ) != BATTLE ) {
		_data->setScene( BATTLE );
	}
}

void Connector::sendScene( ) {
	SCENE global_scene = _data->getScene( );
	if ( _server_scene != global_scene ) {
		_server_scene = global_scene;
		_server->sendDataTcp( _server_scene );
	}
}

void Connector::calcData( ) {
	//Client::NetWorkData send = Client::NetWorkData( );
	//send.test = 1;
	//_server->sendDataUdp( send );
}