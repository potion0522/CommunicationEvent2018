#include "Base.h"

Base::Base( ) :
_flag( 0 ),
_soundflag( 0 ) {
}

Base::~Base( ) {

}

void Base::setFlag( int flag ) {
	_flag = flag;
}

void Base::setSoundFlag( int flag ) {
	_soundflag = flag;
}

int Base::getFlag( ) const {
	return _flag;
}

int Base::getSoundFlag( ) const {
	return _soundflag;
}

void Base::initialize( ) {
	setFlag( 0 );
	setSoundFlag( 0 );
}