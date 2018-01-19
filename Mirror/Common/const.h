#pragma once

const int WIDTH = 1280;
const int HEIGHT = 720;
const double PI = 3.14;
const int KEY_MAX = 256;
const int PLAYER_NUM = 2;
const int MIRROR_MAX = 6;

const int TCP_PORT = 9850;
const int UDP_PORT = 9900;

//óÒãìå^ÇÃóvëfÇëùÇ‚ÇµÇΩÇÁïKÇ∏ïœçXÇµÇƒÇ≠ÇæÇ≥Ç¢
const int IMAGE_DIR_MAX = 1;
const int COLOR_MAX = 6;
const int SCENE_MAX = 6;

struct SendMirrorData {
	bool flag;
	unsigned char player_num;
	unsigned char x;
	unsigned char y;
	unsigned char angle;
};

struct NetWorkData {
	unsigned char player_pos[ PLAYER_NUM ];
	unsigned char phase;
	unsigned char order;
	SendMirrorData cts;
	SendMirrorData stc[ MIRROR_MAX ];
	bool fin;
};

enum MACHINE_TYPE {
	CLIENT,
	SERVER,
	TEST,
};

enum COLOR {
	WHITE,
	BLACK,
	RED,
	BLUE,
	WATER,
	YELLOW,
};

enum SCENE {
	ALL = SCENE_MAX,
	NONE = 0,
	TITLE,
	CONNECT,
	BATTLE,
	SET,
	CALC,	
};

enum IMAGE {
	NONE_IMAGE
};

enum MIRROR_ANGLE {
	LEFT,
	RIGHT
};

enum BATTLE_PHASE {
	SET_PLAYER_PHASE,
	SET_MIRROR_PHASE,
	ATTACK_PHASE,
};