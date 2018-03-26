#include "LiveLazer.h"
#include "Live.h"
#include "Soundplayer.h"
#include "GlobalData.h"
#include "Sound.h"

const short int WAIT_TIME = 120;

LiveLazer::LiveLazer( GlobalDataPtr data ) :
Lazer( data ) {
}

LiveLazer::~LiveLazer( ) {
}

void LiveLazer::update( ) {
	if ( !_play_lazer_se ) {
		SoundplayerPtr player = _data->getSoundplayerPtr( );
		Base::SoundProperty sound = Base::SoundProperty( );
		sound.loop = false;
		sound.top = true;
		sound.wav = _data->getSoundPtr( )->getWav( EFFECT_SOUND, LAZER_SE ).wav;
		player->play( sound );
		_play_lazer_se = true;
	}

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

	//ƒvƒŒƒCƒ„[Ž€–S
	if ( _field->getDeadPlayer( ) != -1 ) {
		_dead_flag = true;
		_dead_pnt.x = ( short int )( _start.x + _dir_vec.x );
		_dead_pnt.y = ( short int )( _start.y + _dir_vec.y );
	}

	//‰æ–ÊŠO
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
			double x = _unit.x * _lazer_speed;
			double y = _unit.y * _lazer_speed;
			_dir_vec.x += x;
			_dir_vec.y -= y;
		}

		Vector tmp = { _start.x + _dir_vec.x, _start.y + _dir_vec.y };
		//‰æ‘œ
		Base::ImageProperty lazer = Base::ImageProperty( );
		lazer.cx = tmp.x + LIVE_GAP_X;
		lazer.cy = tmp.y + LIVE_GAP_Y;
		lazer.lx = lazer.cx - _lazer_size.width / 2  + LIVE_GAP_X;
		lazer.ly = lazer.cy - _lazer_size.height / 2 + LIVE_GAP_Y;
		lazer.rx = lazer.cx + _lazer_size.width / 2  + LIVE_GAP_X;
		lazer.ry = lazer.cy + _lazer_size.height / 2 + LIVE_GAP_Y;
		lazer.angle = getLazerImageAngle( );
		lazer.png = _lazer_image;

		if ( !_lazer_update ) {
			_lazer.push_back( lazer );
		}
	}


	//•`‰æ
	drawLazerLine( );
	drawRefrecEffect( );

	if ( _dead_flag ) {
		if ( _dead_pnt.cnt < DEAD_EFFECT_MAX - 1 ) {
			drawDeadEffect( );
		} else {
			std::vector< Base::ImageProperty >( ).swap( _lazer );
			std::list< Coordinate >( ).swap( _reflec_point );
		}
	}
}

void LiveLazer::drawLazerLine( ) {
	int size = ( int )_lazer.size( );
	if ( size < 1 ) {
		return;
	}

	for ( int i = 0; i < size; i++ ) {
		_drawer->setFrontExtendImage( _lazer[ i ], _lazer_size.width / 2, _lazer_size.height / 2, FIELD_SIZE_RATE, ( _lazer_speed / _lazer_size.height ) );
	}
}

void LiveLazer::drawRefrecEffect( ) {
	int size = ( int )_reflec_point.size( );
	if ( size < 1 ) {
		return;
	}

	const int ANIMATION_TIME = 5;

	std::list< Lazer::Coordinate >::iterator ite;
	ite = _reflec_point.begin( );
	for ( ite; ite != _reflec_point.end( ); ite++ ) {
		Base::ImageProperty image = Base::ImageProperty( );
		image.cx = ite->x;
		image.cy = ite->y;
		image.angle = ite->angle;
		image.png = _lazer_reflect_image;
		if ( ite->cnt < ANIMATION_TIME ) {
			ite->cnt++;
		}
		image.size = ( double )( ite->cnt * ( 1 / ( float )ANIMATION_TIME ) );

		image.cx += LIVE_GAP_X;
		image.cy += LIVE_GAP_Y;

		_drawer->setFrontImage( image );
	}
}

void LiveLazer::drawDeadEffect( ) {
	Base::ImageProperty image = Base::ImageProperty( );
	image.cx = _dead_pnt.x;
	image.cy = _dead_pnt.y;
	image.png = _dead_effect_images[ _dead_pnt.cnt ];
	image.size = 1.5;

	image.cx += LIVE_GAP_X;
	image.cy += LIVE_GAP_Y;

	_drawer->setFrontImage( image );

	_dead_pnt.cnt = _wait / ( DEAD_EFFECT_MAX / 2 );
}