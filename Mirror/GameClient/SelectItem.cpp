#include "SelectItem.h"
#include "GlobalData.h"
#include "Drawer.h"
#include "const.h"
#include "Image.h"

SelectItem::SelectItem( GlobalDataPtr data ) :
_data( data ) {
	setFlag( 1 );
	_drawer = _data->getDrawerPtr( );
	ImagePtr image_ptr = _data->getImagePtr( );
	_back_handle = image_ptr->getPng( NONE_IMAGE, 0 ).png;
}

SelectItem::~SelectItem( ) {
}

std::string SelectItem::getTag( ) {
	return "SELECT_ITEM";
}

void SelectItem::initialize( ) {

}

void SelectItem::update( ) {

	//•`‰æ
	drawBackGround( );
}

void SelectItem::drawBackGround( ) const {
	ImageProperty image = ImageProperty( );
	image.cx = WIDTH / 2;
	image.cy = HEIGHT / 2;
	image.png = _back_handle;

	_drawer->setImage( image );
}