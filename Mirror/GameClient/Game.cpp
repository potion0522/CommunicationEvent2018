#include "Game.h"
#include "GlobalData.h"
#include "Client.h"
#include "Debug.h"
#include "Lazer.h"
#include "Drawer.h"
#include "Image.h"

const int CUTIN_SPEED = 20;
const int WAIT_TIME = 90;

const int PHASE_CUTIN_IDX = 0;
const int PLAYER_CUTIN_IDX = 3;
const int ITEM_CUTIN_IDX = 5;

Game::Game( GlobalDataPtr data ) :
_data( data ) {
	setFlag( 1 );
	_field = _data->getFieldPtr( );
	_client = _data->getClientPtr( );
	_drawer = _data->getDrawerPtr( );
	_lazer = LazerPtr( new Lazer( _data ) );

	_cutin_image = ImageProperty( );

	ImagePtr image_ptr = _data->getImagePtr( );
	{//ハンドルを取得
		//フェーズカットイン
		for ( int i = 0; i < PHASE_CUTIN_MAX; i++ ) {
			_cutin_phase_handles[ i ] = image_ptr->getPng( CUTIN_STRING_IMAGE, PHASE_CUTIN_IDX + i ).png;
		}

		//プレイヤーカットイン
		for ( int i = 0; i < PLAYER_CUTIN_MAX; i++ ) {
			_cutin_player_handles[ i ] = image_ptr->getPng( CUTIN_STRING_IMAGE, PLAYER_CUTIN_IDX + i ).png;
		}

		//アイテムカットイン
		for ( int i = 0; i < ITEM_CUTIN_MAX; i++ ) {
			_cutin_item_handles[ i ] = image_ptr->getPng( CUTIN_STRING_IMAGE, ITEM_CUTIN_IDX + i ).png;
		}

		//バックイメージ
		for ( int i = 0; i < CUTIN_BACK_MAX; i++ ) {
			_background_cutin_handles[ i ] = image_ptr->getPng( CUTIN_BACK_IMAGE, i ).png;
		}
	}
	_background_cutin_image = ImageProperty( );
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
	_cutin = false;
	_change_phase = true;
	_use_item = false;
	_item_callback = false;
	_double_mirror = false;
	_reverse_mirror = false;
	_player_num = 0;
	_cutin_spd_rate = 1.0f;
	_item = 0;
	_phase = SET_PLAYER_PHASE;
	_tmp_mirror = Field::Mirror( );
	_order = ( unsigned char )-1;
	_clicking = 0;

	resetBackCutin( );
	resetStringCutin( );

	_field->setPlayerNum( _client->getPlayerNum( ) );
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

	//フェイズを取得
	BATTLE_PHASE phase = _client->getBattlePhase( );
	if ( _phase != phase ) {
		resetBackCutin( );
		resetStringCutin( );
		_phase = phase;
		_change_phase = true;
		_cutin = false;
		_turn_finish = false;
	}

	if ( _field->getPhase( ) != _phase ) {
 		_field->setPhase( _phase );
	}

	_player_num = _client->getPlayerNum( );
	if ( _player_num != ( unsigned char )-1 ) {
		_field->setPlayerNum( _player_num );
	}

	//フェイズカットイン
	if ( _phase < JUDGE_PHASE && !_cutin && _change_phase ) {
		_cutin_image.png = _cutin_phase_handles[ ( int )_phase ];
		_background_cutin_image.png = _background_cutin_handles[ ( int )CUTIN_BACK_NORMAL ];
		drawBackCutin( );
		drawStringCutin( );
		calcBackCutin( );
		calcStringCutin( );
		return;
	}

	//アイテムチェック
	checkItemFlag( );
	if ( _use_item ) {
		if ( !_cutin && _item != REVERSE_MIRROR ) {
			//アイテムカットイン
			_cutin_spd_rate = 1.8f;
			_cutin_image.png = _cutin_item_handles[ _item ];
			_background_cutin_image.png = _background_cutin_handles[ ( int )CUTIN_BACK_ITEM ];
			drawBackCutin( );
			drawStringCutin( );
			calcBackCutin( );
			calcStringCutin( );
			return;
		}

		invocationItem( );
		return;
	}

	switch ( _phase ) {
	case SET_PLAYER_PHASE:
		updatePlayerPhase( );
		break;
	case SET_MIRROR_PHASE: 
		recvMirrorPhase( );
		_field->setOrder( _order );
		if ( _mirror_phase_recv ) {
			updateMirrorPhase( );
			updateItemCalc( );
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

void Game::calcBackCutin( ) {
	if ( _background_cutin_image.cx < WIDTH / 2 ) {
		_background_cutin_image.cx += CUTIN_SPEED * _cutin_spd_rate;
	} else {
		if ( _background_cutin_image.cnt < WAIT_TIME ) {
			_background_cutin_image.cx = WIDTH / 2;
			_background_cutin_image.cnt++;
			return;
		}
		_background_cutin_image.cx += CUTIN_SPEED * _cutin_spd_rate;

		if ( _background_cutin_image.cx > WIDTH + _background_cutin_image.lx ) {
			_cutin = true;
			_change_phase = false;
			_cutin_spd_rate = 1.0f;
			resetBackCutin( );
		}
	}
}

void Game::calcStringCutin( ) {
	if ( _cutin_image.cx < WIDTH / 2 ) {
		_cutin_image.cx += CUTIN_SPEED * _cutin_spd_rate;
	} else {
		if ( _cutin_image.cnt < WAIT_TIME ) {
			_cutin_image.cx = WIDTH / 2;
			_cutin_image.cnt++;
			return;
		}
		_cutin_image.cx += CUTIN_SPEED * _cutin_spd_rate;

		if ( _cutin_image.cx > WIDTH + _cutin_image.lx ) {
			_cutin = true;
			_change_phase = false;
			_cutin_spd_rate = 1.0f;
			resetStringCutin( );
		}
	}
}

void Game::drawBackCutin( ) const {
	_drawer->setFrontImage( _background_cutin_image );
}

void Game::drawStringCutin( ) const {
	_drawer->setFrontImage( _cutin_image );
}

void Game::resetBackCutin( ) {
	ImagePtr image_ptr = _data->getImagePtr( );
	{//背景イメージ
		Png image = image_ptr->getPng( CUTIN_BACK_IMAGE, 0 );
		_background_cutin_image.cx = image.width / 2 * -1;
		_background_cutin_image.cy = HEIGHT / 2;
		_background_cutin_image.lx = image.width / 2;
		_background_cutin_image.ly = image.height / 2;
		_background_cutin_image.png = image.png;
		_background_cutin_image.cnt = 0;
	}
}

void Game::resetStringCutin( ) {
	ImagePtr image_ptr = _data->getImagePtr( );
	{//フェーズカットインの文字
		Png image = image_ptr->getPng( CUTIN_STRING_IMAGE, 0 );
		_cutin_image.cx = image.width / 2 * -1;
		_cutin_image.cy = HEIGHT / 2;
		_cutin_image.lx = image.width / 2;
		_cutin_image.ly = image.height / 2;
		_cutin_image.png = image.png;
		_cutin_image.cnt = 0;
	}
}

void Game::selectPlayerPos( bool *select ) {
	//ヒットしているポジションを探す
	if ( _field->getTmpPlayerPoint( ) != -1 ) {
		*select = true;
	}

	_field->hitPlayerPos( );

	int pos = _field->getPlayerPosHitNum( );

	if ( !_data->getClickLeft( ) ) {
		return;
	}

	if ( _field->isHitDecisionButton( ) ) {
		return;
	}

	_field->setTmpPlayerPoint( );

	*select = false;
	if ( pos != -1 ) {
		*select = true;
	}
}

void Game::updatePlayerPhase( ) {
	if ( _player_selected ) {
		return;
	}

	_field->setInfoText( "あなたの配置をしてください", YELLOW, Drawer::LITTLE_BIG );

	bool select = false;
	selectPlayerPos( &select );

	if ( !select ) {
		return;
	}

	_field->setInfoText( "" );
	_field->setInfoText( "決定を押して確定してください", RED, Drawer::LITTLE_BIG );
	_field->activeButtonLighting( );
	_field->changeClickButton( );
	
	int clicking = _data->getClickingLeft( );
	if ( clicking > 0 || ( clicking < 1 && _clicking < 1 ) ) {
		_clicking = clicking;
		return;
	}

	if ( !_field->isHitDecisionButton( ) ) {
		_clicking = 0;
		return;
	}

	if ( _field->isSelectedPlayer( ) ) {
		return;
	}

	_field->playerPosSelected( );
	_field->setPlayerPoint( _player_num, _field->getTmpPlayerPoint( ) );
	_client->setPlayerPos( _field->getPlayerPoint( _player_num ) );
	_client->sendTcp( );

	_player_selected = true;
}

void Game::inputTmpMirror( ) {

	int order = _client->getOrder( );

	if ( _order != order ) {
		resetBackCutin( );
		resetStringCutin( );
		_order = order;
		_cutin_spd_rate = 1.5f;
		_cutin = false;
	}

	//どっちが設置しているかのカットイン
	if ( !_cutin ) {
		_cutin_image.png = _cutin_player_handles[ ( _player_num == _order ) ];
		_background_cutin_image.png = _background_cutin_handles[ ( int )CUTIN_BACK_PLAYER ];
		drawBackCutin( );
		drawStringCutin( );
		calcBackCutin( );
		calcStringCutin( );
		return;
	}

	if ( _order != _player_num ) {
		_field->setInfoText( "相手が配置しています", RED, Drawer::LITTLE_BIG );
		return;
	}

	if ( _field->isSelectedMirror( ) ) {
		return;
	}

	_field->setInfoText( "左クリックでマスを選択し、", YELLOW );
	_field->setInfoText( "鏡を配置してください。" );
	_field->setInfoText( "" );
	_field->setInfoText( "もう一度左クリックで向きを変えられます" );
	_field->setInfoText( "" );
	_field->setInfoText( "あなたのターンです", RED, Drawer::LITTLE_BIG );
	if ( _double_mirror ) {
		_field->setInfoText( "2枚目を配置してください", WATER, Drawer::LITTLE_BIG );
	}

	//レーザーリセット選択時は無効
	if ( _field->getSelectItem( ) == LAZER_RESET ) {
		return;
	}

	//リバースミラー選択時は無効
	if ( _field->getSelectItem( ) == REVERSE_MIRROR ) {
		return;
	}

	//ヒットしているマスを探す
	_field->hitFieldPos( );
	int pos = _field->getFieldPosHitNum( );

	if ( _field->isHitDecisionButton( ) ) {
		return;
	}

	if ( _field->getHitItemIdx( ) != -1 ) {
		return;
	}

	if ( !_data->getClickLeft( ) ) {
		return;
	}

	if ( pos < 0 ) {
		_tmp_mirror = Field::Mirror( );
		_field->resetTmpMirror( );
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
	_field->activeButtonLighting( );
	_field->changeClickButton( );
	
	int clicking = _data->getClickingLeft( );
	if ( clicking > 0 || ( clicking < 1 && _clicking < 1 ) ) {
		_clicking = clicking;
		return;
	}

	if ( !_field->isHitDecisionButton( ) ) {
		_clicking = 0;
		return;
	}

	_field->mirrorPosSelected( );
	
	_client->setCtsPlayerNum( );
	_client->setCtsAngle( _tmp_mirror.angle );
	_client->setCtsX( _tmp_mirror.x );
	_client->setCtsY( _tmp_mirror.y );
	_client->setCtsFlag( _tmp_mirror.flag );

	_tmp_mirror = Field::Mirror( );
	_field->resetTmpMirror( );

	if ( _field->getSelectItem( ) != -1 ) {
		return;
	}

	_client->sendTcp( );
	_cutin = false;
}

void Game::updateAttackPhase( ) {
	if ( _reverse_mirror ) {
		_field->reverseMirror( );
		_reverse_mirror = false;
		_cutin = false;
		_cutin_spd_rate = 1.8f;
		resetBackCutin( );
		resetStringCutin( );
	}
	if ( !_cutin ) {
		_cutin_image.png = _cutin_item_handles[ REVERSE_MIRROR ];
		_background_cutin_image.png = _background_cutin_handles[ ( int )CUTIN_BACK_ITEM ];
		drawBackCutin( );
		drawStringCutin( );
		calcBackCutin( );
		calcStringCutin( );
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
		_field->mirrorPosNotSelected( );
		_mirror_phase_recv = false;
		_attack_phase_recv = false;
		_judge_phase_recv = false;
		_order = -1;
		_send_live = false;
		_double_mirror = false;
		_tmp_mirror = Field::Mirror( );
		_field->nextTurn( );
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
		mirror[ i ].flag = _client->getStcFlag( i );
		mirror[ i ].player_num = _client->getStcPlayerNum( i );
		mirror[ i ].x = _client->getStcX( i );
		mirror[ i ].y = _client->getStcY( i );
		mirror[ i ].angle = _client->getStcAngle( i );
	}

	bool del = false;
	if ( mirror[ 0 ].flag && mirror[ 1 ].flag ) {
		if ( mirror[ 0 ].x == mirror[ 1 ].x &&
			 mirror[ 0 ].y == mirror[ 1 ].y &&
			 mirror[ 0 ].angle == mirror[ 1 ].angle ) {
			_field->deleteMirrorPoint( mirror[ 0 ].x + mirror[ 0 ].y * FIELD_COL );
			del = true;
		}
	}

	if ( !del ) {
		for ( int i = 0; i < PLAYER_NUM; i++ ) {
			if ( mirror[ i ].flag ) {
				_field->setMirrorPoint( mirror[ i ].player_num, mirror[ i ].x, mirror[ i ].y, mirror[ i ].angle );
			}
		}
	}

	_lazer->initialize( );
	_attack_phase_recv = true;
}

void Game::recvJudgePhase( ) {
	if ( !_client->isRecvingUdp( ) ) {
		return;
	}
	_judge_phase_recv = true;
}

void Game::updateItemCalc( ) {
	if ( _order != _player_num ) {
		return;
	}

	if ( !_cutin ) {
		return;
	}

	int idx = _field->getHitItemIdx( );
	_field->changeClickButton( );

	int item = _field->getSelectItem( );
	//ボタン点滅
	switch ( item ) {
	case LAZER_RESET    : _field->activeButtonLighting( ); break;
	case DOUBLE_MIRROR  : 
		if ( _field->isSelectedMirror( ) ) {
			_field->activeButtonLighting( );
		}
		break;

	case REVERSE_MIRROR : _field->activeButtonLighting( ); break;
	default: break;
	}
	
	int clicking = _data->getClickingLeft( );
	if ( clicking > 0 || ( clicking < 1 && _clicking < 1 ) ) {
		_clicking = clicking;
		return;
	}

	if ( _field->getFieldPosHitNum( ) != -1 ) {
		_clicking = 0;
		return;
	}

	if ( !_field->isHitDecisionButton( ) ) {
		_clicking = 0;
		if ( _field->getFieldPosHitNum( ) < 0 ) {
			_field->selectItem( idx );
		}
		return;
	}

	item = _field->getSelectItem( );
	if ( item < 0 ) {
		return;
	}
	if ( item == ( int )DOUBLE_MIRROR ) {
		if ( !_field->isSelectedMirror( ) ) {
			return;
		}
		int order = 0;
		if ( !_player_num ) {
			order = ( _turn + 1 ) % 2 + 1;
		} else {
			order = _turn % 2 + 1;
		}

		if ( order != 1 ) {
			_field->mirrorPosNotSelected( );
			return;
		}
	}

	_client->setItemFlag( true );
	_client->setItem( _field->getSelectItem( ) );
	_client->setItemUser( );
	_client->sendTcp( );

	_field->useItem( );
}

void Game::invocationItem( ) {
	if ( _item_callback ) {
		return;
	}

	switch ( ( ITEM )_item ) {
	case LAZER_RESET:
		{//レーザーの位置を変更
			int lazer_pos = _client->getLazerPoint( );
			if ( lazer_pos == ( unsigned char )-1 ) {
				return;
			}
			_field->setLazerPoint( lazer_pos );
			_field->mirrorPosNotSelected( );
			_mirror_phase_recv = false;
			_attack_phase_recv = false;
			_judge_phase_recv = false;
			_order = -1;
			_send_live = false;
			_tmp_mirror = Field::Mirror( );
			_field->nextTurn( );
			resetBackCutin( );
			resetStringCutin( );
		}
		break;

	case DOUBLE_MIRROR:
		{//鏡2枚配置
			int idx = _client->getItemUser( );
			Field::Mirror mirror = Field::Mirror( );
			mirror.x = _client->getStcX( idx );
			mirror.y = _client->getStcY( idx );
			mirror.angle = _client->getStcAngle( idx );
			mirror.player_num = idx;

			_field->setMirrorPoint( mirror.player_num, mirror.x, mirror.y, mirror.angle );

			if ( idx == _player_num ) {
				_field->mirrorPosNotSelected( );
				_tmp_mirror = Field::Mirror( );
				_field->nextTurn( );
				_double_mirror = true;
				resetBackCutin( );
				resetStringCutin( );
			}
		}
		break;

	case REVERSE_MIRROR:
		{
			_reverse_mirror = true;
		}
		break;
	}

	_client->setItemFlag( false );
	_client->setItem( 0 );
	_client->sendTcp( );
	_item_callback = true;
}

void Game::checkItemFlag( ) {
	if ( !_client->isRecvingUdp( ) ) {
		return;
	}

	bool use = _client->isItemFlag( );

	if ( use != _use_item ) {
		_use_item = use;
		if ( !_use_item ) {
			return;
		}
		_item = _client->getItem( );
		_item_callback = false;
		_cutin = false;
		resetBackCutin( );
		resetStringCutin( );
	}

}

bool Game::isWin( ) const {
	return _win;
}