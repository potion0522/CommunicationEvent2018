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
			Vector F = { -1 * x, -1 * y };		//進行ベクトル
			Vector N = vec;					//法線ベクトル
			double dot = F.x * N.x + F.y * N.y;

			Vector P =  Vector( );
			P.x = F.x + dot * N.x;
			P.y = F.y + dot * N.y;
			P.x = -1;
			P.y = 1;

			Vector ref = Vector( );
			ref.x = P.x + dot * N.x;
			ref.y = P.y + dot * N.y;
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