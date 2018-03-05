#pragma once
#include "smart_ptr.h"
#include "Base.h"
#include "const.h"
#include <list>
#include <array>

PTR( Soundplayer );

class Soundplayer : public Base {
public:
	Soundplayer( );
	virtual ~Soundplayer( );

public:
	std::string getTag( );
	void initialize( );
	void finalize( );
	void update( );

public:
	bool isPlaying( SoundProperty wav ) const;
	void play( SoundProperty wav );
	void stop( SoundProperty wav );

private:
	std::list< SoundProperty > _sounds;
	SoundProperty _past;
};

