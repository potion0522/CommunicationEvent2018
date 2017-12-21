#include "Server.h"
#include "DxLib.h"

const int PORT = 8000;

Server::Server( ) {
	initialize( );
}

Server::~Server( ) {
	StopListenNetWork( );
	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		CloseNetWork( _handles[ i ] );
	}
}

std::string Server::getTag( ) {
	return "SERVER";
}

void Server::initialize( ) {
	_connecting = false;
	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		_handles[ i ] = -1;
		_recving[ i ] = false;
	}
	memset( _recv_data, 0, sizeof( Client::NetWorkData ) * MACHINE_MAX );
	createIP( );
	PreparationListenNetWork( PORT );
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
			CloseNetWork( _handles[ i ] );
			_handles[ i ] = -1;
			break;
		}
	}
}

void Server::sendData( int idx, Client::NetWorkData send_data ) {
	NetWorkSend( _handles[ idx ], &send_data, sizeof( Client::NetWorkData ) ); 
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