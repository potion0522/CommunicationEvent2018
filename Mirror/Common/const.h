#pragma once

//スクリーンサイズ
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
//解像度
const int WIDTH = 1280;
const int HEIGHT = 720;
const float WIDTH_F = ( float )WIDTH;
const float HEIGHT_F = ( float )HEIGHT;

const int FRAME = 60;
const int MINUTE = FRAME * 60;
const int TIME_OUT = FRAME * 5;
const int CONNECT_INTERVAL = FRAME * 3;

const double PI = 3.14;
const short int KEY_MAX = 256;
const short int PLAYER_NUM = 2;
const short int TURN_MAX = 3;
const short int ITEM_POSSESSION_MAX = 3;
const short int ITEM_RECOVERY_TIME = 3;
const short int NUMBER_HANDLE_MAX = 20;
const float FIELD_SIZE_RATE = 1.1f;

const int TCP_PORT = 9850;
const int UDP_PORT = 9900;

const short int GAME_START_BUTTON_IDX = 0;
const short int ITEM_SELECT_BUTTON_IDX = 2;
const short int IP_SETTING_BUTTON_IDX = 4;
const short int ITEM_CANCEL_BUTTON_IDX = 6;
const short int BATTLE_BUTTON_IDX = 7;
const short int RETURN_BUTTON_IDX = 11;
const short int SAVE_BUTTON_IDX = 13;

struct SendMirrorData {
	bool flag;
	unsigned char player_num;
	unsigned char x;
	unsigned char y;
	unsigned char angle;
};

struct NetWorkData {
	bool alive;	//自分の生死
	unsigned char cause_of_death;//負けた理由
	unsigned char winner;
	unsigned char player_pos[ PLAYER_NUM ];
	unsigned char lazer_pos;
	unsigned char phase;
	unsigned char order;
	bool item_flag;
	unsigned char item;
	unsigned char item_user;
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
	BLACK,
	WHITE,
	RED,
	BLUE,
	WATER,
	YELLOW,
	PURPLE,
	COLOR_MAX,
};

enum SCENE {
	ALL,
	NONE_SCENE,
	TITLE,
	SELECT_ITEM,
	IP_SETTING,
	CONNECT,
	BATTLE,
	RESULT,
	SCENE_MAX,
};

enum IMAGE {
	NONE_IMAGE,
	BUTTON_IMAGE,
	CUTIN_STRING_IMAGE,
	CUTIN_BACK_IMAGE,
	BATTLE_IMAGE,
	ITEM_IMAGE,
	PLAYER_IMAGE,
	EFFECT_IMAGE,
	LAZER_IMAGE,
	BAR_IMAGE,
	BACKGROUND_IMAGE,
	TITLE_IMAGE,
	COMMAND_IMAGE,
	TIME_IMAGE,
	NUMBER_IMAGE,
	IMAGE_DIR_MAX
};

enum BACKGROUND_COLOR {
	BACKGROUND_NORMAL,
	BACKGROUND_WHITE
};

enum MIRROR_ANGLE {
	LEFT,
	RIGHT,
	MIRROR_ANGLE_MAX,
	ANGLE_NONE
};

enum BATTLE_PHASE {
	SET_PLAYER_PHASE,
	SET_MIRROR_PHASE,
	ATTACK_PHASE,
	JUDGE_PHASE,
};

enum ITEM {
	LAZER_RESET,
	DOUBLE_MIRROR,
	REVERSE_MIRROR,
	ITEM_MAX,
	ITEM_NONE
};

enum SOUND {
	TITLE_BGM,
	GAME_BGM,
	EFFECT_SOUND,
	SOUND_MAX
};

enum BUTTON_STATE {
	NORMAL,
	CLICKING
};

enum SOUND_EFFECT {
	BUTTON_SE,
	SAVE_SE,
	SELECT_SE,
	BACK_SE,
	WIN_SE,
	LOSE_SE,
	SE_MAX
};

enum CAUSE_OF_DEATH {
	CAUSE_HIT,
	CAUSE_TIME,
	CAUSE_MAX,
};