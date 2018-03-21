#pragma once
#include "smart_ptr.h"
#include "Base.h"
#include <vector>
#include <map>
#include <array>

PTR( Cutin );
PTR( GlobalData );
PTR( Drawer );
PTR( Soundplayer );

class Cutin {
public:
	enum CUTIN_TYPE {
		CUTIN_TYPE_PHASE,
		CUTIN_TYPE_PLAYER,
		CUTIN_TYPE_ITEM,
		CUTIN_TYPE_MAX
	};
	enum CUTIN_PHASE {
		CUTIN_PHASE_SET_PLAYER,
		CUTIN_PHASE_SET_MIRROR,
		CUTIN_PHASE_ATTACK,
		CUTIN_PHASE_MAX
	};

	enum CUTIN_PLAYER {
		CUTIN_PLAYER_ENEMY_TURN,
		CUTIN_PLAYER_YOUR_TURN,
		CUTIN_PLAYER_MAX
	};

	enum CUTIN_ITEM {
		CUTIN_ITEM_LAZER_RESET,
		CUTIN_ITEM_DOUBLE_MIRROR,
		CUTIN_ITEM_REVERSE_MIRROR,
		CUTIN_ITEM_MAX
	};

public:
	Cutin( GlobalDataPtr data );
	virtual ~Cutin( );

public:
	void initialize( );
	void update( );
	void setImage( CUTIN_TYPE type, int idx, bool player_turn = false, bool your_turn = false );

public:
	CUTIN_TYPE getCutinType( ) const;
	bool isCutin( ) const;

private:
	void calc( );
	void calcBack( );
	void calcString( );
	void playSound( );
	void reset( );

private:
	void draw( );
	void drawBack( );
	void drawString( ) const;
	void drawMessage( ) const;

private:
	bool _flag;
	bool _player_turn;
	bool _your_turn;
	bool _play_se;
	bool _click;
	CUTIN_TYPE _type;
	int _cutin_half_width;
	int _cutin_half_height;

	//‰æ‘œŠÖŒW
	std::map< CUTIN_TYPE, std::vector< int > > _cutin_handles;
	std::array< int, CUTIN_TYPE_MAX > _back_handles;
	std::array< Base::SoundProperty, CUTIN_TYPE_MAX > _se;

	Base::ImageProperty _cutin_back;
	Base::ImageProperty _cutin_string;

	GlobalDataPtr _data;
	DrawerPtr _drawer;
	SoundplayerPtr _soundplayer;
};

