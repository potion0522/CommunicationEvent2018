#include "Base.h"
#include "smart_ptr.h"
#include "Client.h"

PTR( Console );
PTR( GlobalData );
PTR( Client );
PTR( Drawer );

class Console : public Base {
public:
	Console( GlobalDataPtr data );
	virtual ~Console( );

public:
	std::string getTag( );
	void initialize( );
	void update( );

public:
	bool isRecvingData( ) const;
	Client::NetWorkData getData( );

private:
	void ready( );
	void matching( );
	void battle( );
	bool checkRecvTcp( );

private:
	Client::NetWorkData _recv_data_udp;
	Client::NetWorkData _send_data;

	bool _recving_udp;

	GlobalDataPtr _data;
	ClientPtr _client;
	DrawerPtr _drawer;
};