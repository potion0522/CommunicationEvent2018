#pragma once
#include "Base.h"
#include "smart_ptr.h"
#include <string>

PTR( SetMirror );
PTR( GlobalData );

class SetMirror : public Base {
public:
	SetMirror( GlobalDataPtr data );
	virtual ~SetMirror( );

public:
	std::string getTag( );
	void initialize( );
	void update( );

private:
	GlobalDataPtr _data;
};

