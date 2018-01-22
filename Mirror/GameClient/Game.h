#pragma once
#include "Base.h"
#include "smart_ptr.h"
#include "const.h"
#include <string>

PTR( Game );
PTR( GlobalData );
PTR( Client );
PTR( Field );

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

private:
	int _player_num;
	BATTLE_PHASE _phase;

	GlobalDataPtr _data;
	ClientPtr _client;
	FieldPtr _field;
};

