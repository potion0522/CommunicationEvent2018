#include "Base.h"
#include "smart_ptr.h"
#include "Client.h"

PTR( Console );
PTR( GlobalData );
PTR( Client );

class Console : public Base {
public:
	Console( GlobalDataPtr data );
	virtual ~Console( );

public:
	std::string getTag( );
	void initialize( );
	void update( );

private:
	Client::NetWorkData _recv_data;
	Client::NetWorkData _send_data;

	GlobalDataPtr _data;
	ClientPtr _client;
};