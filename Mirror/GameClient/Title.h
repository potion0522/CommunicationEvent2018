#pragma once
#include "Base.h"
#include "smart_ptr.h"
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
	void checkHitButton( );
	void chengeNextScene( );

private:
	void drawBackGround( ) const;
	void drawTitle( ) const;
	void drawButton( ) const;

private:
	enum BUTTON_TYPE {
		START_BUTTON,
		SETTING_BUTTON,
		BUTTON_TYPE_MAX,
	};

	struct BoxObject {
		BoxCollider collider;
		LightImageProperty image;
	};
	//ïœêî
	short int _hit_button_idx;
	short int _past_hit_button_idx;

	//âÊëúä÷åW
	LightImageProperty _back;
	LightImageProperty _title_logo;
	std::array< BoxObject, BUTTON_TYPE_MAX > _button;
	std::array< int, BUTTON_TYPE_MAX * 2 > _button_handles;

	GlobalDataPtr _data;
	DrawerPtr _drawer;
};

