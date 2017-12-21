#include "Title.h"
#include "DxLib.h"
#include "GlobalData.h"
#include "Image.h"

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
	setFlag( 0 );

	std::shared_ptr< Image > image = _data->getImagePtr( );
	_back.png = image->getPng( NONE_IMAGE, 0 ).png;
	_back.cx = WIDTH / 2;
	_back.cy = HEIGHT / 2;
}

void Title::update( ) {
}

Base::ImageProperty* Title::getBackPng( ) const {
	return ( ImageProperty* )&_back;
}