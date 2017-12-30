#pragma once

const int WIDTH = 1280;
const int HEIGHT = 720;
const double PI = 3.14;
const int KEY_MAX = 256;

const int TCP_PORT = 9850;
const int UDP_PORT = 9900;

//列挙型の要素を増やしたら必ず変更してください
const int IMAGE_DIR_MAX = 1;
const int COLOR_MAX = 6;
const int SCENE_MAX = 3;

enum MACHINE_TYPE {
	CLIENT,
	SERVER,
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
};

enum IMAGE {
	NONE_IMAGE
};
