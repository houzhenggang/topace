#pragma once
#include "afxwin.h"


// CSystemSetDLG dialog

class CSystemSetDLG : public CDialog
{
	DECLARE_DYNAMIC(CSystemSetDLG)

public:
	CSystemSetDLG(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSystemSetDLG();

// Dialog Data
	enum { IDD = IDD_DIALOG_SET_SYS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void InitUI(void);
	CComboBox m_COMBOBOX_Resolution;
	int ResolutionSet;
	
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnCbnSelchangeCombo1();
	CEdit m_EDIT_w;
	CEdit m_EDIT_h;
	afx_msg void OnEnKillfocusEdit1();
	afx_msg void OnEnKillfocusEdit2();
};
