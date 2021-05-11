// CMapTool.cpp: 구현 파일
//

#include "stdafx.h"
#include "MFCTool.h"
#include "CMapTool.h"
#include "afxdialogex.h"
#include "CFileInfo.h"
#include "CTextureManager.h"

// CMapTool 대화 상자

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_MAPTOOL, pParent)
{
	m_iDrawID = 0;
}

CMapTool::~CMapTool()
{
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_PICTURE_MAPTOOL, m_Picture);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_LIST1, &CMapTool::OnLbnSelchangeList1)
END_MESSAGE_MAP()


// CMapTool 메시지 처리기


void CMapTool::OnDropFiles(HDROP hDropInfo)
{
	CDialog::OnDropFiles(hDropInfo);

	UpdateData(TRUE);

	_uint iCount = DragQueryFile(hDropInfo, 0xffffffff/* -1을 넣어주면 전체 갯수를 반환함 */, nullptr, 0);
	TCHAR szFilePath[MAX_PATH] = L"";
	CString strRelativePath = L"";
	TCHAR szFileName[MAX_PATH] = L"";

	for (_uint i = 0; i < iCount; ++i)
	{
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH); // 드롭된 파일 i번째의 파일 주소를 szFilePath로 반환
		strRelativePath = CFileInfo::ConvertRelativePath(szFilePath);
		
		// 파일 이름/확장자만 남김
		CString strFileNameAndExtension = PathFindFileName(strRelativePath);

		// 파일 이름만
		lstrcpy(szFileName, strFileNameAndExtension.GetString());
		PathRemoveExtension(szFileName);

		m_ListBox.AddString(szFileName);
	}

	HorizontalScroll();

	UpdateData(FALSE);
}

void CMapTool::HorizontalScroll()
{
	CString strName;
	CSize tSize;
	_int iCX = 0;

	CDC* pDC = m_ListBox.GetDC();
	
	for (int i = 0; i < m_ListBox.GetCount(); ++i)
	{
		m_ListBox.GetText(i, strName);
		tSize = pDC->GetTextExtent(strName); // 문자열을 픽셀단위 크기로 변환해서 알려주는 함수

		if (iCX < tSize.cx)
			iCX = tSize.cx;
	}

	m_ListBox.ReleaseDC(pDC);

	m_ListBox.SetHorizontalExtent(iCX);
}


void CMapTool::OnLbnSelchangeList1()
{
	UpdateData(TRUE);

	CString strFileName = L"";
	_int iIndex = m_ListBox.GetCurSel();

	m_ListBox.GetText(iIndex, strFileName);

	int i = 0;
	for (i = 0; i < strFileName.GetLength(); ++i)
	{
		if (isdigit(strFileName[i]))
			break;
	}

	strFileName.Delete(0, i);
	m_iDrawID = _ttoi(strFileName);
	
// Draw
	CGraphic_Device::Get_Instance()->Render_Begin();

	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"Terrain", L"Tile", m_iDrawID);
	if (pTexInfo == nullptr) return;

	_float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	_float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	_float fRatioX = WINCX / _float(TILECX);
	_float fRatioY = WINCY / _float(TILECY);

	_matrix matScale, matTrans, matWorld;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&matScale, fRatioX, fRatioX, 0.f);
	D3DXMatrixTranslation(&matTrans, _float(WINCX >> 1), _float(WINCY >> 1), 0.f);
	matWorld = matScale * matTrans;
	CGraphicDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &_vec3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CGraphicDevice::GetInstance()->Render_End(m_Picture.GetSafeHwnd());

	UpdateData(FALSE);
}
