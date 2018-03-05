#pragma once
#include "smart_ptr.h"
#include <string>

PTR( Loading );

class Loading {
public:
	Loading( std::string message );
	virtual ~Loading( );

public:
	void update( );
	void finish( );

private:
	
	static const short int EFFECT_MAX = 8;

	enum RESOURCE {
		BACKGROUND,
		LAZER,
		LAZER_LINE,
		PLAYER,
		RESOURCE_MAX
	};

	int _arg;
	bool _fin;
	std::string _message;
	//std::array< int, RESOURCE_MAX > _handles;
	//std::array< int, EFFECT_MAX > _effect;
};

