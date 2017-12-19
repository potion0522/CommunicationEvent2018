#pragma once
#include "Base.h"
#include "GlobalData.h"
#include "Debug.h"
#include <memory>
#include <map>

class Direction {
public:
	Direction( std::shared_ptr< GlobalData > data );
	virtual ~Direction( );

public:
	void initialize( );
	void update( );

private:
	void initNextProcess( );
	void add( SCENE scene, std::shared_ptr< Base > ptr );
	void run( );

private:
	bool _init;
	SCENE _scene;

	std::shared_ptr< Base > _base;
	std::shared_ptr< GlobalData > _data;
	std::shared_ptr< Debug > _debug;

	std::map< SCENE, std::shared_ptr< Base > > _exe;
};

