#include "Title.h"
#include "GlobalData.h"
#include "Image.h"
#include "Drawer.h"
#include "Client.h"
#include "const.h"

Title::Title( GlobalDataPtr data ) :
_data( data ) {
	setFlag( 1 );
	_drawer = _data->getDrawerPtr( );

	ImagePtr image_ptr = _data->getImagePtr( );

	{//背景
		_back = LightImageProperty( );
		_back.cx = WIDTH / 2;
		_back.cy = HEIGHT / 2;
		_back.png = image_ptr->getPng( BACKGROUND_IMAGE, BACKGROUND_WHITE ).png;
	}

	{//タイトルロゴ
		_title_logo = LightImageProperty( );
		_title_logo.cx = WIDTH_F * 0.5;
		_title_logo.cy = image_ptr->getPng( TITLE_IMAGE, 0 ).height * 0.5;
		_title_logo.png = image_ptr->getPng( TITLE_IMAGE, 0 ).png;
	}

	{//ボタン
		const int BUTTON_X = WIDTH / 2;
		const int BUTTON_Y = ( int )( HEIGHT_F * 0.5f );
		std::array< BoxObject, BUTTON_TYPE_MAX >( ).swap( _button );
		std::array< int, BUTTON_TYPE_MAX * 2 >( ).swap( _button_handles );

		Png png = Png( );
		//ハンドル
		for ( int i = 0; i < BUTTON_TYPE_MAX * 2; i++ ) {
			_button_handles[ i ] = image_ptr->getPng( BUTTON_IMAGE, i ).png;
		}

		//座標、あたり判定
		for ( int i = 0; i < BUTTON_TYPE_MAX; i++ ) {
			png = image_ptr->getPng( BUTTON_IMAGE, i * 2 );
			LightImageProperty *image = &_button[ i ].image;
			image->cx = BUTTON_X;
			image->cy = BUTTON_Y + i * ( png.height * 0.5 + png.height );
			image->png = png.png;

			BoxCollider *box = &_button[ i ].collider;
			box->lx = ( float )( image->cx - png.width * 0.5 );
			box->ly = ( float )( image->cy - png.height * 0.5 );
			box->rx = ( float )( image->cx + png.width * 0.5 );
			box->ry = ( float )( image->cy + png.height * 0.5 );
		}
	}
}

Title::~Title( ) {
}

std::string Title::getTag( ) {
	return "TITLE";
}

void Title::initialize( ) {
	_hit_button_idx = -1;
	_past_hit_button_idx = -1;
}

void Title::update( ) {
	if ( _data->getKeyState( KEY_INPUT_Z ) == 1 && !_data->getCommandFlag( ) ) {
		_data->setScene( CONNECT );
	}
	ClientPtr client = _data->getClientPtr( );
	if ( client->getPhase( ) == "CONNECTING" ) {
		client->disConnect( );
		client->initialize( );
	}

	//クリックのチェック
	checkHitButton( );
	if ( _data->getClickingLeft( ) > 0 ) {
		if ( _hit_button_idx != -1 ) {
			_past_hit_button_idx = _hit_button_idx;
		}
	} else {
		if ( _past_hit_button_idx == _hit_button_idx && _hit_button_idx != -1 ) {
			chengeNextScene( );
		}
		_past_hit_button_idx = -1;
	}

	drawBackGround( );
	drawTitle( );
	drawButton( );

	//_drawer->setFrontString( true, WIDTH / 2, HEIGHT / 2, RED, "Press \"Z\" Key" );
}

void Title::checkHitButton( ) {
	double mouse_x = _data->getMouseX( );
	double mouse_y = _data->getMouseY( );

	for ( int i = 0; i < BUTTON_TYPE_MAX; i++ ) {
		BoxCollider *box = &_button[ i ].collider;
		if ( box->lx <= mouse_x && mouse_x <= box->rx &&
			 box->ly <= mouse_y && mouse_y <= box->ry ) {
			_hit_button_idx = i;
			return;
		}
	}
	_hit_button_idx = -1;
}

void Title::chengeNextScene( ) {
	switch ( ( BUTTON_TYPE )_past_hit_button_idx ) {
	case START_BUTTON   : _data->setScene( CONNECT )     ; break;
	case SETTING_BUTTON : _data->setScene( SELECT_ITEM ) ; break;
	}
}

void Title::drawBackGround( ) const {
	_drawer->setBackImage( _back );
}

void Title::drawTitle( ) const {
	_drawer->setImage( _title_logo );
}

void Title::drawButton( ) const {
	ImageProperty image = ImageProperty( );
	for ( int i = 0; i < BUTTON_TYPE_MAX; i++ ) {
		image.cx = _button[ i ].image.cx;
		image.cy = _button[ i ].image.cy;
		image.png = _button_handles[ i * BUTTON_TYPE_MAX ];

		if ( _past_hit_button_idx != -1 && _past_hit_button_idx == i ) {
			image.png = _button_handles[ i * BUTTON_TYPE_MAX + 1 ];
		}

		_drawer->setImage( image );
	}
}