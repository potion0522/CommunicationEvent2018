#include "Client.h"
#include <string>

const int PORT = 8000;

Client::Client( ) {
	initialize( );
}

Client::~Client( ) {
}

void Client::initialize( ) {
	_phase = READY;
	_recving = false;
	_handle = -1;

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
	const int SIZE = 64;
	char buf[ SIZE ] = { };

	handle = FileRead_open( "IP.ini" );
	FileRead_read( &_ip, sizeof( IPDATA ), handle );

	FileRead_close( handle );
}

void Client::connect( ) {
	_handle = ConnectNetWork( _ip, PORT );
	if ( _handle < 0 ) {
		return;
	}

	_phase = CONNECTING;
}

void Client::recving( ) {
	recvTcp( );
	lost( );
}

void Client::recvTcp( ) {
	int size = GetNetWorkDataLength( _handle );
	if ( size < 0 ) {
		_recving = false;
		return;
	}

	int recv = NetWorkRecv( _handle, &_recv_data, size );
	if ( recv < 0 ) {
		_recving = false;
		return;
	}
	_recving = true;
}

void Client::lost( ) {
	int lost = GetLostNetWork( );
	if ( lost == _handle ) {
		CloseNetWork( _handle );
		_handle = -1;
		_phase = READY;
	}
}


bool Client::isRecving( ) const {
	return _recving;
}

void Client::sendTcp( NetWorkData send_data ) {
	NetWorkSend( _handle, &send_data, sizeof( NetWorkData ) );
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

Client::NetWorkData Client::getData( ) const {
	return _recv_data;
}