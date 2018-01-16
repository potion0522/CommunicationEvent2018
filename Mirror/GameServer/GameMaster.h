#pragma once
#include "Base.h"
#include "smart_ptr.h"
#include "const.h"
#include <string>

PTR( GameMaster );
PTR( GlobalData );
PTR( Connector );
PTR( Log );
PTR( Server );

const int PLAYER = 2;

class GameMaster : public Base {
public:
	GameMaster( GlobalDataPtr data, ConnectorPtr connector, LogPtr log );
	virtual ~GameMaster( );

public:
	std::string getTag( );
	void initialize( );
	void update( );

private:
	void orderPlayer( );
	void setTurn( );
	void attackTurn( );

private:
	NetWorkData _client_data[ PLAYER ];

	bool _matching;
	bool _dice;
	BATTLE_PHASE _phase;

	GlobalDataPtr _data;
	ConnectorPtr _connector;
	LogPtr _log;
	ServerPtr _server;
};

