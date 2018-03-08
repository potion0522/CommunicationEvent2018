#include "DxLib.h"
#include "smart_ptr.h"
#include "GlobalData.h"
#include "Direction.h"
#include "Log.h"
#include "Connector.h"
#include "GameMaster.h"
#include "Command.h"
#include "Debug.h"
#include "ResultServer.h"

/**********************************************************
*														  *
* Base 継承クラスは　setFlag( 1 ); を絶対にすること       *
*														  *
***********************************************************/

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
	SetWindowText( "Mirror" );
	ChangeWindowMode( 1 );
	//SetWindowSize( SCREEN_WIDTH, SCREEN_HEIGHT );
	SetGraphMode( WIDTH, HEIGHT, 32 );
	SetDoubleStartValidFlag( TRUE );
	SetAlwaysRunFlag( TRUE );
	SetConnectTimeOutWait( TIME_OUT );
	if ( DxLib_Init( ) != 0 ) {
		return -1;
	}
	SetDrawScreen( DX_SCREEN_BACK );

	{
		GlobalDataPtr data( new GlobalData( SERVER ) );
		DirectionPtr direction( new Direction( SERVER, data ) );

		CommandPtr command( new Command( data ) );
		command->setFlag( 1 );
		LogPtr log( new Log( data ) );
		ConnectorPtr connector( new Connector( data, log, command ) );
		GameMasterPtr master( new GameMaster( data, connector, log, command ) );
		ResultServerPtr result( new ResultServer( data, master, log ) );

		direction->add( ALL, data );
		direction->add( CONNECT, connector );
		direction->add( BATTLE, master );
		direction->add( RESULT, result );

		// GlobalData のフラグが 0 であれば全プロセス終了
		while ( data->getFlag( ) ) {
			if ( ScreenFlip( ) != 0 || ProcessMessage( ) != 0 || ClearDrawScreen( ) != 0 ) {
				break;
			}
			if ( data->isInitFlag( ) ) {
				direction->initialize( );
				data->getDebugPtr( )->setFlag( 1 );
				data->setScene( CONNECT );
			}

			command->update( );
			direction->run( );
		}
	}

	DxLib_End( );
	return 0;
}