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
	enum CUTIN {
		SET_PLAYER_CUTIN,
		SET_MIRROR_CUTIN,
		ATTACK_CUTIN,
		CUTIN_MAX
	};

	enum CUTIN_BACK_COLOR {
		CUTIN_BACK_NORMAL,
		CUTIN_BACK_PLAYER,
		CUTIN_BACK_ITEM,
		CUTIN_BACK_MAX
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
	void selectPlayerPos( bool *select );
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
	bool _cutin;
	bool _change_phase;
	bool _use_item;
	bool _item_callback;
	bool _double_mirror;
	bool _reverse_mirror;
	int  _turn;
	int _order;
	int  _player_num;
	float _cutin_spd_rate;
	int _item;
	int _clicking;
	BATTLE_PHASE _phase;
	Field::Mirror _tmp_mirror;
	std::array< int, CUTIN_MAX + PLAYER_NUM + ITEM_MAX > _cutin_png;
	std::array< int, CUTIN_BACK_MAX > _background_cutin_png;
	ImageProperty _background_cutin_image;
	ImageProperty _cutin_image;

	GlobalDataPtr _data;
	ClientPtr _client;
	DrawerPtr _drawer;
	FieldPtr _field;
	LazerPtr _lazer;
};

