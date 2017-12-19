#include "DxLib.h"
#include "GlobalData.h"
#include "Direction.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
	ChangeWindowMode( 1 );
	SetGraphMode( WIDTH, HEIGHT, 32 );
	if ( DxLib_Init( ) != 0 ) {
		return -1;
	}
	SetDrawScreen( DX_SCREEN_BACK );

	{
		std::shared_ptr< GlobalData > data( new GlobalData( ) );
		std::shared_ptr< Direction > direction( new Direction( data ) );

		// GlobalData のフラグが 0 であれば全プロセス終了
		while ( data->getFlag( ) ) {
			if ( ScreenFlip( ) != 0 || ProcessMessage( ) != 0 || ClearDrawScreen( ) != 0 ) {
				break;
			}

			//計算フェイズ
			direction->update( );

			if ( data->getKeyState( KEY_INPUT_ESCAPE ) == 1 ) {
				break;
			}
		}
	}

	DxLib_End( );
	return 0;
}