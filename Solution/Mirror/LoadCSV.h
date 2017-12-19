#pragma once
#include <string>
#include <vector>

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