
// MFCToolView.cpp : CMFCToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "MFCTool.h"
#endif

#include "MFCToolDoc.h"
#include "MFCToolView.h"
#include "CTextureManager.h"
#include "CImageManager.h"
#include "CMap.h"
#include "MainFrm.h"
#include "MiniView.h"
#include "Form.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMFCToolView
HWND g_hWND; 
IMPLEMENT_DYNCREATE(CMFCToolView, CScrollView)

BEGIN_MESSAGE_MAP(CMFCToolView, CScrollView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CMFCToolView ����/�Ҹ�

CMFCToolView::CMFCToolView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	
}

CMFCToolView::~CMFCToolView()
{
	CGraphicDevice::DestroyInstance(); 
	CTextureManager::DestroyInstance();
	CImageManager::DestroyInstance();
	SafeDelete(m_pMap);
}

BOOL CMFCToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CScrollView::PreCreateWindow(cs);
}

// CMFCToolView �׸���

void CMFCToolView::OnDraw(CDC* /*pDC*/)
{
// Document (�ʿ�� ������)
	CMFCToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

// ���콺 ����Ʈ
	POINT pt;
	GetCursorPos(&pt); // ��ũ�� ��ü ��ǥ ���� Ŀ�� ��ġ Ȯ��
	ScreenToClient(&pt); // Ŭ���̾�Ʈ�� �°� Ŀ�� ��ġ ��ȯ
	pt.x += GetScrollPos(0);
	pt.y += GetScrollPos(1);

///////////////////////////////////////////////////////////////////
	CGraphicDevice::GetInstance()->RenderBegin(); 

	// Map
	m_pMap->Render();

	// Cursor
	RenderCursor();

	CGraphicDevice::GetInstance()->RenderEnd();
///////////////////////////////////////////////////////////////////

	// GDI+
	HDC hDC = ::GetDC(g_hWND);
	::Graphics g(hDC);
	
	// Coll
	SolidBrush brush(Color(100, 255, 0, 0));
	for (auto rc : m_pMap->m_vecMapData[m_pMap->GetSelectMap()].m_vecColl) {
		g.FillRectangle(&brush, rc.left - GetScrollPos(0), rc.top - GetScrollPos(1), rc.right - rc.left, rc.bottom - rc.top);
	}

	// Coll - Drawing
	if (m_bStartColl) {
		SolidBrush brushDrawing(Color(100, 200, 0, 0));
		g.FillRectangle(&brushDrawing,
			m_ptStartColl.x - GetScrollPos(0),
			m_ptStartColl.y - GetScrollPos(1),
			pt.x - m_ptStartColl.x,
			pt.y - m_ptStartColl.y);
	}

	g.ReleaseHDC(hDC);
}


// CMFCToolView �μ�

BOOL CMFCToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CMFCToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CMFCToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CMFCToolView ����

#ifdef _DEBUG
void CMFCToolView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CMFCToolDoc* CMFCToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCToolDoc)));
	return (CMFCToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCToolView �޽��� ó����


void CMFCToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	g_hWND = m_hWnd;

// Set Window Rect
	SetScrollSizes(MM_TEXT, CSize(TILECX * TILEX, TILECY * TILEY));
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	RECT rcMain = {}; 

	pMain->GetWindowRect(&rcMain);

	SetRect(&rcMain, 0, 0, rcMain.right - rcMain.left, rcMain.bottom - rcMain.top);

//View 
	RECT rcView = {}; 
	GetClientRect(&rcView);
	float fXGap = rcMain.right - float(rcView.right); 
	float fYGap =  WINCY+ (float(rcMain.bottom) - rcView.bottom); 
	
	pMain->SetWindowPos(nullptr, 0, 0, LONG(WINCX + fXGap),LONG(fYGap), SWP_NOZORDER);

	if (FAILED(CGraphicDevice::GetInstance()->Ready()))
	{
		ERR_MSG(L"�׷��� ����̽� ���� ����..."); 
		return; 
	}

	LoadTexture();

	if (nullptr == m_pMap)
	{
		m_pMap = new LJH::CMap; 
		m_pMap->Ready(); 
		m_pMap->SetView(this);
	}

// Tool Sel
	m_iToolSel = TOOL_SEL_LINE;
}


void CMFCToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CForm* pForm = dynamic_cast<CForm*>(pMain->m_SecondSplitter.GetPane(1, 0));
	LJH::CMapData* pMapData = &m_pMap->m_vecMapData[m_pMap->GetSelectMap()];

	POINT pt;
	GetCursorPos(&pt); // ��ũ�� ��ü ��ǥ ���� Ŀ�� ��ġ Ȯ��
	ScreenToClient(&pt); // Ŭ���̾�Ʈ�� �°� Ŀ�� ��ġ ��ȯ
	pt.x += GetScrollPos(0);
	pt.y += GetScrollPos(1);

	switch (m_iToolSel)
	{
	case TOOL_SEL_LINE:
	{
		LINE line;
		line.dir = m_linedir;
		line.vPos.x = (float)m_iSelX;
		line.vPos.y = (float)m_iSelY;

		pMapData->m_vecLine.emplace_back(line);
	}
	break;

	case TOOL_SEL_COLL:
		m_bStartColl = true;
		m_ptStartColl.x = pt.x;
		m_ptStartColl.y = pt.y;
		break;

	case TOOL_SEL_PORTALIN:
		pMapData->m_ptPortalIn = { pt.x, pt.y };
		break;

	case TOOL_SEL_PORTALOUT:
		pMapData->m_ptPortalOut = { pt.x, pt.y };
		break;

	case TOOL_SEL_MONSTER:
	{
		LJH::CMobData mobdata;
		mobdata.m_iMobID = m_iMonster;
		mobdata.m_vPos.x = (float)pt.x;
		mobdata.m_vPos.y = (float)pt.y;
		pMapData->m_vecMob.emplace_back(mobdata);
	}
	break;
	}

	InvalidateRect(nullptr, 0);

	CScrollView::OnLButtonDown(nFlags, point);
}

void CMFCToolView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CForm* pForm = dynamic_cast<CForm*>(pMain->m_SecondSplitter.GetPane(1, 0));
	LJH::CMapData* pMapData = &m_pMap->m_vecMapData[m_pMap->GetSelectMap()];

	POINT pt;
	GetCursorPos(&pt); // ��ũ�� ��ü ��ǥ ���� Ŀ�� ��ġ Ȯ��
	ScreenToClient(&pt); // Ŭ���̾�Ʈ�� �°� Ŀ�� ��ġ ��ȯ
	pt.x += GetScrollPos(0);
	pt.y += GetScrollPos(1);

	switch (m_iToolSel)
	{
	case TOOL_SEL_COLL:
		if (m_bStartColl) {
			m_ptEndColl.x = pt.x;
			m_ptEndColl.y = pt.y;
			
			RECT rc;
			rc.left = m_ptStartColl.x;
			rc.top = m_ptStartColl.y;
			rc.right = m_ptEndColl.x;
			rc.bottom = m_ptEndColl.y;
			pMapData->m_vecColl.emplace_back(rc);
		}
		break;
	}

	m_bStartColl = false;

	InvalidateRect(nullptr, 0);

	CScrollView::OnLButtonUp(nFlags, point);
}

void CMFCToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	UpdateMouseSel();

	InvalidateRect(nullptr, 0);

	CScrollView::OnMouseMove(nFlags, point);
}

void CMFCToolView::LoadTexture()
{
	CTextureManager* pTextureManager = CTextureManager::GetInstance();
	CImageManager* pImageManager = CImageManager::GetInstance();

	if (FAILED(pTextureManager->InsertTexture(L"../resource/image/line.png", L"line", 16, 16))) return;
	if (FAILED(pImageManager->InsertImage(L"line", L"line00_0", 8, 0, 1))) return;
	if (FAILED(pImageManager->InsertImage(L"line", L"line00_1", 9, 0, 1))) return;
	if (FAILED(pImageManager->InsertImage(L"line", L"line00_2", 10, 0, 1))) return;
	if (FAILED(pImageManager->InsertImage(L"line", L"line00_3", 11, 0, 1))) return;
	if (FAILED(pImageManager->InsertImage(L"line", L"line00_4", 12, 0, 1))) return;
	if (FAILED(pImageManager->InsertImage(L"line", L"line00_5", 13, 0, 1))) return;
	if (FAILED(pImageManager->InsertImage(L"line", L"line00_6", 14, 0, 1))) return;
	if (FAILED(pImageManager->InsertImage(L"line", L"line00_7", 15, 0, 1))) return;

	if (FAILED(pTextureManager->InsertTexture(L"../resource/image/smallenemies.png", L"smallenemies", 16, 16))) return;
	// 00
	if (FAILED(pImageManager->InsertImage(L"smallenemies", L"enemy00_idle", 0, 0, 6))) return;
	if (FAILED(pImageManager->InsertImage(L"smallenemies", L"enemy00_walk", 6, 0, 6))) return;
	if (FAILED(pImageManager->InsertImage(L"smallenemies", L"enemy00_hit", 12, 0, 4))) return;
	// 01
	if (FAILED(pImageManager->InsertImage(L"smallenemies", L"enemy01_idle", 0, 1, 6))) return;
	if (FAILED(pImageManager->InsertImage(L"smallenemies", L"enemy01_walk", 6, 1, 6))) return;
	if (FAILED(pImageManager->InsertImage(L"smallenemies", L"enemy01_hit", 12, 1, 4))) return;
	
	if (FAILED(pTextureManager->InsertTexture(L"../resource/image/bigsprites2.png", L"bigsprites2", 16, 16))) return;
	if (FAILED(pImageManager->InsertImage(L"bigsprites2", L"portalin_idle", 12, 2, 6))) return;
	if (FAILED(pImageManager->InsertImage(L"bigsprites2", L"portalout_idle", 0, 0, 6))) return;
}

void CMFCToolView::ChangeNextLineDir()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CForm* pForm = dynamic_cast<CForm*>(pMain->m_SecondSplitter.GetPane(1, 0));

	switch (m_linedir)
	{
	case LINE_DIR_U:
		m_linedir = LINE_DIR_R;
		break;
	case LINE_DIR_R:
		m_linedir = LINE_DIR_D;
		break;
	case LINE_DIR_D:
		m_linedir = LINE_DIR_L;
		break;
	case LINE_DIR_L:
		m_linedir = LINE_DIR_U;
		break;
	}
	pForm->m_tUnitTool.m_Combo_LineDir.SetCurSel(m_linedir);
}

void CMFCToolView::RenderCursor()
{
	POINT pt;
	GetCursorPos(&pt); // ��ũ�� ��ü ��ǥ ���� Ŀ�� ��ġ Ȯ��
	ScreenToClient(&pt); // Ŭ���̾�Ʈ�� �°� Ŀ�� ��ġ ��ȯ
	pt.x += GetScrollPos(0);
	pt.y += GetScrollPos(1);

	switch (m_iToolSel)
	{
	case TOOL_SEL_LINE:
	{
		// Image
		LJH::CImage* pImageLine = CImageManager::GetInstance()->GetImage(L"line", L"line00_0");

		// Image Info
		INFO info;
		info.vPos.x = (float)m_iSelX - GetScrollPos(0);
		info.vPos.y = (float)m_iSelY - GetScrollPos(1);
		switch (m_linedir) {
		case LINE_DIR_U:
			info.vRot.z = 0.f;
			break;
		case LINE_DIR_R:
			info.vRot.z = 90.f;
			break;
		case LINE_DIR_D:
			info.vRot.z = 180.f;
			break;
		case LINE_DIR_L:
			info.vRot.z = 270.f;
			break;
		}
		pImageLine->Render(&info);
	}
		break;

	case TOOL_SEL_COLL:
		break;

	case TOOL_SEL_PORTALIN:
	{
		LJH::CImage* pImage = CImageManager::GetInstance()->GetImage(L"bigsprites2", L"portalin_idle");

		// Image Info
		INFO info;
		info.vPos.x = (float)pt.x - GetScrollPos(0);
		info.vPos.y = (float)pt.y - GetScrollPos(1);
		info.vRot.z = 180.f;
		pImage->Render(&info);
	}
		break;

	case TOOL_SEL_PORTALOUT:
	{
		LJH::CImage* pImage = CImageManager::GetInstance()->GetImage(L"bigsprites2", L"portalout_idle");

		// Image Info
		INFO info;
		info.vPos.x = (float)pt.x - GetScrollPos(0);
		info.vPos.y = (float)pt.y - GetScrollPos(1);
		pImage->Render(&info);
	}
		break;

	case TOOL_SEL_MONSTER:
	{
		// Image
		LJH::CImage* pImage = nullptr;
		if (m_iMonster == 0) {
			pImage = CImageManager::GetInstance()->GetImage(L"smallenemies", L"enemy00_idle");
		}
		else if (m_iMonster == 1) {
			pImage = CImageManager::GetInstance()->GetImage(L"smallenemies", L"enemy01_idle");
		}
		if (!pImage) break;

		// Image Info
		INFO info;
		info.vPos.x = (float)pt.x - GetScrollPos(0);
		info.vPos.y = (float)pt.y - GetScrollPos(1);
		pImage->Render(&info);
	}
		break;
	}
}

void CMFCToolView::AddLineToMap(int iX, int iY)
{
	LINE line;
	line.dir = m_linedir;
	line.vPos.x = (float)iX;
	line.vPos.y = (float)iY;

	m_pMap->m_vecMapData[m_pMap->GetSelectMap()].m_vecLine.emplace_back(line);
}

void CMFCToolView::AddCollToMap(RECT rc)
{
	m_pMap->m_vecMapData[m_pMap->GetSelectMap()].m_vecColl.emplace_back(rc);
}

void CMFCToolView::AddMobToMap(int iX, int iY, int iMobID)
{
	LJH::CMobData mobdata;
	mobdata.m_vPos.x = (float)iX;
	mobdata.m_vPos.y = (float)iY;
	mobdata.m_iMobID = iMobID;

	m_pMap->m_vecMapData[m_pMap->GetSelectMap()].m_vecMob.emplace_back(mobdata);
}

void CMFCToolView::UpdateMouseSel()
{
	// Mouse Point
	POINT pt;
	GetCursorPos(&pt); // ��ũ�� ��ü ��ǥ ���� Ŀ�� ��ġ Ȯ��
	ScreenToClient(&pt); // Ŭ���̾�Ʈ�� �°� Ŀ�� ��ġ ��ȯ
	pt.x += GetScrollPos(0);
	pt.y += GetScrollPos(1);

	m_iSelX = pt.x / TILEGAP * TILEGAP;
	m_iSelY = pt.y / TILEGAP * TILEGAP;

	switch (m_linedir)
	{
	case LINE_DIR_U:
	case LINE_DIR_D:
		m_iSelX += TILEGAP >> 1;
		break;

	case LINE_DIR_R:
	case LINE_DIR_L:
		m_iSelY += TILEGAP >> 1;
		break;
	}
}



void CMFCToolView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case 'R':
		if (m_iToolSel == TOOL_SEL_LINE)
		{
			ChangeNextLineDir();
			UpdateMouseSel();
		}
		break;
	case VK_LEFT:
		SetScrollPos(0, GetScrollPos(0) - 30);
		break;
	case VK_RIGHT:
		SetScrollPos(0, GetScrollPos(0) + 30);
		break;
	case VK_UP:
		SetScrollPos(1, GetScrollPos(1) - 30);
		break;
	case VK_DOWN:
		SetScrollPos(1, GetScrollPos(1) + 30);
		break;
	}

	InvalidateRect(nullptr, 0);

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

