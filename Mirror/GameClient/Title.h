#pragma once
#include "Base.h"
#include "smart_ptr.h"
#include "const.h"
#include <array>

PTR( Title );
PTR( GlobalData );
PTR( Drawer );
PTR( Soundplayer );

class Title : public Base {
private:
	enum BUTTON {
		NONE_BUTTON,
		START_BUTTON,
		ITEM_SETTING_BUTTON,
		IP_SETTING_BUTTON,
		BUTTON_MAX,
	};
public:
	Title( GlobalDataPtr data );
	virtual ~Title( );

public:
	std::string getTag( );
	void initialize( );
	void finalize( );
	void update( );

private:
	void chengeNextScene( );
	void calcButtonAction( );
	BUTTON getHitButton( ) const;
	bool isDrag( ) const;

private:
	void drawBackGround( ) const;
	void drawTitle( ) const;
	void drawStartButton( ) const;
	void drawItemSettingButton( ) const;
	void drawIpSettingButton( ) const;
	
private:

	struct BoxObject {
		BoxCollider collider;
		LightImageProperty image;
	};

	//定数
	static const short int BUTTON_TYPE_MAX = 2;//押されている・押されていない
	static const short int TITLE_BUTTON_MAX = 3;

	//変数
	short int _hit_button_idx;
	short int _past_hit_button_idx;
	bool _startbutton_clicking;
	bool _item_setbutton_clicking;
	bool _ip_setbutton_clicking;

	//画像関係
	LightImageProperty _back;
	LightImageProperty _title_logo;
	std::array< int, BUTTON_TYPE_MAX > _startbutton_handles;
	std::array< int, BUTTON_TYPE_MAX > _item_setbutton_handles;
	std::array< int, BUTTON_TYPE_MAX > _ip_setbutton_handles;
	BoxObject _startbutton;
	BoxObject _item_setbutton;
	BoxObject _ip_setbutton;

	//サウンド関係
	SoundProperty _bgm;
	SoundProperty _button_se;


	GlobalDataPtr _data;
	DrawerPtr _drawer;
	SoundplayerPtr _soundplayer;
};

