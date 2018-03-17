#pragma once
#include "smart_ptr.h"
#include <string>
#include <array>

PTR( Loading );

const short int LOADING_EFFECT_MAX = 8;

class Loading {
public:
	Loading( std::string message );
	Loading( );
	virtual ~Loading( );

public:
	void update( );
	void add( float add );
	void setMaxLength( float max );
	bool isFin( ) const;

private:

	enum RESOURCE {
		BACKGROUND,
		LAZER,
		LAZER_LINE,
		PLAYER,
		RESOURCE_MAX
	};

	bool _flag;
	bool _fin;
	float _length;
	float _max;
	int _cnt;
	int _font_handle;
	std::string _message;
	std::array< int, RESOURCE_MAX > _handles;
	std::array< int, LOADING_EFFECT_MAX > _effect;
};

