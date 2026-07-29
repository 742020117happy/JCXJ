// OpenSourceCERT.cpp : 实现文件
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "OpenSourceCERT.h"
#include "afxdialogex.h"


// COpenSourceCERT 对话框

IMPLEMENT_DYNAMIC(COpenSourceCERT, CDialogEx)

COpenSourceCERT::COpenSourceCERT(CWnd* pParent /*=NULL*/)
	: CDialogEx(COpenSourceCERT::IDD, pParent)
    , m_csOpenSouceCertSavePath(_T(""))
    , m_iDownloadHandle(-1)
    , m_bDownloading(FALSE)
{

}

COpenSourceCERT::~COpenSourceCERT()
{
}

void COpenSourceCERT::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_SAVE_PATH, m_csOpenSouceCertSavePath);
    DDX_Control(pDX, IDC_PROGRESS, m_ctrlProgress);
    DDX_Control(pDX, IDC_STATIC_PROGRESS, m_statProgress);
}


BEGIN_MESSAGE_MAP(COpenSourceCERT, CDialogEx)
    ON_BN_CLICKED(IDOK, &COpenSourceCERT::OnBnClickedOk)
    ON_BN_CLICKED(IDC_BTN_EXPORT_SCENE_FILE, &COpenSourceCERT::OnBnClickedBtnExportSceneFile)
    ON_WM_TIMER()
END_MESSAGE_MAP()


// COpenSourceCERT 消息处理程序


void COpenSourceCERT::OnBnClickedOk()
{
    // TODO:  在此添加控件通知处理程序代码
    CDialogEx::OnOK();
}


void COpenSourceCERT::OnBnClickedBtnExportSceneFile()
{
    // TODO:  在此添加控件通知处理程序代码
    CString strFilter = _T("All Files(*.*)|*.*||");
    CFileDialog fileChose(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);

    if (fileChose.DoModal() != IDOK)
    {
        return;
    }
    m_csOpenSouceCertSavePath = fileChose.GetPathName();

    GetDlgItem(IDC_EDIT_SAVE_PATH)->SetWindowText(m_csOpenSouceCertSavePath);

    UpdateData(TRUE);
    m_iDownloadHandle = NET_DVR_StartDownload(m_lUserID, NET_SDK_DOWNLOAD_OPEN_SOURCE_CERT, NULL, 0, m_csOpenSouceCertSavePath.GetBuffer(0));
    if (m_iDownloadHandle == -1)
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_StartDownload NET_SDK_DOWNLOAD_OPEN_SOURCE_CERT");
        return;
    }
    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_StartDownload NET_SDK_DOWNLOAD_OPEN_SOURCE_CERT");

    m_bDownloading = TRUE;
    m_ctrlProgress.SetPos(0);
    m_statProgress.SetWindowText(_T("0%"));
    SetTimer(TIMER_DOWNLOAD_OPENSOURCE_CERT_PROGRESS, 100, NULL);

}


void COpenSourceCERT::OnTimer(UINT_PTR nIDEvent)
{
    // TODO:  在此添加消息处理程序代码和/或调用默认值
    if (nIDEvent == TIMER_DOWNLOAD_OPENSOURCE_CERT_PROGRESS)
    {
        DWORD dwProgress = 0;
        LONG lStatus = -1;
       if (m_bDownloading)
        {
            lStatus = NET_DVR_GetDownloadState(m_iDownloadHandle, &dwProgress);
            if (lStatus == -1)
            {
                g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GetDownloadState");
                if (!NET_DVR_StopDownload(m_iDownloadHandle))
                {
                    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_StopDownload");
                }
                else
                {
                    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_StopDownload");
                }
            }
            else
            {
                g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_GetDownloadState");
            }
        }
        switch (lStatus)
        {
        case 1: //上传或下载成功
        {
            m_ctrlProgress.SetPos(dwProgress);
            CString str;
            str.Format("%d", dwProgress);
            str += _T("%");
            m_statProgress.SetWindowText(str);
           if (m_bDownloading)
            {
                if (!NET_DVR_StopDownload(m_iDownloadHandle))
                {
                    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_StopDownload");
                }
                else
                {
                    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_StopDownload");
                }
                m_bDownloading = FALSE;
            }

            KillTimer(TIMER_DOWNLOAD_OPENSOURCE_CERT_PROGRESS);
            char sTitle[64] = { 0 };
            char sMsg[64] = { 0 };
            g_StringLanType(sTitle, "开源文件", "open souce cert file");
            g_StringLanType(sMsg, "上传/下载完成", "Upload/Download finished.");
            MessageBox(sMsg, sTitle, MB_OK | MB_ICONWARNING);
            break;
        }
        case 2: //正在上传或下载
        {
            m_ctrlProgress.SetPos(dwProgress);
            CString str;
            str.Format("%d", dwProgress);
            str += _T("%");
            m_statProgress.SetWindowText(str);
            break;
        }
        case 3: //上传或下载失败
        {
            if (m_bDownloading)
            {
                if (!NET_DVR_StopDownload(m_iDownloadHandle))
                {
                    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_StopDownload");
                }
                else
                {
                    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_StopDownload");
                }
                m_bDownloading = FALSE;
            }
            m_iDownloadHandle = -1;
            KillTimer(TIMER_DOWNLOAD_OPENSOURCE_CERT_PROGRESS);
            char sTitle[64] = { 0 };
            char sMsg[64] = { 0 };
            g_StringLanType(sTitle, "开源文件", "open souce cert file");
            g_StringLanType(sMsg, "上传/下载失败", "Upload/Download failed.");
            MessageBox(sMsg, sTitle, MB_OK | MB_ICONWARNING);
            break;
        }
        case 4: //网络断开
        {
             if (m_bDownloading)
            {
                if (!NET_DVR_StopDownload(m_iDownloadHandle))
                {
                    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_StopDownload");
                }
                else
                {
                    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_StopDownload");
                }
                m_bDownloading = FALSE;
            }
             m_iDownloadHandle = -1;
            KillTimer(TIMER_DOWNLOAD_OPENSOURCE_CERT_PROGRESS);
            char sTitle[64] = { 0 };
            char sMsg[64] = { 0 };
            g_StringLanType(sTitle, "开源文件", "open souce cert file");
            g_StringLanType(sMsg, "网络断开", "Network disconnection.");
            MessageBox(sMsg, sTitle, MB_OK | MB_ICONWARNING);
            break;
        }
        }
    }


    CDialogEx::OnTimer(nIDEvent);
}


BOOL COpenSourceCERT::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    m_ctrlProgress.SetRange(0, 100);
    m_ctrlProgress.SetStep(1);
    m_ctrlProgress.SetPos(0);
    m_statProgress.SetWindowText(_T("0%"));
    m_iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常:  OCX 属性页应返回 FALSE
}
