#include "Phase.h"

Phase::Phase( short int player_num ) :
_player_num( player_num ) {
	_order = -1;
	_turn = 1;
}

Phase::~Phase( ) {
}

void Phase::setOrder( int order ) {
	_order = order;
}

void Phase::setTurn( int turn ) {
	_turn = turn;
}