#pragma once
#include "Base.h"
#include "smart_ptr.h"
#include "const.h"
#include <string>
#include <array>

PTR( ResultClient );
PTR( GlobalData );
PTR( Game );
PTR( Drawer );

class ResultClient : public Base {
public:
	ResultClient( GlobalDataPtr data, GamePtr game );
	virtual ~ResultClient( );

public:
	std::string getTag( );
	void initialize( );
	void finalize( );
	void update( );
	
private:
	void calcButtonAction( );
	bool isDrag( ) const;
	int getHitButton( ) const;
	
private:
	void drawReturnButton( ) const;

private:
	struct BoxObject {
		BoxCollider collider;
		LightImageProperty image;
	};

private:
	static const int BUTTON_TYPE_MAX = 2;
	
	std::string convResultMessage( bool win, CAUSE_OF_DEATH cause );
	std::string convWinMessage( CAUSE_OF_DEATH cause );
	std::string convLoseMessage( CAUSE_OF_DEATH cause );
	bool _returnbutton_clicking;
	
private:
	std::array< int, BUTTON_TYPE_MAX > _returnbutton_handles;
	bool _drawn;
	bool _win;
	CAUSE_OF_DEATH _cause;
	short int _cnt;
	BoxObject _returnbutton;

	GlobalDataPtr _data;
	GamePtr _game;
	DrawerPtr _drawer;
};

