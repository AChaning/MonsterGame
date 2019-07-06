#pragma once

#include "Sprite.h"

#include <vector>
using namespace std;


// MACROS /////////////////////////////////////////////////

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

extern HWND g_hWnd;

//////////////////声音播放引擎//////////////////////////////////////////

//当m_uiMIDIPlayerID不为0，表示打开了设备，准备好播放音乐。
//如果为0，则关闭了设备
extern UINT m_uiMIDIPlayerID;//记录MIDI设备ID

void PlayMIDISong(LPTSTR szMIDIFileName = TEXT(""), BOOL bRestart = TRUE);
void ReplayMIDISong();
void PauseMIDISong();
void CloseMIDIPlayer();

////////////////精灵管理器/////////////////////////////////////////

extern vector<Sprite*>     m_vSprites;

//精灵碰撞响应接口，由特定的游戏实现
BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee);

BOOL CheckSpriteCollision(Sprite* pTestSprite);

void AddSprite(Sprite* pSprite);

void DrawSprites(HDC hdc,bool bTrans=true, COLORREF crTransColor=RGB(255,0,255));
void DrawSprites2(HDC hdc,bool bTrans=true, COLORREF crTransColor=RGB(255,0,255));

void UpdateSprites();

void CleanupSprites();

Sprite* IsPointInSprite(int x, int y);

//////////////////////////////////////////////////////////////////////
