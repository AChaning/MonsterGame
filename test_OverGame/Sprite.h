#pragma once

#include "Bitmap.h"

//-----------------------------------------------------------------
// Custom Data Types
//-----------------------------------------------------------------
typedef WORD        BOUNDSACTION;//边界动作
const BOUNDSACTION  BA_STOP   = 0,//停止
					BA_WRAP   = 1,//环绕
					BA_BOUNCE = 2,//弹开
					BA_DIE    = 3;//删除

typedef WORD        SPRITEACTION;  //碰撞动作（add）
const SPRITEACTION  SA_NONE   = 0x0000L,
                    SA_KILL   = 0x0001L;

//-----------------------------------------------------------------
// Sprite Class
//-----------------------------------------------------------------
class Sprite
{
protected:
	// Member Variables
	Bitmap*       m_pBitmap;
	Bitmap*       m_pBitmap2;
	RECT          m_rcPosition;
	POINT         m_ptVelocity;
	int           m_iZOrder;
	RECT          m_rcBounds;
	BOUNDSACTION  m_baBoundsAction;
	BOOL          m_bHidden;

	RECT	      m_rcCollision;  //碰撞矩阵（add）

	int          m_type; //精灵类型
	BOOL         m_death;//死亡判断

	int m_iRow,m_iCol;//精灵帧图片的长宽，即每行帧数和总行数
	bool m_bShowAnim;//是否播放动画帧，停止时可以不播放动画帧
	int* pFrameList; //帧序列数组，即当前播放的是哪个帧序列

	/*帧动画属性*/
	int m_iNumFrame;//总帧数
	int m_iCurFrame;//当前帧索引
	int m_iDelayFrame;//帧切换速度
	int m_iTriggerFrame;//帧计数器
	void UpdateFrame();//切换帧
	//void UpdateFrame2();
	///////////////////////////////////////////////////

	virtual void  CalcCollisionRect();//计算碰撞矩阵

public:


	// Constructor(s)/Destructor
	Sprite(Bitmap* pBitmap,int type);
	Sprite(Bitmap* pBitmap,Bitmap* pBitmap2,int type, RECT& rcBounds, BOUNDSACTION baBoundsAction = BA_STOP);
	Sprite(Bitmap* pBitmap,int type, RECT& rcBounds, BOUNDSACTION baBoundsAction = BA_STOP);//two bitmap
	Sprite(Bitmap* pBitmap,int type, POINT ptPosition, POINT ptVelocity, int iZOrder,
	RECT& rcBounds, BOUNDSACTION baBoundsAction = BA_STOP);
	virtual ~Sprite();
	
	// General Methods
	void          Draw(HDC hDC,bool bTrans=true, COLORREF crTransColor=RGB(255,0,255));
	void			Draw1(HDC hDC, int x, int y, bool bTrans, COLORREF crTransColor);
	void			Draw2(HDC hDC, int x, int y, bool bTrans, COLORREF crTransColor);
	virtual SPRITEACTION  Update();
	BOOL			 IsPointInside(int x, int y);//击中测试，判断鼠标是否单击了精灵
	BOOL	         TestCollision(Sprite* pTestSprite);

	// Accessor Methods
	RECT&   GetCollision() { return m_rcCollision; };
	RECT&   GetPosition(){ return m_rcPosition; };
	void    SetPosition(int x, int y);
	void    SetPosition(POINT ptPosition);
	void    SetPosition(RECT& rcPosition){ CopyRect(&m_rcPosition, &rcPosition); };
	
	void    OffsetPosition(int x, int y);
	POINT   GetVelocity() { return m_ptVelocity; };
	void    SetVelocity(int x, int y);
	void    SetVelocity(POINT ptVelocity);
	BOOL    GetZOrder() { return m_iZOrder; };
	void    SetZOrder(int iZOrder) { m_iZOrder = iZOrder; };
	void    SetBounds(RECT& rcBounds) { CopyRect(&m_rcBounds, &rcBounds); };
	void    SetBoundsAction(BOUNDSACTION ba) { m_baBoundsAction = ba; };
	BOOL    IsHidden() { return m_bHidden; };
	void    SetHidden(BOOL bHidden) { m_bHidden = bHidden; };

	int     GetType(){ return  m_type;};
	BOOL    SetDeath(BOOL s_death){ return m_death=s_death; }
	
	///////////////////////////////////////////////////////////

	void SetRowAndCol(int row, int col){m_iRow = row;m_iCol = col;}
	void SetFrameList(int array[], int iNumFrame);
	void SetDelayFrame(int iDelayFrame);
	void SetBShowAnim(bool show){m_bShowAnim = show;}

	int     GetWidth() { return m_pBitmap->GetWidth()/m_iRow; };
	int     GetHeight() { return m_pBitmap->GetHeight()/m_iCol; };
	


};

/////////////////////////////////////////////////////////

inline void Sprite::CalcCollisionRect()
{
	int iXShrink = (m_rcPosition.left - m_rcPosition.right) / 12;
	int iYShrink = (m_rcPosition.top - m_rcPosition.bottom) / 12;
	CopyRect(&m_rcCollision, &m_rcPosition);
	InflateRect(&m_rcCollision, iXShrink, iYShrink);
}

inline BOOL Sprite::TestCollision(Sprite* pTestSprite)
{
	RECT& rcTest = pTestSprite->GetCollision();
	return m_rcCollision.left <= rcTest.right &&
		rcTest.left <= m_rcCollision.right &&
		m_rcCollision.top <= rcTest.bottom &&
		rcTest.top <= m_rcCollision.bottom;
}

inline BOOL Sprite::IsPointInside(int x, int y)
{
	POINT ptPoint;
	ptPoint.x = x;
	ptPoint.y = y;
	return PtInRect(&m_rcPosition, ptPoint);
}

inline void Sprite::SetPosition(int x, int y)
{
	OffsetRect(&m_rcPosition, x - m_rcPosition.left, y - m_rcPosition.top);
	CalcCollisionRect();
}

inline void Sprite::SetPosition(POINT ptPosition)
{
	OffsetRect(&m_rcPosition, ptPosition.x - m_rcPosition.left,
		ptPosition.y - m_rcPosition.top);
	CalcCollisionRect();
}

inline void Sprite::OffsetPosition(int x, int y)
{
	OffsetRect(&m_rcPosition, x, y);
	CalcCollisionRect();
}

inline void Sprite::SetVelocity(int x, int y)
{
	m_ptVelocity.x = x;
	m_ptVelocity.y = y;
}

inline void Sprite::SetVelocity(POINT ptVelocity)
{
	m_ptVelocity.x = ptVelocity.x;
	m_ptVelocity.y = ptVelocity.y;
}