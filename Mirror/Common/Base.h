#pragma once
#include <string>

class Base {
public:
	Base( );
	virtual ~Base( );

	virtual void setFlag( int flag );
	virtual int getFlag( ) const;
	virtual void setSoundFlag( int flag );
	virtual int getSoundFlag( ) const;
	virtual void update( ) = 0;
	virtual void initialize( ) = 0;
	virtual void finalize( ) = 0;
	virtual std::string getTag( ) = 0;

private:
	int _flag;
	int _soundflag;

public:
#pragma pack( 1 )
	struct ImageProperty {
		int flag;
		bool bright_flag = false;
		double cx;	//中心
		double cy;
		double lx;	//左上
		double ly;
		double rx;	//右下
		double ry;
		double r;	//あたり判定
		double size = 1.0;//画像拡大率
		double angle;
		short int knd;
		short int brt[ 3 ] = { 255, 255, 255 };	//RGB輝度
		short int alpha = 255; //透明度
		int cnt;
		int png = -1;
	};
	struct LightImageProperty {
		double cx;
		double cy;
		int png = -1;
	};
	struct BoxCollider {
		float lx;
		float ly;
		float rx;
		float ry;
	};
	struct SoundProperty {
		int soundflag;
		bool loop;
		bool top;
		int volume = -1;
		int wav = -1;
	};
#pragma pack( )
};