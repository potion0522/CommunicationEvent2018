#pragma once
#include "Base.h"

class Phase {
public:
	Phase( short int player_num );
	virtual ~Phase( );

public:
	virtual void update( ) = 0;
	void setOrder( int order );
	void setTurn( int turn );

protected:
	virtual void recv( ) = 0;

protected:
	short int _player_num;
	short int _order;
	short int _turn;
};

