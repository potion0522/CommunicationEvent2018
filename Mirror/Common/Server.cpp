#include "Server.h"
#include "DxLib.h"
#include "const.h"

Server::Server( ) {
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
	if ( _handle_udp < 0 ) {
		_handle_udp = MakeUDPSocket( -1 );
	}

	_send_data_udp = NetWorkData( );
	memset( _recv_data_udp, 0, sizeof( NetWorkData ) * MACHINE_MAX );

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

void Server::sendDataTcp( bool matching ) {
	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		NetWorkSend( _handles[ i ], &matching, sizeof( bool ) ); 
	}
}

void Server::sendDataUdp( ) {
	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		if ( _handles[ i ] < 0 ) {
			continue;
		}
		IPDATA ip;
		GetNetWorkIP( _handles[ i ], &ip );
		NetWorkSendUDP( _handle_udp, ip, UDP_PORT, &_send_data_udp, sizeof( NetWorkData ) ); 
	}
}
void Server::recvTcp( int idx ) {
	int size = GetNetWorkDataLength( _handles[ idx ] );
	if ( size < 0 ) {
		_recving[ idx ] = false;
		return;
	}

	int recv = NetWorkRecv( _handles[ idx ], &_recv_data_udp[ idx ], size );
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
	fclose( fp );
}

bool Server::isConnecting( int idx ) const {
	return _handles[ idx ] != -1;
}

bool Server::isRecving( int idx ) const {
	return _recving[ idx ];
}

std::string Server::getMachineIpStr( int idx ) {
	std::string ip = "";
	if ( _handles[ idx ] > 0 ) {
		IPDATA tmp;
		GetNetWorkIP( _handles[ idx ], &tmp );
		ip = std::to_string( tmp.d1 );
		ip += ".";
		ip += std::to_string( tmp.d2 );
		ip += ".";
		ip += std::to_string( tmp.d3 );
		ip += ".";
		ip += std::to_string( tmp.d4 );
	}

	return ip;
}

void Server::disConnect( ) {
	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		if ( _handles[ i ] != -1 ) {
			if ( GetNetWorkDataLength( _handles[ i ] ) > 0 ) {
				NetWorkRecvBufferClear( _handles[ i ] );
			}
		}
		CloseNetWork( _handles[ i ] );
	}

	NetWorkRecvBufferClear( _handle_udp );
	DeleteUDPSocket( _handle_udp );

	StopListenNetWork( );
}

void Server::setOrder( int order ) {
	_send_data_udp.order = ( unsigned char )order;
}

void Server::setX( int x ) {
	_send_data_udp.x = ( unsigned char )x;
}

void Server::setY( int y ) {
	_send_data_udp.y = ( unsigned char )y;
}

void Server::setAngle( MIRROR_ANGLE angle ) {
	_send_data_udp.angle = ( unsigned char )angle;
}

int Server::setOrder( int idx ) const {
	return ( int )_recv_data_udp[ idx ].order;
}

int Server::setX( int idx ) const {
	return ( int )_recv_data_udp[ idx ].x;
}

int Server::setY( int idx ) const {
	return ( int )_recv_data_udp[ idx ].y;
}

MIRROR_ANGLE Server::setAngle( int idx ) const {
	return ( MIRROR_ANGLE )_recv_data_udp[ idx ].angle;
}