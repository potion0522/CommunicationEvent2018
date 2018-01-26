#pragma once
#include "Base.h"
#include "smart_ptr.h"
#include "DxLib.h"
#include "const.h"
#include <string>

PTR( Client );

class Client : public Base {

public:
	Client( );
	virtual ~Client( );

public:
	std::string getTag( );
	void update( );
	void initialize( );

public:
	bool isConnectingTcp( ) const;
	bool isRecvingTcp( ) const;
	bool isRecvingUdp( ) const;
	bool isMatching( ) const;
	void sendTcp( );
	std::string getSeverIP( ) const;
	std::string getPhase( ) const;
	void disConnect( );

public:
	void setOrder( int order );
	void setPlayerPos( int pos );
	void setCtsPlayerNum( );
	void setCtsFlag( bool flag );
	void setCtsX( int x );
	void setCtsY( int y );
	void setCtsAngle( MIRROR_ANGLE angle );
	void setFinish( bool fin );
	void setLive( bool live );

public:
	int getPlayerNum( ) const;
	int getLazerPoint( ) const;
	int getOrder( ) const;
	int getPlayerPos( int idx ) const;
	bool getStcFlag( int idx ) const;
	int getStcPlayerNum( int idx ) const;
	int getStcX( int idx ) const;
	int getStcY( int idx ) const;
	MIRROR_ANGLE getStcAngle( int idx ) const;
	BATTLE_PHASE getBattlePhase( ) const;
	int getWinner( ) const;

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
	bool _connecting_tcp;
	bool _recving_tcp;
	bool _recving_udp;
	bool _matching;
	int _player_num;
	NetWorkData _recv_data_udp;
	NetWorkData _send_data_tcp;
	IPDATA _ip;
};

