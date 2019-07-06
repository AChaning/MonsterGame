#pragma once

#include "Sprite.h"

#include <vector>
using namespace std;


// MACROS /////////////////////////////////////////////////

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

extern HWND g_hWnd;

//////////////////������������//////////////////////////////////////////

//��m_uiMIDIPlayerID��Ϊ0����ʾ�����豸��׼���ò������֡�
//���Ϊ0����ر����豸
extern UINT m_uiMIDIPlayerID;//��¼MIDI�豸ID

void PlayMIDISong(LPTSTR szMIDIFileName = TEXT(""), BOOL bRestart = TRUE);
void ReplayMIDISong();
void PauseMIDISong();
void CloseMIDIPlayer();

////////////////���������/////////////////////////////////////////

extern vector<Sprite*>     m_vSprites;

//������ײ��Ӧ�ӿڣ����ض�����Ϸʵ��
BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee);

BOOL CheckSpriteCollision(Sprite* pTestSprite);

void AddSprite(Sprite* pSprite);

void DrawSprites(HDC hdc,bool bTrans=true, COLORREF crTransColor=RGB(255,0,255));
void DrawSprites2(HDC hdc,bool bTrans=true, COLORREF crTransColor=RGB(255,0,255));

void UpdateSprites();

void CleanupSprites();

Sprite* IsPointInSprite(int x, int y);

//////////////////////////////////////////////////////////////////////
