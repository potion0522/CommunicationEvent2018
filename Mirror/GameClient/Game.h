#pragma once
#include "Base.h"
#include "smart_ptr.h"
#include "const.h"
#include <string>
#include <array>

PTR( Game );
PTR( GlobalData );
PTR( Client );
PTR( Field );
PTR( Phase );
PTR( Cutin );
PTR( Soundplayer );

class Game : public Base {
private:
public:
	Game( GlobalDataPtr data );
	virtual ~Game( );

public:
	std::string getTag( );
	void initialize( );
	void finalize( );
	void update( );

public:
	bool isDrawn( ) const;
	bool isWin( ) const;
	CAUSE_OF_DEATH getCauseOfDeath( ) const;

private:
	void changePhase( );

private:
	bool _drawn;//ˆø‚«•ª‚¯
	bool _win;
	CAUSE_OF_DEATH _cause;
	short int _turn;
	short int _order;
	short int _player_num;
	BATTLE_PHASE _phase;
	SoundProperty _bgm;

	GlobalDataPtr _data;
	ClientPtr _client;
	FieldPtr _field;
	PhasePtr _battle;
	CutinPtr _cutin;
	SoundplayerPtr _soundplayer;
};

