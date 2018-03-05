#include "Soundplayer.h"
#include "DxLib.h"
#include "Debug.h"

Soundplayer::Soundplayer( ) {
	setSoundFlag( 1 );
	std::map< int, int >( ).swap( _sounds );
}

Soundplayer::~Soundplayer( ) {
}

std::string Soundplayer::getTag( ) {
	return "SOUNDPLAYER";
}

void Soundplayer::initialize( ) {
	if ( _sounds.size( ) < 1 ) {
		return;
	}

	std::map< int, int >::iterator ite;
	ite = _sounds.begin( );
	for ( ite; ite != _sounds.end( ); ite++ ) {
		if ( CheckSoundMem( ite->second ) == 1 ) {
			StopSoundMem( ite->second );
		}
	}

	std::map< int, int >( ).swap( _sounds );
}

void Soundplayer::finalize( ) {
}

void Soundplayer::update( ) {
	if ( _sounds.size( ) < 1 ) {
		return;
	}

	std::map< int, int >::iterator ite;
	ite = _sounds.begin( );
	for ( ite; ite != _sounds.end( ); ite++ ) {
		if ( CheckSoundMem( ite->second ) == 0 ) {
			ite = _sounds.erase( ite );
		}
		if ( ite == _sounds.end( ) ) {
			return;
		}
	}
}

bool Soundplayer::isPlaying( SoundProperty wav ) const {
	if ( CheckSoundMem( wav.wav ) == TRUE ) {
		return true;
	}
	return false;
}

void Soundplayer::play( SoundProperty wav ) {
	if ( wav.volume > 0 ) {
		ChangeVolumeSoundMem( wav.volume, wav.wav );
	}
	int top_flag = FALSE;
	if ( wav.top ) {
		top_flag = TRUE;
	}
	if ( !wav.isLoop ) {
		PlaySoundMem( wav.wav, DX_PLAYTYPE_BACK, top_flag );
	} else {
		PlaySoundMem( wav.wav, DX_PLAYTYPE_LOOP, top_flag );
	}

	if ( _sounds.find( wav.wav ) == _sounds.end( ) ) {
		_sounds[ wav.wav ] = wav.wav;
	}
}

void Soundplayer::stop( SoundProperty wav ) {
	StopSoundMem( wav.wav );
}