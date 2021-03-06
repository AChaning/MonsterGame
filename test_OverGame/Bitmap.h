#pragma once

#include <windows.h>
#pragma comment(lib,"msimg32.lib")

class Bitmap
{
protected:
	HBITMAP m_hBitmap;
	int m_iWidth, m_iHeight;

	void Free();

public:
	Bitmap(HDC hDC, LPTSTR szFileName);
	Bitmap(HDC hDC, int iWidth, int iHeight, COLORREF crColor = RGB(0, 0, 0));
	virtual ~Bitmap();

	BOOL LoadFromFile(HDC hDC, LPTSTR szFileName);
	BOOL Create(HDC hDC, int iWidth, int iHeight, COLORREF crColor);
	
	void Draw(HDC hDC, int x, int y);
	void Draw(HDC hDC, int x, int y, bool bTrans, COLORREF crTransColor);
	void DrawPart(HDC hDC, int dest_x,int dest_y, int src_x,int src_y, int part_width , int part_height,
		bool bTrans, COLORREF crTransColor);

	int  GetWidth() { return m_iWidth; };
	int  GetHeight() { return m_iHeight; };
};
