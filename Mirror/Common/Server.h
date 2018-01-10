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
	std::string getTag( );
	void update( );
	void initialize( );

public:
	bool isConnecting( int idx ) const;
	bool isRecving( int idx ) const;
	std::string getMachineIpStr( int idx );
	void sendDataTcp( bool matching );
	void sendDataUdp( Client::NetWorkData send_data );
	Client::NetWorkData getData( int idx ) const;
	void disConnect( );

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
	int _handle_udp;
};

