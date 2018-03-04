#pragma once
#include "Phase.h"
#include "smart_ptr.h"
#include "const.h"

PTR( PhaseItem );
PTR( GlobalData );
PTR( Field );
PTR( Client );

class PhaseItem : public Phase {
public:
	PhaseItem( GlobalDataPtr data, FieldPtr field, int player_num );
	virtual ~PhaseItem( );

public:
	void update( );
	void recv( );

public:
	ITEM getItem( ) const;

private:
	void invocationItem( );

private:
	bool _use_item;
	bool _item_callback;
	short int _item;
	int _clicking;

	GlobalDataPtr _data;
	FieldPtr _field;
	ClientPtr _client;
};

