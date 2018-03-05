#include "Soundplayer.h"
#include "DxLib.h"
#include "Debug.h"

Soundplayer::Soundplayer( ) {
	setSoundFlag( 1 );
	_past = SoundProperty( );
}

Soundplayer::~Soundplayer( ) {
}

std::string Soundplayer::getTag( ) {
	return "SOUNDPLAYER";
}

void Soundplayer::initialize( ) {
	if ( CheckSoundMem( _past.wav ) == 1 ) {
		StopSoundMem( _past.wav );
	}
}

void Soundplayer::finalize( ) {
}

void Soundplayer::update( ) {
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
	_past.wav = wav.wav;
}

void Soundplayer::stop( SoundProperty wav ) {
	StopSoundMem( wav.wav );
}