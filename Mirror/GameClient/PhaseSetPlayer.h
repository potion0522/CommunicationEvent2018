#pragma once
#include "Phase.h"
#include "smart_ptr.h"

PTR( PhaseSetPlayer );
PTR( GlobalData );
PTR( Field );
PTR( Cutin );

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

	GlobalDataPtr _data;
	FieldPtr _field;
	CutinPtr _cutin;
};