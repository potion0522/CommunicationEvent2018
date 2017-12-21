#pragma once
#include "smart_ptr.h"
#include "const.h"

PTR( Color );

class Color {
public:
	Color( );
	virtual ~Color( );

public:
	void initialize( );
	int getColor( COLOR col );

private:
	void setWhite( );
	void setBlack( );
	void setRed( );
	void setBlue( );
	void setWater( );
	void setYellow( );

private:
	int _col;
};

