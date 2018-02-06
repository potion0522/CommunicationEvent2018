#include "Game.h"
#include "GlobalData.h"
#include "Client.h"
#include "Debug.h"
#include "Lazer.h"
#include "Drawer.h"
#include "Image.h"

const int CUTIN_SPEED = 20;
const int WAIT_TIME = 90;

Game::Game( GlobalDataPtr data ) :
_data( data ) {
	setFlag( 1 );
	_field = _data->getFieldPtr( );
	_client = _data->getClientPtr( );
	_drawer = _data->getDrawerPtr( );
	_lazer = LazerPtr( new Lazer( _data ) );
}

Game::~Game( ) {
}

std::string Game::getTag( ) {
	return "GAME";
}

void Game::initialize( ) {
	_turn = 1;
	_player_selected = false;
	_mirror_phase_recv = false;
	_attack_phase_recv = false;
	_judge_phase_recv = false;
	_turn_finish = false;
	_send_live = false;
	_win = false;
	_phase_cutin = false;
	_player_num = 0;
	_phase = SET_PLAYER_PHASE;
	_tmp_mirror = Field::Mirror( );

	std::array< ImageProperty, CUTIN_MAX >( ).swap( _cutin );
	setCutin( );
}

void Game::update( ) {
	if ( _client->getPhase( ) != "CONNECTING" ) {
		return;
	}

	DebugPtr debug = _data->getDebugPtr( );
	if ( debug->getFlag( ) ) {
		debug->addLog( "Phase      : " + std::to_string( ( int )_phase ) );
		debug->addLog( "Player Num : " + std::to_string( _player_num ) );
		debug->addLog( "Order Num  : " + std::to_string( _client->getOrder( ) ) );
	}

	BATTLE_PHASE phase = _client->getBattlePhase( );
	if ( _phase != phase ) {
		_phase = phase;
		_phase_cutin = false;
		_turn_finish = false;
	}

	if ( _field->getPhase( ) != _phase ) {
 		_field->setPhase( _phase );
	}

	_player_num = _client->getPlayerNum( );
	if ( _player_num != ( unsigned char )-1 ) {
		_field->setPlayerNum( _player_num );
	}

	if ( !_phase_cutin ) {
		calcPhaseCutin( );
		drawPhaseCutin( );
		return;
	} else {
		setCutin( );
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
}
void Game::calcPhaseCutin( ) {
	if ( _phase > ATTACK_PHASE ) {
		_phase_cutin = true;
		return;
	}

	int idx = ( int )_phase;

	if ( _cutin[ idx ].cx < WIDTH / 2 ) {
		_cutin[ idx ].cx += CUTIN_SPEED;
	} else {
		if ( _cutin[ idx ].cnt < WAIT_TIME ) {
			_cutin[ idx ].cnt++;
			return;
		}
		_cutin[ idx ].cx += CUTIN_SPEED;

		if ( _cutin[ idx ].cx > WIDTH + _cutin[ idx ].lx ) {
			_phase_cutin = true;
		}
	}
}

void Game::drawPhaseCutin( ) const {
	if ( _phase_cutin ) {
		return;
	}
	_drawer->setImage( _cutin[ ( int )_phase ] );
}

void Game::setCutin( ) {
	ImagePtr image_ptr = _data->getImagePtr( );
	for ( int i = 0; i < CUTIN_MAX; i++ ) {
		Png image = image_ptr->getPng( CUTIN_IMAGE, i );
		_cutin[ i ].cx = image.width / 2 * -1;
		_cutin[ i ].cy = HEIGHT / 2;
		_cutin[ i ].lx = image.width / 2;
		_cutin[ i ].ly = image.height / 2;
		_cutin[ i ].png = image.png;
		_cutin[ i ].cnt = 0;
	}
}

void Game::selectPlayerPos( ) {
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
	_field->setTmpPlayerPoint( );
	_player_selected = true;
}

void Game::updatePlayerPhase( ) {
	_field->setInfoText( "あなたの配置をしてください" );
	selectPlayerPos( );

	if ( !_player_selected ) {
		return;
	}

	if ( !_field->isHitDecisionButton( ) ) {
		return;
	}

	if ( !_data->getClickLeft( ) ) {
		return;
	}

	if ( _field->isSelectedPlayer( ) ) {
		return;
	}

	_field->playerPosSelected( );
	_field->setPlayerPoint( _player_num, _field->getTmpPlayerPoint( ) );
	_client->setPlayerPos( _field->getPlayerPoint( _player_num ) );
	_client->sendTcp( );
}

void Game::inputTmpMirror( ) {
	if ( _client->getOrder( ) != _player_num ) {
		_field->setInfoText( "相手が鏡を配置しています", RED );
		return;
	}

	_field->setInfoText( "あなたのターンです", RED );
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
	_field->setInfoText( "鏡を配置してください。" );
	_field->setInfoText( "もう一度左クリックで向きを変えられます" );

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

	//ここにプレイヤーの生死判定
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
		//勝利
		_data->setScene( RESULT );
		_win = true;
	} else if ( winner == ( unsigned char )-1 ) {
		//勝敗なし
		_field->mirrorPosSelected( );
		_mirror_phase_recv = false;
		_attack_phase_recv = false;
		_judge_phase_recv = false;
		_send_live = false;
		_tmp_mirror = Field::Mirror( );
		if ( _turn % TURN_MAX == 0 ) {
			_field->nextTurn( );
		}
		_turn++;
		_field->setTurn( _turn );
		_lazer->clearLazerImage( );
	} else {
		//負け
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

	if ( _attack_phase_recv ) {
		return;
	}

	std::array< Field::Mirror, PLAYER_NUM > mirror;
	for ( int i = 0; i < PLAYER_NUM; i++ ) {
		mirror[ i ].player_num = _client->getStcPlayerNum( i );
		mirror[ i ].x = _client->getStcX( i );
		mirror[ i ].y = _client->getStcY( i );
		mirror[ i ].angle = _client->getStcAngle( i );
	}

	if ( mirror[ 0 ].x == mirror[ 1 ].x &&
		 mirror[ 0 ].y == mirror[ 1 ].y &&
		 mirror[ 0 ].angle == mirror[ 1 ].angle ) {
		_field->deleteMirrorPoint( mirror[ 0 ].x + mirror[ 0 ].y * FIELD_COL );
	} else {
		_field->setMirrorPoint( mirror[ 0 ].player_num, mirror[ 0 ].x, mirror[ 0 ].y, mirror[ 0 ].angle );
		_field->setMirrorPoint( mirror[ 1 ].player_num, mirror[ 1 ].x, mirror[ 1 ].y, mirror[ 1 ].angle );
	}

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