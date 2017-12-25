#include "Console.h"
#include "GlobalData.h"
#include "Color.h"

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
	memset( &_recv_data_tcp, 0, sizeof( Client::NetWorkData ) );
	memset( &_recv_data_udp, 0, sizeof( Client::NetWorkData ) );
	memset( &_send_data, 0, sizeof( Client::NetWorkData ) );
}

void Console::update( ) {
	std::string phase = _client->getPhase( );
	
	if ( phase == "CONNECTING" ) {
		_client->sendTcp( _send_data );

		if ( _client->isRecvingTcp( ) ) {
			_recv_data_tcp = _client->getDataTcp( );
		}
		//if ( _client->isRecvingUdp( ) ) {
		//	_recv_data_udp = _client->getDataUdp( );
		//}
	}


	ColorPtr color( new Color( ) );
	DrawFormatString( WIDTH - 200, HEIGHT - 200, color->getColor( WHITE ), "recv_udp : %d", _recv_data_udp.test );
	DrawFormatString( WIDTH - 200, HEIGHT - 100, color->getColor( WHITE ), "PHASE : %s", phase.c_str( ) );
}