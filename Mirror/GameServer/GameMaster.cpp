#include "GameMaster.h"
#include "GlobalData.h"
#include "Connector.h"
#include "Server.h"
#include "Log.h"
#include "Field.h"
#include "Command.h"
#include <random>
#include <map>

GameMaster::GameMaster( GlobalDataPtr data, ConnectorPtr connector, LogPtr log, CommandPtr command ) :
_data( data ),
_connector( connector ),
_log( log ),
_command( command ) {
	_field = FieldPtr( new Field( _data ) );
	_server = _data->getServerPtr( );
	setFlag( 1 );
}

GameMaster::~GameMaster( ) {
}

std::string GameMaster::getTag( ) {
	return "GAMEMASTER";
}

void GameMaster::initialize( ) {
	_matching = false;
	_dice = true;
	_use_item = false;
	_reverse_mirror = false;
	_winner = -1;
	_turn = 1;
	_item = 0;
	_item_user = 0;
	std::array< Data, PLAYER_NUM >( ).swap( _client_data );
	for ( int i = 0; i < PLAYER_NUM; i++ ) {
		_client_data[ i ].order = i + 1;
	}
	_phase = SET_PLAYER_PHASE;
	_server->setBattlePhase( _phase );
	_field->setPhase( _phase );
}

void GameMaster::update( ) {
	bool matching = _connector->isMatching( );
	if ( _matching != matching ) {
		_matching = matching;
		if ( !_matching ) {
			initialize( );
		}
	}

	if ( !_matching ) {
		return;
	}
	commandExecution( );

	if ( !_dice ) {
		orderPlayer( );
	}

	//アイテムチェック
	bool use = _use_item;
	checkItemFlag( );
	if ( use != _use_item && _use_item ) {
		return;
	}

	if ( _use_item ) {
		checkCallBack( );
		updateItemCalc( );
		_server->sendDataUdp( );
		return;
	}

	switch ( _phase ) {
	case SET_PLAYER_PHASE	: updatePlayerPhase( ); break;
	case SET_MIRROR_PHASE	: updateMirrorPhase( ); break;
	case ATTACK_PHASE		: updateAttackPhase( ); break;
	case JUDGE_PHASE		: updateJudgePhase ( ); break;
	}

	_server->sendDataUdp( );

	switch ( _phase ) {
	case SET_PLAYER_PHASE	: inputPlayerPhase( ); break;
	case SET_MIRROR_PHASE	: inputMirrorPhase( ); break;
	case ATTACK_PHASE		: inputAttackPhase( ); break;
	case JUDGE_PHASE		: checkCallBack   ( ); break;
	}
}

void GameMaster::orderPlayer( ) {
	_client_data[ 0 ].order = ( _turn + 1 ) % 2 + 1;
	_client_data[ 1 ].order = _turn % 2 + 1;
	_dice = true;
}

int GameMaster::getWaitingIdx( ) const {
	int idx = -1;
	for ( int i = 0; i < PLAYER_NUM; i++ ) {
		if ( _client_data[ i ].fin ) {
			continue;
		}
		if ( idx != -1 && _client_data[ i ].order - 1 > idx ) {
			continue;
		}
		idx = i;
	}
	return idx;
}

int GameMaster::getOrderIdx( int order ) const {
	int idx = -1;
	for ( int i = 0; i < PLAYER_NUM; i++ ) {
		if ( _client_data[ i ].order != order ) {
			continue;
		}
		idx = i;
		break;
	}
	return idx;
}

int GameMaster::calcLazerPoint( int exclusion ) {
	int player_one = _field->getPlayerPoint( 0 );
	int player_two = _field->getPlayerPoint( 1 );
	std::map< int, int > point;
	for ( int i = 0; i < PLAYER_POSITION * 2; i++ ) {
		point[ i ] = i;
	}

	point.erase( player_one );
	point.erase( player_two );

	if ( exclusion != -1 ) {
		point.erase( exclusion );
	}

	int rand = _data->getRandom( 0, ( int )point.size( ) );
	std::map< int, int >::iterator ite;
	ite = point.begin( );
	for ( int i = 0; i < rand - 1; i++ ) {
		ite++;
	}

	return ite->second;
}

void GameMaster::updatePlayerPhase( ) {
	int idx = getWaitingIdx( );
	_server->setOrder( -1 );

	if ( idx != -1 ) {
		return;
	}

	for ( int i = 0; i < PLAYER_NUM; i++ ) {
		_field->setPlayerPoint( i, _client_data[ i ].player_pos );
	}

	_phase = SET_MIRROR_PHASE;
	_server->setBattlePhase( _phase );
	_field->setPhase( _phase );

	int lazer = calcLazerPoint( );
	_server->setLazerPos( lazer );
	_field->setLazerPoint( lazer );

	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		_server->setPlayerPos( i, _client_data[ i ].player_pos );
		_client_data[ i ].fin = false;
	}
}

void GameMaster::updateMirrorPhase( ) {
	int idx = getWaitingIdx( );

	_server->setOrder( idx );

	if ( idx != -1 ) {
		return;
	}

	_phase = ATTACK_PHASE;
	_server->setBattlePhase( _phase );
	_field->setPhase( _phase );

	idx = getOrderIdx( 1 );

	//取り消しかどうかを判断
	bool break_mirror = false;
	if ( _client_data[ 0 ].flag && _client_data[ 1 ].flag ) {
		if ( _client_data[ 0 ].x == _client_data[ 1 ].x &&
			 _client_data[ 0 ].y == _client_data[ 1 ].y &&
			 _client_data[ 0 ].angle == _client_data[ 1 ].angle ) {
			break_mirror = true;
		}
	}

	if ( break_mirror ) {
		_field->deleteMirrorPoint( _client_data[ 0 ].x + _client_data[ 0 ].y * FIELD_COL );
	} else {
		for ( int i = 0; i < PLAYER_NUM; i++ ) {
			Data data = _client_data[ idx ];
			if ( data.flag ) {
				_field->setMirrorPoint( idx, data.x, data.y, data.angle );
			}

			idx = ( idx + 1 ) % PLAYER_NUM;
		}
	}

	if ( _reverse_mirror ) {
		_field->reverseMirror( );
		_reverse_mirror = false;
	}

	idx = getOrderIdx( 1 );
	for ( int i = 0; i < PLAYER_NUM; i++ ) {
		Data data = _client_data[ idx ];
		_server->setStcPlayerNum( i, data.player_num );
		_server->setStcX( i, data.x );
		_server->setStcY( i, data.y );
		_server->setStcAngle( i, data.angle );
		_server->setStcFlag( i, data.flag );

		idx = ( idx + 1 ) % PLAYER_NUM;
	}

	for ( int i = 0; i < PLAYER_NUM; i++ ) {
		_client_data[ i ].fin = false;
	}
}

void GameMaster::updateAttackPhase( ) {
	int idx = getWaitingIdx( );
	_server->setOrder( -1 );

	if ( idx != -1 ) {
		return;
	}

	for ( int i = 0; i < PLAYER_NUM; i++ ) {
		if ( !_client_data[ i ].alive ) {
			_winner = ( i + 1 ) % PLAYER_NUM;
			break;
		}
	}
	
	_phase = JUDGE_PHASE;
	_server->setBattlePhase( _phase );
	_server->setStcWinner( _winner );
	_field->setPhase( _phase );

	for ( int i = 0; i < PLAYER_NUM; i++ ) {
		_client_data[ i ].fin = false;
	}
}

void GameMaster::updateJudgePhase( ) {
	int idx = getWaitingIdx( );
	_server->setOrder( -1 );

	if ( idx != -1 ) {
		return;
	}

	if ( _winner < 0 ) {
		//勝敗なし
		_phase = SET_MIRROR_PHASE;
		_field->setPhase( _phase );
		_server->setBattlePhase( _phase );
		_field->nextTurn( );

		if ( _turn % TURN_MAX == 0 ) {
			int lazer = calcLazerPoint( _field->getLazerPointIdx( ) );
			_field->setLazerPoint( lazer );
			_server->setLazerPos( lazer );
		}

		int one = _client_data[ 0 ].player_pos;
		int two = _client_data[ 1 ].player_pos;
		std::array< Data, PLAYER_NUM >( ).swap( _client_data );
		_client_data[ 0 ].player_pos = one;
		_client_data[ 1 ].player_pos = two;
		_dice = false;
		_turn++;
		orderPlayer( );
	} else {
		//勝負あり！
		_data->setScene( RESULT );
	}
}

void GameMaster::inputPlayerPhase( ) {
	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		if ( _server->isRecving( i ) ) {
			_client_data[ i ].player_pos = _server->getPlayerPos( i );
			_client_data[ i ].fin = true;
		}
	}
}

void GameMaster::inputMirrorPhase( ) {
	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		if ( _server->isRecving( i ) ) {
			_client_data[ i ].flag = _server->getCtsFlag( i );
			_client_data[ i ].player_num = i;
			_client_data[ i ].x = _server->getCtsX( i );
			_client_data[ i ].y = _server->getCtsY( i );
			_client_data[ i ].angle = _server->getCtsAngle( i );
			_client_data[ i ].fin = true;
		}
	}
}

void GameMaster::inputAttackPhase( ) {
	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		if ( _server->isRecving( i ) ) {
			_client_data[ i ].alive = _server->getAlive( i );
			_client_data[ i ].fin = true;
		}
	}
}

void GameMaster::checkCallBack( ) {
	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		if ( _server->isRecving( i ) ) {
			_client_data[ i ].fin = true;
		}
	}
}

void GameMaster::updateItemCalc( ) {
	int idx = getWaitingIdx( );
	_server->setOrder( -1 );

	if ( idx != -1 ) {
		return;
	}

	for ( int i = 0; i < PLAYER_NUM; i++ ) {
		_client_data[ i ].fin = false;
	}

	if ( ( ITEM )_item == DOUBLE_MIRROR ) {
		if ( _client_data[ _item_user ].order == PLAYER_NUM ) {
			_client_data[ ( _item_user + 1 ) % PLAYER_NUM ].fin = true;
		}
	}

	if ( ( ITEM )_item == REVERSE_MIRROR ) {
		_client_data[ _item_user ].fin = true;
		if ( _client_data[ _item_user ].order == PLAYER_NUM ) {
			_client_data[ ( _item_user + 1 ) % PLAYER_NUM ].fin = true;
		}
	}

	_use_item = false;
	_item = 0;
	_phase = SET_MIRROR_PHASE;
	_server->setBattlePhase( _phase );
	_server->setItemFlag( false );
	_server->setItem( 0 );
	_field->setPhase( _phase );
}

void GameMaster::invocationItem( ) {
	switch ( _item ) {
	case LAZER_RESET:
		{//レーザーの位置を変更
			int lazer = calcLazerPoint( _field->getLazerPointIdx( ) );
			_field->setLazerPoint( lazer );
			_server->setLazerPos( lazer );
		}
		break;

	case DOUBLE_MIRROR:
		{//鏡2枚設置
			Data data = Data( );
			data.player_num = _item_user;
			data.x = _server->getCtsX( _item_user );
			data.y = _server->getCtsY( _item_user );
			data.angle = _server->getCtsAngle( _item_user );
			_field->setMirrorPoint( _item_user, data.x, data.y, data.angle );

			_server->setStcPlayerNum( _item_user, data.player_num );
			_server->setStcX( _item_user, data.x );
			_server->setStcY( _item_user, data.y );
			_server->setStcAngle( _item_user, data.angle );
			_server->setStcFlag( _item_user, true );
		}
		break;

	case REVERSE_MIRROR:
		{//全鏡反転
			_reverse_mirror = true;
		}
		break;
	}

	_server->setItem( _item );
	_server->setItemUser( _item_user );
	_server->setItemFlag( true );
}

void GameMaster::checkItemFlag( ) {
	bool use = false;
	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		if ( !_server->isItemFlag( i ) ) {
			continue;
		}

		_item = _server->getItem( i );
		_item_user = _server->getItemUser( i );
		use = true;
		break;
	}

	if ( use != _use_item && use ) {
		_use_item = use;
		invocationItem( );

		for ( int i = 0; i < MACHINE_MAX; i++ ) {
			_client_data[ i ].fin = false;
		}
	}
}

void GameMaster::commandExecution( ) {
	if ( _command->getWordNum( ) < 1 ) {
		return;
	}

	if ( _command->getWord( 0 ) == "ALIVE" ) {
		_client_data[ 1 ].alive = true;
		_client_data[ 1 ].fin = true;
		_log->add( "player 1 live" );
	}

	if ( _command->getWord( 0 ) == "DEAD" ) {
		_client_data[ 1 ].alive = false;
		_client_data[ 1 ].fin = true;
		_log->add( "player 1 dead" );
	}

	if ( _command->getWord( 0 ) == "CALLBACK" ) {
		_client_data[ 1 ].fin = true;
		_log->add( "player 1 flag true" );
	}

	///////////////////////////////////////////////////
	if ( _command->getWordNum( ) < 2 ) {
		return;
	}

	if ( _command->getWord( 0 ) != "SET" ) {
		return;
	}

	if ( _command->getWord( 1 ) == "PLAYER" ) {
		std::string pos = _command->getWord( 2 );
		if ( ( int )pos.size( ) > 1 ) {
			return;
		}
		int val = atoi( pos.c_str( ) );
		if ( 0 <= val && val <= 4 ) {
			_client_data[ 1 ].player_pos = val + PLAYER_POSITION;
			_client_data[ 1 ].fin = true;
			_log->add( "player 1 pos " + std::to_string( val ) );
		}
	}

	if ( _command->getWord( 1 ) == "ITEM" ) {
		int value = atoi( _command->getWord( 2 ).c_str( ) );
		if ( value < ( int )ITEM_MAX ) {
			_item = value;
			_use_item = true;
			invocationItem( );
			for ( int i = 0; i < MACHINE_MAX; i++ ) {
				_client_data[ i ].fin = false;
			}
			_log->add( "set item " + _command->getWord( 2 ) );
		}
	}

	///////////////////////////////////////////////////
	if ( _command->getWordNum( ) < 5 ) {
		return;
	}

	if ( _command->getWord( 1 ) == "MIRROR" ) {
		std::string x = _command->getWord( 2 );
		std::string y = _command->getWord( 3 );
		std::string angle = _command->getWord( 4 );
		if ( ( int )x.size( ) > 1 || ( int )y.size( ) > 1 ) {
			return;
		}
		if ( angle != "RIGHT" && angle != "LEFT" ) {
			return;
		}
		const int FIELD_ROW = 5;
		const int FIELD_COL = 5;
		int val_x = atoi( x.c_str( ) );
		int val_y = atoi( y.c_str( ) );
		if ( 0 <= val_x && val_x < FIELD_COL &&
			 0 <= val_y && val_y < FIELD_ROW ) {
			_client_data[ 1 ].flag = true;
			_client_data[ 1 ].player_num = 1;
			_client_data[ 1 ].x = val_x;
			_client_data[ 1 ].y = val_y;
			_client_data[ 1 ].angle = ( angle == "RIGHT" ? RIGHT : LEFT );
			_client_data[ 1 ].fin = true;
			_log->add( "mirror pos ( " + x + ", " + y + " )" );
			_log->add( "       angle " + angle );
		}
	}
}

int GameMaster::getWinner( ) const {
	return _winner;
}