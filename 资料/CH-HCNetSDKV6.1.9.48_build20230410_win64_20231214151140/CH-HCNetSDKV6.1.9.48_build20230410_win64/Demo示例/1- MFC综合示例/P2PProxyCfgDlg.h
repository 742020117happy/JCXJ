#pragma once
#include "afxwin.h"


// P2PProxyCfgDlg 对话框

class P2PProxyCfgDlg : public CDialog
{
	DECLARE_DYNAMIC(P2PProxyCfgDlg)

public:
	P2PProxyCfgDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~P2PProxyCfgDlg();

// 对话框数据
	enum { IDD = IDD_DLG_P2P_PROXY_CFG };

protected:
    virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
    CString m_csP2PProxyState;
    CString m_csAuthAddr;
    CString m_csPlatformAddr;
    CComboBox m_cmbUserName;
    CComboBox m_cmbPlatform;
    CString m_csPassword;
    CString m_csToken;
    CString m_csAppID;
    static BOOL s_LoginSucc;

public:
    afx_msg void OnBnClickedRadEnterprise();
    afx_msg void OnBnClickedOk();
    afx_msg void OnCbnSelchangeComPlatform();
    afx_msg void OnBnClickedRadPrivate();
    afx_msg void OnCbnSelchangeComUsername();
private:
    CString m_csUsername;
};
