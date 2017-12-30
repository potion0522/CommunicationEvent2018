#pragma once
#include "Base.h"
#include "smart_ptr.h"
#include "DxLib.h"
#include "const.h"
#include <string>

PTR( Client );

class Client : public Base {
public:
	struct NetWorkData {
		int test;
	};

public:
	Client( );
	virtual ~Client( );

public:
	std::string getTag( );
	void update( );
	void initialize( );

public:
	bool isRecvingTcp( ) const;
	bool isRecvingUdp( ) const;
	void sendTcp( NetWorkData send_data );
	std::string getSeverIP( ) const;
	std::string getPhase( ) const;
	SCENE getDataTcp( ) const;
	NetWorkData getDataUdp( ) const;
	void disConnect( );

private:
	void readIP( );
	void connect( );
	void recving( );
	void recvTcp( );
	void recvUdp( );
	void lost( );
	void clearBuffer( );

private:
	enum PHASE {
		READY,
		CONNECTING,
	};
	PHASE _phase;

	int _handle_tcp;
	int _handle_udp;
	bool _recving_tcp;
	bool _recving_udp;
	SCENE _recv_data_tcp;
	NetWorkData _recv_data_udp;
	IPDATA _ip;
};

