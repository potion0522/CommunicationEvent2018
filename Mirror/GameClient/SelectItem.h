#pragma once
#include "Base.h"
#include "smart_ptr.h"
#include "const.h"
#include <string>
#include <array>

PTR( SelectItem );
PTR( GlobalData );
PTR( Drawer );

class SelectItem : public Base {
public:
	SelectItem( GlobalDataPtr data );
	virtual ~SelectItem( );

public:
	std::string getTag( );
	void initialize( );
	void update( );

private:
	void checkHitNotSelectItem( );
	void checkHitSelectedItem( );
	void calcItemPosition( );
	void calcDragNotSelectItemPos( );
	void calcDragSelectedItemPos( );
	void calcButtonAction( );
	void setSelectItem( );
	void inputCsv( );
	bool isDrag( ) const;
	bool isHitButton( ) const;

private:
	std::string convItemString( ITEM item );

private:
	void drawNotSelectItem( ) const;
	void drawBackGround( ) const;
	void drawSelectedItemFrame( ) const;
	void drawSelectedItem( ) const;
	void drawSelectingItem( ) const;
	void drawButton( ) const;

private:
	struct BoxObject {
		BoxCollider collider;
		LightImageProperty image;
	};
	struct SelectedItem {
		bool flag;
		short int type;
		BoxObject object;
	};
	enum BUTTON_STATE {
		NORMAL,
		CLICKING
	};

	//変数
	static const int BUTTON_TYPE_MAX = 2;
	short int _past_hit_item;
	short int _hit_notselect_item;//選択可能アイテム
	short int _hit_selected_item;//選択済みアイテム
	short int _wait_time;
	short int _bar_rate;
	short int _bar_width_hulf;
	short int _bar_height_hulf;
	bool _drag;
	bool _drop;
	bool _button_clicking;
	bool _input; //決定
	std::array< BoxCollider, ITEM_POSSESSION_MAX > _select_boxes;

	//画像関係
	int _back_handle;
	int _item_frame_handle;
	short int _item_image_hulfsize;
	std::array< int, ITEM_MAX > _item_handles;
	std::array< int, BUTTON_TYPE_MAX > _button_handles;
	std::array< BoxObject, ITEM_MAX > _items;
	std::array< SelectedItem, ITEM_POSSESSION_MAX > _selected;
	BoxObject _button;
	LightImageProperty _bar_frame;
	LightImageProperty _bar;

	GlobalDataPtr _data;
	DrawerPtr _drawer;
};

