#include "PhaseItem.h"
#include "GlobalData.h"
#include "Field.h"
#include "Client.h"

PhaseItem::PhaseItem( GlobalDataPtr data, FieldPtr field, int player_num ) :
Phase( player_num ),
_data( data ),
_field( field ) {
	_clicking = 0;
}

PhaseItem::~PhaseItem( ) {
}

void PhaseItem::update( ) {
	//アイテムチェック
	recv( );

	if ( _use_item ) {
		//if ( !_cutin && _item != REVERSE_MIRROR ) {
		//	//アイテムカットイン
		//	_cutin_spd_rate = 1.8f;
		//	_cutin_image.png = _cutin_item_handles[ _item ];
		//	_background_cutin_image.png = _background_cutin_handles[ ( int )CUTIN_BACK_ITEM ];
		//	drawBackCutin( );
		//	drawStringCutin( );
		//	calcBackCutin( );
		//	calcStringCutin( );
		//	return;
		//}

		invocationItem( );
		return;
	}


	///////////////////////////////////////////////////////
	if ( _order != _player_num ) {
		return;
	}

	//if ( !_cutin ) {
	//	return;
	//}

	int idx = _field->getHitItemIdx( );
	_field->changeClickButton( );

	int item = _field->getSelectItem( );
	//ボタン点滅
	switch ( item ) {
	case LAZER_RESET:
		_field->activeButtonLighting( );
		break;

	case DOUBLE_MIRROR: 
		if ( _field->isSelectedMirror( ) ) {
			_field->activeButtonLighting( );
		}
		break;

	case REVERSE_MIRROR:
		_field->activeButtonLighting( );
		break;

	default:
		break;
	}
	
	if ( _data->getClickingLeft( ) ) {
		_clicking = _data->getClickingLeft( );	
		return;
	} else if ( _clicking == 0 ) {
		return;
	}
	_clicking = 0;

	if ( _field->getFieldPosHitNum( ) != -1 ) {
		return;
	}

	//決定ボタンを押さないで
	if ( !_field->isHitDecisionButton( ) ) {
		//ミラーコマンドを選択してなくて
		if ( _field->getHitMirrorCommandIdx( ) < 0 ) {
			//フィールドも選択していなければ
			if ( _field->getFieldPosHitNum( ) < 0 ) {
				//キャンセルボタンであれば-1
				if ( _field->isHitItemCancelButton( ) ) {
					idx = -1;
				}
				//アイテム番号セット
				_field->selectItem( idx );
			}
		}
		return;
	}

	item = _field->getSelectItem( );
	if ( item < 0 ) {
		return;
	}
	if ( item == ( int )DOUBLE_MIRROR ) {
		if ( !_field->isSelectedMirror( ) ) {
			return;
		}
		int order = 0;
		if ( !_player_num ) {
			order = ( _turn + 1 ) % 2 + 1;
		} else {
			order = _turn % 2 + 1;
		}

		if ( order != 1 ) {
			_field->mirrorPosNotSelected( );
			return;
		}
	}

	_client->setItemFlag( true );
	_client->setItem( _field->getSelectItem( ) );
	_client->setItemUser( );
	_client->sendTcp( );

	_field->useItem( );
}

void PhaseItem::recv( ) {
	if ( !_client->isRecvingUdp( ) ) {
		return;
	}

	bool use = _client->isItemFlag( );

	if ( use != _use_item ) {
		_use_item = use;
		if ( !_use_item ) {
			return;
		}
		_item = _client->getItem( );
		_item_callback = false;
		//_cutin = false;
		//resetBackCutin( );
		//resetStringCutin( );
	}
}

void PhaseItem::invocationItem( ) {
	if ( _item_callback ) {
		return;
	}

	switch ( ( ITEM )_item ) {
	case LAZER_RESET:
		{//レーザーの位置を変更
			int lazer_pos = _client->getLazerPoint( );
			if ( lazer_pos == ( unsigned char )-1 ) {
				return;
			}
			_field->setLazerPoint( lazer_pos );
			_field->mirrorPosNotSelected( );
			_mirror_phase_recv = false;
			_attack_phase_recv = false;
			_judge_phase_recv = false;
			_order = -1;
			_send_live = false;
			_tmp_mirror = Field::Mirror( );
			_field->nextTurn( );
			resetBackCutin( );
			resetStringCutin( );
		}
		break;

	case DOUBLE_MIRROR:
		{//鏡2枚配置
			int idx = _client->getItemUser( );
			Field::Mirror mirror = Field::Mirror( );
			mirror.x = _client->getStcX( idx );
			mirror.y = _client->getStcY( idx );
			mirror.angle = _client->getStcAngle( idx );
			mirror.player_num = idx;

			_field->setMirrorPoint( mirror.player_num, mirror.x, mirror.y, mirror.angle );

			if ( idx == _player_num ) {
				_field->mirrorPosNotSelected( );
				_tmp_mirror = Field::Mirror( );
				_field->nextTurn( );
				_double_mirror = true;
				resetBackCutin( );
				resetStringCutin( );
			}
		}
		break;

	case REVERSE_MIRROR:
		{
			_reverse_mirror = true;
		}
		break;
	}

	_client->setItemFlag( false );
	_client->setItem( 0 );
	_client->sendTcp( );
	_item_callback = true;
	_item = ( short int )ITEM_NONE;
}

ITEM PhaseItem::getItem( ) const {
	return ( ITEM )_item;
}