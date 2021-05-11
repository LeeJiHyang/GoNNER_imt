// CUnitTool.cpp: 구현 파일
//

#include "stdafx.h"
#include "MFCTool.h"
#include "CUnitTool.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "MFCToolView.h"
#include "CMap.h"


// CUnitTool 대화 상자

IMPLEMENT_DYNAMIC(CUnitTool, CDialog)

CUnitTool::CUnitTool(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_UNITTOOL, pParent)
{
}

CUnitTool::~CUnitTool()
{
}

void CUnitTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LINEDIRECTION, m_Combo_LineDir);
	DDX_Control(pDX, IDC_RADIO_LINE, m_Radio_Line);
	DDX_Control(pDX, IDC_RADIO_COLL, m_Radio_Coll);
	DDX_Control(pDX, IDC_RADIO_PORTAL_IN, m_Radio_Portal_In);
	DDX_Control(pDX, IDC_RADIO_PORTAL_OUT, m_Radio_Portal_Out);
	DDX_Control(pDX, IDC_RADIO_MONSTER, m_Radio_Monster);
	DDX_Control(pDX, IDC_COMBO_MONSTER, m_Combo_Monster);
}


BEGIN_MESSAGE_MAP(CUnitTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CUnitTool::OnBnClickedButtonSave)
	ON_CBN_SELCHANGE(IDC_COMBO_LINEDIRECTION, &CUnitTool::OnCbnSelchangeComboLinedirection)
	ON_BN_CLICKED(IDC_RADIO_LINE, &CUnitTool::OnBnClickedRadioLine)
	ON_BN_CLICKED(IDC_RADIO_COLL, &CUnitTool::OnBnClickedRadioColl)
	ON_BN_CLICKED(IDC_RADIO_PORTAL_IN, &CUnitTool::OnBnClickedRadioPortalIn)
	ON_BN_CLICKED(IDC_RADIO_PORTAL_OUT, &CUnitTool::OnBnClickedRadioPortalOut)
	ON_BN_CLICKED(IDC_RADIO_MONSTER, &CUnitTool::OnBnClickedRadioMonster)
	ON_CBN_SELCHANGE(IDC_COMBO_MONSTER, &CUnitTool::OnCbnSelchangeComboMonster)
END_MESSAGE_MAP()


// CUnitTool 메시지 처리기


BOOL CUnitTool::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// Radio
	m_Radio_Line.SetCheck(true);
	
	// ComboBox - Line Direction
	m_Combo_LineDir.AddString(_T("0 - 상"));
	m_Combo_LineDir.AddString(_T("1 - 우"));
	m_Combo_LineDir.AddString(_T("2 - 하"));
	m_Combo_LineDir.AddString(_T("3 - 좌"));

	SetLineDir(LINE_DIR_U);

	// ComboBox - Monster
	m_Combo_Monster.AddString(_T("0"));
	m_Combo_Monster.AddString(_T("1"));

	SetMonster(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CUnitTool::OnBnClickedButtonSave()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMFCToolView* pToolView = dynamic_cast<CMFCToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
	LJH::CMap* pMap = pToolView->m_pMap;

	CFileDialog Dlg(
		FALSE, // TRUE - 열기, FALSE - 저장
		L"dat",
		L"*.dat",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT /* 파일 덮어쓸 때 경고 */,
		L"Data File(*.dat)|*.dat||",
		this
	);

	TCHAR szCurPath[MAX_PATH] = L"";
	TCHAR szDataPath[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szCurPath);
	PathRemoveFileSpec(szCurPath);
	PathCombine(szDataPath, szCurPath, L"Data");

	Dlg.m_ofn.lpstrInitialDir = szDataPath;

	if (Dlg.DoModal() == IDOK)
	{
		CString strPath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(strPath.GetString(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (hFile == INVALID_HANDLE_VALUE) return;

		pMap->SaveMapData(hFile, pMap->GetSelectMap());

		CloseHandle(hFile);
	}
}

void CUnitTool::SetLineDir(LINE_DIR dir)
{
	m_Combo_LineDir.SetCurSel((int)dir);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMFCToolView* pToolView = dynamic_cast<CMFCToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
	pToolView->m_linedir = dir;
}


void CUnitTool::OnCbnSelchangeComboLinedirection()
{
	LINE_DIR dir = (LINE_DIR)m_Combo_LineDir.GetCurSel();
	SetLineDir(dir);
}

void CUnitTool::OnCbnSelchangeComboMonster()
{
	int i = m_Combo_Monster.GetCurSel();
	SetMonster(i);
}

void CUnitTool::SetMonster(int iSel)
{
	m_Combo_Monster.SetCurSel(iSel);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMFCToolView* pToolView = dynamic_cast<CMFCToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
	pToolView->m_iMonster = iSel;
}

void CUnitTool::OnBnClickedRadioLine()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMFCToolView* pToolView = dynamic_cast<CMFCToolView*>(pMain->m_MainSplitter.GetPane(0, 1));

	pToolView->m_iToolSel = TOOL_SEL_LINE;
}


void CUnitTool::OnBnClickedRadioColl()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMFCToolView* pToolView = dynamic_cast<CMFCToolView*>(pMain->m_MainSplitter.GetPane(0, 1));

	pToolView->m_iToolSel = TOOL_SEL_COLL;
}


void CUnitTool::OnBnClickedRadioPortalIn()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMFCToolView* pToolView = dynamic_cast<CMFCToolView*>(pMain->m_MainSplitter.GetPane(0, 1));

	pToolView->m_iToolSel = TOOL_SEL_PORTALIN;
}


void CUnitTool::OnBnClickedRadioPortalOut()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMFCToolView* pToolView = dynamic_cast<CMFCToolView*>(pMain->m_MainSplitter.GetPane(0, 1));

	pToolView->m_iToolSel = TOOL_SEL_PORTALOUT;
}


void CUnitTool::OnBnClickedRadioMonster()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMFCToolView* pToolView = dynamic_cast<CMFCToolView*>(pMain->m_MainSplitter.GetPane(0, 1));

	pToolView->m_iToolSel = TOOL_SEL_MONSTER;
}


