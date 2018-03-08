#include "Message.h"
#include "Drawer.h"
#include "const.h"

Message::Message( DrawerPtr drawer ) :
_drawer( drawer ) {
	initialize( );
}

Message::~Message( ) {
}

void Message::initialize( ) {
	std::vector< std::string >( ).swap( _message );
}

void Message::update( ) {
	int size = ( int )_message.size( );
	if ( size < 1 ) {
		return;
	}

	const int FONT_PITCH_Y = _drawer->getStringH( Drawer::BIG );
	for ( int i = 0; i < size; i++ ) {
		_drawer->setFrontString( true, WIDTH / 2, HEIGHT / 3 + i * FONT_PITCH_Y, WATER, _message[ i ], Drawer::BIG );
	}

	_drawer->setFrontString( true, WIDTH / 2, HEIGHT / 3 + ( size + 2 ) * FONT_PITCH_Y, WHITE, "ƒNƒŠƒbƒN‚µ‚Ä‚­‚¾‚³‚¢", Drawer::LITTLE_BIG );
}

void Message::add( std::string str ) {
	_message.push_back( str );
}