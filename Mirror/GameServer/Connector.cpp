#include "Connector.h"
#include "GlobalData.h"
#include "Log.h"
#include "Command.h"
#include "LoadCSV.h"
#include "Field.h"
#include <vector>
#include <string.h>

Connector::Connector( GlobalDataPtr data, LogPtr log, CommandPtr command ) :
_data( data ),
_log( log ),
_command( command ) {
	setFlag( 1 );
	_server = _data->getServerPtr( );
}

Connector::~Connector( ) {
}

std::string Connector::getTag( ) {
	return "CONNECTOR";
}

void Connector::initialize( ) {
	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		_connect_state[ i ] = NOT_CONNECTING;
	}
	_matching = false;
	_sending_state = false;
	_command_matching = false;
	_server_scene = CONNECT;
	_log->add( "Connection Waiting" );
}

void Connector::finalize( ) {
	if ( _server->getFlag( ) ) {
		_server->disConnect( );
	}
}

void Connector::update( ) {
	commandExecution( );
	updateConnectState( );
	if ( !_command_matching ) {
		updateMatchingState( );
		sendState( );
	}

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
				//バトル中であれば
				if ( _data->getScene( ) > CONNECT && _data->getScene( ) < RESULT ) {
					_data->setInitFlag( );
				}
			}
			_connect_state[ i ] = NOT_CONNECTING;

			continue;
		}

		if ( _connect_state[ i ] == CONNECTING ) {
			continue;
		}

		//初回接続のみ
		log = "Connect " + _server->getClientIpStr( i );
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

	float time = 0.0f;
	{//制限時間を読み込む
		LoadCSVPtr csv( new LoadCSV( ) );
		std::vector< CsvData > data;
		csv->read( data, "limit" );
		int size = ( int )data.size( );
		for ( int i = 0; i < size; i++ ) {
			if ( data[ i ].tag == "DEAD_COUNT" ) {
				time = ( float )atof( data[ i ].values.front( ).c_str( ) );
				break;
			}
		}

		FieldPtr field = _data->getFieldPtr( );
		field->setDeadCount( time );
	}

	//TCP送信
	_server->sendDataTcp( _matching, time );
	_sending_state = _matching;
	if ( _matching ) {
		_data->setScene( BATTLE );
	}
}

void Connector::commandExecution( ) {
	if ( _command->getWordNum( ) < 1 ) {
		return;
	}
	if ( _command->getWord( 0 ) == "MATCHING" ) {
		_command_matching = true;
		_matching = true;
		sendState( );
		_log->add( "change matching TRUE" );
	}
}

bool Connector::isMatching( ) const {
	return _matching;
}