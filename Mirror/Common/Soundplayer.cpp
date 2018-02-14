#include "Soundplayer.h"
#include "DxLib.h"
#include "Debug.h"

Soundplayer::Soundplayer( ) {
	setSoundFlag( 1 );
}


Soundplayer::~Soundplayer( ) {
}

std::string Soundplayer::getTag( ) {
	return "SOUNDPLAYER";
}

void Soundplayer::initialize( ) {

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
}

void Soundplayer::stop( SoundProperty wav ) {
	StopSoundMem( wav.wav );
}

void Soundplayer::unLoad( SoundProperty wav ) {

}

void Soundplayer::unLoadAll( ) {

}