#include "Direction.h"
#include "DxLib.h"
#include "Base.h"
#include "GlobalData.h"
#include "Debug.h"
#include "Image.h"
#include "Server.h"
#include "Client.h"

Direction::Direction( MACHINE_TYPE type, GlobalDataPtr data ) :
_data( data ) {
	initialize( type );
}

Direction::~Direction( ) {
}

void Direction::initialize( MACHINE_TYPE type ) {
	_scene = NONE;
	_data->initialize( );
	_debug = DebugPtr( new Debug( _data ) );
	_data->setPtr( _debug );
	add( ALL, _debug );
	switch ( type ) {
	case SERVER: add( ALL, _data->getServerPtr( ) ); break;
	case CLIENT: add( ALL, _data->getClientPtr( ) ); break;
	}
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
}
