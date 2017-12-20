#include "Server.h"
#include "DxLib.h"
#include <string>

const int PORT = 8000;

Server::Server( ) {
	initialize( );
}

Server::~Server( ) {
	StopListenNetWork( );
}

void Server::initialize( ) {
	_connecting = false;
	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		_handles[ i ] = -1;
	}
	createIP( );
	PreparationListenNetWork( PORT );
}

void Server::update( ) {
	accept( );
	lost( );
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

void Server::createIP( ) {
	IPDATA ip;
	GetMyIPAddress( &ip );

	std::string ip_str = "";
	ip_str += std::to_string( ip.d1 );
	ip_str += ".";
	ip_str += std::to_string( ip.d2 );
	ip_str += ".";
	ip_str += std::to_string( ip.d3 );
	ip_str += ".";
	ip_str += std::to_string( ip.d4 );

	FILE *fp;
	fopen_s( &fp, "IP.ini", "wb" );
	fwrite( &ip_str, 1, ip_str.size( ), fp );
}

bool Server::isConnecting( int idx ) const {
	return _handles[ idx ] != -1;
}