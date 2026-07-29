// P2PProxyCfgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "P2PProxyCfgDlg.h"
#include "afxdialogex.h"
#include <tlhelp32.h>

BOOL P2PProxyCfgDlg::s_LoginSucc = FALSE;
// P2PProxyCfgDlg 对话框

IMPLEMENT_DYNAMIC(P2PProxyCfgDlg, CDialog)

P2PProxyCfgDlg::P2PProxyCfgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(P2PProxyCfgDlg::IDD, pParent)
    , m_csP2PProxyState(_T(""))
    , m_csAuthAddr(_T(""))
    , m_csPlatformAddr(_T(""))
    , m_csPassword(_T(""))
    , m_csToken(_T(""))
    , m_csAppID(_T(""))
    , m_csUsername(_T(""))
{

}

P2PProxyCfgDlg::~P2PProxyCfgDlg()
{
}

BOOL P2PProxyCfgDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    //     if (GetProcessidFromName("HCP2PProxy.exe") != 0)
    //     {
    //         m_csP2PProxyState = "Running";
    //     }
    //     else
    //     {
    //         m_csP2PProxyState = "Stopped";
    //     }

    m_cmbPlatform.SetCurSel(0);

    m_csAuthAddr = "https://openauth.ys7.com";
    m_csPlatformAddr = "https://open.ys7.com";
    m_csPassword = "pikx189";
    m_csUsername = "234qwe@";
    //m_csPassword = "234qweASD";
    //m_csUsername = "pikx189";
    m_csAppID = "73c0186b95cb4a67bcaa06239087282a";

    ((CButton *)GetDlgItem(IDC_RAD_PRIVATE))->SetCheck(TRUE);
    GetDlgItem(IDC_COM_USERNAME)->EnableWindow(TRUE);
    GetDlgItem(IDC_EDT_PASSWORD)->EnableWindow(TRUE);
    GetDlgItem(IDC_EDT_TOKEN)->EnableWindow(FALSE);

    UpdateData(FALSE);

    OnCbnSelchangeComPlatform();

    if (s_LoginSucc)
    {
        GetDlgItem(IDOK)->SetWindowText("Logout Ezviz Cloud");
    }
    else
    {
        GetDlgItem(IDOK)->SetWindowText("Login Ezviz Cloud");
    }

    //     if (s_OpenSDKLoginSucc)
    //     {
    //         GetDlgItem(IDC_BTN_OPENSDK)->SetWindowText("OpenSDK Logout");
    //     }
    //     else
    //     {
    //         GetDlgItem(IDC_BTN_OPENSDK)->SetWindowText("OpenSDK Login");
    //     }

    return TRUE;
}

void P2PProxyCfgDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_STATIC_P2P_SERVER_STATE, m_csP2PProxyState);
    DDX_Text(pDX, IDC_EDT_ADDRESS, m_csAuthAddr);
    DDX_Text(pDX, IDC_EDT_PLATFORM, m_csPlatformAddr);
    DDX_Control(pDX, IDC_COM_USERNAME, m_cmbUserName);
    DDX_Control(pDX, IDC_COM_PLATFORM, m_cmbPlatform);
    DDX_Text(pDX, IDC_EDT_PASSWORD, m_csPassword);
    DDX_Text(pDX, IDC_EDT_TOKEN, m_csToken);
    DDX_Text(pDX, IDC_EDT_APPID, m_csAppID);
    DDX_CBString(pDX, IDC_COM_USERNAME, m_csUsername);
}


BEGIN_MESSAGE_MAP(P2PProxyCfgDlg, CDialog)
    ON_BN_CLICKED(IDC_RAD_ENTERPRISE, &P2PProxyCfgDlg::OnBnClickedRadEnterprise)
    ON_BN_CLICKED(IDOK, &P2PProxyCfgDlg::OnBnClickedOk)
    ON_CBN_SELCHANGE(IDC_COM_PLATFORM, &P2PProxyCfgDlg::OnCbnSelchangeComPlatform)
    ON_BN_CLICKED(IDC_RAD_PRIVATE, &P2PProxyCfgDlg::OnBnClickedRadPrivate)
    ON_CBN_SELCHANGE(IDC_COM_USERNAME, &P2PProxyCfgDlg::OnCbnSelchangeComUsername)
END_MESSAGE_MAP()


// P2PProxyCfgDlg 消息处理程序


void P2PProxyCfgDlg::OnBnClickedRadEnterprise()
{
    // TODO:  在此添加控件通知处理程序代码
    if (((CButton *)GetDlgItem(IDC_RAD_ENTERPRISE))->GetCheck())
    {
        GetDlgItem(IDC_COM_USERNAME)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDT_PASSWORD)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDT_TOKEN)->EnableWindow(TRUE);
    }
}


void P2PProxyCfgDlg::OnBnClickedOk()
{
    // TODO:  在此添加控件通知处理程序代码

    UpdateData(TRUE);

    if (!s_LoginSucc)
    {
        char szAuthAddr[128] = { 0 };
        char szPlatformAddr[128] = { 0 };
        char szUserName[128] = { 0 };
        char szPassword[128] = { 0 };
        char szAppID[256] = { 0 };
        char szToken[256] = { 0 };

        strcpy(szAppID, m_csAppID.GetBuffer());
        strcpy(szAuthAddr, m_csAuthAddr.GetBuffer());
        strcpy(szPlatformAddr, m_csPlatformAddr.GetBuffer());
        strcpy(szUserName, m_csUsername.GetBuffer());
        strcpy(szPassword, m_csPassword.GetBuffer());
        strcpy(szToken, m_csToken.GetBuffer());

        BOOL bRet = FALSE;
        if (((CButton *)GetDlgItem(IDC_RAD_PRIVATE))->GetCheck())
        {
            //个人用户
            NET_SDK_P2P_SERVER_2C struParam = { 0 };
            struParam.byPlatformType = m_cmbPlatform.GetCurSel();
            struParam.pAppID = szAppID;
            struParam.pAuthAddr = szAuthAddr;
            struParam.pPlatformAddr = szPlatformAddr;
            struParam.pUserName = szUserName;
            struParam.pPassword = szPassword;

            bRet = NET_DVR_SetSDKLocalCfg(NET_SDK_P2P_LOGIN_2C, &struParam);
        }
        else
        {
            //企业用户
            NET_SDK_P2P_SERVER_2B struParam = { 0 };
            struParam.byPlatformType = m_cmbPlatform.GetCurSel();

            struParam.pAppID = szAppID;
            struParam.pAuthAddr = szAuthAddr;
            struParam.pPlatformAddr = szPlatformAddr;
            struParam.pToken = szToken;

            bRet = NET_DVR_SetSDKLocalCfg(NET_SDK_P2P_LOGIN_2B, &struParam);
        }

        if (!bRet)
        {
            s_LoginSucc = FALSE;
            char szErrLan[128] = { 0 };
            if (((CButton *)GetDlgItem(IDC_RAD_PRIVATE))->GetCheck())
            {
                sprintf_s(szErrLan, "NET_DVR_SetSDKLocalCfg(NET_SDK_P2P_LOGIN_2C) Failed!, Error[%d]", NET_DVR_GetLastError());
            }
            else
            {
                sprintf_s(szErrLan, "NET_DVR_SetSDKLocalCfg(NET_SDK_P2P_LOGIN_2B) Failed!, Error[%d]", NET_DVR_GetLastError());
            }

            AfxMessageBox(szErrLan);
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, szErrLan);

            GetDlgItem(IDOK)->SetWindowText("Login Ezviz Cloud");
            return;
        }
        else
        {
            s_LoginSucc = TRUE;
            char szErrLan[128] = { 0 };
            if (((CButton *)GetDlgItem(IDC_RAD_PRIVATE))->GetCheck())
            {
                sprintf_s(szErrLan, "NET_DVR_SetSDKLocalCfg(NET_SDK_P2P_LOGIN_2C) SUCC");
            }
            else
            {
                sprintf_s(szErrLan, "NET_DVR_SetSDKLocalCfg(NET_SDK_P2P_LOGIN_2B) SUCC");
            }

            AfxMessageBox(szErrLan);

            g_pMainDlg->AddLog(0, OPERATION_SUCC_T, szErrLan);

            GetDlgItem(IDOK)->SetWindowText("Logout Ezviz Cloud");

            return;
        }
    }
    else
    {
        s_LoginSucc = FALSE;

        GetDlgItem(IDOK)->SetWindowText("Login Ezviz Cloud");

        BOOL bRet = NET_DVR_SetSDKLocalCfg(NET_SDK_P2P_LOGOUT, NULL);

        if (bRet)
        {
            g_pMainDlg->AddLog(0, OPERATION_SUCC_T, "NET_DVR_SetSDKLocalCfg, NET_SDK_P2P_LOGOUT SUCC");
        }
        else
        {
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, "NET_DVR_SetSDKLocalCfg, NET_SDK_P2P_LOGOUT Failed, Error[%d]",
                NET_DVR_GetLastError());
        }
    }


}


void P2PProxyCfgDlg::OnCbnSelchangeComPlatform()
{
    // TODO:  在此添加控件通知处理程序代码
    m_cmbUserName.ResetContent();
    m_cmbUserName.Clear();

    UpdateData(FALSE);

    if (0 == m_cmbPlatform.GetCurSel())
    {
        m_csUsername = "pikx189";
        m_csPassword = "234qwe@";
        m_cmbUserName.InsertString(0, "pikx189");
        m_cmbUserName.InsertString(1, "15988417252");
        m_cmbUserName.InsertString(2, "18969188533");
        m_csAuthAddr = "https://openauth.ys7.com";
        m_csPlatformAddr = "https://open.ys7.com";

        m_csAppID = "26810f3acd794862b608b6cfbc32a6b8";
    }
    else if (1 == m_cmbPlatform.GetCurSel())
    {
        //国外
        m_csAuthAddr = "https://openauth.ezvizlife.com";
        m_csPlatformAddr = "https://open.ezvizlife.com";
        m_csPassword = "234qweASD";
        m_csUsername = "pikx";
        m_cmbUserName.InsertString(0, "pikx");
        m_csAppID = "ab82d387309311e7ad1952540059e058";
    }
    else
    {
        //test2
        //m_csAuthAddr = "https://test2auth.ys7.com:8643";
        //m_csPlatformAddr = "https://test2.ys7.com:9000";
        //m_csPassword = "Abc123";
        //m_csUsername = "zhanglei_test2";
        //m_cmbUserName.InsertString(0, "zhanglei_test2");
        //m_csAppID = "f24940e782454a0ca7cbf7c2a292a6c7";

        m_csAuthAddr = "https://testusopenauth.ezvizlife.com";
        m_csPlatformAddr = "https://testusopen.ezvizlife.com";
        m_csPassword = "Abc123";
        m_csUsername = "zhanglei_test2";
        m_cmbUserName.InsertString(0, "zhanglei_test2");
        m_csAppID = "22911fc7efe84fa8ad3c0b2b2e3eff1d";
        m_csToken = "da.5fcl52et2wxaejif4h3pyevo19m83msx-5fijg6e4aw-1f29il5-ntmamc1tz";

        ((CButton *)GetDlgItem(IDC_RAD_ENTERPRISE))->SetCheck(TRUE);
        ((CButton *)GetDlgItem(IDC_RAD_PRIVATE))->SetCheck(FALSE);

        GetDlgItem(IDC_COM_USERNAME)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDT_PASSWORD)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDT_TOKEN)->EnableWindow(TRUE);
    }

    m_cmbUserName.SetCurSel(0);

    UpdateData(FALSE);
}


void P2PProxyCfgDlg::OnBnClickedRadPrivate()
{
    // TODO:  在此添加控件通知处理程序代码
    if (((CButton *)GetDlgItem(IDC_RAD_PRIVATE))->GetCheck())
    {
        GetDlgItem(IDC_COM_USERNAME)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDT_PASSWORD)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDT_TOKEN)->EnableWindow(FALSE);
    }
}


void P2PProxyCfgDlg::OnCbnSelchangeComUsername()
{
     // TODO:  在此添加控件通知处理程序代码

    UpdateData(TRUE);

    CString csUserName;

    m_cmbUserName.GetLBText(m_cmbUserName.GetCurSel(), csUserName);

    //m_cmbUserName.GetWindowText(csUserName);

    if (csUserName.Compare("18969188533") == 0)
    {
        m_csPassword = "sdk12345";
    }
    else if (csUserName.Compare("zhanglei_test2") == 0)
    {
        m_csPassword = "Abc123";
    }
    else if (csUserName.Compare("pikx189") == 0)
    {
        m_csPassword = "234qweASD";
    }
    else if (csUserName.Compare("pikx") == 0)
    {
        m_csPassword = "234qweASD";
    }
    else if (csUserName.Compare("15988417252") == 0)
    {
        m_csPassword = "test1234";
    }

    UpdateData(FALSE);
}
