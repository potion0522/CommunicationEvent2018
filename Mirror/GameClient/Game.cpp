#include "Game.h"
#include "GlobalData.h"
#include "Client.h"
#include "Debug.h"
#include "Field.h"

Game::Game( GlobalDataPtr data ) :
_data( data ) {
	_field = _data->getFieldPtr( );
}

Game::~Game( ) {
}

std::string Game::getTag( ) {
	return "GAME";
}

void Game::initialize( ) {
	setFlag( 1 );
	_client = _data->getClientPtr( );
	_player_num = 0;
	_phase = SET_PLAYER_PHASE;
	_client->setFinish( false );
}

void Game::update( ) {
	if ( _client->getPhase( ) != "CONNECTING" ) {
		return;
	}
	if ( _field->getPhase( ) != _phase ) {
		_field->setPhase( _phase );
	}

	if ( _player_num < 1 ) {
		_player_num = _client->getPlayerNum( );
		_field->setPlayerNum( _player_num );
	}

	_phase = _client->getBattlePhase( );
	switch ( _phase ) {
	case SET_PLAYER_PHASE	: updatePlayerPhase( )	; break;
	case SET_MIRROR_PHASE	: updateMirrorPhase( )	; break;
	case ATTACK_PHASE		: updateAttackPhase( )	; break;
	}

	DebugPtr debug = _data->getDebugPtr( );
	if ( debug->getFlag( ) ) {
		debug->addLog( "Phase      : " + std::to_string( ( int )_phase ) );
		debug->addLog( "Player Num : " + std::to_string( _player_num ) );
		debug->addLog( " Order Num : " + std::to_string( _client->getOrder( ) ) );
	}
}

void Game::updatePlayerPhase( ) {
	bool hit = false;
	if ( _data->getClickLeft( ) ) {
		hit = _field->isHitPlayerPos( );
	}
	if ( !hit ) {
		return;
	}
	int pos = _field->getPlayerPosHitNum( );
	if ( pos < 0 ) {
		return;
	}
	_field->setPlayerPoint( _player_num, pos );

	_client->setPlayerPos( pos );
	_client->sendTcp( );
}

void Game::updateMirrorPhase( ) {
	if ( _client->getOrder( ) != _player_num ) {
		return;
	}
	if ( _data->getKeyState( KEY_INPUT_RETURN ) == 1 ) {
		_client->sendTcp( );
	}
}

void Game::updateAttackPhase( ) {
	initialize( );
}