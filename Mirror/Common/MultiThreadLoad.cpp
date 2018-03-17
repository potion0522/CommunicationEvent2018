#pragma once
#include "MultiThreadLoad.h"
#include "DxLib.h"
#include "const.h"

const short int STRING_SIZE = 50;
const short int CHANGE_EFFECT_COUNT = 5;
const short int WAIT = CHANGE_EFFECT_COUNT * MultiThreadLoad::LOADING_EFFECT_MAX - 1;
const short int EFFECT_COUNT_MAX = CHANGE_EFFECT_COUNT * MultiThreadLoad::LOADING_EFFECT_MAX - 1;
const float MAX_LENGTH = WIDTH * 0.35f;

MultiThreadLoad::MultiThreadLoad( std::string message ) :
_message( message ) {
	_flag = true;
	_max = 0;
	_cnt = 0;

	_font_handle = CreateFontToHandle( "メイリオ", STRING_SIZE, -1, DX_FONTTYPE_NORMAL );

	std::array< int, RESOURCE_MAX >( ).swap( _handles );
	std::array< int, LOADING_EFFECT_MAX >( ).swap( _effect );

	//エフェクト
	for ( int i = 0; i < LOADING_EFFECT_MAX; i++ ) {
		_effect[ i ] = LoadGraph( ( "Resources/loading/effect" + std::to_string( i ) + ".png" ).c_str( ) );
	}

	//その他
	for ( int i = 0; i < RESOURCE_MAX; i++ ) {
		_handles[ i ] = LoadGraph( ( "Resources/loading/image" + std::to_string( i ) + ".png" ).c_str( ) );
	}
	SetUseASyncLoadFlag( TRUE );
}

MultiThreadLoad::MultiThreadLoad( ) {
	_flag = false;
	_max = 0;
	_cnt = 0;
}

MultiThreadLoad::~MultiThreadLoad( ) {
}

void MultiThreadLoad::update( ) {
	while ( _flag ) {

		ProcessMessage( );
		ClearDrawScreen( );

		int temp = GetASyncLoadNum( );
		float length = ( _max - GetASyncLoadNum( ) ) / _max;

		//背景
		//DrawGraph( 0, 0, _handles[ Loading::BACKGROUND ], TRUE );

		//メッセージ
		DrawFormatStringToHandle( WIDTH / 2 - STRING_SIZE * ( ( int )_message.length( ) / 4 ), ( int )( HEIGHT * 0.3 ), 0xffffff, _font_handle, _message.c_str( ) );
		DrawFormatStringToHandle( WIDTH / 2 - STRING_SIZE * ( ( int )_message.length( ) / 4 ), ( int )( HEIGHT * 0.4 ), 0xff0000, _font_handle, "%.1f%%", length * 100 );

		const float POS_X = WIDTH * 0.4f;
		const float POS_Y = HEIGHT * 0.65f;
		if ( length < 1 ) {
			//的
			DrawRotaGraphF( POS_X - 50 + MAX_LENGTH, POS_Y, 1, 0, _handles[ MultiThreadLoad::PLAYER ], TRUE );
		}
		//発射台
		DrawRotaGraphF( POS_X - 96, POS_Y, 1, 0, _handles[ MultiThreadLoad::LAZER ], TRUE );
		//レーザー
		DrawRotaGraph3F( POS_X - 50, POS_Y, 8, 0, 1, length * ( MAX_LENGTH / 16 ), -PI / 2, _handles[ MultiThreadLoad::LAZER_LINE ], FALSE, FALSE );


		//ロードし終わったら
		if ( length == 1 ) {
			//エフェクト
			if ( _cnt < EFFECT_COUNT_MAX ) {
				//エフェクトを切り替える
				DrawRotaGraphF( POS_X - 50 + MAX_LENGTH, POS_Y, 1, 0, _effect[ _cnt / CHANGE_EFFECT_COUNT ], TRUE );
			} else {
				DrawRotaGraphF( POS_X - 50 + MAX_LENGTH, POS_Y, 1 + _cnt * 0.0025, 0, _effect[ LOADING_EFFECT_MAX - 1 ], TRUE );
				//猶予を持たせる
				if ( _cnt > WAIT ) {
					////終了////
					SetUseASyncLoadFlag( FALSE );
					return;
				}
			}
			//カウントを増やす
			_cnt++;
		}
		ScreenFlip( );
		WaitVSync( 1 );
	}
}

void MultiThreadLoad::setMaxLength( float max ) {
	_max = max;
}