#pragma once
#include "BaseDraw.h"
#include "smart_ptr.h"

PTR( DrawTitle );
PTR( GlobalData );
PTR( Title );

class DrawTitle : public BaseDraw {
public:
	DrawTitle( GlobalDataPtr data );
	virtual ~DrawTitle( );

public:
	void draw( );

private:
	GlobalDataPtr _data;
	TitleConstPtr _title;
};

