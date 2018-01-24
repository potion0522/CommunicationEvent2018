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
	//中央寄せにするかどうか, x座標, y座標, カラー, 文字列, フォントサイズ, アルファ値
	void setString( bool flag, double x, double y, COLOR col, std::string str, FONTSIZE_TYPE type = NORMAL, int brt = 255 );
	void setLine( double sx, double sy, double ex, double ey, COLOR col = WHITE );
	void setCircle( double x, double y, double r, COLOR col = WHITE, int brt = 255, bool isfill = false );
	void setBlinkCircle( double x, double y, double r );
	int getStringW( FONTSIZE_TYPE type, std::string str ) const;
	int getStringH( FONTSIZE_TYPE type ) const;

private:
	int getBlink( );
	void drawImage( );
	void drawString( );
	void drawLine( );
	void drawCircle( );
	void drawBlinkCircle( );
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
	struct LineProperty {
		float sx;//start
		float sy;
		float ex;//end
		float ey;
		COLOR col;
	};
	struct CircleProperty {
		float cx;//中心
		float cy;
		float r;//半径
		COLOR col;
		int brt;
		bool isFill;
	};
	struct BlinkCircleProperty {
		float bx;//中心
		float by;
		float br;//半径
		bool isFillBlink;
	};
	int _handle_font[ FONTHANDLE_MAX ];
	int _blink;
	int _colcode;
	int _color_change_speed;
	std::array< int, FONTHANDLE_MAX > _size;
	std::list< StringProperty > _strings;
	std::list< ImageProperty > _images;
	std::list< LineProperty > _lines;
	std::list< CircleProperty > _circle;
	std::list< BlinkCircleProperty > _blinkcircle;

	ColorPtr _color;
};

