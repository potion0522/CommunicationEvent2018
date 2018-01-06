#include "Field.h"
#include "GlobalData.h"
#include "Drawer.h"

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
}

void Field::update( ) {
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
	Vector vec = { -1, -1 };
	return vec;
}

bool Field::isHitMirror( double x, double y ) const {
	if ( x == WIDTH / 2 && y == HEIGHT / 2 ) {
		return true;
	}
	return false;
}