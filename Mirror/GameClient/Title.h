#pragma once
#include "Base.h"
#include "smart_ptr.h"
#include "const.h"
#include <array>

PTR( Title );
PTR( GlobalData );
PTR( Drawer );

class Title : public Base {
public:
	Title( GlobalDataPtr data );
	virtual ~Title( );

public:
	std::string getTag( );
	void initialize( );
	void update( );

private:
	void chengeNextScene( );
	void calcButtonAction( );
	int isHitButton( ) const;
	bool isDrag( ) const;

private:
	void drawBackGround( ) const;
	void drawTitle( ) const;
	void drawStartButton( ) const;
	void drawSettingButton( ) const;

private:
	enum BUTTON {
		NONE_BUTTON,
		START_BUTTON,
		SETTING_BUTTON,
		BUTTON_MAX,
	};

	struct BoxObject {
		BoxCollider collider;
		LightImageProperty image;
	};

	//íËêî
	static const int BUTTON_TYPE_MAX = 2;

	//ïœêî
	short int _hit_button_idx;
	short int _past_hit_button_idx;
	bool _startbutton_clicking;
	bool _settingbutton_clicking;

	//âÊëúä÷åW
	LightImageProperty _back;
	LightImageProperty _title_logo;
	std::array< int, BUTTON_TYPE_MAX > _startbutton_handles;
	std::array< int, BUTTON_TYPE_MAX > _settingbutton_handles;
	BoxObject _startbutton;
	BoxObject _settingbutton;

	GlobalDataPtr _data;
	DrawerPtr _drawer;
};

