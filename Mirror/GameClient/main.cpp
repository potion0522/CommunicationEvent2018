#include "DxLib.h"
#include "smart_ptr.h"
#include "GlobalData.h"
#include "Direction.h"
#include "Title.h"
#include "Console.h"
#include "Lazer.h"
#include "Field.h"
#include "Game.h"
#include "ResultClient.h"
#include "SelectItem.h"
#include "IpSetting.h"
#include "LoadCSV.h"
#include <vector>
#include <string>

/**********************************************************
*														  *
* Base 継承クラスは　setFlag( 1 ); を絶対にすること       *
*														  *
***********************************************************/

void setScreenSize( int *width, int *height ) {
	*width = SCREEN_WIDTH;
	*height = SCREEN_HEIGHT;

	LoadCSVPtr load( new LoadCSV( ) );
	std::vector< CsvData > data;
	load->read( data, "screen" );

	int size = ( int )data.size( );
	for ( int i = 0; i < size; i++ ) {
		if ( "CLIENT_WIDTH" == data[ i ].tag ) {
			*width = atoi( data[ i ].values.begin( )->c_str( ) );
			continue;
		}

		if ( "CLIENT_HEIGHT" == data[ i ].tag ) {
			*height = atoi( data[ i ].values.begin( )->c_str( ) );
			continue;
		}
	}
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
	SetWindowText( "Mirror" );
	ChangeWindowMode( TRUE );

	//スクリーンサイズを詰める
	int screen_width = 0;
	int screen_height = 0;
	setScreenSize( &screen_width, &screen_height );

	SetWindowSize( screen_width, screen_height );
	SetGraphMode( WIDTH, HEIGHT, 32 );
	SetDoubleStartValidFlag( TRUE );
	SetAlwaysRunFlag( TRUE );
	SetConnectTimeOutWait( TIME_OUT );
	if ( DxLib_Init( ) != 0 ) {
		return -1;
	}
	SetDrawScreen( DX_SCREEN_BACK );

	{
		GlobalDataPtr data( new GlobalData( CLIENT ) );
		DirectionPtr direction( new Direction( CLIENT, data ) );

		//計算クラス
		ConsolePtr console( new Console( data ) );
		TitlePtr title( new Title( data ) );
		SelectItemPtr slct_item( new SelectItem( data ) );
		IpSettingPtr ip_setting( new IpSetting( data ) );
		FieldPtr field( new Field( data ) );
		data->setPtr( field );
		GamePtr game( new Game( data ) );
		ResultClientPtr result( new ResultClient( data, game ) );

		direction->add( ALL, data );
		direction->add( TITLE, title );
		direction->add( SELECT_ITEM, slct_item );
		direction->add( IP_SETTING, ip_setting );
		direction->add( CONNECT, console );
		direction->add( BATTLE, field );
		direction->add( BATTLE, game );
		direction->add( RESULT, result );

		// GlobalData のフラグが 0 であれば全プロセス終了
		while ( data->getFlag( ) ) {
			if ( ScreenFlip( ) != 0 || ProcessMessage( ) != 0 || ClearDrawScreen( ) != 0 ) {
				break;
			}
			if ( data->isInitFlag( ) ) {
				direction->initialize( );
				data->setScene( TITLE );
			}
			direction->run( );
		}
	}

	DxLib_End( );
	return 0;
}