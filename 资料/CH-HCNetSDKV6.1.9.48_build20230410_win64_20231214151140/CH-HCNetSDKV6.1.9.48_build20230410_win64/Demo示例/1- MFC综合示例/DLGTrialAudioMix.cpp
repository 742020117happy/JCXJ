// DLGTrialAudioMix.cpp : 实现文件
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "DLGTrialAudioMix.h"
#include "afxdialogex.h"
#include "xml/XmlBase.h"

// CDLGTrialAudioMix 对话框

IMPLEMENT_DYNAMIC(CDLGTrialAudioMix, CDialogEx)

CDLGTrialAudioMix::CDLGTrialAudioMix(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDLGTrialAudioMix::IDD, pParent)
    , m_AudioMixInfo(_T(""))
{

}

CDLGTrialAudioMix::~CDLGTrialAudioMix()
{
}

void CDLGTrialAudioMix::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_AUDIOMIX_ID, m_AudioMixID);
    DDX_Text(pDX, IDC_EDIT_AUDIOMIX_INFO, m_AudioMixInfo);
    DDX_Control(pDX, IDC_COMBO_SERIALPORT_ID, m_SerialPortID);
}


BEGIN_MESSAGE_MAP(CDLGTrialAudioMix, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_GET_SINGLE_AUDIOMIXCFG, &CDLGTrialAudioMix::OnBnClickedBtnGetSingleAudiomixcfg)
    ON_BN_CLICKED(IDC_BTN_SET_SINGLE_AUDIOMIXCFG, &CDLGTrialAudioMix::OnBnClickedBtnSetSingleAudiomixcfg)
    ON_BN_CLICKED(IDC_BTN_GET_AUDIOMIXCFG, &CDLGTrialAudioMix::OnBnClickedBtnGetAudiomixcfg)
    ON_BN_CLICKED(IDC_BTN_SET_AUDIOMIXCFG, &CDLGTrialAudioMix::OnBnClickedBtnSetAudiomixcfg)
    ON_BN_CLICKED(IDC_BTN_GET_SINGLE_SERIALPORT_CFG, &CDLGTrialAudioMix::OnBnClickedBtnGetSingleSerialportCfg)
    ON_BN_CLICKED(IDC_BTN_SET_SINGLE_SERIALPORT_CFG, &CDLGTrialAudioMix::OnBnClickedBtnSetSingleSerialportCfg)
    ON_BN_CLICKED(IDC_BTN_GET_SERIALPORT_CFG, &CDLGTrialAudioMix::OnBnClickedBtnGetSerialportCfg)
    ON_BN_CLICKED(IDC_BTN_SET_SERIALPORT_CFG, &CDLGTrialAudioMix::OnBnClickedBtnSetSerialportCfg)
END_MESSAGE_MAP()


// CDLGTrialAudioMix 消息处理程序

BOOL CDLGTrialAudioMix::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    GetAudiomixCap();
    GetSerialCap();

    m_SerialPortID.SetCurSel(0);
    m_AudioMixID.SetCurSel(0);

    UpdateData(FALSE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常:  OCX 属性页应返回 FALSE
}


void CDLGTrialAudioMix::OnBnClickedBtnGetSingleAudiomixcfg()
{
    UpdateData(TRUE);
    NET_DVR_XML_CONFIG_INPUT    struInput = { 0 };
    NET_DVR_XML_CONFIG_OUTPUT   struOuput = { 0 };
    struInput.dwSize = sizeof(struInput);
    struOuput.dwSize = sizeof(struOuput);

    CString strID = _T("");
    m_AudioMixID.GetLBText(m_AudioMixID.GetCurSel(), strID);

    char szUrl[256] = { 0 };
    sprintf(szUrl, "GET /ISAPI/System/Audio/Mixing/%s\r\n", strID);
    struInput.lpRequestUrl = szUrl;
    struInput.dwRequestUrlLen = strlen(szUrl);

    DWORD dwOutputLen = 1024 * 1024;
    char *pOutBuf = new char[dwOutputLen];
    memset(pOutBuf, 0, dwOutputLen);
    struOuput.lpOutBuffer = pOutBuf;
    struOuput.dwOutBufferSize = dwOutputLen;

    if (!NET_DVR_STDXMLConfig(m_lUserID, &struInput, &struOuput))
    {
        g_pMainDlg->AddLog(m_dwDevIndex, OPERATION_FAIL_T, szUrl);
        delete[]pOutBuf;
        pOutBuf = NULL;
        return;
    }
    else
    {
        g_pMainDlg->AddLog(m_dwDevIndex, OPERATION_SUCC_T, szUrl);
        CXmlBase xmlBase;
        xmlBase.Parse(pOutBuf);
        xmlBase.SetRoot();
        m_AudioMixInfo = xmlBase.GetChildren().c_str();
        m_AudioMixInfo.Replace("\n", "\r\n");
    }

    UpdateData(FALSE);

    if (pOutBuf != NULL)
    {
        delete[]pOutBuf;
        pOutBuf = NULL;
    }

    return;
}


void CDLGTrialAudioMix::OnBnClickedBtnSetSingleAudiomixcfg()
{
    UpdateData(TRUE);
    DWORD dwBufLen = 10 * 1024;
    char *pBuf = new char[dwBufLen];
    memset(pBuf, 0, dwBufLen);

    CString strID = _T("");
    m_AudioMixID.GetLBText(m_AudioMixID.GetCurSel(), strID);

    CString temp = m_AudioMixInfo;
    temp.Replace("\r\n", "\n");
    sprintf(pBuf, temp, sizeof(temp));
    char szUrl[256] = { 0 };
    sprintf(szUrl, "PUT /ISAPI/System/Audio/Mixing/%s\r\n", strID);

    NET_DVR_XML_CONFIG_INPUT    struInput = { 0 };
    NET_DVR_XML_CONFIG_OUTPUT   struOuput = { 0 };
    struInput.dwSize = sizeof(struInput);
    struOuput.dwSize = sizeof(struOuput);
    struInput.lpRequestUrl = szUrl;
    struInput.dwRequestUrlLen = strlen(szUrl);
    struInput.lpInBuffer = pBuf;
    struInput.dwInBufferSize = dwBufLen;
    if (!NET_DVR_STDXMLConfig(m_lUserID, &struInput, &struOuput))
    {
        g_pMainDlg->AddLog(m_dwDevIndex, OPERATION_FAIL_T, szUrl);
    }
    else
    {
        g_pMainDlg->AddLog(m_dwDevIndex, OPERATION_SUCC_T, szUrl);
    }
    UpdateData(FALSE);
    if (pBuf != NULL)
    {
        delete[]pBuf;
        pBuf = NULL;
    }
}


void CDLGTrialAudioMix::OnBnClickedBtnGetAudiomixcfg()
{
    UpdateData(TRUE);
    NET_DVR_XML_CONFIG_INPUT    struInput = { 0 };
    NET_DVR_XML_CONFIG_OUTPUT   struOuput = { 0 };
    struInput.dwSize = sizeof(struInput);
    struOuput.dwSize = sizeof(struOuput);
    char szUrl[256] = { 0 };
    sprintf(szUrl, "GET /ISAPI/System/Audio/Mixing\r\n");
    struInput.lpRequestUrl = szUrl;
    struInput.dwRequestUrlLen = strlen(szUrl);
    DWORD dwOutputLen = 1024 * 1024;
    char *pOutBuf = new char[dwOutputLen];
    memset(pOutBuf, 0, dwOutputLen);
    struOuput.lpOutBuffer = pOutBuf;
    struOuput.dwOutBufferSize = dwOutputLen;

    if (!NET_DVR_STDXMLConfig(m_lUserID, &struInput, &struOuput))
    {
        g_pMainDlg->AddLog(m_dwDevIndex, OPERATION_FAIL_T, szUrl);
        delete[]pOutBuf;
        pOutBuf = NULL;
        return;
    }
    else
    {
        g_pMainDlg->AddLog(m_dwDevIndex, OPERATION_SUCC_T, szUrl);
        CXmlBase xmlBase;
        xmlBase.Parse(pOutBuf);
        xmlBase.SetRoot();
        m_AudioMixInfo = xmlBase.GetChildren().c_str();
        m_AudioMixInfo.Replace("\n", "\r\n");
    }

    UpdateData(FALSE);

    if (pOutBuf != NULL)
    {
        delete[]pOutBuf;
        pOutBuf = NULL;
    }

    return;
}


void CDLGTrialAudioMix::OnBnClickedBtnSetAudiomixcfg()
{
    UpdateData(TRUE);
    DWORD dwBufLen = 10 * 1024;
    char *pBuf = new char[dwBufLen];
    memset(pBuf, 0, dwBufLen);

    CString temp = m_AudioMixInfo;
    temp.Replace("\r\n", "\n");
    sprintf(pBuf, temp, sizeof(temp));

    char szUrl[256] = { 0 };
    sprintf(szUrl, "PUT /ISAPI/System/Audio/Mixing\r\n");

    NET_DVR_XML_CONFIG_INPUT    struInput = { 0 };
    NET_DVR_XML_CONFIG_OUTPUT   struOuput = { 0 };
    struInput.dwSize = sizeof(struInput);
    struOuput.dwSize = sizeof(struOuput);
    struInput.lpRequestUrl = szUrl;
    struInput.dwRequestUrlLen = strlen(szUrl);
    struInput.lpInBuffer = pBuf;
    struInput.dwInBufferSize = dwBufLen;
    if (!NET_DVR_STDXMLConfig(m_lUserID, &struInput, &struOuput))
    {
        g_pMainDlg->AddLog(m_dwDevIndex, OPERATION_FAIL_T, szUrl);
    }
    else
    {
        g_pMainDlg->AddLog(m_dwDevIndex, OPERATION_SUCC_T, szUrl);
    }
    UpdateData(FALSE);
    if (pBuf != NULL)
    {
        delete[]pBuf;
        pBuf = NULL;
    }
}


void CDLGTrialAudioMix::OnBnClickedBtnGetSingleSerialportCfg()
{
    UpdateData(TRUE);
    NET_DVR_XML_CONFIG_INPUT    struInput = { 0 };
    NET_DVR_XML_CONFIG_OUTPUT   struOuput = { 0 };
    struInput.dwSize = sizeof(struInput);
    struOuput.dwSize = sizeof(struOuput);

    CString strID = _T("");
    m_SerialPortID.GetLBText(m_SerialPortID.GetCurSel(), strID);

    char szUrl[256] = { 0 };
    sprintf(szUrl, "GET /ISAPI/System/Serial/ports/%s\r\n", strID);
    struInput.lpRequestUrl = szUrl;
    struInput.dwRequestUrlLen = strlen(szUrl);

    DWORD dwOutputLen = 1024 * 1024;
    char *pOutBuf = new char[dwOutputLen];
    memset(pOutBuf, 0, dwOutputLen);
    struOuput.lpOutBuffer = pOutBuf;
    struOuput.dwOutBufferSize = dwOutputLen;

    if (!NET_DVR_STDXMLConfig(m_lUserID, &struInput, &struOuput))
    {
        g_pMainDlg->AddLog(m_dwDevIndex, OPERATION_FAIL_T, szUrl);
        delete[]pOutBuf;
        pOutBuf = NULL;
        return;
    }
    else
    {
        g_pMainDlg->AddLog(m_dwDevIndex, OPERATION_SUCC_T, szUrl);
        CXmlBase xmlBase;
        xmlBase.Parse(pOutBuf);
        xmlBase.SetRoot();
        m_AudioMixInfo = xmlBase.GetChildren().c_str();
        m_AudioMixInfo.Replace("\n", "\r\n");
    }

    UpdateData(FALSE);

    if (pOutBuf != NULL)
    {
        delete[]pOutBuf;
        pOutBuf = NULL;
    }

    return;
}


void CDLGTrialAudioMix::OnBnClickedBtnSetSingleSerialportCfg()
{
    UpdateData(TRUE);
    DWORD dwBufLen = 10 * 1024;
    char *pBuf = new char[dwBufLen];
    memset(pBuf, 0, dwBufLen);

    CString temp = m_AudioMixInfo;
    temp.Replace("\r\n", "\n");
    sprintf(pBuf, temp, sizeof(temp));

    CString strID = _T("");
    m_SerialPortID.GetLBText(m_SerialPortID.GetCurSel(), strID);

    char szUrl[256] = { 0 };
    sprintf(szUrl, "PUT /ISAPI/System/Serial/ports/%s\r\n", strID);

    NET_DVR_XML_CONFIG_INPUT    struInput = { 0 };
    NET_DVR_XML_CONFIG_OUTPUT   struOuput = { 0 };
    struInput.dwSize = sizeof(struInput);
    struOuput.dwSize = sizeof(struOuput);
    struInput.lpRequestUrl = szUrl;
    struInput.dwRequestUrlLen = strlen(szUrl);
    struInput.lpInBuffer = pBuf;
    struInput.dwInBufferSize = dwBufLen;
    if (!NET_DVR_STDXMLConfig(m_lUserID, &struInput, &struOuput))
    {
        g_pMainDlg->AddLog(m_dwDevIndex, OPERATION_FAIL_T, szUrl);
    }
    else
    {
        g_pMainDlg->AddLog(m_dwDevIndex, OPERATION_SUCC_T, szUrl);
    }
    UpdateData(FALSE);
    if (pBuf != NULL)
    {
        delete[]pBuf;
        pBuf = NULL;
    }
}


void CDLGTrialAudioMix::OnBnClickedBtnGetSerialportCfg()
{
    UpdateData(TRUE);
    NET_DVR_XML_CONFIG_INPUT    struInput = { 0 };
    NET_DVR_XML_CONFIG_OUTPUT   struOuput = { 0 };
    struInput.dwSize = sizeof(struInput);
    struOuput.dwSize = sizeof(struOuput);

    char szUrl[256] = { 0 };
    sprintf(szUrl, "GET /ISAPI/System/Serial/ports\r\n");
    struInput.lpRequestUrl = szUrl;
    struInput.dwRequestUrlLen = strlen(szUrl);

    DWORD dwOutputLen = 1024 * 1024;
    char *pOutBuf = new char[dwOutputLen];
    memset(pOutBuf, 0, dwOutputLen);
    struOuput.lpOutBuffer = pOutBuf;
    struOuput.dwOutBufferSize = dwOutputLen;

    if (!NET_DVR_STDXMLConfig(m_lUserID, &struInput, &struOuput))
    {
        g_pMainDlg->AddLog(m_dwDevIndex, OPERATION_FAIL_T, szUrl);
        delete[]pOutBuf;
        pOutBuf = NULL;
        return;
    }
    else
    {
        g_pMainDlg->AddLog(m_dwDevIndex, OPERATION_SUCC_T, szUrl);
        CXmlBase xmlBase;
        xmlBase.Parse(pOutBuf);
        xmlBase.SetRoot();
        m_AudioMixInfo = xmlBase.GetChildren().c_str();
        m_AudioMixInfo.Replace("\n", "\r\n");
    }

    UpdateData(FALSE);

    if (pOutBuf != NULL)
    {
        delete[]pOutBuf;
        pOutBuf = NULL;
    }

    return;
}


void CDLGTrialAudioMix::OnBnClickedBtnSetSerialportCfg()
{
    UpdateData(TRUE);
    DWORD dwBufLen = 10 * 1024;
    char *pBuf = new char[dwBufLen];
    memset(pBuf, 0, dwBufLen);

    CString temp = m_AudioMixInfo;
    temp.Replace("\r\n", "\n");
    sprintf(pBuf, temp, sizeof(temp));

    char szUrl[256] = { 0 };
    sprintf(szUrl, "PUT /ISAPI/System/Serial/ports\r\n");

    NET_DVR_XML_CONFIG_INPUT    struInput = { 0 };
    NET_DVR_XML_CONFIG_OUTPUT   struOuput = { 0 };
    struInput.dwSize = sizeof(struInput);
    struOuput.dwSize = sizeof(struOuput);
    struInput.lpRequestUrl = szUrl;
    struInput.dwRequestUrlLen = strlen(szUrl);
    struInput.lpInBuffer = pBuf;
    struInput.dwInBufferSize = dwBufLen;
    if (!NET_DVR_STDXMLConfig(m_lUserID, &struInput, &struOuput))
    {
        g_pMainDlg->AddLog(m_dwDevIndex, OPERATION_FAIL_T, szUrl);
    }
    else
    {
        g_pMainDlg->AddLog(m_dwDevIndex, OPERATION_SUCC_T, szUrl);
    }
    UpdateData(FALSE);
    if (pBuf != NULL)
    {
        delete[]pBuf;
        pBuf = NULL;
    }
}

bool CDLGTrialAudioMix::GetAudiomixCap()
{
    UpdateData(TRUE);
    NET_DVR_XML_CONFIG_INPUT    struInput = { 0 };
    NET_DVR_XML_CONFIG_OUTPUT   struOuput = { 0 };
    struInput.dwSize = sizeof(struInput);
    struOuput.dwSize = sizeof(struOuput);
    char szUrl[256] = { 0 };
    sprintf(szUrl, "GET /ISAPI/System/Audio/Mixing/capabilities\r\n");
    struInput.lpRequestUrl = szUrl;
    struInput.dwRequestUrlLen = strlen(szUrl);
    DWORD dwOutputLen = 1024 * 1024;
    char *pOutBuf = new char[dwOutputLen];
    memset(pOutBuf, 0, dwOutputLen);
    struOuput.lpOutBuffer = pOutBuf;
    struOuput.dwOutBufferSize = dwOutputLen;

    if (!NET_DVR_STDXMLConfig(m_lUserID, &struInput, &struOuput))
    {
        g_pMainDlg->AddLog(m_dwDevIndex, OPERATION_FAIL_T, szUrl);
        delete[]pOutBuf;
        pOutBuf = NULL;
        return false;
    }
    else
    {
        g_pMainDlg->AddLog(m_dwDevIndex, OPERATION_SUCC_T, szUrl);

        m_AudioMixID.ResetContent();

        CXmlBase xmlBase;
        xmlBase.Parse(pOutBuf);
        xmlBase.SetRoot();
        if (xmlBase.FindElem("AudioMixingList") && xmlBase.IntoElem())
        {
            do 
            {
                if (xmlBase.FindElem("AudioMixing") && xmlBase.IntoElem())
                {
                    if (xmlBase.FindElem("id"))
                    {
                        m_AudioMixID.AddString(xmlBase.GetData().c_str());
                    }

                    xmlBase.OutOfElem();
                }
            } while (xmlBase.NextSibElem());

            xmlBase.OutOfElem();
        }

    }

    UpdateData(FALSE);

    if (pOutBuf != NULL)
    {
        delete[]pOutBuf;
        pOutBuf = NULL;
    }

    return true;
}


bool CDLGTrialAudioMix::GetSerialCap()
{
    UpdateData(TRUE);
    NET_DVR_XML_CONFIG_INPUT    struInput = { 0 };
    NET_DVR_XML_CONFIG_OUTPUT   struOuput = { 0 };
    struInput.dwSize = sizeof(struInput);
    struOuput.dwSize = sizeof(struOuput);
    char szUrl[256] = { 0 };
    sprintf(szUrl, "GET /ISAPI/System/Serial/capabilities\r\n");
    struInput.lpRequestUrl = szUrl;
    struInput.dwRequestUrlLen = strlen(szUrl);
    DWORD dwOutputLen = 1024 * 1024;
    char *pOutBuf = new char[dwOutputLen];
    memset(pOutBuf, 0, dwOutputLen);
    struOuput.lpOutBuffer = pOutBuf;
    struOuput.dwOutBufferSize = dwOutputLen;

    if (!NET_DVR_STDXMLConfig(m_lUserID, &struInput, &struOuput))
    {
        g_pMainDlg->AddLog(m_dwDevIndex, OPERATION_FAIL_T, szUrl);
        delete[]pOutBuf;
        pOutBuf = NULL;
        return false;
    }
    else
    {
        g_pMainDlg->AddLog(m_dwDevIndex, OPERATION_SUCC_T, szUrl);

        m_SerialPortID.ResetContent();

        CXmlBase xmlBase;
        xmlBase.Parse(pOutBuf);
        xmlBase.SetRoot();
        if (xmlBase.FindElem("SerialCap") && xmlBase.IntoElem())
        {
            if (xmlBase.FindElem("rs232SerialNumber"))
            {
                CString strAttributeValue = xmlBase.GetAttributeValue("opt").c_str();

                char *tokenPtr = strtok(strAttributeValue.GetBuffer(), ",");
                while (tokenPtr != NULL)
                {
                    m_SerialPortID.AddString(tokenPtr);
                    tokenPtr = strtok(NULL, ",");
                } 
            }

            if (xmlBase.FindElem("rs485SerialNumber"))
            {
                CString strAttributeValue = (char*)xmlBase.GetAttributeValue("opt").c_str();

                char *tokenPtr = strtok(strAttributeValue.GetBuffer(), ",");
                while (tokenPtr != NULL)
                {
                    m_SerialPortID.AddString(tokenPtr);
                    tokenPtr = strtok(NULL, ",");
                }
            }

            xmlBase.OutOfElem();
        }

    }

    UpdateData(FALSE);

    if (pOutBuf != NULL)
    {
        delete[]pOutBuf;
        pOutBuf = NULL;
    }

    return true;
}

