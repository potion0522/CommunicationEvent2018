#pragma once
#include "Base.h"
#include "GlobalData.h"
#include "Debug.h"
#include "const.h"
#include <memory>
#include <map>

class Direction {
public:
	Direction( std::shared_ptr< GlobalData > data, std::shared_ptr< Debug > debug );
	virtual ~Direction( );

public:
	void initialize( );
	void run( );
	void add( SCENE scene, std::shared_ptr< Base > ptr );

private:
	void update( );
	void initNextProcess( );

private:
	SCENE _scene;

	std::shared_ptr< GlobalData > _data;
	std::shared_ptr< Debug > _debug;
	std::map< SCENE, std::shared_ptr< Base > > _exe;
};

