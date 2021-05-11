
// MFCToolView.h : CMFCToolView Ŭ������ �������̽�
//

#pragma once

#include "CMap.h"

enum {
	TOOL_SEL_LINE,
	TOOL_SEL_COLL,
	TOOL_SEL_PORTALIN,
	TOOL_SEL_PORTALOUT,
	TOOL_SEL_MONSTER,
	TOOL_SEL_NUM
};

class CMFCToolDoc;
class CSingleTex;
class LJH::CMap;
class CMFCToolView : public CScrollView
{
protected: // serialization������ ��������ϴ�.
	CMFCToolView();
	DECLARE_DYNCREATE(CMFCToolView)
public:
// Ư���Դϴ�.
public:
	CMFCToolDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CMFCToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	void LoadTexture();
	void ChangeNextLineDir();
	void RenderCursor();

	// Add Map Info
	void AddLineToMap(int iX, int iY);
	void AddCollToMap(RECT rc);
	void AddMobToMap(int iX, int iY, int iMobID);

	LJH::CMap* m_pMap = nullptr;
	LINE_DIR m_linedir = LINE_DIR_U;
	int m_iMonster = 0;

	int m_iSelX = 0;
	int m_iSelY = 0;
	void UpdateMouseSel();

	int m_iToolSel = TOOL_SEL_LINE;

	// Collision
	bool m_bStartColl = false;
	POINT m_ptStartColl;
	POINT m_ptEndColl;
};

#ifndef _DEBUG  // MFCToolView.cpp�� ����� ����
inline CMFCToolDoc* CMFCToolView::GetDocument() const
   { return reinterpret_cast<CMFCToolDoc*>(m_pDocument); }
#endif

