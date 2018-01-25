#include "Game.h"
#include "GlobalData.h"
#include "Client.h"
#include "Debug.h"
#include "Field.h"

Game::Game( GlobalDataPtr data ) :
_data( data ) {
	_field = _data->getFieldPtr( );
	_client = _data->getClientPtr( );
	_turn = 1;
}

Game::~Game( ) {
}

std::string Game::getTag( ) {
	return "GAME";
}

void Game::initialize( ) {
	setFlag( 1 );
	_player_num = 0;
	_phase = SET_PLAYER_PHASE;
}

void Game::update( ) {
	if ( _client->getPhase( ) != "CONNECTING" ) {
		return;
	}
	BATTLE_PHASE phase = _client->getBattlePhase( );
	if ( _phase != phase ) {
		if ( _phase == ATTACK_PHASE && phase == SET_MIRROR_PHASE ) {
			_field->mirrorPosSelected( );
			_turn++;
		}
		_phase = phase;
	}

	if ( _field->getPhase( ) != _phase ) {
 		_field->setPhase( _phase );
	}

	_player_num = _client->getPlayerNum( );
	if ( _player_num != ( unsigned char )-1 ) {
		_field->setPlayerNum( _player_num );
	}

	switch ( _phase ) {
	case SET_PLAYER_PHASE	: updatePlayerPhase( )	; break;
	case SET_MIRROR_PHASE	: updateMirrorPhase( )	; break;
	case ATTACK_PHASE		: updateAttackPhase( )	; break;
	}

	DebugPtr debug = _data->getDebugPtr( );
	if ( debug->getFlag( ) ) {
		debug->addLog( "Phase      : " + std::to_string( ( int )_phase ) );
		debug->addLog( "Player Num : " + std::to_string( _player_num ) );
		debug->addLog( "Order Num  : " + std::to_string( _client->getOrder( ) ) );
	}
}

void Game::updatePlayerPhase( ) {
	bool hit = false;
	hit = _field->isHitPlayerPos( );

	if ( !hit ) {
		return;
	}
	int pos = _field->getPlayerPosHitNum( );
	if ( pos < 0 ) {
		return;
	}
	if ( !_data->getClickLeft( ) ) {
		return;
	}
	_field->playerPosSelected( );
	_field->setPlayerPoint( _player_num, pos );

	_client->setPlayerPos( pos );
	_client->sendTcp( );
}

void Game::updateMirrorPhase( ) {
	if ( !_client->isRecvingUdp( ) ) {
		return;
	}

	for ( int i = 0; i < PLAYER_NUM; i++ ) {
		int pos = _client->getPlayerPos( i );
		if ( pos == ( unsigned char )-1 ) {
			return;
		}
		_field->setPlayerPoint( i, pos );
	}

	int lazer_pos = _client->getLazerPoint( );
	if ( lazer_pos == ( unsigned char )-1 ) {
		return;
	}
	_field->setLazerPoint( lazer_pos );

	if ( _client->getOrder( ) != _player_num ) {
		return;
	}

	//
	bool hit = false;
	hit = _field->isHitFieldPos( );

	if ( !hit ) {
		return;
	}
	int pos = _field->getFieldPosHitNum( );
	if ( pos < 0 ) {
		return;
	}
	if ( !_data->getClickLeft( ) ) {
		return;
	}

	const int COL = 5;
	int x = pos % COL;
	int y = pos / COL;

	_field->mirrorPosSelected( );
	_field->setMirrorPoint( _player_num, x, y, RIGHT );

	_client->setCtsPlayerNum( );
	_client->setCtsAngle( RIGHT );
	_client->setCtsX( x );
	_client->setCtsY( y );
	_client->setCtsFlag( true );

	_client->sendTcp( );
}

void Game::updateAttackPhase( ) {
	if ( !_client->isRecvingUdp( ) ) {
		return;
	}

	if ( _field->getTurn( ) == _turn ) {
		return;
	}

	for ( int i = 0; i < PLAYER_NUM; i++ ) {
		int player_num = _client->getStcPlayerNum( i );
		int x = _client->getStcX( i );
		int y = _client->getStcY( i );
		MIRROR_ANGLE angle = _client->getStcAngle( i );
		if ( !_client->getStcFlag( i ) ) {
			continue;
		}
		_field->setMirrorPoint( player_num, x, y, angle );
	}
	_field->setTurn( _turn );

	//initialize( );
	//_field->initialize( );
}