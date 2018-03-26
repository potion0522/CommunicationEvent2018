#pragma once
#include "Lazer.h"
#include "smart_ptr.h"

PTR( LiveLazer );
PTR( GlobalData );

class LiveLazer : public Lazer {
public:
	LiveLazer( GlobalDataPtr data );
	virtual ~LiveLazer( );

public:
	void update( );

protected:
	void drawLazerLine( );
	void drawRefrecEffect( );
	void drawDeadEffect( );
};

