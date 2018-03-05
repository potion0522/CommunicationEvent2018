#pragma once
#include "Base.h"
#include "smart_ptr.h"
#include "const.h"
#include "Drawer.h"
#include <vector>
#include <stdio.h>

PTR( Debug );
PTR( GlobalData );
PTR( Color );
PTR( Drawer );
PTR( Command );

class Debug : public Base {
public:
	Debug( GlobalDataPtr data );
	Debug( );
	virtual ~Debug( );

public:
	void update( );
	void initialize( );
	void finalize( );

public:
	std::string getTag( );
	void error( std::string err );
	void addLog( std::string add );
	void setActiveClass( std::string tag );
	void setLine( double sx, double sy, double ex, double ey, COLOR col = WHITE );

private:
	void commandExecution( );
	void drawMousePoint( );
	void printLog( );
	void printActiveClass( );
	void printScene( );
	void drawMyIp( );
	void initLog( );
	void initActiveClass( );

private:
	int _log_y;
	int _active_y;
	FILE *_fp;
	std::vector< std::string > _log;
	std::vector< std::string > _active_class;
	std::string _my_ip;

	GlobalDataPtr _data;
	ColorPtr _color;
	DrawerPtr _drawer;
	CommandPtr _command;
};

