#pragma once
#include "Base.h"
#include "const.h"
#include <memory>
#include <map>

class Direction {
public:
	Direction( std::shared_ptr< class GlobalData > data, std::shared_ptr< class Debug > debug );
	virtual ~Direction( );

public:
	void initialize( );
	void run( );
	void add( SCENE scene, std::shared_ptr< class Base > ptr );

private:
	void update( );
	void initNextProcess( );

private:
	SCENE _scene;

	std::shared_ptr< class GlobalData > _data;
	std::shared_ptr< class Debug > _debug;
	std::map< SCENE, std::shared_ptr< class Base > > _exe;
};

