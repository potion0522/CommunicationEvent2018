#pragma once
#include "Phase.h"
#include "smart_ptr.h"
#include "Field.h"
#include "Base.h"

PTR( PhaseSetMirror );
PTR( GlobalData );
PTR( Field );
PTR( Client );
PTR( Cutin );
PTR( Soundplayer );

class PhaseSetMirror : public Phase {
public:
	PhaseSetMirror( GlobalDataPtr data, FieldPtr field, CutinPtr cutin, short int player_num );
	virtual ~PhaseSetMirror( );

public:
	void update( );
	void recv( );

private:
	void recvItem( );
	void setMirror( );
	void setTmpMirror( );
	
private:
	void updateItem( );
	void invocationItem( );

private:
	bool _use_item;
	bool _item_callback;
	bool _double_mirror;
	bool _recv;
	bool _first;
	int _clicking;
	short int _item;
	short int _order_past;
	Field::Mirror _tmp_mirror;

	Base::SoundProperty _hitfield_se;
	Base::SoundProperty _button_se;
	Base::SoundProperty _mirrorselect_se;
	Base::SoundProperty _itemselect_se;
	Base::SoundProperty _cancel_se;

	GlobalDataPtr _data;
	FieldPtr _field;
	ClientPtr _client;
	CutinPtr _cutin;
	SoundplayerPtr _soundplayer;
};