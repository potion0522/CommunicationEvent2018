#include "Direction.h"
#include "DxLib.h"

Direction::Direction( std::shared_ptr< GlobalData > data ) :
_data( data ) {
	initialize( );
}

Direction::~Direction( ) {
}

void Direction::initialize( ) {
	_scene = NONE;
	_data->initialize( );

	_debug = std::shared_ptr< Debug >( new Debug( _data ) );

	_data->setPtr( _debug );
	_init = false;
}

void Direction::update( ) {
	if ( !_init ) {
		_base = _data;
		add( ALL, _base );
		_base = _debug;
		add( ALL, _base );
		_init = true;
	}
	
	_scene = _data->getScene( );

	run( );

	//デバッグON
	if ( _data->getKeyState( KEY_INPUT_SPACE ) == 1 ) {
		_debug->setFlag( ( _debug->getFlag( ) + 1 ) % 2 );
	}

	//終了
	if ( _data->getKeyState( KEY_INPUT_ESCAPE ) ) {
		_data->setFlag( 0 );
	}
}

void Direction::initNextProcess( ) {
}

void Direction::add( SCENE scene, std::shared_ptr< Base > ptr ) {
	_exe[ scene ] = ptr;
}

void Direction::run( ) {
	std::map< SCENE, std::shared_ptr< Base > >::iterator ite;
	ite = _exe.begin( );
	for ( ite; ite != _exe.end( ); ite++ ) {
		if ( ite->first == _data->getScene( ) ) {
			ite->second->update( );
		}
		if ( ite->first == ALL ) {
			ite->second->update( );
		}
	}
}
