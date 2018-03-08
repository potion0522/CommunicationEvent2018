#include "Base.h"
#include "smart_ptr.h"
#include "Client.h"
#include <Windows.h>

PTR( Console );
PTR( GlobalData );
PTR( Client );
PTR( Drawer );

class Console : public Base {
public:
	Console( GlobalDataPtr data );
	virtual ~Console( );

public:
	std::string getTag( );
	void initialize( );
	void finalize( );
	void update( );

private:
	void ready( );
	void matching( );
	void updateButton( );
	void drawButton( ) const;
	void drawBackGround( ) const;

private:
	bool isHitButton( ) const;

private:
	struct BoxObject {
		LightImageProperty image;
		BoxCollider collider;
	};

	bool _connecting;
	bool _matching;
	bool _clicking;
	short int _cnt;
	int _normal_button_handle;
	int _click_button_handle;
	int _back_image;

	BoxObject _button;
	HCURSOR _cur_hand;

	GlobalDataPtr _data;
	ClientPtr _client;
	DrawerPtr _drawer;
};