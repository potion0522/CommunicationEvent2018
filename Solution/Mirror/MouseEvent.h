#pragma once

class MouseEvent {
public:
	MouseEvent( );
	virtual ~MouseEvent( );

public:
	void initialize( );
	void update( );

private:
	void calcLeftClick( );

public:
	int getPointX( ) const;
	int getPointY( ) const;
	bool getLeftClick( ) const;

private:
	int _mouse;
	int _click_left;
	struct Point {
		int x;
		int y;
	} _point;
};

