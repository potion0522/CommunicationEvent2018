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
	void finalize( );

public:
	void readIP( );
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
	void setCauseOfDeath( CAUSE_OF_DEATH cause );
	void setItemFlag( bool flag );
	void setItem( int item );
	void setItemUser( );
	void setCtsPlayerNum( );
	void setCtsFlag( bool flag );
	void setCtsX( int x );
	void setCtsY( int y );
	void setCtsAngle( MIRROR_ANGLE angle );
	void setFinish( bool fin );
	void setAlive( bool alive );

public:
	std::string getClientIpStr( );
	int getPlayerNum( ) const;
	float getDeadCount( ) const;
	int getLazerPoint( ) const;
	int getOrder( ) const;
	int getPlayerPos( int idx ) const;
	CAUSE_OF_DEATH getCauseOfDeath( ) const;
	bool isItemFlag( ) const;
	int getItem( ) const;
	int getItemUser( ) const;
	bool getStcFlag( int idx ) const;
	int getStcPlayerNum( int idx ) const;
	int getStcX( int idx ) const;
	int getStcY( int idx ) const;
	MIRROR_ANGLE getStcAngle( int idx ) const;
	BATTLE_PHASE getBattlePhase( ) const;
	int getWinner( ) const;

private:
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

	short int _interval;
	int _handle_tcp;
	int _handle_udp;
	bool _connecting_tcp;
	bool _recving_tcp;
	bool _recving_udp;
	bool _matching;
	short int _player_num;
	float _dead_count;
	NetWorkData _recv_data_udp;
	NetWorkData _send_data_tcp;
	IPDATA _ip;
};

