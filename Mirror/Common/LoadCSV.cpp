#include "LoadCSV.h"
#include <algorithm>
#include <iterator>

const std::string DIR = "Resources/csv/";

LoadCSV::LoadCSV( ) {
}

LoadCSV::~LoadCSV( ) {
}

void LoadCSV::read( std::vector< CsvData > &list, std::string path ) {
	FILE *fp;
	if ( fopen_s( &fp, ( DIR + path + ".csv" ).c_str( ), "rb" ) != 0 ) {
		return;
	};

	{//データを入力
		char c = '\0';
		int knd = 0;
		CsvData data;
		memset( &data, 0, sizeof( CsvData ) );
		while ( true ) {
			c = fgetc( fp );

			//終端、または行末
			if ( c == '\r' ) {
				continue;
			}

			if ( c == EOF || c == '\n' ) {
				data.info += '\0';
				list.push_back( data );
				memset( &data, 0, sizeof( CsvData ) );

				if ( c == EOF ) {
					break;
				} 
				if ( c == '\n' ) {
					knd = 0;
					continue;
				}
				if ( c == '\r' ) {
					knd = 0;
					continue;
				}
			}

			//カンマで次のメンバへ
			if ( c == ',' ) {
				data.name += '\0';
				knd += 1;
				continue;
			}

			//情報をを入力
			switch ( knd ) {
			case 0: data.name += c;	break;
			case 1: data.info += c;	break;
			default : continue;
			}
		}
	}
}