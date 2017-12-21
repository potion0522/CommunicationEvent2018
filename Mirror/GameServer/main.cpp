#include "DxLib.h"
#include "smart_ptr.h"
#include "GlobalData.h"
#include "Direction.h"
#include "Server.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
	ChangeWindowMode( 1 );
	SetGraphMode( WIDTH, HEIGHT, 32 );
	if ( DxLib_Init( ) != 0 ) {
		return -1;
	}
	SetDrawScreen( DX_SCREEN_BACK );

	GlobalDataPtr data( new GlobalData( ) );
	ServerPtr server( new Server( ) );
	data->setPtr( server );

	DirectionPtr direction( new Direction( SERVER, data ) );

	// GlobalData のフラグが 0 であれば全プロセス終了
	while ( data->getFlag( ) ) {
		if ( ScreenFlip( ) != 0 || ProcessMessage( ) != 0 || ClearDrawScreen( ) != 0 ) {
			break;
		}

		//計算フェイズ
		direction->run( );

		//描画

	}


	DxLib_End( );
	return 0;
}

/*
	_debug->addLog( "接続を待っています。" );

	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		if ( _server->isConnecting( i ) ) {
			std::string log = "_handle[ " + std::to_string( i ) + " ]が接続しました。";
			_debug->addLog( log );
			Client::NetWorkData data;
			data.test = i + 1;
			_server->sendData( i, data );
		}
	}

	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		if ( !_server->isConnecting( i ) ) {
			continue;
		}
		if ( !_server->isRecving( i ) ) {
			continue;
		}
		Client::NetWorkData data;
		data = _server->getData( i );
		_debug->addLog( std::to_string( data.test ) );
	}
*/