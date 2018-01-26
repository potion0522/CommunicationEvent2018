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
	_connecting_tcp = false;
	_recving_tcp = false;
	_recving_udp = false;
	_matching = false;
	_player_num = 0;
	_handle_tcp = -1;
	_handle_udp = -1;
	_recv_data_udp = NetWorkData( );
	_send_data_tcp = NetWorkData( );

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
		_connecting_tcp = true;
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

	_player_num = 0;
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
	if ( _handle_udp < 0 ) {
		_handle_udp = MakeUDPSocket( UDP_PORT );
	}
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
	if ( lost == _handle_udp ) {
		NetWorkRecvBufferClear( _handle_udp );
		CloseNetWork( _handle_udp );
		_handle_udp = -1;
	}
}

void Client::clearBuffer( ) {
	NetWorkRecvBufferClear( _handle_udp );
}

bool Client::isConnectingTcp( ) const {
	return _connecting_tcp;
}

bool Client::isRecvingTcp( ) const {
	return _recving_tcp;
}

bool Client::isRecvingUdp( ) const {
	return _recving_udp;
}

void Client::sendTcp( ) {
	NetWorkSend( _handle_tcp, &_send_data_tcp, sizeof( NetWorkData ) );
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
	_send_data_tcp.order = ( unsigned char )order;
}

void Client::setPlayerPos( int pos ) {
	_send_data_tcp.player_pos[ _player_num ] = ( unsigned char )pos;
}

void Client::setCtsPlayerNum( ) {
	_send_data_tcp.cts.player_num = _player_num;
}

void Client::setCtsFlag( bool flag ) {
	_send_data_tcp.cts.flag = flag;
}

void Client::setCtsX( int x ) {
	_send_data_tcp.cts.x = ( unsigned char )x;
}

void Client::setCtsY( int y ) {
	_send_data_tcp.cts.y = ( unsigned char )y;
}

void Client::setCtsAngle( MIRROR_ANGLE angle ) {
	_send_data_tcp.cts.angle = ( unsigned char )angle;
}

void Client::setFinish( bool fin ) {
	_send_data_tcp.fin = fin;
}

void Client::setLive( bool live ) {
	_send_data_tcp.live = live;
}

int Client::getPlayerNum( ) const {
	return _player_num;
}

int Client::getLazerPoint( ) const {
	return ( int )_recv_data_udp.lazer_pos;
}

int Client::getOrder( ) const {
	return ( int )_recv_data_udp.order;
}

int Client::getPlayerPos( int idx ) const {
	return ( int )_recv_data_udp.player_pos[ idx ];
}

bool Client::getStcFlag( int idx ) const {
	return _recv_data_udp.stc[ idx ].flag;
}

int Client::getStcPlayerNum( int idx ) const {
	return ( int )_recv_data_udp.stc[ idx ].player_num;
}

int Client::getStcX( int idx ) const {
	return ( int )_recv_data_udp.stc[ idx ].x;
}

int Client::getStcY( int idx ) const {
	return ( int )_recv_data_udp.stc[ idx ].y;
}

MIRROR_ANGLE Client::getStcAngle( int idx ) const {
	return ( MIRROR_ANGLE )_recv_data_udp.stc[ idx ].angle;
}

BATTLE_PHASE Client::getBattlePhase( ) const {
	return ( BATTLE_PHASE )_recv_data_udp.phase;
}

int Client::getWinner( ) const {
	return ( int )_recv_data_udp.winner;
}