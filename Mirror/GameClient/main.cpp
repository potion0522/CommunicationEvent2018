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

/**********************************************************
*														  *
* setFlag( 1 );　をinitializeでしないとupdateされませんよ *
*														  *
***********************************************************/

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
	SetWindowText( "Mirror" );
	ChangeWindowMode( 1 );
	//SetWindowSize( SCREEN_WIDTH, SCREEN_HEIGHT );
	SetGraphMode( WIDTH, HEIGHT, 32 );
	SetDoubleStartValidFlag( TRUE );
	SetAlwaysRunFlag( TRUE );
	SetConnectTimeOutWait( 60 );
	if ( DxLib_Init( ) != 0 ) {
		return -1;
	}
	SetDrawScreen( DX_SCREEN_BACK );

	{
		GlobalDataPtr data( new GlobalData( ) );
		DirectionPtr direction( new Direction( CLIENT, data ) );

		//計算クラス
		ConsolePtr console( new Console( data ) );
		TitlePtr title( new Title( data ) );
		FieldPtr field( new Field( data ) );
		data->setPtr( field );
		GamePtr game( new Game( data ) );
		LazerPtr lazer( new Lazer( data ) );
		ResultClientPtr result( new ResultClient( data, game ) );

		direction->add( TITLE, title );
		direction->add( CONNECT, console );
		direction->add( BATTLE, game );
		direction->add( BATTLE, field );
		direction->add( RESULT, result );

		//data->setScene( BATTLE );
		direction->initialize( );
		// GlobalData のフラグが 0 であれば全プロセス終了
		while ( data->getFlag( ) ) {
			if ( ScreenFlip( ) != 0 || ProcessMessage( ) != 0 || ClearDrawScreen( ) != 0 ) {
				break;
			}
			direction->run( );
		}
	}

	DxLib_End( );
	return 0;
}