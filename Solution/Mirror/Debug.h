#pragma once
#include "Base.h"
#include <stdio.h>

class Debug : public Base {
public:
	Debug( );
	virtual ~Debug( );

public:
	void update( );
	void initialize( );

public:
	void error( char* err );

private:
	FILE *_fp;
};

