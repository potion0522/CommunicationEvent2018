#include "Lazer.h"
#include "GlobalData.h"
#include "Drawer.h"
#include "Debug.h"
#include "Image.h"
#include <cmath>

const double LAZER_SPEED = 30;
const short int WAIT_TIME = 120;
const short int LAZER_R = ( short int )( 8 * FIELD_SIZE_RATE );
const short int MIRROR_R = 5;
const short int PLAYER_R = SQUARE_SIZE / 5 * 2;

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

	_start = Vector( );
	_dir_vec = Vector( );
	_hit_point = Vector( );
	_start = _field->getLazerPoint( );

	{//レーザーポイント取得
		int idx = _field->getLazerPointIdx( );
		if ( idx / PLAYER_POSITION == 0 ) {
			_direct = DIR_RIGHT;
		} else {
			_direct = DIR_DOWN;
		}
	}

	_unit = getDirectVector( );

	std::vector< ImageProperty >( ).swap( _lazer );
	std::list< Coordinate >( ).swap( _reflec_point );
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
		_lazer_update = false;
		updateLazerVector( );
		checkPlayerHit( );
		updateUnitVector( );

		if ( !_lazer_update ) {
			double x = _unit.x * LAZER_SPEED;
			double y = _unit.y * LAZER_SPEED;
			_dir_vec.x += x;
			_dir_vec.y -= y;
		}

		Vector tmp = { _start.x + _dir_vec.x, _start.y + _dir_vec.y };
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
			std::list< Coordinate >( ).swap( _reflec_point );
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
	Vector unit = getDirectVector( );

	//更新を検知
	if ( unit.x == _unit.x && unit.y == _unit.y ) {
		return;
	}

	Vector past = _unit;

	_unit = unit;
	if ( _hit_point.x != 0 && _hit_point.y != 0 ) {
		_start = _hit_point;
		_start.x += _unit.x * ( LAZER_R + MIRROR_R + 1 );
		_start.y -= _unit.y * ( LAZER_R + MIRROR_R + 1 );
	}
	_dir_vec = Vector( );

	//エフェクトをセット
	Coordinate coordinate = Coordinate( );
	coordinate.angle = ( float )getReflectEffectAngle( past, _unit );
	coordinate.x = ( short int )_hit_point.x;
	coordinate.y = ( short int )_hit_point.y;
	_reflec_point.push_back( coordinate );

	_hit_point = Vector( );
}

void Lazer::checkPlayerHit( ) {
	if ( _lazer_update ) {
		return;
	}

	Vector vec = { _start.x + _dir_vec.x, _start.y + _dir_vec.y };
	Vector normal = getDirectVector( );

	bool hit = false;
	if ( vec.x < START_POS_X || vec.y < START_POS_Y ) {
		for ( int i = 0; i < PLAYER_NUM; i++ ) {
			Field::PlayerPos pos = _field->getPlayerPos( i );
			if ( LAZER_SPEED > PLAYER_R + LAZER_R ) {
				for ( int j = 0; j < LAZER_SPEED / ( PLAYER_R + LAZER_R ); j++ ) {
					double a = std::sqrt( std::pow( vec.x - pos.x, 2 ) );
					double b = std::sqrt( std::pow( vec.y - pos.y, 2 ) );
					if ( a + b <= PLAYER_R + LAZER_R ) {
						_field->setDeadPlayer( i );
						hit = true;
						break;
					}
				}
			}

			if ( hit ) {
				break;
			}

			double a = std::sqrt( std::pow( vec.x - pos.x, 2 ) );
			double b = std::sqrt( std::pow( vec.y - pos.y, 2 ) );
			if ( a + b <= PLAYER_R + LAZER_R ) {
				_field->setDeadPlayer( i );
				break;
			}
		}
	}
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

Lazer::DIRECT Lazer::convVectorToDir( Vector vec ) {
	//左手座標で計算
	DIRECT direct = DIRECT( );

	if ( vec.x != 0 ) {
		if ( vec.x < 0 ) {
			direct = DIR_LEFT;
		} else {
			direct = DIR_RIGHT;
		}
	}

	if ( vec.y != 0 ) {
		if ( vec.y < 0 ) {
			direct = DIR_UP;
		} else {
			direct = DIR_DOWN;
		}
	}
	return direct;
}

double Lazer::getReflectEffectAngle( Vector old_vec, Vector new_vec ) {
	//NEを基準に
	Vector before = old_vec;
	Vector after = new_vec;
	DIRECT old_dir = convVectorToDir( before );
	DIRECT new_dir = convVectorToDir( after );

	double angle = 0;
	//左手座標で上下逆のため注意
	switch ( old_dir ) {
	case DIR_UP:
		( new_dir == DIR_RIGHT ) ? angle = 0 : angle = -PI * 0.5;
		break;
	case DIR_DOWN:
		( new_dir == DIR_RIGHT ) ? angle = PI * 0.5 : angle = PI;
		break;
	case DIR_RIGHT:
		( new_dir == DIR_UP ) ? angle = PI : angle = -PI * 0.5;
		break;
	case DIR_LEFT:
		( new_dir == DIR_UP ) ? angle = PI * 0.5 : angle = 0;
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
		_drawer->setFrontExtendImage( _lazer[ i ], _lazer_size.width / 2, _lazer_size.height / 2, FIELD_SIZE_RATE, ( LAZER_SPEED / _lazer_size.height ) );
	}
}

void Lazer::drawRefrecEffect( ) {
	int size = ( int )_reflec_point.size( );
	if ( size < 1 ) {
		return;
	}

	const int ANIMATION_TIME = 5;

	std::list< Coordinate >::iterator ite;
	ite = _reflec_point.begin( );
	for ( ite; ite != _reflec_point.end( ); ite++ ) {
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

void Lazer::updateLazerVector( ) {
	Vector vec = { _start.x + _dir_vec.x, _start.y + _dir_vec.y };

	//ミラーの位置取得
	std::map< int, Field::Mirror > mirrors = _field->getAllMirror( );
	std::map< int, Field::Mirror >::iterator ite;
	ite = mirrors.begin( );

	Vector normal = getDirectVector( );
	const short int R = LAZER_R + MIRROR_R;

	bool hit = false;
	for ( ite; ite != mirrors.end( ); ite++ ) {
		Vector tmp = vec;
		for ( int i = 0; i < LAZER_SPEED / R; i++ ) {
			//三平方の定理
			Vector mirror = Vector( );
			mirror.x = ( START_POS_X + ite->second.x * SQUARE_SIZE + SQUARE_SIZE / 2 );
			mirror.y = ( START_POS_Y + ite->second.y * SQUARE_SIZE + SQUARE_SIZE / 2 );
			double a = std::sqrt( std::pow( tmp.x - mirror.x, 2 ) );
			double b = std::sqrt( std::pow( tmp.y - mirror.y, 2 ) );
			if ( a + b <= R ) {
				//当たった
				setDirect( ite->second.angle );
				hit = true;
				//変更フラグを立てる
				_lazer_update = true;
				//反射ポイントを記録
				_hit_point = mirror;
				break;
			}

			tmp.x += normal.x * R;
			tmp.y -= normal.y * R;
		}
		
		//当たった判定が出たら
		if ( hit ) {
			//反射するポイントまで画像を表示
			//for ( int i = 0; i < ( LAZER_SPEED / _lazer_size.height ) - 1; i++ ) {
			//	ImageProperty lazer = ImageProperty( );
			//	lazer.cx = tmp.x + _unit.x * i * _lazer_size.height;
			//	lazer.cy = tmp.y + _unit.y * i * _lazer_size.height;
			//	lazer.lx = lazer.cx - _lazer_size.width / 2;
			//	lazer.ly = lazer.cy - _lazer_size.height / 2;
			//	lazer.rx = lazer.cx + _lazer_size.width / 2;
			//	lazer.ry = lazer.cy + _lazer_size.height / 2;
			//	lazer.angle = getLazerImageAngle( );
			//	lazer.png = _lazer_image;

			//	_lazer.push_back( lazer );
			//}

			break;
		}
	}
}

void Lazer::setDirect( MIRROR_ANGLE angle ) {
	switch ( _direct ) {
	case DIR_UP :
		angle == RIGHT ? _direct = DIR_RIGHT : _direct = DIR_LEFT;
		break;
	case DIR_DOWN :
		angle == RIGHT ? _direct = DIR_LEFT : _direct = DIR_RIGHT;
		break;
	case DIR_RIGHT :
		angle == RIGHT ? _direct = DIR_UP : _direct = DIR_DOWN;
		break;
	case DIR_LEFT :
		angle == RIGHT ? _direct = DIR_DOWN : _direct = DIR_UP;
		break;
	}
}

Vector Lazer::getDirectVector( ) const {
	//左手座標系
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