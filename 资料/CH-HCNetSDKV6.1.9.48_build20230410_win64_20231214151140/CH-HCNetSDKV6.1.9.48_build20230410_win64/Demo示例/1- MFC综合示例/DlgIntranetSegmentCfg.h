#pragma once


// CDlgIntranetSegmentCfg dialog

class CDlgIntranetSegmentCfg : public CDialog
{
	DECLARE_DYNAMIC(CDlgIntranetSegmentCfg)

public:
	CDlgIntranetSegmentCfg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	enum { IDD = IDD_DLG_INTRANET_SEGMENT_CFG };
	CComboBox	m_comboPhysicalSegment;
	CComboBox	m_comboVirtualSegment;
	CString		m_csPhysicalSegment;
	CString		m_csVirtualSegment;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedGetCapacity();
	afx_msg void OnBnClickedGetIntrnet();
	afx_msg void OnBnClickedPutIntrnet();
	afx_msg void OnCbnSelchangeComboPhysicalSegment();
	afx_msg void OnCbnSelchangeComboVirtualSegment();

	DECLARE_MESSAGE_MAP()

private:
	void OnReboot();

public:
	LONG m_lUserID;
	int  m_iDevIndex;
};
