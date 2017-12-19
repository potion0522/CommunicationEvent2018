#include "DxLib.h"
#include "GlobalData.h"
#include "Debug.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
	ChangeWindowMode( 1 );
	SetGraphMode( WIDTH, HEIGHT, 32 );
	if ( DxLib_Init( ) != 0 ) {
		return -1;
	}
	SetDrawScreen( DX_SCREEN_BACK );

	{
		std::shared_ptr< GlobalData > data( new GlobalData( ) );
		std::shared_ptr< Debug > debug( new Debug( ) );
		data->setFlag( 1 );

		int test = 0;
		// GlobalData のフラグが 0 であれば全プロセス終了
		while ( data->getFlag( ) ) {
			if ( ScreenFlip( ) != 0 || ProcessMessage( ) != 0 || ClearDrawScreen( ) != 0 ) {
				break;
			}

			data->update( );

			debug->addLog( "test" + std::to_string( test ) );
			debug->update( );

			test++;
			if ( data->getKeyState( KEY_INPUT_ESCAPE ) == 1 ) {
				break;
			}
		}
	}

	DxLib_End( );
	return 0;
}