#include "GameMaster.h"
#include "GlobalData.h"
#include "Connector.h"
#include "Server.h"
#include "Log.h"
#include "Field.h"
#include <random>
#include <map>

GameMaster::GameMaster( GlobalDataPtr data, ConnectorPtr connector, LogPtr log ) :
_data( data ),
_connector( connector ),
_log( log ) {
	_field = FieldPtr( new Field( _data ) );
	_server = _data->getServerPtr( );
	setFlag( 1 );
}

GameMaster::~GameMaster( ) {
}

std::string GameMaster::getTag( ) {
	return "GAMEMASTER";
}

void GameMaster::initialize( ) {
	_matching = false;
	_dice = false;
	std::array< Data, PLAYER_NUM >( ).swap( _client_data );
	_phase = SET_PLAYER_PHASE;
	_server->setBattlePhase( _phase );
}

void GameMaster::update( ) {
	bool matching = _connector->isMatching( );
	if ( _matching != matching ) {
		_matching = matching;
		if ( !_matching ) {
			initialize( );
		}
	}

	if ( !_matching ) {
		return;
	}

	if ( !_dice ) {
		orderPlayer( );
	}

	switch ( _phase ) {
	case SET_PLAYER_PHASE	: updatePlayerPhase( ); break;
	case SET_MIRROR_PHASE	: updateMirrorPhase( ); break;
	case ATTACK_PHASE		: updateAttackPhase( ); break;
	}

	_server->sendDataUdp( );

	switch ( _phase ) {
	case SET_PLAYER_PHASE	: inputPlayerPhase( ); break;
	case SET_MIRROR_PHASE	: inputMirrorPhase( ); break;
	case ATTACK_PHASE		: inputAttackPhase( ); break;
	}
}

void GameMaster::orderPlayer( ) {
	std::random_device rd;
	int idx = rd( ) % 2;
	_client_data[ idx ].order = 1;
	idx = ( idx + 1 ) % 2;
	_client_data[ idx ].order = 2;
	_dice = true;
}

int GameMaster::getWaitingIdx( ) const {
	int idx = -1;
	for ( int i = 0; i < PLAYER_NUM; i++ ) {
		if ( _client_data[ i ].fin ) {
			continue;
		}
		if ( idx != -1 && _client_data[ i ].order - 1 > idx ) {
			continue;
		}
		idx = i;
	}
	return idx;
}

int GameMaster::getOrderIdx( int order ) const {
	int idx = -1;
	for ( int i = 0; i < PLAYER_NUM; i++ ) {
		if ( _client_data[ i ].order != order ) {
			continue;
		}
		idx = i;
		break;
	}
	return idx;
}

void GameMaster::updatePlayerPhase( ) {
	int idx = getWaitingIdx( );
	_server->setOrder( -1 );

	if ( idx != -1 ) {
		return;
	}

	_phase = SET_MIRROR_PHASE;
	_server->setBattlePhase( _phase );

	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		_field->setPlayerPoint( i, _client_data[ i ].player_pos );
		_server->setPlayerPos( i, _client_data[ i ].player_pos );
		_client_data[ i ].fin = false;
	}

	int dif = 0;
	int player_one = _server->getPlayerPos( 0 );
	int player_two = _server->getPlayerPos( 1 );
	std::map< int, int > point;
	for ( int i = 0; i < PLAYER_POSITION * 2; i++ ) {
		point[ i ] = i;
	}

	point.erase( player_one );
	point.erase( player_two );

	int rand = _data->getRandom( 0, PLAYER_POSITION * 2 - 2 );
	_server->setLazerPos( point[ rand ] );
}

void GameMaster::updateMirrorPhase( ) {
	int idx = getWaitingIdx( );

	_server->setOrder( idx );

	if ( idx != -1 ) {
		return;
	}

	_phase = ATTACK_PHASE;
	_server->setBattlePhase( _phase );

	idx = getOrderIdx( 1 );
	Data data = _client_data[ idx ];
	_field->setMirrorPoint( idx, data.x, data.y, data.angle );
	data = _client_data[ ( idx + 1 ) % PLAYER_NUM ];
	_field->setMirrorPoint( idx, data.x, data.y, data.angle );

	for ( int i = 0; i < PLAYER_NUM; i++ ) {
		_client_data[ i ].fin = false;
	}
}

void GameMaster::updateAttackPhase( ) {
	initialize( );
}

void GameMaster::inputPlayerPhase( ) {
	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		if ( _server->isRecving( i ) ) {
			_client_data[ i ].player_pos = _server->getPlayerPos( i );
			_client_data[ i ].fin = true;
		}
	}
}

void GameMaster::inputMirrorPhase( ) {
	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		if ( _server->isRecving( i ) ) {
			_client_data[ i ].x = _server->getCtsX( i );
			_client_data[ i ].y = _server->getCtsY( i );
			_client_data[ i ].angle = _server->getCtsAngle( i );
			_client_data[ i ].fin = true;
		}
	}
}

void GameMaster::inputAttackPhase( ) {

}