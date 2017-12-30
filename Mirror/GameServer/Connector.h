#include "Base.h"
#include "smart_ptr.h"
#include "Server.h"

PTR( Connector );
PTR( Server );
PTR( GlobalData );
PTR( Table );

class Connector : public Base {
public:
	Connector( GlobalDataPtr data );
	~Connector( );

public:
	std::string getTag( );
	void update( );
	void initialize( );

private:
	void updateConnectState( );
	void notConnecting( );
	void connecting( );

private:
	Client::NetWorkData _client_data[ MACHINE_MAX ];
	enum CONNECT_STATE {
		NOT_CONNECTING,
		CONNECTING,
	};
	CONNECT_STATE _connect_state[ MACHINE_MAX ];
	bool _matching;

	GlobalDataPtr _data;
	ServerPtr _server;
	TablePtr _table;
};