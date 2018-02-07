#include "Field.h"
#include "GlobalData.h"
#include "Drawer.h"
#include "const.h"
#include "DxLib.h"
#include "Image.h"
#include <random>

const int MOUSE_R = 5;
const int MIRROR_R = 10;
const int CIRCLE_SIZE = SQUARE_SIZE / 5 * 2;
const int PLAYER_POS_X = START_POS_X - SQUARE_SIZE / 2;
const int PLAYER_POS_Y = START_POS_Y - SQUARE_SIZE / 2;
const int PLAYER_SIZE = CIRCLE_SIZE / 2;

char field[ FIELD_COL * FIELD_ROW + 1 ] = 
"     "
"     "
"     "
"     "
"     ";

Field::Field( GlobalDataPtr data ) :
_data( data ) {
	setFlag( 1 );
	_drawer = _data->getDrawerPtr( );
	_image = _data->getImagePtr( );
	_cur_hand  = LoadCursor( NULL, IDC_HAND );
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
	_reflection = false;
	_info_idx = 0;
	_tmp_player_pos = -1;
	_dead_flag = -1;
	_hit_mirror_num = -1;
	_player_num = -1;
	_player_pos_hit_num = -1;
	_field_pos_hit_num = -1;
	_lazer_point_idx = -1;
	_direct = DIR( );
	_tmp_mirror = Mirror( );
	std::map< int, Mirror >( ).swap( _mirrors );
	std::array< Info, INFO_TEXT_MAX >( ).swap( _info );
	_reflection_point = Vector( );
	_phase = SET_PLAYER_PHASE;
	for ( int i = 0; i < PLAYER_POSITION * 2; i++ ) {
		if ( i < PLAYER_POSITION ) {
			_select_player_pos[ i ].x = PLAYER_POS_X;
			_select_player_pos[ i ].y = PLAYER_POS_Y + ( i + 1 ) * SQUARE_SIZE;
		} else {
			_select_player_pos[ i ].x = PLAYER_POS_X + ( i - PLAYER_POSITION + 1 ) * SQUARE_SIZE;
			_select_player_pos[ i ].y = PLAYER_POS_Y;
		}
	}
	for ( int i = 0; i < PLAYER_NUM; i++ ) {
		_player_pos_no[ i ] = -1;
		_player_color[ i ] = ( COLOR )( i + ( int )RED );
	}

	Png image = _image->getPng( BUTTON_IMAGE, 0 );
	_decision_button = ImageProperty( );
	_decision_button.cx = WIDTH / 5;
	_decision_button.cy = HEIGHT / 5;
	_decision_button.lx = _decision_button.cx - image.width / 2;
	_decision_button.rx = _decision_button.cx + image.width / 2;
	_decision_button.ly = _decision_button.cy - image.height / 2;
	_decision_button.ry = _decision_button.cy + image.height / 2;
	_decision_button.png = image.png;
}

void Field::nextTurn( ) {
	_phase = SET_MIRROR_PHASE;
	_mirror_selected = false;
	_reflection = false;
	_dead_flag = -1;
	_hit_mirror_num = -1;
	_field_pos_hit_num = -1;
	_direct = DIR( );
	_tmp_mirror = Mirror( );
	_reflection_point = Vector( );
}

void Field::update( ) {
	drawField( );
	drawPlayer( );
	drawArmament( );
	drawDecisionButton( );
	drawInfo( );
	drawRound( );
	resetInfo( );

	if ( _phase == SET_PLAYER_PHASE ) {
		drawPlayerPos( );
	}

	if ( _phase < SET_MIRROR_PHASE ) {
		return;
	}

	if ( !_mirror_selected ) {
		drawTmpMirror( );
	}
	drawMirror( );
	if ( _phase < ATTACK_PHASE ) {
		return;
	}
}

bool Field::isHitPlayerPos( ) {
	if ( _player_num < 0 ) {
		return false;
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
			return true;
		}
	}
	_player_pos_hit_num = -1;
	return false;
}

bool Field::isSelectedPlayer( ) const {
	return _player_selected;
}

bool Field::isHitFieldPos( ) {
	if ( _player_num < 0 ) {
		return false;
	}
	if ( _mirror_selected ) {
		return false;
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
				return true;
			}
		}
	}
	_field_pos_hit_num = -1;
	return false;
}

bool Field::isHitDecisionButton( ) const {
	int mouse_x = _data->getMouseX( );
	int mouse_y = _data->getMouseY( );

	if ( _decision_button.lx < mouse_x && mouse_x < _decision_button.rx &&
		 _decision_button.ly < mouse_y && mouse_y < _decision_button.ry ) {
		return true;
	}

	return false;
}

bool Field::isSelectedMirror( ) const {
	return _mirror_selected;
}

int Field::getDeadPlayer( ) const {
	return _dead_flag;
}

int Field::getLazerPointIdx( ) const {
	return _lazer_point_idx;
}

void Field::setTurn( int turn ) {
	_turn = turn;
}

void Field::setPlayerNum( int num ) {
	_player_num = num;
}

void Field::setInfoText( std::string str, COLOR col ) {
	_info[ _info_idx ].str = str;
	_info[ _info_idx ].col = col;
	_info_idx = ( _info_idx + 1 ) % INFO_TEXT_MAX;
}

void Field::updateLazerVector( Vector vec ) {
	int x = ( int )( vec.x - START_POS_X );
	int y = ( int )( vec.y - START_POS_Y );

	if ( x < 0 || y < 0 || x > START_POS_X + SQUARE_SIZE * FIELD_COL || y > START_POS_Y + SQUARE_SIZE * FIELD_ROW ) {
		//ÉvÉåÉCÉÑÅ[ÇÃìñÇΩÇËîªíË
		for ( int i = 0; i < PLAYER_NUM; i++ ) {
			int pos = getPlayerPoint( i );
			int player_x = ( int )_select_player_pos[ pos ].x;
			int player_y = ( int )_select_player_pos[ pos ].y;
			int a = ( int )vec.x - player_x;
			int b = ( int )vec.y - player_y;
			if ( sqrt( a * a + b * b ) <= PLAYER_SIZE + 1 ) {
				_dead_flag = i;
				break;
			}
		}
		return;
	}

	//ãæ
	std::map< int, Mirror >::iterator ite;
	ite = _mirrors.begin( );

	bool already = false;
	for ( ite; ite != _mirrors.end( ); ite++ ) {
		double mirror_x = START_POS_X + ite->second.x * SQUARE_SIZE + SQUARE_SIZE * 0.5;
		double mirror_y = START_POS_Y + ite->second.y * SQUARE_SIZE + SQUARE_SIZE * 0.5;
		double lazer_r = 1;

		double a = mirror_x - vec.x;
		double b = mirror_y - vec.y;
		double c = a * a + b * b;
		if ( c <= ( lazer_r + MIRROR_R ) * ( lazer_r + MIRROR_R ) ) {
			already = true;
			Vector next_dir = Vector( );
			switch ( _direct ) {
			case DIR_UP :
				ite->second.angle == RIGHT ? next_dir.x = 1 : next_dir.x = -1;
				break;
			case DIR_DOWN :
				ite->second.angle == RIGHT ? next_dir.x = -1 : next_dir.x = 1;
				break;
			case DIR_RIGHT :
				ite->second.angle == RIGHT ? next_dir.y = 1 : next_dir.y = -1;
				break;
			case DIR_LEFT :
				ite->second.angle == RIGHT ? next_dir.y = -1 : next_dir.y = 1;
				break;			
			}
			if ( !_reflection ) {
				setDirect( next_dir );
				_reflection_point.x = mirror_x;
				_reflection_point.y = mirror_y;
				_reflection = true;
				break;
			}
		}
	}
	if ( !already ) {
		_reflection_point = Vector( );
		_reflection = false;
	}
}

void Field::resetInfo( ) {
	std::array< Info, INFO_TEXT_MAX >( ).swap( _info );
	_info_idx = 0;
}

void Field::setDirect( Vector vec ) {
	if ( vec.x != 0 ) {
		if ( vec.x < 0 ) {
			_direct = DIR_LEFT;
		} else {
			_direct = DIR_RIGHT;
		}
	}

	if ( vec.y != 0 ) {
		if ( vec.y < 0 ) {
			_direct = DIR_DOWN;
		} else {
			_direct = DIR_UP;
		}	
	}
}

void Field::setPhase( BATTLE_PHASE phase ) {
	_phase = phase;
}

void Field::setPlayerPoint( int idx, int pos ) {
	_player_pos_no[ idx ] = pos;
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
	setDirect( dir );
}

void Field::setTmpPlayerPoint( ) {
	_tmp_player_pos = getPlayerPosHitNum( );
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
	field[ x + y * FIELD_COL ] = ( angle == RIGHT ? 'R' : 'L' );
	Mirror mirror = { true, player_num, x, y, angle };

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
	_mirror_selected = !_mirror_selected;
}

Field::Vector Field::getLazerPoint( ) const {
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

Field::Vector Field::getNextDirect( ) const {
	Vector vec = Vector( );
	switch ( _direct ) {
	case DIR_UP :
		vec.x = 0;
		vec.y = 1;
		break;
	case DIR_DOWN :
		vec.x = 0;
		vec.y = -1;
		break;
	case DIR_RIGHT :
		vec.x = 1;
		vec.y = 0;
		break;
	case DIR_LEFT :
		vec.x = -1;
		vec.y = 0;
		break;
	}

	return vec;
}

Field::Vector Field::getReflectionPoint( ) const {
	return _reflection_point;
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

int Field::getPlayerPoint( int idx ) const {
	return _player_pos_no[ idx ];
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

void Field::drawField( ) const {
	//ÉtÉBÅ[ÉãÉhï`âÊ
	for ( int i = 0; i < FIELD_ROW + 1; i++ ) {
		int sx = START_POS_X;
		int sy = START_POS_Y + i * SQUARE_SIZE;
		int ex = START_POS_X + SQUARE_SIZE * FIELD_COL;
		int ey = sy;
		_drawer->setLine( sx, sy, ex, ey, YELLOW );
	}
	for ( int i = 0; i < FIELD_COL + 1; i++ ) {
		int sx = START_POS_X + i * SQUARE_SIZE;
		int sy = START_POS_Y;
		int ex = sx;
		int ey = sy + SQUARE_SIZE * FIELD_ROW;
		_drawer->setLine( sx, sy, ex, ey, YELLOW );
	}
}

void Field::drawArmament( ) const {
	//ñCë‰ï`âÊ
	if ( _lazer_point_idx < 0 ) {
		return;
	}

	double x = _select_player_pos[ _lazer_point_idx ].x;
	double y = _select_player_pos[ _lazer_point_idx ].y;

	_drawer->setCircle( x, y, CIRCLE_SIZE, PURPLE, 150, true );
}

void Field::drawTmpMirror( ) const {
	if ( getFieldPosHitNum( ) != -1 ) {
		int pos = getFieldPosHitNum( );
		int x = pos % FIELD_COL;
		int y = pos / FIELD_COL;
		double line = SQUARE_SIZE / 2;
		double sx = START_POS_X + x * SQUARE_SIZE + SQUARE_SIZE * 0.5;
		double sy = START_POS_Y + y * SQUARE_SIZE + SQUARE_SIZE * 0.5;

		COLOR col = ( _player_num == 0 ? RED : BLUE );
		_drawer->setLine( sx, sy, sx + line * 1, sy - line * 1, col, 150 );
		_drawer->setLine( sx, sy, sx - line * 1, sy + line * 1, col, 150 );
	}

	if ( !_tmp_mirror.flag ) {
		return;
	}

	//ãæï`âÊ
	for ( int i = 0; i < FIELD_ROW; i++ ) {
		for ( int j = 0; j < FIELD_COL; j++ ) {
			if ( _tmp_mirror.x != j || _tmp_mirror.y != i ) {
				continue;
			}
			Vector angle = Vector( );
			switch ( _tmp_mirror.angle ) {
				case RIGHT: 
					angle.x = 1;
					angle.y = 1;
					break;
				case LEFT:
					angle.x = -1;	
					angle.y = 1;
					break;
			}
			double line = SQUARE_SIZE / 2;
			double sx = START_POS_X + _tmp_mirror.x * SQUARE_SIZE + SQUARE_SIZE * 0.5;
			double sy = START_POS_Y + _tmp_mirror.y * SQUARE_SIZE + SQUARE_SIZE * 0.5;

			COLOR col = ( _tmp_mirror.player_num == 0 ? RED : BLUE );
			_drawer->setLine( sx, sy, sx + line * angle.x, sy - line * angle.y, col );
			_drawer->setLine( sx, sy, sx - line * angle.x, sy + line * angle.y, col );
		}
	}
}

void Field::drawDecisionButton( ) const {
	_drawer->setImage( _decision_button );
}

void Field::drawMirror( ) const {
	//ãæï`âÊ
	for ( int i = 0; i < FIELD_ROW * FIELD_COL; i++ ) {
		if ( _mirrors.find( i ) == _mirrors.end( ) ) {
			continue;
		}
		Mirror mirror = _mirrors.find( i )->second;
		Vector angle = Vector( );
				switch ( mirror.angle ) {
				case RIGHT: 
					angle.x = 1;
					angle.y = 1;
					break;
				case LEFT:
					angle.x = -1;	
					angle.y = 1;
					break;
			}

			double line = SQUARE_SIZE / 2;
			double sx = START_POS_X + mirror.x * SQUARE_SIZE + SQUARE_SIZE * 0.5;
			double sy = START_POS_Y + mirror.y * SQUARE_SIZE + SQUARE_SIZE * 0.5;

			COLOR col = ( mirror.player_num == 0 ? RED : BLUE );
			_drawer->setLine( sx, sy, sx + line * angle.x, sy - line * angle.y, col );
			_drawer->setLine( sx, sy, sx - line * angle.x, sy + line * angle.y, col );
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
		double x = _select_player_pos[ i ].x;
		double y = _select_player_pos[ i ].y;
		if ( _player_num == i / PLAYER_POSITION ) {
			if ( getTmpPlayerPoint( ) == i ) {
				_drawer->setCircle( x, y, PLAYER_SIZE, WHITE, 150, true );
				_drawer->setCircle( x, y, PLAYER_SIZE, _player_color[ i / PLAYER_POSITION ] );
				continue;
			}

			if ( getPlayerPosHitNum( ) == i ) {
				_drawer->setCircle( x, y, CIRCLE_SIZE, WHITE, 255, true );
				SetCursor( _cur_hand );
			} else {
				//_drawer->setBlinkCircle( x, y, CIRCLE_SIZE - 1 );
				SetDrawBlendMode( DX_BLENDMODE_ALPHA, ( int )( ( sin( _data->getCount( ) * 0.06 ) + 1 ) * 64 + 156 ) );//ìßñæâª
				_drawer->setCircle( x, y, CIRCLE_SIZE, WHITE, 255, true );
				SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
			}
		}
		_drawer->setCircle( x, y, CIRCLE_SIZE, ( COLOR )( RED + i / PLAYER_POSITION ) );
	}
}

void Field::drawPlayer( ) const {
	if ( !_player_selected ) {
		return;
	}

	for ( int i = 0; i < PLAYER_NUM; i++ ) {
		int pos = getPlayerPoint( i );
		if ( pos < 0 ) {
			continue;
		}

		double x = 0;
		double y = 0;

		x = _select_player_pos[ pos ].x;
		y = _select_player_pos[ pos ].y;

		_drawer->setCircle( x, y, PLAYER_SIZE, WHITE, 150, true );
		_drawer->setCircle( x, y, PLAYER_SIZE, _player_color[ i ] );
	}
}

void Field::drawInfo( ) const {
	int lx = 50;
	int rx = 500;
	int ly = 300;
	int ry = 500;

	DrawBox( lx, ly, rx, ry, 0xffffff, FALSE );

	for ( int i = 0; i < INFO_TEXT_MAX; i++ ) {
		double x = lx + ( rx - lx ) / 2;
		double y = ly + 20 + i * 30;
		_drawer->setString( true, x, y, _info[ i ].col, _info[ i ].str );
	}
}

void Field::drawRound( ) const {
	_drawer->setString( false, 20, 20, RED, "ROUND : " + std::to_string( _turn / TURN_MAX ) + "  TURN : " + std::to_string( _turn ), Drawer::BIG );
}