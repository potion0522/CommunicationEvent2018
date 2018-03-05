#include "PhaseSetPlayer.h"
#include "Field.h"
#include "GlobalData.h"
#include "Client.h"
#include "Cutin.h"
#include "Sound.h"
#include "Soundplayer.h"

PhaseSetPlayer::PhaseSetPlayer( GlobalDataPtr data, FieldPtr field, CutinPtr cutin, short int player_num ) :
Phase( player_num ),
_data( data ),
_field( field ),
_cutin( cutin ) {
	_soundplayer = _data->getSoundplayerPtr( );

	_selected = false;
	_clicking = 0;

	SoundPtr sound_ptr = _data->getSoundPtr( );
	{//setplayer SE
		_setplayer_se = Base::SoundProperty( );
		_setplayer_se.isLoop = false;
		_setplayer_se.top = true;
		_setplayer_se.wav = sound_ptr->getWav( EFFECT_SOUND, SELECT_SE ).wav;
	}

	{//button SE
		_button_se = Base::SoundProperty( );
		_button_se.isLoop = false;
		_button_se.top = true;
		_button_se.wav = sound_ptr->getWav( EFFECT_SOUND, BUTTON_SE ).wav;
	}
}

PhaseSetPlayer::~PhaseSetPlayer( ) {
}

void PhaseSetPlayer::update( ) {
	if ( _cutin->isCutin( ) ) {
		return;
	}

	if ( _selected ) {
		return;
	}

	_field->setInfoText( "" );
	_field->setInfoText( "あなたの配置をしてください", YELLOW, Drawer::LITTLE_BIG );
	_field->setInfoText( "" );
	_field->setInfoText( "あなたのプレイヤーカラーは", YELLOW, Drawer::LITTLE_BIG );
	_field->setInfoText( "", RED, Drawer::SUPER_BIG );
	std::string your_color = "赤";
	switch ( _player_num ) {
		case 0: your_color = "赤"; break;
		case 1: your_color = "青"; break;
	}
	_field->setInfoText( your_color, ( COLOR )( RED + _player_num ), Drawer::SUPER_BIG );
	_field->setInfoText( "" );
	_field->setInfoText( "です", YELLOW, Drawer::LITTLE_BIG );


	bool select = false;
	selectPlayerPos( &select );

	if ( !select ) {
		return;
	}

	_field->setInfoText( "" );
	_field->setInfoText( "決定を押して確定してください", WATER, Drawer::LITTLE_BIG );
	_field->activeButtonLighting( );
	_field->changeClickButton( );
	
	if ( _data->getClickingLeft( ) ) {
		_clicking = _data->getClickingLeft( );	
		return;
	} else if ( _clicking == 0 ) {
		return;
	}
	_clicking = 0;

	if ( !_field->isHitDecisionButton( ) ) {
		_soundplayer->play( _setplayer_se );
		return;
	}

	if ( _field->isSelectedPlayer( ) ) {
		return;
	}
	_soundplayer->play( _button_se );
	_field->playerPosSelected( );
	_field->setPlayerPoint( _player_num, _field->getTmpPlayerPoint( ) );

	_selected = true;
	
	_data->getClientPtr( )->setPlayerPos( _field->getPlayerPosIdx( _player_num ) );
	_data->getClientPtr( )->sendTcp( );
}

void PhaseSetPlayer::selectPlayerPos( bool *select ) {
	//ヒットしているポジションを探す
	if ( _field->getTmpPlayerPoint( ) != -1 ) {
		*select = true;
	}

	_field->hitPlayerPos( );

	int pos = _field->getPlayerPosHitNum( );

	if ( !_data->getClickLeft( ) ) {
		return;
	}

	if ( _field->isHitDecisionButton( ) ) {
		return;
	}

	_field->setTmpPlayerPoint( );

	*select = false;
	if ( pos != -1 ) {
		*select = true;
	}
}