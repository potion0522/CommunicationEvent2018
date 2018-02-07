#include "Title.h"
#include "DxLib.h"
#include "GlobalData.h"
#include "Image.h"
#include "Drawer.h"
#include "Client.h"

Title::Title( GlobalDataPtr data ) :
_data( data ) {
	_drawer = _data->getDrawerPtr( );
}

Title::~Title( ) {
}

std::string Title::getTag( ) {
	return "TITLE";
}

void Title::initialize( ) {
	setFlag( 1 );

	std::shared_ptr< Image > image = _data->getImagePtr( );
	_back.png = image->getPng( NONE_IMAGE, 0 ).png;
	_back.cx = WIDTH / 2;
	_back.cy = HEIGHT / 2;
	_back.size = 1;
}

void Title::update( ) {
	if ( _data->getKeyState( KEY_INPUT_Z ) == 1 && !_data->getCommandFlag( ) ) {
		_data->setScene( CONNECT );
	}
	ClientPtr client = _data->getClientPtr( );
	if ( client->getPhase( ) == "CONNECTING" ) {
		client->disConnect( );
		client->initialize( );
	}

	_drawer->setBackImage( _back );
	_drawer->setFrontString( true, WIDTH / 2, HEIGHT / 3, YELLOW, "MIRROR", Drawer::SUPER_BIG );
	_drawer->setFrontString( true, WIDTH / 2, HEIGHT / 2, RED, "Press \"Z\" Key" );
}