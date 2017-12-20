#pragma once
#include "Base.h"
#include "smart_ptr.h"
#include "Client.h"

PTR( Server );

const int MACHINE_MAX = 2;

class Server : public Base {
public:
	Server( );
	virtual ~Server( );

public:
	void update( );
	void initialize( );

public:
	bool isConnecting( int idx ) const;
	bool isRecving( int idx ) const;
	void sendData( int idx, Client::NetWorkData send_data );
	Client::NetWorkData getData( int idx ) const;

private:
	void accept( );
	void lost( );
	void createIP( );
	void recvTcp( int idx );

private:
	Client::NetWorkData _recv_data[ MACHINE_MAX ];
	bool _connecting;
	bool _recving[ MACHINE_MAX ];
	int _handles[ MACHINE_MAX ];
};

