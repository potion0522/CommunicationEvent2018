#include "PhaseAttack.h"
#include "Field.h"
#include "GlobalData.h"
#include "Client.h"
#include "Client.h"
#include "Cutin.h"
#include "Lazer.h"

const int WAIT_TIME = 30;

PhaseAttack::PhaseAttack( GlobalDataPtr data, FieldPtr field, CutinPtr cutin, short int player_num, bool reverse_flag ) :
Phase( player_num ),
_data( data ),
_field( field ),
_cutin( cutin ) {
	_client = _data->getClientPtr( );
	_lazer = LazerPtr( new Lazer( _data ) );
	_recv = false;
	_send_live = false;
	_wait_time = 0;
	_reverse_mirror = reverse_flag;
}

PhaseAttack::~PhaseAttack( ) {
}

void PhaseAttack::update( ) {
	if ( _cutin->isCutin( ) ) {
		return;
	}

	recv( );
	calc( );
}

void PhaseAttack::calc( ) {
	if ( _reverse_mirror ) {
		_field->reverseMirror( );
		_reverse_mirror = false;
		_cutin->setImage( Cutin::CUTIN_TYPE_ITEM, Cutin::CUTIN_ITEM_REVERSE_MIRROR );
	}

	if ( _cutin->isCutin( ) ) {
		return;
	}

	if ( _wait_time < WAIT_TIME ) {
		_wait_time++;
		return;
	}

	_lazer->update( );
	if ( !_lazer->isFinish( ) ) {
		return;
	}
	if ( _send_live ) {
		return;
	}

	//ここにプレイヤーの生死判定
	CAUSE_OF_DEATH cause = CAUSE_OF_DEATH( );
	int dead = _field->getDeadPlayer( );
	bool alive = true;

	//倒されたプレイヤーが自分ならば
	if ( dead == _player_num ) {
		alive = false;
	}
	if( _field->getDeadCount( ) <= 0 ) {
		alive = false;
		cause = CAUSE_TIME;
		if ( dead != _player_num && dead != -1 ) {
			alive = true;
		}
	}

	_client->setAlive( alive );
	_client->setCauseOfDeath( cause );
	_client->sendTcp( );
	_send_live = true;
}

void PhaseAttack::recv( ) {
	if ( !_client->isRecvingUdp( ) ) {
		return;
	}

	if ( _recv ) {
		return;
	}

	std::array< Field::Mirror, PLAYER_NUM > mirror;
	for ( int i = 0; i < PLAYER_NUM; i++ ) {
		mirror[ i ].flag = _client->getStcFlag( i );
		mirror[ i ].player_num = _client->getStcPlayerNum( i );
		mirror[ i ].x = _client->getStcX( i );
		mirror[ i ].y = _client->getStcY( i );
		mirror[ i ].angle = _client->getStcAngle( i );
	}

	//鏡の配置
	bool del = false;
	if ( mirror[ 0 ].x     == mirror[ 1 ].x     &&
		 mirror[ 0 ].y     == mirror[ 1 ].y     &&
		 mirror[ 0 ].angle == mirror[ 1 ].angle ) {
		del = true;
	}

	if ( del ) {
		_field->deleteMirrorPoint( mirror[ 0 ].x + mirror[ 0 ].y * FIELD_COL );
	} else {
		for ( int i = 0; i < PLAYER_NUM; i++ ) {
			if ( mirror[ i ].flag ) {
				_field->setMirrorPoint( mirror[ i ].player_num, mirror[ i ].x, mirror[ i ].y, mirror[ i ].angle );
			}
		}
	}

	_recv = true;
}