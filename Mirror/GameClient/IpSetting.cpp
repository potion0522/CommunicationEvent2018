#include "IpSetting.h"
#include "GlobalData.h"
#include "Drawer.h"
#include "Image.h"
#include "KeyBoard.h"
#include "Server.h"

const float MARGIN  = WIDTH_F / 5.0f;

const float SAVE_BUTTON_X = WIDTH_F / 3.0f * 2.0f;
const float SAVE_BUTTON_Y = HEIGHT_F / 5.0f;

const float RETURNBUTTON_X = WIDTH_F / 3.0f;
const float RETURNBUTTON_Y = HEIGHT_F / 5.0f;

const float WRITTING_BOX_X = WIDTH_F / 2.0f;
const float WRITTING_BOX_Y = HEIGHT_F / 2.0f;

const int PROGRESS_BAR_X = WIDTH / 2;
const int PROGRESS_BAR_Y = HEIGHT / 3 * 2;

IpSetting::IpSetting( GlobalDataPtr data ) :
_data( data ) {
	setFlag( 1 );
	_drawer = _data->getDrawerPtr( );
	_keyboard = KeyBoardPtr( new KeyBoard( ) );

	//キーコード入力
	std::vector< int >( ).swap( _keycode );
	for ( int i = 0; i < 10; i++ ) {
		_keycode.push_back( _keyboard->getKeyCode( ( KEY )( NUMPAD0_KEY + i ) ) );
	}
	for ( int i = 0; i < 10; i++ ) {
		_keycode.push_back( _keyboard->getKeyCode( ( KEY )( ZERO_KEY + i ) ) );
	}
	_keycode.push_back( _keyboard->getKeyCode( DECIMAL_KEY ) );


	ImagePtr image_ptr = _data->getImagePtr( );
	{//背景
		_background_handle = image_ptr->getPng( BACKGROUND_IMAGE, BACKGROUND_WHITE ).png;
	}

	{//決定ボタン
		float half_width  = image_ptr->getPng( BUTTON_IMAGE, SAVE_BUTTON_IDX ).width * 0.5f;
		float half_height = image_ptr->getPng( BUTTON_IMAGE, SAVE_BUTTON_IDX ).height * 0.5f;

		for ( int i = 0; i < BUTTON_TYPE_MAX; i++ ) {
			_savebutton_handles[ i ] = image_ptr->getPng( BUTTON_IMAGE, SAVE_BUTTON_IDX + i ).png;
		}
		_savebutton.image.cx = SAVE_BUTTON_X;
		_savebutton.image.cy = SAVE_BUTTON_Y;
		_savebutton.image.png = image_ptr->getPng( BUTTON_IMAGE, SAVE_BUTTON_IDX ).png;

		_savebutton.collider.lx = ( float )_savebutton.image.cx - half_width;
		_savebutton.collider.rx = ( float )_savebutton.image.cx + half_width;
		_savebutton.collider.ly = ( float )_savebutton.image.cy - half_height;
		_savebutton.collider.ry = ( float )_savebutton.image.cy + half_height;

	
	//リターンボタン
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

	
	{//プログレスバー
		_bar_frame = LightImageProperty( );
		_bar = LightImageProperty( );

		//フレーム
		_bar_frame.cx = PROGRESS_BAR_X;
		_bar_frame.cy = PROGRESS_BAR_Y;
		_bar_frame.png = image_ptr->getPng( BAR_IMAGE, 0 ).png;

		//バー
		_bar.cx = PROGRESS_BAR_X - image_ptr->getPng( BAR_IMAGE, 0 ).width / 2;
		_bar.cy = PROGRESS_BAR_Y;
		_bar.png = image_ptr->getPng( BAR_IMAGE, 1 ).png;
		_bar_width_half = image_ptr->getPng( BAR_IMAGE, 1 ).width / 2;
		_bar_height_half = image_ptr->getPng( BAR_IMAGE, 1 ).height / 2;

		int length = image_ptr->getPng( BAR_IMAGE, 0 ).width;
		_bar_rate = length / ( FRAME * 2 );
	}
}

IpSetting::~IpSetting( ) {
}

std::string IpSetting::getTag( ) {
	return "IPSETTING";
}

void IpSetting::initialize( ) {
	_drag = false;
	_savebutton_clicking = false;
	_returnbutton_clicking = false;
	_save = false;
	_wait_time = 0;
	_ip = "";
}

void IpSetting::update( ) {
	//Saveを押したら
	if ( _save ) {
		_drawer->setFrontString( true, WIDTH / 2, HEIGHT / 2, YELLOW, "IP情報を保存しています", Drawer::BIG );

		//プログレスバーの表示
		ImageProperty image = ImageProperty( );

		//バー
		image.cx = _bar.cx;
		image.cy = _bar.cy;
		image.png = _bar.png;
		_drawer->setFrontExtendImage( image, 0, _bar_height_half, _wait_time * _bar_rate, 1 );

		//フレーム
		image.cx = _bar_frame.cx;
		image.cy = _bar_frame.cy;
		image.png = _bar_frame.png;
		_drawer->setFrontImage( image );

		if ( _wait_time < ( FRAME * 2 ) ) {
			_wait_time++;
		} else {
			_data->setScene( TITLE );
		}
		return;
	}

	//決定ボタン
	if ( ( _savebutton_clicking || _returnbutton_clicking ) && !isDrag( ) ) {
		int hit = getHitButton( );
		switch ( hit ) {
		case SAVE_BUTTON: 
			{
				ServerPtr _server( new Server( ) );
				_ip.push_back( '\0' );
				_server->createIP( _ip );
				_save = true;
			}
			break;
		case RETURN_BUTTON:
			_data->setScene( TITLE );
			break;
		default:
			_savebutton_clicking = false;
			_returnbutton_clicking = false;
			break;		
		}
	}

	calcButtonAction( );
	writeIp( );

	//描画
	drawBackGround( );
	drawSaveButton( );
	drawReturnButton( );
	drawWritingBox( );
	drawIpString( );
}

void IpSetting::writeIp( ) {
	int size = ( int )_keycode.size( );

	for ( int i = 0; i < size; i++ ) {
		if ( _data->getKeyState( _keycode[ i ] ) == 1 ) {
			_ip += _keyboard->getKeyStr( _keycode[ i ] );
		}
	}

	if ( _data->getKeyState( _keyboard->getKeyCode( BACK_KEY ) ) == 1 ) {
		int del = ( int )_ip.length( );
		if ( del > 0 ) {
			_ip.erase( --del );
		}
	}
}

void IpSetting::calcButtonAction( ) {
	_savebutton.image.png = _savebutton_handles[ NORMAL ];
	_returnbutton.image.png = _returnbutton_handles[ NORMAL ];

	if ( getHitButton( ) == NONE_BUTTON ) {
		return;
	}

	if ( !isDrag( ) ) {
		return;
	}

	switch ( getHitButton( ) ) {
	case SAVE_BUTTON:
		_savebutton.image.png = _savebutton_handles[ CLICKING ];
		_savebutton_clicking = true;
		break;

	case RETURN_BUTTON:
		_returnbutton.image.png = _returnbutton_handles[ CLICKING ];
		_returnbutton_clicking = true;
		break;
	}
}

bool IpSetting::isDrag( ) const {
	if ( _data->getClickingLeft( ) > 0 ) {
		return true;
	}
	return false;
}

int IpSetting::getHitButton( ) const {
	double mouse_x = _data->getMouseX( );
	double mouse_y = _data->getMouseY( );

	//決定
	if ( _savebutton.collider.lx <= mouse_x && mouse_x <= _savebutton.collider.rx &&
		 _savebutton.collider.ly <= mouse_y && mouse_y <= _savebutton.collider.ry ) {
		return SAVE_BUTTON;
	}

	//リターン
	if ( _returnbutton.collider.lx <= mouse_x && mouse_x <= _returnbutton.collider.rx &&
		 _returnbutton.collider.ly <= mouse_y && mouse_y <= _returnbutton.collider.ry ) {
		return RETURN_BUTTON;
	}

	return NONE_BUTTON;
}

void IpSetting::drawBackGround( ) const {
	ImageProperty image = ImageProperty( );
	image.cx = WIDTH / 2;
	image.cy = HEIGHT / 2;
	image.png = _background_handle;

	_drawer->setBackGroundImage( image );
}

void IpSetting::drawIpString( ) const {
	_drawer->setFrontString( true, WRITTING_BOX_X, WRITTING_BOX_Y, BLACK, _ip, Drawer::LITTLE_BIG );
}

void IpSetting::drawWritingBox( ) const {
	const short int LENGTH = _drawer->getStringW( Drawer::BIG, "123.456.789.123" );
	_drawer->setBackBox( WRITTING_BOX_X - LENGTH, WRITTING_BOX_Y - _drawer->getStringH( Drawer::BIG ),
						  WRITTING_BOX_X + LENGTH, WRITTING_BOX_Y + _drawer->getStringH( Drawer::BIG ), BLACK, true );

	_drawer->setBackBox( WRITTING_BOX_X - LENGTH * 0.95, WRITTING_BOX_Y - _drawer->getStringH( Drawer::BIG ) * 0.95,
		                 WRITTING_BOX_X + LENGTH * 0.95, WRITTING_BOX_Y + _drawer->getStringH( Drawer::BIG ) * 0.95, WHITE, true );
}

void IpSetting::drawSaveButton( ) const {
	_drawer->setFrontImage( _savebutton.image );
}

void IpSetting::drawReturnButton( ) const {
	_drawer->setFrontImage( _returnbutton.image );
}