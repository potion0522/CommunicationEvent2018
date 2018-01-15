#include "Debug.h"
#include "DxLib.h"
#include "Color.h"
#include "GlobalData.h"
#include "Command.h"
#include <time.h>

const int ACTIVE_CLASS_X = 20;
const int LOG_X = WIDTH - 200;
const int SCENE_X = 20;
const int Y_POS = 20;
const int ACTIVE_CLASS_INIT_Y = 40;

Debug::Debug( GlobalDataPtr data ) :
_data( data ){
}

Debug::~Debug( ) {
}

void Debug::initialize( ) {
	setFlag( 0 );
	_color = ColorPtr( new Color( ) );
	_command = CommandPtr( new Command( _data ) );
	_drawer = _data->getDrawerPtr( );
	_log_y = Y_POS;
	_active_y = ACTIVE_CLASS_INIT_Y;
	initLog( );
	initActiveClass( );
}

std::string Debug::getTag( ) {
	return "DEBUG";
}

void Debug::error( std::string err ) {
	if ( fopen_s( &_fp, "error.txt", "w" ) != 0 ) {
		printfDx( "Error：ファイルオープンに失敗しました。" );
		exit( 0 );
	}
	time_t now = time( NULL );
	const int SIZE = 128;
	char buf[ SIZE ] = { };
	ctime_s( buf, SIZE, &now );
	//改行を消す
	int len = ( int )strlen( buf );
	buf[ len - 1 ] = '\0';

	fprintf_s( _fp, "[ %s ] %s", buf, err.c_str( ) );
	fclose( _fp );

	DxLib_End( );
	exit( 0 );
}

void Debug::update( ) {
	printLog( );
	printActiveClass( );
	printScene( );
	//DrawCircle( WIDTH / 2, HEIGHT /2, 5, _color->getColor( RED ), TRUE );

	if ( _data->getKeyState( KEY_INPUT_RETURN ) == 1 && !_data->getCommandFlag( ) ) {
		_command->setFlag( 1 );
	}

	_data->setCommandFlag( false );
	if ( _command->getFlag( ) ) {
		_command->update( );
		commandExecution( );
		_data->setCommandFlag( true );
	}


	initLog( );
	initActiveClass( );

	_log_y = Y_POS;
	_active_y = ACTIVE_CLASS_INIT_Y;
}

void Debug::commandExecution( ) {
	if ( _command->getWordNum( ) < 3 ) {
		return;
	}
	if ( _command->getWord( 0 ) != "SET" ) {
		return;
	}
	if ( _command->getWord( 1 ) == "SCENE" ) {
		std::string str = _command->getWord( 2 );
		SCENE scene = NONE;
		if ( str == "TITLE" ) {
			scene = TITLE;
		}
		if ( str == "CONNECT" ) {
			scene = CONNECT;
		}
		if ( str == "BATTLE" ) {
			scene = BATTLE;
		}
		if ( str == "SET" ) {
			scene = SET;
		}
		if ( str == "CALC" ) {
			scene = CALC;
		}

		if ( scene != NONE ) {
			_data->setScene( scene );
		}
	}
}

void Debug::printLog( ) {
	int size = ( int )_log.size( );

	for ( int i = 0; i < size; i++ ) {
		DrawFormatString( LOG_X, _log_y, _color->getColor( WHITE ), "Log : %s", _log[ i ].c_str( ) );
		_log_y += Y_POS;
	}
}

void Debug::printActiveClass( ) {
	int size = ( int )_active_class.size( );

	if ( size < 1 ) {
		return;
	}

	for ( int i = 0; i < size; i++ ) {
		DrawFormatString( ACTIVE_CLASS_X, _active_y, _color->getColor( RED ), "Active : %s", _active_class[ i ].c_str( ) );
		_active_y += Y_POS;
	}
}

void Debug::printScene( ) {
	std::string str = "";
	SCENE scene = _data->getScene( );

	switch ( scene ) {
	case NONE	: str = "none"		; break;
	case TITLE	: str = "title"		; break;
	case CONNECT: str = "connect"	; break;
	case BATTLE	: str = "battle"	; break;
	case SET	: str = "set"		; break;
	case CALC	: str = "calc"		; break;
	default		: str = "please set scene"	; break;
	}

	DrawFormatString( SCENE_X, Y_POS, _color->getColor( WHITE ), "SCENE  : %s", str.c_str( ) );
}

void Debug::initLog( ) {
	if ( _log.size( ) < 1 ) {
		return;
	}
	std::vector< std::string >( ).swap( _log );
}

void Debug::addLog( std::string add ) {
	if ( getFlag( ) < 1 ) {
		return;
	}
	_log.push_back( add );
}

void Debug::initActiveClass( ) {
	if ( _active_class.size( ) < 1 ) {
		return;
	}
	std::vector< std::string >( ).swap( _active_class );
}

void Debug::setActiveClass( std::string tag ) {
	if ( getFlag( ) < 1 ) {
		return;
	}
	int size = ( int )_active_class.size( );
	for ( int i = 0; i < size; i++ ) {
		if ( _active_class[ i ] == tag ) {
			return;
		}
	}
	_active_class.push_back( tag );
}

void Debug::setLine( double sx, double sy, double ex, double ey, COLOR col ) {
	_drawer->setLine( sx, sy, ex, ey, col );
}