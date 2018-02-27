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
	std::string getServerIpStr( );
	std::string getClientIpStr( int idx );
	void sendDataTcp( bool matching );
	void sendDataUdp( );
	void disConnect( );

public:
	void createIP( std::string ipstr );
	void setOrder( int order );
	void setPlayerPos( int idx, int pos );
	void setLazerPos( int pos );
	void setItemFlag( bool flag );
	void setItem( int item );
	void setItemUser( int user );
	void setStcFlag( int idx, bool flag );
	void setStcPlayerNum( int idx, int player_num );
	void setStcX( int idx, int x );
	void setStcY( int idx, int y );
	void setStcAngle( int idx, MIRROR_ANGLE angle );
	void setStcWinner( int winner );
	void setBattlePhase( BATTLE_PHASE phase );

public:
	int getOrder( int idx ) const;
	int getPlayerPos( int idx ) const;
	bool isItemFlag( int idx ) const;
	int getItem( int idx ) const;
	int getItemUser( int idx ) const;
	bool getCtsFlag( int idx ) const;
	int getCtsX( int idx ) const;
	int getCtsY( int idx ) const;
	MIRROR_ANGLE getCtsAngle( int idx ) const;
	BATTLE_PHASE getBattlePhase( int idx ) const;
	bool getFinish( int idx ) const;
	bool getAlive( int idx ) const;

private:
	void accept( );
	void lost( );
	void createIP( );
	void recvTcp( int idx );

private:
	NetWorkData _send_data_udp;
	NetWorkData _recv_data_tcp[ MACHINE_MAX ];
	bool _connecting;
	bool _recving[ MACHINE_MAX ];
	int _handles[ MACHINE_MAX ];
	int _handle_udp;
};

