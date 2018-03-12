#pragma once
#include "Base.h"
#include "smart_ptr.h"
#include "const.h"
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
	std::string convResultMessage( bool win, CAUSE_OF_DEATH cause );
	std::string convWinMessage( CAUSE_OF_DEATH cause );
	std::string convLoseMessage( CAUSE_OF_DEATH cause );

private:
	bool _win;
	CAUSE_OF_DEATH _cause;
	short int _cnt;

	GlobalDataPtr _data;
	GamePtr _game;
	DrawerPtr _drawer;
};

