#pragma once
#include "Field.h"
#include "smart_ptr.h"
#include <string>
#include <vector>

PTR( Live );
PTR( GlobalData );

const int LIVE_ORIGIN_X = WIDTH / 2;
const int LIVE_ORIGIN_Y = HEIGHT / 2;
const int LIVE_GAP_X = LIVE_ORIGIN_X - ( START_POS_X + SQUARE_SIZE * 2 );
const int LIVE_GAP_Y = LIVE_ORIGIN_Y - ( START_POS_Y + SQUARE_SIZE * 2 );

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

