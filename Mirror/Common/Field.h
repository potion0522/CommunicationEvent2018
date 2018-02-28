#pragma once
#include "smart_ptr.h"
#include "Base.h"
#include "const.h"
#include "Drawer.h"
#include <math.h>
#include <string>
#include <array>
#include <map>
#include <Windows.h>

PTR( Field );
PTR( GlobalData );
PTR( Drawer );
PTR( Image );
PTR( Debug );

const short int PLAYER_POSITION = 4;
const short int SQUARE_SIZE = ( short int )( 96 * FIELD_SIZE_RATE );
const short int FIELD_ROW = 4;
const short int FIELD_COL = 4;
const int START_POS_X = WIDTH / 3 * 2 - SQUARE_SIZE * FIELD_COL / 2;
const int START_POS_Y = HEIGHT / 2 - SQUARE_SIZE * FIELD_ROW / 2 + SQUARE_SIZE;
const int INFO_TEXT_MAX = 10;
const int BATTLE_BUTTON_IMAGE_NUM = 4;

class Field : public Base {
#pragma pack( 1 )
private:
	static const short int LAZER_TYPE_MAX = 2;

public:
	struct Vector { 
		double x;
		double y;
		Vector normalize( ) {
			Vector vec = Vector( );
			vec.x = x;
			vec.y = y;

			double length = sqrt( vec.x * vec.x + vec.y * vec.y );

			Vector normal = Vector( );
			normal.x = vec.x / length;
			normal.y = vec.y / length;

			//四捨五入
			int tmp = ( int )( normal.x * 10 ) % 10;
			if ( tmp > 4 ) {
				normal.x += 1;
			}
			normal.x = ( int )normal.x;

			tmp = ( int )( normal.y * 10 ) % 10;
			if ( tmp > 4 ) {
				normal.y += 1;
			}
			normal.y = ( int )normal.y;

			return normal;
		}
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
		MIRROR_ANGLE angle = ANGLE_NONE;
	};

	struct PlayerPos {
		float x;
		float y;
	};

	struct Item {
		bool flag;
		short int type;
		float x;
		float y;
		short int used_turn;
	};

	enum COMMAND {
		COMMAND_LEFT,
		COMMAND_RIGHT,
		COMMAND_DELETE,
		COMMAND_TYPE_MAX,
		COMMAND_NONE
	};
#pragma pack( )

public:
	Field( GlobalDataPtr data );
	virtual ~Field( );

public:
	std::string getTag( );
	void initialize( );
	void nextTurn( );
	void update( );

public:
	void setInfoText( std::string str, COLOR col = YELLOW, Drawer::FONTSIZE_TYPE size = Drawer::NORMAL );
	void updateLazerVector( Vector vec, double spd );
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
	void activeButtonLighting( );
	void deadCount( );

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
	int getHitMirrorCommandIdx( ) const;
	int getDeadCount( ) const;
	COMMAND getMirrorCommand( ) const;
	MIRROR_ANGLE getMirrorAngle( int idx ) const;
	bool isSelectedPlayer( ) const;
	bool isHitDecisionButton( ) const;
	bool isSelectedMirror( ) const;

private:
	void resetInfo( );
	void setDirect( Vector vec );
	void checkHitMirrorCommand( );
	void checkItemRecovery( );

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
	void drawBackGround( ) const;
	void drawMirrorCommand( ) const;
	void drawDeathCount( ) const;

private:
#pragma pack( 1 )
	struct BoxObject {
		LightImageProperty image;
		BoxCollider collider;
		short int half_width;
		short int half_height;
	};
	
	struct Info {
		std::string str;
		COLOR col;
		Drawer::FONTSIZE_TYPE size;
	};

	std::map< int, Mirror > _mirrors;
	std::array< Info, INFO_TEXT_MAX > _info;
	std::array< int, PLAYER_NUM > _player_pos_no;
	std::array< PlayerPos, PLAYER_POSITION * 2 > _select_player_pos;
	std::array< COLOR, PLAYER_NUM >_player_color;
	std::array< Item, ITEM_POSSESSION_MAX > _item;
	bool _player_selected;
	bool _mirror_selected;
	bool _reflection;
	bool _button_lighting;
	short int _turn;
	short int _order;
	short int _info_idx;
	short int _tmp_player_pos;
	short int _dead_flag;
	short int _player_num;
	short int _player_pos_hit_num;
	short int _field_pos_hit_num;
	short int _hit_mirror_num;
	short int _lazer_point_idx;
	short int _select_item;
	short int _dead_count;
	HCURSOR _cur_hand;
	BATTLE_PHASE _phase;
	DIR _direct;
	Mirror _tmp_mirror;
	Vector _reflection_point;
	COMMAND _command;//鏡の処理ボタン

	//画像関連
	int _table_handle;
	std::array< int, PLAYER_NUM > _mirror_handle;
	std::array< int, BATTLE_BUTTON_IMAGE_NUM > _button_handle;
	std::array< int, ITEM_MAX > _item_handle;
	std::array< int, LAZER_TYPE_MAX > _lazer_handle;
	std::array< BoxObject, COMMAND_TYPE_MAX > _mirror_cmd;
	LightImageProperty _button_image;
	LightImageProperty _background;
	LightImageProperty _lazer_image;
	BoxObject _board;
#pragma pack( )

	GlobalDataPtr _data;
	DrawerPtr _drawer;
	ImagePtr _image;
	DebugPtr _debug;
};