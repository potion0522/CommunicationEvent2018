#pragma once
#include "Base.h"
#include "smart_ptr.h"
#include <string>

PTR( SelectItem );
PTR( GlobalData );
PTR( Drawer );

class SelectItem : public Base {
public:
	SelectItem( GlobalDataPtr data );
	virtual ~SelectItem( );

public:
	std::string getTag( );
	void initialize( );
	void update( );

private:
	void drawBackGround( ) const;

private:
	int _back_handle;

	GlobalDataPtr _data;
	DrawerPtr _drawer;
};

