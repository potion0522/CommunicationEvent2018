#include "SetMirror.h"
#include "GlobalData.h"

SetMirror::SetMirror( GlobalDataPtr data ) :
_data( data ) {
}

SetMirror::~SetMirror( ) {
}

std::string SetMirror::getTag( ) {
	return "SETMIRROR";
}

void SetMirror::initialize( ) {
}

void SetMirror::update( ) {

}