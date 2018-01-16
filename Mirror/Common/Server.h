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
	void sendDataUdp( );
	void disConnect( );

public:
	void setOrder( int order );
	void setX( int x );
	void setY( int y );
	void setAngle( MIRROR_ANGLE angle );

public:
	int setOrder( int idx ) const;
	int setX( int idx ) const;
	int setY( int idx ) const;
	MIRROR_ANGLE setAngle( int idx ) const;

private:
	void accept( );
	void lost( );
	void createIP( );
	void recvTcp( int idx );

private:
	NetWorkData _send_data_udp;
	NetWorkData _recv_data_udp[ MACHINE_MAX ];
	bool _connecting;
	bool _recving[ MACHINE_MAX ];
	int _handles[ MACHINE_MAX ];
	int _handle_udp;
};

