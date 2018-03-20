#include "Cutin.h"
#include "const.h"
#include "GlobalData.h"
#include "Image.h"
#include "Drawer.h"
#include "Soundplayer.h"
#include "Sound.h"

const int CUTIN_SPEED = 20;
const int WAIT_TIME = 90;
const int PHASE_CUTIN_IDX = 0;
const int PLAYER_CUTIN_IDX = 3;
const int ITEM_CUTIN_IDX = 5;

Cutin::Cutin( GlobalDataPtr data ) :
_data( data ) {
	_speed = 1.0f;
	_flag = false;
	_player_turn = false;
	_play_se = false;
	_drawer = _data->getDrawerPtr( );
	_soundplayer = _data->getSoundplayerPtr( );

	_cutin = Base::ImageProperty( );
	ImagePtr image = _data->getImagePtr( );
	Png png = image->getPng( CUTIN_BACK_IMAGE, 0 );
	_cutin.cx = png.width / 2 * -1;
	_cutin.cy = HEIGHT / 2;
	_cutin.lx = png.width / 2;
	_cutin.ly = png.height / 2;
	_cutin.cnt = 0;

	std::map< CUTIN_TYPE, std::vector< int > >( ).swap( _cutin_handles );
	std::array< int, CUTIN_TYPE_MAX >( ).swap( _back_handles );

	//�t�F�[�Y�J�b�g�C��
	for ( int i = 0; i < CUTIN_PHASE_MAX; i++ ) {
		_cutin_handles[ CUTIN_TYPE_PHASE ].push_back( image->getPng( CUTIN_STRING_IMAGE, PHASE_CUTIN_IDX + i ).png );
	}

	//�v���C���[�J�b�g�C��
	for ( int i = 0; i < CUTIN_PLAYER_MAX; i++ ) {
		_cutin_handles[ CUTIN_TYPE_PLAYER ].push_back( image->getPng( CUTIN_STRING_IMAGE, PLAYER_CUTIN_IDX + i ).png );
	}

	//�A�C�e���J�b�g�C��
	for ( int i = 0; i < CUTIN_ITEM_MAX; i++ ) {
		_cutin_handles[ CUTIN_TYPE_ITEM ].push_back( image->getPng( CUTIN_STRING_IMAGE, ITEM_CUTIN_IDX + i ).png );
	}

	//�o�b�N�C���[�W
	for ( int i = 0; i < CUTIN_TYPE_MAX; i++ ) {
		_back_handles[ i ] = image->getPng( CUTIN_BACK_IMAGE, i ).png;
	}

	//�T�E���h
	SoundPtr sound = _data->getSoundPtr( );
	std::array< Base::SoundProperty, CUTIN_TYPE_MAX >( ).swap( _se );

	for ( int i = CUTIN_TYPE_PLAYER; i < CUTIN_TYPE_MAX; i++ ) {
		_se[ i ].loop = false;
		_se[ i ].top = true;
		_se[ i ].wav = sound->getWav( EFFECT_SOUND, YOUR_TURN_CUTIN_SE + ( i - CUTIN_TYPE_PLAYER ) ).wav;
	}
}

Cutin::~Cutin( ) {
}

void Cutin::initialize( ) { 
	_speed = 1.0f;
	_flag = false;
	_player_turn = false;
}

void Cutin::update( ) {
	if ( !_flag ) {
		return;
	}

	playSound( );
	calc( );
	draw( );
}

void Cutin::calc( ) {
	if ( _cutin.cx < WIDTH / 2 ) {
		_cutin.cx += CUTIN_SPEED * _speed;
	} else {
		//�v���C���[�J�b�g�C����������
		if ( _player_turn ) {
			drawString( );
			//�N���b�N����܂ŏ����Ȃ�
			if ( _data->getClickLeft( ) ) {
				_player_turn = false;
				_cutin.cnt = WAIT_TIME;
			}
			return;
		}

		if ( _cutin.cnt < WAIT_TIME ) {
			_cutin.cx = WIDTH / 2;
			_cutin.cnt++;
			return;
		}


		_cutin.cx += CUTIN_SPEED * _speed;

		if ( _cutin.cx > WIDTH + _cutin.lx ) {
			_flag = false;
			reset( );
		}
	}
}

void Cutin::playSound( ) {
	if ( _play_se ) {
		return;
	}

	switch ( _type ) {
	case CUTIN_TYPE_PHASE:
		break;

	case CUTIN_TYPE_PLAYER:
		if ( _cutin_handles[ CUTIN_TYPE_PLAYER ][ CUTIN_PLAYER_YOUR_TURN ] == _cutin.png ) {
			_soundplayer->play( _se[ _type ] );
		}
		break;

	case CUTIN_TYPE_ITEM:
		_soundplayer->play( _se[ _type ] );
		break;
	}

	_play_se = true;
}

void Cutin::draw( ) const {
	_drawer->setAllBright( 100, 100, 100 );

	Base::ImageProperty image = Base::ImageProperty( );
	image.cx = _cutin.cx;
	image.cy = _cutin.cy;
	image.png = _back_cutin;
	image.bright_flag = true;
	_drawer->setFrontImage( image );
	_drawer->setFrontImage( _cutin );
}

void Cutin::drawString( ) const {
	_drawer->setFrontString( true, WIDTH / 2, HEIGHT * 0.725, WHITE, "�N���b�N���Ă�������", Drawer::BIG );
}

void Cutin::reset( ) {
	_speed = 1.0f;
	ImagePtr image = _data->getImagePtr( );
	Png png = image->getPng( CUTIN_BACK_IMAGE, 0 );
	_cutin.cx = png.width / 2 * -1;
	_cutin.cy = HEIGHT / 2;
	_cutin.lx = png.width / 2;
	_cutin.ly = png.height / 2;
	_cutin.cnt = 0;
}

void Cutin::setImage( CUTIN_TYPE type, int idx, bool player_turn ) {
	if ( _flag ) {
		return;
	}
	if ( type >= CUTIN_TYPE_MAX ) {
		return;
	}
	if ( idx >= _cutin_handles[ type ].size( ) ) {
		return;
	}
	reset( );
	_back_cutin = _back_handles[ ( int )type ];
	_cutin.png = _cutin_handles[ type ][ idx ];
	_type = type;
	_flag = true;
	_player_turn = player_turn;
	_play_se = false;
}

void Cutin::setSpeed( float speed ) {
	_speed = speed;
}

Cutin::CUTIN_TYPE Cutin::getCutinType( ) const {
	return _type;
}

bool Cutin::isCutin( ) const {
	return _flag;
}