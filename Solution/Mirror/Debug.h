#pragma once
#include "Base.h"
#include "Color.h"
#include <memory>
#include <vector>
#include <string>
#include <stdio.h>

class Debug : public Base {
public:
	Debug( );
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

	std::shared_ptr< Color > _color;
};

