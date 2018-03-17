#pragma once
#include "smart_ptr.h"
#include <string>
#include <array>

PTR( MultiThreadLoad );


class MultiThreadLoad {
public:
	static const short int LOADING_EFFECT_MAX = 8;

public:
	MultiThreadLoad( std::string message );
	MultiThreadLoad( );
	virtual ~MultiThreadLoad( );

public:
	void update( );
	void setMaxLength( float max );

private:

	enum RESOURCE {
		BACKGROUND,
		LAZER,
		LAZER_LINE,
		PLAYER,
		RESOURCE_MAX
	};

	bool _flag;
	float _max;
	int _cnt;
	int _font_handle;
	std::string _message;
	std::array< int, RESOURCE_MAX > _handles;
	std::array< int, LOADING_EFFECT_MAX > _effect;
};

