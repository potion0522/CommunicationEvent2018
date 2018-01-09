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
			// R = F - 2dot( F, N )N

			//進行ベクトル
			Vector F = { x, y };

			//法線ベクトル
			double length = sqrt( vec.x * vec.x + vec.y * vec.y );
			Vector N = { vec.x / length, vec.y / length };

			double dot = N.x * F.x + N.y * F.y;

			Vector P = Vector( );
			P.x = dot * N.x;
			P.y = dot * N.y;

			Vector ref = Vector(  );
			ref.x = F.x - P.x * 2;
			ref.y = F.y - P.y * 2;

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