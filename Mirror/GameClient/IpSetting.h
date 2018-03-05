#pragma once
#include "Base.h"
#include "smart_ptr.h"
#include <string>
#include <array>
#include <vector>
#include <Windows.h>

PTR( IpSetting );
PTR( GlobalData );
PTR( Drawer );
PTR( Soundplayer );
PTR( KeyBoard );

class IpSetting : public Base {
public:
	IpSetting( GlobalDataPtr data );
	virtual ~IpSetting( );

public:
	std::string getTag( );
	void initialize( );
	void finalize( );
	void update( );
	
private:
	void writeIp( );
	void calcButtonAction( );
	bool isDrag( ) const;
	int getHitButton( ) const;

private:
	void drawIpString( ) const;
	void drawWritingBox( ) const;
	void drawBackGround( ) const;
	void drawSaveButton( ) const;
	void drawReturnButton( ) const;

private:
	enum BUTTON {
		NONE_BUTTON,
		SAVE_BUTTON,
		RETURN_BUTTON,
		BUTTON_MAX,
	};
	struct BoxObject {
		BoxCollider collider;
		LightImageProperty image;
	};
	static const short int BUTTON_TYPE_MAX = 2;
	static const short int WRITE_KEYCODE_MAX = 21;

	bool _drag;
	bool _savebutton_clicking;
	bool _returnbutton_clicking;
	bool _save;
	short int _wait_time;
	short int _bar_rate;
	short int _bar_width_half;
	short int _bar_height_half;
	std::vector< int > _keycode;
	std::string _ip;
	HCURSOR _cur_hand;

	BoxObject _savebutton;
	BoxObject _returnbutton;

	//‰æ‘œ
	int _background_handle;
	std::array< int, BUTTON_TYPE_MAX > _savebutton_handles;
	std::array< int, BUTTON_TYPE_MAX > _returnbutton_handles;
	LightImageProperty _bar_frame;
	LightImageProperty _bar;
	
	//ƒTƒEƒ“ƒh
	SoundProperty _button_se;
	SoundProperty _save_se;
	SoundProperty _back_se;

	GlobalDataPtr _data;
	DrawerPtr _drawer;
	SoundplayerPtr _soundplayer;
	KeyBoardPtr _keyboard;
};

