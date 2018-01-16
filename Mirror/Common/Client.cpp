#include "Client.h"

Client::Client( ) {
}

Client::~Client( ) {
	disConnect( );
}

std::string Client::getTag( ) {
	return "CLIENT";
}

void Client::initialize( ) {
	setFlag( 1 );
	_phase = READY;
	_recving_tcp = false;
	_recving_udp = false;
	_matching = false;
	_player_num = 0;
	_handle_tcp = -1;
	if ( _handle_udp < 0 ) {
		_handle_udp = MakeUDPSocket( UDP_PORT );
	}
	_recv_data_udp = NetWorkData( );
	_send_data_udp = NetWorkData( );

	readIP( );
}

void Client::update( ) {
	switch ( _phase ) {
	case READY		: connect( ); break;
	case CONNECTING	: recving( ); break;
	}
}

void Client::readIP( ) {
	int handle = 0;
	_ip = IPDATA( );
	handle = FileRead_open( "IP.ini" );
	if ( handle == 0 ) {
		handle = FileRead_open( "../IP.ini" );
	}
	FileRead_read( &_ip, sizeof( IPDATA ), handle );

	FileRead_close( handle );
}

void Client::connect( ) {
	if ( _handle_tcp < 0 ) {
		int handle = ConnectNetWork( _ip, TCP_PORT );
		if ( handle < 0 ) {
			return;
		}
		_handle_tcp = handle;
		_phase = CONNECTING;
	}
}

void Client::recving( ) {
	recvTcp( );
	recvUdp( );
	lost( );
	clearBuffer( );
}

void Client::recvTcp( ) {
	int data = GetNetWorkDataLength( _handle_tcp );
	if ( data < 1 ) {
		_recving_tcp = false;
		return;
	}

	int size = sizeof( bool ) + sizeof( int );
	char *buf;
	buf = ( char* )malloc( size );
	int recv = NetWorkRecv( _handle_tcp, buf, size );
	_matching = *( bool* )buf;
	if ( _matching ) {
		_player_num = *( int* )( buf + sizeof( bool ) );
	}

	free( buf );

	if ( recv < 0 ) {
		_recving_tcp = false;
		return;
	}
	_recving_tcp = true;
}

void Client::recvUdp( ) {
	_recving_udp = false;
	if ( CheckNetWorkRecvUDP( _handle_udp ) == TRUE ) {
		NetWorkRecvUDP( _handle_udp, NULL, NULL, &_recv_data_udp, sizeof( NetWorkData ), FALSE );
		_recving_udp = true;
	}
}

void Client::lost( ) {
	int lost = GetLostNetWork( );
	if ( lost < 0 ) {
		return;
	}
	if ( lost == _handle_tcp ) {
		NetWorkRecvBufferClear( _handle_tcp );
		CloseNetWork( _handle_tcp );
		_handle_tcp = -1;
		_phase = READY;
	}
}

void Client::clearBuffer( ) {
	NetWorkRecvBufferClear( _handle_udp );
}

bool Client::isRecvingTcp( ) const {
	return _recving_tcp;
}

bool Client::isRecvingUdp( ) const {
	return _recving_udp;
}

void Client::sendTcp( ) {
	NetWorkSend( _handle_tcp, &_send_data_udp, sizeof( NetWorkData ) );
}

std::string Client::getSeverIP( ) const {
	std::string ip = "";
	ip += std::to_string( _ip.d1 );
	ip += ".";
	ip += std::to_string( _ip.d2 );
	ip += ".";
	ip += std::to_string( _ip.d3 );
	ip += ".";
	ip += std::to_string( _ip.d4 );

	return ip;
}

std::string Client::getPhase( ) const {
	std::string phase = "";
	switch ( _phase ) {
	case READY		: phase = "READY"		; break;
	case CONNECTING	: phase = "CONNECTING"	; break;
	}

	return phase;
}

bool Client::isMatching( ) const {
	return _matching;
}

void Client::disConnect( ) {
	if ( _handle_tcp > 0 ) {
		if ( GetNetWorkDataLength( _handle_tcp ) > 0 ) {
			NetWorkRecvBufferClear( _handle_tcp );
		}
	}
	CloseNetWork( _handle_tcp );
	NetWorkRecvBufferClear( _handle_udp );
	DeleteUDPSocket( _handle_udp );
}

void Client::setOrder( int order ) {
	_send_data_udp.order = ( unsigned char )order;
}

void Client::setX( int x ) {
	_send_data_udp.x = ( unsigned char )x;
}

void Client::setY( int y ) {
	_send_data_udp.y = ( unsigned char )y;
}

void Client::setAngle( MIRROR_ANGLE angle ) {
	_send_data_udp.angle = ( unsigned char )angle;
}

void Client::setFinish( bool fin ) {
	_send_data_udp.fin = fin;
}

int Client::getPlayerNum( ) const {
	return _player_num;
}

int Client::getOrder( ) const {
	return ( int )_recv_data_udp.order;
}

int Client::getX( ) const {
	return ( int )_recv_data_udp.x;
}

int Client::getY( ) const {
	return ( int )_recv_data_udp.y;
}

MIRROR_ANGLE Client::getAngle( ) const {
	return ( MIRROR_ANGLE )_recv_data_udp.angle;
}

BATTLE_PHASE Client::getBattlePhase( ) const {
	return ( BATTLE_PHASE )_recv_data_udp.phase;
}