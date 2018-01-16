#include "Base.h"
#include "smart_ptr.h"
#include "Server.h"
#include "const.h"

PTR( Connector );
PTR( Server );
PTR( GlobalData );
PTR( Log );

class Connector : public Base {
public:
	Connector( GlobalDataPtr data, LogPtr log );
	~Connector( );

public:
	std::string getTag( );
	void update( );
	void initialize( );

public:
	bool isMatching( ) const;

private:
	void updateConnectState( );
	void updateMatchingState( );
	void sendState( );

private:
	bool _matching;
	bool _sending_state;
	enum CONNECT_STATE {
		NOT_CONNECTING,
		CONNECTING,
	};
	CONNECT_STATE _connect_state[ MACHINE_MAX ];
	SCENE _server_scene;

	GlobalDataPtr _data;
	ServerPtr _server;
	LogPtr _log;
};