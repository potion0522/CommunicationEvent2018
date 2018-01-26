#include "Lazer.h"
#include "GlobalData.h"
#include "Drawer.h"
#include "Debug.h"
#include <math.h>

const double LAZER_SPEED = 1;

Lazer::Lazer( GlobalDataPtr data ) :
_data( data ) {
	setFlag( 1 );
	_drawer = _data->getDrawerPtr( );
	_field = _data->getFieldPtr( );
}

Lazer::~Lazer( ) {
}

std::string Lazer::getTag( ) {
	return "LAZER";
}

void Lazer::initialize( ) {
	_start = Field::Vector( );
	_dir_vec = Field::Vector( );
	_unit = Field::Vector( );
	_distance = 1;
	_start = _field->getLazerPoint( );
	updateUnitVector( );
}

void Lazer::update( ) {
	if ( _field->getPhase( ) < ATTACK_PHASE ) {
		return;
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
		_field->updateLazerVector( tmp );
		updateUnitVector( );
		_distance = _field->getDistance( );
		if ( _distance == DISTANCE_HALF + 1 ) {
			_start.x += _dir_vec.x;
			_start.y += _dir_vec.y;
			_dir_vec.x = 0;
			_dir_vec.y = 0;
		}
	}

	double draw_x = _start.x + _dir_vec.x;
	double draw_y = _start.y + _dir_vec.y;
	_drawer->setLine( _start.x, _start.y, draw_x, draw_y );

	DebugPtr debug = _data->getDebugPtr( );
	if ( debug->getFlag( ) ) {
		debug->setLine( WIDTH / 2, 0, WIDTH / 2, HEIGHT );
		debug->setLine( 0, HEIGHT / 2, WIDTH, HEIGHT / 2 );
		debug->addLog( "DISTANCE : " + std::to_string( _field->getDistance( ) ) );
	}
}

void Lazer::updateUnitVector( ) {
	_unit = _field->getNextDirect( );
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
	//Field::Vector normal = _field->getNormalVector( _unit.x, _unit.y );
//	Field::Vector ref = _unit.getReflection( normal );
//	updateUnitVector( ref );
	_dir_vec = { 0 };
}
