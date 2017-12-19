#include "DxLib.h"
#include "GlobalData.h"
#include "Direction.h"
#include "Title.h"
#include "Debug.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
	ChangeWindowMode( 1 );
	SetGraphMode( WIDTH, HEIGHT, 32 );
	if ( DxLib_Init( ) != 0 ) {
		return -1;
	}
	SetDrawScreen( DX_SCREEN_BACK );

	std::shared_ptr< GlobalData > data( new GlobalData( ) );
	std::shared_ptr< Debug > debug( new Debug( data ) );

	std::shared_ptr< Direction > direction( new Direction( data, debug ) );
	std::shared_ptr< Title > title( new Title( data ) );

	data->setPtr( debug );
	data->setPtr( title );

	direction->add( ALL, debug );
	direction->add( TITLE, title );

	direction->run( );


	DxLib_End( );
	return 0;
}