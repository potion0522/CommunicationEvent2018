#pragma once
#include "smart_ptr.h"
#include <string>
#include <vector>

PTR( LoadCSV );

struct CsvData {
	std::string name;
	std::string info;
};

class LoadCSV {
public:
	LoadCSV( );
	virtual ~LoadCSV( );

public:
	void read( std::vector< CsvData > &list, std::string file_name );
};