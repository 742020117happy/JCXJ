#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "cjson/cJSON.h"

// CDlgFTPLogUpload 对话框
#define MAX_LEN_XML 10*1024*1024 //XML最大长度

class CDlgFTPLogUpload : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFTPLogUpload)

public:
	CDlgFTPLogUpload(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgFTPLogUpload();

// 对话框数据
	enum { IDD = IDD_DLG_FTP_LOG_UPLOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnTest();
    afx_msg void OnBnClickedBtnSave();
    afx_msg void OnBnClickedBtnGenerate();
    afx_msg void OnCbnSelchangeComboAddrtype();
    afx_msg void OnBnClickedBtnGet();
private:
    void ISAPITransparent(const int iMothod, const CString strCommandStr, CString strInputParam, CString& strOutputParam);
public:
    LONG    m_lUserID;
    int     m_iDevIndex;
    LONG    m_lChannel;
    CComboBox m_comboProtocolType;
    CComboBox m_comboIPType;
    CString m_strConCommand;
    CString m_strDevCode;
    CString m_strPassword;
    CString m_strServerIP;
    int m_iServerPort;
    CString m_strUserName;
    CString m_strCompkey;
    COleDateTime m_ctEndDate;
    COleDateTime m_ctEndTime;
    COleDateTime m_ctStartDate;
    COleDateTime m_ctStartTime;
    BOOL m_bEnableFTP;
    char m_szCommandBuf[512];
    char* m_lpOutputXml;
    CComboBox m_comboAddrType;
    CString m_strDataType;
};
