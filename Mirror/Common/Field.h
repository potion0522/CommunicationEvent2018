#pragma once
#include "smart_ptr.h"
#include "Base.h"
#include "const.h"
#include <string>
#include <array>
#include <map>
#include <Windows.h>

PTR( Field );
PTR( GlobalData );
PTR( Drawer );
PTR( Image );

const int PLAYER_POSITION = 5;
const int SQUARE_SIZE = 96;
const int FIELD_ROW = 5;
const int FIELD_COL = 5;
const int START_POS_X = WIDTH / 3 * 2 - SQUARE_SIZE * FIELD_COL / 2;
const int START_POS_Y = HEIGHT / 2 - SQUARE_SIZE * FIELD_ROW / 2;
const int INFO_TEXT_MAX = 6;
const int BUTTON_IMAGE_NUM = 4;

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
	};

	struct PlayerPos {
		float x;
		float y;
	};

	struct Info {
		std::string str;
		COLOR col;
	};

	struct Item {
		bool flag;
		float x;
		float y;
	};
public:
	Field( GlobalDataPtr data );
	virtual ~Field( );

public:
	std::string getTag( );
	void initialize( );
	void nextTurn( );
	void update( );

public:
	void setInfoText( std::string str, COLOR col = YELLOW );
	void updateLazerVector( Vector vec );
	void hitPlayerPos( );
	void hitFieldPos( );
	void setTurn( int turn );
	void setPlayerNum( int num );
	void setPhase( BATTLE_PHASE phase );
	void setPlayerPoint( int idx, int pos );
	void setLazerPoint( int pos );
	void setTmpPlayerPoint( );
	void resetTmpMirror( );
	void setTmpMirrorPoint( int player_num, int x, int y, MIRROR_ANGLE angle );
	void setMirrorPoint( int player_num, int x, int y, MIRROR_ANGLE angle );
	void deleteMirrorPoint( int idx );
	void setOrder( int order );
	void playerPosSelected( );
	void mirrorPosSelected( );
	void mirrorPosNotSelected( );
	void selectItem( int idx );
	void useItem( );
	void reverseMirror( );
	void changeClickButton( );

public:
	Vector getLazerPoint( ) const;
	Vector getNextDirect( ) const;
	Vector getReflectionPoint( ) const;
	BATTLE_PHASE getPhase( ) const;
	int getTurn( ) const;
	int getTmpPlayerPoint( ) const;
	int getPlayerPoint( int idx ) const;
	int getHitMirrorIdx( ) const;
	int getPlayerPosHitNum( ) const;
	int getFieldPosHitNum( ) const;
	int getDeadPlayer( ) const;
	int getLazerPointIdx( ) const;
	int getSelectItem( ) const;
	int getHitItemIdx( ) const;
	bool isSelectedPlayer( ) const;
	bool isHitDecisionButton( ) const;
	bool isSelectedMirror( ) const;

private:
	void resetInfo( );
	void setDirect( Vector vec );

private:
	//Draw系
	void drawField( );
	void drawArmament( ) const;
	void drawTmpMirror( ) const;
	void drawDecisionButton( ) const;
	void drawMirror( ) const;
	void drawPlayerPos( ) const;
	void drawPlayer( ) const;
	void drawInfo( ) const;
	void drawRound( ) const;
	void drawSettingPlayer( );
	void drawItem( ) const;

private:
	std::map< int, Mirror > _mirrors;
	std::array< Info, INFO_TEXT_MAX > _info;
	std::array< int, PLAYER_NUM > _player_pos_no;
	std::array< PlayerPos, PLAYER_POSITION * 2 > _select_player_pos;
	std::array< COLOR, PLAYER_NUM >_player_color;
	std::array< Item, ITEM_MAX > _item;
	int _turn;
	bool _player_selected;
	bool _mirror_selected;
	bool _reflection;
	int _order;
	int _info_idx;
	int _tmp_player_pos;
	int _dead_flag;
	int _player_num;
	int _player_pos_hit_num;
	int _field_pos_hit_num;
	int _hit_mirror_num;
	int _lazer_point_idx;
	int _select_item;
	HCURSOR _cur_hand;
	BATTLE_PHASE _phase;
	DIR _direct;
	Mirror _tmp_mirror;
	Vector _reflection_point;

	//画像関連
	std::array< int, BUTTON_IMAGE_NUM > _button_image;
	ImageProperty _button;
	ImageProperty _table;
	ImageProperty _pin;

	GlobalDataPtr _data;
	DrawerPtr _drawer;
	ImagePtr _image;
};