#include <unistd.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include "function_patcher_gx2.h"
#include "../kernel/syscalls.h"
#include <iostream>
#include "overlay/video/Overlay.h"
#include <string>
using namespace std;

bool firstFrameDone = false;

int tab_page = 0;
bool isModMenu = false;

//符号なし32bit整数(value)をaddressに書き込む関数
void writeU(unsigned int address, unsigned int value) 
{
    unsigned int* ad = (unsigned int*)address;
    *ad = (unsigned int)value;
}

//float型の値をaddressに書き込む関数
void writeFloatH(unsigned int address, float value) 
{
    unsigned int* ad = (unsigned int*)address;
    *ad = *(unsigned int*)&value;
}

//addressに格納されている符号なし32bit型整数を返す関数
unsigned int readU(unsigned int address) 
{
    unsigned int* a = (unsigned int*)address;
    return *a;
}

//addressに格納されているfloat型の値を返す関数
float readFloatH(unsigned int address) 
{
    int v = readU(address);
    float f = *((float*)&v);
    return f;
}

f32 x_Loc = 50;
f32 y_Loc = 50; 
f32 width = 200;

int count_X = 0;
int pressedX = 0;

bool isLogo = false;

f32 xLoc_frame_cheat = 50 + 14*2.04f;
f32 yLoc_frame_cheat = 80 + (42-108)*2.04f;

//背景のパラメータ
f32 xLoc_Bg = 50;
f32 yLoc_Bg = 80;
float alpha_Bg = 255.0f;
f32 width_Bg = 610;

float alpha_tab1 = 0.0f;
float alpha_tab2 = 0.0f;

//全ページ文のパラメータ
f32 xLoc_wpn_p1 = 50 + 30;
f32 yLoc_wpn_p1 = 80 + 69;
float alpha_wpn_p1 = 255.0f;

f32 xLoc_wpn_p2 = 610 + 50 + 30;
f32 yLoc_wpn_p2 = 80 + 69;
float alpha_wpn_p2 = 0.0f;

f32 xLoc_wpn_p3 = 610*2 + 50 + 30;
f32 yLoc_wpn_p3 = 80 + 69;
float alpha_wpn_p3 = 0.0f;

f32 xLoc_wpn_p4 = 610*3 + 50 + 30;
f32 yLoc_wpn_p4 = 80 + 69;
float alpha_wpn_p4 = 0.0f;

f32 xLoc_wpn_p5 = 610*4 + 50 + 30;
f32 yLoc_wpn_p5 = 80 + 69;
float alpha_wpn_p5 = 0.0f;

f32 xLoc_check = 50 + 30;
f32 yLoc_check = 80 + 69;
float alpha_check = 0.0f;

f32 xLoc_frame = 50 + 30;
f32 yLoc_frame = 80 + 69;
float alpha_frame = 0.0f;

bool isFrameMoving = false;

f32 xLoc_RButton = 25 + 610;
f32 yLoc_RButton = 80 + 69 + 110 + 55;
float alpha_RButton = 0.0f;

f32 xLoc_LButton = 25;
f32 yLoc_LButton = 80 + 69 + 110 + 55;
float alpha_LButton = 0.0f;

f32 width_wpn = 110;

unsigned int p1_1[5];
unsigned int p1_2[5];
unsigned int p1_3[5];
unsigned int p1_4[5];

unsigned int p2_1[5];
unsigned int p2_2[5];
unsigned int p2_3[5];
unsigned int p2_4[5];

unsigned int p3_1[5];
unsigned int p3_2[5];
unsigned int p3_3[5];
unsigned int p3_4[5];

unsigned int p4_1[5];
unsigned int p4_2[5];
unsigned int p4_3[5];
unsigned int p4_4[5];

unsigned int p5_1[5];
unsigned int p5_2[5];
unsigned int p5_3[5];
unsigned int p5_4[5];

bool scrollZR = false;
bool scrollZL = false;
bool scrollR = false;
bool scrollL = false;
bool isUp = false;
bool isDown = false;
bool isRight = false;
bool isLeft = false;

int anim_count = 0;

bool isWeapon = true;

//配列用のフレームの座標
int frameLocX = 0;
int frameLocY = 0;

int scrollCount = 0;

//現在のページ
int open_page = 0;

//ボタンの重複押し対策
struct ButtonState
{
	int counter;
	int isPressed1f;
};

ButtonState LButtonState = {0, false};
ButtonState RButtonState = {0, false};
ButtonState DLButtonState = {0, false};
ButtonState DRButtonState = {0, false};
ButtonState DUButtonState = {0, false};
ButtonState DDButtonState = {0, false};

void weapon_menu()
{
	if(isOverlay)
	{
		//背景
        Overlay::draw_Image(xLoc_Bg, yLoc_Bg, alpha_Bg, width_Bg, "WPNbackground.png");

		//背景の表示(フェードイン)
        if (alpha_Bg < 200.0f)
        {
            alpha_Bg += 20.0f;
			alpha_tab1 += 20.0f*1.275f;
        }
	}
    if (isOverlay && isWeapon)
    {
        p1_1[0] = 0x0000041A;
        p1_1[1] = 0x0000041B;
        p1_1[2] = 0x00000438;
        p1_1[3] = 0x00000439;
        p1_1[4] = 0x00000FD3;
        p1_2[0] = 0x00000FD2;
        p1_2[1] = 0x00000FD4;
        p1_2[2] = 0x00000FC8;
        p1_2[3] = 0x00000FC9;
        p1_2[4] = 0x00000492;
        p1_3[0] = 0x00000493;
        p1_3[1] = 0x00000494;
        p1_3[2] = 0x00000488;
        p1_3[3] = 0x00000489;
        p1_3[4] = 0x00000426;
        p1_4[0] = 0x00000424;
        p1_4[1] = 0x00000425;
        p1_4[2] = 0x00000474;
        p1_4[3] = 0x00000475;
        p1_4[4] = 0x00000416;
        p2_1[0] = 0x000007D0;
        p2_1[1] = 0x000007D1;
        p2_1[2] = 0x0000044C;
        p2_1[3] = 0x0000044D;
        p2_1[4] = 0x000003FC;
        p2_2[0] = 0x000003FD;
        p2_2[1] = 0x00000FA0;
        p2_2[2] = 0x00000FA1;
        p2_2[3] = 0x00000FA2;
        p2_2[4] = 0x00000BBC;
        p2_3[0] = 0x00000BCD;
        p2_3[1] = 0x00000410;
        p2_3[2] = 0x00000411;
        p2_3[3] = 0x00000412;
        p2_3[4] = 0x00000FB4;
        p2_4[0] = 0x00000FB6;
        p2_4[1] = 0x00000FB5;
        p2_4[2] = 0x00001388;
        p2_4[3] = 0x00001389;
        p2_4[4] = 0x0000138A;
        p3_1[0] = 0x00000FAA;
        p3_1[1] = 0x00000FAC;
        p3_1[2] = 0x00000FAB;
        p3_1[3] = 0x000007DA;
        p3_1[4] = 0x000007DB;
        p3_2[0] = 0x000007DC;
        p3_2[1] = 0x000007E4;
        p3_2[2] = 0x000007E5;
        p3_2[3] = 0x000007E6;
        p3_2[4] = 0x00000442;
        p3_3[0] = 0x00000443;
        p3_3[1] = 0x00000456;
        p3_3[2] = 0x00000457;
        p3_3[3] = 0x000007F0;
        p3_3[4] = 0x0000139C;
        p3_4[0] = 0x0000139D;
        p3_4[1] = 0x00000BB8;
        p3_4[2] = 0x00000BBA;
        p3_4[3] = 0x00000BB9;
        p3_4[4] = 0x000007EE;
        p4_1[0] = 0x000007EF;
        p4_1[1] = 0x00001392;
        p4_1[2] = 0x00001393;
        p4_1[3] = 0x00001394;
        p4_1[4] = 0x00000415;
        p4_2[0] = 0x00000FAF;
        p4_2[1] = 0x000007DF;
        p4_2[2] = 0x00000BC2;
        p4_2[3] = 0x00000BC3;
        p4_2[4] = 0x0000042E;
        p4_3[0] = 0x0000042F;
        p4_3[1] = 0x00000430;
        p4_3[2] = 0x00000406;
        p4_3[3] = 0x00000408;
        p4_3[4] = 0x00000407;
        p4_4[0] = 0x000003E8;
        p4_4[1] = 0x000003EA;
        p4_4[2] = 0x000003E9;
        p4_4[3] = 0x000007F8;
        p4_4[4] = 0x000007F9;
        p5_1[0] = 0x00000460;
        p5_1[1] = 0x00000461;
        p5_1[2] = 0x000003F3;
        p5_1[3] = 0x0000047E;
        p5_1[4] = 0x0000047F;
        p5_2[0] = 0x00000FBE;
        p5_2[1] = 0x00000FBF;
        p5_2[2] = 0x0000046A;
        p5_2[3] = 0x0000046B;
        p5_2[4] = 0x0000046C;
        p5_3[0] = 0x000003F2;

        if (alpha_frame < 255.0f)
        {
            alpha_frame += 25.5f;
            alpha_RButton += 25.5f;
            alpha_LButton += 25.5f;
        }

		//もろもろのレンダリング
		Overlay::draw_Image(xLoc_check, yLoc_check, alpha_check, 110, "check.png");
		Overlay::draw_Image(xLoc_wpn_p1 + 110 * 0, yLoc_wpn_p1 + 110 * 0, alpha_wpn_p1, 110, "Wst_Shot_Gravity00.png");
		Overlay::draw_Image(xLoc_wpn_p1 + 110 * 1, yLoc_wpn_p1 + 110 * 0, alpha_wpn_p1, 110, "Wst_Shot_Gravity01.png");
		Overlay::draw_Image(xLoc_wpn_p1 + 110 * 2, yLoc_wpn_p1 + 110 * 0, alpha_wpn_p1, 110, "Wst_Shot_Heavy00.png");
		Overlay::draw_Image(xLoc_wpn_p1 + 110 * 3, yLoc_wpn_p1 + 110 * 0, alpha_wpn_p1, 110, "Wst_Shot_Heavy01.png");
		Overlay::draw_Image(xLoc_wpn_p1 + 110 * 4, yLoc_wpn_p1 + 110 * 0, alpha_wpn_p1, 110, "Wst_Charge_Light01.png");
		Overlay::draw_Image(xLoc_wpn_p1 + 110 * 0, yLoc_wpn_p1 + 110 * 1, alpha_wpn_p1, 110, "Wst_Charge_Light00.png");
		Overlay::draw_Image(xLoc_wpn_p1 + 110 * 1, yLoc_wpn_p1 + 110 * 1, alpha_wpn_p1, 110, "Wst_Charge_Light02.png");
		Overlay::draw_Image(xLoc_wpn_p1 + 110 * 2, yLoc_wpn_p1 + 110 * 1, alpha_wpn_p1, 110, "Wst_Charge_LongScp00.png");
		Overlay::draw_Image(xLoc_wpn_p1 + 110 * 3, yLoc_wpn_p1 + 110 * 1, alpha_wpn_p1, 110, "Wst_Charge_LongScp01.png");
		Overlay::draw_Image(xLoc_wpn_p1 + 110 * 4, yLoc_wpn_p1 + 110 * 1, alpha_wpn_p1, 110, "Wst_Shot_TripleMiddle00.png");
		Overlay::draw_Image(xLoc_wpn_p1 + 110 * 0, yLoc_wpn_p1 + 110 * 2, alpha_wpn_p1, 110, "Wst_Shot_TripleMiddle01.png");
		Overlay::draw_Image(xLoc_wpn_p1 + 110 * 1, yLoc_wpn_p1 + 110 * 2, alpha_wpn_p1, 110, "Wst_Shot_TripleMiddle02.png");
		Overlay::draw_Image(xLoc_wpn_p1 + 110 * 2, yLoc_wpn_p1 + 110 * 2, alpha_wpn_p1, 110, "Wst_Shot_TripleQuick00.png");
		Overlay::draw_Image(xLoc_wpn_p1 + 110 * 3, yLoc_wpn_p1 + 110 * 2, alpha_wpn_p1, 110, "Wst_Shot_TripleQuick01.png");
		Overlay::draw_Image(xLoc_wpn_p1 + 110 * 4, yLoc_wpn_p1 + 110 * 2, alpha_wpn_p1, 110, "Wst_Shot_QuickMiddle02.png");
		Overlay::draw_Image(xLoc_wpn_p1 + 110 * 0, yLoc_wpn_p1 + 110 * 3, alpha_wpn_p1, 110, "Wst_Shot_QuickMiddle00.png");
		Overlay::draw_Image(xLoc_wpn_p1 + 110 * 1, yLoc_wpn_p1 + 110 * 3, alpha_wpn_p1, 110, "Wst_Shot_QuickMiddle01.png");
		Overlay::draw_Image(xLoc_wpn_p1 + 110 * 2, yLoc_wpn_p1 + 110 * 3, alpha_wpn_p1, 110, "Wst_Shot_ExpLightLong00.png");
		Overlay::draw_Image(xLoc_wpn_p1 + 110 * 3, yLoc_wpn_p1 + 110 * 3, alpha_wpn_p1, 110, "Wst_Shot_ExpLightLong01.png");
		Overlay::draw_Image(xLoc_wpn_p1 + 110 * 4, yLoc_wpn_p1 + 110 * 3, alpha_wpn_p1, 110, "Wst_Shot_NormalOct.png");
		Overlay::draw_Image(xLoc_wpn_p2 + 110 * 0, yLoc_wpn_p2 + 110 * 0, alpha_wpn_p2, 110, "Wst_Roller_Compact00.png");
		Overlay::draw_Image(xLoc_wpn_p2 + 110 * 1, yLoc_wpn_p2 + 110 * 0, alpha_wpn_p2, 110, "Wst_Roller_Compact01.png");
		Overlay::draw_Image(xLoc_wpn_p2 + 110 * 2, yLoc_wpn_p2 + 110 * 0, alpha_wpn_p2, 110, "Wst_Shot_Long00.png");
		Overlay::draw_Image(xLoc_wpn_p2 + 110 * 3, yLoc_wpn_p2 + 110 * 0, alpha_wpn_p2, 110, "Wst_Shot_Long01.png");
		Overlay::draw_Image(xLoc_wpn_p2 + 110 * 4, yLoc_wpn_p2 + 110 * 0, alpha_wpn_p2, 110, "Wst_Shot_Precision00.png");
		Overlay::draw_Image(xLoc_wpn_p2 + 110 * 0, yLoc_wpn_p2 + 110 * 1, alpha_wpn_p2, 110, "Wst_Shot_Precision01.png");
		Overlay::draw_Image(xLoc_wpn_p2 + 110 * 1, yLoc_wpn_p2 + 110 * 1, alpha_wpn_p2, 110, "Wst_Charge_Quick00.png");
		Overlay::draw_Image(xLoc_wpn_p2 + 110 * 2, yLoc_wpn_p2 + 110 * 1, alpha_wpn_p2, 110, "Wst_Charge_Quick01.png");
		Overlay::draw_Image(xLoc_wpn_p2 + 110 * 3, yLoc_wpn_p2 + 110 * 1, alpha_wpn_p2, 110, "Wst_Charge_Quick02.png");
		Overlay::draw_Image(xLoc_wpn_p2 + 110 * 4, yLoc_wpn_p2 + 110 * 1, alpha_wpn_p2, 110, "Wst_BigBall_Launcher00.png");
		Overlay::draw_Image(xLoc_wpn_p2 + 110 * 0, yLoc_wpn_p2 + 110 * 2, alpha_wpn_p2, 110, "Wst_BigBall_Launcher01.png");
		Overlay::draw_Image(xLoc_wpn_p2 + 110 * 1, yLoc_wpn_p2 + 110 * 2, alpha_wpn_p2, 110, "Wst_Shot_Normal00.png");
		Overlay::draw_Image(xLoc_wpn_p2 + 110 * 2, yLoc_wpn_p2 + 110 * 2, alpha_wpn_p2, 110, "Wst_Shot_Normal01.png");
		Overlay::draw_Image(xLoc_wpn_p2 + 110 * 3, yLoc_wpn_p2 + 110 * 2, alpha_wpn_p2, 110, "Wst_Shot_Normal02.png");
		Overlay::draw_Image(xLoc_wpn_p2 + 110 * 4, yLoc_wpn_p2 + 110 * 2, alpha_wpn_p2, 110, "Wst_Charge_NormalScp00.png");
		Overlay::draw_Image(xLoc_wpn_p2 + 110 * 0, yLoc_wpn_p2 + 110 * 3, alpha_wpn_p2, 110, "Wst_Charge_NormalScp02.png");
		Overlay::draw_Image(xLoc_wpn_p2 + 110 * 1, yLoc_wpn_p2 + 110 * 3, alpha_wpn_p2, 110, "Wst_Charge_NormalScp01.png");
		Overlay::draw_Image(xLoc_wpn_p2 + 110 * 2, yLoc_wpn_p2 + 110 * 3, alpha_wpn_p2, 110, "Wst_Gatling_Quick00.png");
		Overlay::draw_Image(xLoc_wpn_p2 + 110 * 3, yLoc_wpn_p2 + 110 * 3, alpha_wpn_p2, 110, "Wst_Gatling_Quick01.png");
		Overlay::draw_Image(xLoc_wpn_p2 + 110 * 4, yLoc_wpn_p2 + 110 * 3, alpha_wpn_p2, 110, "Wst_Gatling_Quick02.png");
		Overlay::draw_Image(xLoc_wpn_p3 + 110 * 0, yLoc_wpn_p3 + 110 * 0, alpha_wpn_p3, 110, "Wst_Charge_Normal00.png");
		Overlay::draw_Image(xLoc_wpn_p3 + 110 * 1, yLoc_wpn_p3 + 110 * 0, alpha_wpn_p3, 110, "Wst_Charge_Normal02.png");
		Overlay::draw_Image(xLoc_wpn_p3 + 110 * 2, yLoc_wpn_p3 + 110 * 0, alpha_wpn_p3, 110, "Wst_Charge_Normal01.png");
		Overlay::draw_Image(xLoc_wpn_p3 + 110 * 3, yLoc_wpn_p3 + 110 * 0, alpha_wpn_p3, 110, "Wst_Roller_Normal00.png");
		Overlay::draw_Image(xLoc_wpn_p3 + 110 * 4, yLoc_wpn_p3 + 110 * 0, alpha_wpn_p3, 110, "Wst_Roller_Normal01.png");
		Overlay::draw_Image(xLoc_wpn_p3 + 110 * 0, yLoc_wpn_p3 + 110 * 1, alpha_wpn_p3, 110, "Wst_Roller_Normal02.png");
		Overlay::draw_Image(xLoc_wpn_p3 + 110 * 1, yLoc_wpn_p3 + 110 * 1, alpha_wpn_p3, 110, "Wst_Roller_Heavy00.png");
		Overlay::draw_Image(xLoc_wpn_p3 + 110 * 2, yLoc_wpn_p3 + 110 * 1, alpha_wpn_p3, 110, "Wst_Roller_Heavy01.png");
		Overlay::draw_Image(xLoc_wpn_p3 + 110 * 3, yLoc_wpn_p3 + 110 * 1, alpha_wpn_p3, 110, "Wst_Roller_Heavy02.png");
		Overlay::draw_Image(xLoc_wpn_p3 + 110 * 4, yLoc_wpn_p3 + 110 * 1, alpha_wpn_p3, 110, "Wst_Shot_QuickLong00.png");
		Overlay::draw_Image(xLoc_wpn_p3 + 110 * 0, yLoc_wpn_p3 + 110 * 2, alpha_wpn_p3, 110, "Wst_Shot_QuickLong01.png");
		Overlay::draw_Image(xLoc_wpn_p3 + 110 * 1, yLoc_wpn_p3 + 110 * 2, alpha_wpn_p3, 110, "Wst_Shot_ExpShort00.png");
		Overlay::draw_Image(xLoc_wpn_p3 + 110 * 2, yLoc_wpn_p3 + 110 * 2, alpha_wpn_p3, 110, "Wst_Shot_ExpShort01.png");
		Overlay::draw_Image(xLoc_wpn_p3 + 110 * 3, yLoc_wpn_p3 + 110 * 2, alpha_wpn_p3, 110, "Wst_Roller_BrushMini02.png");
		Overlay::draw_Image(xLoc_wpn_p3 + 110 * 4, yLoc_wpn_p3 + 110 * 2, alpha_wpn_p3, 110, "Wst_Gatling_Hyper00.png");
		Overlay::draw_Image(xLoc_wpn_p3 + 110 * 0, yLoc_wpn_p3 + 110 * 3, alpha_wpn_p3, 110, "Wst_Gatling_Hyper01.png");
		Overlay::draw_Image(xLoc_wpn_p3 + 110 * 1, yLoc_wpn_p3 + 110 * 3, alpha_wpn_p3, 110, "Wst_BigBall_Strong00.png");
		Overlay::draw_Image(xLoc_wpn_p3 + 110 * 2, yLoc_wpn_p3 + 110 * 3, alpha_wpn_p3, 110, "Wst_BigBall_Strong02.png");
		Overlay::draw_Image(xLoc_wpn_p3 + 110 * 3, yLoc_wpn_p3 + 110 * 3, alpha_wpn_p3, 110, "Wst_BigBall_Strong01.png");
		Overlay::draw_Image(xLoc_wpn_p3 + 110 * 4, yLoc_wpn_p3 + 110 * 3, alpha_wpn_p3, 110, "Wst_Roller_BrushMini00.png");
		Overlay::draw_Image(xLoc_wpn_p4 + 110 * 0, yLoc_wpn_p4 + 110 * 0, alpha_wpn_p4, 110, "Wst_Roller_BrushMini01.png");
		Overlay::draw_Image(xLoc_wpn_p4 + 110 * 1, yLoc_wpn_p4 + 110 * 0, alpha_wpn_p4, 110, "Wst_Gatling_Standard00.png");
		Overlay::draw_Image(xLoc_wpn_p4 + 110 * 2, yLoc_wpn_p4 + 110 * 0, alpha_wpn_p4, 110, "Wst_Gatling_Standard01.png");
		Overlay::draw_Image(xLoc_wpn_p4 + 110 * 3, yLoc_wpn_p4 + 110 * 0, alpha_wpn_p4, 110, "Wst_Gatling_Standard02.png");
		Overlay::draw_Image(xLoc_wpn_p4 + 110 * 4, yLoc_wpn_p4 + 110 * 0, alpha_wpn_p4, 110, "Wst_Shot_NormalH.png");
		Overlay::draw_Image(xLoc_wpn_p4 + 110 * 0, yLoc_wpn_p4 + 110 * 1, alpha_wpn_p4, 110, "Wst_Charge_NormalH.png");
		Overlay::draw_Image(xLoc_wpn_p4 + 110 * 1, yLoc_wpn_p4 + 110 * 1, alpha_wpn_p4, 110, "Wst_Roller_NormalH.png");
		Overlay::draw_Image(xLoc_wpn_p4 + 110 * 2, yLoc_wpn_p4 + 110 * 1, alpha_wpn_p4, 110, "Wst_BigBall_Diffusion00.png");
		Overlay::draw_Image(xLoc_wpn_p4 + 110 * 3, yLoc_wpn_p4 + 110 * 1, alpha_wpn_p4, 110, "Wst_BigBall_Diffusion01.png");
		Overlay::draw_Image(xLoc_wpn_p4 + 110 * 4, yLoc_wpn_p4 + 110 * 1, alpha_wpn_p4, 110, "Wst_Shot_Expert00.png");
		Overlay::draw_Image(xLoc_wpn_p4 + 110 * 0, yLoc_wpn_p4 + 110 * 2, alpha_wpn_p4, 110, "Wst_Shot_Expert01.png");
		Overlay::draw_Image(xLoc_wpn_p4 + 110 * 1, yLoc_wpn_p4 + 110 * 2, alpha_wpn_p4, 110, "Wst_Shot_Expert02.png");
		Overlay::draw_Image(xLoc_wpn_p4 + 110 * 2, yLoc_wpn_p4 + 110 * 2, alpha_wpn_p4, 110, "Wst_Shot_Blaze00.png");
		Overlay::draw_Image(xLoc_wpn_p4 + 110 * 3, yLoc_wpn_p4 + 110 * 2, alpha_wpn_p4, 110, "Wst_Shot_Blaze02.png");
		Overlay::draw_Image(xLoc_wpn_p4 + 110 * 4, yLoc_wpn_p4 + 110 * 2, alpha_wpn_p4, 110, "Wst_Shot_Blaze01.png");
		Overlay::draw_Image(xLoc_wpn_p4 + 110 * 0, yLoc_wpn_p4 + 110 * 3, alpha_wpn_p4, 110, "Wst_Shot_Short00.png");
		Overlay::draw_Image(xLoc_wpn_p4 + 110 * 1, yLoc_wpn_p4 + 110 * 3, alpha_wpn_p4, 110, "Wst_Shot_Short02.png");
		Overlay::draw_Image(xLoc_wpn_p4 + 110 * 2, yLoc_wpn_p4 + 110 * 3, alpha_wpn_p4, 110, "Wst_Shot_Short01.png");
		Overlay::draw_Image(xLoc_wpn_p4 + 110 * 3, yLoc_wpn_p4 + 110 * 3, alpha_wpn_p4, 110, "Wst_Roller_BrushNormal00.png");
		Overlay::draw_Image(xLoc_wpn_p4 + 110 * 4, yLoc_wpn_p4 + 110 * 3, alpha_wpn_p4, 110, "Wst_Roller_BrushNormal01.png");
		Overlay::draw_Image(xLoc_wpn_p5 + 110 * 0, yLoc_wpn_p5 + 110 * 0, alpha_wpn_p5, 110, "Wst_Shot_ExpMiddle00.png");
		Overlay::draw_Image(xLoc_wpn_p5 + 110 * 1, yLoc_wpn_p5 + 110 * 0, alpha_wpn_p5, 110, "Wst_Shot_ExpMiddle01.png");
		Overlay::draw_Image(xLoc_wpn_p5 + 110 * 2, yLoc_wpn_p5 + 110 * 0, alpha_wpn_p5, 110, "Wst_Shot_First01.png");
		Overlay::draw_Image(xLoc_wpn_p5 + 110 * 3, yLoc_wpn_p5 + 110 * 0, alpha_wpn_p5, 110, "Wst_Shot_ExpLight00.png");
		Overlay::draw_Image(xLoc_wpn_p5 + 110 * 4, yLoc_wpn_p5 + 110 * 0, alpha_wpn_p5, 110, "Wst_Shot_ExpLight01.png");
		Overlay::draw_Image(xLoc_wpn_p5 + 110 * 0, yLoc_wpn_p5 + 110 * 1, alpha_wpn_p5, 110, "Wst_Charge_Long00.png");
		Overlay::draw_Image(xLoc_wpn_p5 + 110 * 1, yLoc_wpn_p5 + 110 * 1, alpha_wpn_p5, 110, "Wst_Charge_Long01.png");
		Overlay::draw_Image(xLoc_wpn_p5 + 110 * 2, yLoc_wpn_p5 + 110 * 1, alpha_wpn_p5, 110, "Wst_Shot_ExpLong00.png");
		Overlay::draw_Image(xLoc_wpn_p5 + 110 * 3, yLoc_wpn_p5 + 110 * 1, alpha_wpn_p5, 110, "Wst_Shot_ExpLong01.png");
		Overlay::draw_Image(xLoc_wpn_p5 + 110 * 4, yLoc_wpn_p5 + 110 * 1, alpha_wpn_p5, 110, "Wst_Shot_ExpLong02.png");
		Overlay::draw_Image(xLoc_wpn_p5 + 110 * 0, yLoc_wpn_p5 + 110 * 2, alpha_wpn_p5, 110, "Wst_Shot_First00.png");
		Overlay::draw_Image(xLoc_frame, yLoc_frame, alpha_frame, 110, "frame.png");
		Overlay::draw_Image(xLoc_RButton, yLoc_RButton, alpha_RButton, 80, "RButton.png");
		Overlay::draw_Image(xLoc_LButton, yLoc_LButton, alpha_LButton, 80, "LButton.png");

/********************************************************************************************************************/

		//もしLボタンを押したとき
		if((readU(/*アドレス*/) / 0x1000) % 0x10 == 0x1)
		{
			//1フレーム目ならフラグを立てる
			//1フレーム目以外ならフラグを下げる
			if(LButtonState.counter == 0)
				LButtonState.isPressed1f = true;
			else
				LButtonState.isPressed1f = false;
			
			//カウンターを1fずつ値を増やす
			LButtonState.counter++;
		}
		else
		{
			//ボタンを離したらカウンターリセット
			LButtonState.counter = 0;
		}

		//もしRボタンを押したとき
		if((readU(/*アドレス*/) / 0x10000) % 0x10 == 0x2)
		{
			//1フレーム目ならフラグを立てる
			//1フレーム目以外ならフラグを下げる
			if(RButtonState.counter == 0)
				RButtonState.isPressed1f = true;
			else
				RButtonState.isPressed1f = false;
			
			//カウンターを1fずつ値を増やす
			RButtonState.counter++;
		}
		else
		{
			//ボタンを離したらカウンターリセット
			RButtonState.counter = 0;
		}

		//もし十字左ボタンを押したとき
		if((readU(/*アドレス*/) / 0x10) % 0x10 == 0x1)
		{
			//1フレーム目ならフラグを立てる
			//1フレーム目以外ならフラグを下げる
			if(DLButtonState.counter == 0)
				DLButtonState.isPressed1f = true;
			else
				DLButtonState.isPressed1f = false;
			
			//カウンターを1fずつ値を増やす
			DLButtonState.counter++;
		}
		else
		{
			//ボタンを離したらカウンターリセット
			DLButtonState.counter = 0;
		}

		//もし十字右ボタンを押したとき
		if((readU(/*アドレス*/) / 0x10) % 0x10 == 0x2)
		{
			//1フレーム目ならフラグを立てる
			//1フレーム目以外ならフラグを下げる
			if(DRButtonState.counter == 0)
				DRButtonState.isPressed1f = true;
			else
				DRButtonState.isPressed1f = false;
			
			//カウンターを1fずつ値を増やす
			DRButtonState.counter++;
		}
		else
		{
			//ボタンを離したらカウンターリセット
			DRButtonState.counter = 0;
		}

		//もし十字上ボタンを押したとき
		if((readU(/*アドレス*/) / 0x1) % 0x10 == 0x4)
		{
			//1フレーム目ならフラグを立てる
			//1フレーム目以外ならフラグを下げる
			if(DUButtonState.counter == 0)
				DUButtonState.isPressed1f = true;
			else
				DUButtonState.isPressed1f = false;
			
			//カウンターを1fずつ値を増やす
			DUButtonState.counter++;
		}
		else
		{
			//ボタンを離したらカウンターリセット
			DUButtonState.counter = 0;
		}

		//もし十字下ボタンを押したとき
		if((readU(/*アドレス*/) / 0x1) % 0x10 == 0x8)
		{
			//1f目ならフラグを立てる
			//1f目以外ならフラグを下げる
			if(DDButtonState.counter == 0)
				DDButtonState.isPressed1f = true;
			else
				DDButtonState.isPressed1f = false;
			
			//カウンターを1fずつ値を増やす
			DDButtonState.counter++;
		}
		else
		{
			//ボタンを離したらカウンターリセット
			DDButtonState.counter = 0;
		}

		//十字キーでカーソル操作
		//他のカーソル移動と動作が被らないように、!(bool)を条件文に追加
		if(DUButtonState.isPressed1f && !scrollZL && !scrollZR && !scrollR && !scrollL && !isDown && !isRight && !isLeft && frameLocY != 0)
		{
			isUp = true;
		}
		if(DDButtonState.isPressed1f && !scrollZL && !scrollZR && !scrollR && !scrollL && !isUp && !isRight && !isLeft && frameLocY != 3)
		{
			isDown = true;
		}
		if(DRButtonState.isPressed1f && !scrollZL && !scrollZR && !scrollR && !scrollL && !isUp && !isDown && !isLeft && frameLocX != 4)
		{
			isRight = true;
		}
		if(DLButtonState.isPressed1f && !scrollZL && !scrollZR && !scrollR && !scrollL && !isUp && !isDown && !isRight && frameLocX != 0)
		{
			isLeft = true;
		}
		if(LButtonState.isPressed1f && !scrollZL && !scrollZR && !scrollR && !isUp && !isDown && !isRight && !isLeft && open_page != 0)
		{
			scrollL = true;
		}
		if(RButtonState.isPressed1f && !scrollZL && !scrollZR && !scrollL && !isUp && !isDown && !isRight && !isLeft && open_page != 4)
		{
			scrollR = true;
		}

/********************************************************************************************************************************************/

		//選択した武器データを書き込む
		if(((readU(/*アドレス*/) / 0x1000) % 0x10 == 0x2) && !isFrameMoving)
		{
			xLoc_check = xLoc_frame;
			yLoc_check = yLoc_frame;
			alpha_check = 255.5f;

			if(open_page == 0)
			{
				if(frameLocY == 0)
				{
					writeU(/*アドレス*/, p1_1[frameLocX]);
				}
				if(frameLocY == 1)
				{
					writeU(/*アドレス*/, p1_2[frameLocX]);
				}
				if(frameLocY == 2)
				{
					writeU(/*アドレス*/, p1_3[frameLocX]);
				}
				if(frameLocY == 3)
				{
					writeU(/*アドレス*/, p1_4[frameLocX]);
				}
			}

			if(open_page == 1)
			{
				if(frameLocY == 0)
				{
					writeU(/*アドレス*/, p2_1[frameLocX]);
				}
				if(frameLocY == 1)
				{
					writeU(/*アドレス*/, p2_2[frameLocX]);
				}
				if(frameLocY == 2)
				{
					writeU(/*アドレス*/, p2_3[frameLocX]);
				}
				if(frameLocY == 3)
				{
					writeU(/*アドレス*/, p2_4[frameLocX]);
				}
			}

			if(open_page == 2)
			{
				if(frameLocY == 0)
				{
					writeU(/*アドレス*/, p3_1[frameLocX]);
				}
				if(frameLocY == 1)
				{
					writeU(/*アドレス*/, p3_2[frameLocX]);
				}
				if(frameLocY == 2)
				{
					writeU(/*アドレス*/, p3_3[frameLocX]);
				}
				if(frameLocY == 3)
				{
					writeU(/*アドレス*/, p3_4[frameLocX]);
				}
			}

			if(open_page == 3)
			{
				if(frameLocY == 0)
				{
					writeU(/*アドレス*/, p4_1[frameLocX]);
				}
				if(frameLocY == 1)
				{
					writeU(/*アドレス*/, p4_2[frameLocX]);
				}
				if(frameLocY == 2)
				{
					writeU(/*アドレス*/, p4_3[frameLocX]);
				}
				if(frameLocY == 3)
				{
					writeU(/*アドレス*/, p4_4[frameLocX]);
				}
			}

			if(open_page == 4)
			{
				if(frameLocY == 0)
				{
					writeU(/*アドレス*/, p5_1[frameLocX]);
				}
				if(frameLocY == 1)
				{
					writeU(/*アドレス*/, p5_2[frameLocX]);
				}
				if(frameLocY == 2)
				{
					writeU(/*アドレス*/, p5_3[frameLocX]);
				}
				if(frameLocY == 3)
				{
					writeU(/*アドレス*/, p5_4[frameLocX]);
				}
			}

		}

		if(isUp)
		{
			//forやwhileでループさせると1フレームで同時に処理されてしまうためif文で1フレームずつ処理させるように
			if(anim_count < 10)
			{
				yLoc_frame -= 11; 
				anim_count++;
				isFrameMoving = true;
			}
			else
			{
				isUp = false;
				frameLocY--;
				anim_count = 0;
				isFrameMoving = false;
			}
		}

		if(isDown)
		{
			if(anim_count < 10)
			{
				yLoc_frame += 11; 
				anim_count++;
				isFrameMoving = true;
			}
			else
			{
				isDown = false;
				frameLocY++;
				anim_count = 0;
				isFrameMoving = false;
			}
		}

		if(isRight)
		{
			if(anim_count < 10)
			{
				xLoc_frame += 11; 
				anim_count++;
				isFrameMoving = true;
			}
			else
			{
				isRight = false;
				frameLocX++;
				anim_count = 0;
				isFrameMoving = false;
			}
		}

		if(isLeft)
		{
			//forとかwhileでループさせると同時に処理されてしまう
			//if文でループさせるしかない
			if(anim_count < 10)
			{
				xLoc_frame -= 11; 
				anim_count++;
				isFrameMoving = true;
			}
			else
			{
				isLeft = false;
				frameLocX--;
				anim_count = 0;
				isFrameMoving = false;
			}
		}


		//右のページに移動
		if(scrollR && !scrollL && open_page != 4)
		{
			if(anim_count < 10)
			{
				xLoc_wpn_p1 -= 61;
				xLoc_wpn_p2 -= 61;
				xLoc_wpn_p3 -= 61;
				xLoc_wpn_p4 -= 61;
				xLoc_wpn_p5 -= 61;
				if(open_page == 0)
				{
					alpha_wpn_p1 -= (255.0f/10.0f);
					alpha_wpn_p2 += (255.0f/10.0f);
				}
				if(open_page == 1)
				{
					alpha_wpn_p2 -= (255.0f/10.0f);
					alpha_wpn_p3 += (255.0f/10.0f);
				}
				if(open_page == 2)
				{
					alpha_wpn_p3 -= (255.0f/10.0f);
					alpha_wpn_p4 += (255.0f/10.0f);
				}
				if(open_page == 3)
				{
					alpha_wpn_p4 -= (255.0f/10.0f);
					alpha_wpn_p5 += (255.0f/10.0f);
				}
				anim_count++;
			}
			else
			{
				scrollR = false;
				open_page++;
				anim_count = 0;
			}

		}

		if(scrollL && !scrollR && open_page != 0)
		{
			//一度に処理されちゃう？
			if(anim_count < 10)
			{
				xLoc_wpn_p1 += 61;
				xLoc_wpn_p2 += 61;
				xLoc_wpn_p3 += 61;
				xLoc_wpn_p4 += 61;
				xLoc_wpn_p5 += 61;
				if(open_page == 1)
				{
					alpha_wpn_p1 += (255.0f/10.0f);
					alpha_wpn_p2 -= (255.0f/10.0f);
				}
				if(open_page == 2)
				{
					alpha_wpn_p2 += (255.0f/10.0f);
					alpha_wpn_p3 -= (255.0f/10.0f);
				}
				if(open_page == 3)
				{
					alpha_wpn_p3 += (255.0f/10.0f);
					alpha_wpn_p4 -= (255.0f/10.0f);
				}
				if(open_page == 4)
				{
					alpha_wpn_p4 += (255.0f/10.0f);
					alpha_wpn_p5 -= (255.0f/10.0f);
				}
				anim_count++;
			}
			else
			{
				scrollL = false;
				open_page--;
				anim_count = 0;
			}

		}
    }
}


declareFunctionHook(void, GX2CopyColorBufferToScanBuffer, const GX2ColorBuffer *colorBuffer, s32 scan_target)
{
	weapon_menu();
	real_GX2CopyColorBufferToScanBuffer(colorBuffer, scan_target);
}

FunctionHook method_hooks_gx2[] __attribute__((section(".data"))) = {

	makeFunctionHook(GX2CopyColorBufferToScanBuffer, LIB_GX2, STATIC_FUNCTION)
};

u32 method_hooks_size_gx2 __attribute__((section(".data"))) = sizeof(method_hooks_gx2) / sizeof(FunctionHook);

volatile unsigned int method_calls_gx2[sizeof(method_hooks_gx2) / sizeof(FunctionHook) *
									   FUNCTION_PATCHER_METHOD_STORE_SIZE] __attribute__((section(".data")));