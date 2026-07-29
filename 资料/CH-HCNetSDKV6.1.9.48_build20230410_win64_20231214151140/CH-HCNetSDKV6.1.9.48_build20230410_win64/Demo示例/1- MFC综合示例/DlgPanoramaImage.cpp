// DlgPanoramaImage.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgPanoramaImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPanoramaImage dialog


CDlgPanoramaImage::CDlgPanoramaImage(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPanoramaImage::IDD, pParent)
    , m_byBrightness(0)
{
	//{{AFX_DATA_INIT(CDlgPanoramaImage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    memset(&m_struPanoramaImage, 0, sizeof(m_struPanoramaImage));

    m_lChannel = -1;
    m_lUserID = -1;
    m_iDeviceIndex = -1;
    m_iAdjustmentRange = -1;
    memset(m_szStatusBuf, 0, ISAPI_STATUS_LEN);
    n_byR = 0;
    m_byG = 0;
    m_byB = 0;
    m_wR = 0;
    m_wG = 0;
    m_wB = 0;
}


void CDlgPanoramaImage::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgPanoramaImage)
    //DDX_Control(pDX, IDC_COMBO_FUSION_WAY, m_comFusionWay);
    DDX_Control(pDX, IDC_COMBO_FUSION_MODE, m_comFusionMode);
    DDX_Control(pDX, IDC_COMBO_TYPE, m_comType);
    DDX_Control(pDX, IDC_COMBO_SENSOR_NO, m_comSensorNo);
    DDX_Text(pDX, IDC_EDIT_ADJUSTMENT_RANGE, m_iAdjustmentRange);
    DDX_Text(pDX, IDC_EDIT_R, n_byR);
    DDX_Text(pDX, IDC_EDIT_G, m_byG);
    DDX_Text(pDX, IDC_EDIT_B, m_byB);
    DDX_Text(pDX, IDC_EDIT_R_Ex, m_wR);
    DDX_Text(pDX, IDC_EDIT_G_Ex, m_wG);
    DDX_Text(pDX, IDC_EDIT_B_Ex, m_wB);
    //}}AFX_DATA_MAP


    DDX_Control(pDX, IDC_COMBO_RGB_TYPE, m_cmbRgbType);
    DDX_Text(pDX, IDC_EDIT_BRIGHTNESS, m_byBrightness);
    DDX_Control(pDX, IDC_COMBO_PREVIEW_MODE, m_cmbPreviewMode);
}


BEGIN_MESSAGE_MAP(CDlgPanoramaImage, CDialog)
	//{{AFX_MSG_MAP(CDlgPanoramaImage)
	ON_BN_CLICKED(IDC_BTN_GET, OnBtnGet)
	ON_BN_CLICKED(IDC_BTN_SET, OnBtnSet)
    ON_BN_CLICKED(IDC_BTN_ADJUSTMENT, OnBtnSensorAdjustment)
    ON_BN_CLICKED(IDC_BTN_RESET, OnBtnReset)
    ON_BN_CLICKED(IDC_BTN_SENSOR_INFO, OnBtnSensorInfo)
	//}}AFX_MSG_MAP
    ON_CBN_SELCHANGE(IDC_COMBO_TYPE, &CDlgPanoramaImage::OnCbnSelchangeComboType)
    ON_CBN_SELCHANGE(IDC_COMBO_SENSOR_NO, &CDlgPanoramaImage::OnCbnSelchangeComboSensorNo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPanoramaImage message handlers

void CDlgPanoramaImage::OnBtnGet() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);
    m_struPanoramaImage.dwSize = sizeof(m_struPanoramaImage);
  
    NET_DVR_STD_CONFIG struCfg = {0};
    struCfg.lpCondBuffer = &m_lChannel;
    struCfg.dwCondSize = sizeof(m_lChannel);
    struCfg.lpOutBuffer = &m_struPanoramaImage;
    struCfg.dwOutSize = sizeof(m_struPanoramaImage);
    memset(m_szStatusBuf, 0, ISAPI_STATUS_LEN);
    struCfg.lpStatusBuffer = m_szStatusBuf;
    struCfg.dwStatusSize = ISAPI_STATUS_LEN;
    if(!NET_DVR_GetSTDConfig(m_lUserID, NET_DVR_GET_PANORAMAIMAGE, &struCfg))
    {
        OutputDebugString(m_szStatusBuf);
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GET_PANORAMAIMAGE");
        return ;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_GET_PANORAMAIMAGE");
    }

    m_comFusionMode.SetCurSel(m_struPanoramaImage.byFusionMode);
    //m_comFusionWay.SetCurSel(m_struPanoramaImage.byFusionWay);
    m_cmbPreviewMode.SetCurSel(m_struPanoramaImage.byPreviewMode);
    UpdateData(FALSE);	
}

void CDlgPanoramaImage::OnBtnSet() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);

    m_struPanoramaImage.byFusionMode = m_comFusionMode.GetCurSel();
    //m_struPanoramaImage.byFusionWay = m_comFusionWay.GetCurSel();
    m_struPanoramaImage.byPreviewMode = m_cmbPreviewMode.GetCurSel();
    NET_DVR_STD_CONFIG struCfg = {0};
    struCfg.lpCondBuffer = &m_lChannel;
    struCfg.dwCondSize = sizeof(m_lChannel);
    struCfg.lpInBuffer = &m_struPanoramaImage;
    struCfg.dwInSize = sizeof(m_struPanoramaImage);
    memset(m_szStatusBuf, 0, ISAPI_STATUS_LEN);
    struCfg.lpStatusBuffer = m_szStatusBuf;
    struCfg.dwStatusSize = ISAPI_STATUS_LEN;
    
    if(!NET_DVR_SetSTDConfig(m_lUserID, NET_DVR_SET_PANORAMAIMAGE, &struCfg))
    {
        OutputDebugString(m_szStatusBuf);
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_SET_PANORAMAIMAGE");
        return;
    }
    else
    {
        OutputDebugString(m_szStatusBuf);
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_SET_PANORAMAIMAGE");
    }
}

void CDlgPanoramaImage::OnBtnSensorAdjustment() 
{
    UpdateData(TRUE);
    CString m_strInputParam = "GET /ISAPI/Panorama/sensor/capabilities\r\n";
    char *m_pOutBuf = NULL;
    m_pOutBuf = new char[3 * 1024 * 1024];
    DWORD m_dwReturnValue;
    string strMin;
    string strMax;

    NET_DVR_XML_CONFIG_INPUT    struInput = { 0 };
    NET_DVR_XML_CONFIG_OUTPUT   struOuput = { 0 };
    struInput.dwSize = sizeof(struInput);
    struOuput.dwSize = sizeof(struOuput);
    struInput.lpRequestUrl = m_strInputParam.GetBuffer(0);
    struInput.dwRequestUrlLen = m_strInputParam.GetLength();
    struOuput.dwSize = sizeof(struOuput);
    memset(m_pOutBuf, 0, 3*1024*1024);
    struOuput.dwSize = sizeof(NET_DVR_XML_CONFIG_OUTPUT);
    struOuput.lpOutBuffer = m_pOutBuf;
    struOuput.dwOutBufferSize = 3 * 1024 * 1024;

    m_dwReturnValue = NET_DVR_STDXMLConfig(m_lUserID, &struInput, &struOuput);
    if (m_dwReturnValue != 0)
    {
        CXmlBase xmlBase;
        xmlBase.Parse(m_pOutBuf);
        if (xmlBase.FindElem("SensorCap") && xmlBase.IntoElem())
        {
            if (xmlBase.FindElem("R"))
            {
                strMin = xmlBase.GetAttributeValue("min");
                strMax = xmlBase.GetAttributeValue("max");
            }
        }
    }
    int iMin = atoi(strMin.c_str());
    int iMax = atoi(strMax.c_str());

    if (m_pOutBuf != NULL)
    {
        delete[]m_pOutBuf;
        m_pOutBuf = NULL;
    }

    char szLan[256] = {0};
    NET_DVR_SENSOR_ADJUSTMENT m_struSensorAdjustment = {0};
    m_struSensorAdjustment.dwSize = sizeof(m_struSensorAdjustment);
    m_struSensorAdjustment.byType = m_comType.GetCurSel();
    m_struSensorAdjustment.bySensorNo = m_comSensorNo.GetCurSel()+1;
    m_struSensorAdjustment.iAdjustMentRange = m_iAdjustmentRange;
    switch (m_struSensorAdjustment.byType)
    {
    case 4:
        if (n_byR < iMin || n_byR > iMax || m_byG < iMin || m_byG > iMax || m_byB < iMin || m_byB > iMax)
        {
            sprintf(szLan, "配置超出范围, 设备支持配置最小值:%d, 最大值:%d", iMin, iMax);
            MessageBox(szLan);
            return;
        }
        m_struSensorAdjustment.byR = n_byR;
        m_struSensorAdjustment.byG = m_byG;
        m_struSensorAdjustment.byB = m_byB;
        break;
    case 6:
        if (m_wR < iMin || m_wR > iMax || m_wG < iMin || m_wG > iMax || m_wB < iMin || m_wB > iMax)
        {
            sprintf(szLan, "配置超出范围, 设备支持配置最小值:%d, 最大值:%d", iMin, iMax);
            MessageBox(szLan);
            return;
        }
        m_struSensorAdjustment.wRex = (WORD)m_wR;
        m_struSensorAdjustment.wGex = (WORD)m_wG;
        m_struSensorAdjustment.wBex = (WORD)m_wB;
        break;
    default:
        break;
    }    
    m_struSensorAdjustment.byBrightness = m_byBrightness;
    m_struSensorAdjustment.byRgbType = m_cmbRgbType.GetCurSel();
    NET_DVR_STD_CONTROL struControl = {0};
    struControl.lpCondBuffer = &m_struSensorAdjustment;
    struControl.dwCondSize = sizeof(m_struSensorAdjustment);
    
    memset(m_szStatusBuf, 0, 1024);
    struControl.lpStatusBuffer = m_szStatusBuf;
    struControl.dwStatusSize = 1024;
    
    if(!NET_DVR_STDControl(m_lUserID, NET_DVR_SET_SENSOR_ADJUSTMENT, &struControl))
    {
        OutputDebugString(m_szStatusBuf);
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_SET_SENSOR_ADJUSTMENT");
        if (NET_DVR_ERR_EXPOSURE_SYNC == NET_DVR_GetLastError())
        {
            g_StringLanType(szLan, "镜头间曝光同步已开启，不允许配置手动RGB", "Exposure is synchronized, RGB configuration is forbidden.");
            AfxMessageBox(szLan);
        }
        else
        {
            sprintf(szLan, "Error Code:%d, Error Info:%s", NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
            AfxMessageBox(szLan);
        }
    }
    else
    {
        m_MapSenserInfo[m_comSensorNo.GetCurSel()].R_ex = m_wR;
        m_MapSenserInfo[m_comSensorNo.GetCurSel()].G_ex = m_wG;
        m_MapSenserInfo[m_comSensorNo.GetCurSel()].B_ex = m_wB;
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_SET_SENSOR_ADJUSTMENT");
    }
}

void CDlgPanoramaImage::OnBtnReset()
{
    UpdateData(TRUE);
    char szLan[128] = { 0 };
    NET_DVR_SENSOR_RESET m_struSensorReset = { 0 };
    m_struSensorReset.dwSize = sizeof(m_struSensorReset);
    m_struSensorReset.bySensorNo = m_comSensorNo.GetCurSel() + 1;

    NET_DVR_STD_CONTROL struControl = { 0 };
    struControl.lpCondBuffer = &m_struSensorReset;
    struControl.dwCondSize = sizeof(m_struSensorReset);

    memset(m_szStatusBuf, 0, 1024);
    struControl.lpStatusBuffer = m_szStatusBuf;
    struControl.dwStatusSize = 1024;

    if (!NET_DVR_STDControl(m_lUserID, NET_DVR_SENSOR_RESET_CTRL, &struControl))
    {
        OutputDebugString(m_szStatusBuf);
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_SENSOR_RESET_CTRL");
        //         sprintf(szLan, "Error Code:%d, Error Info:%s", NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
        //         AfxMessageBox(szLan);
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_SENSOR_RESET_CTRL");
    }
}

void CDlgPanoramaImage::OnBtnSensorInfo()
{
    UpdateData(TRUE);
    NET_DVR_SENSOR_ADJUSTMENT_INFO m_struSensorAdjustmentInfo = {0};
    m_struSensorAdjustmentInfo.dwSize = sizeof(m_struSensorAdjustmentInfo);
    
    NET_DVR_STD_CONFIG struCfg = {0};
    int iSensorNo = m_comSensorNo.GetCurSel()+1;
    struCfg.lpCondBuffer = &iSensorNo;
    struCfg.dwCondSize = sizeof(int);

    struCfg.lpOutBuffer = &m_struSensorAdjustmentInfo;
    struCfg.dwOutSize = sizeof(m_struSensorAdjustmentInfo);
    memset(m_szStatusBuf, 0, ISAPI_STATUS_LEN);
    struCfg.lpStatusBuffer = m_szStatusBuf;
    struCfg.dwStatusSize = ISAPI_STATUS_LEN;
    if(!NET_DVR_GetSTDConfig(m_lUserID, NET_DVR_GET_SENSOR_ADJUSTMENT_INFO, &struCfg))
    {
        OutputDebugString(m_szStatusBuf);
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GET_SENSOR_ADJUSTMENT_INFO");
        return ;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_GET_SENSOR_ADJUSTMENT_INFO");
    }
    char szAdjustmentInfo[1024] = {0};
    if (m_struSensorAdjustmentInfo.wRex > 255 || m_struSensorAdjustmentInfo.wGex > 255 || m_struSensorAdjustmentInfo.wBex > 255)
    {
        sprintf(szAdjustmentInfo, "dwSize:%d,iPan:%d,iTilt:%d,iRotation:%d,iFieldAngle:%d,RgbType:%d,R_Ex:%d,G_Ex:%d,B_Ex:%d，Brightness：%d", \
            m_struSensorAdjustmentInfo.dwSize, m_struSensorAdjustmentInfo.iPan, m_struSensorAdjustmentInfo.iTilt, \
            m_struSensorAdjustmentInfo.iRotation, m_struSensorAdjustmentInfo.iFieldAngle, m_struSensorAdjustmentInfo.byRgbType,
            m_struSensorAdjustmentInfo.wRex, m_struSensorAdjustmentInfo.wGex, m_struSensorAdjustmentInfo.wBex, m_struSensorAdjustmentInfo.byBrightness);
    }
    else
    {
        sprintf(szAdjustmentInfo, "dwSize:%d,iPan:%d,iTilt:%d,iRotation:%d,iFieldAngle:%d,RgbType:%d,R:%d,G:%d,B:%d，Brightness：%d", \
            m_struSensorAdjustmentInfo.dwSize, m_struSensorAdjustmentInfo.iPan, m_struSensorAdjustmentInfo.iTilt, \
            m_struSensorAdjustmentInfo.iRotation, m_struSensorAdjustmentInfo.iFieldAngle, m_struSensorAdjustmentInfo.byRgbType,
            m_struSensorAdjustmentInfo.byR, m_struSensorAdjustmentInfo.byG, m_struSensorAdjustmentInfo.byB, m_struSensorAdjustmentInfo.byBrightness);
    }
    MessageBox(szAdjustmentInfo);
    return;
}


void CDlgPanoramaImage::OnCbnSelchangeComboType()
{
    UpdateData(TRUE);
    int iTypeNum = m_comType.GetCurSel();
    switch (iTypeNum)
    {
    case 4:
        GetDlgItem(IDC_EDIT_R_Ex)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_G_Ex)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_B_Ex)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_R)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_G)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_B)->EnableWindow(TRUE);
        break;
    case 6:
        GetDlgItem(IDC_EDIT_R)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_G)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_B)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_R_Ex)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_G_Ex)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_B_Ex)->EnableWindow(TRUE);
        break;
    default:
        GetDlgItem(IDC_EDIT_R)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_G)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_B)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_R_Ex)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_G_Ex)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_B_Ex)->EnableWindow(FALSE);
        break;
    }
}


void CDlgPanoramaImage::OnCbnSelchangeComboSensorNo()
{
    // TODO:  在此添加控件通知处理程序代码
    int iTypeNum = m_comSensorNo.GetCurSel();
    if (m_MapSenserInfo.count(iTypeNum))
    {
        m_wR = m_MapSenserInfo[iTypeNum].R_ex;
        m_wG = m_MapSenserInfo[iTypeNum].G_ex;
        m_wB = m_MapSenserInfo[iTypeNum].B_ex;
    }
    else
    {
        m_wR = 0;
        m_wG = 0;
        m_wB = 0;
    }
    UpdateData(FALSE);
}
