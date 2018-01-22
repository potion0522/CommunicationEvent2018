#pragma once
#include "smart_ptr.h"
#include "Base.h"
#include "const.h"
#include <string>
#include <array>

PTR( Field );
PTR( GlobalData );
PTR( Drawer );


class Field : public Base {
public:
	struct Vector { 
		double x;
		double y;
		/*
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
		*/
	};

	enum DIR {
		DIR_UP,
		DIR_DOWN,
		DIR_LEFT,
		DIR_RIGHT
	};

	struct Mirror {
		bool flag;
		int player_num;
		int x;
		int y;
		MIRROR_ANGLE angle;
		//法線ベクトル
	};
public:
	Field( GlobalDataPtr data );
	virtual ~Field( );

public:
	std::string getTag( );
	void initialize( );
	void update( );

public:
	void setLazerVector( Vector vec );
	void setDirect( Vector vec );
	void setPlayerPoint( int idx, int pos );
	void setLazerPoint( );
	void GamePoint( int player_num, int x, int y, MIRROR_ANGLE angle );

public:
	Vector getLazerPoint( ) const;
	Vector getLazerVector( ) const;
	Vector getNormalVector( double x, double y ) const;
	int getPlayerPos( int idx ) const;
	int getDistance( ) const;
	int getHitMirrorIdx( ) const;
	bool isMirror( ) const;

private:
	int getEmptyMirrorsIdx( ) const;

private:
	std::array< Mirror, MIRROR_MAX > _mirrors;
	std::array< int, 2 > _dir_board;	//x, y
	std::array< int, PLAYER_NUM > _player_pos;
	int _hit_mirror_num;
	int _distance;
	DIR _direct;
	Vector _dir_vec;

	GlobalDataPtr _data;
	DrawerPtr _drawer;
};