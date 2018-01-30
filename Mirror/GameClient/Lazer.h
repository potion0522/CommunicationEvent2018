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
public:
	bool isFinish( ) const;

private:
	void updateUnitVector( );
	void updateLazer( );

private:
	bool _fin;
	double _distance;
	Field::Vector _start;
	Field::Vector _dir_vec;
	Field::Vector _unit;

	GlobalDataPtr _data;
	DrawerPtr _drawer;
	FieldPtr _field;
};