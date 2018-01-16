#pragma once

const int WIDTH = 1280;
const int HEIGHT = 720;
const double PI = 3.14;
const int KEY_MAX = 256;

const int TCP_PORT = 9850;
const int UDP_PORT = 9900;

//—ñ‹“Œ^‚Ì—v‘f‚ğ‘‚â‚µ‚½‚ç•K‚¸•ÏX‚µ‚Ä‚­‚¾‚³‚¢
const int IMAGE_DIR_MAX = 1;
const int COLOR_MAX = 6;
const int SCENE_MAX = 6;

struct NetWorkData {
	unsigned char order;
	unsigned char x;
	unsigned char y;
	unsigned char angle;
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