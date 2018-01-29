#include "Game.h"
#include "GlobalData.h"
#include "Client.h"
#include "Debug.h"
#include "Lazer.h"
#include "Drawer.h"

Game::Game( GlobalDataPtr data ) :
_data( data ) {
	setFlag( 1 );
	_field = _data->getFieldPtr( );
	_client = _data->getClientPtr( );
	_lazer = LazerPtr( new Lazer( _data ) );
}

Game::~Game( ) {
}

std::string Game::getTag( ) {
	return "GAME";
}

void Game::initialize( ) {
	_turn = 1;
	_mirror_phase_recv = false;
	_attack_phase_recv = false;
	_judge_phase_recv = false;
	_turn_finish = false;
	_send_live = false;
	_win = false;
	_player_num = 0;
	_phase = SET_PLAYER_PHASE;
	_tmp_mirror = Field::Mirror( );
}

void Game::update( ) {
	if ( _client->getPhase( ) != "CONNECTING" ) {
		return;
	}
	BATTLE_PHASE phase = _client->getBattlePhase( );
	if ( _phase != phase ) {
		_phase = phase;
		_turn_finish = false;
	}

	if ( _field->getPhase( ) != _phase ) {
 		_field->setPhase( _phase );
	}

	_player_num = _client->getPlayerNum( );
	if ( _player_num != ( unsigned char )-1 ) {
		_field->setPlayerNum( _player_num );
	}

	switch ( _phase ) {
	case SET_PLAYER_PHASE:
		updatePlayerPhase( );
		break;
	case SET_MIRROR_PHASE: 
		recvMirrorPhase( );
		if ( _mirror_phase_recv ) {
			updateMirrorPhase( );
		}
		break;
	case ATTACK_PHASE:
		recvAttackPhase( );
		if ( _attack_phase_recv ) {
			updateAttackPhase( );
		}
		break;
	case JUDGE_PHASE:
		if ( _turn_finish ) {
			break;
		}
		recvJudgePhase( );
		if ( _judge_phase_recv ) {
			updateJudgePhase( );
		}
		break;
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

void Game::inputTmpMirror( ) {
	if ( _client->getOrder( ) != _player_num ) {
		return;
	}
	
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
	
	int x = pos % FIELD_COL;
	int y = pos / FIELD_COL;
	if ( _tmp_mirror.x == x && _tmp_mirror.y == y ) {
		_tmp_mirror.angle = ( MIRROR_ANGLE )( ( int )( _tmp_mirror.angle + 1 ) % ( int )MIRROR_ANGLE_MAX );
	} else {
		_tmp_mirror.angle = RIGHT;
	}

	_tmp_mirror.x = x;
	_tmp_mirror.y = y;
	_tmp_mirror.flag = true;

	_field->setTmpMirrorPoint( _player_num, _tmp_mirror.x, _tmp_mirror.y, _tmp_mirror.angle );
}

void Game::updateMirrorPhase( ) {
	inputTmpMirror( );

	if ( !_tmp_mirror.flag ) {
		return;
	}

	bool hit = false;
	hit = _field->isHitDecisionButton( );

	if ( !hit ) {
		return;
	}

	if ( !_data->getClickLeft( ) ) {
		return;
	}
	_field->setMirrorPoint( _player_num, _tmp_mirror.x, _tmp_mirror.y, _tmp_mirror.angle );

	_field->mirrorPosSelected( );

	_client->setCtsPlayerNum( );
	_client->setCtsAngle( _tmp_mirror.angle );
	_client->setCtsX( _tmp_mirror.x );
	_client->setCtsY( _tmp_mirror.y );
	_client->setCtsFlag( true );

	_client->sendTcp( );

	_tmp_mirror = Field::Mirror( );
	_lazer->initialize( );
}

void Game::updateAttackPhase( ) {
	_lazer->update( );
	if ( !_lazer->isFinish( ) ) {
		return;
	}
	if ( _send_live ) {
		return;
	}

	//‚±‚±‚ÉƒvƒŒƒCƒ„[‚Ì¶Ž€”»’è
	int dead = _field->getDeadPlayer( );
	bool alive = true;
	if ( dead == _player_num ) {
		alive = !alive;
	}

	_client->setAlive( alive );
	_client->sendTcp( );
	_send_live = true;
}

void Game::updateJudgePhase( ) {
	int winner = _client->getWinner( );

	if ( winner == _player_num ) {
		//Ÿ—˜
		_data->setScene( RESULT );
		_win = true;
	} else if ( winner == ( unsigned char )-1 ) {
		//Ÿ”s‚È‚µ
		_field->mirrorPosSelected( );
		_turn++;
		_mirror_phase_recv = false;
		_attack_phase_recv = false;
		_judge_phase_recv = false;
		_send_live = false;
		_tmp_mirror = Field::Mirror( );

		if ( _turn > TURN_MAX ) {
			_field->nextRound( );
			_turn = 1;
		}
	} else {
		//•‰‚¯
		_data->setScene( RESULT );
		_win = false;
	}

	_turn_finish = true;
	_client->sendTcp( );
}

void Game::recvMirrorPhase( ) {
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

	_mirror_phase_recv = true;
}

void Game::recvAttackPhase( ) {
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

	_attack_phase_recv = true;
}

void Game::recvJudgePhase( ) {
	if ( !_client->isRecvingUdp( ) ) {
		return;
	}
	_judge_phase_recv = true;
}

bool Game::isWin( ) const {
	return _win;
}