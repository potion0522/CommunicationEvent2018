#pragma once

//スクリーンサイズ
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
//解像度
const int WIDTH = 1280;
const int HEIGHT = 720;

const double PI = 3.14;
const int KEY_MAX = 256;
const int PLAYER_NUM = 2;
const int MIRROR_MAX = 6;

const int TCP_PORT = 9850;
const int UDP_PORT = 9900;

struct SendMirrorData {
	bool flag;
	unsigned char player_num;
	unsigned char x;
	unsigned char y;
	unsigned char angle;
};

struct NetWorkData {
	unsigned char player_pos[ PLAYER_NUM ];
	unsigned char lazer_pos;
	unsigned char phase;
	unsigned char order;
	SendMirrorData cts;
	SendMirrorData stc[ PLAYER_NUM ];
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
	PURPLE,
	COLOR_MAX,
};

enum SCENE {
	NONE,
	TITLE,
	CONNECT,
	BATTLE,
	SCENE_MAX,
};

enum IMAGE {
	NONE_IMAGE,
	IMAGE_DIR_MAX
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