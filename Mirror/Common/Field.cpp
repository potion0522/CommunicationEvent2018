#include "Field.h"
#include "GlobalData.h"
#include "Drawer.h"

const int ROW = 6;
const int COL = 6;
const int SQUARE_SIZE = 64;

const char field[ COL * ROW + 1 ] = 
""
""
""
""
""
""
;

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

	_mirrors[ 0 ].x = WIDTH / 2;
	_mirrors[ 0 ].y = SQUARE_SIZE;
	_mirrors[ 0 ].normal.x = -1;
	_mirrors[ 0 ].normal.y = -1;

	_mirrors[ 1 ].x = WIDTH / 3;
	_mirrors[ 1 ].y = SQUARE_SIZE;
	_mirrors[ 1 ].normal.x = 1;
	_mirrors[ 1 ].normal.y = 1;

	_hit_mirror_num = -1;

	_dir_vec = Vector( );
}

void Field::update( ) {
	_hit_mirror_num = -1;
	for ( int i = 0; i < MIRROR_MAX; i++ ) {
		if ( ( int )( _dir_vec.x - _mirrors[ i ].x ) == 0 && ( int )( _dir_vec.y - _mirrors[ i ].y ) == 0 ) {
			_hit_mirror_num = i;
			break;
		}
	}
}

void Field::setDirLazerVector( Vector vec ) {
	_dir_vec = vec;
}

void Field::setLazerPoint( ) {
}

void Field::setMirrorPoint( ) {

}

Field::Vector Field::getLazerPoint( ) const {
	Vector vec = { WIDTH / 2, HEIGHT };
	return vec;
}

Field::Vector Field::getLazerVector( ) const {
	Vector vec = { 0, 1 };
	return vec;
}

Field::Vector Field::getNormalVector( double x, double y ) const {
	Vector vec = Vector( );
	if ( isHitMirror( ) ) {
		vec = _mirrors[ _hit_mirror_num ].normal;
	}
	return vec;
}

bool Field::isHitMirror( ) const {
	if ( _hit_mirror_num != -1 ) {
		return true;
	}
	return false;
}

int Field::getHitMirrorIdx( ) const {
	return _hit_mirror_num;
}