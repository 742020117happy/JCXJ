// DlgCcdParam1.cpp : implementation file
/**********************************************************
FileName:    DlgCcdParam.cpp
Description: CCD Parameter Config    
Date:        2009/11/11
Note: 		 <global>struct, refer to GeneralDef.h, global variants and functions refer to ClientDemo.cpp   
Modification History:      
    <version> <time>         <desc>
    <1.0    > <2008/06/10>       <created>
***********************************************************/

#include "stdafx.h"
#include "ClientDemo.h"
#include "DlgCcdParam.h"
#include "DlgInfrareCfg.h"
#include "DlgISPParamCfg.h"
#include "DlgSignalLightSync.h"
#include "DlgIOOutCfg.h"
#include "DlgEZVIZAccessCfg.h"
#include "DlgDPCCfg.h"
#include "DlgBuiltinSupplementLight.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCcdParam dialog

/*********************************************************
  Function:	CDlgCcdParam
  Desc:		Constructor
  Input:	pParent, parent window pointer
  Output:	none
  Return:	none
**********************************************************/
CDlgCcdParam::CDlgCcdParam(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCcdParam::IDD, pParent)
    , m_byCalibrationAccurateLevel(0)
    , m_byZoomedInDistantViewLevel(0)
    , m_byHorizontalFOV(0)
    , m_byVerticalFOV(0)
    , m_byBrightnessSuddenChangeSuppression(FALSE)
    , m_iCurChanIndex(-1)
    , m_checkGPSEnabled(FALSE)
{
	//{{AFX_DATA_INIT(CDlgCcdParam)
	m_iBrightness = 0;
	m_iContrast = 0;
	m_iGain = 0;
	m_iSaturation = 0;
	m_iSharpness = 0;
	m_iExposureUserSet = 0;
	m_iVedioExposure = 0;
	m_iUserGain = 0;
	m_byNormalLevel = 0;
	m_bySpectralLevel = 0;
	m_byTemporalLevel = 0;
	m_byAutoCompInter = 0;
	m_bChkLightInhibitEn = FALSE;
	m_bChkIlluminationEn = FALSE;
	m_bChkSmartIREn = FALSE;
	m_byBGain = 0;
	m_byRGain = 0;
	m_byEndTimeHour = 0;
	m_byEndTimeMin = 0;
	m_byEndTimeSec = 0;
	m_dwBackLightX1 = 0;
	m_dwBackLightX2 = 0;
	m_dwBackLightY1 = 0;
	m_dwBackLightY2 = 0;
	m_byBeginTimeHour = 0;
	m_byBeginTimeMin = 0;
	m_byBeginTimeSec = 0;
	m_byDehazeLevel = 0;
	m_bChkCorridorMode = FALSE;
	m_byElectLevel = 0;
	m_byIRDistance = 0;
	m_byPIrisAperture = 0;
	m_bChkISPSet = FALSE;
	m_byLaserAngle = 0;
	m_byLaserBrightness = 0;
	m_byLaserSensitivity = 0;
	m_byLaserLimitBrightness = 0;
	m_byShortIRDistance = 0;
	m_byLongIRDistance = 0;
	m_byAGCGainLevel = 0;
	m_byAGCLightLevel = 0;
	m_byDDEExpertLevel = 0;
	m_byDDELevel = 0;
	m_dwFFCTime = 0;
	m_bLensDistortionCorrection = FALSE;
	m_byIllumination = 0;
	m_bChkLaserEnabled = FALSE;
	m_iDeviceIndex = -1;
	m_lChannel = -1;
	m_byLightAngle = 0;
	m_bOpticalDehaze = FALSE;
    m_iHighTemp = 0;
    m_iLowTemp = 0;
	//}}AFX_DATA_INIT
	memset(&m_struDehaze, 0, sizeof(m_struDehaze));
	memset(&m_struCorridorMode, 0, sizeof(m_struCorridorMode));
	memset(&m_struISPCameraParamCfg, 0, sizeof(m_struISPCameraParamCfg));
}

/*********************************************************
  Function:	DoDataExchange
  Desc:		the map between control and variable
  Input:	pDX, CDataExchange,pass the data exchange object to the window CWnd::DoDataExchange
  Output:	none
  Return:	none
**********************************************************/
void CDlgCcdParam::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgCcdParam)
    DDX_Control(pDX, IDC_COMBO_VIDEOFORMAT, m_comVideoFormat);
    DDX_Control(pDX, IDC_COMBO_FOCUSING_POSITION_STATE, m_comboFocusingPositionState);
    DDX_Control(pDX, IDC_COMBO_AGC_TYPE, m_comAGCType);
    DDX_Control(pDX, IDC_COMBO_AGC_TYPE_TEMP, m_comThermometryAGCType);
    DDX_Control(pDX, IDC_COMBO_FFC_MODE, m_comFFCMode);
    DDX_Control(pDX, IDC_COMBO_DDE_MODE, m_comDDEMode);
    DDX_Control(pDX, IDC_COMBO_MIRROR, m_comboMirror);
    DDX_Control(pDX, IDC_COMBO_LASER_TRIGGER_MODE, m_cmLaserTriggerMode);
    DDX_Control(pDX, IDC_COMBO_CONTROL_MODE, m_cmLaserControlMode);
    DDX_Control(pDX, IDC_COMBO_CAPTURE_MODE2, m_comboCaptureMode2);
    DDX_Control(pDX, IDC_COMBO_CAPTURE_MODE, m_comboCaptureMode);
    DDX_Control(pDX, IDC_COMBO_PIRIS_MODE, m_comboPIrisMode);
    DDX_Control(pDX, IDC_COMBO_SMARTIR_MODE, m_comboSmartIRMode);
    DDX_Control(pDX, IDC_COM_ELECTE, m_comElecteSwitch);
    DDX_Control(pDX, IDC_COMBO_DEHAZE_MODE, m_comboDehazeMode);
    DDX_Control(pDX, IDC_COMBO_LOCALOUTPUTCATE, m_comboLocalOutPutGate);
    DDX_Control(pDX, IDC_COMBO_DAYNIGHT_FILTER_TYPE, m_comboDayNightType);
    DDX_Control(pDX, IDC_COMBO_ALARM_TRIG_STATE, m_comboAlarmTrigMode);
    DDX_Control(pDX, IDC_COMBO_BACKLIGHT_MODE, m_comboBackLightMode);
    DDX_Control(pDX, IDC_COMBO_WHITEBALANCE_MODE, m_comboWhiteBalanceMode);
    DDX_Control(pDX, IDC_COMBO_LIGHT_INHIBIT_LEVEL, m_comboLightInhibitLevel);
    DDX_Control(pDX, IDC_COMBO_GRAY_LEVEL, m_comboGrayLevel);
    DDX_Control(pDX, IDC_COMBO_DIGITAL_ZOOM, m_comboDigitalZoom);
    DDX_Control(pDX, IDC_COMBO_PALETTE_MODE, m_comboPaletteMode);
    DDX_Control(pDX, IDC_COMBO_FOCUS_SPEED, m_comboFocusSpeed);
    DDX_Control(pDX, IDC_COMBO_FILTER_SWITCH, m_comboFilterSwitch);
    DDX_Control(pDX, IDC_COMBO_ENHANCE_MODE, m_comboEnhanceMode);
    DDX_Control(pDX, IDC_COMBO_DIMMERMODE, m_comboDimmerMode);
    DDX_Control(pDX, IDC_COMBO_INOUT_DOOR_MODE, m_comboInOutMode);
    DDX_Control(pDX, IDC_COMBO_NOISEMOVEMODE, m_comboNoiseMoveMode);
    DDX_Control(pDX, IDC_COMBO_IRISMODE, m_ctrlIrisMode);
    DDX_Control(pDX, IDC_COMBO_FUSION_MODE, m_comboFusionMode);
    DDX_Text(pDX, IDC_EDIT_BRIGHTNESS, m_iBrightness);
    DDX_Text(pDX, IDC_EDIT_CONTRAST, m_iContrast);
    DDX_Text(pDX, IDC_EDIT_AGC_HIGH_TEMP, m_iHighTemp);
    DDX_Text(pDX, IDC_EDIT_AGC_LOW_TEMP, m_iLowTemp);
    DDX_Text(pDX, IDC_EDIT_GAIN, m_iGain);
    DDX_Text(pDX, IDC_EDIT_SATURATION, m_iSaturation);
    DDX_Text(pDX, IDC_EDIT_SHARPNESS, m_iSharpness);
    DDX_Text(pDX, IDC_EDIT_EXPOSUREUSERSET, m_iExposureUserSet);
    DDX_Text(pDX, IDC_EDIT_VEDIOEXPOSURE, m_iVedioExposure);
    DDX_Text(pDX, IDC_EDIT_USERGAIN, m_iUserGain);
    DDX_Text(pDX, IDC_EDIT_NORMAILEVEL, m_byNormalLevel);
    DDX_Text(pDX, IDC_EDIT_SPECTRALLEVEL, m_bySpectralLevel);
    DDX_Text(pDX, IDC_EDIT_TEMPORALLEVEL, m_byTemporalLevel);
    DDX_Text(pDX, IDC_EDIT_AUTO_COMP_INTERVAL, m_byAutoCompInter);
    DDX_Check(pDX, IDC_CHK_LIGHT_INHIBIT_EN, m_bChkLightInhibitEn);
    DDX_Check(pDX, IDC_CHK_ILLUMINATION_EN, m_bChkIlluminationEn);
    DDX_Check(pDX, IDC_CHK_SMARTIR_ENABLE, m_bChkSmartIREn);
    DDX_Text(pDX, IDC_EDIT_B_GAIN, m_byBGain);
    DDX_Text(pDX, IDC_EDIT_R_GAIN, m_byRGain);
    DDX_Text(pDX, IDC_EDIT_END_TIME_HOUR1, m_byEndTimeHour);
    DDX_Text(pDX, IDC_EDIT_END_TIME_MIN, m_byEndTimeMin);
    DDX_Text(pDX, IDC_EDIT_END_TIME_SEC, m_byEndTimeSec);
    DDX_Text(pDX, IDC_EDIT_BACKLIGHT_X1, m_dwBackLightX1);
    DDX_Text(pDX, IDC_EDIT_BACKLIGHT_X2, m_dwBackLightX2);
    DDX_Text(pDX, IDC_EDIT_BACKLIGHT_Y1, m_dwBackLightY1);
    DDX_Text(pDX, IDC_EDIT_BACKLIGHT_Y2, m_dwBackLightY2);
    DDX_Text(pDX, IDC_EDIT_BEGIN_TIME_HOUR, m_byBeginTimeHour);
    DDX_Text(pDX, IDC_EDIT_BEGIN_TIME_MIN, m_byBeginTimeMin);
    DDX_Text(pDX, IDC_EDIT_BEGIN_TIME_SEC, m_byBeginTimeSec);
    DDX_Text(pDX, IDC_EDIT_DEHAZE_LEVEL, m_byDehazeLevel);
    DDX_Check(pDX, IDC_CHECK_CORRIDOR_MODE, m_bChkCorridorMode);
    DDX_Text(pDX, IDC_EDIT_ELECT_LEVEL, m_byElectLevel);
    DDX_Text(pDX, IDC_EDIT_IRDISTANCE, m_byIRDistance);
    DDX_Text(pDX, IDC_EDIT_PIRIS_APERTURE, m_byPIrisAperture);
    DDX_Check(pDX, IDC_CHK_ISPSET, m_bChkISPSet);
    DDX_Text(pDX, IDC_EDIT_ANGLE, m_byLaserAngle);
    DDX_Text(pDX, IDC_EDIT_LASER_BRIGHTNESS, m_byLaserBrightness);
    DDX_Text(pDX, IDC_EDIT_LASER_SENSITIVITY, m_byLaserSensitivity);
    DDX_Text(pDX, IDC_EDIT_LIMIT_BRIGHTNESS, m_byLaserLimitBrightness);
    DDX_Text(pDX, IDC_EDIT_SHORTIR_DISTANCE, m_byShortIRDistance);
    DDX_Text(pDX, IDC_EDIT_LONGIR_DISTANCE, m_byLongIRDistance);
    DDX_Text(pDX, IDC_EDIT_AGC_GAINLEVEL, m_byAGCGainLevel);
    DDX_Text(pDX, IDC_EDIT_AGC_LIGHTLEVEL, m_byAGCLightLevel);
    DDX_Text(pDX, IDC_EDIT_DDE_EXPERT_LEVEL, m_byDDEExpertLevel);
    DDX_Text(pDX, IDC_EDIT_DDE_LEVEL, m_byDDELevel);
    DDX_Text(pDX, IDC_EDIT_FFC_TIME, m_dwFFCTime);
    DDX_Check(pDX, IDC_CHECK_LENS_DIST_CORR, m_bLensDistortionCorrection);
    DDX_Text(pDX, IDC_EDIT_LIMIT_ILLUMINATION, m_byIllumination);
    DDX_Check(pDX, IDC_CHECK_LASER_ENABLED, m_bChkLaserEnabled);
    DDX_Text(pDX, IDC_EDIT_LIGHT_ANGLE, m_byLightAngle);
    DDX_Check(pDX, IDC_CHECK_OPTICAL_DEHAZE, m_bOpticalDehaze);
    //}}AFX_DATA_MAP
    DDX_Control(pDX, IDC_COMBO_DistCorrectLevel, m_comDistortionCorrectionLevel);
    DDX_Text(pDX, IDC_EDIT4, m_byCalibrationAccurateLevel);
    DDX_Text(pDX, IDC_EDIT5, m_byZoomedInDistantViewLevel);
    DDX_Text(pDX, IDC_EDIT6, m_byHorizontalFOV);
    DDX_Text(pDX, IDC_EDIT8, m_byVerticalFOV);
    DDX_Check(pDX, IDC_CHECK1, m_byBrightnessSuddenChangeSuppression);
    DDX_Text(pDX, IDC_EDIT_CHANINDEX, m_iCurChanIndex);
    DDX_Check(pDX, IDC_CHECK_GPSENABLED, m_checkGPSEnabled);
}


BEGIN_MESSAGE_MAP(CDlgCcdParam, CDialog)
	//{{AFX_MSG_MAP(CDlgCcdParam)
	ON_BN_CLICKED(IDC_BTN_GET, OnBtnGet)
	ON_BN_CLICKED(IDC_BTN_SET, OnBtnSet)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	ON_BN_CLICKED(IDC_BTN_INFRARE_CFG, OnBtnInfrareCfg)
	ON_BN_CLICKED(IDC_BTN_SET_CORRIDOR_MODE, OnBtnSetCorridorMode)
	ON_BN_CLICKED(IDC_BTN_GET_CORRIDOR_MODE, OnBtnGetCorridorMode)
	ON_BN_CLICKED(IDC_BTN_GET_EX, OnBtnGetEx)
	ON_BN_CLICKED(IDC_BTN_SET_EX, OnBtnSetEx)
	ON_CBN_SELCHANGE(IDC_COMBO_IRISMODE, OnSelchangeComboIrismode)
	ON_CBN_SELCHANGE(IDC_COMBO_PIRIS_MODE, OnSelchangeComboPirisMode)
	ON_CBN_SELCHANGE(IDC_COMBO_SMARTIR_MODE, OnSelchangeComboSmartirMode)
    ON_CBN_SELCHANGE(IDC_COMBO_AGC_TYPE_TEMP, OnSelchangeComboAGCTempMode)
	ON_BN_CLICKED(IDC_BTN_ISP_PARAMSET, OnBtnIspParamset)
	ON_BN_CLICKED(IDC_BTN_SIGNALLIGHTSYNC, OnBtnSignallightsync)
	ON_BN_CLICKED(IDC_BTN_EZVIZ_ACCESSCFG, OnBtnEzvizAccesscfg)
	ON_BN_CLICKED(IDC_BTN_IOOUTCFG, OnBtnIOoutCfg)
	ON_BN_CLICKED(IDC_BTN_DPC, OnBtnDpc)
	ON_BN_CLICKED(IDC_BTN_FFC_MANUAL, OnBtnFfcManual)
	ON_BN_CLICKED(IDC_BTN_FFC_BACKCOMP, OnBtnFfcBackcomp)
	ON_CBN_SELCHANGE(IDC_COMBO_FFC_MODE, OnSelchangeComboFfcMode)
	ON_CBN_SELCHANGE(IDC_COMBO_DDE_MODE, OnSelchangeComboDdeMode)
	ON_CBN_SELCHANGE(IDC_COMBO_AGC_TYPE, OnSelchangeComboAgcType)
	ON_BN_CLICKED(IDC_BTN_FOCUSING_POSITION_STATE, OnBtnFocusingPositionState)
	ON_BN_CLICKED(IDC_BTN_SUPPLEMENTLIGHT, OnBtnSupplementlight)
	//}}AFX_MSG_MAP
    ON_EN_CHANGE(IDC_EDIT_CHANINDEX, &CDlgCcdParam::OnEnChangeEditChanindex)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCcdParam message handlers
/*********************************************************
  Function:	OnBtnGet
  Desc:		CCD parameter get
  Input:	
  Output:	
  Return:	
**********************************************************/
void CDlgCcdParam::OnBtnGet() 
{
    UpdateData(TRUE);
    DWORD dwReturn = 0;
    memset(&m_CcdParam, 0, sizeof(m_CcdParam));
    m_CcdParam.dwSize = sizeof(m_CcdParam);
    TRACE("ccdparamstruct size = %d", sizeof(m_CcdParam));
    if (!NET_DVR_GetDVRConfig(g_struDeviceInfo[m_iDeviceIndex].lLoginID, NET_DVR_GET_CCDPARAMCFG, m_lChannel, &m_CcdParam, sizeof(m_CcdParam), &dwReturn))
    {
		g_pMainDlg->AddLog(g_struDeviceInfo[m_iDeviceIndex].lLoginID, OPERATION_FAIL_T, "NET_DVR_GET_CCDPARAMCFG");
    }
	else
	{
        g_pMainDlg->AddLog(g_struDeviceInfo[m_iDeviceIndex].lLoginID, OPERATION_SUCC_T, "NET_DVR_GET_CCDPARAMCFG");
	}

	if (m_CcdParam.byLocalOutputGate >= 20 && m_CcdParam.byLocalOutputGate <= 28)
	{
		m_comboLocalOutPutGate.SetCurSel(m_CcdParam.byLocalOutputGate - 16);
	}
	
	if (m_CcdParam.byLocalOutputGate >= 40 && m_CcdParam.byLocalOutputGate <= 48)
	{
		m_comboLocalOutPutGate.SetCurSel(m_CcdParam.byLocalOutputGate - 27);
	}
	else
	{
		m_comboLocalOutPutGate.SetCurSel(m_CcdParam.byLocalOutputGate);	
	}

    m_iBrightness = m_CcdParam.struVideoEffect.byBrightnessLevel;
    m_iExposureUserSet = m_CcdParam.struExposure.dwExposureUserSet;
    m_iVedioExposure = m_CcdParam.struExposure.dwVideoExposureSet;
    m_iContrast = m_CcdParam.struVideoEffect.byContrastLevel;
    m_iGain = m_CcdParam.struGain.byGainLevel;
	m_iUserGain = m_CcdParam.struGain.byGainUserSet;
    m_iSaturation = m_CcdParam.struVideoEffect.bySaturationLevel;
    m_iSharpness = m_CcdParam.struVideoEffect.bySharpnessLevel;
	m_bChkSmartIREn = m_CcdParam.struVideoEffect.byEnableFunc&0x01;
	m_bChkIlluminationEn = (m_CcdParam.struVideoEffect.byEnableFunc>>1)&0x01;
	m_bChkLightInhibitEn = (m_CcdParam.struVideoEffect.byEnableFunc>>2)&0x01;
	m_comboLightInhibitLevel.SetCurSel(m_CcdParam.struVideoEffect.byLightInhibitLevel-1);
	m_comboGrayLevel.SetCurSel(m_CcdParam.struVideoEffect.byGrayLevel);
	m_ctrlIrisMode.SetCurSel(m_CcdParam.byIrisMode);

	m_comboWhiteBalanceMode.SetCurSel(m_CcdParam.struWhiteBalance.byWhiteBalanceMode);
	m_byBGain = m_CcdParam.struWhiteBalance.byWhiteBalanceModeBGain;
	m_byRGain = m_CcdParam.struWhiteBalance.byWhiteBalanceModeRGain;

	m_comboMirror.SetCurSel(m_CcdParam.byMirror);
	m_comboNoiseMoveMode.SetCurSel(m_CcdParam.struNoiseRemove.byDigitalNoiseRemoveEnable);
	m_byNormalLevel = m_CcdParam.struNoiseRemove.byDigitalNoiseRemoveLevel;
	m_bySpectralLevel = m_CcdParam.struNoiseRemove.bySpectralLevel;
	m_byTemporalLevel = m_CcdParam.struNoiseRemove.byTemporalLevel;

	m_comboDigitalZoom.SetCurSel(m_CcdParam.byDigitalZoom);
	m_comboDimmerMode.SetCurSel(m_CcdParam.byDimmerMode);
	m_comboEnhanceMode.SetCurSel(m_CcdParam.byEnhancedMode);
	m_comboPaletteMode.SetCurSel(m_CcdParam.byPaletteMode);
	m_comboFilterSwitch.SetCurSel(m_CcdParam.byFilterSwitch);
	m_comboFocusSpeed.SetCurSel(m_CcdParam.byFocusSpeed);
	m_byAutoCompInter = m_CcdParam.byAutoCompensationInterval;

	m_comboInOutMode.SetCurSel(m_CcdParam.bySceneMode);
	
	m_comboDayNightType.SetCurSel(m_CcdParam.struDayNight.byDayNightFilterType);
	m_byBeginTimeHour = m_CcdParam.struDayNight.byBeginTime;
	m_byBeginTimeMin = m_CcdParam.struDayNight.byBeginTimeMin;
	m_byBeginTimeSec = m_CcdParam.struDayNight.byBeginTimeSec;
	m_byEndTimeHour = m_CcdParam.struDayNight.byEndTime;
	m_byEndTimeMin = m_CcdParam.struDayNight.byEndTimeMin;
	m_byEndTimeSec = m_CcdParam.struDayNight.byEndTimeSec;
	m_comboAlarmTrigMode.SetCurSel(m_CcdParam.struDayNight.byAlarmTrigState);

    if (m_CcdParam.struBackLight.byBacklightMode >= 10 && m_CcdParam.struBackLight.byBacklightMode <= 12)
    {
        m_comboBackLightMode.SetCurSel(m_CcdParam.struBackLight.byBacklightMode - 3);
    }
    else
    {
        m_comboBackLightMode.SetCurSel(m_CcdParam.struBackLight.byBacklightMode);
    }
    
	m_dwBackLightX1 = m_CcdParam.struBackLight.dwPositionX1;
	m_dwBackLightX2 = m_CcdParam.struBackLight.dwPositionX2;
	m_dwBackLightY1 = m_CcdParam.struBackLight.dwPositionY1;
	m_dwBackLightY2 = m_CcdParam.struBackLight.dwPositionY2;


	if (!NET_DVR_GetDVRConfig(g_struDeviceInfo[m_iDeviceIndex].lLoginID, NET_DVR_GET_CAMERA_DEHAZE_CFG, m_lChannel, &m_struDehaze, sizeof(m_struDehaze), &dwReturn))
    {
		g_pMainDlg->AddLog(g_struDeviceInfo[m_iDeviceIndex].lLoginID, OPERATION_FAIL_T, "NET_DVR_GET_CAMERA_DEHAZE_CFG");
    }
	else
	{
        g_pMainDlg->AddLog(g_struDeviceInfo[m_iDeviceIndex].lLoginID, OPERATION_SUCC_T, "NET_DVR_GET_CAMERA_DEHAZE_CFG");
	}
	m_comboDehazeMode.SetCurSel(m_struDehaze.byDehazeMode);
	m_byDehazeLevel = m_struDehaze.byLevel;
 
	SetNewInfoToWnd();

    UpdateData(FALSE);
}

/*********************************************************
  Function:	OnBtnSet
  Desc:		CCD parameter set
  Input:	
  Output:	
  Return:	
**********************************************************/
void CDlgCcdParam::OnBtnSet() 
{
    UpdateData(TRUE);
	if((m_iBrightness < 0) || (m_iBrightness >100) || (m_iContrast < 0) || (m_iContrast >100) ||  \
		(m_iSaturation < 0) || (m_iSaturation >100) || (m_iSharpness < 0) || (m_iSharpness >100) ||  \
		(m_iGain < 0) || (m_iGain >100) || (m_iUserGain < 0) || (m_iUserGain > 100))
	{
		return;
	}
    m_CcdParam.struVideoEffect.byBrightnessLevel = m_iBrightness;
    m_CcdParam.struVideoEffect.byContrastLevel = m_iContrast;
    m_CcdParam.struVideoEffect.bySaturationLevel = m_iSaturation;
    m_CcdParam.struVideoEffect.bySharpnessLevel = m_iSharpness;
	m_CcdParam.struVideoEffect.byEnableFunc = 0;
	m_CcdParam.struVideoEffect.byEnableFunc |= m_bChkSmartIREn;
	m_CcdParam.struVideoEffect.byEnableFunc |= (m_bChkIlluminationEn<<1);
	m_CcdParam.struVideoEffect.byEnableFunc |= (m_bChkLightInhibitEn<<2);
	m_CcdParam.struVideoEffect.byLightInhibitLevel = m_comboLightInhibitLevel.GetCurSel()+1;
	m_CcdParam.struVideoEffect.byGrayLevel = m_comboGrayLevel.GetCurSel();

    m_CcdParam.struExposure.dwExposureUserSet = m_iExposureUserSet;
    m_CcdParam.struExposure.dwVideoExposureSet = m_iVedioExposure;
    m_CcdParam.struGain.byGainLevel = m_iGain;
	m_CcdParam.struGain.byGainUserSet = m_iUserGain;
	m_CcdParam.byIrisMode = m_ctrlIrisMode.GetCurSel();
    m_CcdParam.dwSize = sizeof(m_CcdParam);

	m_CcdParam.struWhiteBalance.byWhiteBalanceMode = m_comboWhiteBalanceMode.GetCurSel();
	m_CcdParam.struWhiteBalance.byWhiteBalanceModeBGain = m_byBGain;
	m_CcdParam.struWhiteBalance.byWhiteBalanceModeRGain = m_byRGain;

	m_CcdParam.byMirror = m_comboMirror.GetCurSel();
	m_CcdParam.struNoiseRemove.byDigitalNoiseRemoveEnable = m_comboNoiseMoveMode.GetCurSel();
	m_CcdParam.struNoiseRemove.byDigitalNoiseRemoveLevel = m_byNormalLevel;
	m_CcdParam.struNoiseRemove.bySpectralLevel = m_bySpectralLevel;
	m_CcdParam.struNoiseRemove.byTemporalLevel = m_byTemporalLevel;

	m_CcdParam.byDigitalZoom = m_comboDigitalZoom.GetCurSel();
	m_CcdParam.byDimmerMode = m_comboDimmerMode.GetCurSel();
	m_CcdParam.byEnhancedMode = m_comboEnhanceMode.GetCurSel();
	m_CcdParam.byPaletteMode = m_comboPaletteMode.GetCurSel();
	m_CcdParam.byFilterSwitch = m_comboFilterSwitch.GetCurSel();
	m_CcdParam.byAutoCompensationInterval = m_byAutoCompInter;
	m_CcdParam.byFocusSpeed = m_comboFocusSpeed.GetCurSel();

	m_CcdParam.bySceneMode = m_comboInOutMode.GetCurSel();
	m_CcdParam.byLocalOutputGate = m_comboLocalOutPutGate.GetItemData(m_comboLocalOutPutGate.GetCurSel());
	m_CcdParam.struDayNight.byDayNightFilterType = m_comboDayNightType.GetCurSel();
	m_CcdParam.struDayNight.byBeginTime = m_byBeginTimeHour;
	m_CcdParam.struDayNight.byBeginTimeMin = m_byBeginTimeMin;
	m_CcdParam.struDayNight.byBeginTimeSec = m_byBeginTimeSec;
	m_CcdParam.struDayNight.byEndTime = m_byEndTimeHour;
	m_CcdParam.struDayNight.byEndTimeMin = m_byEndTimeMin;
	m_CcdParam.struDayNight.byEndTimeSec = m_byEndTimeSec;
	m_CcdParam.struDayNight.byAlarmTrigState = m_comboAlarmTrigMode.GetCurSel();

    m_CcdParam.struBackLight.byBacklightMode = m_comboBackLightMode.GetItemData(m_comboBackLightMode.GetCurSel());
	m_CcdParam.struBackLight.dwPositionX1 = m_dwBackLightX1;
	m_CcdParam.struBackLight.dwPositionX2 = m_dwBackLightX2;
	m_CcdParam.struBackLight.dwPositionY1 = m_dwBackLightY1;
	m_CcdParam.struBackLight.dwPositionY2 = m_dwBackLightY2;


    if (!NET_DVR_SetDVRConfig(g_struDeviceInfo[m_iDeviceIndex].lLoginID, NET_DVR_SET_CCDPARAMCFG, m_lChannel, &m_CcdParam, sizeof(m_CcdParam)))
    {
		g_pMainDlg->AddLog(g_struDeviceInfo[m_iDeviceIndex].lLoginID, OPERATION_FAIL_T, "NET_DVR_SET_CCDPARAMCFG");
        return;
    }
	else
	{
		g_pMainDlg->AddLog(g_struDeviceInfo[m_iDeviceIndex].lLoginID, OPERATION_SUCC_T, "NET_DVR_SET_CCDPARAMCFG");
	}

	m_struDehaze.byDehazeMode = m_comboDehazeMode.GetCurSel();
	m_struDehaze.byLevel = m_byDehazeLevel;
    m_struDehaze.dwSize = sizeof(m_struDehaze);
	if (!NET_DVR_SetDVRConfig(g_struDeviceInfo[m_iDeviceIndex].lLoginID, NET_DVR_SET_CAMERA_DEHAZE_CFG, m_lChannel, &m_struDehaze, sizeof(m_struDehaze)))
    {
		g_pMainDlg->AddLog(g_struDeviceInfo[m_iDeviceIndex].lLoginID, OPERATION_FAIL_T, "NET_DVR_SET_CAMERA_DEHAZE_CFG");
        return;
    }
	else
	{
		g_pMainDlg->AddLog(g_struDeviceInfo[m_iDeviceIndex].lLoginID, OPERATION_SUCC_T, "NET_DVR_SET_CAMERA_DEHAZE_CFG");
	}
}

/*********************************************************
  Function:	OnBtnExit
  Desc:		Exit fun
  Input:	
  Output:	
  Return:	
**********************************************************/
void CDlgCcdParam::OnBtnExit() 
{
    CDialog::OnCancel();	
}

/*********************************************************
  Function:	OnInitDialog
  Desc:		Initialize the dialog
  Input:	
  Output:	
  Return:	
**********************************************************/
BOOL CDlgCcdParam::OnInitDialog() 
{
	CDialog::OnInitDialog();
	InitLocalOutPutGate();
	GetDlgItem(IDC_COMBO_PIRIS_MODE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_PIRIS_APERTURE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_PIRIS_APERTURE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_PIRIS_MODE)->ShowWindow(SW_HIDE);
    AddCaptureMode();
    AddCaptureModeP();
    AddBackLightMode();
    m_iCurChanIndex = m_lChannel;
	OnBtnGet();
	OnBtnGetEx();

    UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCcdParam::InitLocalOutPutGate()
{
	char szLan[128] = {0};
	m_comboLocalOutPutGate.ResetContent();
	g_StringLanType(szLan, "本地输出关闭", "Local output off");
	m_comboLocalOutPutGate.InsertString(0, szLan);
    m_comboLocalOutPutGate.SetItemData(0, 0);
	
	g_StringLanType(szLan, "本地输出打开", "Local output on");
	m_comboLocalOutPutGate.InsertString(1, szLan);
    m_comboLocalOutPutGate.SetItemData(1, 1);
	
	g_StringLanType(szLan, "缩放输出", "Scaling output");
	m_comboLocalOutPutGate.InsertString(2, szLan);
    m_comboLocalOutPutGate.SetItemData(2, 11);
	
	g_StringLanType(szLan, "裁剪输出", "Cutting output");
	m_comboLocalOutPutGate.InsertString(3, szLan);
    m_comboLocalOutPutGate.SetItemData(3, 12);
	//
	//20-HDMI_720P50输出开
	//21-HDMI_720P60输出开
	//22-HDMI_1080I60输出开
	//23-HDMI_1080I50输出开
	//24-HDMI_1080P24输出开
	//25-HDMI_1080P25输出开
	//26-HDMI_1080P30输出开
	//27-HDMI_1080P50输出开
	//28-HDMI_1080P60输出开
	g_StringLanType(szLan, "HDMI_720P50输出", "HDMI_720P50 output");
	m_comboLocalOutPutGate.InsertString(4, szLan);
    m_comboLocalOutPutGate.SetItemData(4, 20);
	
	g_StringLanType(szLan, "HDMI_720P60输出", "HDMI_720P60 output");
	m_comboLocalOutPutGate.InsertString(5, szLan);
    m_comboLocalOutPutGate.SetItemData(5, 21);
	
	g_StringLanType(szLan, "HDMI_1080I60输出", "SDI_1080I60 output");
	m_comboLocalOutPutGate.InsertString(6, szLan);
    m_comboLocalOutPutGate.SetItemData(6, 22);
	
	g_StringLanType(szLan, "HDMI_1080I50输出", "HDMI_1080I50 output");
	m_comboLocalOutPutGate.InsertString(7, szLan);
    m_comboLocalOutPutGate.SetItemData(7, 23);
	
	g_StringLanType(szLan, "HDMI_1080P24输出", "HDMI_1080P24 output");
	m_comboLocalOutPutGate.InsertString(8, szLan);
    m_comboLocalOutPutGate.SetItemData(8, 24);
	
	g_StringLanType(szLan, "HDMI_1080P25输出", "HDMI_1080P25 output");
	m_comboLocalOutPutGate.InsertString(9, szLan);
    m_comboLocalOutPutGate.SetItemData(9, 25);
	
	g_StringLanType(szLan, "HDMI_1080P30输出", "HDMI_1080P30 output");
	m_comboLocalOutPutGate.InsertString(10, szLan);
    m_comboLocalOutPutGate.SetItemData(10, 26);
	
	g_StringLanType(szLan, "HDMI_1080P50输出", "HDMI_1080P50 output");
	m_comboLocalOutPutGate.InsertString(11, szLan);
    m_comboLocalOutPutGate.SetItemData(11, 27);
	
	g_StringLanType(szLan, "HDMI_1080P60输出", "HDMI_1080P60 output");
	m_comboLocalOutPutGate.InsertString(12, szLan);
    m_comboLocalOutPutGate.SetItemData(12, 28);
	//SDI输出测试40～48
	//40-SDI_720P50,
	//41-SDI_720P60,
	//42-SDI_1080I50,
	//43-SDI_1080I60,
	//44-SDI_1080P24,
	//45-SDI_1080P25,
	//46-SDI_1080P30,
	//47-SDI_1080P50,
	//48-SDI_1080P60
	g_StringLanType(szLan, "SDI_720P50输出", "SDI_720P50 output");
	m_comboLocalOutPutGate.InsertString(13, szLan);
    m_comboLocalOutPutGate.SetItemData(13, 40);
	
	g_StringLanType(szLan, "SDI_720P60输出", "SDI_720P60 output");
	m_comboLocalOutPutGate.InsertString(14, szLan);
    m_comboLocalOutPutGate.SetItemData(14, 41);
	
	g_StringLanType(szLan, "SDI_1080I50输出", "SDI_1080I50 output");
	m_comboLocalOutPutGate.InsertString(15, szLan);
    m_comboLocalOutPutGate.SetItemData(15, 42);
	
	g_StringLanType(szLan, "SDI_1080I60输出", "SDI_1080I60 output");
	m_comboLocalOutPutGate.InsertString(16, szLan);
    m_comboLocalOutPutGate.SetItemData(16, 43);
	
	g_StringLanType(szLan, "SDI_1080P24输出", "SDI_1080P24 output");
	m_comboLocalOutPutGate.InsertString(17, szLan);
    m_comboLocalOutPutGate.SetItemData(17, 44);
	
	g_StringLanType(szLan, "SDI_1080P25输出", "SDI_1080P25 output");
	m_comboLocalOutPutGate.InsertString(18, szLan);
    m_comboLocalOutPutGate.SetItemData(18, 45);
	
	g_StringLanType(szLan, "SDI_1080P30输出", "SDI_1080P30 output");
	m_comboLocalOutPutGate.InsertString(19, szLan);
    m_comboLocalOutPutGate.SetItemData(19, 46);
	
	g_StringLanType(szLan, "SDI_1080P50输出", "SDI_1080P50 output");
	m_comboLocalOutPutGate.InsertString(20, szLan);
    m_comboLocalOutPutGate.SetItemData(20, 47);
	
	g_StringLanType(szLan, "SDI_1080P60输出", "SDI_1080P60 output");
	m_comboLocalOutPutGate.InsertString(21, szLan);
    m_comboLocalOutPutGate.SetItemData(21, 48);

	m_comboWhiteBalanceMode.ResetContent();
	g_StringLanType(szLan, "手动白平衡","MWB");
	m_comboWhiteBalanceMode.InsertString(0, szLan);
	m_comboWhiteBalanceMode.SetItemData(0, 0);
	
	g_StringLanType(szLan, "自动白平衡1", "AWB1");
	m_comboWhiteBalanceMode.InsertString(1, szLan);
	m_comboWhiteBalanceMode.SetItemData(1, 1);
	
	g_StringLanType(szLan, "自动白平衡2", "AWB2");
	m_comboWhiteBalanceMode.InsertString(2, szLan);
	m_comboWhiteBalanceMode.SetItemData(2, 2);
	
	g_StringLanType(szLan, "锁定白平衡", "Locked WB");
	m_comboWhiteBalanceMode.InsertString(3, szLan);
	m_comboWhiteBalanceMode.SetItemData(3, 3);
	
	g_StringLanType(szLan, "室外", "Outdoor");
	m_comboWhiteBalanceMode.InsertString(4, szLan);
	m_comboWhiteBalanceMode.SetItemData(4, 4);
	
	g_StringLanType(szLan, "室内", "Indoor");
	m_comboWhiteBalanceMode.InsertString(5, szLan);
	m_comboWhiteBalanceMode.SetItemData(5, 5);
	
	g_StringLanType(szLan, "日光灯", "Fluorescent Lamp");
	m_comboWhiteBalanceMode.InsertString(6, szLan);
	m_comboWhiteBalanceMode.SetItemData(6, 6);
	
	g_StringLanType(szLan, "钠灯", "Sodium Lamp");
	m_comboWhiteBalanceMode.InsertString(7, szLan);
	m_comboWhiteBalanceMode.SetItemData(7, 7);
	
	g_StringLanType(szLan, "自动跟随", "Auto-Track");
	m_comboWhiteBalanceMode.InsertString(8, szLan);
	m_comboWhiteBalanceMode.SetItemData(8, 8);
	
	g_StringLanType(szLan, "一次白平衡", "One Push");
	m_comboWhiteBalanceMode.InsertString(9, szLan);
	m_comboWhiteBalanceMode.SetItemData(9, 9);
	
	g_StringLanType(szLan, "室外自动", "Auto-Outdoor");
	m_comboWhiteBalanceMode.InsertString(10, szLan);
	m_comboWhiteBalanceMode.SetItemData(10, 10);
	
	g_StringLanType(szLan, "钠灯自动", "Auto-Sodiumlight");
	m_comboWhiteBalanceMode.InsertString(11, szLan);
	m_comboWhiteBalanceMode.SetItemData(11, 11);
	
	g_StringLanType(szLan, "水银灯", "Mercury Lamp");
	m_comboWhiteBalanceMode.InsertString(12, szLan);
	m_comboWhiteBalanceMode.SetItemData(12, 12);
	
	g_StringLanType(szLan, "自动白平衡", "Auto-WB");
	m_comboWhiteBalanceMode.InsertString(13, szLan);
	m_comboWhiteBalanceMode.SetItemData(13, 13);
	
	g_StringLanType(szLan, "白炽灯", "IncandescentLamp");
	m_comboWhiteBalanceMode.InsertString(14, szLan);
	m_comboWhiteBalanceMode.SetItemData(14, 14);
	
	g_StringLanType(szLan, "暖光灯", "Warm Light Lamp");
	m_comboWhiteBalanceMode.InsertString(15, szLan);
	m_comboWhiteBalanceMode.SetItemData(15, 15);
	
	g_StringLanType(szLan, "自然光", "Natural Light");
	m_comboWhiteBalanceMode.InsertString(16, szLan);
	m_comboWhiteBalanceMode.SetItemData(16, 16);

	
	m_cmLaserControlMode.ResetContent();
	g_StringLanType(szLan, "自动", "Auto");
	m_cmLaserControlMode.AddString(szLan);
	g_StringLanType(szLan, "手动", "manual");
	m_cmLaserControlMode.AddString(szLan);
	
	m_cmLaserTriggerMode.ResetContent();
	g_StringLanType(szLan, "机芯触发", "Camera Module Trigger ");
	m_cmLaserTriggerMode.AddString(szLan);
	g_StringLanType(szLan, "光敏触发", "Photoresistance Trigger");
	m_cmLaserTriggerMode.AddString(szLan);

    m_comFFCMode.ResetContent();
    g_StringLanType(szLan, "定时模式", "Timing");
    m_comFFCMode.AddString(szLan);
    g_StringLanType(szLan, "温度模式", "Temperature");
    m_comFFCMode.AddString(szLan);
    g_StringLanType(szLan, "关", "OFF");
    m_comFFCMode.AddString(szLan);
    m_comFFCMode.SetCurSel(0);

    m_comDDEMode.ResetContent();
    g_StringLanType(szLan, "关闭", "OFF");
    m_comDDEMode.AddString(szLan);
    g_StringLanType(szLan, "正常模式", "Normal");
    m_comDDEMode.AddString(szLan);
    g_StringLanType(szLan, "专家模式", "Expert");
    m_comDDEMode.AddString(szLan);
    m_comDDEMode.SetCurSel(0);

    m_comAGCType.ResetContent();
    g_StringLanType(szLan, "正常", "Normal");
    m_comAGCType.AddString(szLan);
    g_StringLanType(szLan, "高亮", "Highlight");
    m_comAGCType.AddString(szLan);
    g_StringLanType(szLan, "手动", "Manual");
    m_comAGCType.AddString(szLan);
    m_comAGCType.SetCurSel(0);
}

void CDlgCcdParam::OnBtnInfrareCfg() 
{
	// TODO: Add your control notification handler code here
	CDlgInfrareCfg dlg;
	dlg.m_iDevIndex = m_iDeviceIndex;
    dlg.m_lChannel = m_lChannel;
	dlg.DoModal();
}

void CDlgCcdParam::OnBtnSetCorridorMode() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_struCorridorMode.dwSize = sizeof(m_struCorridorMode);
	m_struCorridorMode.byEnableCorridorMode = m_bChkCorridorMode;
	
    if (!NET_DVR_SetDVRConfig(g_struDeviceInfo[m_iDeviceIndex].lLoginID, NET_DVR_SET_CORRIDOR_MODE, m_lChannel, &m_struCorridorMode, sizeof(m_struCorridorMode)))
    {
		g_pMainDlg->AddLog(g_struDeviceInfo[m_iDeviceIndex].lLoginID, OPERATION_FAIL_T, "NET_DVR_SET_CORRIDOR_MODE");
        return;
    }
	else
	{
		g_pMainDlg->AddLog(g_struDeviceInfo[m_iDeviceIndex].lLoginID, OPERATION_SUCC_T, "NET_DVR_SET_CORRIDOR_MODE");
	}
	
}

void CDlgCcdParam::OnBtnGetCorridorMode() 
{
	// TODO: Add your control notification handler code here
	DWORD dwReturn = 0;
	memset(&m_struCorridorMode, 0, sizeof(m_struCorridorMode));
	
    if (!NET_DVR_GetDVRConfig(g_struDeviceInfo[m_iDeviceIndex].lLoginID, NET_DVR_GET_CORRIDOR_MODE, m_lChannel, &m_struCorridorMode, sizeof(m_struCorridorMode), &dwReturn))
    {
		g_pMainDlg->AddLog(g_struDeviceInfo[m_iDeviceIndex].lLoginID, OPERATION_FAIL_T, "NET_DVR_GET_CCDPARAMCFG");
		return;
    }
	else
	{
        g_pMainDlg->AddLog(g_struDeviceInfo[m_iDeviceIndex].lLoginID, OPERATION_SUCC_T, "NET_DVR_GET_CCDPARAMCFG");
	}
	m_bChkCorridorMode = m_struCorridorMode.byEnableCorridorMode;
	UpdateData(FALSE);
}


void CDlgCcdParam::OnBtnGetEx() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	DWORD dwReturn = 0;
	memset(&m_CcdParamEx, 0, sizeof(m_CcdParamEx));
	if (m_bChkISPSet)
	{
		memset(&m_struISPCameraParamCfg, 0, sizeof(m_struISPCameraParamCfg));
        if (!NET_DVR_GetDVRConfig(g_struDeviceInfo[m_iDeviceIndex].lLoginID, NET_DVR_GET_ISP_CAMERAPARAMCFG, m_lChannel, \
			&m_struISPCameraParamCfg, sizeof(m_struISPCameraParamCfg), &dwReturn))
		{
			g_pMainDlg->AddLog(g_struDeviceInfo[m_iDeviceIndex].lLoginID, OPERATION_FAIL_T, "Error Code = %d ;NET_DVR_GET_ISP_CAMERAPARAMCFG", NET_DVR_GetLastError());
		}
		else
		{
			g_pMainDlg->AddLog(g_struDeviceInfo[m_iDeviceIndex].lLoginID, OPERATION_SUCC_T, "Error Code = %d ;NET_DVR_GET_ISP_CAMERAPARAMCFG", NET_DVR_GetLastError());

			memcpy(&m_CcdParamEx, &m_struISPCameraParamCfg.struSelfAdaptiveParam, sizeof(m_CcdParamEx));
		}
	} 
	else
	{
		
		m_CcdParamEx.dwSize = sizeof(m_CcdParamEx);
		TRACE("ccdparamstruct size = %d", sizeof(m_CcdParamEx));
        if (!NET_DVR_GetDVRConfig(g_struDeviceInfo[m_iDeviceIndex].lLoginID, NET_DVR_GET_CCDPARAMCFG_EX, m_lChannel, &m_CcdParamEx, sizeof(m_CcdParamEx), &dwReturn))
		{
			g_pMainDlg->AddLog(g_struDeviceInfo[m_iDeviceIndex].lLoginID, OPERATION_FAIL_T, "Error Code = %d ;NET_DVR_GET_CCDPARAMCFG_EX", NET_DVR_GetLastError());
		}
		else
		{
			g_pMainDlg->AddLog(g_struDeviceInfo[m_iDeviceIndex].lLoginID, OPERATION_SUCC_T, "Error Code = %d ;NET_DVR_GET_CCDPARAMCFG_EX", NET_DVR_GetLastError());
		}
	}

	//m_comboLocalOutPutGate.SetCurSel(m_CcdParamEx.byLocalOutputGate);
	if (m_CcdParamEx.byLocalOutputGate >= 20 && m_CcdParamEx.byLocalOutputGate <= 28)
	{
		m_comboLocalOutPutGate.SetCurSel(m_CcdParamEx.byLocalOutputGate - 16);
	}
	
	if (m_CcdParamEx.byLocalOutputGate >= 40 && m_CcdParamEx.byLocalOutputGate <= 48)
	{
		m_comboLocalOutPutGate.SetCurSel(m_CcdParamEx.byLocalOutputGate - 27);
	}
	else
	{
		m_comboLocalOutPutGate.SetCurSel(m_CcdParamEx.byLocalOutputGate);
	}
	
    m_iBrightness = m_CcdParamEx.struVideoEffect.byBrightnessLevel;
    m_iExposureUserSet = m_CcdParamEx.struExposure.dwExposureUserSet;
    m_iVedioExposure = m_CcdParamEx.struExposure.dwVideoExposureSet;
    m_iContrast = m_CcdParamEx.struVideoEffect.byContrastLevel;
    m_iGain = m_CcdParamEx.struGain.byGainLevel;
	m_iUserGain = m_CcdParamEx.struGain.byGainUserSet;
    m_iSaturation = m_CcdParamEx.struVideoEffect.bySaturationLevel;
    m_iSharpness = m_CcdParamEx.struVideoEffect.bySharpnessLevel;
	m_bChkSmartIREn = m_CcdParamEx.struVideoEffect.byEnableFunc&0x01;
	m_bChkIlluminationEn = (m_CcdParamEx.struVideoEffect.byEnableFunc>>1)&0x01;
	m_bChkLightInhibitEn = (m_CcdParamEx.struVideoEffect.byEnableFunc>>2)&0x01;
	m_comboLightInhibitLevel.SetCurSel(m_CcdParamEx.struVideoEffect.byLightInhibitLevel-1);
	m_comboGrayLevel.SetCurSel(m_CcdParamEx.struVideoEffect.byGrayLevel);
	m_ctrlIrisMode.SetCurSel(m_CcdParamEx.byIrisMode);
	
	m_comboWhiteBalanceMode.SetCurSel(m_CcdParamEx.struWhiteBalance.byWhiteBalanceMode);
	m_byBGain = m_CcdParamEx.struWhiteBalance.byWhiteBalanceModeBGain;
	m_byRGain = m_CcdParamEx.struWhiteBalance.byWhiteBalanceModeRGain;

	m_comVideoFormat.SetCurSel(m_CcdParamEx.byPowerLineFrequencyMode);
	m_comboMirror.SetCurSel(m_CcdParamEx.byMirror);
	m_comboNoiseMoveMode.SetCurSel(m_CcdParamEx.struNoiseRemove.byDigitalNoiseRemoveEnable);
	m_byNormalLevel = m_CcdParamEx.struNoiseRemove.byDigitalNoiseRemoveLevel;
	m_bySpectralLevel = m_CcdParamEx.struNoiseRemove.bySpectralLevel;
	m_byTemporalLevel = m_CcdParamEx.struNoiseRemove.byTemporalLevel;
	
	m_comboDigitalZoom.SetCurSel(m_CcdParamEx.byDigitalZoom);
	m_comboDimmerMode.SetCurSel(m_CcdParamEx.byDimmerMode);
	m_comboEnhanceMode.SetCurSel(m_CcdParamEx.byEnhancedMode);
	m_comboPaletteMode.SetCurSel(m_CcdParamEx.byPaletteMode);
	m_comboFilterSwitch.SetCurSel(m_CcdParamEx.byFilterSwitch);
	m_comboFocusSpeed.SetCurSel(m_CcdParamEx.byFocusSpeed);
	m_byAutoCompInter = m_CcdParamEx.byAutoCompensationInterval;
	
	m_comboInOutMode.SetCurSel(m_CcdParamEx.bySceneMode);
	
	m_comboDayNightType.SetCurSel(m_CcdParamEx.struDayNight.byDayNightFilterType);
	m_byBeginTimeHour = m_CcdParamEx.struDayNight.byBeginTime;
	m_byBeginTimeMin = m_CcdParamEx.struDayNight.byBeginTimeMin;
	m_byBeginTimeSec = m_CcdParamEx.struDayNight.byBeginTimeSec;
	m_byEndTimeHour = m_CcdParamEx.struDayNight.byEndTime;
	m_byEndTimeMin = m_CcdParamEx.struDayNight.byEndTimeMin;
	m_byEndTimeSec = m_CcdParamEx.struDayNight.byEndTimeSec;
	m_comboAlarmTrigMode.SetCurSel(m_CcdParamEx.struDayNight.byAlarmTrigState);
	
    if (m_CcdParam.struBackLight.byBacklightMode >= 10 && m_CcdParam.struBackLight.byBacklightMode <= 12)
    {
        m_comboBackLightMode.SetCurSel(m_CcdParam.struBackLight.byBacklightMode - 3);
    }
    else
    {
        m_comboBackLightMode.SetCurSel(m_CcdParam.struBackLight.byBacklightMode);
    }

	m_dwBackLightX1 = m_CcdParamEx.struBackLight.dwPositionX1;
	m_dwBackLightX2 = m_CcdParamEx.struBackLight.dwPositionX2;
	m_dwBackLightY1 = m_CcdParamEx.struBackLight.dwPositionY1;
	m_dwBackLightY2 = m_CcdParamEx.struBackLight.dwPositionY2;
	
	//透雾
	m_comboDehazeMode.SetCurSel(m_CcdParamEx.struDefogCfg.byMode);
	m_byDehazeLevel = m_CcdParamEx.struDefogCfg.byLevel;
	
	//电子防抖
	m_comElecteSwitch.SetCurSel(m_CcdParamEx.struElectronicStabilization.byEnable);
	m_byElectLevel = m_CcdParamEx.struElectronicStabilization.byLevel;
	
	m_bChkCorridorMode = m_CcdParamEx.struCorridorMode.byEnableCorridorMode;
	
	m_comboSmartIRMode.SetCurSel(m_CcdParamEx.struSmartIRParam.byMode);
	m_byIRDistance = m_CcdParamEx.struSmartIRParam.byIRDistance;
	
	m_byShortIRDistance =m_CcdParamEx.struSmartIRParam.byShortIRDistance;
	m_byLongIRDistance = m_CcdParamEx.struSmartIRParam.byLongIRDistance;

	m_comboPIrisMode.SetCurSel(m_CcdParamEx.struPIrisParam.byMode);
	m_byPIrisAperture = m_CcdParamEx.struPIrisParam.byPIrisAperture;


    if (m_CcdParamEx.byCaptureModeN < 255) //当输入N模式小于255时
    {
        m_comboCaptureMode.SetCurSel(m_CcdParamEx.byCaptureModeN);
    }
    else //当输入模式大于255时
    {
        m_comboCaptureMode.SetCurSel(m_CcdParamEx.wCaptureModeN);
    }

    if (m_comboCaptureMode2.GetItemData(m_comboCaptureMode2.GetCurSel()) < 255) //当输入P模式小于255时
    {
        m_comboCaptureMode2.SetCurSel(m_CcdParamEx.byCaptureModeP);
    }
    else //当输入模式大于255时
    {
        m_comboCaptureMode2.SetCurSel(m_CcdParamEx.wCaptureModeP);
    }

	//激光参数
	m_cmLaserControlMode.SetCurSel(m_CcdParamEx.struLaserParam.byControlMode - 1);
	m_cmLaserTriggerMode.SetCurSel(m_CcdParamEx.struLaserParam.byTriggerMode - 1);
	m_byLaserAngle = m_CcdParamEx.struLaserParam.byAngle;
	m_byLaserBrightness = m_CcdParamEx.struLaserParam.byBrightness;
	m_byLaserSensitivity = m_CcdParamEx.struLaserParam.bySensitivity;
	m_byLaserLimitBrightness = m_CcdParamEx.struLaserParam.byLimitBrightness;//2014-01-26
	m_bChkLaserEnabled = m_CcdParamEx.struLaserParam.byEnabled;
	m_byIllumination = m_CcdParamEx.struLaserParam.byIllumination;
	m_byLightAngle = m_CcdParamEx.struLaserParam.byLightAngle;
	
    m_comFFCMode.SetCurSel(m_CcdParamEx.struFFCParam.byMode - 1);
    m_dwFFCTime = m_CcdParamEx.struFFCParam.wCompensateTime;
    
    m_comDDEMode.SetCurSel(m_CcdParamEx.struDDEParam.byMode - 1);
    m_byDDELevel = m_CcdParamEx.struDDEParam.byNormalLevel;
    m_byDDEExpertLevel = m_CcdParamEx.struDDEParam.byExpertLevel;
    
    m_comThermometryAGCType.SetCurSel(m_CcdParamEx.struThermAGC.byMode);
    m_iHighTemp = m_CcdParamEx.struThermAGC.iHighTemperature;
    m_iLowTemp = m_CcdParamEx.struThermAGC.iLowTemperature;

    m_comAGCType.SetCurSel(m_CcdParamEx.struAGCParam.bySceneType - 1);
    m_byAGCLightLevel = m_CcdParamEx.struAGCParam.byLightLevel;
    m_byAGCGainLevel = m_CcdParamEx.struAGCParam.byGainLevel;
    
	m_bLensDistortionCorrection = m_CcdParamEx.byLensDistortionCorrection;

    if (m_CcdParamEx.byDistortionCorrectionLevel==255)
    {
        m_comDistortionCorrectionLevel.SetCurSel(4);
    } 
    else
    {
        m_comDistortionCorrectionLevel.SetCurSel(m_CcdParamEx.byDistortionCorrectionLevel);
    }
    m_byCalibrationAccurateLevel = m_CcdParamEx.byCalibrationAccurateLevel;
    m_byZoomedInDistantViewLevel = m_CcdParamEx.byZoomedInDistantViewLevel;
    m_byHorizontalFOV = m_CcdParamEx.byHorizontalFOV;
    m_byVerticalFOV = m_CcdParamEx.byVerticalFOV;

	m_bOpticalDehaze = m_CcdParamEx.struOpticalDehaze.byEnable;

#ifdef DEMO_LAN_CN
    m_comboFusionMode.SetCurSel(m_CcdParamEx.byFusionMode);
#else
    //英文版本只支持0~热成像模式、3~可见光模式、 5~融合彩色模式-草地
    m_comboFusionMode.SetCurSel(m_CcdParamEx.byFusionMode / 2);
#endif

    m_byBrightnessSuddenChangeSuppression = m_CcdParamEx.byBrightnessSuddenChangeSuppression;

    m_checkGPSEnabled = m_CcdParamEx.byGPSEnabled;

	SetNewInfoToWnd();

    UpdateData(FALSE);
	
}

void CDlgCcdParam::SetNewInfoToWnd()
{
    OnSelchangeComboAGCTempMode();

	if (m_comboPIrisMode.GetCurSel() != 0)
	{
		GetDlgItem(IDC_STATIC_PIRIS_APERTURE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PIRIS_APERTURE)->EnableWindow(TRUE);
	} 
	else
	{
		GetDlgItem(IDC_STATIC_PIRIS_APERTURE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PIRIS_APERTURE)->EnableWindow(FALSE);		
	}
	
	if (m_comboSmartIRMode.GetCurSel() != 0)
	{
		GetDlgItem(IDC_STATIC_IR_DISTANCE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_IRDISTANCE)->EnableWindow(TRUE);
	} 
	else
	{
		GetDlgItem(IDC_STATIC_IR_DISTANCE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_IRDISTANCE)->EnableWindow(FALSE);		
	}
	
	if (m_ctrlIrisMode.GetCurSel() == 2  || m_ctrlIrisMode.GetCurSel() == 3 || m_ctrlIrisMode.GetCurSel() == 4 || m_ctrlIrisMode.GetCurSel() == 5)
	{
		GetDlgItem(IDC_COMBO_PIRIS_MODE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_PIRIS_APERTURE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_PIRIS_APERTURE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_PIRIS_MODE)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_COMBO_PIRIS_MODE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_PIRIS_APERTURE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_PIRIS_APERTURE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_PIRIS_MODE)->ShowWindow(SW_HIDE);
	}	
}

void CDlgCcdParam::OnBtnSetEx() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if((m_iBrightness < 0) || (m_iBrightness >100) || (m_iContrast < 0) || (m_iContrast >100) ||  \
		(m_iSaturation < 0) || (m_iSaturation >100) || (m_iSharpness < 0) || (m_iSharpness >100) ||  \
		(m_iGain < 0) || (m_iGain >100) || (m_iUserGain < 0) || (m_iUserGain > 100))
	{
		return;
	}
	//memset(&m_CcdParamEx, 0, sizeof(m_CcdParamEx));
    m_CcdParamEx.struVideoEffect.byBrightnessLevel = m_iBrightness;
    m_CcdParamEx.struVideoEffect.byContrastLevel = m_iContrast;
    m_CcdParamEx.struVideoEffect.bySaturationLevel = m_iSaturation;
    m_CcdParamEx.struVideoEffect.bySharpnessLevel = m_iSharpness;
	m_CcdParamEx.struVideoEffect.byEnableFunc = 0;
	m_CcdParamEx.struVideoEffect.byEnableFunc |= m_bChkSmartIREn;
	m_CcdParamEx.struVideoEffect.byEnableFunc |= (m_bChkIlluminationEn<<1);
	m_CcdParamEx.struVideoEffect.byEnableFunc |= (m_bChkLightInhibitEn<<2);
	m_CcdParamEx.struVideoEffect.byLightInhibitLevel = m_comboLightInhibitLevel.GetCurSel()+1;
	m_CcdParamEx.struVideoEffect.byGrayLevel = m_comboGrayLevel.GetCurSel();
	
	m_CcdParamEx.struExposure.byExposureMode = 1;
    m_CcdParamEx.struExposure.dwExposureUserSet = m_iExposureUserSet;
    m_CcdParamEx.struExposure.dwVideoExposureSet = m_iVedioExposure;
    m_CcdParamEx.struGain.byGainLevel = m_iGain;
	m_CcdParamEx.struGain.byGainUserSet = m_iUserGain;
	m_CcdParamEx.byIrisMode = m_ctrlIrisMode.GetCurSel();
    m_CcdParamEx.dwSize = sizeof(m_CcdParamEx);
	
	m_CcdParamEx.struWhiteBalance.byWhiteBalanceMode = m_comboWhiteBalanceMode.GetCurSel();
	m_CcdParamEx.struWhiteBalance.byWhiteBalanceModeBGain = m_byBGain;
	m_CcdParamEx.struWhiteBalance.byWhiteBalanceModeRGain = m_byRGain;
	
	m_CcdParamEx.byPowerLineFrequencyMode = m_comVideoFormat.GetCurSel();
	m_CcdParamEx.byMirror = m_comboMirror.GetCurSel();
	m_CcdParamEx.struNoiseRemove.byDigitalNoiseRemoveEnable = m_comboNoiseMoveMode.GetCurSel();
	m_CcdParamEx.struNoiseRemove.byDigitalNoiseRemoveLevel = m_byNormalLevel;
	m_CcdParamEx.struNoiseRemove.bySpectralLevel = m_bySpectralLevel;
	m_CcdParamEx.struNoiseRemove.byTemporalLevel = m_byTemporalLevel;
	
	m_CcdParamEx.byDigitalZoom = m_comboDigitalZoom.GetCurSel();
	m_CcdParamEx.byDimmerMode = m_comboDimmerMode.GetCurSel();
	m_CcdParamEx.byEnhancedMode = m_comboEnhanceMode.GetCurSel();
	m_CcdParamEx.byPaletteMode = m_comboPaletteMode.GetCurSel();
	m_CcdParamEx.byFilterSwitch = m_comboFilterSwitch.GetCurSel();
	m_CcdParamEx.byAutoCompensationInterval = m_byAutoCompInter;
	m_CcdParamEx.byFocusSpeed = m_comboFocusSpeed.GetCurSel();
	
	m_CcdParamEx.bySceneMode = m_comboInOutMode.GetCurSel();
	m_CcdParamEx.byLocalOutputGate = m_comboLocalOutPutGate.GetItemData(m_comboLocalOutPutGate.GetCurSel());
	m_CcdParamEx.struDayNight.byDayNightFilterType = m_comboDayNightType.GetCurSel();
	m_CcdParamEx.struDayNight.byBeginTime = m_byBeginTimeHour;
	m_CcdParamEx.struDayNight.byBeginTimeMin = m_byBeginTimeMin;
	m_CcdParamEx.struDayNight.byBeginTimeSec = m_byBeginTimeSec;
	m_CcdParamEx.struDayNight.byEndTime = m_byEndTimeHour;
	m_CcdParamEx.struDayNight.byEndTimeMin = m_byEndTimeMin;
	m_CcdParamEx.struDayNight.byEndTimeSec = m_byEndTimeSec;
	m_CcdParamEx.struDayNight.byAlarmTrigState = m_comboAlarmTrigMode.GetCurSel();
	
    m_CcdParamEx.struBackLight.byBacklightMode = m_comboBackLightMode.GetItemData(m_comboBackLightMode.GetCurSel());;
	m_CcdParamEx.struBackLight.dwPositionX1 = m_dwBackLightX1;
	m_CcdParamEx.struBackLight.dwPositionX2 = m_dwBackLightX2;
	m_CcdParamEx.struBackLight.dwPositionY1 = m_dwBackLightY1;
	m_CcdParamEx.struBackLight.dwPositionY2 = m_dwBackLightY2;
	
	m_CcdParamEx.struDefogCfg.byMode = m_comboDehazeMode.GetCurSel();
	m_CcdParamEx.struDefogCfg.byLevel = m_byDehazeLevel;
	
	m_CcdParamEx.struElectronicStabilization.byEnable = m_comElecteSwitch.GetCurSel();
	m_CcdParamEx.struElectronicStabilization.byLevel = m_byElectLevel;
	
	m_CcdParamEx.struCorridorMode.byEnableCorridorMode = m_bChkCorridorMode;
	
	m_CcdParamEx.struSmartIRParam.byMode = m_comboSmartIRMode.GetCurSel();
	m_CcdParamEx.struSmartIRParam.byIRDistance = m_byIRDistance;
	
	m_CcdParamEx.struSmartIRParam.byShortIRDistance = m_byShortIRDistance;
	m_CcdParamEx.struSmartIRParam.byLongIRDistance = m_byLongIRDistance;

	m_CcdParamEx.struPIrisParam.byMode = m_comboPIrisMode.GetCurSel();
	m_CcdParamEx.struPIrisParam.byPIrisAperture = m_byPIrisAperture;

    if (m_comboCaptureMode.GetItemData(m_comboCaptureMode.GetCurSel()) < 255) //当输入N模式小于255时
    {
        m_CcdParamEx.byCaptureModeN = m_comboCaptureMode.GetCurSel();
    }
    else //当输入模式大于255时
    {
        m_CcdParamEx.byCaptureModeN = 255;
        m_CcdParamEx.wCaptureModeN = m_comboCaptureMode.GetItemData(m_comboCaptureMode.GetCurSel());
    }

    if (m_comboCaptureMode2.GetItemData(m_comboCaptureMode2.GetCurSel()) < 255) //当输入P模式小于255时
    {
        m_CcdParamEx.byCaptureModeP = m_comboCaptureMode2.GetCurSel();
    }
    else //当输入模式大于255时
    {
        m_CcdParamEx.byCaptureModeP = 255;
        m_CcdParamEx.wCaptureModeP = m_comboCaptureMode2.GetItemData(m_comboCaptureMode2.GetCurSel());
    }

	m_CcdParamEx.struLaserParam.byControlMode = m_cmLaserControlMode.GetCurSel() + 1;
	m_CcdParamEx.struLaserParam.byTriggerMode = m_cmLaserTriggerMode.GetCurSel() + 1;
	m_CcdParamEx.struLaserParam.bySensitivity = m_byLaserSensitivity;
	m_CcdParamEx.struLaserParam.byBrightness = m_byLaserBrightness;
	m_CcdParamEx.struLaserParam.byAngle = m_byLaserAngle;
	m_CcdParamEx.struLaserParam.byLimitBrightness = m_byLaserLimitBrightness;//2014-01-26
	m_CcdParamEx.struLaserParam.byEnabled = m_bChkLaserEnabled;
	m_CcdParamEx.struLaserParam.byIllumination = m_byIllumination;
	m_CcdParamEx.struLaserParam.byLightAngle = m_byLightAngle;

    m_CcdParamEx.struFFCParam.byMode = m_comFFCMode.GetCurSel() + 1;
    m_CcdParamEx.struFFCParam.wCompensateTime = m_dwFFCTime;
    
    m_CcdParamEx.struDDEParam.byMode = m_comDDEMode.GetCurSel() + 1;
    m_CcdParamEx.struDDEParam.byNormalLevel = m_byDDELevel;
    m_CcdParamEx.struDDEParam.byExpertLevel = m_byDDEExpertLevel;
    
    m_CcdParamEx.struAGCParam.bySceneType = m_comAGCType.GetCurSel() + 1;
    m_CcdParamEx.struAGCParam.byLightLevel = m_byAGCLightLevel;
    m_CcdParamEx.struAGCParam.byGainLevel = m_byAGCGainLevel;
    
    m_CcdParamEx.byLensDistortionCorrection = m_bLensDistortionCorrection;

    if (m_comDistortionCorrectionLevel.GetCurSel()==4)
    {
        m_CcdParamEx.byDistortionCorrectionLevel =255;
    } 
    else
    {
        m_CcdParamEx.byDistortionCorrectionLevel = m_comDistortionCorrectionLevel.GetCurSel();
    }
    
    m_CcdParamEx.byCalibrationAccurateLevel = m_byCalibrationAccurateLevel;
    m_CcdParamEx.byZoomedInDistantViewLevel = m_byZoomedInDistantViewLevel;
    m_CcdParamEx.byHorizontalFOV = m_byHorizontalFOV;
    m_CcdParamEx.byVerticalFOV = m_byVerticalFOV;
    

	m_CcdParamEx.struOpticalDehaze.byEnable = m_bOpticalDehaze;

    m_CcdParamEx.struThermAGC.byMode = m_comThermometryAGCType.GetCurSel();
    m_CcdParamEx.struThermAGC.iHighTemperature = m_iHighTemp;
    m_CcdParamEx.struThermAGC.iLowTemperature = m_iLowTemp;

#ifdef DEMO_LAN_CN
    m_CcdParamEx.byFusionMode = m_comboFusionMode.GetCurSel();
#else
    //英文版本只支持0~热成像模式、3~可见光模式、 5~融合彩色模式-草地
    int index = m_comboFusionMode.GetCurSel();
    m_CcdParamEx.byFusionMode = (index == 0 ? index : index * 2 + 1);
#endif

    m_CcdParamEx.byBrightnessSuddenChangeSuppression = m_byBrightnessSuddenChangeSuppression;

    m_CcdParamEx.byGPSEnabled = m_checkGPSEnabled;

	if (m_bChkISPSet)
	{
		m_struISPCameraParamCfg.dwSize = sizeof(m_struISPCameraParamCfg);
		memcpy(&m_struISPCameraParamCfg.struSelfAdaptiveParam, &m_CcdParamEx, sizeof(m_CcdParamEx));
        if (!NET_DVR_SetDVRConfig(g_struDeviceInfo[m_iDeviceIndex].lLoginID, NET_DVR_SET_ISP_CAMERAPARAMCFG, m_lChannel, \
			&m_struISPCameraParamCfg, sizeof(m_struISPCameraParamCfg)))
		{
			g_pMainDlg->AddLog(g_struDeviceInfo[m_iDeviceIndex].lLoginID, OPERATION_FAIL_T, "Error Code = %d ;NET_DVR_SET_ISP_CAMERAPARAMCFG", NET_DVR_GetLastError());
		}
		else
		{
			g_pMainDlg->AddLog(g_struDeviceInfo[m_iDeviceIndex].lLoginID, OPERATION_SUCC_T, "Error Code = %d ;NET_DVR_SET_ISP_CAMERAPARAMCFG", NET_DVR_GetLastError());
		}
	} 
	else
	{
        if (!NET_DVR_SetDVRConfig(g_struDeviceInfo[m_iDeviceIndex].lLoginID, NET_DVR_SET_CCDPARAMCFG_EX, m_lChannel, &m_CcdParamEx, sizeof(m_CcdParamEx)))
		{
			g_pMainDlg->AddLog(g_struDeviceInfo[m_iDeviceIndex].lLoginID, OPERATION_FAIL_T, "Error Code = %d ;NET_DVR_SET_CCDPARAMCFG_EX", NET_DVR_GetLastError());
		}
		else
		{
			g_pMainDlg->AddLog(g_struDeviceInfo[m_iDeviceIndex].lLoginID, OPERATION_SUCC_T, "Error Code = %d ;NET_DVR_SET_CCDPARAMCFG_EX", NET_DVR_GetLastError());
		}
	}
	return;
}

void CDlgCcdParam::OnSelchangeComboAGCTempMode()
{
    if (m_comThermometryAGCType.GetCurSel() == 0)
    {
        m_comAGCType.EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_AGC_LIGHTLEVEL)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_AGC_GAINLEVEL)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_AGC_HIGH_TEMP)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_AGC_LOW_TEMP)->EnableWindow(FALSE);
    }
    else if (m_comThermometryAGCType.GetCurSel() == 1)
    {
        m_comAGCType.EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_AGC_LIGHTLEVEL)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_AGC_GAINLEVEL)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_AGC_HIGH_TEMP)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_AGC_LOW_TEMP)->EnableWindow(FALSE);
    }
    else if (m_comThermometryAGCType.GetCurSel() == 2)
    {
        m_comAGCType.EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_AGC_LIGHTLEVEL)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_AGC_GAINLEVEL)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_AGC_HIGH_TEMP)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_AGC_LOW_TEMP)->EnableWindow(TRUE);
    }
}

void CDlgCcdParam::OnSelchangeComboIrismode() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_ctrlIrisMode.GetCurSel() == 2 || m_ctrlIrisMode.GetCurSel() == 3 || m_ctrlIrisMode.GetCurSel() == 4 || m_ctrlIrisMode.GetCurSel() == 5)
	{
		GetDlgItem(IDC_COMBO_PIRIS_MODE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_PIRIS_APERTURE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_PIRIS_APERTURE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_PIRIS_MODE)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_COMBO_PIRIS_MODE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_PIRIS_APERTURE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_PIRIS_APERTURE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_PIRIS_MODE)->ShowWindow(SW_HIDE);
	}
	UpdateData(FALSE);
}

void CDlgCcdParam::OnSelchangeComboPirisMode() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_comboPIrisMode.GetCurSel() != 0)
	{
		GetDlgItem(IDC_STATIC_PIRIS_APERTURE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PIRIS_APERTURE)->EnableWindow(TRUE);
	} 
	else
	{
		GetDlgItem(IDC_STATIC_PIRIS_APERTURE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PIRIS_APERTURE)->EnableWindow(FALSE);		
	}
	UpdateData(FALSE);
}

void CDlgCcdParam::OnSelchangeComboSmartirMode() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_comboSmartIRMode.GetCurSel() != 0)
	{
		GetDlgItem(IDC_STATIC_IR_DISTANCE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_IRDISTANCE)->EnableWindow(TRUE);
	} 
	else
	{
		GetDlgItem(IDC_STATIC_IR_DISTANCE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_IRDISTANCE)->EnableWindow(FALSE);		
	}
	UpdateData(FALSE);
}

void CDlgCcdParam::OnBtnIspParamset() 
{
	// TODO: Add your control notification handler code here
	CDlgISPParamCfg dlg;
	dlg.m_iDeviceIndex = m_iDeviceIndex;
	dlg.m_lServerID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;
	dlg.m_pstruISPCameraParamcfg = &m_struISPCameraParamCfg;
	dlg.DoModal();
}

void CDlgCcdParam::OnBtnSignallightsync() 
{
	// TODO: Add your control notification handler code here
	CDlgSignalLightSync dlg;
	dlg.m_lUserID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;
	
	dlg.DoModal();
}


void CDlgCcdParam::OnBtnEzvizAccesscfg() 
{
	// TODO: Add your control notification handler code here
	CDlgEZVIZAccessCfg dlg;
	dlg.m_lUserID = dlg.m_lUserID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;
	dlg.m_iDevIndex = m_iDeviceIndex;
	dlg.DoModal();
}

void CDlgCcdParam::OnBtnIOoutCfg() 
{
	// TODO: Add your control notification handler code here
	CDlgIOOutCfg dlg;
	dlg.m_lUserID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;
	dlg.m_iDeviceIndex = m_iDeviceIndex;
	dlg.DoModal();
}

void CDlgCcdParam::OnBtnDpc() 
{
	// TODO: Add your control notification handler code here
    CDlgDPCCfg dlg;
    dlg.m_lUserID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;
    dlg.m_iDevIndex = m_iDeviceIndex;
    dlg.m_lChannel = m_lChannel;
	dlg.DoModal();
}

void CDlgCcdParam::OnBtnFfcManual() 
{
	// TODO: Add your control notification handler code here
    NET_DVR_FFC_MANUAL_INFO struFFCManualInfo = {0};
    struFFCManualInfo.dwSize = sizeof(NET_DVR_FFC_MANUAL_INFO);
    struFFCManualInfo.dwChannel = m_lChannel;
    // TODO: Add your control notification handler code here
    if (NET_DVR_RemoteControl(g_struDeviceInfo[m_iDeviceIndex].lLoginID, NET_DVR_FFC_MANUAL_CTRL, &struFFCManualInfo, sizeof(NET_DVR_FFC_MANUAL_INFO)))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_FFC_MANUAL_CTRL");
        return; 
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_FFC_MANUAL_CTRL");
        return; 
	} 
}

void CDlgCcdParam::OnBtnFfcBackcomp() 
{
	// TODO: Add your control notification handler code here
    NET_DVR_FFC_BACKCOMP_INFO struFFCBackCompInfo = {0};
    struFFCBackCompInfo.dwSize = sizeof(NET_DVR_FFC_BACKCOMP_INFO);
    struFFCBackCompInfo.dwChannel = m_lChannel;
    // TODO: Add your control notification handler code here
    if (NET_DVR_RemoteControl(g_struDeviceInfo[m_iDeviceIndex].lLoginID, NET_DVR_FFC_BACKCOMP_CTRL, &struFFCBackCompInfo, sizeof(NET_DVR_FFC_BACKCOMP_INFO)))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_FFC_BACKCOMP_CTRL");
        return; 
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_FFC_BACKCOMP_CTRL");
        return; 
    } 
}

void CDlgCcdParam::OnSelchangeComboFfcMode() 
{
	// TODO: Add your control notification handler code here
    if (0 == m_comFFCMode.GetCurSel())
    {
        GetDlgItem(IDC_EDIT_FFC_TIME)->EnableWindow(TRUE);
    }
    else
    {
        GetDlgItem(IDC_EDIT_FFC_TIME)->EnableWindow(FALSE);
	}
}

void CDlgCcdParam::OnSelchangeComboDdeMode() 
{
	// TODO: Add your control notification handler code here
    if (1 == m_comDDEMode.GetCurSel())
    {
        GetDlgItem(IDC_EDIT_DDE_LEVEL)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_DDE_EXPERT_LEVEL)->EnableWindow(FALSE);
    }
    else if (2 == m_comDDEMode.GetCurSel())
    {
        GetDlgItem(IDC_EDIT_DDE_LEVEL)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_DDE_EXPERT_LEVEL)->EnableWindow(TRUE);
    }
    else
    {
        GetDlgItem(IDC_EDIT_DDE_LEVEL)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_DDE_EXPERT_LEVEL)->EnableWindow(FALSE);
    }
}

void CDlgCcdParam::OnSelchangeComboAgcType() 
{
	// TODO: Add your control notification handler code here
    if (2 == m_comAGCType.GetCurSel())
    {
        GetDlgItem(IDC_EDIT_AGC_LIGHTLEVEL)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_AGC_GAINLEVEL)->EnableWindow(TRUE);
    }
    else
    {
        GetDlgItem(IDC_EDIT_AGC_LIGHTLEVEL)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_AGC_GAINLEVEL)->EnableWindow(FALSE);
	}
}

void CDlgCcdParam::OnBtnFocusingPositionState() 
{
	// TODO: Add your control notification handler code here
	DWORD dwReturn = 0;
	NET_DVR_FOCUSING_POSITION_STATE struFocusingPositionState = {0};
    struFocusingPositionState.dwSize = sizeof(struFocusingPositionState);
    if (!NET_DVR_GetDVRConfig(g_struDeviceInfo[m_iDeviceIndex].lLoginID, NET_DVR_GET_FOCUSING_POSITION_STATE, m_lChannel, \
		&struFocusingPositionState, sizeof(struFocusingPositionState), &dwReturn))
    {
		g_pMainDlg->AddLog(g_struDeviceInfo[m_iDeviceIndex].lLoginID, OPERATION_FAIL_T, "NET_DVR_GET_FOCUSING_POSITION_STATE");
    }
	else
	{
        g_pMainDlg->AddLog(g_struDeviceInfo[m_iDeviceIndex].lLoginID, OPERATION_SUCC_T, "NET_DVR_GET_FOCUSING_POSITION_STATE");
	}
	m_comboFocusingPositionState.SetCurSel(struFocusingPositionState.byState);
	UpdateData(FALSE);
}

void CDlgCcdParam::OnBtnSupplementlight() 
{
	// TODO: Add your control notification handler code here
    CDlgBuiltinSupplementLight dlg;
    dlg.m_lServerID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;
    dlg.m_iDeviceIndex = m_iDeviceIndex;
    dlg.m_lChannel = m_lChannel;
	dlg.DoModal();
}


void CDlgCcdParam::OnEnChangeEditChanindex()
{
    UpdateData(TRUE);
    m_lChannel = m_iCurChanIndex;
}

void  CDlgCcdParam::AddCaptureMode()
{
    int index = 0;
    m_comboCaptureMode.InsertString(index, "0-close");
    m_comboCaptureMode.SetItemData(index, 0);
    index++;

    m_comboCaptureMode.InsertString(index, "1-640*480@25fps");
    m_comboCaptureMode.SetItemData(index, 1);
    index++;

    m_comboCaptureMode.InsertString(index, "2-640*480@30ps");
    m_comboCaptureMode.SetItemData(index, 2);
    index++;

    m_comboCaptureMode.InsertString(index, "3-704*576@25fps");
    m_comboCaptureMode.SetItemData(index, 3);
    index++;

    m_comboCaptureMode.InsertString(index, "4-704*480@30fps");
    m_comboCaptureMode.SetItemData(index, 4);
    index++;

    m_comboCaptureMode.InsertString(index, "5-1280*720@25fps");
    m_comboCaptureMode.SetItemData(index, 5);
    index++;

    m_comboCaptureMode.InsertString(index, "6-1280*720@30fps");
    m_comboCaptureMode.SetItemData(index, 6);
    index++;

    m_comboCaptureMode.InsertString(index, "7-1280*720@50fps");
    m_comboCaptureMode.SetItemData(index, 7);
    index++;

    m_comboCaptureMode.InsertString(index, "8-1280*720@60fps");
    m_comboCaptureMode.SetItemData(index, 8);
    index++;

    m_comboCaptureMode.InsertString(index, "9-1280*960@15fps");
    m_comboCaptureMode.SetItemData(index, 9);
    index++;

    m_comboCaptureMode.InsertString(index, "10-1280*960@25fps");
    m_comboCaptureMode.SetItemData(index, 10);
    index++;

    m_comboCaptureMode.InsertString(index, "11-1280*960@30fps");
    m_comboCaptureMode.SetItemData(index, 11);
    index++;

    m_comboCaptureMode.InsertString(index, "12-1280*1024@25fps");
    m_comboCaptureMode.SetItemData(index, 12);
    index++;

    m_comboCaptureMode.InsertString(index, "13-1280*1024@30fps");
    m_comboCaptureMode.SetItemData(index, 13);
    index++;

    m_comboCaptureMode.InsertString(index, "14-1600*900@15fps");
    m_comboCaptureMode.SetItemData(index, 14);
    index++;

    m_comboCaptureMode.InsertString(index, "15-1600*1200@15fps");
    m_comboCaptureMode.SetItemData(index, 15);
    index++;

    m_comboCaptureMode.InsertString(index, "16-1920*1080@15fps");
    m_comboCaptureMode.SetItemData(index, 16);
    index++;

    m_comboCaptureMode.InsertString(index, "17-1920*1080@25fps");
    m_comboCaptureMode.SetItemData(index, 17);
    index++;

    m_comboCaptureMode.InsertString(index, "18-1920*1080@30fps");
    m_comboCaptureMode.SetItemData(index, 18);
    index++;
    m_comboCaptureMode.InsertString(index, "19-1920*1080@50fps");
    m_comboCaptureMode.SetItemData(index, 19);
    index++;

    m_comboCaptureMode.InsertString(index, "20-1920*1080@60fps");
    m_comboCaptureMode.SetItemData(index, 20);
    index++;

    m_comboCaptureMode.InsertString(index, "21-2048*1536@15fps");
    m_comboCaptureMode.SetItemData(index, 21);
    index++;

    m_comboCaptureMode.InsertString(index, "22-2048*1536@20fps");
    m_comboCaptureMode.SetItemData(index, 22);
    index++;

    m_comboCaptureMode.InsertString(index, "23-2048*1536@24fps");
    m_comboCaptureMode.SetItemData(index, 23);
    index++;

    m_comboCaptureMode.InsertString(index, "24-2048*1536@25fps");
    m_comboCaptureMode.SetItemData(index, 24);
    index++;

    m_comboCaptureMode.InsertString(index, "25-2048*1536@30fps");
    m_comboCaptureMode.SetItemData(index, 25);
    index++;

    m_comboCaptureMode.InsertString(index, "26-2560*2048@25fps");
    m_comboCaptureMode.SetItemData(index, 26);
    index++;

    m_comboCaptureMode.InsertString(index, "27-2560*2048@30fps");
    m_comboCaptureMode.SetItemData(index, 27);
    index++;

    m_comboCaptureMode.InsertString(index, "28-2560*1920@7.5fps");
    m_comboCaptureMode.SetItemData(index, 28);
    index++;
    m_comboCaptureMode.InsertString(index, "29-3072*2048@25fps");
    m_comboCaptureMode.SetItemData(index, 29);
    index++;

    m_comboCaptureMode.InsertString(index, "30-3072*2048@30fps");
    m_comboCaptureMode.SetItemData(index, 30);
    index++;

    m_comboCaptureMode.InsertString(index, "31-2048*1536@12.5");
    m_comboCaptureMode.SetItemData(index, 31);
    index++;

    m_comboCaptureMode.InsertString(index, "32-2560*1920@6.25");
    m_comboCaptureMode.SetItemData(index, 32);
    index++;

    m_comboCaptureMode.InsertString(index, "33-1600*1200@25");
    m_comboCaptureMode.SetItemData(index, 33);
    index++;

    m_comboCaptureMode.InsertString(index, "34-1600*1200@30");
    m_comboCaptureMode.SetItemData(index, 34);
    index++;

    m_comboCaptureMode.InsertString(index, "35-1600*1200@12.5");
    m_comboCaptureMode.SetItemData(index, 35);
    index++;
    m_comboCaptureMode.InsertString(index, "36-1600*900@12.5");
    m_comboCaptureMode.SetItemData(index, 36);
    index++;

    m_comboCaptureMode.InsertString(index, "37-1280*960@12.5fps");
    m_comboCaptureMode.SetItemData(index, 37);
    index++;

    m_comboCaptureMode.InsertString(index, "38-800*600@25fps");
    m_comboCaptureMode.SetItemData(index, 38);
    index++;

    m_comboCaptureMode.InsertString(index, "39-800*600@30fps40");
    m_comboCaptureMode.SetItemData(index, 39);
    index++;

    m_comboCaptureMode.InsertString(index, "40-4000*3000@12.5fps");
    m_comboCaptureMode.SetItemData(index, 40);
    index++;

    m_comboCaptureMode.InsertString(index, "41-4000*3000@15fps");
    m_comboCaptureMode.SetItemData(index, 41);
    index++;

    m_comboCaptureMode.InsertString(index, "42-4096*2160@20fps");
    m_comboCaptureMode.SetItemData(index, 42);
    index++;

    m_comboCaptureMode.InsertString(index, "43-3840*2160@20fps");
    m_comboCaptureMode.SetItemData(index, 43);
    index++;

    m_comboCaptureMode.InsertString(index, "44-960*576@25fps");
    m_comboCaptureMode.SetItemData(index, 44);
    index++;

    m_comboCaptureMode.InsertString(index, "45-960*480@30fps");
    m_comboCaptureMode.SetItemData(index, 45);
    index++;

    m_comboCaptureMode.InsertString(index, "46-752*582@25fps");
    m_comboCaptureMode.SetItemData(index, 46);
    index++;

    m_comboCaptureMode.InsertString(index, "47-768*494@30fps");
    m_comboCaptureMode.SetItemData(index, 47);
    index++;

    m_comboCaptureMode.InsertString(index, "48-2560*1440@25fps");
    m_comboCaptureMode.SetItemData(index, 48);
    index++;

    m_comboCaptureMode.InsertString(index, "49-2560*1440@30fps");
    m_comboCaptureMode.SetItemData(index, 49);
    index++;

    m_comboCaptureMode.InsertString(index, "50-720P@100fps");
    m_comboCaptureMode.SetItemData(index, 50);
    index++;

    m_comboCaptureMode.InsertString(index, "51-720P@120fps");
    m_comboCaptureMode.SetItemData(index, 51);
    index++;

    m_comboCaptureMode.InsertString(index, "52-2048*1536@50fps8");
    m_comboCaptureMode.SetItemData(index, 52);
    index++;

    m_comboCaptureMode.InsertString(index, "53-2048*1536@60fps");
    m_comboCaptureMode.SetItemData(index, 53);
    index++;

    m_comboCaptureMode.InsertString(index, "54-3840*2160@25fps");
    m_comboCaptureMode.SetItemData(index, 54);
    index++;

    m_comboCaptureMode.InsertString(index, "55-3840*2160@30fps");
    m_comboCaptureMode.SetItemData(index, 55);
    index++;

    m_comboCaptureMode.InsertString(index, "56-4096*2160@25fps");
    m_comboCaptureMode.SetItemData(index, 56);
    index++;

    m_comboCaptureMode.InsertString(index, "57-4096*2160@30fps)");
    m_comboCaptureMode.SetItemData(index, 57);
    index++;

    m_comboCaptureMode.InsertString(index, "58-1280*1024@50fps");
    m_comboCaptureMode.SetItemData(index, 58);
    index++;

    m_comboCaptureMode.InsertString(index, "59-1280*1024@60fps");
    m_comboCaptureMode.SetItemData(index, 59);
    index++;

    m_comboCaptureMode.InsertString(index, "60-3072*2048@50fps");
    m_comboCaptureMode.SetItemData(index, 60);
    index++;

    m_comboCaptureMode.InsertString(index, "61-3072*2048@60fps");
    m_comboCaptureMode.SetItemData(index, 61);
    index++;

    m_comboCaptureMode.InsertString(index, "62-3072*1728@25fps");
    m_comboCaptureMode.SetItemData(index, 62);
    index++;

    m_comboCaptureMode.InsertString(index, "63-3072*1728@30fps");
    m_comboCaptureMode.SetItemData(index, 63);
    index++;

    m_comboCaptureMode.InsertString(index, "64-3072*1728@50fps0");
    m_comboCaptureMode.SetItemData(index, 64);
    index++;

    m_comboCaptureMode.InsertString(index, "65-3072*1728@60fps");
    m_comboCaptureMode.SetItemData(index, 65);
    index++;

    m_comboCaptureMode.InsertString(index, "66-336*256@50fps");
    m_comboCaptureMode.SetItemData(index, 66);
    index++;

    m_comboCaptureMode.InsertString(index, "67-336*256@60fps");
    m_comboCaptureMode.SetItemData(index, 67);
    index++;

    m_comboCaptureMode.InsertString(index, "68-384*288@50fps");
    m_comboCaptureMode.SetItemData(index, 68);
    index++;

    m_comboCaptureMode.InsertString(index, "69-384*288@60fps");
    m_comboCaptureMode.SetItemData(index, 69);
    index++;

    m_comboCaptureMode.InsertString(index, "70-640*512@50fps");
    m_comboCaptureMode.SetItemData(index, 70);
    index++;

    m_comboCaptureMode.InsertString(index, "71-640*512@60fps");
    m_comboCaptureMode.SetItemData(index, 71);
    index++;

    m_comboCaptureMode.InsertString(index, "72-2592*1944@25fps");
    m_comboCaptureMode.SetItemData(index, 72);
    index++;

    m_comboCaptureMode.InsertString(index, "73-2592*1944@30fps");
    m_comboCaptureMode.SetItemData(index, 73);
    index++;

    m_comboCaptureMode.InsertString(index, "74-2688*1536@25fps");
    m_comboCaptureMode.SetItemData(index, 74);
    index++;

    m_comboCaptureMode.InsertString(index, "75-2688*1536@30fps");
    m_comboCaptureMode.SetItemData(index, 75);
    index++;

    m_comboCaptureMode.InsertString(index, "76-2592*1944@20fps");
    m_comboCaptureMode.SetItemData(index, 76);
    index++;

    m_comboCaptureMode.InsertString(index, "77-2592*1944@15fps");
    m_comboCaptureMode.SetItemData(index, 77);
    index++;

    m_comboCaptureMode.InsertString(index, "78-2688*1520@20fps");
    m_comboCaptureMode.SetItemData(index, 78);
    index++;

    m_comboCaptureMode.InsertString(index, "79-2688*1520@15fps");
    m_comboCaptureMode.SetItemData(index, 79);
    index++;

    m_comboCaptureMode.InsertString(index, "80-2688*1520@25fps");
    m_comboCaptureMode.SetItemData(index, 80);
    index++;

    m_comboCaptureMode.InsertString(index, "81-2688*1520@30fps");
    m_comboCaptureMode.SetItemData(index, 81);
    index++;

    m_comboCaptureMode.InsertString(index, "82-2720*2048@25fps");
    m_comboCaptureMode.SetItemData(index, 82);
    index++;

    m_comboCaptureMode.InsertString(index, "83-2720*2048@30fps");
    m_comboCaptureMode.SetItemData(index, 83);
    index++;

    m_comboCaptureMode.InsertString(index, "84-336*256@25fps");
    m_comboCaptureMode.SetItemData(index, 84);
    index++;

    m_comboCaptureMode.InsertString(index, "85-384*288@25fps");
    m_comboCaptureMode.SetItemData(index, 85);
    index++;

    m_comboCaptureMode.InsertString(index, "86-640*512@25fps");
    m_comboCaptureMode.SetItemData(index, 86);
    index++;

    m_comboCaptureMode.InsertString(index, "87-1280*960@50fps");
    m_comboCaptureMode.SetItemData(index, 87);
    index++;

    m_comboCaptureMode.InsertString(index, "88-1280*960@60fps");
    m_comboCaptureMode.SetItemData(index, 88);
    index++;

    m_comboCaptureMode.InsertString(index, "89-1280*960@100fps");
    m_comboCaptureMode.SetItemData(index, 89);
    index++;

    m_comboCaptureMode.InsertString(index, "90-1280*960@120fps");
    m_comboCaptureMode.SetItemData(index, 90);
    index++;

    m_comboCaptureMode.InsertString(index, "91-4000*3000@20fps");
    m_comboCaptureMode.SetItemData(index, 91);
    index++;

    m_comboCaptureMode.InsertString(index, "92-1920*1200@25fps");
    m_comboCaptureMode.SetItemData(index, 92);
    index++;

    m_comboCaptureMode.InsertString(index, "93-1920*1200@30fps");
    m_comboCaptureMode.SetItemData(index, 93);
    index++;

    m_comboCaptureMode.InsertString(index, "94-2560*1920@25fps");
    m_comboCaptureMode.SetItemData(index, 94);
    index++;

    m_comboCaptureMode.InsertString(index, "95-2560*1920@20fps");
    m_comboCaptureMode.SetItemData(index, 95);
    index++;

    m_comboCaptureMode.InsertString(index, "96-2560*1920@30fps");
    m_comboCaptureMode.SetItemData(index, 96);
    index++;

    m_comboCaptureMode.InsertString(index, "97-1280*1920@25fps");
    m_comboCaptureMode.SetItemData(index, 97);
    index++;

    m_comboCaptureMode.InsertString(index, "98-1280*1920@30fps");
    m_comboCaptureMode.SetItemData(index, 98);
    index++;

    m_comboCaptureMode.InsertString(index, "99-4000*3000@24fps");
    m_comboCaptureMode.SetItemData(index, 99);
    index++;

    m_comboCaptureMode.InsertString(index, "100-4000*3000@25fps");
    m_comboCaptureMode.SetItemData(index, 100);
    index++;

    m_comboCaptureMode.InsertString(index, "101-4000*3000@10fps");
    m_comboCaptureMode.SetItemData(index, 101);
    index++;

    m_comboCaptureMode.InsertString(index, "102-384*288@30fps");
    m_comboCaptureMode.SetItemData(index, 102);
    index++;

    m_comboCaptureMode.InsertString(index, "103-2560*1920@15fps");
    m_comboCaptureMode.SetItemData(index, 103);
    index++;

    m_comboCaptureMode.InsertString(index, "104-2400*3840@25fps");
    m_comboCaptureMode.SetItemData(index, 104);
    index++;

    m_comboCaptureMode.InsertString(index, "105-1200*1920@25fps8");
    m_comboCaptureMode.SetItemData(index, 105);
    index++;

    m_comboCaptureMode.InsertString(index, "106-4096*1800@30fps");
    m_comboCaptureMode.SetItemData(index, 106);
    index++;

    m_comboCaptureMode.InsertString(index, "107-3840*1680@30fps");
    m_comboCaptureMode.SetItemData(index, 107);
    index++;

    m_comboCaptureMode.InsertString(index, "108-2560*1120@30fps");
    m_comboCaptureMode.SetItemData(index, 108);
    index++;

    m_comboCaptureMode.InsertString(index, "109-704*320@30fps");
    m_comboCaptureMode.SetItemData(index, 109);
    index++;

    m_comboCaptureMode.InsertString(index, "110-1280*560@30fps");
    m_comboCaptureMode.SetItemData(index, 110);
    index++;

    m_comboCaptureMode.InsertString(index, "111-4096*1800@25fps");
    m_comboCaptureMode.SetItemData(index, 111);
    index++;

    m_comboCaptureMode.InsertString(index, "112-3840*1680@25fps");
    m_comboCaptureMode.SetItemData(index, 112);
    index++;

    m_comboCaptureMode.InsertString(index, "113-2560*1120@25fps");
    m_comboCaptureMode.SetItemData(index, 113);
    index++;

    m_comboCaptureMode.InsertString(index, "114-704*320@25fps");
    m_comboCaptureMode.SetItemData(index, 114);
    index++;

    m_comboCaptureMode.InsertString(index, "115-1280*560@25fps");
    m_comboCaptureMode.SetItemData(index, 115);
    index++;

    m_comboCaptureMode.InsertString(index, "116-2400*3840@24fps");
    m_comboCaptureMode.SetItemData(index, 116);
    index++;
    m_comboCaptureMode.InsertString(index, "117-3840*2400@24fps");
    m_comboCaptureMode.SetItemData(index, 117);
    index++;

    m_comboCaptureMode.InsertString(index, "118-3840*2400@25fps");
    m_comboCaptureMode.SetItemData(index, 118);
    index++;

    m_comboCaptureMode.InsertString(index, "119-2560*1920@12.5fps");
    m_comboCaptureMode.SetItemData(index, 119);
    index++;

    m_comboCaptureMode.InsertString(index, "120-2560*2048@12fps");
    m_comboCaptureMode.SetItemData(index, 120);
    index++;
    m_comboCaptureMode.InsertString(index, "121-2560*2048@15fps");
    m_comboCaptureMode.SetItemData(index, 121);
    index++;

    m_comboCaptureMode.InsertString(index, "122-2560*1536@25fps");
    m_comboCaptureMode.SetItemData(index, 122);
    index++;

    m_comboCaptureMode.InsertString(index, "123-2560*1536@30fps");
    m_comboCaptureMode.SetItemData(index, 123);
    index++;

    m_comboCaptureMode.InsertString(index, "124-2256*2048@25fps");
    m_comboCaptureMode.SetItemData(index, 124);
    index++;

    m_comboCaptureMode.InsertString(index, "125-2256*2048@30fps");
    m_comboCaptureMode.SetItemData(index, 125);
    index++;

    m_comboCaptureMode.InsertString(index, "126-2592*2592@12.5fps");
    m_comboCaptureMode.SetItemData(index, 126);
    index++;

    m_comboCaptureMode.InsertString(index, "127-2592*2592@15fps");
    m_comboCaptureMode.SetItemData(index, 127);
    index++;

    m_comboCaptureMode.InsertString(index, "128-640*512@30fps");
    m_comboCaptureMode.SetItemData(index, 128);
    index++;

    m_comboCaptureMode.InsertString(index, "129-2048*1520@30fps");
    m_comboCaptureMode.SetItemData(index, 129);
    index++;

    m_comboCaptureMode.InsertString(index, "130-2048*1520@25fps");
    m_comboCaptureMode.SetItemData(index, 130);
    index++;

    m_comboCaptureMode.InsertString(index, "131-3840*2160@24fps");
    m_comboCaptureMode.SetItemData(index, 131);
    index++;

    m_comboCaptureMode.InsertString(index, "132-2592*1520@25fps");
    m_comboCaptureMode.SetItemData(index, 132);
    index++;

    m_comboCaptureMode.InsertString(index, "133-2592*1520@30fps");
    m_comboCaptureMode.SetItemData(index, 133);
    index++;

    m_comboCaptureMode.InsertString(index, "134-2592*1536@25fps");
    m_comboCaptureMode.SetItemData(index, 134);
    index++;

    m_comboCaptureMode.InsertString(index, "135-2592*1536@30fps");
    m_comboCaptureMode.SetItemData(index, 135);
    index++;

    m_comboCaptureMode.InsertString(index, "136-640*960@25fps");
    m_comboCaptureMode.SetItemData(index, 136);
    index++;

    m_comboCaptureMode.InsertString(index, "137-640*960@24fps");
    m_comboCaptureMode.SetItemData(index, 137);
    index++;

    /*m_comboCaptureMode.InsertString(index, "138-3840*1080");
    m_comboCaptureMode.SetItemData(index, 138);
    index++;*/

    m_comboCaptureMode.InsertString(index, "139-3840*1080@25fps");
    m_comboCaptureMode.SetItemData(index, 139);
    index++;

    m_comboCaptureMode.InsertString(index, "140-3840*1080@30fps");
    m_comboCaptureMode.SetItemData(index, 140);
    index++;

    /*m_comboCaptureMode.InsertString(index, "141-704*200");
    m_comboCaptureMode.SetItemData(index, 141);
    index++;*/

    m_comboCaptureMode.InsertString(index, "142-2992*2192@25fps");
    m_comboCaptureMode.SetItemData(index, 142);
    index++;

    m_comboCaptureMode.InsertString(index, "143-2992*2192@30fps");
    m_comboCaptureMode.SetItemData(index, 143);
    index++;

    m_comboCaptureMode.InsertString(index, "144-3008*2160@25fps");
    m_comboCaptureMode.SetItemData(index, 144);
    index++;

    m_comboCaptureMode.InsertString(index, "145-3008*2160@30fps");
    m_comboCaptureMode.SetItemData(index, 145);
    index++;

    m_comboCaptureMode.InsertString(index, "146-3072*1728@20fps");
    m_comboCaptureMode.SetItemData(index, 146);
    index++;

    m_comboCaptureMode.InsertString(index, "147-2560*1440@20fps");
    m_comboCaptureMode.SetItemData(index, 147);
    index++;

    m_comboCaptureMode.InsertString(index, "148-2160*3840@25fps");
    m_comboCaptureMode.SetItemData(index, 148);
    index++;

    m_comboCaptureMode.InsertString(index, "149-2160*3840@30fps");
    m_comboCaptureMode.SetItemData(index, 149);
    index++;

    m_comboCaptureMode.InsertString(index, "150-7008*1080@25fps");
    m_comboCaptureMode.SetItemData(index, 150);
    index++;

    m_comboCaptureMode.InsertString(index, "151-7008*1080@30fps");
    m_comboCaptureMode.SetItemData(index, 151);
    index++;

    m_comboCaptureMode.InsertString(index, "152-3072*2048@20fps");
    m_comboCaptureMode.SetItemData(index, 152);
    index++;

    m_comboCaptureMode.InsertString(index, "153-1536*864@25fps");
    m_comboCaptureMode.SetItemData(index, 153);
    index++;

    m_comboCaptureMode.InsertString(index, "154-2560*1920@24fps");
    m_comboCaptureMode.SetItemData(index, 154);
    index++;

    m_comboCaptureMode.InsertString(index, "155-2400*3840@30fps");
    m_comboCaptureMode.SetItemData(index, 155);
    index++;

    m_comboCaptureMode.InsertString(index, "156-3840*2400@30fps");
    m_comboCaptureMode.SetItemData(index, 156);
    index++;

    m_comboCaptureMode.InsertString(index, "157-3840*2160@15fps");
    m_comboCaptureMode.SetItemData(index, 157);
    index++;

    m_comboCaptureMode.InsertString(index, "158-384*288@8.3fps");
    m_comboCaptureMode.SetItemData(index, 158);
    index++;

    m_comboCaptureMode.InsertString(index, "159-640*512@8.3fps");
    m_comboCaptureMode.SetItemData(index, 159);
    index++;

    m_comboCaptureMode.InsertString(index, "160-160*120@8.3fps");
    m_comboCaptureMode.SetItemData(index, 160);
    index++;

    m_comboCaptureMode.InsertString(index, "161-1024*768@8.3fps");
    m_comboCaptureMode.SetItemData(index, 161);
    index++;

    m_comboCaptureMode.InsertString(index, "162-640*480@8.3fps");
    m_comboCaptureMode.SetItemData(index, 162);
    index++;

    m_comboCaptureMode.InsertString(index, "163-3840*2160@12.5fps");
    m_comboCaptureMode.SetItemData(index, 163);
    index++;

    m_comboCaptureMode.InsertString(index, "164-2304*1296@30fps");
    m_comboCaptureMode.SetItemData(index, 164);
    index++;

    m_comboCaptureMode.InsertString(index, "165-2304*1296@25fps");
    m_comboCaptureMode.SetItemData(index, 165);
    index++;

    m_comboCaptureMode.InsertString(index, "166-2560*1440@24fps");
    m_comboCaptureMode.SetItemData(index, 166);
    index++;

    m_comboCaptureMode.InsertString(index, "167-2688*1512@25fps");
    m_comboCaptureMode.SetItemData(index, 167);
    index++;

    m_comboCaptureMode.InsertString(index, "168-2688*1512@30fps");
    m_comboCaptureMode.SetItemData(index, 168);
    index++;

    m_comboCaptureMode.InsertString(index, "169-2688*1512@50fps");
    m_comboCaptureMode.SetItemData(index, 169);
    index++;

    m_comboCaptureMode.InsertString(index, "170-2688*1512@60fps");
    m_comboCaptureMode.SetItemData(index, 170);
    index++;

    m_comboCaptureMode.InsertString(index, "171-1536*864@30fps");
    m_comboCaptureMode.SetItemData(index, 171);
    index++;

    m_comboCaptureMode.InsertString(index, "172-2560*1440@50fps");
    m_comboCaptureMode.SetItemData(index, 172);
    index++;

    m_comboCaptureMode.InsertString(index, "173-2560*1440@60fps");
    m_comboCaptureMode.SetItemData(index, 173);
    index++;

    m_comboCaptureMode.InsertString(index, "174-2048*2048@25fps");
    m_comboCaptureMode.SetItemData(index, 174);
    index++;

    m_comboCaptureMode.InsertString(index, "175-2048*2048@30fps");
    m_comboCaptureMode.SetItemData(index, 175);
    index++;

    m_comboCaptureMode.InsertString(index, "176-4000*3060@20fps");
    m_comboCaptureMode.SetItemData(index, 176);
    index++;

    m_comboCaptureMode.InsertString(index, "177-3060*3060@25fps");
    m_comboCaptureMode.SetItemData(index, 177);
    index++;

    m_comboCaptureMode.InsertString(index, "178-3060*3060@30fps");
    m_comboCaptureMode.SetItemData(index, 178);
    index++;

    m_comboCaptureMode.InsertString(index, "179-3000*3000@25fps");
    m_comboCaptureMode.SetItemData(index, 179);
    index++;

    m_comboCaptureMode.InsertString(index, "180-3000*3000@30fps");
    m_comboCaptureMode.SetItemData(index, 180);
    index++;

    m_comboCaptureMode.InsertString(index, "181-8160*3616@30fps");
    m_comboCaptureMode.SetItemData(index, 181);
    index++;

    m_comboCaptureMode.InsertString(index, "182-8160*3616@25fps");
    m_comboCaptureMode.SetItemData(index, 182);
    index++;

    m_comboCaptureMode.InsertString(index, "183-3000*3000@20fps");
    m_comboCaptureMode.SetItemData(index, 183);
    index++;

    m_comboCaptureMode.InsertString(index, "184-3000*3000@15fps");
    m_comboCaptureMode.SetItemData(index, 184);
    index++;

    m_comboCaptureMode.InsertString(index, "185-3000*3000@12.5fps2");
    m_comboCaptureMode.SetItemData(index, 185);
    index++;

    m_comboCaptureMode.InsertString(index, "186-5472*3648@25fps");
    m_comboCaptureMode.SetItemData(index, 186);
    index++;

    m_comboCaptureMode.InsertString(index, "187-5472*3648@30fps");
    m_comboCaptureMode.SetItemData(index, 187);
    index++;

    m_comboCaptureMode.InsertString(index, "188-7680*4320@25fps");
    m_comboCaptureMode.SetItemData(index, 188);
    index++;

    m_comboCaptureMode.InsertString(index, "189-7680*4320@30fps");
    m_comboCaptureMode.SetItemData(index, 189);
    index++;

    m_comboCaptureMode.InsertString(index, "190-8160*2400@25fps");
    m_comboCaptureMode.SetItemData(index, 190);
    index++;

    m_comboCaptureMode.InsertString(index, "191-8160*2400@30fps");
    m_comboCaptureMode.SetItemData(index, 191);
    index++;

    m_comboCaptureMode.InsertString(index, "192-5520*2400@25fps");
    m_comboCaptureMode.SetItemData(index, 192);
    index++;

    m_comboCaptureMode.InsertString(index, "93-5520*2400@30fps");
    m_comboCaptureMode.SetItemData(index, 193);
    index++;

    m_comboCaptureMode.InsertString(index, "194-2560*1440@15fps");
    m_comboCaptureMode.SetItemData(index, 194);
    index++;

    m_comboCaptureMode.InsertString(index, "195-1944*1212@24fps");
    m_comboCaptureMode.SetItemData(index, 195);
    index++;

    m_comboCaptureMode.InsertString(index, "196-1944*1212@25fps");
    m_comboCaptureMode.SetItemData(index, 196);
    index++;

    m_comboCaptureMode.InsertString(index, "197-3456*1920@30fps");
    m_comboCaptureMode.SetItemData(index, 197);
    index++;

    m_comboCaptureMode.InsertString(index, "198-4800*2688@25fps");
    m_comboCaptureMode.SetItemData(index, 198);
    index++;

    m_comboCaptureMode.InsertString(index, "199-4800*2688@30fps");
    m_comboCaptureMode.SetItemData(index, 199);
    index++;

    m_comboCaptureMode.InsertString(index, "200-6480*1080@25fps");
    m_comboCaptureMode.SetItemData(index, 200);
    index++;

    m_comboCaptureMode.InsertString(index, "201-6480*1080@30fps");
    m_comboCaptureMode.SetItemData(index, 201);
    index++;

    m_comboCaptureMode.InsertString(index, "203-8640*1440@25fps");
    m_comboCaptureMode.SetItemData(index, 202);
    index++;

    m_comboCaptureMode.InsertString(index, "8640*1440@30fps");
    m_comboCaptureMode.SetItemData(index, 203);
    index++;

    m_comboCaptureMode.InsertString(index, "204-3456*1920@25fps");
    m_comboCaptureMode.SetItemData(index, 204);
    index++;

    m_comboCaptureMode.InsertString(index, "205-2688*1520@50fps");
    m_comboCaptureMode.SetItemData(index, 205);
    index++;

    m_comboCaptureMode.InsertString(index, "206-2688*1520@60fps");
    m_comboCaptureMode.SetItemData(index, 206);
    index++;

    m_comboCaptureMode.InsertString(index, "207-4976*1452@25fps");
    m_comboCaptureMode.SetItemData(index, 207);
    index++;

    m_comboCaptureMode.InsertString(index, "208-4976*1452@30fps");
    m_comboCaptureMode.SetItemData(index, 208);
    index++;

    m_comboCaptureMode.InsertString(index, "209-3200*1800@25fps");
    m_comboCaptureMode.SetItemData(index, 209);
    index++;

    m_comboCaptureMode.InsertString(index, "210-3200*1800@30fps");
    m_comboCaptureMode.SetItemData(index, 210);
    index++;

    m_comboCaptureMode.InsertString(index, "211-5472*3648@24fps");
    m_comboCaptureMode.SetItemData(index, 211);
    index++;

    m_comboCaptureMode.InsertString(index, "212-1920*1080@12.5fps");
    m_comboCaptureMode.SetItemData(index, 212);
    index++;

    m_comboCaptureMode.InsertString(index, "213-2944*1656@20fps");
    m_comboCaptureMode.SetItemData(index, 213);
    index++;

    m_comboCaptureMode.InsertString(index, "214-1920*1080@24fps");
    m_comboCaptureMode.SetItemData(index, 214);
    index++;

    m_comboCaptureMode.InsertString(index, "215-4800*1600@25fps");
    m_comboCaptureMode.SetItemData(index, 215);
    index++;

    m_comboCaptureMode.InsertString(index, "216-4800*1600@30fps");
    m_comboCaptureMode.SetItemData(index, 216);
    index++;

    m_comboCaptureMode.InsertString(index, "217-2560*1440@12.5fps");
    m_comboCaptureMode.SetItemData(index, 217);
    index++;

    m_comboCaptureMode.InsertString(index, "218-6560*3690@1fps");
    m_comboCaptureMode.SetItemData(index, 218);
    index++;

    m_comboCaptureMode.InsertString(index, "219-5120*1400@20fps2");
    m_comboCaptureMode.SetItemData(index, 219);
    index++;

    m_comboCaptureMode.InsertString(index, "220-7680*4320@1fps");
    m_comboCaptureMode.SetItemData(index, 220);
    index++;

    m_comboCaptureMode.InsertString(index, "221-1920*1080@20fps");
    m_comboCaptureMode.SetItemData(index, 221);
    index++;

    m_comboCaptureMode.InsertString(index, "222-5120*1440@20fps");
    m_comboCaptureMode.SetItemData(index, 222);
    index++;

    m_comboCaptureMode.InsertString(index, "223-4080*1808@25fps");
    m_comboCaptureMode.SetItemData(index, 223);
    index++;

    m_comboCaptureMode.InsertString(index, "224-4080*1808@30fps");
    m_comboCaptureMode.SetItemData(index, 224);
    index++;

    m_comboCaptureMode.InsertString(index, "225-4080*1152@25fps");
    m_comboCaptureMode.SetItemData(index, 225);
    index++;

    m_comboCaptureMode.InsertString(index, "226-4080*1152@30fps");
    m_comboCaptureMode.SetItemData(index, 226);
    index++;

    m_comboCaptureMode.InsertString(index, "227-2688*1944@20fps");
    m_comboCaptureMode.SetItemData(index, 227);
    index++;

    m_comboCaptureMode.InsertString(index, "228-2592*1944@24fps");
    m_comboCaptureMode.SetItemData(index, 228);
    index++;

    m_comboCaptureMode.InsertString(index, "229-3200*1800@15fps");
    m_comboCaptureMode.SetItemData(index, 229);
    index++;

    m_comboCaptureMode.InsertString(index, "230-4416*1696@20fps");
    m_comboCaptureMode.SetItemData(index, 230);
    index++;

    m_comboCaptureMode.InsertString(index, "231-3456*1080@25fps");
    m_comboCaptureMode.SetItemData(index, 231);
    index++;

    m_comboCaptureMode.InsertString(index, "232-3200*1800@12.5fps");
    m_comboCaptureMode.SetItemData(index, 232);
    index++;

    m_comboCaptureMode.InsertString(index, "233-2688*1532@25fps");
    m_comboCaptureMode.SetItemData(index, 233);
    index++;

    m_comboCaptureMode.InsertString(index, "234-2688*1532@30fps");
    m_comboCaptureMode.SetItemData(index, 234);
    index++;

    m_comboCaptureMode.InsertString(index, "235-4416*1696@12.5fps");
    m_comboCaptureMode.SetItemData(index, 235);
    index++;

    m_comboCaptureMode.InsertString(index, "236-3840*2048P12.5fps");
    m_comboCaptureMode.SetItemData(index, 236);
    index++;

    m_comboCaptureMode.InsertString(index, "237-3840*4096P12.5fps");
    m_comboCaptureMode.SetItemData(index, 237);
    index++;

    m_comboCaptureMode.InsertString(index, "238-5120*1440@12.5fps");
    m_comboCaptureMode.SetItemData(index, 238);
    index++;

    m_comboCaptureMode.InsertString(index, "239-3840*1080@24fps");
    m_comboCaptureMode.SetItemData(index, 239);
    index++;

    m_comboCaptureMode.InsertString(index, "240-320*256@30fps");
    m_comboCaptureMode.SetItemData(index, 240);
    index++;

    m_comboCaptureMode.InsertString(index, "241-3264*2448@25fps");
    m_comboCaptureMode.SetItemData(index, 241);
    index++;

    m_comboCaptureMode.InsertString(index, "242-3264*2448@30fps");
    m_comboCaptureMode.SetItemData(index, 242);
    index++;

    m_comboCaptureMode.InsertString(index, "243-5430*3054@1fps");
    m_comboCaptureMode.SetItemData(index, 243);
    index++;

    m_comboCaptureMode.InsertString(index, "244-2688*1520@24@24fps");
    m_comboCaptureMode.SetItemData(index, 244);
    index++;

    m_comboCaptureMode.InsertString(index, "245-4000*3000@30fps");
    m_comboCaptureMode.SetItemData(index, 245);
    index++;

    m_comboCaptureMode.InsertString(index, "246-1632*1224@25fps");
    m_comboCaptureMode.SetItemData(index, 246);
    index++;

    m_comboCaptureMode.InsertString(index, "247-1632*1224@30fps");
    m_comboCaptureMode.SetItemData(index, 247);
    index++;

    m_comboCaptureMode.InsertString(index, "248-160*120@25fps");
    m_comboCaptureMode.SetItemData(index, 248);
    index++;

    m_comboCaptureMode.InsertString(index, "249-1920*1440@25fps");
    m_comboCaptureMode.SetItemData(index, 249);
    index++;

    m_comboCaptureMode.InsertString(index, "250-1920*1440@30fps");
    m_comboCaptureMode.SetItemData(index, 250);
    index++;

    m_comboCaptureMode.InsertString(index, "251-3632*1632@20fps");
    m_comboCaptureMode.SetItemData(index, 251);
    index++;

    m_comboCaptureMode.InsertString(index, "252-3040*1368@25fps");
    m_comboCaptureMode.SetItemData(index, 252);
    index++;

    m_comboCaptureMode.InsertString(index, "253-3040*1368@24fps");
    m_comboCaptureMode.SetItemData(index, 253);
    index++;

    m_comboCaptureMode.InsertString(index, "254-5120*1440@25fps");
    m_comboCaptureMode.SetItemData(index, 254);
    index++;


    m_comboCaptureMode.InsertString(index, "256-160*120@50fps");
    m_comboCaptureMode.SetItemData(index, 256);
    index++;

    m_comboCaptureMode.InsertString(index, "257-3200*1800@20fps");
    m_comboCaptureMode.SetItemData(index, 257);
    index++;

    m_comboCaptureMode.InsertString(index, "258-800*480@25fps");
    m_comboCaptureMode.SetItemData(index, 258);
    index++;

    m_comboCaptureMode.InsertString(index, "259-2688*1944@25fps");
    m_comboCaptureMode.SetItemData(index, 259);
    index++;

    m_comboCaptureMode.InsertString(index, "260-640*384@50fps");
    m_comboCaptureMode.SetItemData(index, 260);
    index++;

    m_comboCaptureMode.InsertString(index, "261-8000*6000@1fps");
    m_comboCaptureMode.SetItemData(index, 261);
    index++;

    m_comboCaptureMode.InsertString(index, "262-1440*1080@50fps");
    m_comboCaptureMode.SetItemData(index, 262);
    index++;

    m_comboCaptureMode.InsertString(index, "263-1440*1080@60fps");
    m_comboCaptureMode.SetItemData(index, 263);
    index++;

    m_comboCaptureMode.InsertString(index, "264-8160X3616@24fps");
    m_comboCaptureMode.SetItemData(index, 264);
    index++;

    m_comboCaptureMode.InsertString(index, "265-3632*1632@25fps");
    m_comboCaptureMode.SetItemData(index, 265);
    index++;

    m_comboCaptureMode.InsertString(index, "266-3632*1632@30fps");
    m_comboCaptureMode.SetItemData(index, 266);
    index++;

    m_comboCaptureMode.InsertString(index, "267-3632*1632@20fps");
    m_comboCaptureMode.SetItemData(index, 267);
    index++;

    m_comboCaptureMode.InsertString(index, "268-1760*1320@25fps");
    m_comboCaptureMode.SetItemData(index, 268);
    index++;

    m_comboCaptureMode.InsertString(index, "269-4000*3000@4fps");
    m_comboCaptureMode.SetItemData(index, 269);
    index++;

    m_comboCaptureMode.InsertString(index, "270-192*256@25fps");
    m_comboCaptureMode.SetItemData(index, 270);
    index++;

    m_comboCaptureMode.InsertString(index, "271-720*576@25fps");
    m_comboCaptureMode.SetItemData(index, 271);
    index++;

    m_comboCaptureMode.InsertString(index, "272-720x576@30fps");
    m_comboCaptureMode.SetItemData(index, 272);
    index++;

    m_comboCaptureMode.InsertString(index, "277-1760*1320@12.5fps");
    m_comboCaptureMode.SetItemData(index, 277);
    index++;

    m_comboCaptureMode.InsertString(index, "278-2560*480@25fps");
    m_comboCaptureMode.SetItemData(index, 278);
    index++;

    m_comboCaptureMode.InsertString(index, "279-2048*384@25fps");
    m_comboCaptureMode.SetItemData(index, 279);
    index++;

    m_comboCaptureMode.InsertString(index, "280-96*96@25fps");
    m_comboCaptureMode.SetItemData(index, 280);
    index++;

    m_comboCaptureMode.InsertString(index, "281-320*256@25fps");
    m_comboCaptureMode.SetItemData(index, 281);
    index++;

    m_comboCaptureMode.InsertString(index, "282-6128*1800@25fps");
    m_comboCaptureMode.SetItemData(index, 282);
    index++;

    m_comboCaptureMode.InsertString(index, "283-6128*1800@30fps");
    m_comboCaptureMode.SetItemData(index, 283);
    index++;

    m_comboCaptureMode.InsertString(index, "284-2304*1296@24fps");
    m_comboCaptureMode.SetItemData(index, 284);
    index++;

    m_comboCaptureMode.InsertString(index, "285-2048*1152@25fps");
    m_comboCaptureMode.SetItemData(index, 285);
    index++;

    m_comboCaptureMode.InsertString(index, "286-2048*1152@30fps");
    m_comboCaptureMode.SetItemData(index, 286);
    index++;

    m_comboCaptureMode.InsertString(index, "287-3840*2100@20fps");
    m_comboCaptureMode.SetItemData(index, 287);
    index++;

    m_comboCaptureMode.InsertString(index, "288-96*72@25fps");
    m_comboCaptureMode.SetItemData(index, 288);
    index++;

    m_comboCaptureMode.InsertString(index, "289-2048*1152@24fps");
    m_comboCaptureMode.SetItemData(index, 289);
    index++;

    m_comboCaptureMode.InsertString(index, "290-720*576@50fps");
    m_comboCaptureMode.SetItemData(index, 290);
    index++;

    m_comboCaptureMode.InsertString(index, "291-2368*1776@25fps");
    m_comboCaptureMode.SetItemData(index, 291);
    index++;

    m_comboCaptureMode.InsertString(index, "292-2368*1776@30fps");
    m_comboCaptureMode.SetItemData(index, 292);
    index++;

    m_comboCaptureMode.InsertString(index, "293-1776*1776@25fps");
    m_comboCaptureMode.SetItemData(index, 293);
    index++;

    m_comboCaptureMode.InsertString(index, "294-3776*2832@25fps");
    m_comboCaptureMode.SetItemData(index, 294);
    index++;

    m_comboCaptureMode.InsertString(index, "295-3776*2832@30fps");
    m_comboCaptureMode.SetItemData(index, 295);
    index++;

    m_comboCaptureMode.InsertString(index, "296-2832*2832@25ps");
    m_comboCaptureMode.SetItemData(index, 296);
    index++;

    m_comboCaptureMode.InsertString(index, "297-2832*2832@30");
    m_comboCaptureMode.SetItemData(index, 297);
    index++;

    m_comboCaptureMode.InsertString(index, "298-1776*1776@30fps");
    m_comboCaptureMode.SetItemData(index, 298);
    index++;
}

void  CDlgCcdParam::AddCaptureModeP()
{
    int index = 0;
    m_comboCaptureMode2.InsertString(index, "0-close");
    m_comboCaptureMode2.SetItemData(index, 0);
    index++;

    m_comboCaptureMode2.InsertString(index, "1-640*480@25fps");
    m_comboCaptureMode2.SetItemData(index, 1);
    index++;

    m_comboCaptureMode2.InsertString(index, "2-640*480@30ps");
    m_comboCaptureMode2.SetItemData(index, 2);
    index++;

    m_comboCaptureMode2.InsertString(index, "3-704*576@25fps");
    m_comboCaptureMode2.SetItemData(index, 3);
    index++;

    m_comboCaptureMode2.InsertString(index, "4-704*480@30fps");
    m_comboCaptureMode2.SetItemData(index, 4);
    index++;

    m_comboCaptureMode2.InsertString(index, "5-1280*720@25fps");
    m_comboCaptureMode2.SetItemData(index, 5);
    index++;

    m_comboCaptureMode2.InsertString(index, "6-1280*720@30fps");
    m_comboCaptureMode2.SetItemData(index, 6);
    index++;

    m_comboCaptureMode2.InsertString(index, "7-1280*720@50fps");
    m_comboCaptureMode2.SetItemData(index, 7);
    index++;

    m_comboCaptureMode2.InsertString(index, "8-1280*720@60fps");
    m_comboCaptureMode2.SetItemData(index, 8);
    index++;

    m_comboCaptureMode2.InsertString(index, "9-1280*960@15fps");
    m_comboCaptureMode2.SetItemData(index, 9);
    index++;

    m_comboCaptureMode2.InsertString(index, "10-1280*960@25fps");
    m_comboCaptureMode2.SetItemData(index, 10);
    index++;

    m_comboCaptureMode2.InsertString(index, "11-1280*960@30fps");
    m_comboCaptureMode2.SetItemData(index, 11);
    index++;

    m_comboCaptureMode2.InsertString(index, "12-1280*1024@25fps");
    m_comboCaptureMode2.SetItemData(index, 12);
    index++;

    m_comboCaptureMode2.InsertString(index, "13-1280*1024@30fps");
    m_comboCaptureMode2.SetItemData(index, 13);
    index++;

    m_comboCaptureMode2.InsertString(index, "14-1600*900@15fps");
    m_comboCaptureMode2.SetItemData(index, 14);
    index++;

    m_comboCaptureMode2.InsertString(index, "15-1600*1200@15fps");
    m_comboCaptureMode2.SetItemData(index, 15);
    index++;

    m_comboCaptureMode2.InsertString(index, "16-1920*1080@15fps");
    m_comboCaptureMode2.SetItemData(index, 16);
    index++;

    m_comboCaptureMode2.InsertString(index, "17-1920*1080@25fps");
    m_comboCaptureMode2.SetItemData(index, 17);
    index++;

    m_comboCaptureMode2.InsertString(index, "18-1920*1080@30fps");
    m_comboCaptureMode2.SetItemData(index, 18);
    index++;
    m_comboCaptureMode2.InsertString(index, "19-1920*1080@50fps");
    m_comboCaptureMode2.SetItemData(index, 19);
    index++;

    m_comboCaptureMode2.InsertString(index, "20-1920*1080@60fps");
    m_comboCaptureMode2.SetItemData(index, 20);
    index++;

    m_comboCaptureMode2.InsertString(index, "21-2048*1536@15fps");
    m_comboCaptureMode2.SetItemData(index, 21);
    index++;

    m_comboCaptureMode2.InsertString(index, "22-2048*1536@20fps");
    m_comboCaptureMode2.SetItemData(index, 22);
    index++;

    m_comboCaptureMode2.InsertString(index, "23-2048*1536@24fps");
    m_comboCaptureMode2.SetItemData(index, 23);
    index++;

    m_comboCaptureMode2.InsertString(index, "24-2048*1536@25fps");
    m_comboCaptureMode2.SetItemData(index, 24);
    index++;

    m_comboCaptureMode2.InsertString(index, "25-2048*1536@30fps");
    m_comboCaptureMode2.SetItemData(index, 25);
    index++;

    m_comboCaptureMode2.InsertString(index, "26-2560*2048@25fps");
    m_comboCaptureMode2.SetItemData(index, 26);
    index++;

    m_comboCaptureMode2.InsertString(index, "27-2560*2048@30fps");
    m_comboCaptureMode2.SetItemData(index, 27);
    index++;

    m_comboCaptureMode2.InsertString(index, "28-2560*1920@7.5fps");
    m_comboCaptureMode2.SetItemData(index, 28);
    index++;
    m_comboCaptureMode2.InsertString(index, "29-3072*2048@25fps");
    m_comboCaptureMode2.SetItemData(index, 29);
    index++;

    m_comboCaptureMode2.InsertString(index, "30-3072*2048@30fps");
    m_comboCaptureMode2.SetItemData(index, 30);
    index++;

    m_comboCaptureMode2.InsertString(index, "31-2048*1536@12.5");
    m_comboCaptureMode2.SetItemData(index, 31);
    index++;

    m_comboCaptureMode2.InsertString(index, "32-2560*1920@6.25");
    m_comboCaptureMode2.SetItemData(index, 32);
    index++;

    m_comboCaptureMode2.InsertString(index, "33-1600*1200@25");
    m_comboCaptureMode2.SetItemData(index, 33);
    index++;

    m_comboCaptureMode2.InsertString(index, "34-1600*1200@30");
    m_comboCaptureMode2.SetItemData(index, 34);
    index++;

    m_comboCaptureMode2.InsertString(index, "35-1600*1200@12.5");
    m_comboCaptureMode2.SetItemData(index, 35);
    index++;
    m_comboCaptureMode2.InsertString(index, "36-1600*900@12.5");
    m_comboCaptureMode2.SetItemData(index, 36);
    index++;

    m_comboCaptureMode2.InsertString(index, "37-1280*960@12.5fps");
    m_comboCaptureMode2.SetItemData(index, 37);
    index++;

    m_comboCaptureMode2.InsertString(index, "38-800*600@25fps");
    m_comboCaptureMode2.SetItemData(index, 38);
    index++;

    m_comboCaptureMode2.InsertString(index, "39-800*600@30fps40");
    m_comboCaptureMode2.SetItemData(index, 39);
    index++;

    m_comboCaptureMode2.InsertString(index, "40-4000*3000@12.5fps");
    m_comboCaptureMode2.SetItemData(index, 40);
    index++;

    m_comboCaptureMode2.InsertString(index, "41-4000*3000@15fps");
    m_comboCaptureMode2.SetItemData(index, 41);
    index++;

    m_comboCaptureMode2.InsertString(index, "42-4096*2160@20fps");
    m_comboCaptureMode2.SetItemData(index, 42);
    index++;

    m_comboCaptureMode2.InsertString(index, "43-3840*2160@20fps");
    m_comboCaptureMode2.SetItemData(index, 43);
    index++;

    m_comboCaptureMode2.InsertString(index, "44-960*576@25fps");
    m_comboCaptureMode2.SetItemData(index, 44);
    index++;

    m_comboCaptureMode2.InsertString(index, "45-960*480@30fps");
    m_comboCaptureMode2.SetItemData(index, 45);
    index++;

    m_comboCaptureMode2.InsertString(index, "46-752*582@25fps");
    m_comboCaptureMode2.SetItemData(index, 46);
    index++;

    m_comboCaptureMode2.InsertString(index, "47-768*494@30fps");
    m_comboCaptureMode2.SetItemData(index, 47);
    index++;

    m_comboCaptureMode2.InsertString(index, "48-2560*1440@25fps");
    m_comboCaptureMode2.SetItemData(index, 48);
    index++;

    m_comboCaptureMode2.InsertString(index, "49-2560*1440@30fps");
    m_comboCaptureMode2.SetItemData(index, 49);
    index++;

    m_comboCaptureMode2.InsertString(index, "50-720P@100fps");
    m_comboCaptureMode2.SetItemData(index, 50);
    index++;

    m_comboCaptureMode2.InsertString(index, "51-720P@120fps");
    m_comboCaptureMode2.SetItemData(index, 51);
    index++;

    m_comboCaptureMode2.InsertString(index, "52-2048*1536@50fps8");
    m_comboCaptureMode2.SetItemData(index, 52);
    index++;

    m_comboCaptureMode2.InsertString(index, "53-2048*1536@60fps");
    m_comboCaptureMode2.SetItemData(index, 53);
    index++;

    m_comboCaptureMode2.InsertString(index, "54-3840*2160@25fps");
    m_comboCaptureMode2.SetItemData(index, 54);
    index++;

    m_comboCaptureMode2.InsertString(index, "55-3840*2160@30fps");
    m_comboCaptureMode2.SetItemData(index, 55);
    index++;

    m_comboCaptureMode2.InsertString(index, "56-4096*2160@25fps");
    m_comboCaptureMode2.SetItemData(index, 56);
    index++;

    m_comboCaptureMode2.InsertString(index, "57-4096*2160@30fps)");
    m_comboCaptureMode2.SetItemData(index, 57);
    index++;

    m_comboCaptureMode2.InsertString(index, "58-1280*1024@50fps");
    m_comboCaptureMode2.SetItemData(index, 58);
    index++;

    m_comboCaptureMode2.InsertString(index, "59-1280*1024@60fps");
    m_comboCaptureMode2.SetItemData(index, 59);
    index++;

    m_comboCaptureMode2.InsertString(index, "60-3072*2048@50fps");
    m_comboCaptureMode2.SetItemData(index, 60);
    index++;

    m_comboCaptureMode2.InsertString(index, "61-3072*2048@60fps");
    m_comboCaptureMode2.SetItemData(index, 61);
    index++;

    m_comboCaptureMode2.InsertString(index, "62-3072*1728@25fps");
    m_comboCaptureMode2.SetItemData(index, 62);
    index++;

    m_comboCaptureMode2.InsertString(index, "63-3072*1728@30fps");
    m_comboCaptureMode2.SetItemData(index, 63);
    index++;

    m_comboCaptureMode2.InsertString(index, "64-3072*1728@50fps0");
    m_comboCaptureMode2.SetItemData(index, 64);
    index++;

    m_comboCaptureMode2.InsertString(index, "65-3072*1728@60fps");
    m_comboCaptureMode2.SetItemData(index, 65);
    index++;

    m_comboCaptureMode2.InsertString(index, "66-336*256@50fps");
    m_comboCaptureMode2.SetItemData(index, 66);
    index++;

    m_comboCaptureMode2.InsertString(index, "67-336*256@60fps");
    m_comboCaptureMode2.SetItemData(index, 67);
    index++;

    m_comboCaptureMode2.InsertString(index, "68-384*288@50fps");
    m_comboCaptureMode2.SetItemData(index, 68);
    index++;

    m_comboCaptureMode2.InsertString(index, "69-384*288@60fps");
    m_comboCaptureMode2.SetItemData(index, 69);
    index++;

    m_comboCaptureMode2.InsertString(index, "70-640*512@50fps");
    m_comboCaptureMode2.SetItemData(index, 70);
    index++;

    m_comboCaptureMode2.InsertString(index, "71-640*512@60fps");
    m_comboCaptureMode2.SetItemData(index, 71);
    index++;

    m_comboCaptureMode2.InsertString(index, "72-2592*1944@25fps");
    m_comboCaptureMode2.SetItemData(index, 72);
    index++;

    m_comboCaptureMode2.InsertString(index, "73-2592*1944@30fps");
    m_comboCaptureMode2.SetItemData(index, 73);
    index++;

    m_comboCaptureMode2.InsertString(index, "74-2688*1536@25fps");
    m_comboCaptureMode2.SetItemData(index, 74);
    index++;

    m_comboCaptureMode2.InsertString(index, "75-2688*1536@30fps");
    m_comboCaptureMode2.SetItemData(index, 75);
    index++;

    m_comboCaptureMode2.InsertString(index, "76-2592*1944@20fps");
    m_comboCaptureMode2.SetItemData(index, 76);
    index++;

    m_comboCaptureMode2.InsertString(index, "77-2592*1944@15fps");
    m_comboCaptureMode2.SetItemData(index, 77);
    index++;

    m_comboCaptureMode2.InsertString(index, "78-2688*1520@20fps");
    m_comboCaptureMode2.SetItemData(index, 78);
    index++;

    m_comboCaptureMode2.InsertString(index, "79-2688*1520@15fps");
    m_comboCaptureMode2.SetItemData(index, 79);
    index++;

    m_comboCaptureMode2.InsertString(index, "80-2688*1520@25fps");
    m_comboCaptureMode2.SetItemData(index, 80);
    index++;

    m_comboCaptureMode2.InsertString(index, "81-2688*1520@30fps");
    m_comboCaptureMode2.SetItemData(index, 81);
    index++;

    m_comboCaptureMode2.InsertString(index, "82-2720*2048@25fps");
    m_comboCaptureMode2.SetItemData(index, 82);
    index++;

    m_comboCaptureMode2.InsertString(index, "83-2720*2048@30fps");
    m_comboCaptureMode2.SetItemData(index, 83);
    index++;

    m_comboCaptureMode2.InsertString(index, "84-336*256@25fps");
    m_comboCaptureMode2.SetItemData(index, 84);
    index++;

    m_comboCaptureMode2.InsertString(index, "85-384*288@25fps");
    m_comboCaptureMode2.SetItemData(index, 85);
    index++;

    m_comboCaptureMode2.InsertString(index, "86-640*512@25fps");
    m_comboCaptureMode2.SetItemData(index, 86);
    index++;

    m_comboCaptureMode2.InsertString(index, "87-1280*960@50fps");
    m_comboCaptureMode2.SetItemData(index, 87);
    index++;

    m_comboCaptureMode2.InsertString(index, "88-1280*960@60fps");
    m_comboCaptureMode2.SetItemData(index, 88);
    index++;

    m_comboCaptureMode2.InsertString(index, "89-1280*960@100fps");
    m_comboCaptureMode2.SetItemData(index, 89);
    index++;

    m_comboCaptureMode2.InsertString(index, "90-1280*960@120fps");
    m_comboCaptureMode2.SetItemData(index, 90);
    index++;

    m_comboCaptureMode2.InsertString(index, "91-4000*3000@20fps");
    m_comboCaptureMode2.SetItemData(index, 91);
    index++;

    m_comboCaptureMode2.InsertString(index, "92-1920*1200@25fps");
    m_comboCaptureMode2.SetItemData(index, 92);
    index++;

    m_comboCaptureMode2.InsertString(index, "93-1920*1200@30fps");
    m_comboCaptureMode2.SetItemData(index, 93);
    index++;

    m_comboCaptureMode2.InsertString(index, "94-2560*1920@25fps");
    m_comboCaptureMode2.SetItemData(index, 94);
    index++;

    m_comboCaptureMode2.InsertString(index, "95-2560*1920@20fps");
    m_comboCaptureMode2.SetItemData(index, 95);
    index++;

    m_comboCaptureMode2.InsertString(index, "96-2560*1920@30fps");
    m_comboCaptureMode2.SetItemData(index, 96);
    index++;

    m_comboCaptureMode2.InsertString(index, "97-1280*1920@25fps");
    m_comboCaptureMode2.SetItemData(index, 97);
    index++;

    m_comboCaptureMode2.InsertString(index, "98-1280*1920@30fps");
    m_comboCaptureMode2.SetItemData(index, 98);
    index++;

    m_comboCaptureMode2.InsertString(index, "99-4000*3000@24fps");
    m_comboCaptureMode2.SetItemData(index, 99);
    index++;

    m_comboCaptureMode2.InsertString(index, "100-4000*3000@25fps");
    m_comboCaptureMode2.SetItemData(index, 100);
    index++;

    m_comboCaptureMode2.InsertString(index, "101-4000*3000@10fps");
    m_comboCaptureMode2.SetItemData(index, 101);
    index++;

    m_comboCaptureMode2.InsertString(index, "102-384*288@30fps");
    m_comboCaptureMode2.SetItemData(index, 102);
    index++;

    m_comboCaptureMode2.InsertString(index, "103-2560*1920@15fps");
    m_comboCaptureMode2.SetItemData(index, 103);
    index++;

    m_comboCaptureMode2.InsertString(index, "104-2400*3840@25fps");
    m_comboCaptureMode2.SetItemData(index, 104);
    index++;

    m_comboCaptureMode2.InsertString(index, "105-1200*1920@25fps8");
    m_comboCaptureMode2.SetItemData(index, 105);
    index++;

    m_comboCaptureMode2.InsertString(index, "106-4096*1800@30fps");
    m_comboCaptureMode2.SetItemData(index, 106);
    index++;

    m_comboCaptureMode2.InsertString(index, "107-3840*1680@30fps");
    m_comboCaptureMode2.SetItemData(index, 107);
    index++;

    m_comboCaptureMode2.InsertString(index, "108-2560*1120@30fps");
    m_comboCaptureMode2.SetItemData(index, 108);
    index++;

    m_comboCaptureMode2.InsertString(index, "109-704*320@30fps");
    m_comboCaptureMode2.SetItemData(index, 109);
    index++;

    m_comboCaptureMode2.InsertString(index, "110-1280*560@30fps");
    m_comboCaptureMode2.SetItemData(index, 110);
    index++;

    m_comboCaptureMode2.InsertString(index, "111-4096*1800@25fps");
    m_comboCaptureMode2.SetItemData(index, 111);
    index++;

    m_comboCaptureMode2.InsertString(index, "112-3840*1680@25fps");
    m_comboCaptureMode2.SetItemData(index, 112);
    index++;

    m_comboCaptureMode2.InsertString(index, "113-2560*1120@25fps");
    m_comboCaptureMode2.SetItemData(index, 113);
    index++;

    m_comboCaptureMode2.InsertString(index, "114-704*320@25fps");
    m_comboCaptureMode2.SetItemData(index, 114);
    index++;

    m_comboCaptureMode2.InsertString(index, "115-1280*560@25fps");
    m_comboCaptureMode2.SetItemData(index, 115);
    index++;

    m_comboCaptureMode2.InsertString(index, "116-2400*3840@24fps");
    m_comboCaptureMode2.SetItemData(index, 116);
    index++;
    m_comboCaptureMode2.InsertString(index, "117-3840*2400@24fps");
    m_comboCaptureMode2.SetItemData(index, 117);
    index++;

    m_comboCaptureMode2.InsertString(index, "118-3840*2400@25fps");
    m_comboCaptureMode2.SetItemData(index, 118);
    index++;

    m_comboCaptureMode2.InsertString(index, "119-2560*1920@12.5fps");
    m_comboCaptureMode2.SetItemData(index, 119);
    index++;

    m_comboCaptureMode2.InsertString(index, "120-2560*2048@12fps");
    m_comboCaptureMode2.SetItemData(index, 120);
    index++;
    m_comboCaptureMode2.InsertString(index, "121-2560*2048@15fps");
    m_comboCaptureMode2.SetItemData(index, 121);
    index++;

    m_comboCaptureMode2.InsertString(index, "122-2560*1536@25fps");
    m_comboCaptureMode2.SetItemData(index, 122);
    index++;

    m_comboCaptureMode2.InsertString(index, "123-2560*1536@30fps");
    m_comboCaptureMode2.SetItemData(index, 123);
    index++;

    m_comboCaptureMode2.InsertString(index, "124-2256*2048@25fps");
    m_comboCaptureMode2.SetItemData(index, 124);
    index++;

    m_comboCaptureMode2.InsertString(index, "125-2256*2048@30fps");
    m_comboCaptureMode2.SetItemData(index, 125);
    index++;

    m_comboCaptureMode2.InsertString(index, "126-2592*2592@12.5fps");
    m_comboCaptureMode2.SetItemData(index, 126);
    index++;

    m_comboCaptureMode2.InsertString(index, "127-2592*2592@15fps");
    m_comboCaptureMode2.SetItemData(index, 127);
    index++;

    m_comboCaptureMode2.InsertString(index, "128-640*512@30fps");
    m_comboCaptureMode2.SetItemData(index, 128);
    index++;

    m_comboCaptureMode2.InsertString(index, "129-2048*1520@30fps");
    m_comboCaptureMode2.SetItemData(index, 129);
    index++;

    m_comboCaptureMode2.InsertString(index, "130-2048*1520@25fps");
    m_comboCaptureMode2.SetItemData(index, 130);
    index++;

    m_comboCaptureMode2.InsertString(index, "131-3840*2160@24fps");
    m_comboCaptureMode2.SetItemData(index, 131);
    index++;

    m_comboCaptureMode2.InsertString(index, "132-2592*1520@25fps");
    m_comboCaptureMode2.SetItemData(index, 132);
    index++;

    m_comboCaptureMode2.InsertString(index, "133-2592*1520@30fps");
    m_comboCaptureMode2.SetItemData(index, 133);
    index++;

    m_comboCaptureMode2.InsertString(index, "134-2592*1536@25fps");
    m_comboCaptureMode2.SetItemData(index, 134);
    index++;

    m_comboCaptureMode2.InsertString(index, "135-2592*1536@30fps");
    m_comboCaptureMode2.SetItemData(index, 135);
    index++;

    m_comboCaptureMode2.InsertString(index, "136-640*960@25fps");
    m_comboCaptureMode2.SetItemData(index, 136);
    index++;

    m_comboCaptureMode2.InsertString(index, "137-640*960@24fps");
    m_comboCaptureMode2.SetItemData(index, 137);
    index++;

    /*m_comboCaptureMode2.InsertString(index, "138-3840*1080");
    m_comboCaptureMode2.SetItemData(index, 138);
    index++;*/

    m_comboCaptureMode2.InsertString(index, "139-3840*1080@25fps");
    m_comboCaptureMode2.SetItemData(index, 139);
    index++;

    m_comboCaptureMode2.InsertString(index, "140-3840*1080@30fps");
    m_comboCaptureMode2.SetItemData(index, 140);
    index++;

    /*m_comboCaptureMode2.InsertString(index, "141-704*200");
    m_comboCaptureMode2.SetItemData(index, 141);
    index++;*/

    m_comboCaptureMode2.InsertString(index, "142-2992*2192@25fps");
    m_comboCaptureMode2.SetItemData(index, 142);
    index++;

    m_comboCaptureMode2.InsertString(index, "143-2992*2192@30fps");
    m_comboCaptureMode2.SetItemData(index, 143);
    index++;

    m_comboCaptureMode2.InsertString(index, "144-3008*2160@25fps");
    m_comboCaptureMode2.SetItemData(index, 144);
    index++;

    m_comboCaptureMode2.InsertString(index, "145-3008*2160@30fps");
    m_comboCaptureMode2.SetItemData(index, 145);
    index++;

    m_comboCaptureMode2.InsertString(index, "146-3072*1728@20fps");
    m_comboCaptureMode2.SetItemData(index, 146);
    index++;

    m_comboCaptureMode2.InsertString(index, "147-2560*1440@20fps");
    m_comboCaptureMode2.SetItemData(index, 147);
    index++;

    m_comboCaptureMode2.InsertString(index, "148-2160*3840@25fps");
    m_comboCaptureMode2.SetItemData(index, 148);
    index++;

    m_comboCaptureMode2.InsertString(index, "149-2160*3840@30fps");
    m_comboCaptureMode2.SetItemData(index, 149);
    index++;

    m_comboCaptureMode2.InsertString(index, "150-7008*1080@25fps");
    m_comboCaptureMode2.SetItemData(index, 150);
    index++;

    m_comboCaptureMode2.InsertString(index, "151-7008*1080@30fps");
    m_comboCaptureMode2.SetItemData(index, 151);
    index++;

    m_comboCaptureMode2.InsertString(index, "152-3072*2048@20fps");
    m_comboCaptureMode2.SetItemData(index, 152);
    index++;

    m_comboCaptureMode2.InsertString(index, "153-1536*864@25fps");
    m_comboCaptureMode2.SetItemData(index, 153);
    index++;

    m_comboCaptureMode2.InsertString(index, "154-2560*1920@24fps");
    m_comboCaptureMode2.SetItemData(index, 154);
    index++;

    m_comboCaptureMode2.InsertString(index, "155-2400*3840@30fps");
    m_comboCaptureMode2.SetItemData(index, 155);
    index++;

    m_comboCaptureMode2.InsertString(index, "156-3840*2400@30fps");
    m_comboCaptureMode2.SetItemData(index, 156);
    index++;

    m_comboCaptureMode2.InsertString(index, "157-3840*2160@15fps");
    m_comboCaptureMode2.SetItemData(index, 157);
    index++;

    m_comboCaptureMode2.InsertString(index, "158-384*288@8.3fps");
    m_comboCaptureMode2.SetItemData(index, 158);
    index++;

    m_comboCaptureMode2.InsertString(index, "159-640*512@8.3fps");
    m_comboCaptureMode2.SetItemData(index, 159);
    index++;

    m_comboCaptureMode2.InsertString(index, "160-160*120@8.3fps");
    m_comboCaptureMode2.SetItemData(index, 160);
    index++;

    m_comboCaptureMode2.InsertString(index, "161-1024*768@8.3fps");
    m_comboCaptureMode2.SetItemData(index, 161);
    index++;

    m_comboCaptureMode2.InsertString(index, "162-640*480@8.3fps");
    m_comboCaptureMode2.SetItemData(index, 162);
    index++;

    m_comboCaptureMode2.InsertString(index, "163-3840*2160@12.5fps");
    m_comboCaptureMode2.SetItemData(index, 163);
    index++;

    m_comboCaptureMode2.InsertString(index, "164-2304*1296@30fps");
    m_comboCaptureMode2.SetItemData(index, 164);
    index++;

    m_comboCaptureMode2.InsertString(index, "165-2304*1296@25fps");
    m_comboCaptureMode2.SetItemData(index, 165);
    index++;

    m_comboCaptureMode2.InsertString(index, "166-2560*1440@24fps");
    m_comboCaptureMode2.SetItemData(index, 166);
    index++;

    m_comboCaptureMode2.InsertString(index, "167-2688*1512@25fps");
    m_comboCaptureMode2.SetItemData(index, 167);
    index++;

    m_comboCaptureMode2.InsertString(index, "168-2688*1512@30fps");
    m_comboCaptureMode2.SetItemData(index, 168);
    index++;

    m_comboCaptureMode2.InsertString(index, "169-2688*1512@50fps");
    m_comboCaptureMode2.SetItemData(index, 169);
    index++;

    m_comboCaptureMode2.InsertString(index, "170-2688*1512@60fps");
    m_comboCaptureMode2.SetItemData(index, 170);
    index++;

    m_comboCaptureMode2.InsertString(index, "171-1536*864@30fps");
    m_comboCaptureMode2.SetItemData(index, 171);
    index++;

    m_comboCaptureMode2.InsertString(index, "172-2560*1440@50fps");
    m_comboCaptureMode2.SetItemData(index, 172);
    index++;

    m_comboCaptureMode2.InsertString(index, "173-2560*1440@60fps");
    m_comboCaptureMode2.SetItemData(index, 173);
    index++;

    m_comboCaptureMode2.InsertString(index, "174-2048*2048@25fps");
    m_comboCaptureMode2.SetItemData(index, 174);
    index++;

    m_comboCaptureMode2.InsertString(index, "175-2048*2048@30fps");
    m_comboCaptureMode2.SetItemData(index, 175);
    index++;

    m_comboCaptureMode2.InsertString(index, "176-4000*3060@20fps");
    m_comboCaptureMode2.SetItemData(index, 176);
    index++;

    m_comboCaptureMode2.InsertString(index, "177-3060*3060@25fps");
    m_comboCaptureMode2.SetItemData(index, 177);
    index++;

    m_comboCaptureMode2.InsertString(index, "178-3060*3060@30fps");
    m_comboCaptureMode2.SetItemData(index, 178);
    index++;

    m_comboCaptureMode2.InsertString(index, "179-3000*3000@25fps");
    m_comboCaptureMode2.SetItemData(index, 179);
    index++;

    m_comboCaptureMode2.InsertString(index, "180-3000*3000@30fps");
    m_comboCaptureMode2.SetItemData(index, 180);
    index++;

    m_comboCaptureMode2.InsertString(index, "181-8160*3616@30fps");
    m_comboCaptureMode2.SetItemData(index, 181);
    index++;

    m_comboCaptureMode2.InsertString(index, "182-8160*3616@25fps");
    m_comboCaptureMode2.SetItemData(index, 182);
    index++;

    m_comboCaptureMode2.InsertString(index, "183-3000*3000@20fps");
    m_comboCaptureMode2.SetItemData(index, 183);
    index++;

    m_comboCaptureMode2.InsertString(index, "184-3000*3000@15fps");
    m_comboCaptureMode2.SetItemData(index, 184);
    index++;

    m_comboCaptureMode2.InsertString(index, "185-3000*3000@12.5fps2");
    m_comboCaptureMode2.SetItemData(index, 185);
    index++;

    m_comboCaptureMode2.InsertString(index, "186-5472*3648@25fps");
    m_comboCaptureMode2.SetItemData(index, 186);
    index++;

    m_comboCaptureMode2.InsertString(index, "187-5472*3648@30fps");
    m_comboCaptureMode2.SetItemData(index, 187);
    index++;

    m_comboCaptureMode2.InsertString(index, "188-7680*4320@25fps");
    m_comboCaptureMode2.SetItemData(index, 188);
    index++;

    m_comboCaptureMode2.InsertString(index, "189-7680*4320@30fps");
    m_comboCaptureMode2.SetItemData(index, 189);
    index++;

    m_comboCaptureMode2.InsertString(index, "190-8160*2400@25fps");
    m_comboCaptureMode2.SetItemData(index, 190);
    index++;

    m_comboCaptureMode2.InsertString(index, "191-8160*2400@30fps");
    m_comboCaptureMode2.SetItemData(index, 191);
    index++;

    m_comboCaptureMode2.InsertString(index, "192-5520*2400@25fps");
    m_comboCaptureMode2.SetItemData(index, 192);
    index++;

    m_comboCaptureMode2.InsertString(index, "93-5520*2400@30fps");
    m_comboCaptureMode2.SetItemData(index, 193);
    index++;

    m_comboCaptureMode2.InsertString(index, "194-2560*1440@15fps");
    m_comboCaptureMode2.SetItemData(index, 194);
    index++;

    m_comboCaptureMode2.InsertString(index, "195-1944*1212@24fps");
    m_comboCaptureMode2.SetItemData(index, 195);
    index++;

    m_comboCaptureMode2.InsertString(index, "196-1944*1212@25fps");
    m_comboCaptureMode2.SetItemData(index, 196);
    index++;

    m_comboCaptureMode2.InsertString(index, "197-3456*1920@30fps");
    m_comboCaptureMode2.SetItemData(index, 197);
    index++;

    m_comboCaptureMode2.InsertString(index, "198-4800*2688@25fps");
    m_comboCaptureMode2.SetItemData(index, 198);
    index++;

    m_comboCaptureMode2.InsertString(index, "199-4800*2688@30fps");
    m_comboCaptureMode2.SetItemData(index, 199);
    index++;

    m_comboCaptureMode2.InsertString(index, "200-6480*1080@25fps");
    m_comboCaptureMode2.SetItemData(index, 200);
    index++;

    m_comboCaptureMode2.InsertString(index, "201-6480*1080@30fps");
    m_comboCaptureMode2.SetItemData(index, 201);
    index++;

    m_comboCaptureMode2.InsertString(index, "203-8640*1440@25fps");
    m_comboCaptureMode2.SetItemData(index, 202);
    index++;

    m_comboCaptureMode2.InsertString(index, "8640*1440@30fps");
    m_comboCaptureMode2.SetItemData(index, 203);
    index++;

    m_comboCaptureMode2.InsertString(index, "204-3456*1920@25fps");
    m_comboCaptureMode2.SetItemData(index, 204);
    index++;

    m_comboCaptureMode2.InsertString(index, "205-2688*1520@50fps");
    m_comboCaptureMode2.SetItemData(index, 205);
    index++;

    m_comboCaptureMode2.InsertString(index, "206-2688*1520@60fps");
    m_comboCaptureMode2.SetItemData(index, 206);
    index++;

    m_comboCaptureMode2.InsertString(index, "207-4976*1452@25fps");
    m_comboCaptureMode2.SetItemData(index, 207);
    index++;

    m_comboCaptureMode2.InsertString(index, "208-4976*1452@30fps");
    m_comboCaptureMode2.SetItemData(index, 208);
    index++;

    m_comboCaptureMode2.InsertString(index, "209-3200*1800@25fps");
    m_comboCaptureMode2.SetItemData(index, 209);
    index++;

    m_comboCaptureMode2.InsertString(index, "210-3200*1800@30fps");
    m_comboCaptureMode2.SetItemData(index, 210);
    index++;

    m_comboCaptureMode2.InsertString(index, "211-5472*3648@24fps");
    m_comboCaptureMode2.SetItemData(index, 211);
    index++;

    m_comboCaptureMode2.InsertString(index, "212-1920*1080@12.5fps");
    m_comboCaptureMode2.SetItemData(index, 212);
    index++;

    m_comboCaptureMode2.InsertString(index, "213-2944*1656@20fps");
    m_comboCaptureMode2.SetItemData(index, 213);
    index++;

    m_comboCaptureMode2.InsertString(index, "214-1920*1080@24fps");
    m_comboCaptureMode2.SetItemData(index, 214);
    index++;

    m_comboCaptureMode2.InsertString(index, "215-4800*1600@25fps");
    m_comboCaptureMode2.SetItemData(index, 215);
    index++;

    m_comboCaptureMode2.InsertString(index, "216-4800*1600@30fps");
    m_comboCaptureMode2.SetItemData(index, 216);
    index++;

    m_comboCaptureMode2.InsertString(index, "217-2560*1440@12.5fps");
    m_comboCaptureMode2.SetItemData(index, 217);
    index++;

    m_comboCaptureMode2.InsertString(index, "218-6560*3690@1fps");
    m_comboCaptureMode2.SetItemData(index, 218);
    index++;

    m_comboCaptureMode2.InsertString(index, "219-5120*1400@20fps2");
    m_comboCaptureMode2.SetItemData(index, 219);
    index++;

    m_comboCaptureMode2.InsertString(index, "220-7680*4320@1fps");
    m_comboCaptureMode2.SetItemData(index, 220);
    index++;

    m_comboCaptureMode2.InsertString(index, "221-1920*1080@20fps");
    m_comboCaptureMode2.SetItemData(index, 221);
    index++;

    m_comboCaptureMode2.InsertString(index, "222-5120*1440@20fps");
    m_comboCaptureMode2.SetItemData(index, 222);
    index++;

    m_comboCaptureMode2.InsertString(index, "223-4080*1808@25fps");
    m_comboCaptureMode2.SetItemData(index, 223);
    index++;

    m_comboCaptureMode2.InsertString(index, "224-4080*1808@30fps");
    m_comboCaptureMode2.SetItemData(index, 224);
    index++;

    m_comboCaptureMode2.InsertString(index, "225-4080*1152@25fps");
    m_comboCaptureMode2.SetItemData(index, 225);
    index++;

    m_comboCaptureMode2.InsertString(index, "226-4080*1152@30fps");
    m_comboCaptureMode2.SetItemData(index, 226);
    index++;

    m_comboCaptureMode2.InsertString(index, "227-2688*1944@20fps");
    m_comboCaptureMode2.SetItemData(index, 227);
    index++;

    m_comboCaptureMode2.InsertString(index, "228-2592*1944@24fps");
    m_comboCaptureMode2.SetItemData(index, 228);
    index++;

    m_comboCaptureMode2.InsertString(index, "229-3200*1800@15fps");
    m_comboCaptureMode2.SetItemData(index, 229);
    index++;

    m_comboCaptureMode2.InsertString(index, "230-4416*1696@20fps");
    m_comboCaptureMode2.SetItemData(index, 230);
    index++;

    m_comboCaptureMode2.InsertString(index, "231-3456*1080@25fps");
    m_comboCaptureMode2.SetItemData(index, 231);
    index++;

    m_comboCaptureMode2.InsertString(index, "232-3200*1800@12.5fps");
    m_comboCaptureMode2.SetItemData(index, 232);
    index++;

    m_comboCaptureMode2.InsertString(index, "233-2688*1532@25fps");
    m_comboCaptureMode2.SetItemData(index, 233);
    index++;

    m_comboCaptureMode2.InsertString(index, "234-2688*1532@30fps");
    m_comboCaptureMode2.SetItemData(index, 234);
    index++;

    m_comboCaptureMode2.InsertString(index, "235-4416*1696@12.5fps");
    m_comboCaptureMode2.SetItemData(index, 235);
    index++;

    m_comboCaptureMode2.InsertString(index, "236-3840*2048P12.5fps");
    m_comboCaptureMode2.SetItemData(index, 236);
    index++;

    m_comboCaptureMode2.InsertString(index, "237-3840*4096P12.5fps");
    m_comboCaptureMode2.SetItemData(index, 237);
    index++;

    m_comboCaptureMode2.InsertString(index, "238-5120*1440@12.5fps");
    m_comboCaptureMode2.SetItemData(index, 238);
    index++;

    m_comboCaptureMode2.InsertString(index, "239-3840*1080@24fps");
    m_comboCaptureMode2.SetItemData(index, 239);
    index++;

    m_comboCaptureMode2.InsertString(index, "240-320*256@30fps");
    m_comboCaptureMode2.SetItemData(index, 240);
    index++;

    m_comboCaptureMode2.InsertString(index, "241-3264*2448@25fps");
    m_comboCaptureMode2.SetItemData(index, 241);
    index++;

    m_comboCaptureMode2.InsertString(index, "242-3264*2448@30fps");
    m_comboCaptureMode2.SetItemData(index, 242);
    index++;

    m_comboCaptureMode2.InsertString(index, "243-5430*3054@1fps");
    m_comboCaptureMode2.SetItemData(index, 243);
    index++;

    m_comboCaptureMode2.InsertString(index, "244-2688*1520@24@24fps");
    m_comboCaptureMode2.SetItemData(index, 244);
    index++;

    m_comboCaptureMode2.InsertString(index, "245-4000*3000@30fps");
    m_comboCaptureMode2.SetItemData(index, 245);
    index++;

    m_comboCaptureMode2.InsertString(index, "246-1632*1224@25fps");
    m_comboCaptureMode2.SetItemData(index, 246);
    index++;

    m_comboCaptureMode2.InsertString(index, "247-1632*1224@30fps");
    m_comboCaptureMode2.SetItemData(index, 247);
    index++;

    m_comboCaptureMode2.InsertString(index, "248-160*120@25fps");
    m_comboCaptureMode2.SetItemData(index, 248);
    index++;

    m_comboCaptureMode2.InsertString(index, "249-1920*1440@25fps");
    m_comboCaptureMode2.SetItemData(index, 249);
    index++;

    m_comboCaptureMode2.InsertString(index, "250-1920*1440@30fps");
    m_comboCaptureMode2.SetItemData(index, 250);
    index++;

    m_comboCaptureMode2.InsertString(index, "251-3632*1632@20fps");
    m_comboCaptureMode2.SetItemData(index, 251);
    index++;

    m_comboCaptureMode2.InsertString(index, "252-3040*1368@25fps");
    m_comboCaptureMode2.SetItemData(index, 252);
    index++;

    m_comboCaptureMode2.InsertString(index, "253-3040*1368@24fps");
    m_comboCaptureMode2.SetItemData(index, 253);
    index++;

    m_comboCaptureMode2.InsertString(index, "254-5120*1440@25fps");
    m_comboCaptureMode2.SetItemData(index, 254);
    index++;

    m_comboCaptureMode2.InsertString(index, "256-160*120@50fps");
    m_comboCaptureMode2.SetItemData(index, 256);
    index++;

    m_comboCaptureMode2.InsertString(index, "257-3200*1800@20fps");
    m_comboCaptureMode2.SetItemData(index, 257);
    index++;

    m_comboCaptureMode2.InsertString(index, "258-800*480@25fps");
    m_comboCaptureMode2.SetItemData(index, 258);
    index++;

    m_comboCaptureMode2.InsertString(index, "259-2688*1944@25fps");
    m_comboCaptureMode2.SetItemData(index, 259);
    index++;

    m_comboCaptureMode2.InsertString(index, "260-640*384@50fps");
    m_comboCaptureMode2.SetItemData(index, 260);
    index++;

    m_comboCaptureMode2.InsertString(index, "261-8000*6000@1fps");
    m_comboCaptureMode2.SetItemData(index, 261);
    index++;

    m_comboCaptureMode2.InsertString(index, "262-1440*1080@50fps");
    m_comboCaptureMode2.SetItemData(index, 262);
    index++;

    m_comboCaptureMode2.InsertString(index, "263-1440*1080@60fps");
    m_comboCaptureMode2.SetItemData(index, 263);

    index++;
    m_comboCaptureMode2.InsertString(index, "264-8160X3616@24fps");
    m_comboCaptureMode2.SetItemData(index, 264);
    index++;

    m_comboCaptureMode2.InsertString(index, "265-3632*1632@25fps");
    m_comboCaptureMode2.SetItemData(index, 265);
    index++;

    m_comboCaptureMode2.InsertString(index, "266-3632*1632@30fps");
    m_comboCaptureMode2.SetItemData(index, 266);
    index++;

    m_comboCaptureMode2.InsertString(index, "267-3632*1632@20fps");
    m_comboCaptureMode2.SetItemData(index, 267);
    index++;

    m_comboCaptureMode2.InsertString(index, "268-1760*1320@25fps");
    m_comboCaptureMode2.SetItemData(index, 268);
    index++;

    m_comboCaptureMode2.InsertString(index, "269-4000*3000@4fps");
    m_comboCaptureMode2.SetItemData(index, 269);
    index++;

    m_comboCaptureMode2.InsertString(index, "270-192*256@25fps");
    m_comboCaptureMode2.SetItemData(index, 270);
    index++;

    m_comboCaptureMode2.InsertString(index, "271-720*576@25fps");
    m_comboCaptureMode2.SetItemData(index, 271);
    index++;

    m_comboCaptureMode2.InsertString(index, "272-720x576@30fps");
    m_comboCaptureMode2.SetItemData(index, 272);
    index++;

    m_comboCaptureMode2.InsertString(index, "273-960*432@25fps");
    m_comboCaptureMode2.SetItemData(index, 273);
    index++;

    m_comboCaptureMode2.InsertString(index, "274-960*432@30fps");
    m_comboCaptureMode2.SetItemData(index, 274);
    index++;

    m_comboCaptureMode2.InsertString(index, "275-1200*536@25fps");
    m_comboCaptureMode2.SetItemData(index, 275);
    index++;

    m_comboCaptureMode2.InsertString(index, "276-1200*536@30fps");
    m_comboCaptureMode2.SetItemData(index, 276);
    index++;

    m_comboCaptureMode2.InsertString(index, "277-1760*1320@12.5fps");
    m_comboCaptureMode2.SetItemData(index, 277);
    index++;

    m_comboCaptureMode2.InsertString(index, "278-2560*480@25fps");
    m_comboCaptureMode2.SetItemData(index, 278);
    index++;

    m_comboCaptureMode2.InsertString(index, "279-2048*384@25fps");
    m_comboCaptureMode2.SetItemData(index, 279);
    index++;

    m_comboCaptureMode2.InsertString(index, "280-96*96@25fps");
    m_comboCaptureMode2.SetItemData(index, 280);
    index++;

    m_comboCaptureMode2.InsertString(index, "281-320*256@25fps");
    m_comboCaptureMode2.SetItemData(index, 281);
    index++;

    m_comboCaptureMode2.InsertString(index, "282-6128*1800@25fps");
    m_comboCaptureMode2.SetItemData(index, 282);
    index++;

    m_comboCaptureMode2.InsertString(index, "283-6128*1800@30fps");
    m_comboCaptureMode2.SetItemData(index, 283);
    index++;

    m_comboCaptureMode2.InsertString(index, "284-2304*1296@24fps");
    m_comboCaptureMode2.SetItemData(index, 284);
    index++;

    m_comboCaptureMode2.InsertString(index, "285-2048*1152@25fps");
    m_comboCaptureMode.SetItemData(index, 285);
    index++;

    m_comboCaptureMode2.InsertString(index, "286-2048*1152@30fps");
    m_comboCaptureMode.SetItemData(index, 286);
    index++;

    m_comboCaptureMode2.InsertString(index, "285-2048*1152@25fps");
    m_comboCaptureMode2.SetItemData(index, 285);
    index++;

    m_comboCaptureMode2.InsertString(index, "286-2048*1152@30fps");
    m_comboCaptureMode2.SetItemData(index, 286);
    index++;

    m_comboCaptureMode2.InsertString(index, "287-3840*2100@20fps");
    m_comboCaptureMode2.SetItemData(index, 287);
    index++;

    m_comboCaptureMode2.InsertString(index, "288-96*72@25fps");
    m_comboCaptureMode2.SetItemData(index, 288);
    index++;

    m_comboCaptureMode2.InsertString(index, "289-2048*1152@24fps");
    m_comboCaptureMode2.SetItemData(index, 289);
    index++;

    m_comboCaptureMode2.InsertString(index, "290-720*576@50fps");
    m_comboCaptureMode2.SetItemData(index, 290);
    index++;

    m_comboCaptureMode2.InsertString(index, "291-2368*1776@25fps");
    m_comboCaptureMode2.SetItemData(index, 291);
    index++;

    m_comboCaptureMode2.InsertString(index, "292-2368*1776@30fps");
    m_comboCaptureMode2.SetItemData(index, 292);
    index++;

    m_comboCaptureMode2.InsertString(index, "293-1776*1776@25fps");
    m_comboCaptureMode2.SetItemData(index, 293);
    index++;

    m_comboCaptureMode2.InsertString(index, "294-3776*2832@25fps");
    m_comboCaptureMode2.SetItemData(index, 294);
    index++;

    m_comboCaptureMode2.InsertString(index, "295-3776*2832@30fps");
    m_comboCaptureMode2.SetItemData(index, 295);
    index++;

    m_comboCaptureMode2.InsertString(index, "296-2832*2832@25ps");
    m_comboCaptureMode2.SetItemData(index, 296);
    index++;

    m_comboCaptureMode2.InsertString(index, "297-2832*2832@30");
    m_comboCaptureMode2.SetItemData(index, 297);
    index++;

    m_comboCaptureMode2.InsertString(index, "298-1776*1776@30fps");
    m_comboCaptureMode2.SetItemData(index, 298);
    index++;
}

void  CDlgCcdParam::AddBackLightMode()
{
    int index = 0;
    m_comboBackLightMode.InsertString(index, "0-OFF");
    m_comboBackLightMode.SetItemData(index, 0);
    index++;

    m_comboBackLightMode.InsertString(index, "1-UP");
    m_comboBackLightMode.SetItemData(index, 1);
    index++;

    m_comboBackLightMode.InsertString(index, "2-DOWN");
    m_comboBackLightMode.SetItemData(index, 2);
    index++;

    m_comboBackLightMode.InsertString(index, "3-LEFT");
    m_comboBackLightMode.SetItemData(index, 3);
    index++;

    m_comboBackLightMode.InsertString(index, "4-RIGHT");
    m_comboBackLightMode.SetItemData(index, 4);
    index++;

    m_comboBackLightMode.InsertString(index, "5-MIDDLE");
    m_comboBackLightMode.SetItemData(index, 5);
    index++;

    m_comboBackLightMode.InsertString(index, "6-USERSET");
    m_comboBackLightMode.SetItemData(index, 6);
    index++;

    m_comboBackLightMode.InsertString(index, "10-START");
    m_comboBackLightMode.SetItemData(index, 10);
    index++;

    m_comboBackLightMode.InsertString(index, "11-AUTO");
    m_comboBackLightMode.SetItemData(index, 11);
    index++;

    m_comboBackLightMode.InsertString(index, "12-Multi regional backlight compensation");
    m_comboBackLightMode.SetItemData(index, 12);
    index++;
}