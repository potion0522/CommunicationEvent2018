#include "Console.h"
#include "GlobalData.h"
#include "Color.h"
#include "Drawer.h"
#include "Image.h"

Console::Console( GlobalDataPtr data ) :
_data( data ) {
	setFlag( 1 );
	_client = _data->getClientPtr( );
	_drawer = _data->getDrawerPtr( );
	_cur_hand = LoadCursor( NULL, IDC_HAND );

	ImagePtr image_ptr = _data->getImagePtr( );

	//ハンドル取得
	_normal_button_handle = image_ptr->getPng( BUTTON_IMAGE, RETURN_BUTTON_IDX ).png;
	_click_button_handle = image_ptr->getPng( BUTTON_IMAGE, RETURN_BUTTON_IDX + 1 ).png;

	_button = BoxObject( );
	LightImageProperty *image = &_button.image;
	image->cx = WIDTH / 2;
	image->cy = HEIGHT * 0.75f;
	image->png = _normal_button_handle;

	BoxCollider *box = &_button.collider;
	box->lx = ( float )image->cx - image_ptr->getPng( BUTTON_IMAGE, RETURN_BUTTON_IDX ).width / 2;
	box->ly = ( float )image->cy - image_ptr->getPng( BUTTON_IMAGE, RETURN_BUTTON_IDX ).height / 2;
	box->rx = ( float )image->cx + image_ptr->getPng( BUTTON_IMAGE, RETURN_BUTTON_IDX ).width;
	box->ry = ( float )image->cy + image_ptr->getPng( BUTTON_IMAGE, RETURN_BUTTON_IDX ).height;

	_back_image = image_ptr->getPng( BACKGROUND_IMAGE, 0 ).png;
}

Console::~Console( ) {
}

std::string Console::getTag( ) {
	return "CONSOLE";
}

void Console::initialize( ) {
	_connecting = false;
	_matching = false;
	_clicking = false;
	_cnt = 0;
	_button.image.png = _normal_button_handle;
}

void Console::finalize( ) {
	if ( _client->getFlag( ) ) {
		_client->disConnect( );
	}
}

void Console::update( ) {
	if ( _cnt > CONNECT_INTERVAL * 5 ) {
		_data->setInitFlag( );
		_data->setMessage( "サーバーが見つかりません" );
	}

	drawBackGround( );

	std::string phase = _client->getPhase( );
	if ( phase == "READY" ) {
		ready( );
	}
	if ( phase == "CONNECTING" ) {
		_cnt = 0;
		matching( );
	}
}

void Console::ready( ) {
	if ( _connecting ) {
		_data->setInitFlag( );
		if ( _matching ) {
			_data->setMessage( "相手との接続が切れました" );
		} else {
			_data->setMessage( "サーバーとの接続が切れました" );
		}
		return;
	}

	std::string finding = "- Connect Server -";
	std::string ip = _client->getSeverIP( );
	int size_rate = _drawer->getStringH( Drawer::BIG );
	_drawer->setFrontString( true, WIDTH / 2, HEIGHT / 2 - size_rate, YELLOW, finding, Drawer::BIG );
	_drawer->setFrontString( true, WIDTH / 2, HEIGHT / 2, WHITE, ip );

	//サーバー接続待機時間を計測
	_cnt++;
}

void Console::matching( ) {
	_connecting = true;

	//マッチングしていたら
	if ( _client->isMatching( ) ) {
		//バトル中ならリターン
		if ( _data->getScene( ) != CONNECT ) {
			return;
		}
	} else if ( _matching ) {
		//さっきまでマッチングしていた場合は初期化
		_data->setInitFlag( );
		return;
	}

	//マッチングしていないのにバトルに行っていたら
	//強制的にコネクト画面に戻す
	if ( _data->getScene( ) != CONNECT ) {
		_data->setScene( CONNECT );
		_client->disConnect( );
		_client->initialize( );
	}

	//マッチング待機中にタイトルに戻りたかったら
	updateButton( );

	if ( _client->isRecvingTcp( ) ) {
		if ( _client->isMatching( ) ) {
			_matching = true;
			_data->setScene( BATTLE );
		}
	}

	std::string matching = "- Matching -";
	_drawer->setFrontString( true, WIDTH / 2, HEIGHT / 2 - 60, YELLOW, matching, Drawer::SUPER_BIG );
	drawButton( );
}

void Console::updateButton( ) {
	if ( !isHitButton( ) ) {
		_button.image.png = _normal_button_handle;
		_clicking = false;
		return;
	}

	//クリックし初めを検知
	if ( _data->getClickLeft( ) && !_clicking ) {
		_clicking = true;
		_button.image.png = _click_button_handle;
		return;
	}

	if ( !_clicking ) {
		return;
	}

	//クリックが解除されたらボタンのヒットを取得
	if ( _data->getClickingLeft( ) < 1 ) {
		_data->setInitFlag( );
	}
}

void Console::drawButton( ) const {
	_drawer->setFrontImage( _button.image );
}

void Console::drawBackGround( ) const {
	LightImageProperty image = LightImageProperty( );
	image.cx = WIDTH / 2;
	image.cy = HEIGHT / 2;
	image.png = _back_image;
	_drawer->setBackImage( image );
}

bool Console::isHitButton( ) const {
	int mouse_x = _data->getMouseX( );
	int mouse_y = _data->getMouseY( );

	//決定
	if ( _button.collider.lx <= mouse_x && mouse_x <= _button.collider.rx &&
		 _button.collider.ly <= mouse_y && mouse_y <= _button.collider.ry ) {
		SetCursor( _cur_hand );
		return true;
	}
	return false;
}