#include "PhaseJudge.h"
#include "GlobalData.h"
#include "Client.h"
#include "Field.h"

PhaseJudge::PhaseJudge( GlobalDataPtr data, FieldPtr field, int player_num ) :
Phase( player_num ),
_data( data ),
_field( field ) {
	_client = _data->getClientPtr( );

	_recv = false;
	_turn_fin = false;
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
		_win = true;
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
		_win = false;
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