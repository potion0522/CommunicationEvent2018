#pragma once
#include "Base.h"

class BaseDraw {
public:
	BaseDraw( );
	virtual ~BaseDraw( );
	virtual void draw( ) = 0;

protected:
	void drawPng( Base::ImageProperty *png );
};

