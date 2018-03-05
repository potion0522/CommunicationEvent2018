#pragma once
#include "Phase.h"
#include "smart_ptr.h"
#include "Base.h"

PTR( PhaseSetPlayer );
PTR( GlobalData );
PTR( Field );
PTR( Cutin );
PTR( Soundplayer );

class PhaseSetPlayer : public Phase {
public:
	PhaseSetPlayer( GlobalDataPtr data, FieldPtr field, CutinPtr cutin, short int player_num );
	virtual ~PhaseSetPlayer( );

public:
	void update( );
	void recv( ) { };

private:
	void selectPlayerPos( bool *select );

private:
	bool _selected;
	int _clicking;

	Base::SoundProperty _setplayer_se;
	Base::SoundProperty _button_se;
	
	GlobalDataPtr _data;
	FieldPtr _field;
	CutinPtr _cutin;
	SoundplayerPtr _soundplayer;
};