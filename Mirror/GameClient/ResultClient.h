#pragma once
#include "Base.h"
#include "smart_ptr.h"
#include <string>

PTR( ResultClient );
PTR( GlobalData );
PTR( Game );
PTR( Drawer );

class ResultClient : public Base {
public:
	ResultClient( GlobalDataPtr data, GamePtr game );
	virtual ~ResultClient( );

public:
	std::string getTag( );
	void update( );
	void initialize( );

private:
	bool _win;

	GlobalDataPtr _data;
	GamePtr _game;
	DrawerPtr _drawer;
};

