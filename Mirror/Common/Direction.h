#pragma once
#include "smart_ptr.h"
#include "const.h"
#include <map>
#include <vector>

PTR( Direction );
PTR( GlobalData );
PTR( Debug );
PTR( Base );

class Direction {
public:
	Direction( MACHINE_TYPE type, GlobalDataPtr data );
	virtual ~Direction( );

public:
	void initialize( );
	void run( );
	void add( SCENE scene, BasePtr ptr );

private:
	void update( );
	void initNextProcess( );

private:
	SCENE _scene;

	GlobalDataPtr _data;
	DebugPtr _debug;
	std::map< SCENE, std::vector< BasePtr > > _exe;
};

