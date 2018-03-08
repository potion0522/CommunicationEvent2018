#pragma once
#include "smart_ptr.h"
#include <string>
#include <vector>

PTR( Message );
PTR( Drawer );

/*

画面いっぱいに注意事項や、エラー内容を表示するクラスです。
例：
　　サーバーとの接続にしっぱいしました

*/


class Message {
public:
	Message( DrawerPtr drawer );
	virtual ~Message( );

public:
	void initialize( );
	void update( );

public:
	void add( std::string str );

private:
	std::vector< std::string > _message;

	DrawerPtr _drawer;
};

