#include "Field.h"
#include "GlobalData.h"
#include "Drawer.h"
#include "const.h"
#include "Client.h"
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
}

Field::~Field( ) {
}

std::string Field::getTag( ) {
	return "FIELD";
}

void Field::initialize( ) {
	setFlag( 1 );
	_drawer = _data->getDrawerPtr( );
	_hit_mirror_num = -1;
	_distance = 0;
	_mouse_x = 0;
	_mouse_y = 0;
	_selected = false;
	_color = WHITE;
	_direct = DIR( );
	_dir_vec = Vector( );
	std::array< Mirror, MIRROR_MAX >( ).swap( _mirrors );
	_phase = SET_PLAYER_PHASE;
	_player_num = _data->getClientPtr( )->getPlayerNum( );
	for ( int i = 0; i < PLAYER_POSITION; i++ ) {
		if ( _player_num ) {
			_player_pos[ i ].x = PLAYER_POS_X;
			_player_pos[ i ].y = PLAYER_POS_Y + ( i + 1 ) * SQUARE_SIZE;
			_color = RED;
		} else {
			_player_pos[ i ].x = PLAYER_POS_X + ( i + 1 ) * SQUARE_SIZE;
			_player_pos[ i ].y = PLAYER_POS_Y;
			_color = BLUE;
		}
	}
}

void Field::update( ) {
	drawField( );
	drawPlayerPos( );
	if ( _phase == SET_PLAYER_PHASE ) {
		isHitPlayerPos( );
		drawPlayer( );
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
	bool result = false;
	if ( _data->getClickLeft( ) && !_selected ) {
		_mouse_x = _data->getMouseX( );
		_mouse_y = _data->getMouseY( );
		for ( int i = 0; i < PLAYER_POSITION; i++ ) {
			double x = _player_pos[ i ].x;
			double y = _player_pos[ i ].y;
			double distance = sqrt( ( _mouse_x - x ) * ( _mouse_x - x ) + ( _mouse_y - y ) * ( _mouse_y - y ) );
			if ( distance <= CIRCLE_SIZE ) {
				result = true;
				_selected = true;
			}
		}
	}
	return result;
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

void Field::drawMirror( ) const {
	//鏡描画
	for ( int i = 0; i <  ROW; i++ ) {
		for ( int j = 0; j < COL; j++ ) {
			int idx = j + COL * i;
			Vector m = Vector( );
			switch ( field[ idx ] ) {
			case 'R': 
				m.x = 1;
				m.y = 1;
				break;
			case 'L':
				m.x = -1;	
				m.y = 1;
				break;
			}
			if ( m.x != 0 ) {
				double line = SQUARE_SIZE / 2;
				double sx = START_POS_X + j * SQUARE_SIZE + SQUARE_SIZE * 0.5;
				double sy = START_POS_Y + i * SQUARE_SIZE + SQUARE_SIZE * 0.5;
				_drawer->setLine( sx, sy, sx + line * m.x, sy - line * m.y );
				_drawer->setLine( sx, sy, sx - line * m.x, sy + line * m.y );
			}
		}
	}
}

void Field::drawPlayerPos( ) const {
	for ( int i = 0; i < ROW; i++ ) {
		double x = PLAYER_POS_X;
		double y = PLAYER_POS_Y + ( i + 1 ) * SQUARE_SIZE;
		_drawer->setCircle( x, y, CIRCLE_SIZE, RED );
	}
	for ( int i = 0; i < COL; i++ ) {
		double x = PLAYER_POS_X + ( i + 1 ) * SQUARE_SIZE;
		double y = PLAYER_POS_Y;
		_drawer->setCircle( x, y, CIRCLE_SIZE, BLUE );
	}
}


void Field::drawPlayer( ) const {
	for ( int i = 0; i < PLAYER_POSITION; i++ ) {
		double x = _player_pos[ i ].x;
		double y = _player_pos[ i ].y;
		double distance = sqrt( ( _mouse_x - x ) * ( _mouse_x - x ) + ( _mouse_y - y ) * ( _mouse_y - y ) );
		if ( distance <= CIRCLE_SIZE ) {
			_drawer->setCircle( x, y, CIRCLE_SIZE / 2, _color );
		}
	}
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
	_player_no[ idx ] = pos;
}

void Field::setLazerPoint( ) {

}

void Field::setMirrorPoint( int player_num, int x, int y, MIRROR_ANGLE angle ) {
	int idx = getEmptyMirrorsIdx( );
	if ( idx < 0 ) {
		return;
	}

	_mirrors[ idx ].flag = true;
	_mirrors[ idx ].player_num = player_num;
	_mirrors[ idx ].x = x;
	_mirrors[ idx ].y = y;
	_mirrors[ idx ].angle = angle;
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

int Field::getPlayerPos( int idx ) const {
	return _player_no[ idx ];
}

int Field::getDistance( ) const {
	return _distance;
}

int Field::getHitMirrorIdx( ) const {
	return _hit_mirror_num;
}

bool Field::isMirror( ) const {
	if ( _hit_mirror_num != -1 ) {
		return true;
	}
	return false;
}

int Field::getEmptyMirrorsIdx( ) const {
	int idx = -1;
	for ( int i = 0; i < MIRROR_MAX; i++ ) {
		if ( _mirrors[ i ].flag != false ) {
			continue;
		}
		idx = i;
		break;
	}
	return idx;
}