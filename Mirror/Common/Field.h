#pragma once
#include "smart_ptr.h"
#include "Base.h"
#include <string>
#include <array>

PTR( Field );
PTR( GlobalData );
PTR( Drawer );

const int MIRROR_MAX = 6;

class Field : public Base {
public:
	struct Vector { 
		double x;
		double y;
		Vector getReflection( Vector vec ) {
			// R = F - 2dot( F, N )N

			//進行ベクトル
			Vector F = { x, y };

			//法線ベクトル
			double length = sqrt( vec.x * vec.x + vec.y * vec.y );
			Vector N = { vec.x / length, vec.y / length };

			double dot = N.x * F.x + N.y * F.y;

			Vector P = Vector( );
			P.x = dot * N.x;
			P.y = dot * N.y;

			Vector ref = Vector( );
			ref.x = F.x - P.x * 2;
			ref.y = F.y - P.y * 2;

			int tmp_a = ( int )( ref.x * 10 );
			int tmp_b = ( int )( ref.y * 10 );

			ref.x = tmp_a * 0.1;
			ref.y = tmp_b * 0.1;

			return ref;
		};
	};

	enum MIRROR_ANGLE {
		LEFT,
		RIGHT
	};

	struct Mirror {
		int flag;
		double x;
		double y;
		MIRROR_ANGLE angle;
		//法線ベクトル
		Vector normal;
	};
public:
	Field( GlobalDataPtr data );
	virtual ~Field( );

public:
	std::string getTag( );
	void initialize( );
	void update( );

public:
	void setDirLazerVector( Vector vec );
	void setLazerPoint( );
	void setMirrorPoint( );

public:
	Vector getLazerPoint( ) const;
	Vector getLazerVector( ) const;
	Vector getNormalVector( double x, double y ) const;
	bool isHitMirror( ) const;
	int getHitMirrorIdx( ) const;

private:
	std::array< Mirror, MIRROR_MAX > _mirrors;
	int _hit_mirror_num;
	Vector _dir_vec;

	GlobalDataPtr _data;
	DrawerPtr _drawer;
};