#include "PhaseJudge.h"
#include "GlobalData.h"
#include "Client.h"
#include "Field.h"
#include "Sound.h"
#include "Soundplayer.h"

PhaseJudge::PhaseJudge( GlobalDataPtr data, FieldPtr field, int player_num ) :
Phase( player_num ),
_data( data ),
_field( field ) {
	_client = _data->getClientPtr( );
	_soundplayer = _data->getSoundplayerPtr( );

	_recv = false;
	_turn_fin = false;
	_cause = CAUSE_OF_DEATH( );

	SoundPtr sound_ptr = _data->getSoundPtr( );
	{//Winner SE
		_win_se = Base::SoundProperty( );
		_win_se.isLoop = false;
		_win_se.top = true;
		_win_se.wav = sound_ptr->getWav( EFFECT_SOUND, WIN_SE ).wav;
	}

	{//Loser SE
		_lose_se = Base::SoundProperty( );
		_lose_se.isLoop = false;
		_lose_se.top = true;
		_lose_se.wav = sound_ptr->getWav( EFFECT_SOUND, LOSE_SE ).wav;
	}
}

PhaseJudge::~PhaseJudge( ) {
}

void PhaseJudge::update( ) {
	recv( );

	if ( !_recv ) {
		return;
	}
	if ( _turn_fin ) {
		return;
	}

	short int winner = _client->getWinner( );

	if ( winner == _player_num ) {
		//Ÿ—˜
		_data->setScene( RESULT );
		_soundplayer->play( _win_se );
		_win = true;
		_cause = _client->getCauseOfDeath( );
	} else if ( winner == -1 ) {
		//Ÿ”s‚È‚µ
		_field->mirrorPosNotSelected( );
		_order = -1;
		_field->nextTurn( );
		_turn++;
		_field->setTurn( _turn );
	} else {
		//•‰‚¯
		_data->setScene( RESULT );
		_soundplayer->play( _lose_se );
		_win = false;
		_cause = _client->getCauseOfDeath( );
	}

	_turn_fin = true;
	_client->sendTcp( );
}

void PhaseJudge::recv( ) {
	if ( !_client->isRecvingUdp( ) ) {
		return;
	}
	_recv = true;
}

bool PhaseJudge::isWin( ) const {
	return _win;
}

CAUSE_OF_DEATH PhaseJudge::getCauseOfDeath( ) const {
	return _cause;
}