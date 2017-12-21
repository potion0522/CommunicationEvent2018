#pragma once
#include "Base.h"
#include "smart_ptr.h"

PTR( Title );
PTR( GlobalData );

class Title : public Base {
public:
	Title( GlobalDataPtr data );
	virtual ~Title( );

public:
	std::string getTag( );
	void update( );
	void initialize( );

public:
	ImageProperty* getBackPng( ) const;

private:
	ImageProperty _back;

	GlobalDataPtr _data;
};

