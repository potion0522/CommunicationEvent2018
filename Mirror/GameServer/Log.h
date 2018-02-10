#pragma once
#include "Base.h"
#include "smart_ptr.h"
#include <array>

PTR( GlobalData );
PTR( Drawer );

const int SERVER_CONSOLE_LOG_MAX = 8;

class Log {
public:
	Log( GlobalDataPtr data );
	virtual ~Log( );

public:
	void initialize( );
	void update( );

public:
	void add( std::string log );
	void reset( );

private:
	void isLogPring( );

private:
	std::array< std::string, SERVER_CONSOLE_LOG_MAX > _log_time;
	std::array< std::string, SERVER_CONSOLE_LOG_MAX > _log;

	GlobalDataPtr _data;
	DrawerPtr _drawer;
};

