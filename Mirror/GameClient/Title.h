#pragma once
#include "Base.h"
#include "smart_ptr.h"

PTR( Title );
PTR( GlobalData );
PTR( Drawer );

class Title : public Base {
public:
	Title( GlobalDataPtr data );
	virtual ~Title( );

public:
	std::string getTag( );
	void update( );
	void initialize( );

private:
	ImageProperty _back;

	GlobalDataPtr _data;
	DrawerPtr _drawer;
};

