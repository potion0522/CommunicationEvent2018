#pragma once
#include "Base.h"
#include "smart_ptr.h"
#include <string>

PTR( ResultServer );
PTR( GlobalData );
PTR( GameMaster );
PTR( Drawer );

class ResultServer : public Base {
public:
	ResultServer( GlobalDataPtr data, GameMasterPtr game );
	virtual ~ResultServer( );

public:
	std::string getTag( );
	void initialize( );
	void update( );

private:
	int _win;
	int _cnt;

	GlobalDataPtr _data;
	GameMasterPtr _game;
	DrawerPtr _drawer;
};

