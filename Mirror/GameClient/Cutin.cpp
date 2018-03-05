#include "Cutin.h"
#include "const.h"
#include "GlobalData.h"
#include "Image.h"
#include "Drawer.h"

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
	_drawer = _data->getDrawerPtr( );

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
}

Cutin::~Cutin( ) {
}

void Cutin::update( ) {
	if ( !_flag ) {
		return;
	}

	calc( );
	draw( );
	drawString( );
}

void Cutin::calc( ) {
	if ( _cutin.cx < WIDTH / 2 ) {
		_cutin.cx += CUTIN_SPEED * _speed;
	} else {
		if ( _cutin.cnt < WAIT_TIME ) {
			_cutin.cx = WIDTH / 2;
			_cutin.cnt++;
			return;
		}
		//プレイヤーカットインであれば
		if ( _player_turn ) {
			//クリックするまで消えない
			if ( _data->getClickLeft( ) ) {
				_player_turn = false;
			}
			return;
		}

		_cutin.cx += CUTIN_SPEED * _speed;

		if ( _cutin.cx > WIDTH + _cutin.lx ) {
			_flag = false;
			reset( );
		}
	}
}

void Cutin::draw( ) const {
	_drawer->setAllBright( 100, 100, 100 );

	Base::ImageProperty image = Base::ImageProperty( );
	image.cx = _cutin.cx;
	image.cy = _cutin.cy;
	image.png = _back_cutin;
	image.bright_flag = true;
	_drawer->setBackImage( image );
	_drawer->setBackImage( _cutin );
}

void Cutin::drawString( ) const {
	if ( !_player_turn ) {
		return;
	}
	_drawer->setFrontString( true, WIDTH / 2, HEIGHT * 0.725, WHITE, "クリックしてください", Drawer::BIG );
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