#include "KeyBoard.h"

KeyBoard::KeyBoard( ) {
	_keys[ KEY_INPUT_0 ]			= "0";
	_keys[ KEY_INPUT_1 ]			= "1";
	_keys[ KEY_INPUT_2 ]			= "2";
	_keys[ KEY_INPUT_3 ]			= "3";
	_keys[ KEY_INPUT_4 ]			= "4";
	_keys[ KEY_INPUT_5 ]			= "5";
	_keys[ KEY_INPUT_6 ]			= "6";
	_keys[ KEY_INPUT_7 ]			= "7";
	_keys[ KEY_INPUT_8 ]			= "8";
	_keys[ KEY_INPUT_9 ]			= "9";
	_keys[ KEY_INPUT_A ]			= "A";
	_keys[ KEY_INPUT_B ]			= "B";
	_keys[ KEY_INPUT_C ]			= "C";
	_keys[ KEY_INPUT_D ]			= "D";
	_keys[ KEY_INPUT_E ]			= "E";
	_keys[ KEY_INPUT_F ]			= "F";
	_keys[ KEY_INPUT_G ]			= "G";
	_keys[ KEY_INPUT_H ]			= "H";
	_keys[ KEY_INPUT_I ]			= "I";
	_keys[ KEY_INPUT_J ]			= "J";
	_keys[ KEY_INPUT_K ]			= "K";
	_keys[ KEY_INPUT_L ]			= "L";
	_keys[ KEY_INPUT_M ]			= "M";
	_keys[ KEY_INPUT_N ]			= "N";
	_keys[ KEY_INPUT_O ]			= "O";
	_keys[ KEY_INPUT_P ]			= "P";
	_keys[ KEY_INPUT_Q ]			= "Q";
	_keys[ KEY_INPUT_R ]			= "R";
	_keys[ KEY_INPUT_S ]			= "S";
	_keys[ KEY_INPUT_T ]			= "T";
	_keys[ KEY_INPUT_U ]			= "U";
	_keys[ KEY_INPUT_V ]			= "V";
	_keys[ KEY_INPUT_W ]			= "W";
	_keys[ KEY_INPUT_X ]			= "X";
	_keys[ KEY_INPUT_Y ]			= "Y";
	_keys[ KEY_INPUT_Z ]			= "Z";
	_keys[ KEY_INPUT_MINUS ]		= "-";
	_keys[ KEY_INPUT_YEN ]			= "\\";
	_keys[ KEY_INPUT_PREVTRACK ]	= "^";
	_keys[ KEY_INPUT_PERIOD ]		= ".";
	_keys[ KEY_INPUT_SLASH ]		= "/";
	_keys[ KEY_INPUT_NUMPAD0 ]		= "0";
	_keys[ KEY_INPUT_NUMPAD1 ]		= "1";
	_keys[ KEY_INPUT_NUMPAD2 ]		= "2";
	_keys[ KEY_INPUT_NUMPAD3 ]		= "3";
	_keys[ KEY_INPUT_NUMPAD4 ]		= "4";
	_keys[ KEY_INPUT_NUMPAD5 ]		= "5";
	_keys[ KEY_INPUT_NUMPAD6 ]		= "6";
	_keys[ KEY_INPUT_NUMPAD7 ]		= "7";
	_keys[ KEY_INPUT_NUMPAD8 ]		= "8";
	_keys[ KEY_INPUT_NUMPAD9 ]		= "9";
	_keys[ KEY_INPUT_MULTIPLY ]		= "*";
	_keys[ KEY_INPUT_ADD ]			= "+";
	_keys[ KEY_INPUT_SUBTRACT ]		= "-";
	_keys[ KEY_INPUT_DECIMAL ]		= ".";
	_keys[ KEY_INPUT_DIVIDE ]		= "/";
	_keys[ KEY_INPUT_SPACE ]		= " ";
}

KeyBoard::~KeyBoard( ) {
}

std::string KeyBoard::getKeyStr( int key_code ) {
	if ( _keys.find( key_code ) == _keys.end( ) ) {
		return "";
	}
	return _keys[ key_code ];
}