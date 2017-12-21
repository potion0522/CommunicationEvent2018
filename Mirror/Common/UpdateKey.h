#pragma once
#include "const.h"

class UpdateKey {
public:
	UpdateKey( );
	virtual ~UpdateKey( );

public:
	void initialize( );
	void update( );

public:
	int getKeyState( int num ) const;

private:
	void inputKeyState( );

private:
	int _key[ KEY_MAX ];
};