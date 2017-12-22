#include "Console.h"
#include "GlobalData.h"

Console::Console( GlobalDataPtr data ) :
_data( data ) {
	initialize( );
}

Console::~Console( ) {
}

std::string Console::getTag( ) {
	return "CONSOLE";
}

void Console::initialize( ) {
	setFlag( 1 );
	_client = _data->getClientPtr( );
	memset( &_recv_data, 0, sizeof( Client::NetWorkData ) );
	memset( &_send_data, 0, sizeof( Client::NetWorkData ) );
}

void Console::update( ) {
	if ( _client->isRecving( ) ) {
		_recv_data = _client->getData( );
	}
	
	if ( _client->getPhase( ).find( "CONNECTING" ) != std::string::npos ) {
		_client->sendTcp( _send_data );
	}
}