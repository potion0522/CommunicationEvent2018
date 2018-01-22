#include "DxLib.h"
#include "smart_ptr.h"
#include "GlobalData.h"
#include "Direction.h"
#include "Title.h"
#include "Console.h"
#include "Lazer.h"
#include "Field.h"
#include "Game.h"

/**********************************************************
*														  *
* setFlag( 1 );　をinitializeでしないとupdateされませんよ *
*														  *
***********************************************************/

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
	ChangeWindowMode( 1 );
	SetDoubleStartValidFlag( TRUE );
	SetAlwaysRunFlag( TRUE );
	SetConnectTimeOutWait( 60 );
	SetGraphMode( WIDTH, HEIGHT, 32 );
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
		GamePtr mirror( new Game( data ) );
		LazerPtr lazer( new Lazer( data ) );
		FieldPtr field( new Field( data ) );

		direction->add( TITLE, title );
		direction->add( CONNECT, console );
		direction->add( SET, mirror );
		direction->add( CALC, lazer );
		direction->add( CALC, field );

		data->setPtr( field );

		direction->initialize( );
		data->setScene( CALC );
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