// DlgInfoDiffusionGuard.cpp : 实现文件
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "DlgInfoDiffusionGuard.h"
#include "afxdialogex.h"

#define  WM_MSG_UPDATE_OUTBUFFER WM_USER+999
// CDlgInfoDiffusionGuard 对话框

IMPLEMENT_DYNAMIC(CDlgInfoDiffusionGuard, CDialogEx)

CDlgInfoDiffusionGuard::CDlgInfoDiffusionGuard(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgInfoDiffusionGuard::IDD, pParent)
    , m_dwCommand(0)
    , m_strInbuffer1(_T("POST /ISAPI/AccessControl/UserInfo/Record?format=json"))
    , m_dwDataType(0)
    , m_strInbuffer2(_T(""))
    , m_strOutBuffer(_T(""))
   // , m_strStatus(_T(""))
{
    m_dwLinkHandle = -1;
    m_bStatusIndex = true;
    m_bDataIndex = true;
}

CDlgInfoDiffusionGuard::~CDlgInfoDiffusionGuard()
{
}

void CDlgInfoDiffusionGuard::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_CBIndex(pDX, IDC_CMB_COMMAND, m_dwCommand);
    DDX_Text(pDX, IDC_EDT_INBUFFER1, m_strInbuffer1);
    DDX_CBIndex(pDX, IDC_CMB_DATA_TYPE, m_dwDataType);
    DDX_Text(pDX, IDC_EDT_INBUFFER2, m_strInbuffer2);
    DDX_Text(pDX, IDC_EDT_OUTBUFFER, m_strOutBuffer);
    //DDX_Text(pDX, IDC_STATIC_STATUS, m_strStatus);
}


BEGIN_MESSAGE_MAP(CDlgInfoDiffusionGuard, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_STARTLINK, &CDlgInfoDiffusionGuard::OnBnClickedBtnStartlink)
    ON_BN_CLICKED(IDC_BTN_SENDDATA, &CDlgInfoDiffusionGuard::OnBnClickedBtnSenddata)
    ON_BN_CLICKED(IDC_BTN_CLOSELINK, &CDlgInfoDiffusionGuard::OnBnClickedBtnCloselink)
    ON_MESSAGE(WM_MSG_UPDATE_OUTBUFFER, OnMsgUpdateOutBuffer)
END_MESSAGE_MAP()


// CDlgInfoDiffusionGuard 消息处理程序

BOOL CDlgInfoDiffusionGuard::OnInitDialog()
{
    CDialog::OnInitDialog();
    m_iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
    m_lUserID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;
    return TRUE;
}

LRESULT CDlgInfoDiffusionGuard::OnMsgUpdateOutBuffer(WPARAM wParam, LPARAM lParam)
{
    UpdateData(FALSE);
    return 0;
}

void CALLBACK RemoteConfigCallback(DWORD dwType, void* lpBuffer, DWORD dwBufLen, void* pUserData)
{
    CString strItem = "";
    CDlgInfoDiffusionGuard* pDlgGaurd = (CDlgInfoDiffusionGuard*)pUserData;
    pDlgGaurd->processLinkData(dwType, lpBuffer, dwBufLen);
}
void CDlgInfoDiffusionGuard::processLinkData(DWORD dwType, void* lpBuffer, DWORD dwBufLen)
{
    if (dwType == NET_SDK_CALLBACK_TYPE_STATUS)
    {
        //strItem = _T("");

        DWORD dwStatus = *(DWORD*)lpBuffer;
        if (dwStatus == NET_SDK_CALLBACK_STATUS_PROCESSING)
        {
            SetDlgItemText(IDC_STATIC_STATUS, _T("1001"));
        }
        //返回下面状态时关闭长连接
        else if (dwStatus == NET_SDK_CALLBACK_STATUS_FAILED)
        {
            SetDlgItemText(IDC_STATIC_STATUS, _T("1002"));
        }
        else if (dwStatus == NET_SDK_CALLBACK_STATUS_SUCCESS)
        {
            SetDlgItemText(IDC_STATIC_STATUS, _T("1000"));
        }
        else if (dwStatus == NET_SDK_CALLBACK_STATUS_EXCEPTION)
        {
            SetDlgItemText(IDC_STATIC_STATUS, _T("1003"));
        }
        else
        {
            SetDlgItemText(IDC_STATIC_STATUS, _T("unknown"));
        }
        m_bStatusIndex = true;
    }
    else if (dwType == NET_SDK_CALLBACK_TYPE_DATA)
    {
        char byTempOutBuffer[10 * 1024] = { 0 };
        if (dwBufLen >= 10 * 1024)
        {
            dwBufLen = 10 * 1024 - 1;
        }
        memcpy(byTempOutBuffer, lpBuffer, dwBufLen);
        string str_gb2312 = UTF2GB(byTempOutBuffer);
        m_strOutBuffer = str_gb2312.c_str();
        m_bDataIndex = true;
    }
    PostMessage(WM_MSG_UPDATE_OUTBUFFER, 0, 0);
}
void CDlgInfoDiffusionGuard::OnBnClickedBtnStartlink()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    int dwTempCommand = 0;
    switch (m_dwCommand)
    {
    case 0:
        dwTempCommand = NET_DVR_JSON_CONFIG;
        break;
    case 1:
        dwTempCommand = NET_DVR_FACE_DATA_RECORD;
        break;
    default:
        break;
    }
    m_dwLinkHandle = NET_DVR_StartRemoteConfig(m_lUserID, dwTempCommand, m_strInbuffer1.GetBuffer(0), m_strInbuffer1.GetLength(), RemoteConfigCallback, this);
    if (m_dwLinkHandle >= 0)
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_StartRemoteConfig success");
    }
    else
    {
        int dwError = NET_DVR_GetLastError();
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_StartRemoteConfig error[%d]", dwError);
    }
}


void CDlgInfoDiffusionGuard::OnBnClickedBtnSenddata()
{
    // TODO:  在此添加控件通知处理程序代码
    char szLan[128] = { 0 };
    if (m_dwLinkHandle < 0)
    {
        g_StringLanType(szLan, "请开启连接", "please start link");
        AfxMessageBox(szLan);
        return;
    }
    if (!m_bDataIndex || !m_bStatusIndex)
    {
        g_StringLanType(szLan, "等待上次交互响应", "waiting for answer");
        AfxMessageBox(szLan);
        return;
    }
    UpdateData(TRUE);
    SetDlgItemText(IDC_STATIC_STATUS, _T("unknown"));
    m_strOutBuffer.Empty();
    UpdateData(FALSE);
    switch (m_dwCommand)
    {
    case 0:
        if (NET_DVR_SendRemoteConfig(m_dwLinkHandle, ENUM_SEND_JSON_DATA, m_strInbuffer2.GetBuffer(0), m_strInbuffer2.GetLength()))
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_SendRemoteConfig ENUM_SEND_JSON_DATA success");
            m_bStatusIndex = false;
            m_bDataIndex = false;
            return;
        }
        else
        {
            int dwError = NET_DVR_GetLastError();
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_SendRemoteConfig ENUM_SEND_JSON_DATA error[%d]", dwError);
            return;
        }
        break;
    default:
        break;
    }
    
}


void CDlgInfoDiffusionGuard::OnBnClickedBtnCloselink()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    char szLan[128] = { 0 };
    if (m_dwLinkHandle < 0)
    {
        g_StringLanType(szLan, "暂无连接", "no link");
        AfxMessageBox(szLan);
    }
    else
    {
        if (NET_DVR_StopRemoteConfig(m_dwLinkHandle))
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_StopRemoteConfig success");
            m_bStatusIndex = true;
            m_bDataIndex = true;
        }
        else
        {
            int dwError = NET_DVR_GetLastError();
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_StopRemoteConfig error[%d]", dwError);
        }
    }
    SetDlgItemText(IDC_STATIC_STATUS, _T("unknown"));
    m_strOutBuffer.Empty();
    UpdateData(FALSE);
}
