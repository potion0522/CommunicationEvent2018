#pragma once
#include "smart_ptr.h"
#include <string>
#include <vector>

PTR( LoadCSV );

struct CsvData {
	std::string tag;
	std::vector< std::string > values;
};

class LoadCSV {
public:
	LoadCSV( );
	virtual ~LoadCSV( );

public:
	//QÆ‚Å’l‚ğ’¼Ú“ü‚ê‚é‚½‚ßˆø”•K{
	void read( std::vector< CsvData > &list, std::string file_name );

private:
	void findComma( std::vector< std::string > &value, std::string str );
};