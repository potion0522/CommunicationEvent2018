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
	void setBackGroundImage( ImageProperty png );
	void setBackGroundImage( LightImageProperty png );
	void setBackImage( ImageProperty png );
	void setBackImage( LightImageProperty png );
	void setFrontImage( ImageProperty png );
	void setFrontImage( LightImageProperty png );
	//ベースプロパティ, 画像の中心X, 画像の中心Y, 最終的な横幅の比率, 最終的な高さの比率
	void setExtendBackImage( ImageProperty base, float image_cx, float image_cy, double extend_width, double extend_height );
	void setExtendBackImage( LightImageProperty base, float image_cx, float image_cy, double extend_width, double extend_height );
	void setExtendFrontImage( ImageProperty base, float image_cx, float image_cy, double extend_width, double extend_height );
	void setExtendFrontImage( LightImageProperty base, float image_cx, float image_cy, double extend_width, double extend_height );
	//中央寄せにするかどうか, x座標, y座標, カラー, 文字列, フォントサイズ, アルファ値
	void setFrontString( bool flag, double x, double y, COLOR col, std::string str, FONTSIZE_TYPE type = NORMAL, int brt = 255 );
	void setBackString( bool flag, double x, double y, COLOR col, std::string str, FONTSIZE_TYPE type = NORMAL, int brt = 255 );
	void setLine( double sx, double sy, double ex, double ey, COLOR col = WHITE, int brt = 255 );
	void setCircle( double x, double y, double r, COLOR col = WHITE, int brt = 255, bool isfill = false );
	void setFrontBox( double lx, double ly, double rx, double ry, COLOR col = WHITE );
	void setBackBox( double lx, double ly, double rx, double ry, COLOR col = WHITE );
	int getStringW( FONTSIZE_TYPE type, std::string str ) const;
	int getStringH( FONTSIZE_TYPE type ) const;

private:
	void drawBackGroundImage( );
	void drawBackImage( );
	void drawFrontImage( );
	void drawFrontString( );
	void drawBackString( );
	void drawLine( );
	void drawCircle( );
	void drawFrontBox( );
	void drawBackBox( );
	void reset( );

private:
	struct ExtendImageProperty {
		ImageProperty base = ImageProperty( );
		//フラグがたっていなければ通常の描画
		bool extend = false;
		float image_cx;//画像の中心X
		float image_cy;//画像の中心Y
		double extend_width;//最終的な横幅
		double extend_height;//最終的な高さ
	};

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
	struct BoxProperty {
		float lx;
		float ly;
		float rx;
		float ry;
		COLOR col;
	};
	int _handle_font[ FONT_TYPE_MAX ];
	ImageProperty _background_image;
	std::array< int, FONT_TYPE_MAX > _size;
	std::list< StringProperty > _front_strings;  //画像の上に表示
	std::list< StringProperty > _back_strings;  //画像のしたに表示
	std::list< ExtendImageProperty > _front_images;
	std::list< ExtendImageProperty > _back_images;
	std::list< LineProperty > _lines;
	std::list< CircleProperty > _circles;
	std::list< BoxProperty > _back_boxes;
	std::list< BoxProperty > _front_boxes;

	ColorPtr _color;
};

