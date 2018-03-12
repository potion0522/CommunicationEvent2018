#pragma once
#include "Phase.h"
#include "smart_ptr.h"
#include "Base.h"
#include "const.h"

PTR( PhaseJudge );
PTR( GlobalData );
PTR( Client );
PTR( Field );
PTR( Soundplayer );

class PhaseJudge : public Phase {
public:
	PhaseJudge( GlobalDataPtr data, FieldPtr field, int player_num );
	virtual ~PhaseJudge( );

public:
	void update( );
	void recv( );

public:
	bool isWin( ) const;
	CAUSE_OF_DEATH getCauseOfDeath( ) const;

private:
	bool _recv;
	bool _turn_fin;
	bool _win;
	CAUSE_OF_DEATH _cause;
	
	Base::SoundProperty _win_se;
	Base::SoundProperty _lose_se;

	GlobalDataPtr _data;
	ClientPtr _client;
	FieldPtr _field;
	SoundplayerPtr _soundplayer;
};

