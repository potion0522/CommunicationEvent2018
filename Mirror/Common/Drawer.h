#pragma once
#include "smart_ptr.h"
#include "Base.h"
#include "const.h"
#include <list>

PTR( Drawer );
PTR( Color );

class Drawer : public Base {
public:
	Drawer( );
	virtual ~Drawer( );

public:
	std::string getTag( );
	void update( );

public:
	void setImage( ImageProperty png );
	void setString( double x, double y, COLOR col, std::string str );

private:
	void drawImage( );
	void drawString( );
	void reset( );

private:
	struct StringProperty {
		float x;
		float y;
		COLOR col;
		std::string str;
	};
	std::list< StringProperty > _strings;
	std::list< ImageProperty > _images;

	ColorPtr _color;
};

