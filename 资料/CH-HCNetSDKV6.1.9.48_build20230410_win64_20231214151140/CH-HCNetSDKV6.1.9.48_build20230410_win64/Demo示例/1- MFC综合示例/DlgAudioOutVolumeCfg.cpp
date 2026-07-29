// DlgAudioOutVolumeCfg.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgAudioOutVolumeCfg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAudioOutVolumeCfg dialog


CDlgAudioOutVolumeCfg::CDlgAudioOutVolumeCfg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAudioOutVolumeCfg::IDD, pParent)
    , m_dwTalkVolume(0)
{
	//{{AFX_DATA_INIT(CDlgAudioOutVolumeCfg)
	//}}AFX_DATA_INIT
}


void CDlgAudioOutVolumeCfg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgAudioOutVolumeCfg)
    //}}AFX_DATA_MAP
    DDX_Text(pDX, IDC_EDIT_AUDIO_IN_VOLUME, m_iAudioInVolume);
    DDX_Text(pDX, IDC_EDIT_AUDIO_IN_TALK_VOLUME, m_dwTalkVolume);
    DDX_Control(pDX, IDC_COMBO_AUDIO_IN, m_comboAudioOutIndex);
}


BEGIN_MESSAGE_MAP(CDlgAudioOutVolumeCfg, CDialog)
	//{{AFX_MSG_MAP(CDlgAudioOutVolumeCfg)
	ON_BN_CLICKED(IDC_BTN_GET, OnBtnGet)
	ON_BN_CLICKED(IDC_BTN_SET, OnBtnSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAudioOutVolumeCfg message handlers
void CDlgAudioOutVolumeCfg::OnBtnGet() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);
	
	char szLan[128] = {0};
	NET_DVR_VOLUME_CFG struVolumeCfg = {0};
	memset(&struVolumeCfg, 0, sizeof(NET_DVR_VOLUME_CFG));

    DWORD dwReturnedBytes = 0;

    CString strID = _T("");
    m_comboAudioOutIndex.GetLBText(m_comboAudioOutIndex.GetCurSel(), strID);

    if (NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_AUDIOOUT_VOLUME_CFG, atoi(strID), &struVolumeCfg, sizeof(NET_DVR_VOLUME_CFG), &dwReturnedBytes))
	{
		g_pMainDlg->AddLog(m_dwDevIndex, OPERATION_SUCC_T, "NET_DVR_GET_AUDIOOUT_VOLUME_CFG");
		g_StringLanType(szLan, "获取音频输出口音量调节参数成功!", "Success to get audio out volume parameter config");
		//	AfxMessageBox(szLan);
	}
	else
	{
		g_pMainDlg->AddLog(m_dwDevIndex, OPERATION_FAIL_T, "NET_DVR_GET_AUDIOOUT_VOLUME_CFG");
		g_StringLanType(szLan, "获取音频输出口音量调节参数失败!", "Fail to get audio out volume parameter config");
		AfxMessageBox(szLan);
		return;
	}
	
    m_iAudioInVolume = struVolumeCfg.wVolume[0];
    m_dwTalkVolume = struVolumeCfg.wTalkVolume;

	UpdateData(FALSE);
}

void CDlgAudioOutVolumeCfg::OnBtnSet() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData(TRUE);
	
	char szLan[128] = {0};
	NET_DVR_VOLUME_CFG struVolumeCfg = {0};
	memset(&struVolumeCfg, 0, sizeof(NET_DVR_VOLUME_CFG));
	
	struVolumeCfg.dwSize = sizeof(NET_DVR_VOLUME_CFG);
	
	struVolumeCfg.wVolume[0] = m_iAudioInVolume;
    struVolumeCfg.wTalkVolume = (WORD)m_dwTalkVolume;

    CString strID = _T("");
    m_comboAudioOutIndex.GetLBText(m_comboAudioOutIndex.GetCurSel(), strID);

    if (NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_AUDIOOUT_VOLUME_CFG, atoi(strID), &struVolumeCfg, sizeof(NET_DVR_VOLUME_CFG)))
	{
		g_pMainDlg->AddLog(m_dwDevIndex, OPERATION_SUCC_T, "NET_DVR_SET_AUDIOOUT_VOLUME_CFG");
		g_StringLanType(szLan, "设置音频输出口音量调节参数成功!", "Success to set audio out volume parameter config");
		//	AfxMessageBox(szLan);
	}
	else
	{
		g_pMainDlg->AddLog(m_dwDevIndex, OPERATION_FAIL_T, "NET_DVR_SET_AUDIOOUT_VOLUME_CFG");
		g_StringLanType(szLan, "设置音频输出口音量调节参数失败!", "Fail to set audio out volume parameter config");
		AfxMessageBox(szLan);
		return;
	}
}

BOOL CDlgAudioOutVolumeCfg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

    char szLan[128] = { 0 };
    for (int i = 1; i <= 16; i++)
    {
        sprintf(szLan, "%d", i);
        m_comboAudioOutIndex.AddString(szLan);
    }

    GetAudioCap();

    m_comboAudioOutIndex.SetCurSel(0);

	OnBtnGet();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

bool CDlgAudioOutVolumeCfg::GetAudioCap()
{
    UpdateData(TRUE);
    NET_DVR_XML_CONFIG_INPUT    struInput = { 0 };
    NET_DVR_XML_CONFIG_OUTPUT   struOuput = { 0 };
    struInput.dwSize = sizeof(struInput);
    struOuput.dwSize = sizeof(struOuput);
    char szUrl[256] = { 0 };
    sprintf(szUrl, "GET /ISAPI/System/Audio/channels\r\n");
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

        m_comboAudioOutIndex.ResetContent();

        CXmlBase xmlBase;
        xmlBase.Parse(pOutBuf);
        xmlBase.SetRoot();
        if (xmlBase.FindElem("AudioChannelList") && xmlBase.IntoElem())
        {
            do
            {
                if (xmlBase.FindElem("AudioChannel") && xmlBase.IntoElem())
                {
                    if (xmlBase.FindElem("id"))
                    {
                        CString strID = xmlBase.GetData().c_str();

                        if (xmlBase.FindElem("type") && xmlBase.GetData() == "audioOut")
                        {
                            m_comboAudioOutIndex.AddString(strID);
                        }
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

    char szLan[128] = { 0 };
    if (m_comboAudioOutIndex.GetCount() == 0)
    {
        for (int i = 1; i <= 16; i++)
        {
            sprintf(szLan, "%d", i);
            m_comboAudioOutIndex.AddString(szLan);
        }
    }

    return true;
}


