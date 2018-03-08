#include "Direction.h"
#include "DxLib.h"
#include "Base.h"
#include "GlobalData.h"
#include "Debug.h"
#include "Image.h"
#include "Server.h"
#include "Client.h"
#include "Drawer.h"

Direction::Direction( MACHINE_TYPE type, GlobalDataPtr data ) :
_data( data ) {
	_scene = NONE_SCENE;
	_debug = DebugPtr( new Debug( _data ) );
	_data->setPtr( _debug );
	add( ALL, _data->getDrawerPtr( ) );
	add( ALL, _debug );
	switch ( type ) {
	case SERVER: add( CONNECT, _data->getServerPtr( ) ); break;
	case CLIENT: add( CONNECT, _data->getClientPtr( ) ); break;
	}
}

Direction::~Direction( ) {
	std::map< SCENE, std::vector< BasePtr > >::iterator ite;
	ite = _exe.begin( );
	for ( ite; ite != _exe.end( ); ite++ ) {
		int size = ( int )ite->second.size( );
		for ( int i = 0; i < size; i++ ) {
			ite->second[ i ]->finalize( );
		}
	}
}

void Direction::initialize( ) {
	//全て初期化
	std::map< SCENE, std::vector< BasePtr > >::iterator ite;
	ite = _exe.begin( );
	for ( ite; ite != _exe.end( ); ite++ ) {
		int size = ( int )ite->second.size( );
		for ( int i = 0; i < size; i++ ) {
			ite->second[ i ]->initialize( );
		}
	}
}

void Direction::update( ) {
	SCENE scene = _data->getScene( );
	if ( _scene != scene ) {
		_scene = scene;
		initNextProcess( );
	}

	//デバッグON
	if ( _data->getKeyState( KEY_INPUT_SPACE ) == 1 && _data->getMachineType( ) == CLIENT ) {
		if ( !_data->isCommandFlag( ) ) {
			_debug->setFlag( ( _debug->getFlag( ) + 1 ) % 2 );
		}
	}

	//終了
	if ( _data->getKeyState( KEY_INPUT_ESCAPE ) == 1 ) {
		if ( !_data->isCommandFlag( ) ) {
			_data->setFlag( 0 );
		}
	}
}

void Direction::initNextProcess( ) {
	int size = ( int )_exe[ _scene ].size( );
	for ( int i = 0; i < size; i++ ) {
		if ( _scene == CONNECT ) {
			break;
		}
		_exe[ _scene ][ i ]->initialize( );
	}
}

void Direction::add( SCENE scene, BasePtr ptr ) {
	_exe[ scene ].push_back( ptr );
}

void Direction::run( ) {
	update( );

	//エラーメッセージがあるかどうか
	bool error = _data->isMessageFlag( );

	std::map< SCENE, std::vector< BasePtr > >::iterator ite;
	ite = _exe.begin( );
	for ( ite; ite != _exe.end( ); ite++ ) {
		SCENE ite_scene = ite->first;

		//現在見ているシーンがオールとコネクト以外であったらコンティニュー
		if ( ite_scene != _scene && ite_scene != ALL ) {
			if ( ite_scene != CONNECT ) {
				continue;
			}
		}

		//今見ているのがコネクトで現在進行中のシーンが
		//バトルより大きい(リザルト)かコネクトより小さい(タイトル)であったらコンティニュー
		if ( ite_scene == CONNECT && ( BATTLE < _scene || _scene < CONNECT ) ) {
			continue;
		}

		//エラーメッセージがあれば
		if ( error ) {
			if ( ite_scene != ALL ) {
				continue;
			}
		}

		int size = ( int )ite->second.size( );

		if ( size < 1 ) {
			continue;
		}

		std::vector< BasePtr > exe;
		exe = ite->second;

		for ( int i = 0; i < size; i++ ) {
			if ( !exe[ i ]->getFlag( ) ) {
				continue;
			}
			//エラーメッセージがあったら
			if ( error ) {
				std::string tag = exe[ i ]->getTag( );
				if ( tag != "GLOBALDATA" && tag != "DRAWER" ) {
					continue;
				}
			}

			if ( _debug->getFlag( ) ) {
				_debug->setActiveClass( exe[ i ]->getTag( ) );
			}
			exe[ i ]->update( );
		}
	}
}
