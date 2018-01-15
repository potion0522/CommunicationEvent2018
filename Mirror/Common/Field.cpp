#include "Field.h"
#include "GlobalData.h"
#include "Drawer.h"
#include "const.h"

const int ROW = 6;
const int COL = 6;
const int SQUARE_SIZE = 96;
const int START_POS_X = WIDTH / 2 - SQUARE_SIZE * COL / 2;
const int START_POS_Y = HEIGHT / 2 - SQUARE_SIZE * ROW / 2;
const int DISTANCE = SQUARE_SIZE;
const int DISTANCE_HALF = SQUARE_SIZE / 2;

char field[ COL * ROW + 1 ] = 
"      "
"      "
"      "
"      "
"      "
"      ";

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
	const int INIT_X = ( WIDTH - COL * SQUARE_SIZE ) / 2;

	_mirrors[ 0 ].flag = 1;
	_mirrors[ 0 ].x = 3;
	_mirrors[ 0 ].y = 0;
	_mirrors[ 0 ].normal.x = -1;
	_mirrors[ 0 ].normal.y = -1;
	_mirrors[ 0 ].angle = LEFT;

	_mirrors[ 1 ].flag = 1;
	_mirrors[ 1 ].x = 2;
	_mirrors[ 1 ].y = 0;
	_mirrors[ 1 ].normal.x = 1;
	_mirrors[ 1 ].normal.y = -1;
	_mirrors[ 1 ].angle = RIGHT;

	for ( int i = 0; i < MIRROR_MAX; i++ ) {
		if ( _mirrors[ i ].flag ) {
			char c = ' ';
			switch ( _mirrors[ i ].angle ) {
			case RIGHT	: c = 'R'; break;
			case LEFT	: c = 'L'; break;
			}
			field[ ( int )( _mirrors[ i ].x + _mirrors[ i ].y * COL ) ] = c;
		}
	}

	_hit_mirror_num = -1;
	_distance = 0;
	_direct = DIR( );
	_dir_vec = Vector( );
}

void Field::update( ) {
	_hit_mirror_num = -1;
	for ( int i = 0; i < MIRROR_MAX; i++ ) {
		double pos_x = START_POS_X + _mirrors[ i ].x * SQUARE_SIZE + SQUARE_SIZE * 0.5;
		double pos_y = START_POS_Y + _mirrors[ i ].y * SQUARE_SIZE + SQUARE_SIZE * 0.5;
		if ( ( int )( _dir_vec.x - pos_x ) == 0 && ( int )( _dir_vec.y - pos_y ) == 0 ) {
			_hit_mirror_num = i;
			break;
		}
	}

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

void Field::setLazerPoint( ) {
}

void Field::setMirrorPoint( ) {

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
	if ( isMirror( ) ) {
		vec = _mirrors[ _hit_mirror_num ].normal;
	}
	return vec;
}

int Field::getDistance( ) const {
	return _distance;
}

bool Field::isMirror( ) const {
	if ( _hit_mirror_num != -1 ) {
		return true;
	}
	return false;
}

int Field::getHitMirrorIdx( ) const {
	return _hit_mirror_num;
}