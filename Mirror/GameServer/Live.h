#pragma once
#include "Field.h"
#include "smart_ptr.h"
#include <string>
#include <vector>

PTR( Live );
PTR( GlobalData );

class Live : public Field {
public:
	Live( GlobalDataPtr data );
	virtual ~Live( );

public:
	std::string getTag( );
	void update( );

protected:
	void drawField( );
	void drawArmament( ) const;
	void drawMirror( ) const;
	void drawPlayer( ) const;
	void drawBackGround( ) const;
};

