#pragma once
#include "Base.h"
#include "smart_ptr.h"
#include "const.h"
#include <string>

PTR( Game );
PTR( GlobalData );
PTR( Client );
PTR( Field );
PTR( Lazer );

class Game : public Base {
public:
	Game( GlobalDataPtr data );
	virtual ~Game( );

public:
	std::string getTag( );
	void initialize( );
	void update( );

private:
	void updatePlayerPhase( );
	void updateMirrorPhase( );
	void updateAttackPhase( );
	void updateLazerClass( );
	void updateJudgePhase( );

private:
	bool _turn_finish;
	bool _send_live;
	int _turn;
	int _player_num;
	BATTLE_PHASE _phase;

	GlobalDataPtr _data;
	ClientPtr _client;
	FieldPtr _field;
	LazerPtr _lazer;
};

