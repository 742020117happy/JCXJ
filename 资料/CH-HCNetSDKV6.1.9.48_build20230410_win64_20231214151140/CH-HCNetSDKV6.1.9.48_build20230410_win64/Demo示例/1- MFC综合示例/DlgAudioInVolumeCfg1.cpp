// DlgAudioInVolumeCfg1.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgAudioInVolumeCfg1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAudioInVolumeCfg dialog


CDlgAudioInVolumeCfg::CDlgAudioInVolumeCfg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAudioInVolumeCfg::IDD, pParent)
    , m_iAudioVolume(0)
{
	//{{AFX_DATA_INIT(CDlgAudioInVolumeCfg)
	m_bChkPhantomPower = FALSE;
	m_bEnableFBC = FALSE;
	m_bEnableAEC = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgAudioInVolumeCfg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgAudioInVolumeCfg)
    DDX_Check(pDX, IDC_CHK_PHANTOM_POWER, m_bChkPhantomPower);
    DDX_Check(pDX, IDC_CHK_ENABLE_FBC, m_bEnableFBC);
    DDX_Check(pDX, IDC_CHK_AEC, m_bEnableAEC);
    //}}AFX_DATA_MAP
    DDX_Control(pDX, IDC_COMBO_AUDIO_OUT_INDEX, m_comboAudioOutIndex);
    DDX_Text(pDX, IDC_EDIT_AUDIO_VOLUME, m_iAudioVolume);
    DDX_Control(pDX, IDC_COMBO_AUDIO_IN, m_comboAudioInIndex);
}


BEGIN_MESSAGE_MAP(CDlgAudioInVolumeCfg, CDialog)
	//{{AFX_MSG_MAP(CDlgAudioInVolumeCfg)
	ON_BN_CLICKED(IDC_BTN_GET, OnBtnGet)
	ON_BN_CLICKED(IDC_BTN_SET, OnBtnSet)
	//}}AFX_MSG_MAP
    ON_CBN_SELCHANGE(IDC_COMBO_AUDIO_OUT_INDEX, &CDlgAudioInVolumeCfg::OnCbnSelchangeComboAudioOutIndex)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAudioInVolumeCfg message handlers

void CDlgAudioInVolumeCfg::OnBtnGet() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);

	char szLan[128] = {0};
 
	memset(&m_struVolumeCfg, 0, sizeof(NET_DVR_VOLUME_CFG));

    DWORD dwReturnedBytes = 0;

    CString strID = _T("");
    m_comboAudioInIndex.GetLBText(m_comboAudioInIndex.GetCurSel(), strID);
	
    if (NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_AUDIOIN_VOLUME_CFG, atoi(strID), &m_struVolumeCfg, sizeof(NET_DVR_VOLUME_CFG), &dwReturnedBytes))
	{
		g_pMainDlg->AddLog(m_dwDevIndex, OPERATION_SUCC_T, "NET_DVR_GET_AUDIOIN_VOLUME_CFG");
		g_StringLanType(szLan, "获取音频输入口音量调节参数成功!", "Success to get audio in volume parameter config");
		//	AfxMessageBox(szLan);
	}
	else
	{
		g_pMainDlg->AddLog(m_dwDevIndex, OPERATION_FAIL_T, "NET_DVR_GET_AUDIOIN_VOLUME_CFG");
		g_StringLanType(szLan, "获取音频输入口音量调节参数失败!", "Fail to get audio in volume parameter config");
		AfxMessageBox(szLan);
		return;
	}
	
    m_comboAudioOutIndex.SetCurSel(m_dwLastIndex);
    if (0 <= m_dwLastIndex && m_dwLastIndex < 8)
    {
        m_iAudioVolume = m_struVolumeCfg.wVolume[m_dwLastIndex];
    }
    else if (8 <= m_dwLastIndex && m_dwLastIndex < 16)
    {
        m_iAudioVolume = m_struVolumeCfg.wVolumeEx[m_dwLastIndex - 8];
    }
    m_bChkPhantomPower = m_struVolumeCfg.byPhantomPowerSupply - 1;
	m_bEnableAEC = m_struVolumeCfg.byEnableAEC;
	m_bEnableFBC = m_struVolumeCfg.byEnableFBC[m_dwLastIndex];
	
	UpdateData(FALSE);
}

void CDlgAudioInVolumeCfg::OnBtnSet() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData(TRUE);
	
	char szLan[128] = {0};
	m_struVolumeCfg.dwSize = sizeof(NET_DVR_VOLUME_CFG);
	
    DWORD dwIndex = 0;
    if (m_comboAudioOutIndex.GetCurSel() < 0)
    {
        dwIndex = 0;
    }
    else
    {
        dwIndex = m_comboAudioOutIndex.GetCurSel();
    }

    if (0 <= m_dwLastIndex && m_dwLastIndex < 8)
    {
        m_struVolumeCfg.wVolume[m_dwLastIndex] = m_iAudioVolume;
    }
    else if (8 <= m_dwLastIndex && m_dwLastIndex < 16)
    {
        m_struVolumeCfg.wVolumeEx[m_dwLastIndex - 8] = m_iAudioVolume;
    }

	m_struVolumeCfg.byPhantomPowerSupply = m_bChkPhantomPower+1;
	m_struVolumeCfg.byEnableAEC = m_bEnableAEC;
	m_struVolumeCfg.byEnableFBC[dwIndex] = m_bEnableFBC;

    CString strID = _T("");
    m_comboAudioInIndex.GetLBText(m_comboAudioInIndex.GetCurSel(), strID);
	
    if (NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_AUDIOIN_VOLUME_CFG, atoi(strID), &m_struVolumeCfg, sizeof(NET_DVR_VOLUME_CFG)))
	{
		g_pMainDlg->AddLog(m_dwDevIndex, OPERATION_SUCC_T, "NET_DVR_SET_AUDIOIN_VOLUME_CFG");
		g_StringLanType(szLan, "设置音频输入口音量调节参数成功!", "Success to set audio in volume parameter config");
		//	AfxMessageBox(szLan);
	}
	else
	{
		g_pMainDlg->AddLog(m_dwDevIndex, OPERATION_FAIL_T, "NET_DVR_SET_AUDIOIN_VOLUME_CFG");
		g_StringLanType(szLan, "设置音频输入口音量调节参数失败!", "Fail to set audio in volume parameter config");
		AfxMessageBox(szLan);
		return;
	}
}

BOOL CDlgAudioInVolumeCfg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_dwLastIndex = 0;
	// TODO: Add extra initialization here

	char szLan[128] = {0};
    memset(&m_struVolumeCfg,0,sizeof(m_struVolumeCfg));

    for (int i = 1; i <= 16; i++)
    {
        sprintf(szLan, "%d", i);
        m_comboAudioInIndex.AddString(szLan);
    }

    for (int i = 1; i <= 16; i++)
    {
        sprintf(szLan, "%d", i);
        m_comboAudioOutIndex.AddString(szLan);
    }

    GetAudioCap();

    m_comboAudioInIndex.SetCurSel(0);
    m_comboAudioOutIndex.SetCurSel(0);

	OnBtnGet();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgAudioInVolumeCfg::OnCbnSelchangeComboAudioOutIndex()
{
    // TODO:  在此添加控件通知处理程序代码
    m_dwLastIndex = m_comboAudioOutIndex.GetCurSel();
    if (0 <= m_dwLastIndex && m_dwLastIndex < 8)
    {
        m_iAudioVolume = m_struVolumeCfg.wVolume[m_dwLastIndex];
    }
    else if (8 <= m_dwLastIndex && m_dwLastIndex < 16)
    {
        m_iAudioVolume = m_struVolumeCfg.wVolumeEx[m_dwLastIndex - 8];
    }
    UpdateData(FALSE);
}

bool CDlgAudioInVolumeCfg::GetAudioCap()
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

        m_comboAudioInIndex.ResetContent();
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

                        if (xmlBase.FindElem("type") && xmlBase.GetData() == "audioIn")
                        {
                            m_comboAudioInIndex.AddString(strID);
                        }
                        else if (xmlBase.FindElem("type") && xmlBase.GetData() == "audioOut")
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
    if (m_comboAudioInIndex.GetCount() == 0)
    {
        for (int i = 1; i <= 16; i++)
        {
            sprintf(szLan, "%d", i);
            m_comboAudioInIndex.AddString(szLan);
        }
    }

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


