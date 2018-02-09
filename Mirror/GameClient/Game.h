#pragma once
#include "Base.h"
#include "smart_ptr.h"
#include "const.h"
#include "Field.h"
#include <string>
#include <array>

PTR( Game );
PTR( GlobalData );
PTR( Client );
PTR( Drawer );
PTR( Field );
PTR( Lazer );

class Game : public Base {
private:
	static const int BACKGROUND_CUTIN_MAX = 2;
	enum CUTIN {
		SET_PLAYER_CUTIN,
		SET_MIRROR_CUTIN,
		ATTACK_CUTIN,
		CUTIN_MAX
	};

	enum CUTIN_BACK_COLOR {
		NORMAL,
		PLAYER,
	};
public:
	Game( GlobalDataPtr data );
	virtual ~Game( );

public:
	std::string getTag( );
	void initialize( );
	void update( );

public:
	bool isWin( ) const;

private:
	void calcBackCutin( );
	void calcStringCutin( );
	void drawBackCutin( ) const;
	void drawStringCutin( ) const;
private:
	void resetBackCutin( );
	void resetStringCutin( );
	void selectPlayerPos( );
	void updatePlayerPhase( );
	void updateMirrorPhase( );
	void inputTmpMirror( );
	void updateAttackPhase( );
	void updateJudgePhase( );
	void recvMirrorPhase( );
	void recvAttackPhase( );
	void recvJudgePhase( );
	void updateItemCalc( );
	void invocationItem( );
	void checkItemFlag( );

private:
	bool _player_selected;
	bool _mirror_phase_recv;
	bool _attack_phase_recv;
	bool _judge_phase_recv;
	bool _turn_finish;
	bool _send_live;
	bool _win;
	bool _phase_cutin;
	bool _player_cutin;
	bool _use_item;
	bool _item_callback;
	int  _turn;
	int _order;
	int  _player_num;
	float _cutin_spd_rate;
	int _item;
	BATTLE_PHASE _phase;
	Field::Mirror _tmp_mirror;
	std::array< int, CUTIN_MAX + PLAYER_NUM > _cutin_png;
	std::array< int, BACKGROUND_CUTIN_MAX > _background_cutin_png;
	ImageProperty _background_cutin_image;
	ImageProperty _cutin_image;

	GlobalDataPtr _data;
	ClientPtr _client;
	DrawerPtr _drawer;
	FieldPtr _field;
	LazerPtr _lazer;
};

