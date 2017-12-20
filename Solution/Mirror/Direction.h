#pragma once
#include "smart_ptr.h"
#include "const.h"
#include <map>

PTR( Direction );
PTR( GlobalData );
PTR( Debug );
PTR( Base );
PTR( Title );

class Direction {
public:
	Direction( GlobalDataPtr data );
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
	TitlePtr _title;
	std::map< SCENE, BasePtr > _exe;
};

