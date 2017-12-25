#include "Drawer.h"
#include "GlobalData.h"
#include "BaseDraw.h"
#include "DrawTitle.h"

Drawer::Drawer( GlobalDataPtr data ) :
_data( data ) {
	initialize( );
}

Drawer::~Drawer( ) {
}

void Drawer::initialize( ) {
	_title = DrawTitlePtr( new DrawTitle( _data ) );
	set( TITLE, _title );
}

void Drawer::update( ) {
	SCENE scene = _data->getScene( );

	std::map< SCENE, std::vector< BaseDrawPtr > >::iterator ite;
	ite = _drawer.begin( );
	for ( ite; ite != _drawer.end( ); ite++ ) {
		if ( ite->first == scene ) {
			int size = ( int )ite->second.size( );
			for ( int i = 0; i < size; i++ ) {
				ite->second[ i ]->draw( );
			}
		}
	}
}

void Drawer::set( SCENE scene, BaseDrawPtr ptr ) {
	_drawer[ scene ].push_back( ptr );
}