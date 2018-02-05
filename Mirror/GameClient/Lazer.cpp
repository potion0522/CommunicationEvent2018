#include "Lazer.h"
#include "GlobalData.h"
#include "Drawer.h"
#include "Debug.h"
#include "Image.h"
#include <math.h>

const double LAZER_SPEED = 5;

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
	_fin = false;
	_distance = 1;
	_start = Field::Vector( );
	_dir_vec = Field::Vector( );
	_unit = Field::Vector( );
	_start = _field->getLazerPoint( );
	updateUnitVector( );

	ImagePtr image_ptr = _data->getImagePtr( );
	_lazer_image = image_ptr->getPng( BATTLE_IMAGE, 0 ).png;
}

void Lazer::update( ) {
	if ( _field->getPhase( ) < ATTACK_PHASE ) {
		return;
	}
	if ( _fin ) {
		return;
	}
	if ( _dir_vec.x + _start.x > START_POS_X + SQUARE_SIZE * FIELD_COL + SQUARE_SIZE ||
		 _dir_vec.x + _start.x < START_POS_X - SQUARE_SIZE ||
		 _dir_vec.y + _start.y > START_POS_Y + SQUARE_SIZE * FIELD_ROW + SQUARE_SIZE ||
		 _dir_vec.y + _start.y < START_POS_Y - SQUARE_SIZE ) {
		_fin = true;
		return;
	}

	double x = _unit.x * LAZER_SPEED;
	double y = _unit.y * LAZER_SPEED;
	_dir_vec.x += x;
	_dir_vec.y -= y;

	Field::Vector tmp = { _start.x + _dir_vec.x, _start.y + _dir_vec.y };
	_field->updateLazerVector( tmp );
	updateUnitVector( );

	//‰æ‘œ
	ImageProperty lazer;
	lazer.cx = tmp.x;
	lazer.cy = tmp.y;
	lazer.angle = getLazerImageAngle( );
	lazer.png = _lazer_image;

	_lazer.push_back( lazer );

	//•`‰æ
	double draw_x = _start.x + _dir_vec.x;
	double draw_y = _start.y + _dir_vec.y;
	_drawer->setLine( _start.x, _start.y, draw_x, draw_y );

	int size = ( int )_lazer.size( );
	for ( int i = 0; i < size; i++ ) {
		_drawer->setImage( _lazer[ i ] );
	}
}

bool Lazer::isFinish( ) const {
	return _fin;
}

void Lazer::clearLazerImage( ) {
	std::vector< ImageProperty >( ).swap( _lazer );
}

void Lazer::updateUnitVector( ) {
	Field::Vector unit = _field->getNextDirect( );
	if ( unit.x == _unit.x && unit.y == _unit.y ) {
		return;
	}
	_unit = unit;
	Field::Vector pos = _field->getReflectionPoint( );
	if ( pos.x != 0 && pos.y != 0 ) {
		_start = pos;
	}
	_dir_vec = Field::Vector( );
}

double Lazer::getLazerImageAngle( ) {
	double angle = 0;
	if ( _unit.x != 0 ) {
		if ( _unit.x < 0 ) {
			angle = PI;
		} else {
			angle = 0;
		}
	}

	if ( _unit.y != 0 ) {
		if ( _unit.y < 0 ) {
			angle = PI / 2;
		} else {
			angle = PI / 2 * 3;
		}	
	}

	return angle;
}