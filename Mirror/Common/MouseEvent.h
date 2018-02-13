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
	void calcRightClick( );

public:
	int getPointX( ) const;
	int getPointY( ) const;
	bool getClickLeft( ) const;
	bool getClickRight( ) const;
	int getClickingLeft( ) const;
	int getClickingRight( ) const;

private:
	int _mouse;
	int _click_left;
	int _click_right;
	struct Point {
		int x;
		int y;
	} _point;
};

