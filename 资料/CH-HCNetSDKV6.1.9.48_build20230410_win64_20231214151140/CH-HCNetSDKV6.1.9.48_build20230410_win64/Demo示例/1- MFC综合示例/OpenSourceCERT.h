#pragma once
#include "afxcmn.h"


// COpenSourceCERT 对话框
#define TIMER_DOWNLOAD_OPENSOURCE_CERT_PROGRESS 1 //上传下载进度定时器
class COpenSourceCERT : public CDialogEx
{
	DECLARE_DYNAMIC(COpenSourceCERT)

public:
	COpenSourceCERT(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COpenSourceCERT();

// 对话框数据
    enum { IDD = IDD_DLG_OPEN__SOURCE_CERT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    LONG m_lUserID;
    int m_iDeviceIndex;
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedBtnExportSceneFile();
    CString m_csOpenSouceCertSavePath;
    CProgressCtrl m_ctrlProgress;
    int m_iDownloadHandle;
    BOOL m_bDownloading;      //正在下载
    
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    CStatic m_statProgress;
    virtual BOOL OnInitDialog();
};
