#pragma once


// CUnitTool 대화 상자

class CUnitTool : public CDialog
{
	DECLARE_DYNAMIC(CUnitTool)

public:
	CUnitTool(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CUnitTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSave();

	void SetLineDir(LINE_DIR dir);

	CComboBox m_Combo_LineDir;
	CButton m_Radio_Line;
	CButton m_Radio_Coll;
	CButton m_Radio_Portal_In;
	CButton m_Radio_Portal_Out;
	CButton m_Radio_Monster;
	afx_msg void OnCbnSelchangeComboLinedirection();
	afx_msg void OnBnClickedRadioLine();
	afx_msg void OnBnClickedRadioColl();
	afx_msg void OnBnClickedRadioPortalIn();
	afx_msg void OnBnClickedRadioPortalOut();
	afx_msg void OnBnClickedRadioMonster();

	CComboBox m_Combo_Monster;
	afx_msg void OnCbnSelchangeComboMonster();
	void SetMonster(int iSel);
};
