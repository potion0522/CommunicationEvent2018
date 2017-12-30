#pragma once
#include "smart_ptr.h"
#include "Base.h"
#include "const.h"
#include <list>
#include <array>

PTR( Drawer );
PTR( Color );

class Drawer : public Base {
public:
	static const int FONTHANDLE_MAX = 4;
	enum FONTSIZE_TYPE {
		SMALL,
		NORMAL,
		LITTLE_BIG,
		BIG,
	};

public:
	Drawer( );
	virtual ~Drawer( );

public:
	std::string getTag( );
	void initialize( );
	void update( );

public:
	void setImage( ImageProperty png );
	void setString( double x, double y, COLOR col, std::string str, FONTSIZE_TYPE type = NORMAL, int brt = 255 );
	int getStringW( FONTSIZE_TYPE type, std::string str ) const;
	int getStringH( FONTSIZE_TYPE type ) const;

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
		int brt;
		int handle;
	};
	int _handle_font[ FONTHANDLE_MAX ];
	std::array< int, FONTHANDLE_MAX > _size;
	std::list< StringProperty > _strings;
	std::list< ImageProperty > _images;

	ColorPtr _color;
};

