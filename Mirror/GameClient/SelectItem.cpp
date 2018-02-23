#include "SelectItem.h"
#include "GlobalData.h"
#include "Drawer.h"
#include "Image.h"
#include "KeyBoard.h"
#include <errno.h>
#include <assert.h>

const float MARGIN  = WIDTH_F / 5.0f;

const float NOTSELECT_PITCH   = ( WIDTH_F - MARGIN * 2.0f ) / ITEM_MAX;
const float START_NOTSELECT_X = MARGIN + NOTSELECT_PITCH - NOTSELECT_PITCH / 2.0f;
const float START_NOTSELECT_Y = HEIGHT_F * 0.5f;

const float SELECTED_PITCH   = ( WIDTH_F - MARGIN * 2.0f ) / ITEM_POSSESSION_MAX;
const float START_SELECTED_X = MARGIN + SELECTED_PITCH - SELECTED_PITCH / 2.0f;
const float START_SELECTED_Y = HEIGHT * 0.8f;

const short int SAVE_BUTTON_IDX = 10;
const float DECISIONBUTTON_X = WIDTH_F / 3.0f * 2.0f;
const float DECISIONBUTTON_Y = HEIGHT_F / 5.0f;

const short int RETURN_BUTTON_IDX = 8;
const float RETURNBUTTON_X = WIDTH_F / 3.0f;
const float RETURNBUTTON_Y = HEIGHT_F / 5.0f;

const int PROGRESS_BAR_X = WIDTH / 2;
const int PROGRESS_BAR_Y = HEIGHT / 3 * 2;

std::string path = "Resources/csv/item.csv";

SelectItem::SelectItem( GlobalDataPtr data ) :
_data( data ) {
	setFlag( 1 );
	_drawer = _data->getDrawerPtr( );

	{//CSVの初期化
		FILE *fp;
		errno_t csv_open = fopen_s( &fp, path.c_str( ), "rb" );

		//前回の起動データがなければ
		if ( csv_open != 0 ) {
			//空データ生成
			csv_open = fopen_s( &fp, path.c_str( ), "wb" );
			assert( !csv_open );
		}
		fclose( fp );
	}

	std::array< BoxObject, ITEM_MAX >( ).swap( _items );
	std::array< BoxCollider, ITEM_POSSESSION_MAX >( ).swap( _select_boxes );
	_decisionbutton = BoxObject( );
	_returnbutton = BoxObject( );

	ImagePtr image_ptr = _data->getImagePtr( );
	{//背景
		_back_handle = image_ptr->getPng( BACKGROUND_IMAGE, BACKGROUND_WHITE ).png;
	}

	{//アイテムの画像
		for ( int i = 0; i < ITEM_MAX; i++ ) {
			int handle = image_ptr->getPng( ITEM_IMAGE, i ).png;
			_items[ i ].image.png = handle;
			_item_handles[ i ] = handle;
		}
		_item_image_halfsize = ( short int )( ( float )image_ptr->getPng( ITEM_IMAGE, 0 ).width * 0.5f );
	}
	_item_frame_handle = image_ptr->getPng( ITEM_IMAGE, ITEM_MAX ).png;

	{//決定ボタン
		float half_width  = image_ptr->getPng( BUTTON_IMAGE, SAVE_BUTTON_IDX ).width * 0.5f;
		float half_height = image_ptr->getPng( BUTTON_IMAGE, SAVE_BUTTON_IDX ).height * 0.5f;

		for ( int i = 0; i < BUTTON_TYPE_MAX; i++ ) {
			_decisionbutton_handles[ i ] = image_ptr->getPng( BUTTON_IMAGE, BATTLE_BUTTON_IDX + i ).png;
		}
		_decisionbutton.image.cx = DECISIONBUTTON_X;
		_decisionbutton.image.cy = DECISIONBUTTON_Y;
		_decisionbutton.image.png = image_ptr->getPng( BUTTON_IMAGE, BATTLE_BUTTON_IDX ).png;

		_decisionbutton.collider.lx = ( float )_decisionbutton.image.cx - half_width;
		_decisionbutton.collider.rx = ( float )_decisionbutton.image.cx + half_width;
		_decisionbutton.collider.ly = ( float )_decisionbutton.image.cy - half_height;
		_decisionbutton.collider.ry = ( float )_decisionbutton.image.cy + half_height;

	
	//リターンボタン
		for ( int i = 0; i < BUTTON_TYPE_MAX; i++ ) {
			_returnbutton_handles[ i ] = image_ptr->getPng( BUTTON_IMAGE, RETURN_BUTTON_IDX + i ).png;
		}
		_returnbutton.image.cx = RETURNBUTTON_X;
		_returnbutton.image.cy = RETURNBUTTON_Y;
		_returnbutton.image.png = image_ptr->getPng( BUTTON_IMAGE, RETURN_BUTTON_IDX ).png;

		_returnbutton.collider.lx = ( float )_returnbutton.image.cx - half_width;
		_returnbutton.collider.rx = ( float )_returnbutton.image.cx + half_width;
		_returnbutton.collider.ly = ( float )_returnbutton.image.cy - half_height;
		_returnbutton.collider.ry = ( float )_returnbutton.image.cy + half_height;
	}

	{//プログレスバー
		_bar_frame = LightImageProperty( );
		_bar = LightImageProperty( );

		//フレーム
		_bar_frame.cx = PROGRESS_BAR_X;
		_bar_frame.cy = PROGRESS_BAR_Y;
		_bar_frame.png = image_ptr->getPng( BAR_IMAGE, 0 ).png;

		//バー
		_bar.cx = PROGRESS_BAR_X - image_ptr->getPng( BAR_IMAGE, 0 ).width / 2;
		_bar.cy = PROGRESS_BAR_Y;
		_bar.png = image_ptr->getPng( BAR_IMAGE, 1 ).png;
		_bar_width_half = image_ptr->getPng( BAR_IMAGE, 1 ).width / 2;
		_bar_height_half = image_ptr->getPng( BAR_IMAGE, 1 ).height / 2;

		int length = image_ptr->getPng( BAR_IMAGE, 0 ).width;
		_bar_rate = length / ( FRAME * 2 );
	}


	//選択中アイテムの枠の当たり判定
	for ( int i = 0; i < ITEM_POSSESSION_MAX; i++ ) {
		_select_boxes[ i ].lx = START_SELECTED_X + SELECTED_PITCH * i - _item_image_halfsize;
		_select_boxes[ i ].rx = START_SELECTED_X + SELECTED_PITCH * i + _item_image_halfsize;
		_select_boxes[ i ].ly = START_SELECTED_Y - _item_image_halfsize;
		_select_boxes[ i ].ry = START_SELECTED_Y + _item_image_halfsize;
	}
}

SelectItem::~SelectItem( ) {
}

std::string SelectItem::getTag( ) {
	return "SELECT_ITEM";
}

void SelectItem::initialize( ) {
	_past_hit_item = -1;
	_hit_notselect_item = -1;
	_hit_selected_item = -1;
	_wait_time = 0;
	_drag = false;
	_drop = false;
	_decisionbutton_clicking = false;
	_returnbutton_clicking = false;
	_input = false;
	std::array< SelectedItem, ITEM_POSSESSION_MAX >( ).swap( _selected );
}

void SelectItem::update( ) {
	KeyBoardPtr keyboard( new KeyBoard( ) );
	if ( _data->getKeyState( keyboard->getKeyCode( KeyBoard::X_KEY ) ) == 1 ) {
		_data->setScene( TITLE );
	}

	//決定を押したら
	if ( _input ) {
		_drawer->setFrontString( true, WIDTH / 2, HEIGHT / 2, YELLOW, "アイテム情報を保存しています", Drawer::BIG );

		//プログレスバーの表示
		ImageProperty image = ImageProperty( );

		//バー
		image.cx = _bar.cx;
		image.cy = _bar.cy;
		image.png = _bar.png;
		_drawer->setExtendFrontImage( image, 0, _bar_height_half, _wait_time * _bar_rate, 1 );

		//フレーム
		image.cx = _bar_frame.cx;
		image.cy = _bar_frame.cy;
		image.png = _bar_frame.png;
		_drawer->setFrontImage( image );

		if ( _wait_time < ( FRAME * 2 ) ) {
			_wait_time++;
		} else {
			_data->setScene( TITLE );
		}
		return;
	}

	//座標計算
	calcItemPosition( );

	//当たり判定チェック
	checkHitNotSelectItem( );
	checkHitSelectedItem( );

	//決定ボタン
	if ( ( _decisionbutton_clicking || _returnbutton_clicking ) && !isDrag( ) ) {
		int hit = isHitButton( );
		switch ( hit ) {
		case DECISION_BUTTON   : inputCsv( )              ; break;
		case RETURN_BUTTON     : _data->setScene( TITLE ) ; break;
		default:
			_decisionbutton_clicking = false;
			_returnbutton_clicking = false;
			break;		
		}
	}

	calcButtonAction( );

	//アイテムの移動
	if ( isDrag( ) ) {
		calcDragNotSelectItemPos( );
		calcDragSelectedItemPos( );
	} else {
		if ( !_drag ) {
			_past_hit_item = -1;
		}
		setSelectItem( );
	}

	//描画
	drawNotSelectItem( );
	drawBackGround( );
	drawSelectedItemFrame( );
	drawSelectedItem( );
	drawDecisionButton( );
	drawReturnButton( );
	//ドラッグ中のアイテムは一番上に描画
	drawSelectingItem( );
	//_drawer->setFrontBox( _button.collider.lx, _button.collider.ly, _button.collider.rx, _button.collider.ry, RED );
	//_drawer->setFrontString( false, 20, HEIGHT - 20, RED, std::to_string( _past_hit_item ), Drawer::BIG );
}

void SelectItem::calcItemPosition( ) {
	for ( int i = 0; i < ITEM_MAX; i++ ) {
		if ( _hit_notselect_item == i && isDrag( ) ) {
			continue;
		}
		//画像の座標
		_items[ i ].image.cx = START_NOTSELECT_X + NOTSELECT_PITCH * i;
		_items[ i ].image.cy = START_NOTSELECT_Y;

		//当たり判定
		_items[ i ].collider.lx = ( float )_items[ i ].image.cx - _item_image_halfsize;
		_items[ i ].collider.ly = ( float )_items[ i ].image.cy - _item_image_halfsize;
		_items[ i ].collider.rx = ( float )_items[ i ].image.cx + _item_image_halfsize;
		_items[ i ].collider.ry = ( float )_items[ i ].image.cy + _item_image_halfsize;
	}
}

void SelectItem::checkHitNotSelectItem( ) {
	if ( ( _hit_notselect_item != -1 || _hit_selected_item != -1 ) && isDrag( ) ) {
		return;
	}
	if( _decisionbutton_clicking || _returnbutton_clicking ){
		return;
	}

	double mouse_x = _data->getMouseX( );
	double mouse_y = _data->getMouseY( );

	for ( int i = 0; i < ITEM_MAX; i++ ) {
		BoxCollider box = BoxCollider( );
		box.lx = _items[ i ].collider.lx;
		box.ly = _items[ i ].collider.ly;
		box.rx = _items[ i ].collider.rx;
		box.ry = _items[ i ].collider.ry;
		if ( box.lx <= mouse_x && mouse_x <= box.rx &&
			 box.ly <= mouse_y && mouse_y <= box.ry ) {
			_hit_notselect_item = i;
			return;
		}
	}
	_hit_notselect_item = -1;
}

void SelectItem::checkHitSelectedItem( ) {
	if ( ( _hit_selected_item != -1 || _hit_notselect_item != -1 ) && isDrag( ) ) {
		return;
	}

	if( _decisionbutton_clicking || _returnbutton_clicking ){
		return;
	}

	double mouse_x = _data->getMouseX( );
	double mouse_y = _data->getMouseY( );

	for ( int i = 0; i < ITEM_POSSESSION_MAX; i++ ) {
		if ( !_selected[ i ].flag ) {
			continue;
		}
		BoxCollider box = BoxCollider( );
		box.lx = _select_boxes[ i ].lx;
		box.ly = _select_boxes[ i ].ly;
		box.rx = _select_boxes[ i ].rx;
		box.ry = _select_boxes[ i ].ry;
		if ( box.lx <= mouse_x && mouse_x <= box.rx &&
			 box.ly <= mouse_y && mouse_y <= box.ry ) {
			_hit_selected_item = i;
			return;
		}
	}
	_hit_selected_item = -1;
}

void SelectItem::calcDragNotSelectItemPos( ) {
	if ( _hit_notselect_item < 0 ) {
		return;
	}
	//ドラッグフラグを立てる
	_drag = true;

	double mouse_x = _data->getMouseX( );
	double mouse_y = _data->getMouseY( );

	//座標をマウスを基点に移動
	_items[ _hit_notselect_item ].image.cx = mouse_x;
	_items[ _hit_notselect_item ].image.cy = mouse_y;

	//setSelectように保存しておく
	_past_hit_item = _hit_notselect_item;

	//画面外処理
	if ( mouse_x < 0 || WIDTH  < mouse_x ||
		 mouse_y < 0 || HEIGHT < mouse_y ) {
		_hit_notselect_item = -1;
	}
}

void SelectItem::calcDragSelectedItemPos( ) {
	if ( _hit_selected_item < 0 ) {
		return;
	}
	//ドラッグフラグを立てる
	_drag = true;

	//ドロップフラグを立てる
	//(既に選択されていたアイテムを何もないところにドロップするとフラグが折れる)
	_drop = true;

	double mouse_x = _data->getMouseX( );
	double mouse_y = _data->getMouseY( );

	//座標をマウスを基点に移動
	_selected[ _hit_selected_item ].object.image.cx = mouse_x;
	_selected[ _hit_selected_item ].object.image.cy = mouse_y;

	//setSelectように保存しておく
	_past_hit_item = _hit_selected_item;

	//画面外処理
	if ( mouse_x < 0 || WIDTH  < mouse_x ||
		 mouse_y < 0 || HEIGHT < mouse_y ) {
		_hit_selected_item = -1;
	}
}

void SelectItem::calcButtonAction( ) {
	_decisionbutton.image.png = _decisionbutton_handles[ NORMAL ];
	_returnbutton.image.png = _returnbutton_handles[ NORMAL ];

	if ( isHitButton( ) == NONE_BUTTON ) {
		return;
	}

	if ( _hit_notselect_item != -1 || _hit_selected_item != -1 ) {
		return;
	}

	if ( !isDrag( ) ) {
		return;
	}

	if ( isHitButton( ) == DECISION_BUTTON ) {
		_decisionbutton.image.png = _decisionbutton_handles[ CLICKING ];
		_decisionbutton_clicking = true;
	} else if ( isHitButton( ) == RETURN_BUTTON ) {
		_returnbutton.image.png = _returnbutton_handles[ CLICKING ];
		_returnbutton_clicking = true;
	}
}

void SelectItem::inputCsv( ) {
	std::array< int, ITEM_MAX > values;
	std::array< int, ITEM_MAX >( ).swap( values );

	//集計
	bool empty = true;
	for ( int i = 0; i < ITEM_POSSESSION_MAX; i++ ) {
		if ( !_selected[ i ].flag ) {
			continue;
		}

		empty = false;
		values[ _selected[ i ].type ]++;
	}

	//なにも選択しなかったら
	if ( empty ) {
		_decisionbutton_clicking = false;
		_input = true;
		return;
	}

	FILE *fp;
	errno_t csv_open = fopen_s( &fp, path.c_str( ), "wb" );

	assert( !csv_open );

	//集計結果書き込み
	for ( int i = 0; i < ITEM_MAX; i++ ) {
		std::string tag = convItemString( ( ITEM )i );
		std::string value = std::to_string( values[ i ] );

		fprintf( fp, tag.c_str( ) );
		fprintf( fp, "," );
		fprintf( fp, value.c_str( ) );
		fprintf( fp, "\n" );
	}

	fclose( fp );
	_decisionbutton_clicking = false;
	_input = true;
}

bool SelectItem::isDrag( ) const {
	if ( _data->getClickingLeft( ) > 0 ) {
		return true;
	}
	return false;
}

int SelectItem::isHitButton( ) const {
	double mouse_x = _data->getMouseX( );
	double mouse_y = _data->getMouseY( );

	//決定
	if ( _decisionbutton.collider.lx <= mouse_x && mouse_x <= _decisionbutton.collider.rx &&
		 _decisionbutton.collider.ly <= mouse_y && mouse_y <= _decisionbutton.collider.ry ) {
		return DECISION_BUTTON;
	}

	//リターン
	if ( _returnbutton.collider.lx <= mouse_x && mouse_x <= _returnbutton.collider.rx &&
		 _returnbutton.collider.ly <= mouse_y && mouse_y <= _returnbutton.collider.ry ) {
		return RETURN_BUTTON;
	}

	return NONE_BUTTON;
}

void SelectItem::setSelectItem( ) {
	if ( !_drag ) {
		return;
	}
	_drag = false;

	double mouse_x = _data->getMouseX( );
	double mouse_y = _data->getMouseY( );

	BoxCollider box = BoxCollider( );
	box.lx = ( float )mouse_x - _item_image_halfsize;
	box.ly = ( float )mouse_y - _item_image_halfsize;
	box.rx = ( float )mouse_x + _item_image_halfsize;
	box.ry = ( float )mouse_y + _item_image_halfsize;

	for ( int i = 0; i < ITEM_POSSESSION_MAX; i++ ) {
		if ( ( _select_boxes[ i ].lx <= box.lx && box.lx <= _select_boxes[ i ].rx ||
			   _select_boxes[ i ].lx <= box.rx && box.rx <= _select_boxes[ i ].rx ) //x
			&&
			 ( _select_boxes[ i ].ly <= box.ly && box.ly <= _select_boxes[ i ].ry ||
			   _select_boxes[ i ].ly <= box.ry && box.ry <= _select_boxes[ i ].ry ) //y
			) {
			_selected[ i ].flag = true;
			if ( _drop ) {
				//既に選択されていたものであれば
				_selected[ i ].type = _selected[ _past_hit_item ].type;
			} else {
				//上のアイテム一覧から持ってきたものならば
				_selected[ i ].type = _past_hit_item;
			}

			BoxCollider *collider = &_selected[ i ].object.collider;
			*collider = _select_boxes[ i ];

			LightImageProperty *image = &_selected[ i ].object.image;
			image->cx = collider->lx + _item_image_halfsize;
			image->cy = collider->ly + _item_image_halfsize;
			image->png = _item_handles[ _selected[ i ].type ];

			if ( _drop ) {
				//既に選択されていたものを動かしたなら
				if ( i != _past_hit_item ) {
					//同じ場所でないなら
					_selected[ _past_hit_item ] = SelectedItem( );
				}
			}
			_drop = false;
			return;
		}
	}

	if ( !_drop ) {
		return;
	}
	_selected[ _past_hit_item ] = SelectedItem( );
	_drop = false;
}

void SelectItem::drawNotSelectItem( ) const {
	ImageProperty image = ImageProperty( );

	for ( int i = 0; i < ITEM_MAX; i++ ) {
		image = ImageProperty( );
		if ( _hit_notselect_item == i && isDrag( ) ) {
			continue;
		}
		image.cx = _items[ i ].image.cx;
		image.cy = _items[ i ].image.cy;
		image.png = _items[ i ].image.png;
		_drawer->setFrontImage( image );
	}
}

void SelectItem::drawBackGround( ) const {
	ImageProperty image = ImageProperty( );
	image.cx = WIDTH / 2;
	image.cy = HEIGHT / 2;
	image.png = _back_handle;

	_drawer->setBackImage( image );
}

void SelectItem::drawSelectedItemFrame( ) const {
	for ( int i = 0; i < ITEM_POSSESSION_MAX; i++ ) {
		LightImageProperty image = LightImageProperty( );
		image.cx = _select_boxes[ i ].lx + _item_image_halfsize;
		image.cy = _select_boxes[ i ].ly + _item_image_halfsize;
		image.png = _item_frame_handle;
		_drawer->setFrontImage( image );
	}
}

void SelectItem::drawSelectedItem( ) const {
	ImageProperty image = ImageProperty( );
	for ( int i = 0; i < ITEM_POSSESSION_MAX; i++ ) {
		if ( !_selected[ i ].flag ) {
			continue;
		}
		image = ImageProperty( );
		image.cx  = _selected[ i ].object.image.cx;
		image.cy  = _selected[ i ].object.image.cy;
		image.png = _selected[ i ].object.image.png;

		_drawer->setFrontImage( image );
	}
}

void SelectItem::drawSelectingItem( ) const {
	if ( !isDrag( ) ) {
		return;
	}
	LightImageProperty tmp = LightImageProperty( );
	int hit = _hit_notselect_item;

	if ( hit < 0 ) {
		hit = _hit_selected_item;
		if ( hit < 0 ) {
			return;
		}
		tmp = _selected[ hit ].object.image;
	} else {
		tmp = _items[ hit ].image;
	}

	ImageProperty image = ImageProperty( );
	image.cx  = tmp.cx;
	image.cy  = tmp.cy;
	image.png = tmp.png;
	_drawer->setFrontImage( image );
}

void SelectItem::drawDecisionButton( ) const {
	_drawer->setFrontImage( _decisionbutton.image );
}

void SelectItem::drawReturnButton( ) const {
	_drawer->setFrontImage( _returnbutton.image );
}

std::string SelectItem::convItemString( ITEM item ) {
	std::string str;

	switch ( item ) {
	case LAZER_RESET    : str = "LAZER_RESET"   ; break;
	case DOUBLE_MIRROR  : str = "DOUBLE_MIRROR" ; break;
	case REVERSE_MIRROR : str = "REVERSE_MIRROR"; break;
	default: str = "DEFAULT"; break;
	}

	return str;
}