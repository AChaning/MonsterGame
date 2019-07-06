#include "Sprite.h"

//-----------------------------------------------------------------
// Sprite Constructor(s)/Destructor
//-----------------------------------------------------------------
Sprite::Sprite(Bitmap* pBitmap,int type)
{
	// Initialize the member variables
	m_type=type;
	m_pBitmap = pBitmap;
	SetRect(&m_rcPosition, 0, 0, pBitmap->GetWidth(), pBitmap->GetHeight());

	CalcCollisionRect();
	
	m_ptVelocity.x = m_ptVelocity.y = 0;
	m_iZOrder = 0;
	SetRect(&m_rcBounds, 0, 0, 640, 480);
	m_baBoundsAction = BA_STOP;
	m_bHidden = FALSE;

	//////////////////////////////////////
	m_iRow = m_iCol = 1;
	m_iNumFrame = 1;
	m_iCurFrame = 0;
	m_iDelayFrame=0;
	m_iTriggerFrame=0;
	pFrameList = NULL;

	m_bShowAnim = false;
}

Sprite::Sprite(Bitmap* pBitmap,int type, RECT& rcBounds, BOUNDSACTION baBoundsAction)
{

	m_type=type;
	// Calculate a random position
	int iXPos = rand() % (rcBounds.right - rcBounds.left);
	int iYPos = rand() % (rcBounds.bottom - rcBounds.top);
	
	// Initialize the member variables
	m_pBitmap = pBitmap;
	SetRect(&m_rcPosition, iXPos, iYPos, iXPos + pBitmap->GetWidth(),
		iYPos + pBitmap->GetHeight());

	CalcCollisionRect();

	m_ptVelocity.x = m_ptVelocity.y = 0;
	m_iZOrder = 0;
	CopyRect(&m_rcBounds, &rcBounds);
	m_baBoundsAction = baBoundsAction;
	m_bHidden = FALSE;

	m_iRow = m_iCol = 1;
	m_iNumFrame = 1;
	m_iCurFrame = 0;
	m_iDelayFrame=0;
	m_iTriggerFrame=0;
	pFrameList = NULL;
	m_bShowAnim = false;
}

Sprite::Sprite(Bitmap* pBitmap,Bitmap* pBitmap2,int type, RECT& rcBounds, BOUNDSACTION baBoundsAction)
{
	m_type=type;
	// Calculate a random position
	int iXPos = rand() % (rcBounds.right - rcBounds.left);
	int iYPos = rand() % (rcBounds.bottom - rcBounds.top);
	
	// Initialize the member variables
	m_pBitmap = pBitmap;
	m_pBitmap2 = pBitmap2;
	SetRect(&m_rcPosition, iXPos, iYPos, iXPos + pBitmap->GetWidth(),
		iYPos + pBitmap->GetHeight());
	//SetRect(&m_rcPosition, iXPos, iYPos, iXPos + pBitmap2->GetWidth(),
	//	iYPos + pBitmap2->GetHeight());
	//CalcCollisionRect();
	CalcCollisionRect();

	m_ptVelocity.x = m_ptVelocity.y = 0;
	m_iZOrder = 0;
	CopyRect(&m_rcBounds, &rcBounds);
	m_baBoundsAction = baBoundsAction;
	m_bHidden = FALSE;

	m_iRow = m_iCol = 1;
	m_iNumFrame = 1;
	m_iCurFrame = 0;
	m_iDelayFrame=0;
	m_iTriggerFrame=0;
	pFrameList = NULL;
	m_bShowAnim = false;
}

Sprite::Sprite(Bitmap* pBitmap,int type, POINT ptPosition, POINT ptVelocity, int iZOrder,
			   RECT& rcBounds, BOUNDSACTION baBoundsAction)
{
	m_type=type;
	// Initialize the member variables
	m_pBitmap = pBitmap;
	SetRect(&m_rcPosition, ptPosition.x, ptPosition.y,
		ptPosition.x + pBitmap->GetWidth(), ptPosition.y + pBitmap->GetHeight());

	CalcCollisionRect();

	m_ptVelocity = ptVelocity;
	m_iZOrder = iZOrder;
	CopyRect(&m_rcBounds, &rcBounds);
	m_baBoundsAction = baBoundsAction;
	m_bHidden = FALSE;

	m_iRow = m_iCol = 1;
	m_iNumFrame = 1;
	m_iCurFrame = 0;
	m_iDelayFrame=0;
	m_iTriggerFrame=0;
	pFrameList = NULL;
	m_bShowAnim = false;
}

Sprite::~Sprite()
{
	if(pFrameList != NULL)
	{
		delete[] pFrameList;
		pFrameList = NULL;
	}
}


void Sprite::SetFrameList(int array[], int iNumFrame)
{
	m_iNumFrame = iNumFrame;
	m_iCurFrame = 0;
	if(pFrameList != NULL)
	{
		delete[] pFrameList;
		pFrameList = NULL;
	}
	pFrameList = new int[iNumFrame];
	memcpy(pFrameList, array,sizeof(int)*iNumFrame);

	//重新计算位置矩形
	RECT rcPos = GetPosition();
	rcPos.bottom = rcPos.top + GetHeight();
	rcPos.right = rcPos.left + GetWidth();
	SetPosition(rcPos);
}

void Sprite::SetDelayFrame(int iDelayFrame)
{
	m_iDelayFrame = iDelayFrame;
}


//-----------------------------------------------------------------
// Sprite General Methods
//-----------------------------------------------------------------
SPRITEACTION Sprite::Update()//完成精灵移动和碰撞检测
{
	//死亡判断
	if(m_death==TRUE){
		return SA_KILL;
	}

	UpdateFrame();

	//更新位置
	POINT ptNewPosition, ptSpriteSize, ptBoundsSize;
	ptNewPosition.x = m_rcPosition.left + m_ptVelocity.x;
	ptNewPosition.y = m_rcPosition.top + m_ptVelocity.y;
	ptSpriteSize.x = m_rcPosition.right - m_rcPosition.left;
	ptSpriteSize.y = m_rcPosition.bottom - m_rcPosition.top;
	ptBoundsSize.x = m_rcBounds.right - m_rcBounds.left;
	ptBoundsSize.y = m_rcBounds.bottom - m_rcBounds.top;
	
	// 检查边界动作
	// 环绕?
	if (m_baBoundsAction == BA_WRAP)
	{
		if ((ptNewPosition.x + ptSpriteSize.x) < m_rcBounds.left)
			ptNewPosition.x = m_rcBounds.right;//到了左边，就移动到右边边界
		else if (ptNewPosition.x > m_rcBounds.right)
			ptNewPosition.x = m_rcBounds.left - ptSpriteSize.x;
		if ((ptNewPosition.y + ptSpriteSize.y) < m_rcBounds.top)
			ptNewPosition.y = m_rcBounds.bottom;
		else if (ptNewPosition.y > m_rcBounds.bottom)
			ptNewPosition.y = m_rcBounds.top - ptSpriteSize.y;
	}
	// 弹开?（更改速度方向）
	else if (m_baBoundsAction == BA_BOUNCE)
	{
		BOOL bBounce = FALSE;
		POINT ptNewVelocity = m_ptVelocity;
		if (ptNewPosition.x < m_rcBounds.left)
		{
			bBounce = TRUE;
			ptNewPosition.x = m_rcBounds.left;
			ptNewVelocity.x = -ptNewVelocity.x;
		}
		else if ((ptNewPosition.x + ptSpriteSize.x) > m_rcBounds.right)
		{
			bBounce = TRUE;
			ptNewPosition.x = m_rcBounds.right - ptSpriteSize.x;
			ptNewVelocity.x = -ptNewVelocity.x;
		}
		if (ptNewPosition.y < m_rcBounds.top)
		{
			bBounce = TRUE;
			ptNewPosition.y = m_rcBounds.top;
			ptNewVelocity.y = -ptNewVelocity.y;
		}
		else if ((ptNewPosition.y + ptSpriteSize.y) > m_rcBounds.bottom)
		{
			bBounce = TRUE;
			ptNewPosition.y = m_rcBounds.bottom - ptSpriteSize.y;
			ptNewVelocity.y = -ptNewVelocity.y;
		}
		if (bBounce)//重设速度
			SetVelocity(ptNewVelocity);
	}

	// Die?
	else if (m_baBoundsAction == BA_DIE)
	{
		if ((ptNewPosition.x + ptSpriteSize.x) < m_rcBounds.left ||
			ptNewPosition.x > m_rcBounds.right ||
			(ptNewPosition.y + ptSpriteSize.y) < m_rcBounds.top ||
			ptNewPosition.y > m_rcBounds.bottom)
			return SA_KILL;
	}
	
	//停止(默认)
	else
	{
		//改变速度，以停止精灵
		if (ptNewPosition.x  < m_rcBounds.left ||
			ptNewPosition.x > (m_rcBounds.right - ptSpriteSize.x))
		{
			ptNewPosition.x = max(m_rcBounds.left, min(ptNewPosition.x,
				m_rcBounds.right - ptSpriteSize.x));
			SetVelocity(0, 0);
		}
		if (ptNewPosition.y  < m_rcBounds.top ||
			ptNewPosition.y > (m_rcBounds.bottom - ptSpriteSize.y))
		{
			ptNewPosition.y = max(m_rcBounds.top, min(ptNewPosition.y,
				m_rcBounds.bottom - ptSpriteSize.y));
			SetVelocity(0, 0);
		}
	}
	SetPosition(ptNewPosition);

	return SA_NONE;
}

void Sprite::Draw(HDC hDC,bool bTrans, COLORREF crTransColor)
{
	// Draw the sprite if it isn't hidden
	if (m_pBitmap != NULL && !m_bHidden)
	{
		if(m_iNumFrame <= 1)
			m_pBitmap->Draw(hDC, m_rcPosition.left, m_rcPosition.top, bTrans, crTransColor);
		else 
			m_pBitmap->DrawPart(hDC,m_rcPosition.left,m_rcPosition.top,
			(pFrameList[m_iCurFrame]%m_iRow)*GetWidth(),
			(pFrameList[m_iCurFrame]/m_iRow)*GetHeight(),
			GetWidth(),GetHeight(),bTrans,crTransColor);
	}
}

void Sprite::Draw1(HDC hDC, int x, int y, bool bTrans, COLORREF crTransColor){
	if (m_pBitmap != NULL && !m_bHidden)
		m_pBitmap->Draw(hDC, x, y, bTrans, crTransColor);
}

void Sprite::Draw2(HDC hDC, int x, int y, bool bTrans, COLORREF crTransColor){
	if (m_pBitmap2 != NULL && !m_bHidden)
		m_pBitmap2->Draw(hDC, x,y, bTrans, crTransColor);
}



void Sprite::UpdateFrame()
{
	if(m_bShowAnim)
	{
		++m_iTriggerFrame;
		if(m_iTriggerFrame>=m_iDelayFrame)//要切换到下一帧了
		{
			++m_iCurFrame;
			if(m_iCurFrame>=m_iNumFrame){
				m_iCurFrame = 0;
			}
				
			m_iTriggerFrame = 0;
		}
	}
	else //停止播放动画帧
	{
		m_iCurFrame = 0;
	}
}


