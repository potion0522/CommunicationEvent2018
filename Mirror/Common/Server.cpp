#include "Server.h"
#include "DxLib.h"
#include "const.h"

Server::Server( ) {
	initialize( );
}

Server::~Server( ) {
	disConnect( );
}

std::string Server::getTag( ) {
	return "SERVER";
}

void Server::initialize( ) {
	setFlag( 1 );
	_connecting = false;
	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		_handles[ i ] = -1;
		_recving[ i ] = false;
	}
	_handle_udp = MakeUDPSocket( -1 );

	memset( _recv_data, 0, sizeof( Client::NetWorkData ) * MACHINE_MAX );

	createIP( );
	PreparationListenNetWork( TCP_PORT );
}

void Server::update( ) {
	accept( );
	lost( );
	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		if ( isConnecting( i ) ) {
			recvTcp( i );
		}
	}
}

void Server::accept( ) {
	int handle = GetNewAcceptNetWork( );

	if ( handle < 0 ) {
		return;
	}

	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		if ( _handles[ i ] < 0 ) {
			_handles[ i ] = handle;
			break;
		}
	}
	
	_connecting = true;
}

void Server::lost( ) {
	int lost = GetLostNetWork( );
	if ( lost < 0 ) {
		return;
	}
	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		if ( lost == _handles[ i ] ) {
			NetWorkRecvBufferClear( _handles[ i ] );
			CloseNetWork( _handles[ i ] );
			_handles[ i ] = -1;
			break;
		}
	}
}

void Server::sendDataTcp( int idx, Client::NetWorkData send_data ) {
	NetWorkSend( _handles[ idx ], &send_data, sizeof( Client::NetWorkData ) ); 
}

void Server::sendDataUdp( Client::NetWorkData send_data ) {
	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		if ( _handles[ i ] < 0 ) {
			continue;
		}
		IPDATA ip;
		GetNetWorkIP( _handles[ i ], &ip );
		NetWorkSendUDP( _handle_udp, ip, UDP_PORT, &send_data, sizeof( Client::NetWorkData ) ); 
	}
}
void Server::recvTcp( int idx ) {
	int size = GetNetWorkDataLength( _handles[ idx ] );
	if ( size < 0 ) {
		_recving[ idx ] = false;
		return;
	}

	int recv = NetWorkRecv( _handles[ idx ], &_recv_data[ idx ], sizeof( Client::NetWorkData ) );
	if ( recv < 0 ) {
		_recving[ idx ] = false;
		return;
	}
	_recving[ idx ] = true;
}

void Server::createIP( ) {
	IPDATA ip;
	GetMyIPAddress( &ip );

	FILE *fp;
	fopen_s( &fp, "IP.ini", "wb" );
	if ( fp == NULL ) {
		return;
	}
	fwrite( &ip, sizeof( IPDATA ), 1, fp );
}

bool Server::isConnecting( int idx ) const {
	return _handles[ idx ] != -1;
}

bool Server::isRecving( int idx ) const {
	return _recving[ idx ];
}

Client::NetWorkData Server::getData( int idx ) const {
	return _recv_data[ idx ];
}

void Server::disConnect( ) {
	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		if ( _handles[ i ] != -1 ) {
			NetWorkRecvBufferClear( _handles[ i ] );
		}
		CloseNetWork( _handles[ i ] );
	}

	NetWorkRecvBufferClear( _handle_udp );
	DeleteUDPSocket( _handle_udp );

	StopListenNetWork( );
}