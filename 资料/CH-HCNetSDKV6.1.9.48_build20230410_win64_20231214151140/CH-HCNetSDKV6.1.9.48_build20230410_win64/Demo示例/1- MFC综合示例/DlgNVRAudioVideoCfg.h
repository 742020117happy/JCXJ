#pragma once
#include "afxwin.h"


// CDlgNVRAudioVideoCfg 对话框

class CDlgNVRAudioVideoCfg : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgNVRAudioVideoCfg)

public:
	CDlgNVRAudioVideoCfg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgNVRAudioVideoCfg();

// 对话框数据
	enum { IDD = IDD_DLG_TEST_VIDEOAUDIO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持10.16.96.21

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnSet();
    virtual BOOL OnInitDialog();
    afx_msg void OnCbnSelchangeComboTypeAv();
    afx_msg void OnBnClickedBtnExit();
private:
    void showDlgView();
public:
    LONG m_lUserID;
    int m_iDeviceIndex;
    BOOL m_bOpen;
    int m_iAudioChannel;
    int m_iVideoChannel;
    CComboBox m_comboInterface;
    CComboBox m_comboSplitNum;
    CComboBox m_comboType;
};
