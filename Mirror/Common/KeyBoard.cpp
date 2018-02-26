#include "KeyBoard.h"
#include "DxLib.h"

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

int KeyBoard::getKeyCode( KEY key_code ) {
	int code = -1;

	switch ( key_code ) {
		case ZERO_KEY	 	 : code = KEY_INPUT_0		  ; break;
		case ONE_KEY	     : code = KEY_INPUT_1		  ; break;
		case TWO_KEY	     : code = KEY_INPUT_2		  ; break;
		case THREE_KEY	 	 : code = KEY_INPUT_3		  ; break;
		case FOUR_KEY	 	 : code = KEY_INPUT_4		  ; break;
		case FIVE_KEY	 	 : code = KEY_INPUT_5		  ; break;
		case SIX_KEY	     : code = KEY_INPUT_6		  ; break;
		case SEVEN_KEY	 	 : code = KEY_INPUT_7		  ; break;
		case EIGHT_KEY	 	 : code = KEY_INPUT_8		  ; break;
		case NINE_KEY	 	 : code = KEY_INPUT_9		  ; break;
		case A_KEY	         : code = KEY_INPUT_A		  ; break;
		case B_KEY	         : code = KEY_INPUT_B		  ; break;
		case C_KEY	         : code = KEY_INPUT_C		  ; break;
		case D_KEY	         : code = KEY_INPUT_D		  ; break;
		case E_KEY	         : code = KEY_INPUT_E		  ; break;
		case F_KEY	         : code = KEY_INPUT_F		  ; break;
		case G_KEY	         : code = KEY_INPUT_G		  ; break;
		case H_KEY	         : code = KEY_INPUT_H		  ; break;
		case I_KEY	         : code = KEY_INPUT_I		  ; break;
		case J_KEY	         : code = KEY_INPUT_J		  ; break;
		case K_KEY	         : code = KEY_INPUT_K		  ; break;
		case L_KEY	         : code = KEY_INPUT_L		  ; break;
		case M_KEY	         : code = KEY_INPUT_M		  ; break;
		case N_KEY	         : code = KEY_INPUT_N		  ; break;
		case O_KEY	         : code = KEY_INPUT_O		  ; break;
		case P_KEY	         : code = KEY_INPUT_P		  ; break;
		case Q_KEY	         : code = KEY_INPUT_Q		  ; break;
		case R_KEY	         : code = KEY_INPUT_R		  ; break;
		case S_KEY	         : code = KEY_INPUT_S		  ; break;
		case T_KEY	         : code = KEY_INPUT_T		  ; break;
		case U_KEY	         : code = KEY_INPUT_U		  ; break;
		case V_KEY	         : code = KEY_INPUT_V		  ; break;
		case W_KEY	         : code = KEY_INPUT_W		  ; break;
		case X_KEY	         : code = KEY_INPUT_X		  ; break;
		case Y_KEY	         : code = KEY_INPUT_Y		  ; break;
		case Z_KEY	         : code = KEY_INPUT_Z		  ; break;
		case MINUS_KEY	     : code = KEY_INPUT_MINUS	  ; break;
		case YEN_KEY	     : code = KEY_INPUT_YEN	      ; break;
		case PREVTRACK_KEY   : code = KEY_INPUT_PREVTRACK ; break;
		case PERIOD_KEY      : code = KEY_INPUT_PERIOD	  ; break;
		case SLASH_KEY	     : code = KEY_INPUT_SLASH	  ; break;
		case NUMPAD0_KEY     : code = KEY_INPUT_NUMPAD0	  ; break;
		case NUMPAD1_KEY     : code = KEY_INPUT_NUMPAD1	  ; break;
		case NUMPAD2_KEY     : code = KEY_INPUT_NUMPAD2	  ; break;
		case NUMPAD3_KEY     : code = KEY_INPUT_NUMPAD3	  ; break;
		case NUMPAD4_KEY     : code = KEY_INPUT_NUMPAD4	  ; break;
		case NUMPAD5_KEY     : code = KEY_INPUT_NUMPAD5	  ; break;
		case NUMPAD6_KEY     : code = KEY_INPUT_NUMPAD6	  ; break;
		case NUMPAD7_KEY     : code = KEY_INPUT_NUMPAD7	  ; break;
		case NUMPAD8_KEY     : code = KEY_INPUT_NUMPAD8	  ; break;
		case NUMPAD9_KEY     : code = KEY_INPUT_NUMPAD9	  ; break;
		case MULTIPLY_KEY    : code = KEY_INPUT_MULTIPLY  ; break;
		case ADD_KEY         : code = KEY_INPUT_ADD 	  ; break;
		case SUBTRACT_KEY    : code = KEY_INPUT_SUBTRACT  ; break;
		case DECIMAL_KEY     : code = KEY_INPUT_DECIMAL	  ; break;
		case DIVIDE_KEY      : code = KEY_INPUT_DIVIDE	  ; break;
		case SPACE_KEY       : code = KEY_INPUT_SPACE     ; break;
		case ENTER_KEY       : code = KEY_INPUT_RETURN    ; break;
		case BACK_KEY        : code = KEY_INPUT_BACK      ; break;
		case ESC_KEY         : code = KEY_INPUT_ESCAPE    ; break;
		case RIGHT_KEY       : code = KEY_INPUT_RIGHT     ; break;
		case LEFT_KEY        : code = KEY_INPUT_LEFT      ; break;
		case UP_KEY          : code = KEY_INPUT_UP	      ; break;
		case DOWN_KEY        : code = KEY_INPUT_DOWN      ; break;
	}
	return code;
}