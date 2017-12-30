#include "Client.h"
#include "const.h"

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
	_handle_tcp = -1;
	_handle_udp = MakeUDPSocket( UDP_PORT );

	memset( &_recv_data_tcp, 0, sizeof( NetWorkData ) );
	memset( &_recv_data_udp, 0, sizeof( NetWorkData ) );

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
}

void Client::recvTcp( ) {
	int size = GetNetWorkDataLength( _handle_tcp );
	if ( size < 1 ) {
		_recving_tcp = false;
		return;
	}

	int recv = NetWorkRecv( _handle_tcp, &_recv_data_tcp, size );
	if ( recv < 1 ) {
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


bool Client::isRecvingTcp( ) const {
	return _recving_tcp;
}

bool Client::isRecvingUdp( ) const {
	return _recving_udp;
}

void Client::sendTcp( NetWorkData send_data ) {
	NetWorkSend( _handle_tcp, &send_data, sizeof( NetWorkData ) );
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

Client::NetWorkData Client::getDataTcp( ) const {
	return _recv_data_tcp;
}

Client::NetWorkData Client::getDataUdp( ) const {
	return _recv_data_udp;
}

void Client::disConnect( ) {
	if ( _handle_tcp != -1 ) {
		NetWorkRecvBufferClear( _handle_tcp );
	}
	NetWorkRecvBufferClear( _handle_udp );
	CloseNetWork( _handle_tcp );
	DeleteUDPSocket( _handle_udp );
}