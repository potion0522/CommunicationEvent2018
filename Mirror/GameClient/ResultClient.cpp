#include "ResultClient.h"
#include "GlobalData.h"
#include "Game.h"
#include "Drawer.h"
#include "Image.h"

const float RETURNBUTTON_X = WIDTH  / 2;
const float RETURNBUTTON_Y = HEIGHT / 5 * 3.5 ;

ResultClient::ResultClient( GlobalDataPtr data, GamePtr game ) :
_data( data ),
_game( game ) {
	setFlag( 1 );
	_drawer = _data->getDrawerPtr( );
	_cur_hand = LoadCursor( NULL, IDC_HAND );

	_returnbutton = BoxObject( );
	ImagePtr image_ptr = _data->getImagePtr( );

	//リターンボタン
		float half_width  = image_ptr->getPng( BUTTON_IMAGE, SAVE_BUTTON_IDX ).width * 0.5f;
		float half_height = image_ptr->getPng( BUTTON_IMAGE, SAVE_BUTTON_IDX ).height * 0.5f;
		for ( int i = 0; i < BUTTON_TYPE_MAX; i++ ) {
			_returnbutton_handles[ i ] = image_ptr->getPng( BUTTON_IMAGE, RETURN_BUTTON_IDX + i ).png;
		}
		_returnbutton.image.cx = RETURNBUTTON_X;
		_returnbutton.image.cy = RETURNBUTTON_Y;
		_returnbutton.image.png = image_ptr->getPng( BUTTON_IMAGE, RETURN_BUTTON_IDX ).png;

		_returnbutton.collider.lx = ( float )_returnbutton.image.cx - half_width;
		_returnbutton.collider.rx = ( float )_returnbutton.image.cx + half_width;
		_returnbutton.collider.ly = ( float )_returnbutton.image.cy - half_height;
		_returnbutton.collider.ry = ( float )_returnbutton.image.cy + half_height;
}

ResultClient::~ResultClient( ) {
}

std::string ResultClient::getTag( ) {
	return "RESULTCLIENT";
}

void ResultClient::initialize( ) {
	_drawn = _game->isDrawn( );
	_win = _game->isWin( );
	_cause = _game->getCauseOfDeath( );
	_cnt = FRAME * 10;
}

void ResultClient::finalize( ) {
}

void ResultClient::update( ) {

	std::string result;
	if ( _drawn ) {
		//引き分け
		result = "引き分け";
	} else {
		if ( _win ) {
			result = "あなたの勝ち";
		} else {
			result = "あなたの負け";
		}
	}

	std::string result_message = convResultMessage( _win, _cause );
	_drawer->setFrontString( true, WIDTH / 2, HEIGHT / 3, RED, result, Drawer::SUPER_BIG );
	_drawer->setFrontString( true, WIDTH / 2, HEIGHT / 2, WATER, result_message, Drawer::LITTLE_BIG );

	_cnt--;

	if ( _data->getClickLeft( ) ) {
		_cnt -= FRAME;
	}
	if ( _cnt <= 0 ) {
		_data->setInitFlag( );
	}

	if ( _returnbutton_clicking && !isDrag( ) ) {
		switch ( getHitButton( ) ) {
			case true:
				_data->setInitFlag( );
				break;
			default:
				_returnbutton_clicking = false;
				break;
		}
	}

	calcButtonAction( );
	drawReturnButton( );
}

void ResultClient::calcButtonAction( ) {
	_returnbutton.image.png = _returnbutton_handles[ NORMAL ];

	if ( !getHitButton( ) ) {
		return;
	}

	if ( !isDrag( ) ) {
		return;
	}

	if ( getHitButton( ) ) {
		_returnbutton.image.png = _returnbutton_handles[ CLICKING ];
		_returnbutton_clicking = true;
	}
}

bool ResultClient::isDrag( ) const {
	if ( _data->getClickingLeft( ) > 0 ) {
		return true;
	}
	return false;
}

bool ResultClient::getHitButton( ) const {
	double mouse_x = _data->getMouseX( );
	double mouse_y = _data->getMouseY( );

	if ( _returnbutton.collider.lx <= mouse_x && mouse_x <= _returnbutton.collider.rx &&
		_returnbutton.collider.ly <= mouse_y && mouse_y <= _returnbutton.collider.ry ) {
		SetCursor( _cur_hand );
		return true;
	}

	return false;
}

void ResultClient::drawReturnButton( ) const {
	_drawer->setFrontImage( _returnbutton.image );
}

std::string ResultClient::convResultMessage( bool win, CAUSE_OF_DEATH cause ) {
	std::string str;

	if ( _drawn ) {
		return "お互いの時間が０になってしまいました.....";
	}

	if ( win ) {
		str = convWinMessage( cause );
	} else {
		str = convLoseMessage( cause );
	}
	return str;
}

std::string ResultClient::convWinMessage( CAUSE_OF_DEATH cause ) {
	std::string str;
	switch ( cause ) {
	case CAUSE_HIT:
		str = "お見事！相手を倒すことに成功しました！";
		break;

	case CAUSE_TIME:
		str = "相手は時間切れみたいです。あなたの戦略に万歳！";
		break;

	default:
		break;
	}
	return str;
}

std::string ResultClient::convLoseMessage( CAUSE_OF_DEATH cause ) {
	std::string str;
	switch ( cause ) {
	case CAUSE_HIT:
		str = "こちらの的が壊されてしまいました......";
		break;

	case CAUSE_TIME:
		str = "時間切れのようです。戦略が甘かったみたいです....";
		break;

	default:
		break;
	}
	return str;
}