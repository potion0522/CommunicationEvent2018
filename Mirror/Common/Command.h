#pragma once
#include "smart_ptr.h"
#include <string>
#include <vector>

PTR( Command );
PTR( KeyBoard );
PTR( GlobalData );
PTR( Drawer );

class Command {
public:
	Command( GlobalDataPtr data );
	virtual ~Command( );

public:
	void setFlag( int num );
	bool getFlag( ) const;
	void update( );

public:
	std::string getWord( int idx );
	int getWordNum( ) const;
	void clearWord( );

private:
	void setWord( int begin = 0 );

private:
	bool _flag;
	int _pos;
	std::string _command;
	std::vector< std::string > _words;

	KeyBoardPtr _keyboard;
	GlobalDataPtr _data;
	DrawerPtr _drawer;
};

