#include "SetMirror.h"
#include "GlobalData.h"
#include "Client.h"
#include "Debug.h"

SetMirror::SetMirror( GlobalDataPtr data ) :
_data( data ) {
}

SetMirror::~SetMirror( ) {
}

std::string SetMirror::getTag( ) {
	return "SETMIRROR";
}

void SetMirror::initialize( ) {
	setFlag( 1 );
	_client = _data->getClientPtr( );
	_player_num = 0;
	_phase = SET_PLAYER_PHASE;
	_client->setFinish( false );
}

void SetMirror::update( ) {
	if ( _client->getPhase( ) != "CONNECTING" ) {
		return;
	}
	if ( _player_num < 1 ) {
		_player_num = _client->getPlayerNum( );
	}

	_phase = _client->getBattlePhase( );
	switch ( _phase ) {
	case SET_PLAYER_PHASE	: updatePlayerPhase( )	; break;
	case SET_MIRROR_PHASE	: updateMirrorPhase( )	; break;
	case ATTACK_PHASE		: updateAttackPhase( )	; break;
	}

	DebugPtr debug = _data->getDebugPtr( );
	if ( debug->getFlag( ) ) {
		debug->addLog( "Player Num : " + std::to_string( _player_num ) );
		debug->addLog( " Order Num : " + std::to_string( _client->getOrder( ) ) );
	}
}

void SetMirror::updatePlayerPhase( ) {

}

void SetMirror::updateMirrorPhase( ) {
	if ( _client->getOrder( ) != _player_num ) {
		return;
	}
	if ( _data->getKeyState( KEY_INPUT_RETURN ) == 1 ) {
		_client->setFinish( true );
		_client->sendTcp( );
	}
}

void SetMirror::updateAttackPhase( ) {
	initialize( );
}