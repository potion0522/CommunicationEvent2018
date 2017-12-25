#pragma once
#include "smart_ptr.h"
#include "const.h"
#include <map>
#include <vector>

PTR( Drawer );
PTR( BaseDraw );
PTR( GlobalData );
PTR( DrawTitle );

class Drawer {
public:
	Drawer( GlobalDataPtr data );
	virtual ~Drawer( );

public:
	void initialize( );
	void update( );

public:
	void set( SCENE scene, BaseDrawPtr ptr ); 

private:
	std::map< SCENE, std::vector< BaseDrawPtr > > _drawer;
	GlobalDataPtr _data;
	DrawTitlePtr _title;
};

