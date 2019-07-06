#define WIN32_LEAN_AND_MEAN  // just say no to MFC

#include <windows.h>   // include all the windows headers
#include <windowsx.h>  // include useful macros
#include <mmsystem.h>  // very important and include WINMM.LIB too!
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<time.h>
#include "ScrollingBackground.h"
#include "Sprite.h"
#include "GameEngine.h"

#include <fstream>  
#include <iostream>  
using namespace std; 

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "msimg32.lib")

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)
//////////////////////////////////////////////////////////////////////

#define WINDOW_CLASS_NAME "WINCLASS1"

void GamePaint(HDC hDC);
void MouseMove(HDC hDC);
void MouseButtonDown(int x, int y, BOOL change);
void HandleKeys();

const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 700;

const int state_menu = 0;
const int state_gameOver = 1;
const int state_loading = 2;
const int state_foreword = 3;
const int state_start1=4;
const int state_start2=5;


const int stateM_solo = 10;
const int stateM_party = 11;
const int stateM_pvp = 12;
const int stateM_story = 13;
const int stateM_exit = 14;
const int stateM_rank = 15;

const int zorder_player = 4;
const int zorder_animal = 0;
const int zorder_enemy = 2;
const int zorder_bull = 3;


const int type_player = 101;
const int type_playerBull = 102;  //玩家
const int type_playerCrueBull = 103;  //玩家导弹
const int type_enemy = 201;      //敌方
const int type_enemyBull = 202;  //敌方导弹
const int type_enemyBull2 = 2022;  //敌方导弹
const int type_miniboss = 203;
const int type_miniboss_bull = 204;
const int type_animal = 301;


const int type_door_enemy=403;
const int type_door_god1 =404;
const int type_door_god2 =405;
const int type_door_god3 =406;
const int type_door_god4 =407;
const int type_god =408;
const int type_god2 =409;
const int type_god3 =410;
const int type_god4 =410;
///Menu//////////////////////////////////////

Bitmap* g_pMenuBitmap;

Bitmap* g_pSoloBitmap;   Bitmap* g_pSoloBitmap2;
Bitmap* g_pPartyBitmap;  Bitmap* g_pPartyBitmap2;
Bitmap* g_pPvpBitmap;    Bitmap* g_pPvpBitmap2;
Bitmap* g_pStoryBitmap;  Bitmap* g_pStoryBitmap2;
Bitmap* g_pExitBitmap;   Bitmap* g_pExitBitmap2;
Bitmap* g_pRankBitmap;   Bitmap* g_pRankBitmap2;
Bitmap* g_pBackBitmap;   Bitmap* g_pBackBitmap2; 

Sprite* g_pSoloSprite;
Sprite* g_pPartySprite;
Sprite* g_pPvpSprite;
Sprite* g_pStorySprite;
Sprite* g_pExitSprite;
Sprite* g_pRankSprite;
Sprite* g_pBackSprite;
//Ranking
int PlaySorce[10];
int PlaySorceMax=0;
int PlaySorceMin=0;

///Loading//////////////////////////////////
Bitmap* g_pLoadingBitmap;

//foreword///////////////////
Bitmap* g_pDoorBitmap;
Bitmap* g_pDoor2Bitmap;
Bitmap* g_pGodBitmap;
Bitmap* g_pGodBitmap2;
Bitmap* g_pGodBitmap3;
Bitmap* g_pGodBitmap4;
Bitmap* g_pTalkBitmap1;
Bitmap* g_pTalkBitmap2;
Bitmap* g_pTalkBitmap3;
Bitmap* g_pTalkBitmap4;

Sprite* g_pDoorSprite;
Sprite* g_pDoor2Sprite;
Sprite* g_pDoor3Sprite;
Sprite* g_pDoor4Sprite;
Sprite* g_pGodSprite;
Sprite* g_pGodSprite2;
Sprite* g_pGodSprite3;
Sprite* g_pGodSprite4;



///Player Sprite/////////////////////////////////////
HWND      main_window_handle = NULL; // globally track main window
HINSTANCE hinstance_app      = NULL; // globally track hinstance
HDC         g_hCollisionDC;     // 碰撞 HDC  
HBITMAP     g_pCollisionBitmap;  //碰撞 位图句柄

Bitmap*     g_pPlayBitmap;
Sprite*     g_pPlaySprite; 

Bitmap*     g_pPlayBitmap2;
Sprite*     g_pPlaySprite2; 

int direction = 1;
int direction2 = 2;
int keystate = 0;
int keystate2 = 0;

RECT    rcBounds1 = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
RECT    rcBounds2 = { 0, 70, WINDOW_WIDTH, WINDOW_HEIGHT-60};//Fly Sprite


BOOL stop = false;
BOOL stop2 = false;

Bitmap* g_pPlayerBullBitmap;
Sprite* g_pPlayerBullSprite;
int frames_playerBull[4][8]={
	{0,1,2,3,4,5,6,7},
	{8,9,10,11,12,13,14,15},
	{16,17,18,19,20,21,22,23},
	{24,25,26,27,28,29,30,31}
};


Bitmap* g_pPlayerDestroyBitmap;
Sprite* g_pPlayerDestroySprite;
int frames_playerDestory[1][5]={
	{0,1,2,3,4}
};



////Enemy//////////////////////////////////////////

int EnemyNum=0;


Bitmap* g_pFlyEnemyBitmaps1;
Bitmap* g_pFlyEnemyBitmaps2;
Bitmap* g_pFlyEnemyBitmaps3;
Bitmap* g_pFlyEnemyBitmaps4;
Bitmap* g_pFlyEnemyBitmaps5;
Bitmap* g_pWalkEnemyBitmaps1;

Bitmap* g_pMinibossBitmap1;

Sprite* g_pFlyEnemySprite1;
Sprite* g_pFlyEnemySprite2;
Sprite* g_pFlyEnemySprite3;
Sprite* g_pFlyEnemySprite4;
Sprite* g_pFlyEnemySprite5;
Sprite* g_pWalkEnemySprite1;

Sprite* g_pMinibossSprite1;

Bitmap* g_pEnemyBullBitmap1;////敌方的导弹
Sprite* g_pEnemyBullSprite1;

Bitmap* g_pEnemyBullBitmap2;////敌方的导弹
Sprite* g_pEnemyBullSprite2;

int frames_animal[4][4]={
	{0,1,2,3},
	{4,5,6,7},
	{8,9,10,11},
	{12,13,14,15}
};

int frames_2X2[2][2]={
	{0,1},
	{2,3}
};



///Gameover Bitmap//////////////////////
Bitmap*     g_pGameOverBitmap;    //gameover背景


// GLOBALS ////////////////////////////////////////////////

HWND      g_hWnd = NULL; // globally track main window
HINSTANCE g_hInstance = NULL; // globally track hinstance


HDC         g_hOffscreenDC;
HBITMAP     g_hOffscreenBitmap;

BackgroundLayer*      g_pBG0Layer;
BackgroundLayer*      g_pBG1Layer;
BackgroundLayer*      g_pBG2Layer;
BackgroundLayer*      g_pBG3Layer;

////////////////////////////////////
//int         g_type; //1:ship 2:Enemy 3:ShipBullet 4:EnemyBullet
int         g_iInputDelay;  //输入延迟变量，有助于改变键盘和游戏杆的输入响应，以便改进游戏的可玩性
int         g_iNumLives; //剩余Hp
int         g_iNumLives2; //剩余Hp
int         g_iScore;//得分
int         g_iScore2;//得分
double         g_difficultyValue;//难度
BOOL        g_bGameOver;//游戏是否结束
int state =0;

BOOL   f_drawTalk1=false;
BOOL   f_drawTalk2=false;
BOOL   f_drawTalk3=false;
BOOL   f_drawTalk4=false;
// FUNCTIONS //////////////////////////////////////////////

RECT rcBounds = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT }; //完整视口
RECT rcViewport = { 0, 20, WINDOW_WIDTH, WINDOW_HEIGHT };//滚动背景视口


///////////////////////////////////////////////////
//排序
void bubbleSort(int* pData,int length)
{
    int temp;
    for(int i = 0;i != length;++i)
    {
        for (int j = 0; j != length; ++j)
        {
            if (pData[i] > pData[j])
            {
                temp = pData[i];
                pData[i] = pData[j];
                pData[j] = temp;
            }
        }
    }
}

void File(){
	//g_iScore1和PlaySorce[]比较，插入相应位置
		int arr[11]={0};
		for(int l=0;l<10;l++){
			arr[l]=PlaySorce[l];
		}
		arr[10]=g_iScore;
		bubbleSort(arr,11);
		for(int h=0;h<10;h++){
			PlaySorce[h]=arr[h];
		}

		//写入文件
		ofstream OpenFile("file.txt");  
		if (OpenFile.fail())  {  
			cout<<"打开文件错误!"<<endl;  
			exit(0);  
		}

		for(int k=0;k<10;k++){
			OpenFile<<PlaySorce[k]<<" ";  
		}

		OpenFile.close();  
				//system("pause");  
}

//该游戏的工作方式，没有理由需要响应任何精灵碰撞
BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee)
{
	if(state==stateM_solo || state==stateM_party){
		if(type_playerBull==pSpriteHittee->GetType() && type_enemy==pSpriteHitter->GetType()){

			pSpriteHitter->SetDeath(TRUE);
			pSpriteHittee->SetDeath(TRUE);
			g_iScore=g_iScore+5;

		}
		if(type_enemy==pSpriteHittee->GetType() && type_player==pSpriteHitter->GetType()){
			g_iNumLives=g_iNumLives-5;
			pSpriteHittee->SetDeath(TRUE);
			if(g_iNumLives<=0){
				File();
				state=state_gameOver;
			}
		}
		if(type_playerBull==pSpriteHittee->GetType() && type_miniboss==pSpriteHitter->GetType()){
			g_iScore=g_iScore+10;
			pSpriteHitter->SetDeath(TRUE);
			pSpriteHittee->SetDeath(TRUE);
			if(g_iNumLives<=0){
				File();
				state=state_gameOver;
			}
		}
		if(type_enemyBull==pSpriteHittee->GetType() && type_player==pSpriteHitter->GetType()){
			g_iNumLives=g_iNumLives-10;
			pSpriteHittee->SetDeath(TRUE);
			if(g_iNumLives<=0){
				File();
				state=state_gameOver;
			}
			
		}
		if(type_miniboss_bull==pSpriteHittee->GetType() && type_player==pSpriteHitter->GetType()){
			pSpriteHittee->SetDeath(TRUE);
			g_iNumLives=g_iNumLives-50;
			if(g_iNumLives<=0){
				File();
				state=state_gameOver;
			}
		}

	}


	////foreword/////////////////////////////////////////////////////////////
	if(state==state_foreword){
		if(type_door_god1==pSpriteHittee->GetType() && type_player==pSpriteHitter->GetType()){
			/*g_pGodSprite=new Sprite (g_pGodBitmap,type_god,rcBounds1, BA_STOP);*/
			g_pGodSprite->SetPosition(pSpriteHittee->GetPosition().left-70,pSpriteHittee->GetPosition().top+5);
			g_pGodSprite->SetRowAndCol(2,2);
			g_pGodSprite->SetDelayFrame(3);
			g_pGodSprite->SetFrameList(frames_2X2[0],2);
			g_pGodSprite->SetBShowAnim(true);
			g_pGodSprite->SetZOrder(1);
			AddSprite(g_pGodSprite);
		
			pSpriteHittee->SetDeath(TRUE);
		}
		if(type_door_god2==pSpriteHittee->GetType() && type_player==pSpriteHitter->GetType()){
			/*g_pGodSprite2=new Sprite (g_pGodBitmap2,type_god2,rcBounds1, BA_STOP);*/
			g_pGodSprite2->SetPosition(pSpriteHittee->GetPosition().right,pSpriteHittee->GetPosition().top);
			g_pGodSprite2->SetRowAndCol(2,2);
			g_pGodSprite2->SetDelayFrame(3);
			g_pGodSprite2->SetFrameList(frames_2X2[1],2);
			g_pGodSprite2->SetBShowAnim(true);
			g_pGodSprite2->SetZOrder(1);
			AddSprite(g_pGodSprite2);

			pSpriteHittee->SetDeath(TRUE);
		}
		if(type_door_god3==pSpriteHittee->GetType() && type_player==pSpriteHitter->GetType()){
			/*g_pGodSprite3=new Sprite (g_pGodBitmap3,type_god3,rcBounds1, BA_STOP);*/
			g_pGodSprite3->SetPosition(pSpriteHittee->GetPosition().right-73,pSpriteHittee->GetPosition().top-78);
			g_pGodSprite3->SetRowAndCol(2,2);
			g_pGodSprite3->SetDelayFrame(3);
			g_pGodSprite3->SetFrameList(frames_2X2[0],2);
			g_pGodSprite3->SetBShowAnim(true);
			g_pGodSprite3->SetZOrder(1);
			AddSprite(g_pGodSprite3);
	
			pSpriteHittee->SetDeath(TRUE);
		}
		if(type_door_god4==pSpriteHittee->GetType() && type_player==pSpriteHitter->GetType()){
			g_pGodSprite4=new Sprite (g_pGodBitmap4,type_god4,rcBounds1, BA_STOP);
			g_pGodSprite4->SetPosition(pSpriteHittee->GetPosition());
			g_pGodSprite4->SetRowAndCol(2,2);
			g_pGodSprite4->SetDelayFrame(4);
			g_pGodSprite4->SetFrameList(frames_2X2[0],2);
			g_pGodSprite4->SetBShowAnim(true);
			g_pGodSprite4->SetZOrder(1);
			AddSprite(g_pGodSprite4);
	
			f_drawTalk4=true;
		}
		if(type_playerBull==pSpriteHittee->GetType() && type_enemy==pSpriteHitter->GetType()){

			pSpriteHitter->SetDeath(TRUE);
			pSpriteHittee->SetDeath(TRUE);

		}
	}
	

	///////PVP//////////////////////////////////////////////
	if(state==stateM_pvp){

		//player1导弹打中player2 
		if(type_playerBull==pSpriteHittee->GetType() && type_enemy==pSpriteHitter->GetType()){
			pSpriteHittee->SetDeath(TRUE);
			g_iNumLives2=g_iNumLives2-5;
			if(g_iNumLives2<=0){
				MessageBox(main_window_handle, TEXT("Player1 win !!!"), TEXT("GameOver"), MB_OK);
				g_iNumLives=g_iNumLives2=200;
				g_iScore=g_iScore2=0;
				state=state_menu;
			}
		}

		//player2导弹打中player1
		if(type_enemyBull==pSpriteHittee->GetType() && type_player==pSpriteHitter->GetType()){
			pSpriteHittee->SetDeath(TRUE);
			g_iNumLives=g_iNumLives-5;
			if(g_iNumLives<=0){
				MessageBox(main_window_handle, TEXT("Player2 win !!!"), TEXT("GameOver"), MB_OK);
				g_iNumLives=g_iNumLives2=200;
				g_iScore=g_iScore2=0;
				state=state_menu;
			}


		}

		if(type_enemyBull2==pSpriteHittee->GetType() && type_player==pSpriteHitter->GetType()){
			pSpriteHittee->SetDeath(TRUE);
			g_iNumLives=g_iNumLives-5;
			if(g_iNumLives<=0){
				MessageBox(main_window_handle, TEXT("Player2 win !!!"), TEXT("GameOver"), MB_OK);
				g_iNumLives=g_iNumLives2=200;
				g_iScore=g_iScore2=0;
				state=state_menu;
			}
			
			
		}


		if(type_enemyBull2==pSpriteHittee->GetType() && type_enemy==pSpriteHitter->GetType())
		{
			pSpriteHittee->SetDeath(TRUE);
			g_iNumLives2=g_iNumLives2-5;
		    if(g_iNumLives2<=0) {
			    MessageBox(main_window_handle, TEXT("Player1 win !!!"), TEXT("GameOver"), MB_OK);
				g_iNumLives=g_iNumLives2=200;
				g_iScore=g_iScore2=0;
				state=state_menu;
		
		}

		}
	
		if(type_enemyBull2==pSpriteHittee->GetType() && type_playerBull==pSpriteHitter->GetType()){

			pSpriteHitter->SetDeath(TRUE);
			pSpriteHittee->SetDeath(TRUE);

		}
	
	if(type_enemyBull2==pSpriteHittee->GetType() && type_enemyBull==pSpriteHitter->GetType()){

			pSpriteHitter->SetDeath(TRUE);
			pSpriteHittee->SetDeath(TRUE);
			

		}

	}
	return FALSE;
}


void GameStart(HDC hDC)
{
	
	g_pLoadingBitmap= new Bitmap(hDC, "Res/Loading.bmp");
	g_pLoadingBitmap->Draw(hDC,0,0);

	//文件加载
	int i,datalen=0;
    ifstream file("file.txt");
    while( ! file.eof() )
    file>>PlaySorce[datalen++];
    file.close();
	bubbleSort(PlaySorce,datalen); //从大到小排序

	srand(GetTickCount());
	state = state_menu;
	g_bGameOver = FALSE;
	g_difficultyValue=1;
	g_iScore=0;
	g_iScore2=0;
	g_iNumLives=200;
	g_iNumLives2=200;
	g_hOffscreenDC = CreateCompatibleDC(hDC);
	g_hOffscreenBitmap = CreateCompatibleBitmap(hDC,WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(g_hOffscreenDC, g_hOffscreenBitmap);

		g_pMenuBitmap= new Bitmap(hDC, "Res/bg.bmp");

		
		g_pBG0Layer = new BackgroundLayer(hDC, "Res/bg_00.bmp");
		g_pBG0Layer->SetViewport(rcViewport);
		g_pBG0Layer->SetSpeed(1);
		g_pBG0Layer->SetDirection(SD_LEFT);
 
		g_pBG1Layer = new BackgroundLayer(hDC, "Res/bg_11.bmp");
		g_pBG1Layer->SetViewport(rcViewport);
		g_pBG1Layer->SetSpeed(1);
		g_pBG1Layer->SetDirection(SD_LEFT);

		g_pBG2Layer = new BackgroundLayer(hDC, "Res/bg_22.bmp");
 		g_pBG2Layer->SetViewport(rcViewport);
		g_pBG2Layer->SetSpeed(2);
		g_pBG2Layer->SetDirection(SD_LEFT);

		g_pBG3Layer = new BackgroundLayer(hDC, "Res/bg_33.bmp");
 		g_pBG3Layer->SetViewport(rcViewport);
		g_pBG3Layer->SetSpeed(3);
		g_pBG3Layer->SetDirection(SD_LEFT);

		g_pSoloBitmap= new Bitmap(hDC, "Res/solo1.bmp");
		g_pPartyBitmap= new Bitmap(hDC, "Res/party1.bmp");
		g_pPvpBitmap= new Bitmap(hDC, "Res/pvp1.bmp");
		g_pStoryBitmap= new Bitmap(hDC, "Res/story1.bmp");
		g_pExitBitmap= new Bitmap(hDC, "Res/exit1.bmp");
		g_pRankBitmap= new Bitmap(hDC, "Res/R1.bmp");
		g_pBackBitmap= new Bitmap(hDC, "Res/back1.bmp");
		
		g_pSoloBitmap2= new Bitmap(hDC, "Res/solo2.bmp");
		g_pPartyBitmap2= new Bitmap(hDC, "Res/party2.bmp");
		g_pPvpBitmap2= new Bitmap(hDC, "Res/pvp2.bmp");
		g_pStoryBitmap2= new Bitmap(hDC, "Res/story2.bmp");
		g_pExitBitmap2= new Bitmap(hDC, "Res/exit2.bmp");
		g_pRankBitmap2= new Bitmap(hDC, "Res/R2.bmp");
		g_pBackBitmap2= new Bitmap(hDC, "Res/back2.bmp");

		g_pSoloSprite = new Sprite(g_pSoloBitmap,g_pSoloBitmap2,10,rcBounds);
		g_pSoloSprite->SetPosition(700,120);
		g_pSoloSprite->SetZOrder(1); 

		g_pPartySprite = new Sprite(g_pPartyBitmap,g_pPartyBitmap2,10,rcBounds);
		g_pPartySprite->SetPosition(700,220);
		g_pPartySprite->SetZOrder(1); 

		g_pPvpSprite = new Sprite(g_pPvpBitmap,g_pPvpBitmap2,10,rcBounds);
		g_pPvpSprite->SetPosition(700,320);
		g_pPvpSprite->SetZOrder(1); 

		g_pStorySprite = new Sprite(g_pStoryBitmap,g_pStoryBitmap2,10,rcBounds);
		g_pStorySprite->SetPosition(700,20);
		g_pStorySprite->SetZOrder(1); 

		g_pExitSprite = new Sprite(g_pExitBitmap,g_pExitBitmap2,10,rcBounds);
		g_pExitSprite->SetPosition(700,520);
		g_pExitSprite->SetZOrder(1);

		g_pRankSprite= new Sprite(g_pRankBitmap,g_pRankBitmap2,10,rcBounds);
		g_pRankSprite->SetPosition(700,420);
		g_pRankSprite->SetZOrder(1);

		g_pBackSprite= new Sprite(g_pBackBitmap,g_pBackBitmap2,10,rcBounds);
		g_pBackSprite->SetPosition(800,420);
		g_pBackSprite->SetZOrder(1);

		g_pPlayBitmap = new Bitmap(hDC, "Res/player_1.bmp");
		g_pPlayBitmap2 = new Bitmap(hDC, "Res/player_2.bmp");

		//加载Enemy
		g_pFlyEnemyBitmaps1 = new Bitmap(hDC, "Res/flyEnemy1.bmp");
		g_pFlyEnemyBitmaps2 = new Bitmap(hDC, "Res/flyEnemy2.bmp");
		g_pFlyEnemyBitmaps3 = new Bitmap(hDC, "Res/flyEnemy3.bmp");
		g_pFlyEnemyBitmaps4 = new Bitmap(hDC, "Res/flyEnemy4.bmp");
		g_pFlyEnemyBitmaps5 = new Bitmap(hDC, "Res/flyEnemy5.bmp");
		g_pWalkEnemyBitmaps1= new Bitmap(hDC, "Res/walkEnemy1.bmp");
		g_pMinibossBitmap1 = new Bitmap(hDC, "Res/minboss1.bmp");



		//other
		g_pPlayerDestroyBitmap = new Bitmap(hDC, "Res/E.bmp");
		g_pPlayerBullBitmap= new Bitmap(hDC, "Res/PlayerBull.bmp");
		g_pEnemyBullBitmap1= new Bitmap(hDC, "Res/shot.bmp");
		g_pEnemyBullBitmap2= new Bitmap(hDC, "Res/EnemyMissile.bmp");
		g_pGameOverBitmap = new Bitmap(hDC,"Res/GameOver.bmp");

		//foreword
		g_pDoorBitmap = new Bitmap(hDC, "Res/door.bmp");
		g_pDoor2Bitmap = new Bitmap(hDC, "Res/door2.bmp");
		g_pGodBitmap = new Bitmap(hDC, "Res/god.bmp");
		g_pGodBitmap2 = new Bitmap(hDC, "Res/god3.bmp");
		g_pGodBitmap3 = new Bitmap(hDC, "Res/god2.bmp");
		g_pGodBitmap4 = new Bitmap(hDC, "Res/god4.bmp");
		g_pTalkBitmap1= new Bitmap(hDC, "Res/talk1.bmp");
		g_pTalkBitmap2= new Bitmap(hDC, "Res/talk2.bmp");
		g_pTalkBitmap3= new Bitmap(hDC, "Res/talk3.bmp");
		g_pTalkBitmap4= new Bitmap(hDC, "Res/talk4.bmp");

		g_pGodSprite=new Sprite (g_pGodBitmap,type_god,rcBounds1, BA_STOP);
		g_pGodSprite2=new Sprite (g_pGodBitmap2,type_god,rcBounds1, BA_STOP);
		g_pGodSprite3=new Sprite (g_pGodBitmap3,type_god,rcBounds1, BA_STOP);

		GamePaint(g_hOffscreenDC);
}	

void GameEnd()
{
	DeleteObject(g_hOffscreenBitmap);
	DeleteDC(g_hOffscreenDC);  
		
	// Cleanup the scrolling background and landscape layer

	delete g_pBG3Layer;
	delete g_pBG2Layer;
	delete g_pBG1Layer;
    delete g_pBG0Layer;
	delete g_pMenuBitmap;

	delete g_pSoloBitmap;
	delete g_pPartyBitmap;
	delete g_pPvpBitmap;
	delete g_pStoryBitmap;
	delete g_pExitBitmap;
	delete g_pSoloBitmap2;
	delete g_pPartyBitmap2;
	delete g_pPvpBitmap2;
	delete g_pStoryBitmap2;
	delete g_pExitBitmap2;
	delete g_pRankBitmap;
	delete g_pRankBitmap2;
	delete g_pBackBitmap;
	delete g_pBackBitmap2;

	delete g_pFlyEnemyBitmaps1;
	delete g_pFlyEnemyBitmaps2;
	delete g_pFlyEnemyBitmaps3;
	delete g_pWalkEnemySprite1; 
	delete g_pEnemyBullBitmap1;	
	delete g_pEnemyBullBitmap2;	

	delete g_pMinibossBitmap1;
	delete g_pGameOverBitmap;
	delete g_pPlayBitmap;
	delete g_pPlayerDestroyBitmap;

	delete g_pGodBitmap;
	delete g_pGodBitmap2;
	delete g_pGodBitmap3;
	delete g_pGodBitmap4;
	delete g_pTalkBitmap1;
	delete g_pTalkBitmap2;
	delete g_pTalkBitmap3;
	delete g_pTalkBitmap4;

	CleanupSprites();
}


void HandleKeys()
{
	
	if(KEYDOWN(VK_RIGHT))
	{
		direction = 1;
		g_pPlaySprite->SetFrameList(frames_animal[3],4);
		g_pPlaySprite->SetVelocity(20,0);
		g_pPlaySprite->SetDelayFrame(3);
		g_pPlaySprite->SetBShowAnim(true);
	}
	else if(KEYDOWN(VK_LEFT))
	{
		direction = 2;
		g_pPlaySprite->SetFrameList(frames_animal[1],4);
		g_pPlaySprite->SetVelocity(-20,0);
		g_pPlaySprite->SetDelayFrame(3);
		g_pPlaySprite->SetBShowAnim(true);
	}

	if(KEYDOWN(VK_UP))
	{
		g_pPlaySprite->SetVelocity(0,-15);
		g_pPlaySprite->SetDelayFrame(3);
	}
	else if(KEYDOWN(VK_DOWN))
	{
		g_pPlaySprite->SetVelocity(0,15);
		g_pPlaySprite->SetDelayFrame(3);
	}


	
	if(KEYDOWN('K'))
	{

		if(++g_iInputDelay>4){

			g_pPlayerBullSprite=new Sprite(g_pPlayerBullBitmap,type_playerBull,rcBounds1,BA_DIE);
			g_pPlayerBullSprite->SetFrameList(frames_playerBull[0],8);
			g_pPlayerBullSprite->SetRowAndCol(8,4);
			g_pPlayerBullSprite->SetDelayFrame(1);
			g_pPlayerBullSprite->SetZOrder(zorder_bull);
			if(direction==1){
				g_pPlayerBullSprite->SetPosition(g_pPlaySprite->GetPosition().right-20,g_pPlaySprite->GetPosition().top+12);
				g_pPlayerBullSprite->SetVelocity(25,0);
			}
			else if(direction==2){
				g_pPlayerBullSprite->SetPosition(g_pPlaySprite->GetPosition().left+5,g_pPlaySprite->GetPosition().top+12);
				g_pPlayerBullSprite->SetVelocity(-25,0);
			}
			g_pPlayerBullSprite->SetBShowAnim(true);
			
			AddSprite(g_pPlayerBullSprite);
			g_iInputDelay=0;
		}		
	}

	if(KEYDOWN('L'))
	{


		if(++g_iInputDelay>4){

			g_pPlayerBullSprite=new Sprite(g_pPlayerBullBitmap,type_playerBull,rcBounds1,BA_DIE);
			g_pPlayerBullSprite->SetFrameList(frames_playerBull[3],8);
			g_pPlayerBullSprite->SetRowAndCol(8,4);
			g_pPlayerBullSprite->SetDelayFrame(1);
			g_pPlayerBullSprite->SetZOrder(zorder_bull);
			if(direction==1){
				g_pPlayerBullSprite->SetPosition(g_pPlaySprite->GetPosition().right-20,g_pPlaySprite->GetPosition().top+12);
				g_pPlayerBullSprite->SetVelocity(25,0);
			}
			else if(direction==2){
				g_pPlayerBullSprite->SetPosition(g_pPlaySprite->GetPosition().left+5,g_pPlaySprite->GetPosition().top+12);
				g_pPlayerBullSprite->SetVelocity(-25,0);
			}
			g_pPlayerBullSprite->SetBShowAnim(true);
			
			AddSprite(g_pPlayerBullSprite);
			g_iInputDelay=0;
			
		}		
		
	}
	
}
void HandleKeys2()
{
if(KEYDOWN('D'))
	{
		direction2 = 1;
		g_pPlaySprite2->SetFrameList(frames_animal[3],4);
		g_pPlaySprite2->SetVelocity(20,0);
		g_pPlaySprite2->SetDelayFrame(3);
		g_pPlaySprite2->SetBShowAnim(true);
	}
	else if(KEYDOWN('A'))
	{
		direction2 = 2;
		g_pPlaySprite2->SetFrameList(frames_animal[1],4);
		g_pPlaySprite2->SetVelocity(-20,0);
		g_pPlaySprite2->SetDelayFrame(3);
		g_pPlaySprite2->SetBShowAnim(true);
	}

	if(KEYDOWN('W'))
	{
		g_pPlaySprite2->SetVelocity(0,-15);
		g_pPlaySprite2->SetDelayFrame(3);
	}
	else if(KEYDOWN('S'))
	{
		g_pPlaySprite2->SetVelocity(0,15);
		g_pPlaySprite2->SetDelayFrame(3);
	}


	
	if(KEYDOWN('G'))
	{

		if(++g_iInputDelay>4){

			g_pPlayerBullSprite=new Sprite(g_pPlayerBullBitmap,type_playerBull,rcBounds1,BA_DIE);
			g_pPlayerBullSprite->SetFrameList(frames_playerBull[1],8);
			g_pPlayerBullSprite->SetRowAndCol(8,4);
			g_pPlayerBullSprite->SetDelayFrame(1);
			g_pPlayerBullSprite->SetZOrder(zorder_bull);
			if(direction2==1){
				g_pPlayerBullSprite->SetPosition(g_pPlaySprite2->GetPosition().right-20,g_pPlaySprite2->GetPosition().top+12);
				g_pPlayerBullSprite->SetVelocity(25,0);
			}
			else if(direction2==2){
				g_pPlayerBullSprite->SetPosition(g_pPlaySprite2->GetPosition().left+5,g_pPlaySprite2->GetPosition().top+12);
				g_pPlayerBullSprite->SetVelocity(-25,0);
			}
			g_pPlayerBullSprite->SetBShowAnim(true);
			
			AddSprite(g_pPlayerBullSprite);
			g_iInputDelay=0;
		}		
	}

	if(KEYDOWN('H'))
	{


		if(++g_iInputDelay>4)
		{

			g_pPlayerBullSprite=new Sprite(g_pPlayerBullBitmap,type_playerBull,rcBounds1,BA_DIE);
			g_pPlayerBullSprite->SetFrameList(frames_playerBull[2],8);
			g_pPlayerBullSprite->SetRowAndCol(8,4);
			g_pPlayerBullSprite->SetDelayFrame(1);
			g_pPlayerBullSprite->SetZOrder(zorder_bull);
			if(direction2==1)
			{
				g_pPlayerBullSprite->SetPosition(g_pPlaySprite2->GetPosition().right-20,g_pPlaySprite2->GetPosition().top+12);
				g_pPlayerBullSprite->SetVelocity(25,0);
			}
			else if(direction2==2)
			{
				g_pPlayerBullSprite->SetPosition(g_pPlaySprite2->GetPosition().left+5,g_pPlaySprite2->GetPosition().top+12);
				g_pPlayerBullSprite->SetVelocity(-25,0);
			}
			g_pPlayerBullSprite->SetBShowAnim(true);
			
			AddSprite(g_pPlayerBullSprite);
			g_iInputDelay=0;
			
		}		
    }
}

//Pvp player2的按键
void HandleKeys3()
{
if(KEYDOWN('D'))
	{
		direction2 = 1;
		g_pPlaySprite2->SetFrameList(frames_animal[3],4);
		g_pPlaySprite2->SetVelocity(20,0);
		g_pPlaySprite2->SetDelayFrame(3);
		g_pPlaySprite2->SetBShowAnim(true);
	}
	else if(KEYDOWN('A'))
	{
		direction2 = 2;
		g_pPlaySprite2->SetFrameList(frames_animal[1],4);
		g_pPlaySprite2->SetVelocity(-20,0);
		g_pPlaySprite2->SetDelayFrame(3);
		g_pPlaySprite2->SetBShowAnim(true);
	}

	if(KEYDOWN('W'))
	{
		g_pPlaySprite2->SetVelocity(0,-15);
		g_pPlaySprite2->SetDelayFrame(3);
	}
	else if(KEYDOWN('S'))
	{
		g_pPlaySprite2->SetVelocity(0,15);
		g_pPlaySprite2->SetDelayFrame(3);
	}


	
	if(KEYDOWN('G'))
	{

		if(++g_iInputDelay>4){

			g_pPlayerBullSprite=new Sprite(g_pPlayerBullBitmap,type_enemyBull,rcBounds1,BA_DIE);
			g_pPlayerBullSprite->SetFrameList(frames_playerBull[1],8);
			g_pPlayerBullSprite->SetRowAndCol(8,4);
			g_pPlayerBullSprite->SetDelayFrame(1);
			g_pPlayerBullSprite->SetZOrder(zorder_bull);
			if(direction2==1){
				g_pPlayerBullSprite->SetPosition(g_pPlaySprite2->GetPosition().right-20,g_pPlaySprite2->GetPosition().top+12);
				g_pPlayerBullSprite->SetVelocity(25,0);
			}
			else if(direction2==2){
				g_pPlayerBullSprite->SetPosition(g_pPlaySprite2->GetPosition().left+5,g_pPlaySprite2->GetPosition().top+12);
				g_pPlayerBullSprite->SetVelocity(-25,0);
			}
			g_pPlayerBullSprite->SetBShowAnim(true);
			
			AddSprite(g_pPlayerBullSprite);
			g_iInputDelay=0;
		}		
	}

	if(KEYDOWN('H'))
	{


		if(++g_iInputDelay>4)
		{

			g_pPlayerBullSprite=new Sprite(g_pPlayerBullBitmap,type_enemyBull,rcBounds1,BA_DIE);
			g_pPlayerBullSprite->SetFrameList(frames_playerBull[2],8);
			g_pPlayerBullSprite->SetRowAndCol(8,4);
			g_pPlayerBullSprite->SetDelayFrame(1);
			g_pPlayerBullSprite->SetZOrder(zorder_bull);
			if(direction2==1)
			{
				g_pPlayerBullSprite->SetPosition(g_pPlaySprite2->GetPosition().right-20,g_pPlaySprite2->GetPosition().top+12);
				g_pPlayerBullSprite->SetVelocity(25,0);
			}
			else if(direction2==2)
			{
				g_pPlayerBullSprite->SetPosition(g_pPlaySprite2->GetPosition().left+5,g_pPlaySprite2->GetPosition().top+12);
				g_pPlayerBullSprite->SetVelocity(-25,0);
			}
			g_pPlayerBullSprite->SetBShowAnim(true);
			
			AddSprite(g_pPlayerBullSprite);
			g_iInputDelay=0;
			
		}		
    }
}  //pvp

void forewordLoading(){
		g_pPlaySprite = new Sprite(g_pPlayBitmap,type_player,rcBounds1, BA_STOP); 
		g_pPlaySprite->SetPosition(50,300);
		g_pPlaySprite->SetRowAndCol(4,4);
		g_pPlaySprite->SetDelayFrame(3);
		g_pPlaySprite->SetFrameList(frames_animal[3],4);
		g_pPlaySprite->SetBShowAnim(true);
		g_pPlaySprite->SetZOrder(zorder_player);
		AddSprite(g_pPlaySprite);

		g_pDoorSprite= new Sprite(g_pDoorBitmap,type_door_god1,rcBounds1, BA_STOP); 
		g_pDoorSprite->SetPosition(370,130);
		g_pDoorSprite->SetRowAndCol(2,2);
		g_pDoorSprite->SetDelayFrame(3);
		g_pDoorSprite->SetFrameList(frames_2X2[0],2);
		g_pDoorSprite->SetBShowAnim(true);
		g_pDoorSprite->SetZOrder(1);
		AddSprite(g_pDoorSprite);

		g_pDoor2Sprite= new Sprite(g_pDoor2Bitmap,type_door_god2,rcBounds1, BA_STOP); 
		g_pDoor2Sprite->SetPosition(600,570);
		g_pDoor2Sprite->SetRowAndCol(2,2);
		g_pDoor2Sprite->SetDelayFrame(3);
		g_pDoor2Sprite->SetFrameList(frames_2X2[0],2);
		g_pDoor2Sprite->SetBShowAnim(true);
		g_pDoor2Sprite->SetZOrder(1);
		AddSprite(g_pDoor2Sprite);

		g_pDoor3Sprite= new Sprite(g_pDoorBitmap,type_door_god3,rcBounds1, BA_STOP); 
		g_pDoor3Sprite->SetPosition(800,300);
		g_pDoor3Sprite->SetRowAndCol(2,2);
		g_pDoor3Sprite->SetDelayFrame(3);
		g_pDoor3Sprite->SetFrameList(frames_2X2[1],2);
		g_pDoor3Sprite->SetBShowAnim(true);
		g_pDoor3Sprite->SetZOrder(1);
		AddSprite(g_pDoor3Sprite);

		g_pDoor4Sprite= new Sprite(g_pDoorBitmap,type_door_god4,rcBounds1, BA_STOP); 
		g_pDoor4Sprite->SetPosition(1000,60);
		g_pDoor4Sprite->SetRowAndCol(2,2);
		g_pDoor4Sprite->SetDelayFrame(3);
		g_pDoor4Sprite->SetFrameList(frames_2X2[1],2);
		g_pDoor4Sprite->SetBShowAnim(true);
		g_pDoor4Sprite->SetZOrder(1);
		AddSprite(g_pDoor4Sprite);

}

void MouseButtonDown(int x, int y, BOOL change)
{
	POINT MousePoint;
	GetCursorPos(&MousePoint);
	if(state==state_menu){
		if(g_pSoloSprite->IsPointInside(MousePoint.x,MousePoint.y-70)){  
			CleanupSprites();//?
			g_pPlaySprite = new Sprite(g_pPlayBitmap,type_player,rcBounds1, BA_STOP);  
			g_pPlaySprite->SetPosition(50,300);
			g_pPlaySprite->SetRowAndCol(4,4);
			g_pPlaySprite->SetDelayFrame(3);
			g_pPlaySprite->SetFrameList(frames_animal[3],4);
			g_pPlaySprite->SetBShowAnim(true);
			g_pPlaySprite->SetZOrder(zorder_player);
			AddSprite(g_pPlaySprite);
			state=stateM_solo;
		}
		if(g_pPartySprite->IsPointInside(MousePoint.x,MousePoint.y-70)){
			g_iNumLives=400;
			CleanupSprites();//?
			g_pPlaySprite = new Sprite(g_pPlayBitmap,type_player,rcBounds1, BA_STOP);  
			g_pPlaySprite->SetPosition(50,300);
			g_pPlaySprite->SetRowAndCol(4,4);
			g_pPlaySprite->SetDelayFrame(3);
			g_pPlaySprite->SetFrameList(frames_animal[3],4);
			g_pPlaySprite->SetBShowAnim(true);
			g_pPlaySprite->SetZOrder(zorder_player);
			AddSprite(g_pPlaySprite);

			g_pPlaySprite2 = new Sprite(g_pPlayBitmap2,type_player,rcBounds1, BA_STOP);  
			g_pPlaySprite2->SetPosition(500,300);
			g_pPlaySprite2->SetRowAndCol(4,4);
			g_pPlaySprite2->SetDelayFrame(3);
			g_pPlaySprite2->SetFrameList(frames_animal[1],4);
			g_pPlaySprite2->SetBShowAnim(true);
			g_pPlaySprite2->SetZOrder(zorder_player);
			AddSprite(g_pPlaySprite2);

			state=stateM_party;
		}
		if(g_pPvpSprite->IsPointInside(MousePoint.x,MousePoint.y-70)){
			CleanupSprites();//?
			g_pPlaySprite = new Sprite(g_pPlayBitmap,type_player,rcBounds1, BA_STOP); 
			g_pPlaySprite->SetPosition(50,300);
			g_pPlaySprite->SetRowAndCol(4,4);
			g_pPlaySprite->SetDelayFrame(3);
			g_pPlaySprite->SetFrameList(frames_animal[3],4);
			g_pPlaySprite->SetBShowAnim(true);
			g_pPlaySprite->SetZOrder(zorder_player);
			AddSprite(g_pPlaySprite);
	
			g_pPlaySprite2 = new Sprite(g_pPlayBitmap2,type_enemy,rcBounds1, BA_STOP); 
			g_pPlaySprite2->SetPosition(500,300);
			g_pPlaySprite2->SetRowAndCol(4,4);
			g_pPlaySprite2->SetDelayFrame(3);
			g_pPlaySprite2->SetFrameList(frames_animal[1],4);
			g_pPlaySprite2->SetBShowAnim(true);
			g_pPlaySprite2->SetZOrder(zorder_player);
			AddSprite(g_pPlaySprite2);
			state=stateM_pvp;
		}
		if(g_pStorySprite->IsPointInside(MousePoint.x,MousePoint.y-70)){
			//state=stateM_story;
			CleanupSprites();
			forewordLoading();
			state=state_foreword;
		}
		if(g_pExitSprite->IsPointInside(MousePoint.x,MousePoint.y-70)){
			PostQuitMessage(0);		
			state=stateM_exit;
		}
		if(g_pRankSprite->IsPointInside(MousePoint.x,MousePoint.y-70)){
			CleanupSprites();
			state=stateM_rank;
		}
	}

	else if(state==stateM_rank || state==stateM_pvp){
		if(g_pBackSprite->IsPointInside(MousePoint.x,MousePoint.y-70)){
			CleanupSprites();
			state=state_menu;
		}
	
	}


	///Foreword
	else if(state==state_foreword){
		
		if(g_pGodSprite->IsPointInside(MousePoint.x+10,MousePoint.y)){
				f_drawTalk1=true;
			}
			if(g_pGodSprite2->IsPointInside(MousePoint.x,MousePoint.y)){
				f_drawTalk2=true;
			}
			if(g_pGodSprite3->IsPointInside(MousePoint.x,MousePoint.y)){
				f_drawTalk3=true;
			}
			if(g_pDoor4Sprite->IsPointInside(MousePoint.x-5,MousePoint.y)){

				CleanupSprites();

				state=state_menu;
			}
		
	}

}

void MouseMove(HDC hDC)
{
	POINT MousePoint;
	GetCursorPos(&MousePoint);

	if(state==state_menu){
		if(g_pSoloSprite->IsPointInside(MousePoint.x,MousePoint.y-70)){
			g_pSoloSprite->Draw2(hDC,g_pSoloSprite->GetPosition().left,g_pSoloSprite->GetPosition().bottom,TRUE,RGB(255,0,255));
		}
		if(g_pPartySprite->IsPointInside(MousePoint.x,MousePoint.y-70)){
			g_pPartySprite->Draw2(hDC,g_pPartySprite->GetPosition().left,g_pPartySprite->GetPosition().bottom,TRUE,RGB(255,0,255));
		}
		if(g_pPvpSprite->IsPointInside(MousePoint.x,MousePoint.y-70)){
			g_pPvpSprite->Draw2(hDC,g_pPvpSprite->GetPosition().left,g_pPvpSprite->GetPosition().bottom,TRUE,RGB(255,0,255));
		}
		if(g_pStorySprite->IsPointInside(MousePoint.x,MousePoint.y-70)){
			g_pStorySprite->Draw2(hDC,g_pStorySprite->GetPosition().left,g_pStorySprite->GetPosition().bottom,TRUE,RGB(255,0,255));
		}
		if(g_pExitSprite->IsPointInside(MousePoint.x,MousePoint.y-70)){
			g_pExitSprite->Draw2(hDC,g_pExitSprite->GetPosition().left,g_pExitSprite->GetPosition().bottom,TRUE,RGB(255,0,255));
		}
		if(g_pRankSprite->IsPointInside(MousePoint.x,MousePoint.y-70)){
			g_pRankSprite->Draw2(hDC,g_pRankSprite->GetPosition().left,g_pRankSprite->GetPosition().bottom,TRUE,RGB(255,0,255));
		}
	}
	if(state==stateM_rank || state==stateM_pvp){
		if(g_pBackSprite->IsPointInside(MousePoint.x,MousePoint.y-70)){
			g_pBackSprite->Draw2(hDC,g_pBackSprite->GetPosition().left,g_pBackSprite->GetPosition().bottom,TRUE,RGB(255,0,255));
		}
	}
}


void GamePaint(HDC hDC) //对于GamePaint函数，最重要的是它的绘制顺序
{
	if(state==state_menu){
		g_pMenuBitmap->Draw(g_hOffscreenDC,0,0);
		g_pSoloSprite->Draw1(g_hOffscreenDC,g_pSoloSprite->GetPosition().left,g_pSoloSprite->GetPosition().bottom,TRUE,RGB(255,0,255));
		g_pPartySprite->Draw1(g_hOffscreenDC,g_pPartySprite->GetPosition().left,g_pPartySprite->GetPosition().bottom,TRUE,RGB(255,0,255));
		g_pPvpSprite->Draw1(g_hOffscreenDC,g_pPvpSprite->GetPosition().left,g_pPvpSprite->GetPosition().bottom,TRUE,RGB(255,0,255));
		g_pStorySprite->Draw1(g_hOffscreenDC,g_pStorySprite->GetPosition().left,g_pStorySprite->GetPosition().bottom,TRUE,RGB(255,0,255));
		g_pExitSprite->Draw1(g_hOffscreenDC,g_pExitSprite->GetPosition().left,g_pExitSprite->GetPosition().bottom,TRUE,RGB(255,0,255));
		g_pRankSprite->Draw1(g_hOffscreenDC,g_pRankSprite->GetPosition().left,g_pRankSprite->GetPosition().bottom,TRUE,RGB(255,0,255));
		MouseMove(hDC);

	}
	if(state==state_foreword){
			g_pBG0Layer->Draw(g_hOffscreenDC,0,0);
			g_pBG1Layer->Draw(g_hOffscreenDC,0,0,true,RGB(0,0,0));
			g_pBG2Layer->Draw(g_hOffscreenDC, 0,0,true,RGB(0,0,0));
			g_pBG3Layer->Draw(g_hOffscreenDC, 0,0,true,RGB(0,0,0));
			/*SetBkMode(hDC, TRANSPARENT);*/
			TextOut(hDC,10,10,"  你想要知道些什么吗，精灵们会告诉你的。  ",strlen("  你想要知道些什么吗，精灵们会告诉你的。  "));
			TextOut(hDC,10,40,"  但是他们需要的召唤，你可以试着礼貌地去敲一下门 。 ",strlen("  但是他们需要的召唤，你可以试着礼貌地去敲一下门 。 "));
			TextOut(hDC,10,70,"  （player1按上下左右可移动，k或l攻击；player2按w s a d 移动，g或h攻击） 。",strlen("  （player1按上下左右可移动，k或l攻击；player2按w s a d 移动，g或h攻击） 。"));
			TextOut(hDC,10,100,"  （点击精灵可查看对话） 。 ",strlen("  （点击精灵可查看对话） 。 "));
			
			if(f_drawTalk1==TRUE){
				g_pTalkBitmap1->Draw(g_hOffscreenDC,30,220,true,RGB(255,255,255));
			}
			if(f_drawTalk2==TRUE){
				g_pTalkBitmap2->Draw(g_hOffscreenDC,400,150,true,RGB(255,255,255));
			}
			if(f_drawTalk3==TRUE){
				g_pTalkBitmap3->Draw(g_hOffscreenDC,890,220,true,RGB(255,255,255));
			}
			if(f_drawTalk4==TRUE){
				g_pTalkBitmap4->Draw(g_hOffscreenDC,600,20,true,RGB(255,255,255));
			}
		DrawSprites(g_hOffscreenDC);
	}
	

	if(state==stateM_solo || state==stateM_party){
		g_pBG0Layer->Draw(g_hOffscreenDC,0,0);
		g_pBG1Layer->Draw(g_hOffscreenDC,0,0,true,RGB(0,0,0));
		g_pBG2Layer->Draw(g_hOffscreenDC, 0,0,true,RGB(0,0,0));
		g_pBG3Layer->Draw(g_hOffscreenDC, 0,0,true,RGB(0,0,0));


		char str[100];
		sprintf(str,"%d",g_iScore);
		TextOut(hDC,10,10,"score:",strlen("score:"));
		TextOut(hDC,60,10,str ,strlen(str));
		char str2[100];
		sprintf(str2,"%d",g_iNumLives);
		TextOut(hDC,200,10,"  Lives:",strlen("  Lives:"));
		TextOut(hDC,300,10,str2 ,strlen(str2));
	
		DrawSprites(g_hOffscreenDC);
	}
	
	if(state==stateM_rank){
		g_pMenuBitmap->Draw(g_hOffscreenDC,0,0);
		g_pBackSprite->Draw1(g_hOffscreenDC,g_pBackSprite->GetPosition().left,g_pBackSprite->GetPosition().bottom,TRUE,RGB(255,0,255));
		MouseMove(hDC);

		/*SetBkMode(hDC, TRANSPARENT);*/
		char score1[100];
		sprintf(score1,"%d",PlaySorce[0]);
		TextOut(hDC,700,100,"NO 1 .",strlen("NO 1 ."));
		TextOut(hDC,800,100,score1 ,strlen(score1));

		char score2[100];
		sprintf(score2,"%d",PlaySorce[1]);
		TextOut(hDC,700,130,"NO 2 .",strlen("NO 2 ."));
		TextOut(hDC,800,130,score2 ,strlen(score2));

		char score3[100];
		sprintf(score3,"%d",PlaySorce[2]);
		TextOut(hDC,700,160,"NO 3 .",strlen("NO 3 ."));
		TextOut(hDC,800,160,score3 ,strlen(score3));

		char score4[100];
		sprintf(score4,"%d",PlaySorce[3]);
		TextOut(hDC,700,190,"NO 4 .",strlen("NO 4 ."));
		TextOut(hDC,800,190,score4 ,strlen(score4));

		char score5[100];
		sprintf(score5,"%d",PlaySorce[4]);
		TextOut(hDC,700,220,"NO 5 .",strlen("NO 5 ."));
		TextOut(hDC,800,220,score5 ,strlen(score5));

		char score6[100];
		sprintf(score6,"%d",PlaySorce[5]);
		TextOut(hDC,700,250,"NO 6 .",strlen("NO 6 ."));
		TextOut(hDC,800,250,score6 ,strlen(score6));

		char score7[100];
		sprintf(score7,"%d",PlaySorce[6]);
		TextOut(hDC,700,280,"NO 7 .",strlen("NO 7 ."));
		TextOut(hDC,800,280,score7 ,strlen(score7));

		char score8[100];
		sprintf(score8,"%d",PlaySorce[7]);
		TextOut(hDC,700,310,"NO 8 .",strlen("NO 8 ."));
		TextOut(hDC,800,310,score8 ,strlen(score8));

		char score9[100];
		sprintf(score9,"%d",PlaySorce[8]);
		TextOut(hDC,700,340,"NO 9 .",strlen("NO 9 ."));
		TextOut(hDC,800,340,score9 ,strlen(score9));

		char score10[100];
		sprintf(score10,"%d",PlaySorce[9]);
		TextOut(hDC,700,370,"NO 10 .",strlen("NO 10 ."));
		TextOut(hDC,800,370,score10 ,strlen(score10));

		DrawSprites(g_hOffscreenDC);
	}


	//////PVP/////////////////////////////////////////////////////
	if(state==stateM_pvp){
		g_pBG0Layer->Draw(g_hOffscreenDC,0,0);
		g_pBG1Layer->Draw(g_hOffscreenDC,0,0,true,RGB(0,0,0));
		g_pBG2Layer->Draw(g_hOffscreenDC, 0,0,true,RGB(0,0,0));
		g_pBG3Layer->Draw(g_hOffscreenDC, 0,0,true,RGB(0,0,0));

		g_pBackSprite->SetPosition(940,540);
		g_pBackSprite->Draw1(g_hOffscreenDC,g_pBackSprite->GetPosition().left,g_pBackSprite->GetPosition().bottom,TRUE,RGB(255,0,255));
		MouseMove(hDC);
		


		char str2[100];
		sprintf(str2,"%d",g_iNumLives);
		TextOut(hDC,200,10,"  Lives:",strlen("  Lives:"));
		TextOut(hDC,300,10,str2 ,strlen(str2));

		char str4[100];
		sprintf(str4,"%d",g_iNumLives2);
		TextOut(hDC,700,10,"  Lives2:",strlen("  Lives2:"));
		TextOut(hDC,800,10,str4 ,strlen(str4));

		DrawSprites(g_hOffscreenDC);
	}

}

void EnemyCreat(){
	if(g_iScore<150){
			int i=(rand()%1000);
			if(i<40){
				g_pFlyEnemySprite1 = new Sprite(g_pFlyEnemyBitmaps1,type_enemy,rcBounds2, BA_DIE);  
				g_pFlyEnemySprite1->SetPosition(1200,rand()%500);
				g_pFlyEnemySprite1->SetRowAndCol(4,4);
				g_pFlyEnemySprite1->SetDelayFrame(2);
				g_pFlyEnemySprite1->SetFrameList(frames_animal[1],4);
				g_pFlyEnemySprite1->SetVelocity(-10,0);
				g_pFlyEnemySprite1->SetBShowAnim(true);
				g_pFlyEnemySprite1->SetZOrder(zorder_enemy);
				AddSprite(g_pFlyEnemySprite1);
				
			}
			if(i<50){
				g_pFlyEnemySprite3 = new Sprite(g_pFlyEnemyBitmaps3,type_enemy,rcBounds2, BA_DIE); 
				g_pFlyEnemySprite3->SetPosition(0,rand()%500);
				g_pFlyEnemySprite3->SetRowAndCol(4,4);
				g_pFlyEnemySprite3->SetDelayFrame(2);
				g_pFlyEnemySprite3->SetFrameList(frames_animal[3],4);
				g_pFlyEnemySprite3->SetVelocity(6,0);
				g_pFlyEnemySprite3->SetBShowAnim(true);
				g_pFlyEnemySprite3->SetZOrder(zorder_enemy);
				AddSprite(g_pFlyEnemySprite3);
				
			}
			if(i<30){
				g_pFlyEnemySprite2 = new Sprite(g_pFlyEnemyBitmaps2,type_enemy,rcBounds2, BA_DIE); 
				g_pFlyEnemySprite2->SetPosition(1200,rand()%500);
				g_pFlyEnemySprite2->SetRowAndCol(4,4);
				g_pFlyEnemySprite2->SetDelayFrame(2);
				g_pFlyEnemySprite2->SetFrameList(frames_animal[1],4);
				g_pFlyEnemySprite2->SetVelocity(-8,0);
				g_pFlyEnemySprite2->SetBShowAnim(true);
				g_pFlyEnemySprite2->SetZOrder(zorder_enemy);
				AddSprite(g_pFlyEnemySprite2);
				
			}
			
		}
		else if(g_iScore>=150 && g_iScore<300){

			int i=(rand()%1000);
			if(i<50){
				g_pFlyEnemySprite4 = new Sprite(g_pFlyEnemyBitmaps4,type_enemy,rcBounds2, BA_DIE);  
				g_pFlyEnemySprite4->SetPosition(1200,rand()%500);
				g_pFlyEnemySprite4->SetRowAndCol(4,4);
				g_pFlyEnemySprite4->SetDelayFrame(2);
				g_pFlyEnemySprite4->SetFrameList(frames_animal[1],4);
				g_pFlyEnemySprite4->SetVelocity(-5,0);
				g_pFlyEnemySprite4->SetBShowAnim(true);
				g_pFlyEnemySprite4->SetZOrder(zorder_enemy);
				AddSprite(g_pFlyEnemySprite4);	
			}
			if(i<60){
				g_pFlyEnemySprite5 = new Sprite(g_pFlyEnemyBitmaps5,type_enemy,rcBounds2, BA_DIE);  
				g_pFlyEnemySprite5->SetPosition(0,rand()%500);
				g_pFlyEnemySprite5->SetRowAndCol(4,4);
				g_pFlyEnemySprite5->SetDelayFrame(2);
				g_pFlyEnemySprite5->SetFrameList(frames_animal[3],4);
				g_pFlyEnemySprite5->SetVelocity(5,0);
				g_pFlyEnemySprite5->SetBShowAnim(true);
				g_pFlyEnemySprite5->SetZOrder(zorder_enemy);
				AddSprite(g_pFlyEnemySprite5);	
			}

			vector<Sprite*>::iterator siSprite;
			for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++){
				if((*siSprite)->GetType()==type_enemy){
					if(rand()%100==0){
						g_pEnemyBullSprite1 = new Sprite(g_pEnemyBullBitmap1,type_enemyBull,rcBounds, BA_DIE);  
						g_pEnemyBullSprite1->SetPosition((*siSprite)->GetPosition());
						g_pEnemyBullSprite1->SetVelocity(((*siSprite)->GetVelocity().x)*4,0);
						g_pEnemyBullSprite1->SetZOrder(zorder_enemy);
						AddSprite(g_pEnemyBullSprite1);
					}	
				}
			}

		}
		else if(g_iScore>=300 && g_iScore<400){
			if(rand()%100>93){
				g_pMinibossSprite1 = new Sprite(g_pMinibossBitmap1,type_miniboss,rcBounds2, BA_DIE);  
				g_pMinibossSprite1->SetPosition(rand()%1000,rand()%600-100);
				g_pMinibossSprite1->SetRowAndCol(2,2);
				g_pMinibossSprite1->SetDelayFrame(2);
				g_pMinibossSprite1->SetFrameList(frames_2X2[0],2);
				g_pMinibossSprite1->SetVelocity(0,0);
				g_pMinibossSprite1->SetBShowAnim(true);
				g_pMinibossSprite1->SetZOrder(zorder_enemy);
				AddSprite(g_pMinibossSprite1);	
			}
			vector<Sprite*>::iterator siSprite;
			for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++){
				if((*siSprite)->GetType()==type_miniboss){
					/*if(rand()%100>5){*/
						g_pEnemyBullSprite2 = new Sprite(g_pEnemyBullBitmap2,type_miniboss_bull,rcBounds, BA_DIE);  
						g_pEnemyBullSprite2->SetPosition((*siSprite)->GetPosition().right-55,(*siSprite)->GetPosition().top+55);
						g_pEnemyBullSprite2->SetVelocity(0,30);
						g_pEnemyBullSprite2->SetZOrder(zorder_enemy);
						AddSprite(g_pEnemyBullSprite2);
					/*}*/
				}
			}

		}
		else if(g_iScore>=400){
			
			int j=(rand()%1000);
			if(j<40){
				g_pMinibossSprite1 = new Sprite(g_pMinibossBitmap1,type_miniboss,rcBounds2, BA_DIE);  
				g_pMinibossSprite1->SetPosition(rand()%1000,rand()%600-100);
				g_pMinibossSprite1->SetRowAndCol(2,2);
				g_pMinibossSprite1->SetDelayFrame(2);
				g_pMinibossSprite1->SetFrameList(frames_2X2[0],2);
				g_pMinibossSprite1->SetVelocity(0,0);
				g_pMinibossSprite1->SetBShowAnim(true);
				g_pMinibossSprite1->SetZOrder(zorder_enemy);
				AddSprite(g_pMinibossSprite1);	

			}

			int i=(rand()%1500);
			if(i<40){
				g_pFlyEnemySprite4 = new Sprite(g_pFlyEnemyBitmaps4,type_enemy,rcBounds2, BA_DIE);  
				g_pFlyEnemySprite4->SetPosition(1200,rand()%500);
				g_pFlyEnemySprite4->SetRowAndCol(4,4);
				g_pFlyEnemySprite4->SetDelayFrame(2);
				g_pFlyEnemySprite4->SetFrameList(frames_animal[1],4);
				g_pFlyEnemySprite4->SetVelocity(-5,0);
				g_pFlyEnemySprite4->SetBShowAnim(true);
				g_pFlyEnemySprite4->SetZOrder(zorder_enemy);
				AddSprite(g_pFlyEnemySprite4);	
			}
			if(i<30){
				g_pFlyEnemySprite5 = new Sprite(g_pFlyEnemyBitmaps5,type_enemy,rcBounds2, BA_DIE);  
				g_pFlyEnemySprite5->SetPosition(0,rand()%500);
				g_pFlyEnemySprite5->SetRowAndCol(4,4);
				g_pFlyEnemySprite5->SetDelayFrame(2);
				g_pFlyEnemySprite5->SetFrameList(frames_animal[3],4);
				g_pFlyEnemySprite5->SetVelocity(5,0);
				g_pFlyEnemySprite5->SetBShowAnim(true);
				g_pFlyEnemySprite5->SetZOrder(zorder_enemy);
				AddSprite(g_pFlyEnemySprite5);	
			}
			if(i<45){
				g_pFlyEnemySprite1 = new Sprite(g_pFlyEnemyBitmaps1,type_enemy,rcBounds2, BA_DIE);  
				g_pFlyEnemySprite1->SetPosition(1200,rand()%500);
				g_pFlyEnemySprite1->SetRowAndCol(4,4);
				g_pFlyEnemySprite1->SetDelayFrame(2);
				g_pFlyEnemySprite1->SetFrameList(frames_animal[1],4);
				g_pFlyEnemySprite1->SetVelocity(-7,0);
				g_pFlyEnemySprite1->SetBShowAnim(true);
				g_pFlyEnemySprite1->SetZOrder(zorder_enemy);
				AddSprite(g_pFlyEnemySprite1);
				
			}
			if(i<15){
				g_pFlyEnemySprite3 = new Sprite(g_pFlyEnemyBitmaps3,type_enemy,rcBounds2, BA_DIE); 
				g_pFlyEnemySprite3->SetPosition(0,rand()%500);
				g_pFlyEnemySprite3->SetRowAndCol(4,4);
				g_pFlyEnemySprite3->SetDelayFrame(2);
				g_pFlyEnemySprite3->SetFrameList(frames_animal[3],4);
				g_pFlyEnemySprite3->SetVelocity(6,0);
				g_pFlyEnemySprite3->SetBShowAnim(true);
				g_pFlyEnemySprite3->SetZOrder(zorder_enemy);
				AddSprite(g_pFlyEnemySprite3);
				
			}

			vector<Sprite*>::iterator siSprite;
			for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++){
				if((*siSprite)->GetType()==type_miniboss){
					if(rand()%100>1){
						g_pEnemyBullSprite2 = new Sprite(g_pEnemyBullBitmap2,type_miniboss_bull,rcBounds, BA_DIE);  
						g_pEnemyBullSprite2->SetPosition((*siSprite)->GetPosition().right-55,(*siSprite)->GetPosition().top+55);
						g_pEnemyBullSprite2->SetVelocity(0,40);
						g_pEnemyBullSprite2->SetZOrder(zorder_enemy);
						AddSprite(g_pEnemyBullSprite2);
					}
				}

				if((*siSprite)->GetType()==type_enemy){
					if(rand()%100==0){
						g_pEnemyBullSprite1 = new Sprite(g_pEnemyBullBitmap1,type_enemyBull,rcBounds, BA_DIE);  
						g_pEnemyBullSprite1->SetPosition((*siSprite)->GetPosition());
						g_pEnemyBullSprite1->SetVelocity(((*siSprite)->GetVelocity().x)*4,0);
						g_pEnemyBullSprite1->SetZOrder(zorder_enemy);
						AddSprite(g_pEnemyBullSprite1);
					}	
				}
			}


		}
}

void GameCycle(HDC hDC)
{	

	//Menu
	if(state==state_menu){

		UpdateSprites();

		GamePaint(g_hOffscreenDC);
		// Blit the offscreen bitmap to the game screen
		BitBlt(hDC, 0, 0, WINDOW_WIDTH,WINDOW_HEIGHT, g_hOffscreenDC, 0, 0, SRCCOPY);

		ReleaseDC(main_window_handle, hDC);
	}

	///STORY///////////////////////////////////////////////////////////////////////////////////////
	//ForeWord(only solo and party)
	else if(state==state_foreword){

		//Player
		int x = g_pPlaySprite->GetPosition().left+30;
		int y = g_pPlaySprite->GetPosition().bottom;
		COLORREF col = 0;
		if(KEYDOWN(VK_UP))
			col = GetPixel(g_hCollisionDC,x,y-1);
		if(KEYDOWN(VK_DOWN))
			col = GetPixel(g_hCollisionDC,x,y+1);
		if(KEYDOWN(VK_RIGHT))
			col = GetPixel(g_hCollisionDC,x+1,y);
		if(KEYDOWN(VK_LEFT))
			col = GetPixel(g_hCollisionDC,x-1,y);
	
		if(col == 0)
			stop = true;
		else
			stop = false;
	
		if(stop)
			g_pPlaySprite->SetVelocity(0,0);

		//-----------------------------------------

		if(f_drawTalk1){
			if(rand()%100<2){
				g_pFlyEnemySprite4 = new Sprite(g_pFlyEnemyBitmaps4,type_enemy,rcBounds2,BA_DIE);  
				g_pFlyEnemySprite4->SetPosition(1200,rand()%500);
				g_pFlyEnemySprite4->SetRowAndCol(4,4);
				g_pFlyEnemySprite4->SetDelayFrame(2);
				g_pFlyEnemySprite4->SetFrameList(frames_animal[1],4);
				g_pFlyEnemySprite4->SetVelocity(-5,0);
				g_pFlyEnemySprite4->SetBShowAnim(true);
				g_pFlyEnemySprite4->SetZOrder(0);
				AddSprite(g_pFlyEnemySprite4);	
			}
		}

		//---------------------------------------------
		//更新精灵
		UpdateSprites();

		GamePaint(g_hOffscreenDC);

		HandleKeys();

		// Blit the offscreen bitmap to the game screen
		BitBlt(hDC, 0, 0, WINDOW_WIDTH,WINDOW_HEIGHT, g_hOffscreenDC, 0, 0, SRCCOPY);

		ReleaseDC(main_window_handle, hDC);

	}


    //SOLO  单人模式/////////////////////////////////////////////////////////////////////////////////////////

	else if(state==stateM_solo){

		//滚动背景
		g_pBG0Layer->Update();
		g_pBG1Layer->Update();
		g_pBG2Layer->Update();
		g_pBG3Layer->Update();

		//Player
		int x = g_pPlaySprite->GetPosition().left+30;
		int y = g_pPlaySprite->GetPosition().bottom;
		COLORREF col = 0;
		if(KEYDOWN(VK_UP))
			col = GetPixel(g_hCollisionDC,x,y-1);
		if(KEYDOWN(VK_DOWN))
			col = GetPixel(g_hCollisionDC,x,y+1);
		if(KEYDOWN(VK_RIGHT))
			col = GetPixel(g_hCollisionDC,x+1,y);
		if(KEYDOWN(VK_LEFT))
			col = GetPixel(g_hCollisionDC,x-1,y);
	
		if(col == 0)
			stop = true;
		else
			stop = false;
	
		if(stop)
			g_pPlaySprite->SetVelocity(0,0);

		//-----------------------------------------------
		EnemyCreat();
		
		//-----------------------------------------------

		//更新精灵
		UpdateSprites();

		GamePaint(g_hOffscreenDC);

		HandleKeys();

		// Blit the offscreen bitmap to the game screen
		BitBlt(hDC, 0, 0, WINDOW_WIDTH,WINDOW_HEIGHT, g_hOffscreenDC, 0, 0, SRCCOPY);

		ReleaseDC(main_window_handle, hDC);


	}


	//PARTY  合作模式/////////////////////////////////////////////////////////////////////////////////////////

	else if(state==stateM_party){

		//滚动背景
		g_pBG0Layer->Update();
		g_pBG1Layer->Update();
		g_pBG2Layer->Update();
		g_pBG3Layer->Update();

		//Player
		int x = g_pPlaySprite->GetPosition().left+30;
		int y = g_pPlaySprite->GetPosition().bottom;

		int z  = g_pPlaySprite->GetPosition().left+30;
		int c = g_pPlaySprite->GetPosition().bottom;
		
		COLORREF col = 0;
		COLORREF col2 = 0;
		if(KEYDOWN(VK_UP))
			col = GetPixel(g_hCollisionDC,x,y-1);
		if(KEYDOWN(VK_DOWN))
			col = GetPixel(g_hCollisionDC,x,y+1);
		if(KEYDOWN(VK_RIGHT))
			col = GetPixel(g_hCollisionDC,x+1,y);
		if(KEYDOWN(VK_LEFT))
			col = GetPixel(g_hCollisionDC,z-1,c);
		if(KEYDOWN('W'))
			col2 = GetPixel(g_hCollisionDC,z,c-1);
		if(KEYDOWN('S'))
			col2 = GetPixel(g_hCollisionDC,z,c+1);
		if(KEYDOWN('D'))
			col2 = GetPixel(g_hCollisionDC,z+1,c);
		if(KEYDOWN('A'))
			col2 = GetPixel(g_hCollisionDC,z-1,c);
	
		if(col == 0)
			stop = true;
		else
			stop = false;

		if(col2 == 0)
			stop2 = true;
		else
			stop2 = false;
	
		if(stop)
		{
				g_pPlaySprite->SetVelocity(0,0);  
		}

		if(stop2)
		{
                g_pPlaySprite2->SetVelocity(0,0);
 		}

		//-----------------------------------------------
		EnemyCreat();
		
		//-----------------------------------------------

		//更新精灵
		UpdateSprites();

		GamePaint(g_hOffscreenDC);

		HandleKeys();
		HandleKeys2();
		// Blit the offscreen bitmap to the game screen
		BitBlt(hDC, 0, 0, WINDOW_WIDTH,WINDOW_HEIGHT, g_hOffscreenDC, 0, 0, SRCCOPY);

		ReleaseDC(main_window_handle, hDC);


	}



	///PVP  对战模式/////////////////////////////////////////////////////////////////////////////////////////

	else if(state==stateM_pvp){
		g_pBG0Layer->Update();
		g_pBG1Layer->Update();
		g_pBG2Layer->Update();
		g_pBG3Layer->Update();

		//Player
		int x = g_pPlaySprite->GetPosition().left+30;
		int y = g_pPlaySprite->GetPosition().bottom;

		int z  = g_pPlaySprite->GetPosition().left+30;
		int c = g_pPlaySprite->GetPosition().bottom;
		
		COLORREF col = 0;
		COLORREF col2 = 0;
		if(KEYDOWN(VK_UP))
			col = GetPixel(g_hCollisionDC,x,y-1);
		if(KEYDOWN(VK_DOWN))
			col = GetPixel(g_hCollisionDC,x,y+1);
		if(KEYDOWN(VK_RIGHT))
			col = GetPixel(g_hCollisionDC,x+1,y);
		if(KEYDOWN(VK_LEFT))
			col = GetPixel(g_hCollisionDC,z-1,c);
		if(KEYDOWN('W'))
			col2 = GetPixel(g_hCollisionDC,z,c-1);
		if(KEYDOWN('S'))
			col2 = GetPixel(g_hCollisionDC,z,c+1);
		if(KEYDOWN('D'))
			col2 = GetPixel(g_hCollisionDC,z+1,c);
		if(KEYDOWN('A'))
			col2 = GetPixel(g_hCollisionDC,z-1,c);
	
		if(col == 0)
			stop = true;
		else
			stop = false;

		if(col2 == 0)
			stop2 = true;
		else
			stop2 = false;
	
		if(stop)
		{
				g_pPlaySprite->SetVelocity(0,0);  
		}

		if(stop2)
		{
                g_pPlaySprite2->SetVelocity(0,0);
 		}
		//------------------------------------------------------

		

		if(g_iScore<50)
		{
			int i=(rand()%1000);
			if(i<50)
			{
				g_pEnemyBullSprite1 = new Sprite(g_pEnemyBullBitmap1,type_enemyBull2,rcBounds1, BA_DIE);  
				g_pEnemyBullSprite1->SetPosition(rand()%1000,700);
				g_pEnemyBullSprite1->SetVelocity(0,-20);
				g_pEnemyBullSprite1->SetZOrder(zorder_enemy);
				AddSprite(g_pEnemyBullSprite1);
				
				g_pFlyEnemySprite1 = new Sprite(g_pFlyEnemyBitmaps1,type_enemyBull2,rcBounds2, BA_DIE);  
				g_pFlyEnemySprite1->SetPosition(1200,rand()%500);
				g_pFlyEnemySprite1->SetRowAndCol(4,4);
				g_pFlyEnemySprite1->SetDelayFrame(2);
				g_pFlyEnemySprite1->SetFrameList(frames_animal[1],4);
				g_pFlyEnemySprite1->SetVelocity(-10,0);
				g_pFlyEnemySprite1->SetBShowAnim(true);
				g_pFlyEnemySprite1->SetZOrder(zorder_enemy);
				AddSprite(g_pFlyEnemySprite1);
		
			}

			
		//------------------------------------------------------------------
		
		}
       UpdateSprites();

		GamePaint(g_hOffscreenDC);

		HandleKeys();
		HandleKeys3();
		// Blit the offscreen bitmap to the game screen
		BitBlt(hDC, 0, 0, WINDOW_WIDTH,WINDOW_HEIGHT, g_hOffscreenDC, 0, 0, SRCCOPY);

		ReleaseDC(main_window_handle, hDC);
	}


	////////////////////////////////////////////////////////////////

	if(state==state_gameOver){
		

		g_pGameOverBitmap->Draw(hDC,0,-20,TRUE,RGB(255,0,255));
		MessageBox(main_window_handle, TEXT("YOU DIED"), TEXT("GameOver"), MB_OK);
		 int result = MessageBox( NULL , TEXT("死于我手里的亲爱的使者啊，能听到我说话吗？") , TEXT("Monster") , MB_ICONINFORMATION|MB_YESNO);
		  switch(result)/*注意！使用Unicode应用TEXT包围字串*/
		{
		   case IDYES:MessageBox(main_window_handle,TEXT("别害怕，因为你已经死了，所以不必要再害怕了。怪物都没有恐惧的不是吗，怪物可以理所当然地随便夺走生命..."),TEXT("Monster"),MB_OK);
				  MessageBox(main_window_handle, TEXT("正如你们的大天使一样...知道吗，我们来这里，只是想寻找一个能栖息的地方。却被认为是诅咒，被你们赶净杀绝，最后的最后，逼迫我们的下场那就是..."), TEXT("Monster"), MB_OK);
				  MessageBox(main_window_handle, TEXT("和所谓的怪物弟兄们地狱相见吧..."), TEXT("Monster"), MB_OK);
				  break;
			case IDNO:MessageBox(main_window_handle,TEXT("就这么怕我们吗...原来你也是个胆小鬼啊,死了也真不值得可怜！"),TEXT("Monster"),MB_OK);break;
		}
		CleanupSprites();
		state=state_menu;
		g_iScore=0;
		g_iNumLives=200;
	}

	if(state==stateM_rank){



		g_pMenuBitmap->Draw(hDC,0,0);

		GamePaint(g_hOffscreenDC);
		// Blit the offscreen bitmap to the game screen
		BitBlt(hDC, 0, 0, WINDOW_WIDTH,WINDOW_HEIGHT, g_hOffscreenDC, 0, 0, SRCCOPY);

		ReleaseDC(main_window_handle, hDC);
		
	}

}



/////////////////////////////////////////////////////////////////////

LRESULT CALLBACK WindowProc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
	switch(msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:
		MouseButtonDown(LOWORD(lparam), HIWORD(lparam), TRUE);
		return 0;
		
	default:
		break;		
    }
	
	return (DefWindowProc(hwnd, msg, wparam, lparam));
}

// WINMAIN ////////////////////////////////////////////////
int WINAPI WinMain(	HINSTANCE hinstance,
				   HINSTANCE hprevinstance,
				   LPSTR lpcmdline,
				   int ncmdshow)
{
	WNDCLASSEX winclass;
	winclass.cbSize         = sizeof(WNDCLASSEX);
	winclass.style			= CS_DBLCLKS | CS_OWNDC |CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc	= WindowProc;
	winclass.cbClsExtra		= 0;
	winclass.cbWndExtra		= 0;
	winclass.hInstance		= hinstance;
	winclass.hIcon			= LoadIcon(hinstance, IDI_APPLICATION);
	winclass.hCursor		= LoadCursor(NULL, IDC_ARROW); 
	winclass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName	= NULL;
	winclass.lpszClassName	= WINDOW_CLASS_NAME;
	winclass.hIconSm        = LoadIcon(hinstance, IDI_APPLICATION);
	
	g_hInstance = hinstance;
	
	if (!RegisterClassEx(&winclass))
		return(0);
	
	HWND	   hwnd;
	if (!(hwnd = CreateWindowEx(NULL,                // extended style
		WINDOW_CLASS_NAME,   // class
		"map", // title
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		0,0,	  // initial x,y
		WINDOW_WIDTH, // initial width
		WINDOW_HEIGHT,// initial height
		NULL,	  // handle to parent 
		NULL,	  // handle to menu
		hinstance,// instance of this application
		NULL)))	// extra creation parms
		return(0);
	
	g_hWnd = hwnd;
	
	HDC hdc = GetDC(hwnd);
	
	GameStart(hdc);
	
	MSG		   msg;
	while(TRUE)
	{
		DWORD start = GetTickCount();
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{ 
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} 
		
		GameCycle(hdc);

		if(GetTickCount()-start <33)
			Sleep(33- (GetTickCount()-start));		
	}
	
	GameEnd();

	ReleaseDC(hwnd, hdc);
	
	return(msg.wParam);
}



