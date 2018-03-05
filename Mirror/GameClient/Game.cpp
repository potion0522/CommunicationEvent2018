#include "Game.h"
#include "GlobalData.h"
#include "Client.h"
#include "Debug.h"
#include "Image.h"
#include "Sound.h"
#include "Cutin.h"
#include "Field.h"
#include "Soundplayer.h"
#include "PhaseSetPlayer.h"
#include "PhaseSetMirror.h"
#include "PhaseAttack.h"
#include "PhaseJudge.h"

const int CUTIN_SPEED = 20;
const int WAIT_TIME = 90;

const int PHASE_CUTIN_IDX = 0;
const int PLAYER_CUTIN_IDX = 3;
const int ITEM_CUTIN_IDX = 5;

const short int bgm_no = 0;

Game::Game( GlobalDataPtr data ) :
_data( data ) {
	setFlag( 1 );
	_field = _data->getFieldPtr( );
	_client = _data->getClientPtr( );
	_cutin = CutinPtr( new Cutin( _data ) );

	{//bgm
		SoundPtr sound_ptr = _data->getSoundPtr( );
		_bgm = SoundProperty( );
		_bgm.isLoop = true;
		_bgm.top = true;
		_bgm.wav = sound_ptr->getWav( GAME_BGM, bgm_no ).wav;
	}
}

Game::~Game( ) {
}

std::string Game::getTag( ) {
	return "GAME";
}

void Game::initialize( ) {
	_turn = 1;
	_win = false;
	_player_num = _client->getPlayerNum( );
	_phase = SET_PLAYER_PHASE;
 	_field->setPhase( _phase );
	_order = -1;

	changePhase( );

	_field->setPlayerNum( _client->getPlayerNum( ) );
}

void Game::finalize( ) {
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

	if ( !_soundplayer->isPlaying( _bgm ) ) {
		_soundplayer->play( _bgm );
	}

	//プレイヤーナンバー取得
	_player_num = _client->getPlayerNum( );
	if ( _player_num != -1 ) {
		_field->setPlayerNum( _player_num );
	}

	//オーダーナンバー取得
	_order = _client->getOrder( );

	//ターン取得
	_turn = _field->getTurn( );

	//アップデート
	_cutin->update( );
	_battle->setOrder( _order );
	_battle->setTurn( _turn );
	_battle->update( );

	//終了判定がでたら
	if ( _data->getScene( ) == RESULT ) {
		_soundplayer->stop( _bgm );
		PhaseJudgePtr judge = std::dynamic_pointer_cast< PhaseJudge >( _battle );
		_win = judge->isWin( );
	}

	//フェイズを取得
	BATTLE_PHASE phase = _client->getBattlePhase( );
	if ( _phase != phase ) {
		_phase = phase;
 		_field->setPhase( _phase );
		changePhase( );
	}
}

void Game::changePhase( ) {
	switch ( _phase ) {
	case SET_PLAYER_PHASE:
		_battle = PhaseSetPlayerPtr( new PhaseSetPlayer( _data, _field, _cutin, _player_num ) );
		_cutin->setImage( Cutin::CUTIN_TYPE_PHASE, Cutin::CUTIN_PHASE_SET_PLAYER );
		break;
	case SET_MIRROR_PHASE:
		_battle = PhaseSetMirrorPtr( new PhaseSetMirror( _data, _field, _cutin, _player_num ) );
		_cutin->setImage( Cutin::CUTIN_TYPE_PHASE, Cutin::CUTIN_PHASE_SET_MIRROR );
		break;
	case ATTACK_PHASE:
		_battle = PhaseAttackPtr( new PhaseAttack( _data, _field, _cutin, _player_num, _field->isReverseFlag( ) ) );
		_cutin->setImage( Cutin::CUTIN_TYPE_PHASE, Cutin::CUTIN_PHASE_ATTACK );
		break;
	case JUDGE_PHASE:
		_battle = PhaseJudgePtr( new PhaseJudge( _data, _field, _player_num ) );
		break;
	}
}

bool Game::isWin( ) const {
	return _win;
}