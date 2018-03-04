#pragma once
#include "Phase.h"
#include "smart_ptr.h"

PTR( PhaseAttack );
PTR( GlobalData );
PTR( Client );
PTR( Field );
PTR( Lazer );
PTR( Cutin );

class PhaseAttack : public Phase {
public:
	PhaseAttack( GlobalDataPtr data, FieldPtr field, CutinPtr cutin, short int player_num, bool reverse_flag );
	virtual ~PhaseAttack( );

public:
	void update( );

private:
	void calc( );
	void recv( );

private:
	bool _recv;
	bool _reverse_mirror;
	bool _send_live;

	GlobalDataPtr _data;
	ClientPtr _client;
	FieldPtr _field;
	LazerPtr _lazer;
	CutinPtr _cutin;
};

