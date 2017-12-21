#include "Base.h"

Base::Base( ) :
_flag( 0 ) {
}

Base::~Base( ) {

}

void Base::setFlag( int flag ) {
	_flag = flag;
}

int Base::getFlag( ) const {
	return _flag;
}

void Base::initialize( ) {
	setFlag( 0 );
}