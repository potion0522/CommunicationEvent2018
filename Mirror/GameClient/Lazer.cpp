#include "Lazer.h"
#include "GlobalData.h"
#include "Drawer.h"
#include "Debug.h"
#include <math.h>

const double LAZER_SPEED = 0.5;

Lazer::Lazer( GlobalDataPtr data ) :
_data( data ) {
}

Lazer::~Lazer( ) {
}

std::string Lazer::getTag( ) {
	return "LAZER";
}

void Lazer::initialize( ) {
	setFlag( 1 );
	_start = Field::Vector( );
	_dir_vec = Field::Vector( );
	_unit = Field::Vector( );
	_distance = 0;
	_drawer = _data->getDrawerPtr( );
	_field = _data->getFieldPtr( );
	_start = _field->getLazerPoint( );
	_dir_vec = _field->getLazerVector( );
	updateUnitVector( _dir_vec );
}

void Lazer::update( ) {
	if ( _field->isMirror( ) ) {
		updateLazer( );
	}

	double x = _unit.x * LAZER_SPEED;
	double y = _unit.y * LAZER_SPEED;
	_dir_vec.x += x;
	_dir_vec.y -= y;

	double length = sqrt( x * x + y * y );
	_distance -= length;
	if ( _distance <= 0 ) {
		_dir_vec.x -= _unit.x * fabsf( ( float )_distance );
		_dir_vec.y += _unit.y * fabsf( ( float )_distance );

		Field::Vector tmp = { _start.x + _dir_vec.x, _start.y + _dir_vec.y };
		_field->setDirect( _unit );
		_field->setLazerVector( tmp );

		_distance = _field->getDistance( );
	}

	double draw_x = _start.x + _dir_vec.x;
	double draw_y = _start.y + _dir_vec.y;
	_drawer->setLine( _start.x, _start.y, draw_x, draw_y );

	DebugPtr debug = _data->getDebugPtr( );
	if ( debug->getFlag( ) ) {
		debug->setLine( WIDTH / 2, 0, WIDTH / 2, HEIGHT );
		debug->setLine( 0, HEIGHT / 2, WIDTH, HEIGHT / 2 );
		debug->addLog( "DISTANCE : " + std::to_string( _distance ) );
	}
}

void Lazer::updateUnitVector( Field::Vector vec ) {
	double a = vec.x * vec.x;
	double b = vec.y * vec.y;
	double length = sqrt( a + b );
	_unit.x = vec.x / length;
	_unit.y = vec.y / length;
}

void Lazer::updateLazer( ) {
	updateStartPos( );
	convReflectionVector( );
}

void Lazer::updateStartPos( ) {
	_start.x = ( int )( _start.x + _dir_vec.x );
	_start.y = ( int )( _start.y + _dir_vec.y );
}

void Lazer::convReflectionVector( ) {
	Field::Vector normal = _field->getNormalVector( _unit.x, _unit.y );
	Field::Vector ref = _unit.getReflection( normal );
	updateUnitVector( ref );
	_dir_vec = { 0 };
}
