#pragma once
#include "Base.h"
#include "smart_ptr.h"
#include "DxLib.h"
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
	void update( );
	void initialize( );

public:
	bool isRecving( ) const;
	void sendTcp( NetWorkData send_data );
	std::string getSeverIP( ) const;
	std::string getPhase( ) const;
	NetWorkData getData( ) const;

private:
	void readIP( );
	void connect( );
	void recving( );
	void recvTcp( );
	void lost( );

private:
	enum PHASE {
		READY,
		CONNECTING,
	};
	PHASE _phase;

	int _handle;
	bool _recving;
	NetWorkData _recv_data;
	IPDATA _ip;
};

