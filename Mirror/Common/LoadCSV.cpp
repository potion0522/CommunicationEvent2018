#include "LoadCSV.h"
#include <iterator>
#include <assert.h>
#include <errno.h>

const std::string DIR = "Resources/csv/";

LoadCSV::LoadCSV( ) {
}

LoadCSV::~LoadCSV( ) {
}

void LoadCSV::read( std::vector< CsvData > &list, std::string path ) {
	FILE *fp;
	errno_t read_csv;
	read_csv = fopen_s( &fp, ( DIR + path + ".csv" ).c_str( ), "rb" );

	assert( !read_csv );

	const int SIZE = 256;
	int cnt = 0;
	{//データを入力
		while ( true ) {
			CsvData data = CsvData( );

			char buf[ SIZE ] = { };
			if ( fgets( buf, SIZE, fp ) == NULL ) {
				break;
			}
			std::string str = buf;

			//タグを入力
			int pos = ( int )str.find_first_of( ',' );
			data.tag = str.substr( 0, pos );
			
			std::string values = str.substr( pos + 1, str.length( ) - pos );
			pos = ( int )values.find( "\r\n" );
			values = values.substr( 0, pos );

			findComma( data.values, values );

			list.push_back( data );
			
			cnt++;
		}

		//char c = '\0';
		//int knd = 0;
		//CsvData data;
		//memset( &data, 0, sizeof( CsvData ) );
		//while ( true ) {
		//	c = fgetc( fp );

		//	//終端、または行末
		//	if ( c == '\r' ) {
		//		continue;
		//	}

		//	if ( c == EOF || c == '\n' ) {
		//		data.value += '\0';
		//		list.push_back( data );
		//		memset( &data, 0, sizeof( CsvData ) );

		//		if ( c == EOF ) {
		//			break;
		//		} 
		//		if ( c == '\n' ) {
		//			knd = 0;
		//			continue;
		//		}
		//		if ( c == '\r' ) {
		//			knd = 0;
		//			continue;
		//		}
		//	}

		//	//カンマで次のメンバへ
		//	if ( c == ',' ) {
		//		data.tag += '\0';
		//		knd += 1;
		//		continue;
		//	}

		//	//情報をを入力
		//	switch ( knd ) {
		//	case 0: data.tag += c; break;
		//	case 1: data.value += c; break;
		//	default : continue;
		//	}
		//}
	}
	fclose( fp );
}

void LoadCSV::findComma( std::vector< std::string > &value, std::string str ) {
	int pos = ( int )str.find_first_of( ',' );

	if ( pos < 0 ) {
		value.push_back( str );
		return;
	}

	std::string cut = str.substr( 0, pos );
	value.push_back( cut );

	std::string next = str.substr( pos + 1, str.length( ) - pos );
	
	pos = ( int )next.find_first_of( ',' );
	if ( pos < 0 ) {
		value.push_back( next );
		return;
	}

	findComma( value, next );
}