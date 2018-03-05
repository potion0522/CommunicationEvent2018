#include "Base.h"
#include "smart_ptr.h"
#include "Server.h"
#include "const.h"

PTR( Connector );
PTR( Server );
PTR( GlobalData );
PTR( Log );
PTR( Command );

class Connector : public Base {
public:
	Connector( GlobalDataPtr data, LogPtr log, CommandPtr command );
	~Connector( );

public:
	std::string getTag( );
	void update( );
	void initialize( );
	void finalize( );

public:
	bool isMatching( ) const;

private:
	void updateConnectState( );
	void updateMatchingState( );
	void sendState( );
	void commandExecution( );

private:
	bool _matching;
	bool _sending_state;
	bool _command_matching;
	enum CONNECT_STATE {
		NOT_CONNECTING,
		CONNECTING,
	};
	CONNECT_STATE _connect_state[ MACHINE_MAX ];
	SCENE _server_scene;

	GlobalDataPtr _data;
	ServerPtr _server;
	LogPtr _log;
	CommandPtr _command;
};