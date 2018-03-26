#pragma once
#include "smart_ptr.h"
#include "const.h"
#include <Windows.h>
#include <string>
#include <vector>

PTR( Image );
PTR( MultiThreadLoad );

struct Png {
	int width;
	int height;
	int png;
};

class Image {
public:
	Image( );
	virtual ~Image( );

public:
	void initialize( );
	Png getPng( IMAGE item, int num ) const;

private:
	void check( int png ) const;
	void inputImage( );
	void inputFileName( std::string path );

private:
	struct Directory {
		std::string name;		//ディレクトリ名
		std::vector< Png > png;	//読み込んだ画像
	};
	bool _image_load;

	std::vector< Directory > _data;
	std::vector< WIN32_FIND_DATA > _file;
	int _dir_num;

	MultiThreadLoadPtr _load;
};