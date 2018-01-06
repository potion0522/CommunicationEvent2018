#pragma once
#include "smart_ptr.h"

PTR( Test );
PTR( GlobalData );
PTR( Drawer );

class Test {
public:
	Test( GlobalDataPtr data );
	virtual ~Test( );

public:
	void initialize( );
	void update( );

private:
	void drawLine( );

private:
	GlobalDataPtr _data;
	DrawerPtr _drawer;
};

