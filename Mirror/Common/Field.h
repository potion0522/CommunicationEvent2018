#include "smart_ptr.h"
#include "Base.h"
#include <string>

PTR( Field );
PTR( GlobalData );
PTR( Drawer );

class Field : public Base {
public:
	struct Vector { 
		double x;
		double y;
		Vector getReflection( Vector vec ) {
			double var = x * vec.x + y + vec.y;
			Vector ref = Vector( );
			ref.x = x - ( x + 2 * var * vec.x );
			ref.y = y - ( y + 2 * var * vec.y );
			return ref;
		};
	};
public:
	Field( GlobalDataPtr data );
	virtual ~Field( );

public:
	std::string getTag( );
	void initialize( );
	void update( );

public:
	Vector getLazerPoint( ) const;
	Vector getLazerVector( ) const;
	Vector getNormalVector( double x, double y ) const;
	bool isHitMirror( double x, double y ) const;

private:
	GlobalDataPtr _data;
	DrawerPtr _drawer;
};