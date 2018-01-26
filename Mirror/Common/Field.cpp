#include "Field.h"
#include "GlobalData.h"
#include "Drawer.h"
#include "const.h"
#include "DxLib.h"
#include <random>

const int MOUSE_R = 5;
const int ROW = 5;
const int COL = 5;
const int SQUARE_SIZE = 96;
const int CIRCLE_SIZE = SQUARE_SIZE / 5 * 2;
const int START_POS_X = WIDTH / 3 * 2 - SQUARE_SIZE * COL / 2;
const int START_POS_Y = HEIGHT / 2 - SQUARE_SIZE * ROW / 2;
const int PLAYER_POS_X = START_POS_X - SQUARE_SIZE / 2;
const int PLAYER_POS_Y = START_POS_Y - SQUARE_SIZE / 2;
const int DISTANCE = SQUARE_SIZE;
const int DISTANCE_HALF = SQUARE_SIZE / 2;

char field[ COL * ROW + 1 ] = 
"     "
"     "
"     "
"     "
"     ";

Field::Field( GlobalDataPtr data ) :
_data( data ) {
	setFlag( 1 );
	_drawer = _data->getDrawerPtr( );
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
	_turn = 0;
	_hit_mirror_num = -1;
	_distance = 0;
	_player_num = -1;
	_player_pos_hit_num = -1;
	_field_pos_hit_num = -1;
	_lazer_pos = -1;
	_direct = DIR( );
	_dir_vec = Vector( );
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
	for ( int i = 0; i < ROW; i++ ) {
		for ( int j = 0; j < COL; j++ ) {
			double table_left = START_POS_X + j * SQUARE_SIZE;
			double table_top  = START_POS_Y + i * SQUARE_SIZE;
			if ( table_left < mouse_x && mouse_x   < table_left + SQUARE_SIZE &&
				 table_top  < mouse_y && mouse_y < table_top  + SQUARE_SIZE ) {
				_field_pos_hit_num = j + i * COL;
				return true;
			}
		}
	}
	_field_pos_hit_num = -1;
	return false;
}

bool Field::isSelectedMirror( ) const {
	return _mirror_selected;
}

void Field::drawField( ) const {
	//フィールド描画
	for ( int i = 0; i < ROW + 1; i++ ) {
		int sx = START_POS_X;
		int sy = START_POS_Y + i * SQUARE_SIZE;
		int ex = START_POS_X + SQUARE_SIZE * COL;
		int ey = sy;
		_drawer->setLine( sx, sy, ex, ey, YELLOW );
	}
	for ( int i = 0; i < COL + 1; i++ ) {
		int sx = START_POS_X + i * SQUARE_SIZE;
		int sy = START_POS_Y;
		int ex = sx;
		int ey = sy + SQUARE_SIZE * ROW;
		_drawer->setLine( sx, sy, ex, ey, YELLOW );
	}
}

void Field::drawArmament( ) const {
	//砲台描画
	if ( _lazer_pos < 0 ) {
		return;
	}

	double x = _select_player_pos[ _lazer_pos ].x;
	double y = _select_player_pos[ _lazer_pos ].y;

	_drawer->setCircle( x, y, CIRCLE_SIZE, PURPLE, 150, true );
}

void Field::drawMirror( ) const {
	//鏡描画
	for ( int i = 0; i < ROW * COL; i++ ) {
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

	//int mirror_num = 0;
	//for ( int i = 0; i < ROW; i ++ ) {
	//	for ( int j = 0; j < COL; j++ ) {
	//		int idx = i * COL + j;
	//		if ( field[ idx ] == ' ' ) {
	//			continue;
	//		}
	//		Vector angle = Vector( );
	//			switch ( field[ idx ] ) {
	//			case 'R': 
	//				angle.x = 1;
	//				angle.y = 1;
	//				break;
	//			case 'L':
	//				angle.x = -1;	
	//				angle.y = 1;
	//				break;
	//		}

	//		double line = SQUARE_SIZE / 2;
	//		double sx = START_POS_X + j * SQUARE_SIZE + SQUARE_SIZE * 0.5;
	//		double sy = START_POS_Y + i * SQUARE_SIZE + SQUARE_SIZE * 0.5;

	//		COLOR col = ( _mirrors.find( idx )->second.player_num == 0 ? RED : BLUE );
	//		_drawer->setLine( sx, sy, sx + line * angle.x, sy - line * angle.y, col );
	//		_drawer->setLine( sx, sy, sx - line * angle.x, sy + line * angle.y, col );
	//		mirror_num++;
	//	}
	//}
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

		_drawer->setCircle( x, y, CIRCLE_SIZE / 2, WHITE, 150, true );
		_drawer->setCircle( x, y, CIRCLE_SIZE / 2, _player_color[ i ] );
	}
}

void Field::setTurn( int turn ) {
	_turn = turn;
}

void Field::setPlayerNum( int num ) {
	_player_num = num;
}

void Field::setLazerVector( Vector vec ) {
	_dir_vec = vec;
	int x = ( int )( vec.x - START_POS_X ) / SQUARE_SIZE;
	int y = ( int )( vec.y - START_POS_Y ) / SQUARE_SIZE;
	if ( vec.x - START_POS_X > 0 && ( int )( vec.x - START_POS_X ) % SQUARE_SIZE == 0 ) {
		x--;
	}
	if ( vec.y - START_POS_Y > 0 && ( int )( vec.y - START_POS_Y ) % SQUARE_SIZE == 0 ) {
		y--;
	}

	if ( x != _dir_board[ 0 ] || y != _dir_board[ 1 ] ) {
		_dir_board[ 0 ] = x;
		_dir_board[ 1 ] = y;

		std::array< int, 2 > next_board;
		switch ( _direct ) {
		case DIR_UP :
			next_board[ 0 ] = _dir_board[ 0 ];
			next_board[ 1 ] = _dir_board[ 1 ] - 1;
			break;
		case DIR_DOWN :
			next_board[ 0 ] = _dir_board[ 0 ];
			next_board[ 1 ] = _dir_board[ 1 ] + 1;
			break;
		case DIR_RIGHT :
			next_board[ 0 ] = _dir_board[ 0 ] + 1;
			next_board[ 1 ] = _dir_board[ 1 ];
			break;
		case DIR_LEFT :
			next_board[ 0 ] = _dir_board[ 0 ] - 1;
			next_board[ 1 ] = _dir_board[ 1 ];
			break;
		}

		//進行先がフィールド外であれば
		if ( next_board[ 0 ] < 0 || next_board[ 1 ] < 0 ) {
			return;
		}

		_distance = DISTANCE;
		if ( field[ next_board[ 0 ] + next_board[ 1 ] * COL ] != ' ' ) {
			_distance = DISTANCE_HALF;
		}
	}
}

void Field::setDirect( Vector vec ) {
	if ( vec.x < 0 ) {
		_direct = DIR_LEFT;
	} else {
		_direct = DIR_RIGHT;
	}

	if ( vec.y < 0 ) {
		_direct = DIR_DOWN;
	} else {
		_direct = DIR_UP;
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
}

void Field::setMirrorPoint( int player_num, int x, int y, MIRROR_ANGLE angle ) {
	int idx = x + y * COL;
	field[ x + y * COL ] = ( angle == RIGHT ? 'R' : 'L' );
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
	Vector vec = { START_POS_X + 3 * SQUARE_SIZE + SQUARE_SIZE * 0.5, START_POS_Y + SQUARE_SIZE * ROW };
	return vec;
}

Field::Vector Field::getLazerVector( ) const {
	Vector vec = { 0, 1 };
	return vec;
}

Field::Vector Field::getNormalVector( double x, double y ) const {
	Vector vec = Vector( );
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

bool Field::isMirror( ) const {
	if ( _hit_mirror_num != -1 ) {
		return true;
	}
	return false;
}