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
	enum FONTSIZE_TYPE {
		SMALL,
		NORMAL,
		LITTLE_BIG,
		BIG,
		SUPER_BIG,
		FONT_TYPE_MAX
	};

public:
	Drawer( );
	virtual ~Drawer( );

public:
	std::string getTag( );
	void initialize( );
	void update( );

public:
	void setBackImage( ImageProperty png );
	void setImage( ImageProperty png );
	//中央寄せにするかどうか, x座標, y座標, カラー, 文字列, フォントサイズ, アルファ値
	void setFrontString( bool flag, double x, double y, COLOR col, std::string str, FONTSIZE_TYPE type = NORMAL, int brt = 255 );
	void setBackString( bool flag, double x, double y, COLOR col, std::string str, FONTSIZE_TYPE type = NORMAL, int brt = 255 );
	void setLine( double sx, double sy, double ex, double ey, COLOR col = WHITE, int brt = 255 );
	void setCircle( double x, double y, double r, COLOR col = WHITE, int brt = 255, bool isfill = false );
	int getStringW( FONTSIZE_TYPE type, std::string str ) const;
	int getStringH( FONTSIZE_TYPE type ) const;

private:
	int getBlink( );
	void drawBackImage( );
	void drawImage( );
	void drawFrontString( );
	void drawBackString( );
	void drawLine( );
	void drawCircle( );
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
		int brt;
	};
	struct CircleProperty {
		float cx;//中心
		float cy;
		float r;//半径
		COLOR col;
		int brt;
		bool isFill;
	};
	int _handle_font[ FONT_TYPE_MAX ];
	int _blink;
	int _colcode;
	int _color_change_speed;
	ImageProperty _back_image;
	std::array< int, FONT_TYPE_MAX > _size;
	std::list< StringProperty > _front_strings;  //画像の上に表示
	std::list< StringProperty > _back_strings;  //画像のしたに表示
	std::list< ImageProperty > _images;
	std::list< LineProperty > _lines;
	std::list< CircleProperty > _circle;

	ColorPtr _color;
};

