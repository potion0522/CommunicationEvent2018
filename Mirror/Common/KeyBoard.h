#pragma once
#include "smart_ptr.h"
#include "DxLib.h"
#include <map>
#include <string>

PTR( KeyBoard );

class KeyBoard {
public:
	KeyBoard( );
	virtual ~KeyBoard( );

public:
	std::string getKeyStr( int key_code );

private:
	std::map< int, std::string > _keys;
};

