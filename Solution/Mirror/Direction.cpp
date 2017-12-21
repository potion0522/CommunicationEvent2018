#include "Direction.h"
#include "DxLib.h"
#include "Base.h"
#include "GlobalData.h"
#include "Debug.h"
#include "Title.h"
#include "Image.h"
#include "Server.h"
#include "Client.h"

Direction::Direction( GlobalDataPtr data ) :
_data( data ) {
	initialize( );
}

Direction::~Direction( ) {
}

void Direction::initialize( ) {
	_scene = NONE;
	_data->initialize( );

	_debug = DebugPtr( new Debug( _data ) );
	_title = TitlePtr( new Title( _data ) );
	_server = ServerPtr( new Server( ) );
	_server->setFlag( 1 );

	_data->setPtr( _debug );
	_data->setPtr( _title );

	add( ALL, _debug );
	add( TITLE, _title );
	add( ALL, _server );
}

void Direction::update( ) {
	_data->update( );
	_scene = _data->getScene( );

	//デバッグON
	if ( _data->getKeyState( KEY_INPUT_SPACE ) == 1 ) {
		_debug->setFlag( ( _debug->getFlag( ) + 1 ) % 2 );
	}

	_debug->addLog( "test" );

	//終了
	if ( _data->getKeyState( KEY_INPUT_ESCAPE ) ) {
		_data->setFlag( 0 );
	}
}

void Direction::initNextProcess( ) {
}

void Direction::add( SCENE scene, BasePtr ptr ) {
	_exe[ scene ].push_back( ptr );
}

void Direction::run( ) {
	update( );

	std::map< SCENE, std::vector< BasePtr > >::iterator ite;
	ite = _exe.begin( );
	for ( ite; ite != _exe.end( ); ite++ ) {
		SCENE ite_scene = ite->first;
		if ( ite_scene != _scene && ite_scene != ALL ) {
			continue;
		}

		int size = ( int )ite->second.size( );
		for ( int i = 0; i < size; i++ ) {
			if ( !ite->second[ i ]->getFlag( ) ) {
				continue;
			}
			_debug->setActiveClass( ite->second[ i ]->getTag( ) );
			ite->second[ i ]->update( );
		}
	}
	
	_debug->addLog( "接続を待っています。" );

	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		if ( _server->isConnecting( i ) ) {
			std::string log = "_handle[ " + std::to_string( i ) + " ]が接続しました。";
			_debug->addLog( log );
			Client::NetWorkData data;
			data.test = i + 1;
			_server->sendData( i, data );
		}
	}

	for ( int i = 0; i < MACHINE_MAX; i++ ) {
		if ( !_server->isConnecting( i ) ) {
			continue;
		}
		if ( !_server->isRecving( i ) ) {
			continue;
		}
		Client::NetWorkData data;
		data = _server->getData( i );
		_debug->addLog( std::to_string( data.test ) );
	}
}
