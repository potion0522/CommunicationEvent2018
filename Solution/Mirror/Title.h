#pragma once
#include "Base.h"
#include <memory>

class Title : public Base {
public:
	Title( std::shared_ptr< class GlobalData > data );
	virtual ~Title( );

public:
	void update( );
	void initialize( );

public:
	ImageProperty* getBackPng( ) const;

private:
	ImageProperty _back;

	std::shared_ptr< class GlobalData > _data;
};

