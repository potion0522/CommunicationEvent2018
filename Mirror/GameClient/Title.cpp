#include "Title.h"
#include "GlobalData.h"
#include "Image.h"
#include "Drawer.h"
#include "Client.h"
#include "const.h"

const float STARTBUTTON_X = WIDTH_F / 2.0f;
const float STARTBUTTON_Y = HEIGHT_F / 2.0f;

const float SETTINGBUTTON_X = STARTBUTTON_X;
const float SETTINGBUTTON_Y = HEIGHT_F / 4.0f * 3.0f;

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

	{//startボタン
		float half_width  = image_ptr->getPng( BUTTON_IMAGE, GAME_START_BUTTON_IDX ).width * 0.5f;
		float half_height = image_ptr->getPng( BUTTON_IMAGE, GAME_START_BUTTON_IDX ).height * 0.5f;

		for ( int i = 0; i < BUTTON_TYPE_MAX; i++ ) {
			_startbutton_handles[ i ] = image_ptr->getPng( BUTTON_IMAGE, GAME_START_BUTTON_IDX + i ).png;
		}
		_startbutton.image.cx = STARTBUTTON_X;
		_startbutton.image.cy = STARTBUTTON_Y;
		_startbutton.image.png = image_ptr->getPng( BUTTON_IMAGE, GAME_START_BUTTON_IDX ).png;

		_startbutton.collider.lx = ( float )_startbutton.image.cx - half_width;
		_startbutton.collider.rx = ( float )_startbutton.image.cx + half_width;
		_startbutton.collider.ly = ( float )_startbutton.image.cy - half_height;
		_startbutton.collider.ry = ( float )_startbutton.image.cy + half_height;
	
	//settingボタン
		for ( int i = 0; i < BUTTON_TYPE_MAX; i++ ) {
			_settingbutton_handles[ i ] = image_ptr->getPng( BUTTON_IMAGE, ITEM_SELECT_BUTTON_IDX + i ).png;
		}
		_settingbutton.image.cx = SETTINGBUTTON_X;
		_settingbutton.image.cy = SETTINGBUTTON_Y;
		_settingbutton.image.png = image_ptr->getPng( BUTTON_IMAGE, ITEM_SELECT_BUTTON_IDX ).png;

		_settingbutton.collider.lx = ( float )_settingbutton.image.cx - half_width;
		_settingbutton.collider.rx = ( float )_settingbutton.image.cx + half_width;
		_settingbutton.collider.ly = ( float )_settingbutton.image.cy - half_height;
		_settingbutton.collider.ry = ( float )_settingbutton.image.cy + half_height;
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
	_startbutton_clicking = false;
	_settingbutton_clicking = false;
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
	chengeNextScene( );
	calcButtonAction( );

	drawBackGround( );
	drawTitle( );
	drawStartButton( );
	drawSettingButton( );
}

void Title::chengeNextScene( ) {
	if ( ( _startbutton_clicking || _settingbutton_clicking ) && !isDrag( ) ) {
		int hit = isHitButton( );
		switch ( hit ) {
		case START_BUTTON   : _data->setScene( CONNECT )     ; break;
		case SETTING_BUTTON : _data->setScene( SELECT_ITEM ) ; break;
		default:
			_startbutton_clicking = false;
			_settingbutton_clicking = false;
			break;		
		}
	}
}

void Title::calcButtonAction( ){
	_startbutton.image.png = _startbutton_handles[ NORMAL ];
	_settingbutton.image.png = _settingbutton_handles[ NORMAL ];

	if ( isHitButton( ) == NONE_BUTTON ) {
		return;
	}

	if ( !isDrag( ) ) {
		return;
	}

	if ( isHitButton( ) == START_BUTTON ) {
		_startbutton.image.png = _startbutton_handles[ CLICKING ];
		_startbutton_clicking = true;
	} else if ( isHitButton( ) == SETTING_BUTTON ) {
		_settingbutton.image.png = _settingbutton_handles[ CLICKING ];
		_settingbutton_clicking = true;
	}
}

int Title::isHitButton( ) const {
	double mouse_x = _data->getMouseX( );
	double mouse_y = _data->getMouseY( );
	
	//start button
	if ( _startbutton.collider.lx <= mouse_x && mouse_x <= _startbutton.collider.rx &&
		 _startbutton.collider.ly <= mouse_y && mouse_y <= _startbutton.collider.ry ) {
		return START_BUTTON;
	}

	//setting button
	if ( _settingbutton.collider.lx <= mouse_x && mouse_x <= _settingbutton.collider.rx &&
		 _settingbutton.collider.ly <= mouse_y && mouse_y <= _settingbutton.collider.ry ) {
		return SETTING_BUTTON;
	}

	return NONE_BUTTON;
}

bool Title::isDrag( ) const {
	if ( _data->getClickingLeft( ) > 0 ) {
		return true;
	}
	return false;
}

void Title::drawBackGround( ) const {
	_drawer->setBackImage( _back );
}

void Title::drawTitle( ) const {
	_drawer->setFrontImage( _title_logo );
}

void Title::drawStartButton( ) const {
	_drawer->setFrontImage( _startbutton.image );
}

void Title::drawSettingButton( ) const {
	_drawer->setFrontImage( _settingbutton.image );
}