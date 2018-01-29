#pragma once
#include <string>

class Base {
public:
	Base( );
	virtual ~Base( );

	virtual void setFlag( int flag );
	virtual int getFlag( ) const;
	virtual void update( ) = 0;
	virtual void initialize( );
	virtual std::string getTag( ) = 0;

private:
	int _flag;

public:
	struct ImageProperty {
		int flag;
		double cx;	//’†S
		double cy;
		double lx;	//¶ã
		double ly;
		double rx;	//‰E‰º
		double ry;
		double r;	//‚ ‚½‚è”»’è
		double size = 1;//‰æ‘œŠg‘å—¦
		double angle;
		int knd;
		int brt = 255;
		int cnt;
		int png;
	};
};