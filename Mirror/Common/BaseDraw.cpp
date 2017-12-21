#include "BaseDraw.h"
#include "DxLib.h"

BaseDraw::BaseDraw( ) {
}

BaseDraw::~BaseDraw( ) {
}

void BaseDraw::drawPng( Base::ImageProperty *png ) {
	DrawRotaGraphF( ( float )png->cx, ( float )png->cy, ( float )png->size, ( float )png->angle, png->png, TRUE );
}