#pragma once
#include "Base.h"
#include "smart_ptr.h"
#include "const.h"
#include "Field.h"
#include <string>

PTR( Game );
PTR( GlobalData );
PTR( Client );
PTR( Drawer );
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

public:
	bool isWin( ) const;

private:
	void updatePlayerPhase( );
	void updateMirrorPhase( );
	void inputTmpMirror( );
	void updateAttackPhase( );
	void updateJudgePhase( );
	void recvMirrorPhase( );
	void recvAttackPhase( );
	void recvJudgePhase( );

private:
	bool _mirror_phase_recv;
	bool _attack_phase_recv;
	bool _judge_phase_recv;
	bool _turn_finish;
	bool _send_live;
	bool _win;
	int  _turn;
	int  _player_num;
	BATTLE_PHASE _phase;
	Field::Mirror _tmp_mirror;

	GlobalDataPtr _data;
	ClientPtr _client;
	DrawerPtr _drawer;
	FieldPtr _field;
	LazerPtr _lazer;
};

