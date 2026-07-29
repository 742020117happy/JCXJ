// DlgBuiltinSupplementLight.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgBuiltinSupplementLight.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgBuiltinSupplementLight dialog


CDlgBuiltinSupplementLight::CDlgBuiltinSupplementLight(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBuiltinSupplementLight::IDD, pParent)
    , m_byHighIrLightBrightness(0)
    , m_byHighWhiteLightBrightness(0)
    , m_byIrLightBrightness(0)
    , m_byLowIrLightBrightness(0)
    , m_byLowWhiteLightBrightness(0)
    , m_byWhiteLightBrightness(0)
    , m_byIrLightbrightnessLimit(0)
    , m_byWhiteLightbrightnessLimit(0)
{
	//{{AFX_DATA_INIT(CDlgBuiltinSupplementLight)
	m_iBLimit = 0;
	m_TimeStart = COleDateTime::GetCurrentTime();
	//}}AFX_DATA_INIT

    memset(&m_struBuiltinSupplementLight, 0, sizeof(m_struBuiltinSupplementLight));
    m_lServerID = -1;
    m_iDeviceIndex = -1;
    m_lChannel = -1;
    memset(m_szStatusBuf, 0, ISAPI_STATUS_LEN);
}


void CDlgBuiltinSupplementLight::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgBuiltinSupplementLight)
    DDX_DateTimeCtrl(pDX, IDC_DATETIME_STOP, m_TimeStop);
    DDX_Control(pDX, IDC_COMBO_MODE, m_comboMode);
    DDX_Text(pDX, IDC_EDIT_BLimit, m_iBLimit);
    DDX_DateTimeCtrl(pDX, IDC_DATETIME_START, m_TimeStart);
    //}}AFX_DATA_MAP
    DDX_Control(pDX, IDC_COMBO_MIXMODE, m_comboMixMode);
    DDX_Control(pDX, IDC_COMBO_SUPLIGHTMODE, m_SupLightMode);
    DDX_Text(pDX, IDC_EDIT_HIRLBRIGHTNESS, m_byHighIrLightBrightness);
    DDX_Text(pDX, IDC_EDIT_HWLBRIGHTNESS, m_byHighWhiteLightBrightness);
    DDX_Text(pDX, IDC_EDIT_IRLBRIGHTNESS, m_byIrLightBrightness);
    DDX_Text(pDX, IDC_EDIT_LIRLBRIGHTNESS, m_byLowIrLightBrightness);
    DDX_Text(pDX, IDC_EDIT_LLBRIGHTNESS, m_byLowWhiteLightBrightness);
    DDX_Text(pDX, IDC_EDIT_WLBRIGHTNESS, m_byWhiteLightBrightness);
    DDX_Text(pDX, IDC_EDIT_IRLBRIGHTNESSLIMIT, m_byIrLightbrightnessLimit);
    DDX_Text(pDX, IDC_EDIT_WLBRIGHTNESSLIMIT, m_byWhiteLightbrightnessLimit);
}


BEGIN_MESSAGE_MAP(CDlgBuiltinSupplementLight, CDialog)
	//{{AFX_MSG_MAP(CDlgBuiltinSupplementLight)
	ON_BN_CLICKED(IDC_BTN_SET, OnBtnSet)
	ON_BN_CLICKED(IDC_BTN_GET, OnBtnGet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBuiltinSupplementLight message handlers

void CDlgBuiltinSupplementLight::OnBtnSet() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);

    m_struBuiltinSupplementLight.dwSize = sizeof(m_struBuiltinSupplementLight);
    m_struBuiltinSupplementLight.byBrightnessLimit = m_iBLimit;
    m_struBuiltinSupplementLight.byMode = m_comboMode.GetCurSel();
    m_struBuiltinSupplementLight.struSchedTime.struStartTime.byHour = m_TimeStart.GetHour();
    m_struBuiltinSupplementLight.struSchedTime.struStartTime.byMinute = m_TimeStart.GetMinute();
    m_struBuiltinSupplementLight.struSchedTime.struStartTime.bySecond = m_TimeStart.GetSecond();

    m_struBuiltinSupplementLight.struSchedTime.struStopTime.byHour = m_TimeStop.GetHour();
    m_struBuiltinSupplementLight.struSchedTime.struStopTime.byMinute = m_TimeStop.GetMinute();
    m_struBuiltinSupplementLight.struSchedTime.struStopTime.bySecond = m_TimeStop.GetSecond();

    m_struBuiltinSupplementLight.byMixedLightRegulatMode = m_comboMixMode.GetCurSel();
    m_struBuiltinSupplementLight.bySupplementLightMode = m_SupLightMode.GetCurSel();

    m_struBuiltinSupplementLight.byHighLrLightBrightness = m_byHighIrLightBrightness;
    m_struBuiltinSupplementLight.byHighBrightnessLimit = m_byHighWhiteLightBrightness;

    m_struBuiltinSupplementLight.byLrLightBrightness = m_byIrLightBrightness;
    m_struBuiltinSupplementLight.byWhiteLightBrightness = m_byWhiteLightBrightness;
    m_struBuiltinSupplementLight.byLowLrLightBrightness = m_byLowIrLightBrightness;
    m_struBuiltinSupplementLight.byLowBrightnessLimit = m_byLowWhiteLightBrightness;
    m_struBuiltinSupplementLight.byIrLightbrightnessLimit = m_byIrLightbrightnessLimit;
    m_struBuiltinSupplementLight.byWhiteLightbrightnessLimit = m_byWhiteLightbrightnessLimit;

    NET_DVR_STD_CONFIG struCfg = {0};
    struCfg.lpCondBuffer = &m_lChannel;
    struCfg.dwCondSize = sizeof(m_lChannel);
    struCfg.lpInBuffer = &m_struBuiltinSupplementLight;
    struCfg.dwInSize = sizeof(m_struBuiltinSupplementLight);
    memset(m_szStatusBuf, 0, ISAPI_STATUS_LEN);
    struCfg.lpStatusBuffer = m_szStatusBuf;
    struCfg.dwStatusSize = ISAPI_STATUS_LEN;
    
    if(!NET_DVR_SetSTDConfig(m_lServerID, NET_DVR_SET_SUPPLEMENTLIGHT, &struCfg))
    {
        OutputDebugString(m_szStatusBuf);
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_SET_SUPPLEMENTLIGHT");
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_SET_SUPPLEMENTLIGHT");
        OutputDebugString(m_szStatusBuf);   
    }
}

void CDlgBuiltinSupplementLight::OnBtnGet() 
{
	// TODO: Add your control notification handler code here
    
    NET_DVR_STD_CONFIG struCfg = {0};
    struCfg.lpCondBuffer = &m_lChannel;
    struCfg.dwCondSize = sizeof(m_lChannel);
    struCfg.lpOutBuffer = &m_struBuiltinSupplementLight;
    struCfg.dwOutSize = sizeof(m_struBuiltinSupplementLight);
    memset(m_szStatusBuf, 0, ISAPI_STATUS_LEN);
    struCfg.lpStatusBuffer = m_szStatusBuf;
    struCfg.dwStatusSize = ISAPI_STATUS_LEN;
    
    if(!NET_DVR_GetSTDConfig(m_lServerID, NET_DVR_GET_SUPPLEMENTLIGHT, &struCfg))
    {
        OutputDebugString(m_szStatusBuf);
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GET_SUPPLEMENTLIGHT");
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_GET_SUPPLEMENTLIGHT");
        
    }

    //开始时间
    m_TimeStart.SetTime(m_struBuiltinSupplementLight.struSchedTime.struStartTime.byHour, \
        m_struBuiltinSupplementLight.struSchedTime.struStartTime.byMinute, \
        m_struBuiltinSupplementLight.struSchedTime.struStartTime.bySecond
        );
    
    //结束时间	
    m_TimeStop.SetTime(m_struBuiltinSupplementLight.struSchedTime.struStopTime.byHour, \
        m_struBuiltinSupplementLight.struSchedTime.struStopTime.byMinute, \
        m_struBuiltinSupplementLight.struSchedTime.struStopTime.bySecond
		);

    m_iBLimit = m_struBuiltinSupplementLight.byBrightnessLimit;
    m_comboMode.SetCurSel(m_struBuiltinSupplementLight.byMode);

    m_comboMixMode.SetCurSel(m_struBuiltinSupplementLight.byMixedLightRegulatMode);
    m_SupLightMode.SetCurSel(m_struBuiltinSupplementLight.bySupplementLightMode);
    m_byHighIrLightBrightness = m_struBuiltinSupplementLight.byHighLrLightBrightness;
    m_byHighWhiteLightBrightness = m_struBuiltinSupplementLight.byHighBrightnessLimit;;
    m_byIrLightBrightness = m_struBuiltinSupplementLight.byLrLightBrightness;
    m_byWhiteLightBrightness = m_struBuiltinSupplementLight.byWhiteLightBrightness;
    m_byLowIrLightBrightness = m_struBuiltinSupplementLight.byLowLrLightBrightness;
    m_byLowWhiteLightBrightness = m_struBuiltinSupplementLight.byLowBrightnessLimit;
    m_byIrLightbrightnessLimit = m_struBuiltinSupplementLight.byIrLightbrightnessLimit;
    m_byWhiteLightbrightnessLimit = m_struBuiltinSupplementLight.byWhiteLightbrightnessLimit;

    UpdateData(FALSE);
}
