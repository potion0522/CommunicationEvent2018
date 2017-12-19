#pragma once
#include "Base.h"
#include "const.h"
#include <memory>

class GlobalData : public Base {
public:
	GlobalData( );
	virtual ~GlobalData( );

public:
	void initialize( );
	void update( );

public:
	int getKeyState( int key ) const;
	int getMouseX( ) const;
	int getMouseY( ) const;
	bool getClickLeft( ) const;
	SCENE getScene( ) const;

public:
	void setPtr( std::shared_ptr< class Debug > ptr );
	void setPtr( std::shared_ptr< class Title > ptr );

	std::shared_ptr< class Image > getImagePtr( ) const;
	std::shared_ptr< class Debug > getDebugPtr( ) const;
	std::shared_ptr< class Title > getTitlePtr( ) const;

private:
	SCENE _scene;

	std::shared_ptr< class UpdateKey > _key;
	std::shared_ptr< class MouseEvent > _mouse;
	std::shared_ptr< class Image > _image;
	std::shared_ptr< class Debug > _debug;
	std::shared_ptr< class Title > _title;
};

