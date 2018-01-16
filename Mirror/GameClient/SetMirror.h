#pragma once
#include "Base.h"
#include "smart_ptr.h"
#include "const.h"
#include <string>

PTR( SetMirror );
PTR( GlobalData );
PTR( Client );

class SetMirror : public Base {
public:
	SetMirror( GlobalDataPtr data );
	virtual ~SetMirror( );

public:
	std::string getTag( );
	void initialize( );
	void update( );

private:
	void setTurn( );
	void attackTurn( );

private:
	int _player_num;
	BATTLE_PHASE _phase;

	GlobalDataPtr _data;
	ClientPtr _client;
};

