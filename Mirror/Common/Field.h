#pragma once
#include "smart_ptr.h"
#include "Base.h"
#include "const.h"
#include <string>
#include <array>
#include <Windows.h>

PTR( Field );
PTR( GlobalData );
PTR( Drawer );

const int PLAYER_POSITION = 5;

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

	struct PlayerPos {
		double x;
		double y;
	};
public:
	Field( GlobalDataPtr data );
	virtual ~Field( );

public:
	std::string getTag( );
	void initialize( );
	void update( );

public:
	void setPlayerNum( int num );
	void setLazerVector( Vector vec );
	void setDirect( Vector vec );
	void setPhase( BATTLE_PHASE phase );
	void setPlayerPoint( int idx, int pos );
	void setLazerPoint( int pos );
	void setMirrorPoint( int player_num, int x, int y, MIRROR_ANGLE angle );
	void playerPosSelected( );

public:
	Vector getLazerPoint( ) const;
	Vector getLazerVector( ) const;
	Vector getNormalVector( double x, double y ) const;
	BATTLE_PHASE getPhase( ) const;
	int getPlayerPoint( int idx ) const;
	int getDistance( ) const;
	int getHitMirrorIdx( ) const;
	int getPlayerPosHitNum( ) const;
	bool isMirror( ) const;
	bool isHitPlayerPos( );
	bool isSelectedPlayer( ) const;

private:
	int getEmptyMirrorsIdx( ) const;
	//Draw系
	void drawField( ) const;
	void drawArmament( ) const;
	void drawMirror( ) const;
	void drawPlayerPos( ) const;
	void drawPlayer( ) const;

private:
	std::array< Mirror, MIRROR_MAX > _mirrors;
	std::array< int, 2 > _dir_board;	//x, y
	std::array< int, PLAYER_NUM > _player_pos_no;
	std::array< PlayerPos, PLAYER_POSITION * 2 > _select_player_pos;
	std::array< COLOR, PLAYER_NUM >_player_color;
	int _player_num;
	int _player_pos_hit_num;
	int _lazer_pos;
	int _hit_mirror_num;
	int _distance;
	HCURSOR _cur_hand;
	bool _selected;
	BATTLE_PHASE _phase;
	DIR _direct;
	Vector _dir_vec;

	GlobalDataPtr _data;
	DrawerPtr _drawer;
};