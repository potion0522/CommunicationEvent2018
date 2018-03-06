#pragma once
#include "smart_ptr.h"
#include <string>

PTR( Loading2 );

class Loading2 {
public:
	Loading2( std::string message );
	virtual ~Loading2( );

public:
	void update( );
	void add( float add );
	void setMaxLength( float max );
	bool isFinish( ) const;

private:
	bool _fin;
	float _length;
	float _max;
	int _cnt;
	int _font_handle;
	std::string _message;
};

