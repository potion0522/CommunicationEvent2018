#pragma once
#include "Loading.h"
#include "DxLib.h"
#include "const.h"

Loading::Loading( std::string message ) {
	_message = message;
	_arg = 0;
	_fin = false;
	//_drawer = Drawer( );
	//std::array< int, RESOURCE_MAX >( ).swap( _handles );
	//std::array< int, EFFECT_MAX >( ).swap( _effect );

	////エフェクト
	//for ( int i = 0; i < EFFECT_MAX; i++ ) {
	//	_effect[ i ] = LoadGraph( ( "Resources/loading/effect" + std::to_string( i ) + ".png" ).c_str( ) );
	//}

	////その他
	//for ( int i = 0; i < RESOURCE_MAX; i++ ) {
	//	_handles[ i ] = LoadGraph( ( "Resources/loading/image" + std::to_string( i ) + ".png" ).c_str( ) );
	//}
}

Loading::~Loading( ) {
}

void Loading::update( ) {
	while ( _arg < 1000 ) {
		ProcessMessage( );
		ClearDrawScreen( );

		//Base::LightImageProperty image = Base::LightImageProperty( );
		//image.cx = WIDTH / 2;
		//image.cy = HEIGHT / 2;
		//image.png = _handles[ Loading::BACKGROUND ];

		//_drawer.setBackGroundImage( image );
		//_drawer.setFrontString( true, WIDTH / 2, HEIGHT / 2, RED, _message );
		//_drawer.update( );

		DrawString( 0, 0, "aaaaaaa", 0xffffff );

		ScreenFlip( );
		_arg++;
	}
}

void Loading::finish( ) {
	_fin = true;
}
