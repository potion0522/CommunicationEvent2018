#pragma once
#include "Phase.h"
#include "smart_ptr.h"

PTR( PhaseJudge );
PTR( GlobalData );
PTR( Client );
PTR( Field );

class PhaseJudge : public Phase {
public:
	PhaseJudge( GlobalDataPtr data, FieldPtr field, int player_num );
	virtual ~PhaseJudge( );

public:
	void update( );
	void recv( );

public:
	bool isWin( ) const;

private:
	bool _recv;
	bool _turn_fin;
	bool _win;

	GlobalDataPtr _data;
	ClientPtr _client;
	FieldPtr _field;
};

