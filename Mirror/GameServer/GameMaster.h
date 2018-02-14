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

public:
	int getWinner( ) const;

private:
	void commandExecution( );
	void orderPlayer( );
	int getWaitingIdx( ) const;
	int getOrderIdx( int order ) const;
	int calcLazerPoint( int exclusion = -1 );
	void updatePlayerPhase( );
	void updateMirrorPhase( );
	void updateAttackPhase( );
	void updateJudgePhase( );
	void inputPlayerPhase( );
	void inputMirrorPhase( );
	void inputAttackPhase( );
	void checkCallBack( );
	void updateItemCalc( );
	void invocationItem( );
	void checkItemFlag( );

private:
	struct Data {
		bool flag;
		bool alive;
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
	bool _use_item;
	bool _reverse_mirror;
	int _item;
	int _item_user;
	int _winner;
	int _turn;
	BATTLE_PHASE _phase;

	GlobalDataPtr _data;
	ConnectorPtr _connector;
	LogPtr _log;
	ServerPtr _server;
	FieldPtr _field;
	CommandPtr _command;
};

