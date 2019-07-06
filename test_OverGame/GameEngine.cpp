#include "GameEngine.h"


//////////////////声音播放引擎//////////////////////////////////////////

UINT m_uiMIDIPlayerID = 0;
/*
播放MIDI歌曲
szMIDIFileName=""时表示已经开始播放了一首歌曲，并且现在暂停了播放，需要继续播放歌曲
bRestart表示如何播放歌曲，如果继续播放，则为false；如果从头开始播放，则为true
*/
void PlayMIDISong(LPTSTR szMIDIFileName, BOOL bRestart)
{
	// See if the MIDI player needs to be opened
	if (m_uiMIDIPlayerID == 0)
	{
		// Open the MIDI player by specifying the device and filename
		MCI_OPEN_PARMS mciOpenParms;
		mciOpenParms.lpstrDeviceType = "sequencer";
		mciOpenParms.lpstrElementName = szMIDIFileName;
		if (mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT,
			(DWORD)&mciOpenParms) == 0)
			// Get the ID for the MIDI player
			m_uiMIDIPlayerID = mciOpenParms.wDeviceID;
		else
			// There was a problem, so just return
			return;
	}
	
	// Restart the MIDI song, if necessary
	if (bRestart)
	{
		MCI_SEEK_PARMS mciSeekParms;
		if (mciSendCommand(m_uiMIDIPlayerID, MCI_SEEK, MCI_SEEK_TO_START,
			(DWORD)&mciSeekParms) != 0)
			// There was a problem, so close the MIDI player
			CloseMIDIPlayer();
	}
	
	// 为了达成重复播放的目的，必须让我们的程序能够接收到
	//MM_MCINOTIFY的消息，这个函数调用的方式，就是传递
	//WM_PLAY消息给系统，叫他开始播放。

	// Play the MIDI song
	MCI_PLAY_PARMS mciPlayParms;
	mciPlayParms.dwCallback = (DWORD)g_hWnd;
	if (mciSendCommand(m_uiMIDIPlayerID, MCI_PLAY, MCI_NOTIFY, (DWORD)&mciPlayParms) != 0)
		// There was a problem, so close the MIDI player
		CloseMIDIPlayer();
}

void ReplayMIDISong()
{
	mciSendCommand(m_uiMIDIPlayerID, MCI_SEEK, MCI_SEEK_TO_START,NULL);	
	MCI_PLAY_PARMS mciPlayParms;
	mciPlayParms.dwCallback = (DWORD)g_hWnd;
	mciSendCommand(m_uiMIDIPlayerID, MCI_PLAY, MCI_NOTIFY, (DWORD)&mciPlayParms);
}

void PauseMIDISong()
{
	// Pause the currently playing song, if possible
	if (m_uiMIDIPlayerID != 0)
		mciSendCommand(m_uiMIDIPlayerID, MCI_PAUSE, 0, NULL);
}

void CloseMIDIPlayer()
{
	// Close the MIDI player, if possible
	if (m_uiMIDIPlayerID != 0)
	{
		mciSendCommand(m_uiMIDIPlayerID, MCI_CLOSE, 0, NULL);
		m_uiMIDIPlayerID = 0;
	}
}

///////////精灵管理器////////////////////////////////////////////////////////////////

vector<Sprite*>     m_vSprites;


void AddSprite(Sprite* pSprite)
{
	if (pSprite != NULL)
	{
		// See if there are sprites already in the sprite vector
		if (m_vSprites.size() > 0)
		{
			// Find a spot in the sprite vector to insert the sprite by its z-order
			vector<Sprite*>::iterator siSprite;
			for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)
				if (pSprite->GetZOrder() < (*siSprite)->GetZOrder())
				{
					// Insert the sprite into the sprite vector
					m_vSprites.insert(siSprite, pSprite);
					return;
				}
		}

		// The sprite's z-order is highest, so add it to the end of the vector
		m_vSprites.push_back(pSprite);
	}
}

void DrawSprites(HDC hDC,bool bTrans, COLORREF crTransColor)
{
	// Draw the sprites in the sprite vector
	vector<Sprite*>::iterator siSprite;
	for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)
		(*siSprite)->Draw(hDC,bTrans,crTransColor);
}



void UpdateSprites()
{
	// Check to see if the sprite vector needs to grow
	if (m_vSprites.size() >= (m_vSprites.capacity() / 2))
		m_vSprites.reserve(m_vSprites.capacity() * 2);

	// Update the sprites in the sprite vector
	RECT          rcOldSpritePos;
	SPRITEACTION  saSpriteAction;
	vector<Sprite*>::iterator siSprite;
	for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)
	{
		// Save the old sprite position in case we need to restore it
		rcOldSpritePos = (*siSprite)->GetPosition();

		// Update the sprite
		saSpriteAction = (*siSprite)->Update();

		// Handle the SA_KILL sprite action
		if (saSpriteAction & SA_KILL)
		{
			delete (*siSprite);
			siSprite = m_vSprites.erase(siSprite);
			if(siSprite!=m_vSprites.begin()){
				siSprite--;
			}
			
			continue;
		}

		// See if the sprite collided with any others
		if (CheckSpriteCollision(*siSprite))
			// Restore the old sprite position
			(*siSprite)->SetPosition(rcOldSpritePos);
	}
}

void CleanupSprites()
{
	// Delete and remove the sprites in the sprite vector
	vector<Sprite*>::iterator siSprite;
	for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)
	{
		delete (*siSprite);
		//siSprite = m_vSprites.erase(siSprite);
		//siSprite--;
	}
	m_vSprites.clear();
}

Sprite* IsPointInSprite(int x, int y)
{
	// See if the point is in a sprite in the sprite vector
	vector<Sprite*>::reverse_iterator siSprite;
	for (siSprite = m_vSprites.rbegin(); siSprite != m_vSprites.rend(); siSprite++)
		if (!(*siSprite)->IsHidden() && (*siSprite)->IsPointInside(x, y))
			return (*siSprite);

	// The point is not in a sprite
	return NULL;
}

BOOL CheckSpriteCollision(Sprite* pTestSprite)
{
	// See if the sprite has collided with any other sprites
	vector<Sprite*>::iterator siSprite;
	for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)
	{
		// Make sure not to check for collision with itself
		if (pTestSprite == (*siSprite))
			continue;

		// Test the collision
		if (pTestSprite->TestCollision(*siSprite))
			// Collision detected
			return SpriteCollision((*siSprite), pTestSprite);
	}

	// No collision
	return FALSE;
}
