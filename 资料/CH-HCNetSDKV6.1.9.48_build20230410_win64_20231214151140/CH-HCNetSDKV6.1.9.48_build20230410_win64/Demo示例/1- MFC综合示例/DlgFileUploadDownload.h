#pragma once
#include "afxwin.h"


// CDlgFileUploadDownload 对话框

#define WM_UPDATE_STATUS 1002
#define WM_UPLOAD_FINISH 1003
#define WM_UPDATE_DOWNDLOAD_STATUS 1004
#define WM_DOWNLOAD_FINISH 1005
typedef struct THREAD_PARAM
{
    LPVOID pThis;
    BOOL bExit;
}_THREAD_PARAM;

class CDlgFileUploadDownload : public CDialogEx
{
    DECLARE_DYNAMIC(CDlgFileUploadDownload)
public:
    CDlgFileUploadDownload(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CDlgFileUploadDownload();

    // 对话框数据
    enum { IDD = IDD_DLG_FILE_UPLOAD_DOWNLOAD };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持


    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnUpload();
    LONG    m_lUserID;
    int     m_iDevIndex;
    LONG    m_lChannel;
    LONG    m_lUploadHandle;
    LONG    m_lDownloadHandle;
    BOOL    m_bUpLoading;
    BOOL    m_bDownLoading;
    CComboBox m_comboUploadType;
    CString m_strFileUploadPath;
    CString m_strFileParaPath;
    CString m_strLicenseKeyFilePath;
    afx_msg void OnBnClickedBtnFileUpload();
    afx_msg void OnBnClickedBtnFilePathUpload();
    afx_msg void OnBnClickedBtnSelectFile();
    afx_msg void OnClose();
    void CloseThread();
    void CloseDownLoadThread();
    CWinThread* m_pThread;
    CWinThread* m_pDownThread;
    THREAD_PARAM m_ThreadParam;
    THREAD_PARAM m_DownThreadParam;
protected:
    afx_msg LRESULT OnUpdateStatus(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnUploadFinish(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnUpdateDownloadStatus(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnDownloadFinish(WPARAM wParam, LPARAM lParam);
public:
    afx_msg void OnBnClickedBtnSelectLicensekeyFile();
    CString m_strTaskID;
    CString m_strPID;
    afx_msg void OnBnClickedBtnFileDownload();
    CComboBox m_combDownloadType;
    CString m_csDowmloadFilePath;
    afx_msg void OnBnClickedBtnFilePathDownload();
    int m_iWallNo;
    CComboBox m_combWindowType;
    CComboBox m_combFileType;
    int m_iWindowNo;
    int m_iMaterialNo;
};
