#include "Field.h"
#include "GlobalData.h"
#include "const.h"
#include "Image.h"
#include "LoadCSV.h"
#include "Debug.h"
#include <random>

const short int MOUSE_R = 5;
const short int CIRCLE_SIZE = SQUARE_SIZE / 5 * 2;
const short int PLAYER_POS_X = START_POS_X - SQUARE_SIZE / 2;
const short int PLAYER_POS_Y = START_POS_Y - SQUARE_SIZE / 2;
const short int DECISION_BUTTON_X = WIDTH / 3 * 2;
const short int DECISION_BUTTON_Y = START_POS_Y - ( short int )( SQUARE_SIZE * 1.5 );
const short int MIRROR_IMAGE_IDX = 2;
const short int BOARD_X = WIDTH / 5;
const short int BOARD_Y = HEIGHT / 2;
const short int ITEM_POS_X = BOARD_X - ( short int )( SQUARE_SIZE * 1.5 );
const short int ITEM_POS_Y = BOARD_Y;
const short int INFO_Y = BOARD_Y - ( short int )( SQUARE_SIZE * 2.2 );
const short int DEATH_COUNT_MAX = MINUTE * 5;
const short int NONACTIVE_BRIGHT = 130;

enum IMAGE_IDX {
	BOARD_IDX,
	LAZER_RIGHT_IDX,
	LAZER_TABLE_IDX,
	TABLE_IDX,
	MIRROR_IDX,
	FIRST_TURN = 7
};

Field::Field( GlobalDataPtr data ) :
_data( data ) {
	setFlag( 1 );
	_drawer = _data->getDrawerPtr( );
	_image = _data->getImagePtr( );
	_debug = _data->getDebugPtr( );
	_cur_hand  = LoadCursor( NULL, IDC_HAND );
	std::array< Info, INFO_TEXT_MAX >( ).swap( _info );
	std::array< Item, ITEM_POSSESSION_MAX >( ).swap( _item );
	_table_handle = -1;
	_time_string_handle = -1;
	_timeboard_handle = -1;
	_item_not_handle = -1;
	std::array< int, PLAYER_NUM >( ).swap( _player_handle );
	std::array< int, NUMBER_HANDLE_MAX >( ).swap( _number_handle );
	std::array< int, PLAYER_NUM >( ).swap( _mirror_handle );
	std::array< int, BATTLE_BUTTON_IMAGE_NUM >( ).swap( _button_handle );
	std::array< int, ITEM_MAX >( ).swap( _item_handle );
	std::array< int, LAZER_TYPE_MAX >( ).swap( _lazer_handle );
	std::array< int, PLAYER_NUM >( ).swap( _first_or_second_handle );
	_button_image = LightImageProperty( );
	_lazer_image = LightImageProperty( );
	_item_cancel = BoxObject( );

	Png png = Png( );
	//プレイヤー
	for ( int i = 0; i < PLAYER_NUM; i++ ) {
		_player_handle[ i ] = _image->getPng( PLAYER_IMAGE, i ).png;
	}

	//ボタン
	for ( int i = 0; i < BATTLE_BUTTON_IMAGE_NUM; i++ ) {
		_button_handle[ i ] = _image->getPng( BUTTON_IMAGE, BATTLE_BUTTON_IDX + i ).png;
	}
	_button_image.cx = DECISION_BUTTON_X;
	_button_image.cy = DECISION_BUTTON_Y;

	//フィールド
	_table_handle = _image->getPng( BATTLE_IMAGE, TABLE_IDX ).png;

	//ミラー
	for ( int i = 0; i < PLAYER_NUM; i++ ) {
		_mirror_handle[ i ] = _image->getPng( BATTLE_IMAGE, MIRROR_IDX + i ).png;
	}

	//アイテム
	for ( int i = 0; i < ITEM_MAX; i++ ) {
		_item_handle[ i ] = _image->getPng( ITEM_IMAGE, i ).png;
	}
	for ( int i = 0; i < ITEM_POSSESSION_MAX; i++ ) {
		_item[ i ].x = ( float )( ITEM_POS_X + i * SQUARE_SIZE + SQUARE_SIZE * 0.5 );
		_item[ i ].y = ( float )ITEM_POS_Y;
	}

	//レーザー
	for ( int i = 0; i < LAZER_TYPE_MAX; i++ ) {
		_lazer_handle[ i ] = _image->getPng( BATTLE_IMAGE, LAZER_RIGHT_IDX + i ).png;
	}

	//ボード(info)
	_board = BoxObject( );
	_board.image.cx = BOARD_X;
	_board.image.cy = BOARD_Y;
	_board.half_width = _image->getPng( BATTLE_IMAGE, BOARD_IDX ).width / 2;
	_board.half_height = _image->getPng( BATTLE_IMAGE, BOARD_IDX ).height / 2;
	_board.image.png = _image->getPng( BATTLE_IMAGE, BOARD_IDX ).png;

	_board.collider.lx = ( float )( _board.image.cx - _board.half_width  );
	_board.collider.ly = ( float )( _board.image.cy - _board.half_height );
	_board.collider.rx = ( float )( _board.image.cx + _board.half_width  );
	_board.collider.ry = ( float )( _board.image.cy + _board.half_height );

	//背景	
	_background = LightImageProperty( );
	_background.cx = WIDTH / 2;
	_background.cy = HEIGHT / 2;
	_background.png = _image->getPng( BACKGROUND_IMAGE, BACKGROUND_NORMAL ).png;

	//鏡の設置コマンド
	std::array< BoxObject, COMMAND_TYPE_MAX >( ).swap( _mirror_cmd );
	const short int PITCH = ( SQUARE_SIZE * FIELD_ROW ) / ( short int )COMMAND_TYPE_MAX;
	const short int MIRROR_COMMAND_X = START_POS_X + SQUARE_SIZE * FIELD_COL + SQUARE_SIZE;
	const short int MIRROR_COMMAND_Y = START_POS_Y + _image->getPng( COMMAND_IMAGE, 0 ).height / 2;

	for ( int i = 0; i < COMMAND_TYPE_MAX; i++ ) {
		//画像関連
		LightImageProperty *image = &_mirror_cmd[ i ].image;
		image->png = _image->getPng( COMMAND_IMAGE, i ).png;
		image->cx = MIRROR_COMMAND_X;
		image->cy = MIRROR_COMMAND_Y + PITCH * i;
		//当たり判定
		_mirror_cmd[ i ].half_width = _image->getPng( COMMAND_IMAGE, i ).width / 2;
		_mirror_cmd[ i ].half_height = _image->getPng( COMMAND_IMAGE, i ).height / 2;

		BoxCollider *box = &_mirror_cmd[ i ].collider;
		box->lx = ( float )image->cx - _mirror_cmd[ i ].half_width;
		box->ly = ( float )image->cy - _mirror_cmd[ i ].half_height;
		box->rx = ( float )image->cx + _mirror_cmd[ i ].half_width;
		box->ry = ( float )image->cy + _mirror_cmd[ i ].half_height;
	}

	//アイテムキャンセルボタン
	const short int ITEM_CANCEL_BUTTON_X = BOARD_X;
	const short int ITEM_CANCEL_BUTTON_Y = ITEM_POS_Y + ( short int )( SQUARE_SIZE * 1.25 );
	LightImageProperty *image = &_item_cancel.image;
	image->cx  = ITEM_CANCEL_BUTTON_X;
	image->cy  = ITEM_CANCEL_BUTTON_Y;
	image->png = _image->getPng( BUTTON_IMAGE, ITEM_CANCEL_BUTTON_IDX ).png;

	//当たり判定
	_item_cancel.half_width = _image->getPng( BUTTON_IMAGE, ITEM_CANCEL_BUTTON_IDX ).width / 2;
	_item_cancel.half_height = _image->getPng( BUTTON_IMAGE, ITEM_CANCEL_BUTTON_IDX ).height / 2;
	BoxCollider *box = &_item_cancel.collider;
	box->lx = ( float )image->cx - _item_cancel.half_width;
	box->ly = ( float )image->cy - _item_cancel.half_height;
	box->rx = ( float )image->cx + _item_cancel.half_width;
	box->ry = ( float )image->cy + _item_cancel.half_height;

	//時間表示背景
	_timeboard_handle = _image->getPng( TIME_IMAGE, 0 ).png;
	_time_string_handle = _image->getPng( TIME_IMAGE, 1 ).png;

	//数字画像
	for ( int i = 0; i < NUMBER_HANDLE_MAX; i++ ) {
		_number_handle[ i ] = _image->getPng( NUMBER_IMAGE, i ).png;
	}

	//先攻後攻
	for ( int i = 0; i < PLAYER_NUM; i++ ) {
		_first_or_second_handle[ i ] = _image->getPng( BATTLE_IMAGE, FIRST_TURN + i ).png;
	}

	//アイテム使用不可
	_item_not_handle = _image->getPng( ITEM_IMAGE, ITEM_NOT ).png;
}

Field::~Field( ) {
}

std::string Field::getTag( ) {
	return "FIELD";
}

void Field::initialize( ) {
	_turn = 1;
	_player_selected = false;
	_mirror_selected = false;
	_button_lighting = false;
	_reverse_mirror = false;
	_blind_field = false;
	_first = true;
	_order = -1;
	_info_idx = 0;
	_tmp_player_pos = -1;
	_dead_player = -1;
	_hit_mirror_num = -1;
	_player_num = -1;
	_player_pos_hit_num = -1;
	_field_pos_hit_num = -1;
	_lazer_point_idx = -1;
	_select_item = -1;
	_dead_count = DEATH_COUNT_MAX;
	_tmp_mirror = Mirror( );
	std::map< int, Mirror >( ).swap( _mirrors );
	_command = COMMAND_NONE;

	_phase = SET_PLAYER_PHASE;

	//CSV
	LoadCSVPtr csv( new LoadCSV( ) );

	{//アイテムを読み込む
		std::vector< CsvData > item_data;
		csv->read( item_data, "item" );

		std::array< int, ITEM_MAX > values;
		std::array< int, ITEM_MAX >( ).swap( values );

		std::vector< CsvData >::iterator ite;
		ite = item_data.begin( );

		//値を詰める
		for ( ite; ite != item_data.end( ); ite++ ) {
			values[ ( int )std::distance( item_data.begin( ), ite ) ] = atoi( ite->values[ 0 ].c_str( ) );
		}

		//反映
		int idx = 0;
		int value_idx = 0;
		while ( true ) {
			if ( values[ value_idx ] > 0 ) {
				_item[ idx ].flag = true;
				_item[ idx ].type = value_idx;
				values[ value_idx ]--;
				idx++;
				if ( idx >= ITEM_POSSESSION_MAX ) {
					break;
				}
				continue;
			}
			value_idx++;
			if ( value_idx >= ITEM_MAX ) {
				break;
			}
		}
	}
	{//制限時間を読み込む
		std::vector< CsvData > limit;
		csv->read( limit, "limit" );
		if ( limit.begin( )->tag == "DEATH_COUNT" ) {
			_dead_count = ( short int )( MINUTE * atof( limit.begin( )->values.begin( )->c_str( ) ) );
		}
	}

	for ( int i = 0; i < PLAYER_POSITION * 2; i++ ) {
		if ( i < PLAYER_POSITION ) {
			_select_player_pos[ i ].x = ( float )PLAYER_POS_X;
			_select_player_pos[ i ].y = ( float )PLAYER_POS_Y + ( i + 1 ) * SQUARE_SIZE;
		} else {
			_select_player_pos[ i ].x = ( float )PLAYER_POS_X + ( i - PLAYER_POSITION + 1 ) * SQUARE_SIZE;
			_select_player_pos[ i ].y = ( float )PLAYER_POS_Y;
		}
	}
	for ( int i = 0; i < PLAYER_NUM; i++ ) {
		_player_pos_idx[ i ] = -1;
		_player_color[ i ] = ( COLOR )( i + ( int )RED );
	}
}

void Field::finalize( ) {
}

void Field::nextTurn( ) {
	_phase = SET_MIRROR_PHASE;
	_mirror_selected = false;
	_reverse_mirror = false;
	_blind_field = false;
	_order = -1;
	_dead_player = -1;
	_hit_mirror_num = -1;
	_field_pos_hit_num = -1;
	_select_item = -1;
	_tmp_mirror = Mirror( );
	_command = COMMAND_NONE;
}

void Field::update( ) {
	if ( _select_item != -1 ) {
		resetInfo( );
		switch ( ( ITEM )_item[ _select_item ].type ) {
		case LAZER_RESET:
			setInfoText( "" );
			setInfoText( "レーザーの位置を強制的に変更します", RED );
			setInfoText( "ターン経過はなく", RED );
			setInfoText( "" );
			setInfoText( "このターンのやり直しができます", WATER );
			break;

		case DOUBLE_MIRROR:
			setInfoText( "" );
			setInfoText( "鏡を2枚配置できます", RED );
			setInfoText( "先攻のプレイヤーのみ発動できます", WATER );
			setInfoText( "" );
			setInfoText( "鏡を1枚配置した状態で決定をしてください", RED );
			setInfoText( "1枚目の鏡は相手にも見えてしまいます", RED );
			break;

		case REVERSE_MIRROR:
			setInfoText( "" );
			setInfoText( "配置されている全ての鏡の向きを", RED );
			setInfoText( "反転させます", RED );
			setInfoText( "" );
			setInfoText( "使用したターンは鏡が設置できません", WATER );
			break;
		}
	}

	//計算
	if ( _phase == SET_MIRROR_PHASE ) {
		checkHitMirrorCommand( );
	}
	checkItemRecovery( );
	//描画
	drawBackGround( );
	drawField( );
	drawPlayer( );
	drawArmament( );
	drawInfo( );
	drawDecisionButton( );
	drawFirstOrSecond( );
	resetInfo( );
	_button_lighting = false;

	//デバッグ
	if ( _debug->getFlag( ) ) {
		_debug->addLog( "MirrorCmdIdx : " + std::to_string( ( int )_command ) );
	}


	if ( _phase == SET_PLAYER_PHASE ) {
		drawPlayerPos( );
	}

	if ( _phase < SET_MIRROR_PHASE ) {
		return;
	}
	//レーザー移動までのターン数
	drawItem( );
	drawItemCancelButton( );
	drawDeathCount( );
	drawRound( );

	//ミラー設置時のみ
	if ( _phase == SET_MIRROR_PHASE ) {
		if ( _order != -1 ) {
			//drawSettingPlayer( );
		}
		drawMirrorCommand( );
	}
	//アタックフェイズ時は描画しない
	if ( _phase != ATTACK_PHASE ) {
		drawTmpMirror( );
	}
	
	if ( _phase < ATTACK_PHASE ) {
		return;
	}
	drawMirror( );
}

void Field::hitPlayerPos( ) {
	if ( _player_num < 0 ) {
		return;
	}
	double mouse_x = _data->getMouseX( );
	double mouse_y = _data->getMouseY( );
	for ( int i = 0; i < PLAYER_POSITION * 2; i++ ) {
		if ( _player_num == 0 && i > PLAYER_POSITION - 1 ) {
			continue;
		}
		if ( _player_num == 1 && i < PLAYER_POSITION ) {
			continue;
		}
		double x = _select_player_pos[ i ].x;
		double y = _select_player_pos[ i ].y;
		double distance = sqrt( ( mouse_x - x ) * ( mouse_x - x ) + ( mouse_y - y ) * ( mouse_y - y ) );
		if ( distance <= CIRCLE_SIZE + MOUSE_R ) {
			_player_pos_hit_num = i;
			return;
		}
	}
	_player_pos_hit_num = -1;
	return;
}

bool Field::isSelectedPlayer( ) const {
	return _player_selected;
}

void Field::hitFieldPos( ) {
	if ( _player_num < 0 ) {
		return;
	}
	if ( _mirror_selected ) {
		return;
	}
	int mouse_x = _data->getMouseX( );
	int mouse_y = _data->getMouseY( );
	for ( int i = 0; i < FIELD_ROW; i++ ) {
		for ( int j = 0; j < FIELD_COL; j++ ) {
			double table_left = START_POS_X + j * SQUARE_SIZE;
			double table_top  = START_POS_Y + i * SQUARE_SIZE;
			if ( table_left < mouse_x && mouse_x   < table_left + SQUARE_SIZE &&
				 table_top  < mouse_y && mouse_y < table_top  + SQUARE_SIZE ) {
				_field_pos_hit_num = j + i * FIELD_COL;
				return;
			}
		}
	}
	_field_pos_hit_num = -1;
	return;
}

bool Field::isHitDecisionButton( ) const {
	int mouse_x = _data->getMouseX( );
	int mouse_y = _data->getMouseY( );

	Png png = _image->getPng( BUTTON_IMAGE, BATTLE_BUTTON_IDX );
	int lx = DECISION_BUTTON_X - png.width / 2;
	int rx = DECISION_BUTTON_X + png.width / 2;
	int ly = DECISION_BUTTON_Y - png.height / 2;
	int ry = DECISION_BUTTON_Y + png.height / 2;

	if ( lx < mouse_x && mouse_x < rx &&
		 ly < mouse_y && mouse_y < ry ) {
		return true;
	}

	return false;
}

int Field::getHitItemIdx( ) const {
	int mouse_x = _data->getMouseX( );
	int mouse_y = _data->getMouseY( );

	for ( int i = 0; i < ( int )ITEM_MAX; i++ ) {
		if ( !_item[ i ].flag ) {
			continue;
		}

		float lx = ( float )( _item[ i ].x - SQUARE_SIZE * 0.5 );
		float ly = ( float )( _item[ i ].y - SQUARE_SIZE * 0.5 );
		float rx = ( float )( _item[ i ].x + SQUARE_SIZE * 0.5 );
		float ry = ( float )( _item[ i ].y + SQUARE_SIZE * 0.5 );

		if ( lx <= mouse_x && mouse_x <= rx &&
			 ly <= mouse_y && mouse_y <= ry ) {

			//後攻でダブルミラーなら使用不可
			if ( !_first && _item[ i ].type == DOUBLE_MIRROR ) {
				return -1;
			}

			SetCursor( _cur_hand );
			return i;
		}
	}

	return -1;
}

int Field::getHitMirrorCommandIdx( ) const {
	int mouse_x = _data->getMouseX( );
	int mouse_y = _data->getMouseY( );

	short int hit = -1;
	for ( int i = 0; i < COMMAND_TYPE_MAX; i++ ) {
		if ( _mirror_cmd[ i ].collider.lx <= mouse_x && mouse_x <= _mirror_cmd[ i ].collider.rx &&
			 _mirror_cmd[ i ].collider.ly <= mouse_y && mouse_y <= _mirror_cmd[ i ].collider.ry ) {
			hit = i;
			break;
		}
	}

	return hit;
}

int Field::getDeadCount( ) const{
	return _dead_count;
}

std::map< int, Field::Mirror > &Field::getAllMirror( ) {
	return _mirrors;
}

Field::COMMAND Field::getMirrorCommand( ) const {
	return _command;
}

MIRROR_ANGLE Field::getMirrorAngle( int idx ) const {
	MIRROR_ANGLE angle = ANGLE_NONE;
	if ( _mirrors.find( idx ) != _mirrors.end( ) ) {
		angle = _mirrors.find( idx )->second.angle;
	}
	return angle;
}

bool Field::isSelectedMirror( ) const {
	return _mirror_selected;
}

bool Field::isHitItemCancelButton( ) const {
	int mouse_x = _data->getMouseX( );
	int mouse_y = _data->getMouseY( );

	if ( _item_cancel.collider.lx <= mouse_x && mouse_x <= _item_cancel.collider.rx && 
		 _item_cancel.collider.ly <= mouse_y && mouse_y <= _item_cancel.collider.ry ) {
		return true;
	}
	return false;
}

bool Field::isReverseFlag( ) const {
	return _reverse_mirror;
}

int Field::getDeadPlayer( ) const {
	return _dead_player;
}

int Field::getLazerPointIdx( ) const {
	return _lazer_point_idx;
}

int Field::getSelectItem( ) const {
	if ( _select_item < 0 ) {
		return -1;
	}
	return _item[ _select_item ].type;
}

void Field::setTurn( int turn ) {
	_turn = turn;
}

void Field::setPlayerNum( int num ) {
	_player_num = num;
	_button_image.png = _button_handle[ _player_num * PLAYER_NUM ];
}

void Field::setInfoText( std::string str, COLOR col, Drawer::FONTSIZE_TYPE size ) {
	_info[ _info_idx ].str = str;
	_info[ _info_idx ].col = col;
	_info[ _info_idx ].size = size;
	_info_idx = ( _info_idx + 1 ) % INFO_TEXT_MAX;
}

void Field::resetInfo( ) {
	std::array< Info, INFO_TEXT_MAX >( ).swap( _info );
	_info_idx = 0;
}

void Field::checkHitMirrorCommand( ) {
	int mouse_x = _data->getMouseX( );
	int mouse_y = _data->getMouseY( );

	short int hit = -1;
	for ( int i = 0; i < COMMAND_TYPE_MAX; i++ ) {
		if ( _mirror_cmd[ i ].collider.lx <= mouse_x && mouse_x <= _mirror_cmd[ i ].collider.rx &&
			 _mirror_cmd[ i ].collider.ly <= mouse_y && mouse_y <= _mirror_cmd[ i ].collider.ry ) {
			hit = i;
			break;
		}
	}

	if ( !_data->getClickLeft( ) ) {
		return;
	}

	if ( hit < 0 ) {
		return;
	}

	_command = ( COMMAND )hit;
}

void Field::checkItemRecovery( ) {
	for ( int i = 0; i < ITEM_POSSESSION_MAX; i++ ) {
		if ( _item[ i ].flag ) {
			continue;
		}
		if ( _item[ i ].used_turn + ITEM_RECOVERY_TIME == _turn ) {
			_item[ i ].flag = true;
		}
	}
}

void Field::setOrder( int order ) {
	_order = order;
}

void Field::setPhase( BATTLE_PHASE phase ) {
	_phase = phase;
}

void Field::setPlayerPoint( int idx, int pos ) {
	_player_pos_idx[ idx ] = pos;
}

void Field::setLazerPoint( int pos ) {
	_lazer_point_idx = pos;
	Vector dir = Vector( );
	if ( _lazer_point_idx < PLAYER_POSITION ) {
		dir.x = 1;
		dir.y = 0;
	} else {
		dir.x = 0;
		dir.y = -1;
	}

	if ( _lazer_point_idx < 0 ) {
		return;
	}
	_lazer_image.cx = _select_player_pos[ _lazer_point_idx ].x;
	_lazer_image.cy = _select_player_pos[ _lazer_point_idx ].y;
	_lazer_image.png = _lazer_handle[ _lazer_point_idx / PLAYER_POSITION ];
}

void Field::setTmpPlayerPoint( ) {
	_tmp_player_pos = getPlayerPosHitNum( );
}

void Field::resetTmpMirror( ) {
	_tmp_mirror = Mirror( );
}

void Field::setTmpMirrorPoint( int player_num, int x, int y, MIRROR_ANGLE angle ) {
	_tmp_mirror.player_num = player_num;
	_tmp_mirror.x = x;
	_tmp_mirror.y = y;
	_tmp_mirror.angle = angle;
	_tmp_mirror.flag = true;
}

void Field::setMirrorPoint( int player_num, int x, int y, MIRROR_ANGLE angle ) {
	int idx = x + y * FIELD_COL;
	if ( angle == ANGLE_NONE ) {
		if ( _mirrors.find( idx ) != _mirrors.end( ) ) {
			_mirrors.erase( idx );
		}
		return;
	}

	Mirror mirror = Mirror( );
	mirror.flag = true;
	mirror.player_num = player_num;
	mirror.x = x;
	mirror.y = y;
	mirror.angle = angle;

	std::map< int, Mirror >::iterator ite;
	ite = _mirrors.begin( );
	for ( ite; ite != _mirrors.end( ); ite++ ) {
		Mirror tmp = ite->second;
		if ( tmp.x == mirror.x &&
			 tmp.y == mirror.y &&
			 tmp.angle == mirror.angle ) {
			deleteMirrorPoint( idx );
			return;
		}
	}

	_mirrors[ idx ] = mirror;
}

void Field::deleteMirrorPoint( int idx ) {
	if ( _mirrors.find( idx ) != _mirrors.end( ) ) {
		_mirrors.erase( idx );
	}
}

void Field::playerPosSelected( ) {
	_player_selected = !_player_selected;
}

void Field::mirrorPosSelected( ) {
	_mirror_selected = true;
}

void Field::mirrorPosNotSelected( ) {
	_mirror_selected = false;
}

void Field::selectItem( int idx ) {
	_select_item = idx;
}

void Field::useItem( ) {
	if ( _select_item < 0 ) {
		return;
	}
	_item[ _select_item ].flag = false;
	_item[ _select_item ].used_turn = _turn;
	_select_item = -1;
}

void Field::reverseMirror( ) {
	std::map< int, Mirror >::iterator ite;
	ite = _mirrors.begin( );

	for ( ite; ite != _mirrors.end( ); ite++ ) {
		ite->second.angle = ( MIRROR_ANGLE )( ( ( int )ite->second.angle + 1 ) % ( int )MIRROR_ANGLE_MAX );
	}

	_reverse_mirror = false;
}

void Field::changeClickButton( ) {
	int idx = _player_num * PLAYER_NUM;

	if ( _data->getClickingLeft( ) > 0 && isHitDecisionButton( ) ) {
		idx++;
		_button_lighting = false;
	}

	_button_image.png = _image->getPng( BUTTON_IMAGE, BATTLE_BUTTON_IDX + idx ).png;
}

void Field::activeButtonLighting( ) {
	_button_lighting = true;
}


void Field::deadCount( ) {
	_dead_count--;
	if( _dead_count < 0 ) {
		_dead_count = 0;
	}
}

void Field::setDeadPlayer( int player ) {
	_dead_player = player;
}

void Field::setReverseFlag( ) {
	_reverse_mirror = true;
}

void Field::setFirstOrSecond( bool first ) {
	_first = first;
}

void Field::blindField( ) {
	_blind_field = true;
}

void Field::clearBlindField( ) {
	_blind_field = false;
}

Vector Field::getLazerPoint( ) const {
	Vector vec = Vector( );
	if ( _lazer_point_idx < PLAYER_POSITION ) {
		vec.x = PLAYER_POS_X;
		vec.y = PLAYER_POS_Y + ( _lazer_point_idx + 1 ) * SQUARE_SIZE;
	} else {
		vec.x = PLAYER_POS_X + ( _lazer_point_idx % PLAYER_POSITION + 1 ) * SQUARE_SIZE;
		vec.y = PLAYER_POS_Y;
	}

	return vec;
}

Field::PlayerPos Field::getPlayerPos( int idx ) const {
	if ( idx >= PLAYER_NUM ) {
		return PlayerPos( );
	}
	return _select_player_pos[ _player_pos_idx[ idx ] ];
}

BATTLE_PHASE Field::getPhase( ) const{
	return _phase;
}

int Field::getTurn( ) const {
	return _turn;
}

int Field::getTmpPlayerPoint( ) const {
	return _tmp_player_pos;
}

int Field::getPlayerPosIdx( int idx ) const {
	return _player_pos_idx[ idx ];
}
int Field::getHitMirrorIdx( ) const {
	return _hit_mirror_num;
}

int Field::getPlayerPosHitNum( ) const {
	return _player_pos_hit_num;
}

int Field::getFieldPosHitNum( ) const {
	return _field_pos_hit_num;
}

void Field::drawFirstOrSecond( ) const {
	if ( _order < 0 ) {
		return;
	}

	if ( _order != _player_num ) {
		return;
	}

	const short int X = WIDTH / 2;
	const short int Y = 25;
	ImageProperty image = ImageProperty( );
	image.cx = X;
	image.cy = Y;
	image.alpha = 150 + ( int )( sin( 0.03 * _data->getCount( ) ) * 70 );
	if ( _first ) {
		image.png = _first_or_second_handle[ 0 ];
	} else {
		image.png = _first_or_second_handle[ 1 ];
	}

	_drawer->setFrontImage( image );
}

void Field::drawField( ) {
	if ( getFieldPosHitNum( ) != -1 ) {
		SetCursor( _cur_hand );
	}

	//フィールド描画
	for ( int i = 0; i < FIELD_ROW; i++ ) {
		for ( int j = 0; j < FIELD_COL; j++ ) {
			ImageProperty image = ImageProperty( );
			image.cx = START_POS_X + j * SQUARE_SIZE + SQUARE_SIZE * 0.5;
			image.cy = START_POS_Y + i * SQUARE_SIZE + SQUARE_SIZE * 0.5;
			image.png = _table_handle;
			image.size = FIELD_SIZE_RATE;
			//ミラーを置こうとしているなら
			if ( _phase == SET_MIRROR_PHASE && ( i * FIELD_COL + j ) == getFieldPosHitNum( ) ) {
				image.bright_flag = true;
				//プレイヤーカラー
				switch ( _player_num ) {
				case 0://赤
					image.brt[ 0 ] = 255;
					image.brt[ 1 ] = 150;
					image.brt[ 2 ] = 150;
					break;
				case 1://青
					image.brt[ 0 ] = 150;
					image.brt[ 1 ] = 150;
					image.brt[ 2 ] = 255;
					break;
				}
				image.alpha = ( short int )( sin( 0.05 * _data->getCount( ) ) * 25 + 230 );
			}
			_drawer->setBackImage( image );
		}
	}

}

void Field::drawArmament( ) const {
	//砲台描画
	if ( _lazer_point_idx < 0 ) {
		return;
	}
	ImageProperty image = ImageProperty( );
	image.cx = _lazer_image.cx;
	image.cy = _lazer_image.cy;
	image.png = _lazer_image.png;
	image.size = FIELD_SIZE_RATE;
	_drawer->setBackImage( image );
}

void Field::drawTmpMirror( ) const {
	//鏡描画
	std::map< int, Mirror > tmp_mirrors = _mirrors;
	if ( _tmp_mirror.flag ) {
		int idx = _tmp_mirror.x + _tmp_mirror.y * FIELD_COL;
		if ( _tmp_mirror.angle == ANGLE_NONE ) {
			if ( tmp_mirrors.find( idx ) != tmp_mirrors.end( ) ) {
				tmp_mirrors.erase( idx );
			}
		} else {
			tmp_mirrors[ idx ] = _tmp_mirror;
		}
	}

	for ( int i = 0; i < FIELD_ROW * FIELD_COL; i++ ) {
		if ( tmp_mirrors.find( i ) == tmp_mirrors.end( ) ) {
			continue;
		}
		Mirror mirror = tmp_mirrors.find( i )->second;
		ImageProperty image = ImageProperty( );

		float angle = 0;
		if ( mirror.angle == ANGLE_NONE ) {
			continue;
		}

		if ( mirror.angle != RIGHT ) {
			angle = ( float )PI / 2;
		}
		image.cx = START_POS_X + mirror.x * SQUARE_SIZE + SQUARE_SIZE * 0.5;
		image.cy = START_POS_Y + mirror.y * SQUARE_SIZE + SQUARE_SIZE * 0.5;
		image.angle = angle;
		image.png = _mirror_handle[ mirror.player_num ];
		image.size = FIELD_SIZE_RATE;

		if ( _blind_field ) {
			image.alpha = 0;
		}
		_drawer->setBackImage( image );
	}
}

void Field::drawDecisionButton( ) const {
	if ( isHitDecisionButton( ) ) {
		SetCursor( _cur_hand );
	}

	ImageProperty image = ImageProperty( );
	image.cx = _button_image.cx;
	image.cy = _button_image.cy;
	image.png = _button_image.png;
	if ( !_button_lighting ) {
		image.bright_flag = true;
		for ( int j = 0; j < 3; j++ ) {
			image.brt[ j ] = 130;
		}
	}

	_drawer->setBackImage( image );
}

void Field::drawMirror( ) const {
	//鏡描画
	for ( int i = 0; i < FIELD_ROW * FIELD_COL; i++ ) {
		if ( _mirrors.find( i ) == _mirrors.end( ) ) {
			continue;
		}
		Mirror mirror = _mirrors.find( i )->second;
		ImageProperty image = ImageProperty( );

		float angle = 0;
		if ( mirror.angle != RIGHT ) {
			angle = ( float )PI / 2;
		}
		image.cx = START_POS_X + mirror.x * SQUARE_SIZE + SQUARE_SIZE * 0.5;
		image.cy = START_POS_Y + mirror.y * SQUARE_SIZE + SQUARE_SIZE * 0.5;
		image.angle = angle;
		image.png = _mirror_handle[ mirror.player_num ];
		image.size = FIELD_SIZE_RATE;
		_drawer->setBackImage( image );
	}
}

void Field::drawPlayerPos( ) const {
	if ( _player_selected ) {
		return;
	}
	if ( _player_num < 0 ) {
		return;
	}
	
	for ( int i = 0; i < PLAYER_POSITION * 2; i++ ) {
		if ( _player_num != i / PLAYER_POSITION ) {
			continue;
		}
		ImageProperty image = ImageProperty( );
		double x = _select_player_pos[ i ].x;
		double y = _select_player_pos[ i ].y;
		image.png = _player_handle[ i / PLAYER_POSITION ];
		image.cx = x;
		image.cy = y;
		
		if ( _player_num == i / PLAYER_POSITION ) {
			if ( getTmpPlayerPoint( ) == i ) {		
				_drawer->setBackImage( image );
				continue;
			}

			if ( getPlayerPosHitNum( ) == i ) {
				_drawer->setFrontCircle( x, y, CIRCLE_SIZE, WHITE, 255, true );
				SetCursor( _cur_hand );
			} else {
				_drawer->setFrontCircle( x, y, CIRCLE_SIZE, WHITE, ( int )( ( sin( _data->getCount( ) * 0.06 ) + 1 ) * 64 + 50 ), true );
			}
		}
		_drawer->setFrontCircle( x, y, CIRCLE_SIZE, ( COLOR )( RED + i / PLAYER_POSITION ) );
	}
}

void Field::drawPlayer( ) const {
	if ( !_player_selected ) {
		return;
	}

	for ( int i = 0; i < PLAYER_NUM; i++ ) {
		if ( _dead_player == i ) {
			continue;
		}

		int pos = getPlayerPosIdx( i );
		if ( pos < 0 ) {
			continue;
		}

		double x = 0;
		double y = 0;

		x = _select_player_pos[ pos ].x;
		y = _select_player_pos[ pos ].y;

		ImageProperty image = ImageProperty( );
		image.png = _player_handle[ i ];
		image.cx = x;
		image.cy = y;
		image.size = FIELD_SIZE_RATE;
		_drawer->setBackImage( image );
	}
}

void Field::drawInfo( ) const {
	_drawer->setBackExtendImage( _board.image, ( float )_board.half_width, ( float )_board.half_height, 1.4, 2.5 );

	int y = INFO_Y;
	for ( int i = 0; i < INFO_TEXT_MAX; i++ ) {
		_drawer->setFrontString( true, BOARD_X, y, _info[ i ].col, _info[ i ].str, _info[ i ].size );
		y += _drawer->getStringH( _info[ i ].size ) + 3;
	}
}

void Field::drawRound( ) const {
	std::string str = "レーザーの移動まであと : ";
	const short int LENGTH = _drawer->getStringW( Drawer::NORMAL, str ) / 2;
	const short int POS_X = BOARD_X - LENGTH;
	const short int POS_Y = BOARD_Y + SQUARE_SIZE * 2;
	_drawer->setFrontString( false, POS_X, POS_Y, YELLOW, str );
	_drawer->setFrontString( false, POS_X + LENGTH * 2, POS_Y, RED, std::to_string( TURN_MAX - ( _turn - 1 ) % TURN_MAX ), Drawer::BIG );
}

void Field::drawSettingPlayer( ) {
	int x = START_POS_X + 60;
	int y = 0;
	int y_red = START_POS_Y + FIELD_ROW * SQUARE_SIZE + SQUARE_SIZE / 2;
	int y_blue = y_red + 30;
	int r = CIRCLE_SIZE / 3;
	int alpha = ( int )( ( sin( _data->getCount( ) * 0.06 ) + 1 ) * 64 + 50 );
	std::string str = "  ---- 少女思考中";
	std::string dot;
	switch ( _data->getCount( ) / FRAME % 4 ) {
		case 0:		dot = "";		break;
		case 1:		dot = ".";		break;
		case 2:		dot = "..";		break;
		case 3:		dot = "...";	break;
	}
	_drawer->setFrontCircle( x, y_red,  r, RED,  alpha, true );
	_drawer->setFrontCircle( x, y_blue, r, BLUE, alpha, true );
	if ( _order != _player_num ) {
		if( _player_num == 0 ){
			y = y_blue;
			_drawer->setFrontCircle( x, y_red, r, RED, 255, true );
		} else {
			y = y_red;
			_drawer->setFrontCircle( x, y_blue, r, BLUE, 255, true );
		}
	} else {
		if ( _player_num == 0 ) {
			y = y_red;
			_drawer->setFrontCircle( x, y_blue, r, BLUE, 255, true );
		} else {
			y = y_blue;
			_drawer->setFrontCircle( x, y_red, r, RED, 255, true );
		}
	}
	_drawer->setFrontString( false, x, y + 3, WHITE, str + dot, Drawer::LITTLE_BIG );
}

void Field::drawItem( ) const {
	//アイテム
	for ( int i = 0; i < ITEM_POSSESSION_MAX; i++ ) {
		if ( !_item[ i ].flag ) {
			continue;
		}

		ImageProperty image = ImageProperty( );
		image.cx = _item[ i ].x;
		image.cy = _item[ i ].y;
		image.flag = _item[ i ].flag;
		image.png = _item_handle[ _item[ i ].type ];
		image.alpha = 100;

		if ( _phase == SET_MIRROR_PHASE ) {
			if ( i == _select_item ) {
				image.alpha = 255;
			}
			if ( i == getHitItemIdx( ) ) {
				image.alpha = 255;
			}
		}

		_drawer->setBackImage( image );

		if ( !_first && _item[ i ].type == DOUBLE_MIRROR ) {
			image.png = _item_not_handle;
			image.alpha = 255;
			_drawer->setBackImage( image );
		}
	}
}

void Field::drawBackGround( ) const {
	_drawer->setBackGroundImage( _background );
}

void Field::drawMirrorCommand( ) const {
	if ( getHitMirrorCommandIdx( ) != -1 ) {
		SetCursor( _cur_hand );
	}

	for ( int i = 0; i < COMMAND_TYPE_MAX; i++ ) {
		ImageProperty image = ImageProperty( );
		image.cx = _mirror_cmd[ i ].image.cx;
		image.cy = _mirror_cmd[ i ].image.cy;
		image.png = _mirror_cmd[ i ].image.png;
		image.bright_flag = true;
		if ( i != getMirrorCommand( ) ) {
			for ( int j = 0; j < 3; j++ ) {
				image.brt[ j ] = NONACTIVE_BRIGHT;
			}
		}
		_drawer->setBackImage( image );
	}
}

void Field::drawDeathCount( ) const {
	ImageProperty image = ImageProperty( );
	const short int TIME_BOARD_X = WIDTH - 50;
	const short int TIME_BOARD_Y = 150;
	const short int TIME_STRING_X = WIDTH - 100;
	const short int TIME_STRING_Y = 100;
	const short int TIME_NUMBER_X = WIDTH - 130;
	const short int TIME_NUMBER_Y = 170;

	//カウント背景
	image.cx = TIME_BOARD_X;
	image.cy = TIME_BOARD_Y;
	image.png = _timeboard_handle;
	_drawer->setBackExtendImage( image, SQUARE_SIZE / 2, SQUARE_SIZE / 2, 3, 1.7 );

	//残り時間(文字)
	image = ImageProperty( );
	image.cx = TIME_STRING_X;
	image.cy = TIME_STRING_Y;
	image.size = 0.35;
	image.png = _time_string_handle;
	_drawer->setBackImage( image );


	//残り時間(数字)
	short int tmp = _dead_count / FRAME;
	std::string index = std::to_string( tmp );

	std::vector< int > number;
	std::vector< int > frame;
	short int length = ( short int )index.length( );

	for ( int i = 0; i < length; i++ ) {
		short int handle_idx = tmp % 10;
		number.push_back( _number_handle[ handle_idx * 2 ] );
		frame.push_back( _number_handle[ handle_idx * 2 + 1 ] );
		tmp /= 10;
	}

	for ( int i = 0; i < length; i++ ) {
		image = ImageProperty( );
		//フレーム
		image.cx = TIME_NUMBER_X + i * 50;
		image.cy = TIME_NUMBER_Y;
		image.png = frame[ length - i - 1 ];
		_drawer->setBackImage( image );

		//中身
		image.png = number[ length - i - 1 ];

		//残り2桁
		if ( length <= 2 ) {
			image.bright_flag = true;
			image.brt[ 0 ] = 255;
			image.brt[ 1 ] = 155;
			image.brt[ 2 ] = 0;

			//残り1桁
			if ( length <= 1 ) {
				image.brt[ 0 ] = 255;
				image.brt[ 1 ] = 0;
				image.brt[ 2 ] = 0;
			}
		}

		_drawer->setBackImage( image );
	}
}

void Field::drawItemCancelButton( ) const {
	if ( isHitItemCancelButton( ) ) {
		SetCursor( _cur_hand );
	}

	ImageProperty image = ImageProperty( );
	image.cx  = _item_cancel.image.cx;
	image.cy  = _item_cancel.image.cy;
	image.png = _item_cancel.image.png;
	image.bright_flag = true;

	short int brt = NONACTIVE_BRIGHT;
	if ( getSelectItem( ) != -1 || _tmp_mirror.flag ) {
		brt = 255;
	}

	for ( int i = 0; i < 3; i++ ) {
		image.brt[ i ] = brt;
	}

	_drawer->setBackImage( image );
}
