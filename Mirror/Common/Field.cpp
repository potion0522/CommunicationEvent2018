#include "Field.h"
#include "GlobalData.h"
#include "Drawer.h"
#include "const.h"
#include "DxLib.h"
#include "Image.h"
#include <random>

const int MOUSE_R = 5;
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
	_player_selected = false;
	_mirror_selected = false;
	_reflection = false;
	_turn = 0;
	_dead_flag = -1;
	_hit_mirror_num = -1;
	_distance = 0;
	_player_num = -1;
	_player_pos_hit_num = -1;
	_field_pos_hit_num = -1;
	_lazer_pos = -1;
	_direct = DIR( );
	_dir_vec = Vector( );
	_tmp_mirror = Mirror( );
	std::map< int, Mirror >( ).swap( _mirrors );
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
	_decision_button.cx = 100;
	_decision_button.cy = 100;
	_decision_button.lx = _decision_button.cx - image.width / 2;
	_decision_button.rx = _decision_button.cx + image.width / 2;
	_decision_button.ly = _decision_button.cx - image.height / 2;
	_decision_button.ry = _decision_button.cx + image.height / 2;
	_decision_button.png = image.png;
}

void Field::nextRound( ) {
	_phase = SET_MIRROR_PHASE;
	_mirror_selected = false;
	_reflection = false;
	_turn = 0;
	_dead_flag = -1;
	_hit_mirror_num = -1;
	_distance = 0;
	_field_pos_hit_num = -1;
	_direct = DIR( );
	_dir_vec = Vector( );
	_tmp_mirror = Mirror( );
	std::array< int, 2 >( ).swap( _dir_board );
	std::map< int, Mirror >( ).swap( _mirrors );
}

void Field::update( ) {
	drawField( );
	drawPlayer( );
	drawArmament( );

	if ( _phase == SET_PLAYER_PHASE ) {
		drawPlayerPos( );
	}

	if ( _phase < SET_MIRROR_PHASE ) {
		return;
	}

	if ( !_mirror_selected ) {
		drawDecisionButton( );
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
	double mouse_x = ( double )_data->getMouseX( );
	double mouse_y = ( double )_data->getMouseY( );
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
	if ( !_tmp_mirror.flag ) {
		return false;
	}
	if ( _mirror_selected ) {
		return false;
	}
	double mouse_x = ( double )_data->getMouseX( );
	double mouse_y = ( double )_data->getMouseY( );

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

void Field::setTurn( int turn ) {
	_turn = turn;
}

void Field::setPlayerNum( int num ) {
	_player_num = num;
}

void Field::updateLazerVector( Vector vec ) {
	_dir_vec = vec;
	int x = ( int )( vec.x - START_POS_X );
	int y = ( int )( vec.y - START_POS_Y );

	if ( x < 0 || y < 0 || x > START_POS_X + SQUARE_SIZE * FIELD_COL || y > START_POS_Y + SQUARE_SIZE * FIELD_ROW ) {
		_distance = 1;
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

	x /= SQUARE_SIZE;
	y /= SQUARE_SIZE;

	if ( _distance == DISTANCE_HALF && !_reflection ) {
		std::map< int, Mirror >::iterator ite;
		ite = _mirrors.begin( );
		for ( ite; ite != _mirrors.end( ); ite++ ) {
			int mirror_x = ite->second.x;
			int mirror_y = ite->second.y;
			if ( mirror_x != x || mirror_y != y ) {
				continue;
			}
			MIRROR_ANGLE mirror_angle = ite->second.angle;
			Vector next_dir = Vector( );
			switch ( _direct ) {
			case DIR_UP :
				mirror_angle == RIGHT ? next_dir.x = 1 : next_dir.x = -1;
				break;
			case DIR_DOWN :
				mirror_angle == RIGHT ? next_dir.x = -1 : next_dir.x = 1;
				break;
			case DIR_RIGHT :
				mirror_angle == RIGHT ? next_dir.y = 1 : next_dir.y = -1;
				break;
			case DIR_LEFT :
				mirror_angle == RIGHT ? next_dir.y = -1 : next_dir.y = 1;
				break;			
			}
			setDirect( next_dir );
			_distance = DISTANCE_HALF + 1;
			_reflection = true;
			return;
		}

	}
	_dir_board[ 0 ] = x;
	_dir_board[ 1 ] = y;

	_reflection = false;

	_distance = DISTANCE;
	if ( field[ _dir_board[ 0 ] + _dir_board[ 1 ] * FIELD_COL ] != ' ' ) {
		_distance = DISTANCE_HALF;
	}
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
	_lazer_pos = pos;
	Vector dir = Vector( );
	if ( _lazer_pos < PLAYER_POSITION ) {
		dir.x = 1;
		dir.y = 0;
	} else {
		dir.x = 0;
		dir.y = -1;
	}
	setDirect( dir );
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
	Mirror mirror = {
		true,
		player_num,
		x,
		y,
		angle
	};

	if ( _mirrors.size( ) > MIRROR_MAX ) {
		return;
	}
	_mirrors[ idx ] = mirror;
}

void Field::playerPosSelected( ) {
	_player_selected = !_player_selected;
}

void Field::mirrorPosSelected( ) {
	_mirror_selected = !_mirror_selected;
}

Field::Vector Field::getLazerPoint( ) const {
	Vector vec = Vector( );
	if ( _lazer_pos < PLAYER_POSITION ) {
		vec.x = PLAYER_POS_X;
		vec.y = PLAYER_POS_Y + ( _lazer_pos + 1 ) * SQUARE_SIZE;
	} else {
		vec.x = PLAYER_POS_X + ( _lazer_pos % PLAYER_POSITION + 1 ) * SQUARE_SIZE;
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

BATTLE_PHASE Field::getPhase( ) const{
	return _phase;
}

int Field::getTurn( ) const {
	return _turn;
}

int Field::getPlayerPoint( int idx ) const {
	return _player_pos_no[ idx ];
}

int Field::getDistance( ) const {
	return _distance;
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
	if ( _lazer_pos < 0 ) {
		return;
	}

	double x = _select_player_pos[ _lazer_pos ].x;
	double y = _select_player_pos[ _lazer_pos ].y;

	_drawer->setCircle( x, y, CIRCLE_SIZE, PURPLE, 150, true );
}

void Field::drawTmpMirror( ) const {
	if ( getFieldPosHitNum( ) > 0 ) {
		int pos = getFieldPosHitNum( );
		int x = pos % FIELD_COL;
		int y = pos / FIELD_COL;
		double line = SQUARE_SIZE / 2;
		double sx = START_POS_X + x * SQUARE_SIZE + SQUARE_SIZE * 0.5;
		double sy = START_POS_Y + y * SQUARE_SIZE + SQUARE_SIZE * 0.5;

		COLOR col = ( _tmp_mirror.player_num == 0 ? RED : BLUE );
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
			if ( getPlayerPosHitNum( ) == i ) {
				_drawer->setCircle( x, y, CIRCLE_SIZE, WHITE, 255, true );
				SetCursor( _cur_hand );
			} else {
				_drawer->setBlinkCircle( x, y, CIRCLE_SIZE - 1 );
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