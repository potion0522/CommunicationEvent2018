#pragma once
#include "Base.h"
#include "smart_ptr.h"
#include <vector>
#include <string>
#include <stdio.h>

PTR( Debug );
PTR( GlobalData );
PTR( Color );

class Debug : public Base {
public:
	Debug( GlobalDataPtr data );
	virtual ~Debug( );

public:
	void update( );
	void initialize( );

public:
	void error( std::string err );
	void addLog( std::string add );

private:
	void printLog( );
	void initLog( );

private:
	FILE *_fp;
	std::vector< std::string > _log;

	GlobalDataPtr _data;
	ColorPtr _color;
};

