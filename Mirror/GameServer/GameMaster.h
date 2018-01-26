#pragma once
#include "Base.h"
#include "smart_ptr.h"
#include "const.h"
#include <string>
#include <array>

PTR( GameMaster );
PTR( GlobalData );
PTR( Connector );
PTR( Log );
PTR( Server );
PTR( Field );
PTR( Command );

class GameMaster : public Base {
public:
	GameMaster( GlobalDataPtr data, ConnectorPtr connector, LogPtr log, CommandPtr command );
	virtual ~GameMaster( );

public:
	std::string getTag( );
	void initialize( );
	void update( );

private:
	void commandExecution( );
	void orderPlayer( );
	int getWaitingIdx( ) const;
	int getOrderIdx( int order ) const;
	void updatePlayerPhase( );
	void updateMirrorPhase( );
	void updateAttackPhase( );
	void updateJudgePhase( );
	void inputPlayerPhase( );
	void inputMirrorPhase( );
	void inputAttackPhase( );
	void inputJudgePhase( );

private:
	struct Data {
		bool live;
		int player_pos;
		int player_num;
		int order;
		int x;
		int y;
		MIRROR_ANGLE angle;
		bool fin;
	};
	std::array< Data, PLAYER_NUM > _client_data;

	bool _matching;
	bool _dice;
	int _winner;
	BATTLE_PHASE _phase;

	GlobalDataPtr _data;
	ConnectorPtr _connector;
	LogPtr _log;
	ServerPtr _server;
	FieldPtr _field;
	CommandPtr _command;
};

