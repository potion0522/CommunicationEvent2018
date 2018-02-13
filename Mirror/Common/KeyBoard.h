#pragma once
#include "smart_ptr.h"
#include <map>
#include <string>

PTR( KeyBoard );

class KeyBoard {
public:
	enum KEY {
		ZERO_KEY	 ,    
		ONE_KEY	     ,    
		TWO_KEY	     ,    
		THREE_KEY	 ,    
		FOUR_KEY	 ,    
		FIVE_KEY	 ,    
		SIX_KEY	     ,    
		SEVEN_KEY	 ,    
		EIGHT_KEY	 ,    
		NINE_KEY	 ,    
		A_KEY	     ,    
		B_KEY	     ,    
		C_KEY	     ,    
		D_KEY	     ,    
		E_KEY	     ,    
		F_KEY	     ,    
		G_KEY	     ,    
		H_KEY	     ,    
		I_KEY	     ,    
		J_KEY	     ,    
		K_KEY	     ,    
		L_KEY	     ,    
		M_KEY	     ,    
		N_KEY	     ,    
		O_KEY	     ,    
		P_KEY	     ,    
		Q_KEY	     ,    
		R_KEY	     ,    
		S_KEY	     ,    
		T_KEY	     ,    
		U_KEY	     ,    
		V_KEY	     ,    
		W_KEY	     ,    
		X_KEY	     ,    
		Y_KEY	     ,    
		Z_KEY	     ,    
		MINUS_KEY	 ,    
		YEN_KEY	     ,
		PREVTRACK_KEY,   
		PERIOD_KEY   ,   
		SLASH_KEY	 ,    
		NUMPAD0_KEY  ,   
		NUMPAD1_KEY  ,   
		NUMPAD2_KEY  ,   
		NUMPAD3_KEY  ,   
		NUMPAD4_KEY  ,   
		NUMPAD5_KEY  ,   
		NUMPAD6_KEY  ,   
		NUMPAD7_KEY  ,   
		NUMPAD8_KEY  ,   
		NUMPAD9_KEY  ,   
		MULTIPLY_KEY ,   
		ADD_KEY      ,   
		SUBTRACT_KEY ,   
		DECIMAL_KEY  ,   
		DIVIDE_KEY   ,   
		SPACE_KEY    ,   
		ENTER_KEY    ,
		BACK_KEY     ,
		ESC_KEY      ,
	};

public:
	KeyBoard( );
	virtual ~KeyBoard( );

public:
	std::string getKeyStr( int key_code );
	int getKeyCode( KEY key_code );

private:
	std::map< int, std::string > _keys;
};

