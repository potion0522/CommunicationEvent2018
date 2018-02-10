#include "Command.h"
#include "KeyBoard.h"
#include "GlobalData.h"
#include "Drawer.h"
#include "const.h"
#include <map>

const int COMMAND_X = 20;
const int COMMAND_Y = HEIGHT - 20;

Command::Command( GlobalDataPtr data ) :
_data( data ) {
	_keyboard = KeyBoardPtr( new KeyBoard( ) );
	_drawer = _data->getDrawerPtr( );
	_pos = 0;
}

Command::~Command( ) {
}

void Command::update( ) {
	_drawer->setFrontString( false, COMMAND_X, COMMAND_Y, WHITE, _command );
	clearWord( );

	for ( int i = 0; i < KEY_MAX; i++ ) {
		int key_code = 0;
		if ( _data->getKeyState( i ) == 1 ) {
			_command += _keyboard->getKeyStr( i );
		}
	}

	if ( _data->getKeyState( KEY_INPUT_BACK ) == 1 ) {
		int del = ( int )_command.length( );
		if ( del > 0 ) {
			_command.erase( --del );
		}
	}

	if ( _data->getKeyState( KEY_INPUT_RETURN ) == 1 ) {
		setWord( );
		std::string( ).swap( _command );
	}

	if ( _data->getKeyState( KEY_INPUT_ESCAPE ) == 1 ) {
		setFlag( 0 );
		std::string( ).swap( _command );
		clearWord( );
	}
}

void Command::setFlag( int num ) {
	if ( !num ) {
		_flag = false;
		return;
	}
	_flag = true;
}

bool Command::getFlag( ) const {
	return _flag;
}

std::string Command::getWord( int idx ) {
	return _words[ idx ];
}

int Command::getWordNum( ) const {
	return ( int )_words.size( );
}

void Command::setWord( int begin ) {
	int space_pos = ( int )_command.find( " ", begin );
	if ( space_pos == std::string::npos ) {
		if ( _command.length( ) > 0 ) {
			_words.push_back( _command.substr( begin, _command.length( ) - ( begin - 1 ) ) );
		}
		return;
	}

	int str_length = space_pos;
	if ( begin > 0 ) {
		str_length = space_pos - begin;
	}
	_words.push_back( _command.substr( begin, str_length ) );
	setWord( space_pos + 1 );
}

void Command::clearWord( ) {
	std::vector< std::string >( ).swap( _words );
}