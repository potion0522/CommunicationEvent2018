#include "Test.h"
#include "GlobalData.h"
#include "const.h"
#include "Drawer.h"

Test::Test( GlobalDataPtr data ) :
_data( data ) {
	initialize( );
}

Test::~Test( ) {
}

void Test::initialize( ) {
	_data->initialize( TEST );
	_drawer = _data->getDrawerPtr( );
	_drawer->initialize( );
}

void Test::update( ) {
	_data->update( );

	//drawLine( );

	_drawer->update( );
}

void Test::drawLine( ) {
	const int ROW = 6;
	const int COL = 6;
	const int PITCH = HEIGHT / 10;
	const int START_POS = WIDTH / 2 - PITCH * COL / 2;
	for ( int i = 0; i < 2; i++ ) {
		for ( int j = 0; j < COL; j++ ) {
			double sx = START_POS + j * PITCH * ( ( i + 1 ) % 2 );
			double sy = PITCH + j * PITCH * ( i % 2 );
			double ex = START_POS + ( sx - START_POS ) * ( ( i + 1 ) % 2 ) + COL * PITCH * ( i % 2 );
			double ey = PITCH + ( sy - PITCH ) * ( i % 2 ) + ROW * PITCH * ( ( i + 1 ) % 2 );
			_drawer->setLine( sx, sy, ex, ey );
			_drawer->setString( false, sx, sy, WHITE, std::to_string( i * ROW + j ) );
		}
		double sx = START_POS + ( i + 1 ) % 2 * COL * PITCH;
		double sy = PITCH + i * ROW * PITCH;
		double ex = sx + i % 2 * COL * PITCH;
		double ey = sy + ( i + 1 ) % 2 * ROW * PITCH;
		_drawer->setLine( sx, sy, ex, ey );
	}
}
