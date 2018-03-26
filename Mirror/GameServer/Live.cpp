#include "Live.h"

Live::Live( GlobalDataPtr data ) :
Field( data ) {
}

Live::~Live( ) {
}

std::string Live::getTag( ) {
	return "LIVE";
}

void Live::update( ) {
	drawField( );
	drawArmament( );
	drawMirror( );
	drawPlayer( );
	drawBackGround( );
}

void Live::drawField( ) {
	if ( getFieldPosHitNum( ) != -1 ) {
		SetCursor( _cur_hand );
	}
	//ƒtƒB[ƒ‹ƒh•`‰æ
	for ( int i = 0; i < FIELD_ROW; i++ ) {
		for ( int j = 0; j < FIELD_COL; j++ ) {
			ImageProperty image = ImageProperty( );
			image.cx = START_POS_X + j * SQUARE_SIZE + SQUARE_SIZE * 0.5;
			image.cy = START_POS_Y + i * SQUARE_SIZE + SQUARE_SIZE * 0.5;
			image.png = _table_handle;
			image.size = FIELD_SIZE_RATE;

			image.cx += LIVE_GAP_X;
			image.cy += LIVE_GAP_Y;
			_drawer->setBackImage( image );
		}
	}
}

void Live::drawArmament( ) const {
	//–C‘ä•`‰æ
	if ( _lazer_point_idx < 0 ) {
		return;
	}
	ImageProperty image = ImageProperty( );
	image.cx = _lazer_image.cx;
	image.cy = _lazer_image.cy;
	image.png = _lazer_image.png;
	image.size = FIELD_SIZE_RATE;

	image.cx += LIVE_GAP_X;
	image.cy += LIVE_GAP_Y;
	_drawer->setBackImage( image );
}

void Live::drawMirror( ) const {
	//‹¾•`‰æ
	for ( int i = 0; i < FIELD_ROW * FIELD_COL; i++ ) {
		if ( _mirrors.find( i ) == _mirrors.end( ) ) {
			continue;
		}
		Mirror mirror = _mirrors.find( i )->second;
		ImageProperty image = ImageProperty( );

		float angle = 0;
		if ( mirror.angle != RIGHT ) {
			angle = ( float )PI / 2;
		}
		image.cx = START_POS_X + mirror.x * SQUARE_SIZE + SQUARE_SIZE * 0.5;
		image.cy = START_POS_Y + mirror.y * SQUARE_SIZE + SQUARE_SIZE * 0.5;
		image.angle = angle;
		image.png = _mirror_handle[ mirror.player_num ];
		image.size = FIELD_SIZE_RATE;

		image.cx += LIVE_GAP_X;
		image.cy += LIVE_GAP_Y;
		_drawer->setBackImage( image );
	}
}


void Live::drawPlayer( ) const {
	if ( !_player_selected ) {
		return;
	}

	for ( int i = 0; i < PLAYER_NUM; i++ ) {
		if ( _dead_player == i ) {
			continue;
		}

		int pos = getPlayerPosIdx( i );
		if ( pos < 0 ) {
			continue;
		}

		double x = 0;
		double y = 0;

		x = _select_player_pos[ pos ].x;
		y = _select_player_pos[ pos ].y;

		ImageProperty image = ImageProperty( );
		image.png = _player_handle[ i ];
		image.cx = x;
		image.cy = y;
		image.size = FIELD_SIZE_RATE;

		image.cx += LIVE_GAP_X;
		image.cy += LIVE_GAP_Y;
		_drawer->setBackImage( image );
	}
}

void Live::drawBackGround( ) const {
	_drawer->setBackGroundImage( _background );
}
