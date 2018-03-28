#include "Cutin.h"
#include "const.h"
#include "GlobalData.h"
#include "Image.h"
#include "Drawer.h"
#include "Soundplayer.h"
#include "Sound.h"

const float CUTIN_SPEED = 20 * 1.8f;
const int WAIT_TIME = 90;
const int PHASE_CUTIN_IDX = 0;
const int PLAYER_CUTIN_IDX = 3;
const int ITEM_CUTIN_IDX = 5;
const int CUTIN_BACKIMAGE_SIZEMAX_CNT = 15;
const int FINISH_TIME = WAIT_TIME + CUTIN_BACKIMAGE_SIZEMAX_CNT / 2;
const short int DEFAULT_BRT = 100;

Cutin::Cutin( GlobalDataPtr data ) :
_data( data ) {
	_flag = false;
	_player_turn = false;
	_your_turn = false;
	_play_se = false;
	_click = true;
	_brt = DEFAULT_BRT;
	_drawer = _data->getDrawerPtr( );
	_soundplayer = _data->getSoundplayerPtr( );

	_cutin_back = Base::ImageProperty( );
	_cutin_string = Base::ImageProperty( );

	ImagePtr image = _data->getImagePtr( );
	Png png = image->getPng( CUTIN_BACK_IMAGE, 0 );

	_cutin_half_width = png.width / 2;
	_cutin_half_height = png.height / 2;

	{//カットイン背景
		_cutin_back.cx = WIDTH / 2;
		_cutin_back.cy = HEIGHT / 2;
	}

	{//カットイン文字
		_cutin_string.cx = png.width / 2 * -1;
		_cutin_string.cy = HEIGHT / 2;
		_cutin_string.lx = png.width / 2;
		_cutin_string.ly = png.height / 2;
	}

	std::map< CUTIN_TYPE, std::vector< int > >( ).swap( _cutin_handles );
	std::array< int, CUTIN_TYPE_MAX >( ).swap( _back_handles );

	//フェーズカットイン
	for ( int i = 0; i < CUTIN_PHASE_MAX; i++ ) {
		_cutin_handles[ CUTIN_TYPE_PHASE ].push_back( image->getPng( CUTIN_STRING_IMAGE, PHASE_CUTIN_IDX + i ).png );
	}

	//プレイヤーカットイン
	for ( int i = 0; i < CUTIN_PLAYER_MAX; i++ ) {
		_cutin_handles[ CUTIN_TYPE_PLAYER ].push_back( image->getPng( CUTIN_STRING_IMAGE, PLAYER_CUTIN_IDX + i ).png );
	}

	//アイテムカットイン
	for ( int i = 0; i < CUTIN_ITEM_MAX; i++ ) {
		_cutin_handles[ CUTIN_TYPE_ITEM ].push_back( image->getPng( CUTIN_STRING_IMAGE, ITEM_CUTIN_IDX + i ).png );
	}

	//バックイメージ
	for ( int i = 0; i < CUTIN_TYPE_MAX; i++ ) {
		_back_handles[ i ] = image->getPng( CUTIN_BACK_IMAGE, i ).png;
	}

	//サウンド
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
	_flag = false;
	_player_turn = false;
	_your_turn = false;
	_click = false;
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
	_brt = DEFAULT_BRT;
	calcBack( );
	calcString( );
}

void Cutin::draw( ) {
	drawBack( );
	drawString( );
}

void Cutin::calcBack( ) {
	if ( _cutin_string.cnt < WAIT_TIME ) {
		if ( _cutin_back.cnt < CUTIN_BACKIMAGE_SIZEMAX_CNT ) {
			_cutin_back.cnt++;
		}
		return;

	} else {

		//プレイヤーターンで
		if ( _player_turn ) {
			//クリックされていれば
			if ( _click ) {
				_click = false;
				_player_turn = false;
			} else {
				return;
			}
		}

		//この関数を呼ぶのが先なので１フレームずらす
		if ( _cutin_string.cnt > WAIT_TIME - 1 ) {
			if ( _cutin_back.cnt > 0 ) {
				_cutin_back.cnt--;
			}
		}
	}
}

void Cutin::calcString( ) {
	if ( _cutin_string.cx < WIDTH / 2 ) {
		_cutin_string.cx += CUTIN_SPEED;

		//超過したら戻す
		if ( _cutin_string.cx > WIDTH / 2 ) {
			_cutin_string.cx = WIDTH / 2;
		}
	} else {
		//プレイヤーカットインだったら
		if ( _player_turn && _your_turn ) {
			drawMessage( );
			//クリックするまで消えない
			if ( _data->getClickLeft( ) ) {
				_cutin_string.cnt = WAIT_TIME;
				_click = true;
			}
			return;
		}

		_cutin_string.cnt++;
		if ( _cutin_string.cnt < WAIT_TIME ) {
			_cutin_string.cx++; 
			return;
		}

		_cutin_string.cx += CUTIN_SPEED;

		if ( _cutin_string.cnt > FINISH_TIME ) {
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
		if ( _your_turn ) {
			_soundplayer->play( _se[ _type ] );
		}
		break;

	case CUTIN_TYPE_ITEM:
		_soundplayer->play( _se[ _type ] );
		break;
	}

	_play_se = true;
}

void Cutin::drawBack( ) {
	_drawer->setAllBright( _brt, _brt, _brt );
	_drawer->setFrontExtendImage( _cutin_back, ( float )_cutin_half_width, ( float )_cutin_half_height, 1, ( float )_cutin_back.cnt / ( float )CUTIN_BACKIMAGE_SIZEMAX_CNT );
}

void Cutin::drawString( ) const {
	_drawer->setFrontImage( _cutin_string );
}

void Cutin::drawMessage( ) const {
	_drawer->setFrontString( true, WIDTH / 2, HEIGHT * 0.725, WHITE, "クリックしてください", Drawer::BIG );
}

void Cutin::reset( ) {
	ImagePtr image = _data->getImagePtr( );
	Png png = image->getPng( CUTIN_BACK_IMAGE, 0 );

	{//カットイン背景
		_cutin_back.cx = WIDTH / 2;
		_cutin_back.cy = HEIGHT / 2;
		_cutin_back.cnt = 0;
	}

	{//カットイン文字
		_cutin_string.cx = png.width / 2 * -1;
		_cutin_string.cy = HEIGHT / 2;
		_cutin_string.lx = png.width / 2;
		_cutin_string.ly = png.height / 2;
		_cutin_string.cnt = 0;
	}
}

void Cutin::setImage( CUTIN_TYPE type, int idx, bool player_turn, bool your_turn ) {
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
	_cutin_back.png = _back_handles[ ( int )type ];
	_cutin_string.png = _cutin_handles[ type ][ idx ];
	_type = type;
	_flag = true;
	_player_turn = player_turn;
	_play_se = false;
	_your_turn = your_turn;
}

Cutin::CUTIN_TYPE Cutin::getCutinType( ) const {
	return _type;
}

bool Cutin::isCutin( ) const {
	return _flag;
}