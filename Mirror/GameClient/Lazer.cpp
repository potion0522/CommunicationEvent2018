#include "Lazer.h"
#include "GlobalData.h"
#include "Drawer.h"
#include "Debug.h"
#include "Image.h"
#include <math.h>

const double LAZER_SPEED = 20;
const int WAIT_TIME = 120;

Lazer::Lazer( GlobalDataPtr data ) :
_data( data ) {
	setFlag( 1 );
	_drawer = _data->getDrawerPtr( );
	_field = _data->getFieldPtr( );

	ImagePtr image_ptr = _data->getImagePtr( );
	_lazer_image = image_ptr->getPng( LAZER_IMAGE, 0 ).png;
	_lazer_size.width = ( float )image_ptr->getPng( LAZER_IMAGE, 0 ).width;
	_lazer_size.height = ( float )image_ptr->getPng( LAZER_IMAGE, 0 ).height;

	_lazer_reflect_image = image_ptr->getPng( LAZER_IMAGE, 1 ).png;

	for ( int i = 0; i < DEAD_EFFECT_MAX; i++ ) {
		_dead_effect_images[ i ] = image_ptr->getPng( EFFECT_IMAGE, i ).png;
	}
}

Lazer::~Lazer( ) {
}

std::string Lazer::getTag( ) {
	return "LAZER";
}

void Lazer::initialize( ) {
	_lazer_update = false;
	_fin = false;
	_dead_flag = false;
	_distance = 1;
	_wait = 0;
	_start = Field::Vector( );
	_dir_vec = Field::Vector( );
	_unit = Field::Vector( );
	_start = _field->getLazerPoint( );
	updateUnitVector( );
	std::vector< ImageProperty >( ).swap( _lazer );
	std::list< Coordinate >( ).swap( _reflec_pnt );
	_dead_pnt = Coordinate( );

	const short int ADJUSTMENT = ( short int )( SQUARE_SIZE * 0.4 );
	int idx = _field->getLazerPointIdx( );
	if ( idx / PLAYER_POSITION ) {
		_start.y += ADJUSTMENT;
	} else {
		_start.x += ADJUSTMENT;
	}
}

void Lazer::update( ) {
	if ( _field->getPhase( ) < ATTACK_PHASE ) {
		return;
	}
	if ( _fin ) {
		return;
	}

	if ( _dead_flag ) {
		if ( _wait < WAIT_TIME ) {
			_wait++;
		} else {
			_fin = true;
		}
	}

	//プレイヤー死亡
	if ( _field->getDeadPlayer( ) != -1 ) {
		_dead_flag = true;
		_dead_pnt.x = ( short int )( _start.x + _dir_vec.x );
		_dead_pnt.y = ( short int )( _start.y + _dir_vec.y );
	}

	//画面外
	if ( _dir_vec.x + _start.x > START_POS_X + SQUARE_SIZE * FIELD_COL + SQUARE_SIZE ||
		 _dir_vec.x + _start.x < START_POS_X - SQUARE_SIZE ||
		 _dir_vec.y + _start.y > START_POS_Y + SQUARE_SIZE * FIELD_ROW + SQUARE_SIZE ||
		 _dir_vec.y + _start.y < START_POS_Y - SQUARE_SIZE ) {
		_fin = true;
		return;
	}

	if ( !_dead_flag ) {
		double x = _unit.x * LAZER_SPEED;
		double y = _unit.y * LAZER_SPEED;
		_dir_vec.x += x;
		_dir_vec.y -= y;

		_lazer_update = false;
		Field::Vector tmp = { _start.x + _dir_vec.x, _start.y + _dir_vec.y };
		_field->updateLazerVector( tmp, LAZER_SPEED );
		updateUnitVector( );

		//画像
		ImageProperty lazer;
		lazer.cx = tmp.x;
		lazer.cy = tmp.y;
		lazer.lx = lazer.cx - _lazer_size.width / 2;
		lazer.ly = lazer.cy - _lazer_size.height / 2;
		lazer.rx = lazer.cx + _lazer_size.width / 2;
		lazer.ry = lazer.cy + _lazer_size.height / 2;
		lazer.angle = getLazerImageAngle( );
		lazer.png = _lazer_image;

		if ( !_lazer_update ) {
			_lazer.push_back( lazer );
		}
	}


	//描画
	drawLazerLine( );
	drawRefrecEffect( );

	if ( _dead_flag ) {
		if ( _dead_pnt.cnt < DEAD_EFFECT_MAX - 1 ) {
			drawDeadEffect( );
		} else {
			std::vector< ImageProperty >( ).swap( _lazer );
			std::list< Coordinate >( ).swap( _reflec_pnt );
		}
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
	Field::Vector tmp = Field::Vector( );
	tmp = _unit;

	_unit = unit;
	Field::Vector pos = _field->getReflectionPoint( );
	if ( pos.x != 0 && pos.y != 0 ) {
		_start = pos;
	}
	_dir_vec = Field::Vector( );
	_lazer_update = true;

	//エフェクトをセット
	Coordinate coordinate = Coordinate( );
	coordinate.angle = ( float )getReflectEffectAngle( tmp, _unit );
	coordinate.x = ( short int )_start.x;
	coordinate.y = ( short int )_start.y;
	_reflec_pnt.push_back( coordinate );
}

double Lazer::getLazerImageAngle( ) {
	double angle = 0;
	if ( _unit.x != 0 ) {
		if ( _unit.x < 0 ) {
			angle = PI / 2;
		} else {
			angle = PI / 2 * 3;
		}
	}

	if ( _unit.y != 0 ) {
		if ( _unit.y < 0 ) {
			angle = PI;
		} else {
			angle = 0;
		}	
	}

	return angle;
}

Field::Vector Lazer::convNormalVector( Field::Vector vec ) {
	Field::Vector normal = vec;
	double length = sqrt( vec.x * vec.x + vec.y * vec.y );

	normal.x /= length;
	normal.y /= length;

	return normal;
}

Field::DIR Lazer::convVectorToDir( Field::Vector vec ) {
	//左手座標で計算
	Field::DIR direct = Field::DIR( );

	if ( vec.x != 0 ) {
		if ( vec.x < 0 ) {
			direct = Field::DIR_LEFT;
		} else {
			direct = Field::DIR_RIGHT;
		}
	}

	if ( vec.y != 0 ) {
		if ( vec.y < 0 ) {
			direct = Field::DIR_UP;
		} else {
			direct = Field::DIR_DOWN;
		}
	}
	return direct;
}

double Lazer::getReflectEffectAngle( Field::Vector old_vec, Field::Vector new_vec ) {
	//NEを基準に
	Field::Vector before = convNormalVector( old_vec );
	Field::Vector after = convNormalVector( new_vec );
	Field::DIR old_dir = convVectorToDir( before );
	Field::DIR new_dir = convVectorToDir( after );

	double angle = 0;
	//左手座標で上下逆のため注意
	switch ( old_dir ) {
	case Field::DIR_UP:
		( new_dir == Field::DIR_RIGHT ) ? angle = 0 : angle = -PI * 0.5;
		break;
	case Field::DIR_DOWN:
		( new_dir == Field::DIR_RIGHT ) ? angle = PI * 0.5 : angle = PI;
		break;
	case Field::DIR_RIGHT:
		( new_dir == Field::DIR_UP ) ? angle = PI : angle = -PI * 0.5;
		break;
	case Field::DIR_LEFT:
		( new_dir == Field::DIR_UP ) ? angle = PI * 0.5 : angle = 0;
		break;
	}

	return angle;
}

void Lazer::drawLazerLine( ) const {
	int size = ( int )_lazer.size( );
	if ( size < 1 ) {
		return;
	}

	for ( int i = 0; i < size; i++ ) {
		_drawer->setExtendFrontImage( _lazer[ i ], _lazer_size.width / 2, _lazer_size.height / 2, 1, LAZER_SPEED / _lazer_size.height );
	}
}

void Lazer::drawRefrecEffect( ) {
	int size = ( int )_reflec_pnt.size( );
	if ( size < 1 ) {
		return;
	}

	const int ANIMATION_TIME = 5;

	std::list< Coordinate >::iterator ite;
	ite = _reflec_pnt.begin( );
	for ( ite; ite != _reflec_pnt.end( ); ite++ ) {
		ImageProperty image = ImageProperty( );
		image.cx = ite->x;
		image.cy = ite->y;
		image.angle = ite->angle;
		image.png = _lazer_reflect_image;
		if ( ite->cnt < ANIMATION_TIME ) {
			ite->cnt++;
		}
		image.size = ( double )( ite->cnt * ( 1 / ( float )ANIMATION_TIME ) );

		_drawer->setFrontImage( image );
	}
}

void Lazer::drawDeadEffect( ) {
	ImageProperty image = ImageProperty( );
	image.cx = _dead_pnt.x;
	image.cy = _dead_pnt.y;
	image.png = _dead_effect_images[ _dead_pnt.cnt ];
	image.size = 1.5;
	_drawer->setFrontImage( image );

	_dead_pnt.cnt = _wait / ( DEAD_EFFECT_MAX / 2 );
}