#pragma once
#include "Base.h"
#include "smart_ptr.h"
#include "Field.h"
#include <string>

PTR( Lazer );
PTR( GlobalData );
PTR( Drawer );
PTR( Field );

class Lazer : public Base {
public:
	Lazer( GlobalDataPtr data );
	virtual ~Lazer( );

public:
	std::string getTag( );
	void initialize( );
	void update( );

private:
	void updateUnitVector( Field::Vector vec );
	void updateLazer( );
	void updateStartPos( );
	void convReflectionVector( );

private:
	Field::Vector _start;
	Field::Vector _dir_vec;
	Field::Vector _unit;
	GlobalDataPtr _data;
	DrawerPtr _drawer;
	FieldPtr _field;
};