#include "Title.h"
#include "DxLib.h"
#include "GlobalData.h"
#include "Image.h"
#include "BaseDraw.h"
#include "Drawer.h"

Title::Title( GlobalDataPtr data ) :
_data( data ) {
	initialize( );
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
	if ( _data->getKeyState( KEY_INPUT_Z ) == 1 ) {
		_data->setScene( CONNECT );
	}
	
	DrawerPtr drawer = _data->getDrawerPtr( );
	drawer->setImage( _back );
}

Base::ImageProperty* Title::getBackPng( ) const {
	return ( ImageProperty* )&_back;
}