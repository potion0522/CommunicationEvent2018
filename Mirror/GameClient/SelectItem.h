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
	void calcItemPosition( );
	void checkHitItem( );
	void calcDragItemPos( );
	void setSelectItem( );
	bool isDrag( ) const;

private:
	void drawItemImage( ) const;
	void drawBackGround( ) const;
	void drawSelectedItemFrame( ) const;
	void drawSelectedItem( ) const;

private:
	struct BoxCollider {
		float lx;
		float ly;
		float rx;
		float ry;
	};
	struct ItemBox {
		BoxCollider collider;
		LightImageProperty image;
	};
	struct SelectedItem {
		bool flag;
		short int type;
	};

	//ïœêî
	short int _hit_item;
	bool _drag;
	bool _drop;
	std::array< BoxCollider, ITEM_POSSESSION_MAX > _select_boxes;

	//âÊëúä÷åW
	int _back_handle;
	short int _item_image_hulfsize;
	std::array< ItemBox, ITEM_MAX > _items;
	std::array< SelectedItem, ITEM_POSSESSION_MAX > _selected;

	GlobalDataPtr _data;
	DrawerPtr _drawer;
};

