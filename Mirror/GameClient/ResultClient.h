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
	void initialize( );
	void finalize( );
	void update( );

private:
	bool _win;
	short int _cnt;

	GlobalDataPtr _data;
	GamePtr _game;
	DrawerPtr _drawer;
};

