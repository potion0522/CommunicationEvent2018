#include "Connector.h"
#include "GlobalData.h"
#include "Log.h"
#include <string.h>

Connector::Connector( GlobalDataPtr data, LogPtr log ) :
_data( data ),
_log( log ) {
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
	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		_connect_state[ i ] = NOT_CONNECTING;
	}
	_matching = false;
	_sending_state = false;
	_server_scene = CONNECT;
	_log->add( "Connection Waiting" );
}

void Connector::update( ) {
	updateConnectState( );
	updateMatchingState( );
	sendState( );

	_log->update( );
}

void Connector::updateConnectState( ) {
	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		std::string log = "";
		if ( !_server->isConnecting( i ) ) {
			//ロストしたとき
			if ( _connect_state[ i ] == CONNECTING ) {
				log = "DisConnect Machine [ " + std::to_string( i ) + " ]";
				_log->add( log );
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
		_log->add( log );
	}
}

void Connector::updateMatchingState( ) {
	//クライアントにマッチング相手が見つかったことを教える
	_matching = false;
	int check = 0;
	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		if ( _connect_state[ i ] == CONNECTING ) {
			check++;
			continue;
		}
	}

	if ( check == MACHINE_MAX ) {
		_matching = true;
	}
}

void Connector::sendState( ) {
	if ( _sending_state == _matching ) {
		return;
	}
	_server->sendDataTcp( _matching );
	_sending_state = _matching;
}

bool Connector::isMatching( ) const {
	return _matching;
}