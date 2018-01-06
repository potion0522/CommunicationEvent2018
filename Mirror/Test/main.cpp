#include "DxLib.h"
#include "GlobalData.h"
#include "Test.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
	ChangeWindowMode( 1 );
	SetDoubleStartValidFlag( TRUE );
	SetAlwaysRunFlag( TRUE );
	SetConnectTimeOutWait( 10 );
	SetGraphMode( WIDTH, HEIGHT, 32 );
	if ( DxLib_Init( ) != 0 ) {
		return -1;
	}
	SetDrawScreen( DX_SCREEN_BACK );

	{
		GlobalDataPtr data( new GlobalData( ) );
		TestPtr test( new Test( data ) );

		// GlobalData のフラグが 0 であれば全プロセス終了
		while ( data->getFlag( ) ) {
			if ( ScreenFlip( ) != 0 || ProcessMessage( ) != 0 || ClearDrawScreen( ) != 0 ) {
				break;
			}
			if ( data->getKeyState( KEY_INPUT_ESCAPE ) == 1 ) {
				break;
			}
			test->update( );
		}
	}

	DxLib_End( );
	return 0;
}