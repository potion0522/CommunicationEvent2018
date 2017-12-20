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
	void sendData( int idx, Client::NetWorkData send_data );

private:
	void accept( );
	void lost( );
	void createIP( );

private:
	Client::NetWorkData recv_data[ MACHINE_MAX ];
	bool _connecting;
	int _handles[ MACHINE_MAX ];
};

