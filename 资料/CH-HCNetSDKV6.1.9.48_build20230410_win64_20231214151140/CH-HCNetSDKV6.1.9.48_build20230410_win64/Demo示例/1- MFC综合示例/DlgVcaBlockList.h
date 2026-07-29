#if !defined(AFX_DLGVCABLOCKLIST_H__514F1BD8_20AD_41E1_B63D_2570F02DD002__INCLUDED_)
#define AFX_DLGVCABLOCKLIST_H__514F1BD8_20AD_41E1_B63D_2570F02DD002__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgVcaBlockList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgVcaBlockList dialog

class CDlgVcaBlockList : public CDialog
{
// Construction
public:
	CDlgVcaBlockList(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgVcaBlockList)
	enum { IDD = IDD_DLG_VCA_BLOCKLIST };
	CComboBox	m_comboLevel;
	CListCtrl	m_listBlockList;
	CComboBox	m_comboNativePlace;
	CComboBox	m_comboCerfiticateType;
	CComboBox	m_comboBlockListType;
	CComboBox	m_comboSex;
	CString	m_csCertificateNumber;
	DWORD	m_dwGroupID;
	CString	m_csName;
	CString	m_csRemark;
	DWORD	m_dwRegisterID;
	CString	m_csBirthDate;
	CString	m_csFastRegPicPath;
	BOOL	m_bChkImportPicModel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgVcaBlockList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgVcaBlockList)
	afx_msg void OnBtnRegister();
	afx_msg void OnBtnSearch();
	afx_msg void OnBtnExit();
	afx_msg void OnDblclkListBlocklist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnUpdate();
	afx_msg void OnBtnDelete();
	afx_msg void OnBtnGetBlocklistPic();
	afx_msg void OnBtnFastScan();
	afx_msg void OnBtnFastRegister();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	LONG    m_lServerID;
    LONG    m_lChannel;
    int     m_iDevIndex;
	LONG    m_lFindHandle;
	HANDLE  m_hFindThread;
	LONG    m_lBlockListNum;
	char    m_chFilename[256]; //抓图保存的位置
	DWORD   m_dwFileSize;

	NET_VCA_BLOCKLIST_PARA m_struBlockListPara;
	NET_VCA_BLOCKLIST_PIC m_struBlockListPic;
	void GetBlockListInfoFromWnd(NET_VCA_BLOCKLIST_INFO& struBlockListInfo);
	NET_VCA_BLOCKLIST_FASTREGISTER_PARA m_struBlFastPara;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGVCABLOCKLIST_H__514F1BD8_20AD_41E1_B63D_2570F02DD002__INCLUDED_)
