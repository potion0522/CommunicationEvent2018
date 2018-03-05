#include "Server.h"
#include "DxLib.h"
#include "const.h"

Server::Server( ) {
}

Server::~Server( ) {
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
	_handle_udp = -1;

	_send_data_udp = NetWorkData( );
	_send_data_udp.player_pos[ 0 ] = -1;
	_send_data_udp.player_pos[ 1 ] = -1;
	_send_data_udp.lazer_pos = -1;
	memset( _recv_data_tcp, 0, sizeof( NetWorkData ) * MACHINE_MAX );

	createIP( );
	PreparationListenNetWork( TCP_PORT );
}

void Server::finalize( ) {
	disConnect( );
}

void Server::update( ) {
	accept( );
	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		if ( isConnecting( i ) ) {
			recvTcp( i );
		}
	}
	lost( );
	NetWorkRecvBufferClear( _handle_udp );
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
		int size = sizeof( bool ) + sizeof( int );
		char *buf;
		buf = ( char* )malloc( size );
		bool *match = ( bool* )buf;
		int	 *order = ( int * )( buf + sizeof( bool ) );

		*match = matching;
		*order = i;

		NetWorkSend( _handles[ i ], buf, size );

		free( buf );
	}
}

void Server::sendDataUdp( ) {
	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		if ( _handles[ i ] < 0 ) {
			continue;
		}
		if ( _handle_udp < 0 ) {
			_handle_udp = MakeUDPSocket( -1 );
		}

		IPDATA ip;
		GetNetWorkIP( _handles[ i ], &ip );
		NetWorkSendUDP( _handle_udp, ip, UDP_PORT, &_send_data_udp, sizeof( NetWorkData ) ); 
	}
}

void Server::recvTcp( int idx ) {
	int size = GetNetWorkDataLength( _handles[ idx ] );
	if ( size < 1 ) {
		_recving[ idx ] = false;
		return;
	}

	int recv = NetWorkRecv( _handles[ idx ], &_recv_data_tcp[ idx ], size );
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

void Server::createIP( std::string ipstr ) {
	IPDATA ip;

	short int pos = ( short int )ipstr.find_first_of( "." );
	short int ipint = atoi( ipstr.substr( 0, pos ).c_str( ) );
	ip.d1 = ( unsigned char )ipint;

	ipstr = ipstr.substr(  pos + 1, ipstr.length( ) - pos );
	pos = ( short int )ipstr.find_first_of( "." );
	ipint = atoi( ipstr.substr( 0, pos ).c_str( ) );
	ip.d2 = ( unsigned char )ipint;

	ipstr = ipstr.substr(  pos + 1, ipstr.length( ) - pos );
	pos = ( short int )ipstr.find_first_of( "." );
	ipint = atoi( ipstr.substr( 0, pos ).c_str( ) );
	ip.d3 = ( unsigned char )ipint;

	ipstr = ipstr.substr(  pos + 1, ipstr.length( ) - pos );
	ipint = atoi( ipstr.c_str( ) );
	ip.d4 = ( unsigned char )ipint;
	
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

std::string Server::getServerIpStr( ) {
	IPDATA ip;
	GetMyIPAddress( &ip );

	std::string str = "";
	str += std::to_string( ip.d1 );
	str += ".";
	str += std::to_string( ip.d2 );
	str += ".";
	str += std::to_string( ip.d3 );
	str += ".";
	str += std::to_string( ip.d4 );

	return str;
}

std::string Server::getClientIpStr( int idx ) {
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

void Server::setOrder( int order ) {
	_send_data_udp.order = ( unsigned char )order;
}


void Server::setPlayerPos( int idx, int pos ) {
	_send_data_udp.player_pos[ idx ] = ( unsigned char )pos;
}

void Server::setLazerPos( int pos ) {
	_send_data_udp.lazer_pos = ( unsigned char )pos;
}

void Server::setItemFlag( bool flag ) {
	_send_data_udp.item_flag = flag;
}

void Server::setItem( int item ) {
	_send_data_udp.item = ( unsigned char )item;
}

void Server::setItemUser( int user ) {
	_send_data_udp.item_user = ( unsigned char )user;
}

void Server::setStcFlag( int idx, bool flag ) {
	_send_data_udp.stc[ idx ].flag = flag;
}

void Server::setStcPlayerNum( int idx, int player_num ) {
	_send_data_udp.stc[ idx ].player_num = ( unsigned char )player_num;
}

void Server::setStcX( int idx, int x ) {
	_send_data_udp.stc[ idx ].x = ( unsigned char )x;
}

void Server::setStcY( int idx, int y ) {
	_send_data_udp.stc[ idx ].y = ( unsigned char )y;
}

void Server::setStcAngle( int idx, MIRROR_ANGLE angle ) {
	_send_data_udp.stc[ idx ].angle = ( unsigned char )angle;
}

void Server::setStcWinner( int winner ) {
	_send_data_udp.winner = winner;
}

void Server::setBattlePhase( BATTLE_PHASE phase ) {
	_send_data_udp.phase = ( unsigned char )phase;
}

int Server::getOrder( int idx ) const {
	if ( _recv_data_tcp[ idx ].order == ( unsigned char )-1 ) {
		return -1;
	}
	return ( int )_recv_data_tcp[ idx ].order;
}

int Server::getPlayerPos( int idx ) const {
	return ( int )_recv_data_tcp[ idx ].player_pos[ idx ];
}

bool Server::isItemFlag( int idx ) const {
	return _recv_data_tcp[ idx ].item_flag;
}

int Server::getItem( int idx ) const {
	return ( int )_recv_data_tcp[ idx ].item;
}

int Server::getItemUser( int idx ) const {
	return ( int )_recv_data_tcp[ idx ].item_user;
}

bool Server::getCtsFlag( int idx ) const {
	return _recv_data_tcp[ idx ].cts.flag;
}

int Server::getCtsX( int idx ) const {
	return ( int )_recv_data_tcp[ idx ].cts.x;
}

int Server::getCtsY( int idx ) const {
	return ( int )_recv_data_tcp[ idx ].cts.y;
}

MIRROR_ANGLE Server::getCtsAngle( int idx ) const {
	return ( MIRROR_ANGLE )_recv_data_tcp[ idx ].cts.angle;
}

BATTLE_PHASE Server::getBattlePhase( int idx ) const {
	return ( BATTLE_PHASE )_recv_data_tcp[ idx ].phase;
}

bool Server::getFinish( int idx ) const {
	return _recv_data_tcp[ idx ].fin;
}

bool Server::getAlive( int idx ) const {
	return _recv_data_tcp[ idx ].alive;
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