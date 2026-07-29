/**********************************************************
FileName:    DlgVcaRuleCfgEx.cpp
Description: VCA Rule config and relate config      
Date:        2019/08/30
Note: 		
<version> <time>         <desc>
<1.0    > <2019/08/30>       <created>
***********************************************************/
// DlgVcaRuleCfgEx.cpp : implementation file
//
#include "stdafx.h"
#include "clientdemo.h"
#include "DlgVcaRuleCfgEx.h"
#include "DlgVcaEnterRegion.h"
#include "DlgVcaMaskRegion.h"
#include <math.h>
#include "gdiplus/GdiPlus.h"
#include "DlgVcaLF.h"
#include "DrawFun.h"
#include "./xml/XmlBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
CDlgVcaRuleCfgEx *g_pDlgVcaRuleCfgEx;  
#define  AlarmLine 1
#define  AlarmArea 2
/////////////////////////////////////////////////////////////////////////////
// CDlgVcaRuleCfgEx dialog

/*********************************************************
Function:	DrawAlertShowEx
Desc:		Callback of Draw on preview
Input:	nPort that is Player Handle
hDc    : DC handle of player window
dwUser: User data
Output:	none
Return:	none
**********************************************************/
void CALLBACK DrawAlertShowEx(LONG lRealHandle, HDC hDc, DWORD dwUser)
{
    SetBkMode(hDc, TRANSPARENT);
    SetTextColor(hDc, RGB(255, 255, 255));
    if(g_pDlgVcaRuleCfgEx->m_bRuleActive)
    {
        g_pDlgVcaRuleCfgEx->F_DrawFun(lRealHandle, hDc, dwUser); 
    }
    
}

/*********************************************************
Function:	CDlgVcaRuleCfgEx
Desc:		Constructor
Input:	
Output:	
Return:	
**********************************************************/
CDlgVcaRuleCfgEx::CDlgVcaRuleCfgEx(CWnd* pParent /*=NULL*/)
: CDialog(CDlgVcaRuleCfgEx::IDD, pParent)
, m_iRuleParam3(0)
, m_iRuleParam4(0)
, m_iRuleParam5(0)
, m_iRuleParam6(0)
{
    //{{AFX_DATA_INIT(CDlgVcaRuleCfgEx)
    m_bFilterActive = FALSE;
    m_bSetMaxFilter = FALSE;
    m_bSetMiniFilter = FALSE;
    m_bHandleType1 = FALSE;
    m_bHandleType2 = FALSE;
    m_bHandleType3 = FALSE;
    m_bHandleType4 = FALSE;
    m_bHandleType5 = FALSE;
    m_bDrawPolygon = FALSE;
    m_bRuleActive = FALSE;
    m_csRuleName = _T("");
    m_iHour11 = 0;
    m_iHour12 = 0;
    m_iHour21 = 0;
    m_iHour22 = 0;
    m_iMin11 = 0;
    m_iMin12 = 0;
    m_iMin21 = 0;
    m_iMin22 = 0;
    m_bDrawLine = FALSE;
	m_fMinHeight = 0.0f;
	m_fMinWidth = 0.0f;
	m_fMaxHeight = 0.0f;
	m_fMaxWidth = 0.0f;
	m_bChkPicRecordEn = FALSE;
	m_iHour31 = 0;
	m_iHour32 = 0;
	m_iHour41 = 0;
	m_iHour42 = 0;
	m_iHour51 = 0;
	m_iHour52 = 0;
	m_iHour61 = 0;
	m_iHour62 = 0;
	m_iHour71 = 0;
	m_iHour72 = 0;
	m_iHour81 = 0;
	m_iHour82 = 0;
	m_iMin31 = 0;
	m_iMin32 = 0;
	m_iMin41 = 0;
	m_iMin42 = 0;
	m_iMin51 = 0;
	m_iMin52 = 0;
	m_iMin61 = 0;
	m_iMin62 = 0;
	m_iMin71 = 0;
	m_iMin72 = 0;
	m_iMin81 = 0;
	m_iMin82 = 0;
	m_fTriggerArea = 0;

	//}}AFX_DATA_INIT
    m_lServerID = -1;
    m_iDevIndex = -1;
    m_iVcaChannel = -1;
    memset(&m_struBehaviorAbility, 0, sizeof(m_struBehaviorAbility)) ;
    memset(&m_struVcaChanInParam, 0, sizeof(m_struVcaChanInParam));
    memset(&m_struVcaCtrlCfg, 0, sizeof(m_struVcaCtrlCfg));
    memset(&m_struSizeFilter, 0, sizeof(m_struSizeFilter));
    memset(&m_rcWnd, 0, sizeof(m_rcWnd));
    m_lPlayHandle = -1;
    m_dwPosNum = 0;
    memset(&m_struVcaPolygon[0], 0, sizeof(NET_VCA_POLYGON)*MAX_RULE_NUM_V42);
    memset(&m_struVcaLine[0], 0, sizeof(NET_VCA_LINE)*MAX_RULE_NUM_V42);
    memset(&m_bCloseIn[0], 0, sizeof(BOOL)*MAX_RULE_NUM_V42);
    memset(&m_bNeedRedraw[0], 0, sizeof(BOOL)*MAX_RULE_NUM_V42);
    memset(&m_bMouseMove[0], 0, sizeof(BOOL)*MAX_RULE_NUM_V42);
    m_iCurRuleID = -1;
    m_lPUServerID = -1;
	memset(&m_bAlarmOut[0], 0, sizeof(BOOL)*MAX_ALARMOUT_V30);
	m_dwEventType = 0xffffffff;
    //
    memset(&m_struChanGroup, 0, sizeof(m_struChanGroup));
    memset(&m_struVcaRule, 0, sizeof(m_struVcaRule));
    memset(&m_struVcaChanAbilityXml, 0, sizeof(m_struVcaChanAbilityXml));

    //参数控件 用于批量隐藏
    m_staticRuleParamIDs[0] = IDC_STATIC_RULE_PARAM1;
    m_staticRuleParamIDs[1] = IDC_COMBO_RULE_PARAM1;
    m_staticRuleParamIDs[2] = IDC_STATIC_RULE_PARAM2;
    m_staticRuleParamIDs[3] = IDC_COMBO_RULE_PARAM2;
    m_staticRuleParamIDs[4] = IDC_STATIC_RULE_PARAM3;
    m_staticRuleParamIDs[5] = IDC_EDIT_RULE_PARAM3;
    m_staticRuleParamIDs[6] = IDC_STATIC_RULE_PARAM4;
    m_staticRuleParamIDs[7] = IDC_EDIT_RULE_PARAM4;
    m_staticRuleParamIDs[8] = IDC_STATIC_RULE_PARAM5;
    m_staticRuleParamIDs[9] = IDC_EDIT_RULE_PARAM5;
    m_staticRuleParamIDs[10] = IDC_STATIC_RULE_PARAM6;
    m_staticRuleParamIDs[11] = IDC_EDIT_RULE_PARAM6;

}

/*********************************************************
Function:	DoDataExchange
Desc:		the map between control and variable
Input:	none
Output:	none
Return:	none
**********************************************************/
void CDlgVcaRuleCfgEx::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgVcaRuleCfgEx)
    DDX_Control(pDX, IDC_COMBO_RELSNAPCHAN3, m_comboRelSnapChan3);
    DDX_Control(pDX, IDC_COMBO_RELSNAPCHAN2, m_comboRelSnapChan2);
    DDX_Control(pDX, IDC_COMBO_RELSNAPCHAN1, m_comboRelSnapChan1);
    DDX_Control(pDX, IDC_COMBO_FILTER_STRATEGY, m_comboFilterStrategy);
    DDX_Control(pDX, IDC_COMBO_TRIGGERPOINT, m_comboTriggerPoint);
    DDX_Control(pDX, IDC_COMBO_TRIGGERMODE, m_comboTriggerMode);
    DDX_Control(pDX, IDC_COMBO_ALARM_DELAY, m_comboAlarmDelay);
    DDX_Control(pDX, IDC_COMBO_UP_LASTALARM, m_cmUpLastAlarm);
    DDX_Control(pDX, IDC_COMBO_FILTER_MODE, m_comboFilterMode);
    DDX_Control(pDX, IDC_LIST_RECORD_CHAN, m_listRecordChan);
    DDX_Control(pDX, IDC_COMBOWEEKDAY, m_comboWeekday);
    DDX_Control(pDX, IDC_COMBOCOPYTIME, m_comboCopyTime);
    DDX_Control(pDX, IDC_COMBO_RULE_ID, m_comboRuleID);
    DDX_Control(pDX, IDC_COMBO_RULE_TYPE, m_comboRuleType);
    DDX_Control(pDX, IDC_COMBO_PIC_QUALITY, m_comboPicQuality);
    DDX_Control(pDX, IDC_COMBO_PRO_PIC_TYPE, m_comboProPicType);
    DDX_Check(pDX, IDC_CHECK_FILTER_ACTIVE, m_bFilterActive);
    DDX_Check(pDX, IDC_CHECK_SET_MAX_FILTER, m_bSetMaxFilter);
    DDX_Check(pDX, IDC_CHECK_SET_MINI_FILTER, m_bSetMiniFilter);
    DDX_Check(pDX, IDC_CHECKHANDLETYPE1, m_bHandleType1);
    DDX_Check(pDX, IDC_CHECKHANDLETYPE2, m_bHandleType2);
    DDX_Check(pDX, IDC_CHECKHANDLETYPE3, m_bHandleType3);
    DDX_Check(pDX, IDC_CHECKHANDLETYPE4, m_bHandleType4);
    DDX_Check(pDX, IDC_CHECKHANDLETYPE5, m_bHandleType5);
    DDX_Check(pDX, IDC_CHK_DRAW_POLYGON, m_bDrawPolygon);
    DDX_Check(pDX, IDC_CHK_RULE_ACTIVE, m_bRuleActive);
    DDX_Text(pDX, IDC_EDIT_RULE_NAME, m_csRuleName);
    DDV_MaxChars(pDX, m_csRuleName, 31);
    DDX_Text(pDX, IDC_EDITHOUR11, m_iHour11);
    DDX_Text(pDX, IDC_EDITHOUR12, m_iHour12);
    DDX_Text(pDX, IDC_EDITHOUR21, m_iHour21);
    DDX_Text(pDX, IDC_EDITHOUR22, m_iHour22);
    DDX_Text(pDX, IDC_EDITMIN11, m_iMin11);
    DDX_Text(pDX, IDC_EDITMIN12, m_iMin12);
    DDX_Text(pDX, IDC_EDITMIN21, m_iMin21);
    DDX_Text(pDX, IDC_EDITMIN22, m_iMin22);
    DDX_Check(pDX, IDC_CHK_DRAW_LINE, m_bDrawLine);
    DDX_Text(pDX, IDC_EDIT_MIN_HEIGHT, m_fMinHeight);
    DDX_Text(pDX, IDC_EDIT_MIN_WIDTH, m_fMinWidth);
    DDX_Text(pDX, IDC_EDIT_MAX_HEIGHT, m_fMaxHeight);
    DDX_Text(pDX, IDC_EDIT_MAX_WIDTH, m_fMaxWidth);
    DDX_Check(pDX, IDC_CHK_PIC_RECORD_EN, m_bChkPicRecordEn);
    DDX_Text(pDX, IDC_EDITHOUR31, m_iHour31);
    DDX_Text(pDX, IDC_EDITHOUR32, m_iHour32);
    DDX_Text(pDX, IDC_EDITHOUR41, m_iHour41);
    DDX_Text(pDX, IDC_EDITHOUR42, m_iHour42);
    DDX_Text(pDX, IDC_EDITHOUR51, m_iHour51);
    DDX_Text(pDX, IDC_EDITHOUR52, m_iHour52);
    DDX_Text(pDX, IDC_EDITHOUR61, m_iHour61);
    DDX_Text(pDX, IDC_EDITHOUR62, m_iHour62);
    DDX_Text(pDX, IDC_EDITHOUR71, m_iHour71);
    DDX_Text(pDX, IDC_EDITHOUR72, m_iHour72);
    DDX_Text(pDX, IDC_EDITHOUR81, m_iHour81);
    DDX_Text(pDX, IDC_EDITHOUR82, m_iHour82);
    DDX_Text(pDX, IDC_EDITMIN31, m_iMin31);
    DDX_Text(pDX, IDC_EDITMIN32, m_iMin32);
    DDX_Text(pDX, IDC_EDITMIN41, m_iMin41);
    DDX_Text(pDX, IDC_EDITMIN42, m_iMin42);
    DDX_Text(pDX, IDC_EDITMIN51, m_iMin51);
    DDX_Text(pDX, IDC_EDITMIN52, m_iMin52);
    DDX_Text(pDX, IDC_EDITMIN61, m_iMin61);
    DDX_Text(pDX, IDC_EDITMIN62, m_iMin62);
    DDX_Text(pDX, IDC_EDITMIN71, m_iMin71);
    DDX_Text(pDX, IDC_EDITMIN72, m_iMin72);
    DDX_Text(pDX, IDC_EDITMIN81, m_iMin81);
    DDX_Text(pDX, IDC_EDITMIN82, m_iMin82);
    DDX_Text(pDX, IDC_EDIT_TRIGGERAREA, m_fTriggerArea);

    DDX_Control(pDX, IDC_COMBO_RULE_PARAM1, m_comboRuleParam1);
    DDX_Control(pDX, IDC_COMBO_RULE_PARAM2, m_comboRuleParam2);
    DDX_Control(pDX, IDC_EDIT_RULE_PARAM3, m_editRuleParam3);
    DDX_Control(pDX, IDC_EDIT_RULE_PARAM4, m_editRuleParam4);
    DDX_Control(pDX, IDC_EDIT_RULE_PARAM5, m_editRuleParam5);
    DDX_Control(pDX, IDC_EDIT_RULE_PARAM6, m_editRuleParam6);
    DDX_Control(pDX, IDC_STATIC_RULE_PARAM1, m_staticRuleParam1);
    DDX_Control(pDX, IDC_STATIC_RULE_PARAM2, m_staticRuleParam2);
    DDX_Control(pDX, IDC_STATIC_RULE_PARAM3, m_staticRuleParam3);
    DDX_Control(pDX, IDC_STATIC_RULE_PARAM4, m_staticRuleParam4);
    DDX_Control(pDX, IDC_STATIC_RULE_PARAM5, m_staticRuleParam5);
    DDX_Control(pDX, IDC_STATIC_RULE_PARAM6, m_staticRuleParam6);

    DDX_Text(pDX, IDC_EDIT_RULE_PARAM3, m_iRuleParam3);
    DDX_Text(pDX, IDC_EDIT_RULE_PARAM4, m_iRuleParam4);
    DDX_Text(pDX, IDC_EDIT_RULE_PARAM5, m_iRuleParam5);
    DDX_Text(pDX, IDC_EDIT_RULE_PARAM6, m_iRuleParam6);
    DDX_Control(pDX, IDC_COMBO_CHANNEL_LIST, m_cmbVCAChannelID);
    DDX_Control(pDX, IDC_COMBO_CHANNEL_GROUP_LIST, m_cmbVCAChannelGroupNo);

    //}}AFX_DATA_MAP


    DDX_Control(pDX, IDC_COMBO_RESOLUTION, m_comboResolution);
    DDX_Control(pDX, IDC_COMBO_RULE_PRIORITY, m_combEventPriority);
    DDX_Control(pDX, IDC_LIST_ALARM_OUT, m_listAlarmOut);
}

/*********************************************************
Function:	BEGIN_MESSAGE_MAP
Desc:		the map between control and function
Input:	
Output:	
Return:	
**********************************************************/
BEGIN_MESSAGE_MAP(CDlgVcaRuleCfgEx, CDialog)
//{{AFX_MSG_MAP(CDlgVcaRuleCfgEx)
    ON_BN_CLICKED(IDC_BTN_SET_ONE_RULE, OnBtnSetOneRule)
    ON_CBN_SELCHANGE(IDC_COMBO_RULE_ID, OnSelchangeComboRuleId)
    ON_BN_CLICKED(IDC_CHK_RULE_ACTIVE, OnChkRuleActive)
    ON_CBN_SELCHANGE(IDC_COMBO_RULE_TYPE, OnSelchangeComboRuleType)
    ON_BN_CLICKED(IDC_CHK_DRAW_POLYGON, OnChkDrawPolygon)
    ON_BN_CLICKED(IDC_CHECK_FILTER_ACTIVE, OnCheckFilterActive)
    ON_BN_CLICKED(IDC_CHECK_SET_MINI_FILTER, OnCheckSetMiniFilter)
    ON_BN_CLICKED(IDC_CHECK_SET_MAX_FILTER, OnCheckSetMaxFilter)
    ON_BN_CLICKED(IDC_CHK_DRAW_LINE, OnChkDrawLine)
    ON_BN_CLICKED(IDC_TIME_OK, OnTimeOk)
    ON_BN_CLICKED(IDC_TIME_COPY, OnTimeCopy)
    ON_CBN_SELCHANGE(IDC_COMBOWEEKDAY, OnSelchangeComboweekday)
    ON_BN_CLICKED(IDC_CHECKHANDLETYPE4, OnCheckhandletype4)
    ON_EN_CHANGE(IDC_EDIT_RULE_NAME, OnChangeEditRuleName)
	ON_CBN_SELCHANGE(IDC_COMBO_FILTER_MODE, OnSelchangeComboFilterMode)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO_TRIGGERMODE, OnSelchangeComboTriggermode)
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BTN_SAVE_ONE_RULE, &CDlgVcaRuleCfgEx::OnBnClickedBtnSaveOneRule)
    ON_BN_CLICKED(IDCANCEL, &CDlgVcaRuleCfgEx::OnBnClickedCancel)
    ON_BN_CLICKED(ID_BTN_GET_RULE_CFG, &CDlgVcaRuleCfgEx::OnBnClickedBtnGetRuleCfg)
    ON_CBN_SELCHANGE(IDC_COMBO_CHANNEL_LIST, &CDlgVcaRuleCfgEx::OnCbnSelchangeComboChannelList)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgVcaRuleCfgEx message handlers

BOOL CDlgVcaRuleCfgEx::PreTranslateMessage(MSG* pMsg) 
{
    // TODO: Add your specialized code here and/or call the base class
    CPoint pt(0,0);
    GetDlgItem(IDC_PLAYWND)->GetWindowRect(&m_rcWnd);
    GetCursorPos(&pt);
    CString strTemp = _T("");
    DWORD i=0;
    BOOL bOneLine = TRUE,bXOneLine = TRUE,bYOneLine = TRUE;
    
    switch(pMsg->message)  // Message of Draw
    {
    case WM_LBUTTONDOWN:
        if( (m_bDrawLine || m_bDrawPolygon)&&
            (GetAsyncKeyState(VK_LCONTROL)<0 || GetAsyncKeyState(VK_RCONTROL)<0))//Press CTRL to draw
        {
            if (m_bDrawPolygon)// Draw polygon
            {
                if(PtInRect(&m_rcWnd,pt))
                {	
                    if(m_bNeedRedraw[m_iCurRuleID])
                    {			
                        if(m_bCloseIn[m_iCurRuleID])
                        {
                            m_dwPosNum = 0;
                            m_struVcaPolygon[m_iCurRuleID].dwPointNum = m_dwPosNum;		
                        }
                        if(m_dwPosNum>9)
                        {
                            char szLan[128] = {0};
                            g_StringLanType(szLan,"多边形顶点不能超过10个!","Apexes of polygon should less than 11");
                            AfxMessageBox(szLan);
                            return TRUE;
                        }
                        if((float)(pt.x-m_rcWnd.left)/(float)m_rcWnd.Width() <= m_struVcaPolygon[m_iCurRuleID].struPos[m_dwPosNum-1].fX+0.01 && (float)(pt.x-m_rcWnd.left)/(float)m_rcWnd.Width() >= m_struVcaPolygon[m_iCurRuleID].struPos[m_dwPosNum-1].fX-0.01\
                            && (float)(pt.y-m_rcWnd.top)/(float)m_rcWnd.Height() <= m_struVcaPolygon[m_iCurRuleID].struPos[m_dwPosNum-1].fY+0.01 && (float)(pt.y-m_rcWnd.top)/(float)m_rcWnd.Height() >= m_struVcaPolygon[m_iCurRuleID].struPos[m_dwPosNum-1].fY-0.01)
                        {
                            char szLan[128] = {0};
                            g_StringLanType(szLan, "不能连续在同一点上画区域", "Can't draw region on the point used");
                            AfxMessageBox(szLan);
                            return TRUE;
                        }
                        m_bCloseIn[m_iCurRuleID] = FALSE;
                        m_bMouseMove[m_iCurRuleID] = FALSE;
                        m_struVcaPolygon[m_iCurRuleID].struPos[m_dwPosNum].fX = (float)(pt.x-m_rcWnd.left)/(float)m_rcWnd.Width();
                        m_struVcaPolygon[m_iCurRuleID].struPos[m_dwPosNum].fY = (float)(pt.y-m_rcWnd.top)/(float)m_rcWnd.Height();
                        m_dwPosNum++;
                        m_struVcaPolygon[m_iCurRuleID].dwPointNum = m_dwPosNum;			
                    }
                    else
                    {
                        return TRUE;
                    }
                }
            }
            if (m_bDrawLine) 
            {
                if(PtInRect(&m_rcWnd,pt))
                {		
                    if(!m_bMouseMove[m_iCurRuleID])
                    {
                        m_struVcaLine[m_iCurRuleID].struStart.fX = (float)(pt.x-m_rcWnd.left)/(float)m_rcWnd.Width();
                        m_struVcaLine[m_iCurRuleID].struStart.fY = (float)(pt.y-m_rcWnd.top)/(float)m_rcWnd.Height();		
                        m_struVcaLine[m_iCurRuleID].struEnd.fX = (float)(pt.x-m_rcWnd.left)/(float)m_rcWnd.Width();
                        m_struVcaLine[m_iCurRuleID].struEnd.fY = (float)(pt.y-m_rcWnd.top)/(float)m_rcWnd.Height();			
                        m_bMouseMove[m_iCurRuleID] = TRUE;
                    }
                    else
                    {
                        if ((float)(pt.x-m_rcWnd.left)/(float)m_rcWnd.Width() <= m_struVcaLine[m_iCurRuleID].struStart.fX+0.01 && (float)(pt.x-m_rcWnd.left)/(float)m_rcWnd.Width() >= m_struVcaLine[m_iCurRuleID].struStart.fX-0.01\
                            && (float)(pt.y-m_rcWnd.top)/(float)m_rcWnd.Height() <= m_struVcaLine[m_iCurRuleID].struStart.fY+0.01 && (float)(pt.y-m_rcWnd.top)/(float)m_rcWnd.Height() >= m_struVcaLine[m_iCurRuleID].struStart.fY-0.01)
                        {
                            char szLan[128] = {0};
                            g_StringLanType(szLan, "不能连续在同一点上画区域", "Can't draw region on the point used");
                            AfxMessageBox(szLan);
                            return TRUE;
                        }
                        m_struVcaLine[m_iCurRuleID].struEnd.fX = (float)(pt.x-m_rcWnd.left)/(float)m_rcWnd.Width();
                        m_struVcaLine[m_iCurRuleID].struEnd.fY = (float)(pt.y-m_rcWnd.top)/(float)m_rcWnd.Height();			
                        m_bMouseMove[m_iCurRuleID] = FALSE;
                    }
                    
                }
                else
                {
                    return TRUE;
                }
            }
            
        }//First coordinate of Minimal Rectangle
        else if(m_bSetMiniFilter && (GetAsyncKeyState(VK_LCONTROL)<0 || GetAsyncKeyState(VK_RCONTROL)<0))
        {
            if(PtInRect(&m_rcWnd,pt))
            {
                m_struSizeFilter.struMiniRect.fX = (float)(pt.x-m_rcWnd.left)/(float)m_rcWnd.Width();
                m_struSizeFilter.struMiniRect.fY = (float)(pt.y-m_rcWnd.top)/(float)m_rcWnd.Height();
            }
        }//First coordinate of Maximal Rectangle
        else if(m_bSetMaxFilter && (GetAsyncKeyState(VK_LCONTROL)<0 || GetAsyncKeyState(VK_RCONTROL)<0))
        {
            if(PtInRect(&m_rcWnd,pt))
            {
                m_struSizeFilter.struMaxRect.fX = (float)(pt.x-m_rcWnd.left)/(float)m_rcWnd.Width();
                m_struSizeFilter.struMaxRect.fY = (float)(pt.y-m_rcWnd.top)/(float)m_rcWnd.Height();
            }
        }
        break;
    case WM_MOUSEMOVE:
        if((m_bDrawLine || m_bDrawPolygon)&&(GetAsyncKeyState(VK_LCONTROL)<0 || GetAsyncKeyState(VK_RCONTROL)<0))
        {
            if (m_bDrawPolygon)
            {
                if(PtInRect(&m_rcWnd,pt))
                {
                    if(m_dwPosNum > 9)
                    {
                        return TRUE;
                    }
                    if( m_bNeedRedraw[m_iCurRuleID] && !m_bCloseIn[m_iCurRuleID])
                    {
                        m_bMouseMove[m_iCurRuleID] = TRUE;
                        m_struVcaPolygon[m_iCurRuleID].struPos[m_dwPosNum].fX = (float)(pt.x-m_rcWnd.left)/(float)m_rcWnd.Width();
                        m_struVcaPolygon[m_iCurRuleID].struPos[m_dwPosNum].fY = (float)(pt.y-m_rcWnd.top)/(float)m_rcWnd.Height();
                        m_struVcaPolygon[m_iCurRuleID].dwPointNum = m_dwPosNum+1;				
                    }
                }
            }
            
            if(m_bDrawLine)
            {
                if(PtInRect(&m_rcWnd,pt))
                {
                    if(m_bMouseMove)
                    {
                        m_struVcaLine[m_iCurRuleID].struEnd.fX = (float)(pt.x-m_rcWnd.left)/(float)m_rcWnd.Width();
                        m_struVcaLine[m_iCurRuleID].struEnd.fY = (float)(pt.y-m_rcWnd.top)/(float)m_rcWnd.Height();			
                    }
                }
            }
        }
        else if(m_bSetMiniFilter && (GetAsyncKeyState(VK_LCONTROL)<0 || GetAsyncKeyState(VK_RCONTROL)<0)) //尺寸过滤器
        {
            if(PtInRect(&m_rcWnd,pt))
            {
                m_struSizeFilter.struMiniRect.fWidth = (float)(pt.x-m_rcWnd.left)/(float)m_rcWnd.Width() - m_struSizeFilter.struMiniRect.fX;
                m_struSizeFilter.struMiniRect.fHeight = (float)(pt.y-m_rcWnd.top)/(float)m_rcWnd.Height() - m_struSizeFilter.struMiniRect.fY;
            }
            
        }
        else if(m_bSetMaxFilter && (GetAsyncKeyState(VK_LCONTROL)<0 || GetAsyncKeyState(VK_RCONTROL)<0)) //尺寸过滤器
        {
            if(PtInRect(&m_rcWnd,pt))
            {	
                m_struSizeFilter.struMaxRect.fWidth = (float)(pt.x-m_rcWnd.left)/(float)m_rcWnd.Width() - m_struSizeFilter.struMaxRect.fX;
                m_struSizeFilter.struMaxRect.fHeight = (float)(pt.y-m_rcWnd.top)/(float)m_rcWnd.Height() - m_struSizeFilter.struMaxRect.fY;
            }
        }
        break;
    case WM_RBUTTONDOWN://Click right mouse to close the rectangle. 
        if(m_bDrawPolygon && (GetAsyncKeyState(VK_LCONTROL)<0 || GetAsyncKeyState(VK_RCONTROL)<0))
        {
            if(PtInRect(&m_rcWnd,pt))
            {
                if(m_dwPosNum > 9)
                {
                    return TRUE;
                }
                for(i=0; i<m_dwPosNum; i++)
                {
                    if(m_struVcaPolygon[m_iCurRuleID].struPos[i].fX != m_struVcaPolygon[m_iCurRuleID].struPos[i+1].fX)
                    {
                        bXOneLine = FALSE;
                        break;
                    }
                }
                for(i=0; i<m_dwPosNum; i++)
                {
                    if(m_struVcaPolygon[m_iCurRuleID].struPos[i].fY != m_struVcaPolygon[m_iCurRuleID].struPos[i+1].fY)
                    {
                        bYOneLine = FALSE;
                        break;
                    }
                }
                bOneLine = bXOneLine||bYOneLine;
                if(bOneLine)
                {
                    char szlan[128] = {0};
                    g_StringLanType(szlan, "所画点成一线，无法构成区域!","The points is on a line and can't create a rectangle!");
                    AfxMessageBox(szlan);
                    return TRUE;
                }
                if( m_bNeedRedraw[m_iCurRuleID] && !m_bCloseIn[m_iCurRuleID])
                {
                    if(m_bMouseMove[m_iCurRuleID])
                    {
                        m_bMouseMove[m_iCurRuleID] = FALSE;
                        m_struVcaPolygon[m_iCurRuleID].dwPointNum--;
                        m_struVcaPolygon[m_iCurRuleID].struPos[m_struVcaPolygon[m_iCurRuleID].dwPointNum].fX = 0;
                        m_struVcaPolygon[m_iCurRuleID].struPos[m_struVcaPolygon[m_iCurRuleID].dwPointNum].fY = 0;
                    }
                    m_bCloseIn[m_iCurRuleID] = TRUE;
                }
            }
        }
        else if(m_bDrawPolygon)
        {
            if(PtInRect(&m_rcWnd,pt))
            {
                if(m_dwPosNum == 0)
                {
                    return TRUE;
                }
                if( m_bNeedRedraw[m_iCurRuleID] && !m_bCloseIn[m_iCurRuleID])
                {
                    m_struVcaPolygon[m_iCurRuleID].dwPointNum--;
                    m_dwPosNum--;
                    m_struVcaPolygon[m_iCurRuleID].struPos[m_struVcaPolygon[m_iCurRuleID].dwPointNum].fX = 0;
                    m_struVcaPolygon[m_iCurRuleID].struPos[m_struVcaPolygon[m_iCurRuleID].dwPointNum].fY = 0;
                }
                else if( m_bNeedRedraw[m_iCurRuleID] && m_bCloseIn[m_iCurRuleID])
                {
                    memset(&m_struVcaPolygon[m_iCurRuleID], 0, sizeof(m_struVcaPolygon[m_iCurRuleID]));
                    m_dwPosNum = 0;
                    m_bCloseIn[m_iCurRuleID] = FALSE;
                }
            }
        }
        break;
    default:
        break;
    }
    return CDialog::PreTranslateMessage(pMsg);
}

/*********************************************************
Function:	OnInitDialog
Desc:		 
Input:	
Output:	
Return:	
**********************************************************/
BOOL CDlgVcaRuleCfgEx::OnInitDialog() 
{
    CDialog::OnInitDialog();
    // TODO: Add extra initialization here

    // add channel Group + channel No.
    g_AddChanInfoToComBox(m_cmbVCAChannelID, m_iDevIndex, TRUE);

    m_cmbVCAChannelGroupNo.ResetContent();
    DWORD dwGroupNo = 0;
    if (g_struDeviceInfo[m_iDevIndex].iDeviceChanNum % MAX_CHANNUM_V30 != 0)
    {
        dwGroupNo = g_struDeviceInfo[m_iDevIndex].iDeviceChanNum / MAX_CHANNUM_V30 + 1;
    }
    else
    {
        dwGroupNo = g_struDeviceInfo[m_iDevIndex].iDeviceChanNum / MAX_CHANNUM_V30;
    }

    //数字报警通道号是从33开始，前32个预留给模拟报警，所以第一组是33-64，第二组是65-128，这里加1组
    char szLan[128] = { 0 };
    for (int i = 0; i < dwGroupNo + 1; i++)
    {
        sprintf(szLan, "The %dth Group", i + 1);
        m_cmbVCAChannelGroupNo.AddString(szLan);
    }

    m_cmbVCAChannelGroupNo.SetCurSel(0);

    
    CRect rect;
    GetDlgItem(IDC_PLAYWND)->GetWindowRect(&rect);
    ScreenToClient(&rect);
    int nWidth = (int)((rect.Height()*352)/288);
    rect.left  += (long)fabs((long double)(rect.right - rect.left - nWidth))/2;
    rect.right = rect.left + nWidth;
    GetDlgItem(IDC_PLAYWND)->MoveWindow(rect);
    
    g_pDlgVcaRuleCfgEx = this; // 

    StartPlay();

    GetVcaParam();
    
    InitWnd();
   
    m_comboRuleID.SetCurSel(0);  
    m_iCurRuleID = m_comboRuleID.GetCurSel();
    m_comboWeekday.SetCurSel(0);

//
    UpdateData(FALSE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgVcaRuleCfgEx::InitWnd()
{   
	int i = 0;
    char szLan[128] = {0};
    for (i = 0; i<MAX_RULE_NUM_V42; i++)
    {
        m_bNeedRedraw[i] = FALSE;//Redraw polygon
        m_bCloseIn[i] = TRUE;//Polygon is closed or not
    }
    
	m_comboRelSnapChan1.ResetContent();
	m_comboRelSnapChan2.ResetContent();
	m_comboRelSnapChan3.ResetContent();

	int nIndex = 0;
	for (i = g_struDeviceInfo[m_iDevIndex].iStartChan; i  <= g_struDeviceInfo[m_iDevIndex].iAnalogChanNum ; i++)
	{
		sprintf(szLan, "Analog Chan%d", i);
		m_comboRelSnapChan1.InsertString(nIndex,szLan);
		m_comboRelSnapChan2.InsertString(nIndex,szLan);
		m_comboRelSnapChan3.InsertString(nIndex,szLan);
		m_comboRelSnapChan1.SetItemData(nIndex, i);
		m_comboRelSnapChan2.SetItemData(nIndex, i);
		m_comboRelSnapChan3.SetItemData(nIndex, i);
		nIndex++;
	}
    
    for (i = 0; i  < g_struDeviceInfo[m_iDevIndex].pStruIPParaCfgV40->dwDChanNum ; i++)
	{
		sprintf(szLan, "IP Chan%d", i + 1);
		m_comboRelSnapChan1.InsertString(nIndex,szLan);
		m_comboRelSnapChan2.InsertString(nIndex,szLan);
		m_comboRelSnapChan3.InsertString(nIndex,szLan);
		m_comboRelSnapChan1.SetItemData(nIndex, i + g_struDeviceInfo[m_iDevIndex].pStruIPParaCfgV40->dwStartDChan);
		m_comboRelSnapChan2.SetItemData(nIndex, i + g_struDeviceInfo[m_iDevIndex].pStruIPParaCfgV40->dwStartDChan);
		m_comboRelSnapChan3.SetItemData(nIndex, i + g_struDeviceInfo[m_iDevIndex].pStruIPParaCfgV40->dwStartDChan);
		nIndex++;
	}


	g_StringLanType(szLan,"不使用","None");
	m_comboRelSnapChan1.InsertString(0,szLan);
	m_comboRelSnapChan2.InsertString(0,szLan);
	m_comboRelSnapChan3.InsertString(0,szLan);
	m_comboRelSnapChan1.SetItemData(0,0);
	m_comboRelSnapChan2.SetItemData(0,0);
	m_comboRelSnapChan3.SetItemData(0,0);
	m_comboRelSnapChan1.SetCurSel(0);
	m_comboRelSnapChan2.SetCurSel(0);
	m_comboRelSnapChan3.SetCurSel(0);

    InitRuleType(m_struBehaviorAbility.dwAbilityType, m_struBehaviorAbility.dwAbilityTypeEx);
    InitRuleID(m_struBehaviorAbility.byMaxRuleNum);

    nIndex = 0;
    m_comboFilterMode.ResetContent();
    g_StringLanType(szLan, "像素模式","Pix Mode");
    m_comboFilterMode.InsertString(nIndex, szLan);
    m_comboFilterMode.SetItemData(nIndex, IMAGE_PIX_MODE);
    nIndex++;

    g_StringLanType(szLan, "实际大小模式","Real Mode");
    m_comboFilterMode.InsertString(nIndex, szLan);
    m_comboFilterMode.SetItemData(nIndex, REAL_WORLD_MODE);
    nIndex++;

    g_StringLanType(szLan, "默认模式","Default Mode");
    m_comboFilterMode.InsertString(nIndex, szLan);
    m_comboFilterMode.SetItemData(nIndex, DEFAULT_MODE);
    nIndex++;

    g_StringLanType(szLan, "不启用","Disable");
	m_comboTriggerMode.InsertString(0,szLan);
	m_comboTriggerMode.SetItemData(0,TRIGGER_MODE_NONE);
	
    g_StringLanType(szLan, "轨迹点","Track point");
	m_comboTriggerMode.InsertString(1,szLan);
	m_comboTriggerMode.SetItemData(1,TRIGGER_MODE_TRACKPOINT);
	
	g_StringLanType(szLan, "目标面积","Target area");
	m_comboTriggerMode.InsertString(2,szLan);
	m_comboTriggerMode.SetItemData(2,TRIGGER_MODE_TARGETAREA);

	g_StringLanType(szLan, "中","Center");
	m_comboTriggerPoint.InsertString(0,szLan);
	m_comboTriggerPoint.SetItemData(0,TRIGGER_POINT_CENTER);

	g_StringLanType(szLan, "上","Up");
	m_comboTriggerPoint.InsertString(1,szLan);
	m_comboTriggerPoint.SetItemData(1,TRIGGER_POINT_UP);

	g_StringLanType(szLan, "下","Down");
	m_comboTriggerPoint.InsertString(2,szLan);
	m_comboTriggerPoint.SetItemData(2,TRIGGER_POINT_DOWN);

	g_StringLanType(szLan, "不启用","Disable");
	m_comboFilterStrategy.InsertString(0,szLan);
	m_comboFilterStrategy.SetItemData(0,FILTER_STRATEGY_NONE);


	g_StringLanType(szLan, "宽度和高度","Width and height");
	m_comboFilterStrategy.InsertString(1,szLan);
	m_comboFilterStrategy.SetItemData(1,FILTER_STRATEGY_WIDTH_AND_HEIGHT);

	g_StringLanType(szLan, "面积过滤","Area");
	m_comboFilterStrategy.InsertString(2,szLan);
	m_comboFilterStrategy.SetItemData(2,FILTER_STRATEGY_AERA);
    
	m_comboTriggerMode.SetCurSel(0);
	m_comboTriggerPoint.SetCurSel(0);
	m_comboFilterStrategy.SetCurSel(0);
}


BOOL CDlgVcaRuleCfgEx::GetVcaParam()
{
//     if (!GetVcaCtrlCfg())
//     {
//         AfxMessageBox("Fail to get vca ctrl");
//         return FALSE;
//     }
   
//     m_struVcaChanInParam.byVCAType = m_struVcaCtrlCfg.struCtrlInfo[m_iVcaChannel -1].byVCAType;
//     if (m_struVcaChanInParam.byVCAType == VCA_ATM)
//     {
//         m_struVcaChanInParam.byMode = m_struVcaCtrlCfg.struCtrlInfo[m_iVcaChannel -1].byMode;
//     }

	m_struVcaChanInParam.byVCAType = m_byVCAType;
	if (m_byVCAType == VCA_ATM)
	{
		m_struVcaChanInParam.byMode = m_byMode;
	}
    
    if (!GetVcaChanAbility())
    {
        AfxMessageBox("Fail to get vca chan ability");
        return FALSE;
    }

    if (!GetVcaRuleCfg())
    {
        AfxMessageBox("Fail to get vca rule config");
        return FALSE;
    }

    //获取DEVICE_ABILITY_INFO中VcaChanAbility
    if (!GetVcaChanAbilityXml())
    {
        AfxMessageBox("Fail to get vca chan ability xml");
        return FALSE;
    }
    return TRUE;
}

BOOL CDlgVcaRuleCfgEx::GetVcaCtrlCfg()
{
    BOOL bRet = FALSE;
    DWORD dwReturn = 0;
    if(!NET_DVR_GetDVRConfig(m_lServerID, NET_DVR_GET_VCA_CTRLCFG, -1, &m_struVcaCtrlCfg, sizeof(m_struVcaCtrlCfg), &dwReturn))
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_GET_VCA_CTRLCFG");
        bRet = FALSE;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_GET_VCA_CTRLCFG");
        bRet = TRUE;
    }
    return bRet;
}

BOOL CDlgVcaRuleCfgEx::GetVcaChanAbility()
{
    BOOL bRet = FALSE; 
    if (!NET_DVR_GetDeviceAbility(m_lServerID, VCA_CHAN_ABILITY, (char*)&m_struVcaChanInParam,\
        sizeof(m_struVcaChanInParam), (char*)&m_struBehaviorAbility, sizeof(m_struBehaviorAbility)))
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "VCA_CHAN_ABILITY");
        bRet = FALSE;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "VCA_CHAN_ABILITY");
        bRet = TRUE;
    }

    return bRet;
}

BOOL CDlgVcaRuleCfgEx::GetVcaChanAbilityXml()
{
    BOOL bRet = FALSE;
    char  pInBuffer[256] = "";
    char m_szOutBuffer[1024 * 10] = "";
    int iCurChannel = m_cmbVCAChannelID.GetItemData(m_cmbVCAChannelID.GetCurSel());
    iCurChannel = NET_DVR_SDKChannelToISAPI(m_lServerID, iCurChannel, TRUE);
    sprintf(pInBuffer, "<?xml version=\"1.0\" encoding=\"utf-8\"?> <VcaChanAbility version = \"2.0\"> \
                                      <channelNO>%d</channelNO></VcaChanAbility>", iCurChannel);
    if (!NET_DVR_GetDeviceAbility(m_lServerID, DEVICE_ABILITY_INFO, pInBuffer, \
        sizeof(pInBuffer), m_szOutBuffer, sizeof(m_szOutBuffer)))
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "DEVICE_ABILITY_INFO");
        bRet = FALSE;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "DEVICE_ABILITY_INFO");
        memset(&m_struVcaChanAbilityXml, 0, sizeof(m_struVcaChanAbilityXml));
        ALARM_PIC_RESOLUTION struAlarmPicRes = { 0 };
        int iResCount = 0;
        //读取xml的内容
        CXmlBase xmlBase;
        xmlBase.Parse(m_szOutBuffer);
        if (xmlBase.FindElem("VcaChanAbility") && xmlBase.IntoElem())
        {
            if (xmlBase.FindElem("AlarmPicResolutionList") && xmlBase.IntoElem())
            {
                do
                {
                    struAlarmPicRes.iIndex = std::stoi(xmlBase.GetAttributeValue("index"));
                    memcpy(struAlarmPicRes.szName, xmlBase.GetAttributeValue("index").c_str(), xmlBase.GetAttributeValue("index").length());
                    memcpy(struAlarmPicRes.szResolution, xmlBase.GetAttributeValue("resolution").c_str(), xmlBase.GetAttributeValue("resolution").length());
                    memcpy(&m_struVcaChanAbilityXml.struAlarmPicResolutionList[iResCount],& struAlarmPicRes, sizeof(struAlarmPicRes));
                    iResCount++;
                    m_struVcaChanAbilityXml.iAlarmPicResolutionListLen = iResCount;
                } while (xmlBase.NextSibElem() && xmlBase.FindElem("picResolutionEntry"));
            }
        }

        bRet = TRUE;
    }
    return bRet;
}

BOOL CDlgVcaRuleCfgEx::GetVcaRuleCfg()
{
    BOOL bRet = FALSE;
    DWORD dwReturn = 0;
    memset(&m_struVcaRule.m_struRuleCfg, 0, sizeof(m_struVcaRule.m_struRuleCfg));
    if (!NET_DVR_GetDeviceConfig(m_lServerID, NET_DVR_GET_RULECFG_V42, 1, &m_struChanGroup, sizeof(m_struChanGroup), \
        &dwReturn, &m_struVcaRule.m_struRuleCfg, sizeof(m_struVcaRule.m_struRuleCfg)))
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_GET_RULECFG_V42 Chan Group[%d] chanNo[%d]", \
            m_struChanGroup.dwGroup, m_struChanGroup.dwChannel);
        bRet = FALSE;
    }
    else
    {
        if (dwReturn == NET_DVR_NOERROR)
        {
            g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_GET_RULECFG_V42 Chan Group[%d] chanNo[%d]",
                m_struChanGroup.dwGroup, m_struChanGroup.dwChannel);
            bRet = TRUE;
        }
        else
        {
            g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_GET_RULECFG_V42 Chan Group[%d] chanNo[%d]", \
                m_struChanGroup.dwGroup, m_struChanGroup.dwChannel);
            bRet = FALSE;
        }
    }
    return bRet;
}

BOOL CDlgVcaRuleCfgEx::SetVcaRuleCfg()
{
    BOOL bRet= FALSE;

    DWORD dwRetList = NET_DVR_NOERROR;

    bRet = NET_DVR_SetDeviceConfig(m_lServerID, NET_DVR_SET_RULECFG_V42, 1, &m_struChanGroup, sizeof(m_struChanGroup),
        &dwRetList, &m_struVcaRule.m_struRuleCfg, sizeof(m_struVcaRule.m_struRuleCfg));
    if(!bRet)
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_SET_RULECFG_V42 ChanGroup[%d ] ChanNo[%d]", m_struChanGroup.dwGroup, m_struChanGroup.dwChannel);
    }
    else
    {
        if (dwRetList == NET_DVR_NOERROR)
        {
            g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_SET_RULECFG_V42 ChanGroup[%d ] ChanNo[%d]", m_struChanGroup.dwGroup, m_struChanGroup.dwChannel);
            bRet = TRUE;
        }
        else
        {
            char szLan[128] = "";
            sprintf(szLan, "NET_DVR_SET_RULECFG_V42 Error[%d]", dwRetList);
            AfxMessageBox(szLan);
        }
    }
    return bRet;
}

/*********************************************************
Function:	OnBtnSetOneRule
Desc:		
Input:	
Output:	
Return:	void
**********************************************************/
void CDlgVcaRuleCfgEx::OnBtnSetOneRule() 
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    GetGroupInfoFromWnd();
    //获取最后一次的规则配置
    GetUploadCfgFromWnd();
    GetOneVcaRuleFromWnd(m_struVcaRule.m_struRuleCfg.struRule[m_iCurRuleID]);
    GetPtzPositionFromWnd();

    char szLan[128] = {0};
    if (!SetVcaRuleCfg())
    {
        sprintf(szLan, "Fail to Set Channel Group[%d]  ChanNo[%d] VCA behavior rule", \
                        m_struChanGroup.dwGroup,m_struChanGroup.dwChannel);
        AfxMessageBox(szLan);
    }
    else
    {
        sprintf(szLan, "Success to Set Channel Group[%d]  ChanNo[%d] VCA behavior rule", \
            m_struChanGroup.dwGroup, m_struChanGroup.dwChannel);
        AfxMessageBox(szLan);
    }
}




/*********************************************************
Function:	InsertRuleType
Desc:	Insert Ability type in m_comboRuleType and m_comboRuleTypeEx. 
Input:	int iBehaviorAbilityType
Output:	
Return:	BOOL 
**********************************************************/
BOOL CDlgVcaRuleCfgEx::InitRuleType(int iBehaviorAbilityType, int iBehaviorAbilityTypeEx)
{
    m_comboRuleType.ResetContent();
    char szLan[128] = {0};
    int index = 0;
    if((iBehaviorAbilityType&TRAVERSE_PLANE_ABILITY) != 0)
    {
        g_StringLanType(szLan, "穿越警戒面", "Traverse Plane");
        m_comboRuleType.InsertString(index, szLan);
        m_comboRuleType.SetItemData(index, ENUM_VCA_EVENT_TRAVERSE_PLANE);
        index++;
    }
    if((iBehaviorAbilityType&ENTER_AREA_ABILITY) != 0)
    {
        g_StringLanType(szLan, "进入区域", "Enter Area");
        m_comboRuleType.InsertString(index, szLan);
        m_comboRuleType.SetItemData(index, ENUM_VCA_EVENT_ENTER_AREA);
        index++;
    }
    if((iBehaviorAbilityType&EXIT_AREA_ABILITY) != 0)
    {
        g_StringLanType(szLan, "离开区域", "Exit Area");
        m_comboRuleType.InsertString(index, szLan);
        m_comboRuleType.SetItemData(index, ENUM_VCA_EVENT_EXIT_AREA);
        index++;
    }
    if((iBehaviorAbilityType&INTRUSION_ABILITY) != 0)
    {
        g_StringLanType(szLan, "入侵", "Intrusion");
        m_comboRuleType.InsertString(index, szLan);
        m_comboRuleType.SetItemData(index, ENUM_VCA_EVENT_INTRUSION);
        index++;
    }
    if((iBehaviorAbilityType&LOITER_ABILITY) != 0)
    {
        g_StringLanType(szLan, "徘徊", "Loiter");
        m_comboRuleType.InsertString(index, szLan);
        m_comboRuleType.SetItemData(index, ENUM_VCA_EVENT_LOITER);
        index++;
    }
    if((iBehaviorAbilityType&LEFT_TAKE_ABILITY) != 0)
    {
        g_StringLanType(szLan, "物品遗留拿取", "Left and Take");
        m_comboRuleType.InsertString(index, szLan);
        m_comboRuleType.SetItemData(index, ENUM_VCA_EVENT_LEFT_TAKE);
        index++;
    }
    if((iBehaviorAbilityType&PARKING_ABILITY) != 0)
    {
        g_StringLanType(szLan, "停车", "Parking");
        m_comboRuleType.InsertString(index, szLan);
        m_comboRuleType.SetItemData(index, ENUM_VCA_EVENT_PARKING);
        index++;
    }
    if((iBehaviorAbilityType&RUN_ABILITY) != 0)
    {
        g_StringLanType(szLan, "快速移动", "Running");
        m_comboRuleType.InsertString(index, szLan);
        m_comboRuleType.SetItemData(index, ENUM_VCA_EVENT_RUN);
        index++;
    }
    if((iBehaviorAbilityType&HIGH_DENSITY_ABILITY) != 0)
    {
        g_StringLanType(szLan, "人员聚集", "Staff Density");
        m_comboRuleType.InsertString(index, szLan);
        m_comboRuleType.SetItemData(index, ENUM_VCA_EVENT_HIGH_DENSITY);
        index++;
    }
    if ((iBehaviorAbilityType & VIOLENT_MOTION_ABILITY) != 0)
    {
        g_StringLanType(szLan, "剧烈运动", "Violent motion");
        m_comboRuleType.InsertString(index, szLan);
        m_comboRuleType.SetItemData(index, ENUM_VCA_EVENT_VIOLENT_MOTION);
        index++;
    }
    
    if ((iBehaviorAbilityType & REACH_HIGHT_ABILITY) != 0)
    {
        g_StringLanType(szLan, "攀高检测", "Reach hight");
        m_comboRuleType.InsertString(index, szLan);
        m_comboRuleType.SetItemData(index, ENUM_VCA_EVENT_REACH_HIGHT);
        index++;
    }

    if ((iBehaviorAbilityType & GET_UP_ABILITY) != 0)
    {
        g_StringLanType(szLan, "起身检测", "Get up");
        m_comboRuleType.InsertString(index, szLan);
        m_comboRuleType.SetItemData(index, ENUM_VCA_EVENT_GET_UP);
        index++;
    }
    
	if ((iBehaviorAbilityType & KEY_PERSON_GET_UP_ABILITY) != 0)
    {
        g_StringLanType(szLan, "重点目标起身检测", "Get up");
        m_comboRuleType.InsertString(index, szLan);
        m_comboRuleType.SetItemData(index, ENUM_VCA_EVENT_KEY_PERSON_GET_UP);
        index++;
    }

    if (iBehaviorAbilityType & LEFT_ABILITY)
    {
        g_StringLanType(szLan, "物品遗留", "Left");
        m_comboRuleType.InsertString(index, szLan);
        m_comboRuleType.SetItemData(index, ENUM_VCA_EVENT_LEFT);
        index++;
    }

    if (iBehaviorAbilityType & TAKE_ABILITY)
    {
        g_StringLanType(szLan, "物品拿取", "Take");
        m_comboRuleType.InsertString(index, szLan);
        m_comboRuleType.SetItemData(index, ENUM_VCA_EVENT_TAKE);
        index++;
    }

	if (iBehaviorAbilityType & LEAVE_POSITION)
	{
		g_StringLanType(szLan, "岗位状态", "Leave Position");
        m_comboRuleType.InsertString(index, szLan);
        m_comboRuleType.SetItemData(index, ENUM_VCA_EVENT_LEAVE_POSITION);
        index++;
	}

	if (iBehaviorAbilityType & TRAIL_ABILITY)
    {
        g_StringLanType(szLan, "尾随", "Trail");
        m_comboRuleType.InsertString(index, szLan);
        m_comboRuleType.SetItemData(index, ENUM_VCA_EVENT_TRAIL);
        index++;
    }
	
	if (iBehaviorAbilityType & STANDUP_ABILITY)
    {
        g_StringLanType(szLan, "起立", "Stand Up");
        m_comboRuleType.InsertString(index, szLan);
        m_comboRuleType.SetItemData(index, ENUM_VCA_EVENT_STANDUP);
        index++;
    }
	
	if (iBehaviorAbilityType & FALL_DOWN_ABILITY)
    {
        g_StringLanType(szLan, "倒地", "Fall Down");
        m_comboRuleType.InsertString(index, szLan);
        m_comboRuleType.SetItemData(index, ENUM_VCA_EVENT_FALL_DOWN);
        index++;
    }
	
	if (iBehaviorAbilityType & AUDIO_ABNORMAL_ABILITY)
    {
        g_StringLanType(szLan, "声强突变", "Audio abnormal");
        m_comboRuleType.InsertString(index, szLan);
        m_comboRuleType.SetItemData(index, ENUM_VCA_EVENT_AUDIO_ABNORMAL);
        index++;
    }

	if (iBehaviorAbilityType & ADV_REACH_HEIGHT_ABILITY)
    {
        g_StringLanType(szLan, "折线攀高", "ADV Reach height");
        m_comboRuleType.InsertString(index, szLan);
        m_comboRuleType.SetItemData(index, ENUM_VCA_EVENT_ADV_REACH_HEIGHT);
        index++;
    }
    
	if (iBehaviorAbilityType & TOILET_TARRY_ABILITY)
    {
        g_StringLanType(szLan, "区域滞留超时", "Toilet tarry");
        m_comboRuleType.InsertString(index, szLan);
        m_comboRuleType.SetItemData(index, ENUM_VCA_EVENT_TOILET_TARRY);
        index++;
    }

	if (iBehaviorAbilityType & YARD_TARRY_ABILITY)
    {
        g_StringLanType(szLan, "放风场滞留", "Yard tarry");
        m_comboRuleType.InsertString(index, szLan);
        m_comboRuleType.SetItemData(index, ENUM_VCA_EVENT_YARD_TARRY);
        index++;
    }

	if (iBehaviorAbilityType & ADV_TRAVERSE_PLANE_ABILITY)
	{
		g_StringLanType(szLan, "折线警戒面", "ADV Traverse plane");
        m_comboRuleType.InsertString(index, szLan);
        m_comboRuleType.SetItemData(index, ENUM_VCA_EVENT_ADV_TRAVERSE_PLANE);
        index++;
	}

    if (iBehaviorAbilityType & OVER_TIME_ABILITY)
    {
        g_StringLanType(szLan, "操作超时", "over time");
        m_comboRuleType.InsertString(index, szLan);
        m_comboRuleType.SetItemData(index, ENUM_VCA_EVENT_OVER_TIME);
        index++;
    }

    if (iBehaviorAbilityType & HUMAN_ENTER_ABILITY)
    {
        g_StringLanType(szLan, "人员进入", "Human enter");
        m_comboRuleType.InsertString(index, szLan);
        m_comboRuleType.SetItemData(index, ENUM_VCA_EVENT_HUMAN_ENTER);
        index++;
    }

    if((iBehaviorAbilityType&STICK_UP_ABILITY) != 0)
    {
        g_StringLanType(szLan, "贴纸条", "Stick up");
        m_comboRuleType.InsertString(index, szLan);
        m_comboRuleType.SetItemData(index, ENUM_VCA_EVENT_STICK_UP);
        index++;
    }
    if ((iBehaviorAbilityType&INSTALL_SCANNER_ABILITY) != 0)
    {
        g_StringLanType(szLan, "安装读卡器", "Install Scanner");
        m_comboRuleType.InsertString(index, szLan);
        m_comboRuleType.SetItemData(index, ENUM_VCA_EVENT_INSTALL_SCANNER);
        index++;
    }

   if((iBehaviorAbilityTypeEx&PEOPLENUM_CHANGE_ABILITY) != 0)
   {
	   g_StringLanType(szLan, "人数变化事件", "People Num Change");
	   m_comboRuleType.InsertString(index, szLan);
	   m_comboRuleType.SetItemData(index, ENUM_VCA_EVENT_PEOPLENUM_CHANGE);
	   index++;
   }

   if ((iBehaviorAbilityTypeEx&SPACING_CHANGE_ABILITY) != 0)
   {
	   g_StringLanType(szLan, "间距变化事件", "Spacing Change");
	   m_comboRuleType.InsertString(index, szLan);
	   m_comboRuleType.SetItemData(index, ENUM_VCA_EVENT_SPACING_CHANGE);
	   index++;
    }

   if ((iBehaviorAbilityTypeEx&EVENT_FAKECARD) != 0)
   {
       g_StringLanType(szLan, "伪造卡检测", "Fake card");
       m_comboRuleType.InsertString(index, szLan);
       m_comboRuleType.SetItemData(index, ENUM_VCA_EVENT_FAKECARD);
       index++;
   }
    return TRUE;
    
}


/*********************************************************
Function:	InsertRuleID
Desc:	display rule ID in m_comboRuleID
Input:	
Output:	
Return:	BOOL 
**********************************************************/
BOOL CDlgVcaRuleCfgEx::InitRuleID(int RuleNum)
{
    m_comboRuleID.ResetContent();
    char sbuf[128] = {0};
    for (int i= 0; i < RuleNum; i++)
    {	
        char szLan[128] = {0};
        char sBufCn[128] = {0};
        char sBufEn[128] = {0};
        sprintf(sBufCn, "规则%d", i);
        sprintf(sBufEn, "Rule%d", i);
        g_StringLanType(szLan,sBufCn,sBufEn);
        m_comboRuleID.InsertString(i,szLan);
    }
    return TRUE;
}




/*********************************************************
Function:	OnSelchangeComboRuleId
Desc:		Modify combo for rule ID
Input:	
Output:	
Return:	void
**********************************************************/
void CDlgVcaRuleCfgEx::OnSelchangeComboRuleId() 
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    //切换规则之前先保存数据
    GetOneVcaRuleFromWnd(m_struVcaRule.m_struRuleCfg.struRule[m_iCurRuleID]);

    m_iCurRuleID = m_comboRuleID.GetCurSel();
    //切换后显示切换后的数据
    memcpy(&m_struOneRule, &m_struVcaRule.m_struRuleCfg.struRule[m_iCurRuleID], sizeof(NET_VCA_ONE_RULE_V42));
    
   SetOneVcaRuleToWnd(m_struOneRule);

    UpdateData(FALSE);
    OnChkRuleActive();

}


/*********************************************************
Function:	ShowRuleTypeWnd
Desc:	According to event type, show or Hide correlative control
Input:	VCA_RULE_EVENT_TYPE_EX struEventType
Output:	
Return:	 BOOL 
**********************************************************/
BOOL CDlgVcaRuleCfgEx::ShowRuleTypeWnd(VCA_RULE_EVENT_TYPE_EX struEventType)
{
    //隐藏规则参数控件
    for (int i = 0; i < RULE_PARAM_IDS_NUM; i++)
    {
        GetDlgItem(m_staticRuleParamIDs[i])->ShowWindow(SW_HIDE);
    }
    char szLan[128] = { 0 };
    GetDlgItem(IDC_CHK_DRAW_LINE)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_CHK_DRAW_POLYGON)->ShowWindow(SW_HIDE);

    //if 支持警戒面的事件
    if (struEventType == ENUM_VCA_EVENT_TRAVERSE_PLANE || struEventType == ENUM_VCA_EVENT_REACH_HIGHT)
    {
        //共用的参数
        //1.警戒面
        g_StringLanType(szLan, "画警戒面(注:按住ctrl画警戒面)", "Draw Line(Note:hold Ctrl)");
        GetDlgItem(IDC_CHK_DRAW_LINE)->SetWindowTextA(szLan);
        GetDlgItem(IDC_CHK_DRAW_LINE)->ShowWindow(SW_SHOW);

        m_bDrawLine = FALSE;
        m_bDrawPolygon = FALSE;
        memset(&m_struVcaPolygon[m_iCurRuleID], 0, sizeof(NET_VCA_LINE));

        if (struEventType ==ENUM_VCA_EVENT_TRAVERSE_PLANE )
        {
            //1.dwCrossDirection 
            g_StringLanType(szLan, "穿越方向", "Cross Direct");
            m_staticRuleParam1.SetWindowText(szLan);
            m_staticRuleParam1.ShowWindow(SW_SHOW);
            
            //填充下拉框  0-双向   1-由左至右   2-由右至左 
            m_comboRuleParam1.InsertString(0, "Bi-Direction");
            m_comboRuleParam1.SetItemData(0, 0);
            m_comboRuleParam1.InsertString(1, "Left2Right");
            m_comboRuleParam1.SetItemData(1, 1);
            m_comboRuleParam1.InsertString(2, "Right2Left");
            m_comboRuleParam1.SetItemData(2, 2);
            GetDlgItem(IDC_COMBO_RULE_PARAM1)->ShowWindow(SW_SHOW);

            //2.bySensitivity 
            g_StringLanType(szLan, "灵敏度", "Sensitivity");
            m_staticRuleParam3.SetWindowText(szLan);
            m_staticRuleParam3.ShowWindow(SW_SHOW);
            m_editRuleParam3.ShowWindow(SW_SHOW);

            //3.byDetectionTarget 
            g_StringLanType(szLan, "检测目标", "Target Type");
            m_staticRuleParam2.SetWindowText(szLan);
            m_staticRuleParam2.ShowWindow(SW_SHOW);

           m_comboRuleParam2.InsertString(0, "all");
            m_comboRuleParam2.SetItemData(0, 0);
            m_comboRuleParam2.InsertString(1, "human");
            m_comboRuleParam2.SetItemData(1, 1);
            m_comboRuleParam2.InsertString(2, "vehicle");
            m_comboRuleParam2.SetItemData(2, 2);
            m_comboRuleParam2.ShowWindow(SW_SHOW);

        }
        else if (struEventType == ENUM_VCA_EVENT_REACH_HIGHT)
        {
            //触发时间阈值，1-100秒 
            g_StringLanType(szLan, "触发时间阈值:", "Trigger time:");
            m_staticRuleParam3.SetWindowText(szLan);
            m_staticRuleParam3.ShowWindow(SW_SHOW);
            
            m_editRuleParam3.ShowWindow(SW_SHOW);
        }

    }
    else // else 支持检测区域的事件
    {
        //事件公用参数 
        //事件公用参数1.区域范围 
        GetDlgItem(IDC_CHK_DRAW_POLYGON)->ShowWindow(SW_SHOW);
        m_bDrawLine = FALSE;
        m_bDrawPolygon = FALSE;
        memset(&m_struVcaLine[m_iCurRuleID], 0, sizeof(NET_VCA_LINE));

        //事件公用参数1.区域范围 

        //进入离开区域
        if (struEventType == ENUM_VCA_EVENT_ENTER_AREA || struEventType == ENUM_VCA_EVENT_EXIT_AREA)
        {
            //2.bySensitivity 
            g_StringLanType(szLan, "灵敏度", "Sensitivity");
            m_staticRuleParam3.SetWindowText(szLan);
            m_staticRuleParam3.ShowWindow(SW_SHOW);
            m_editRuleParam3.ShowWindow(SW_SHOW);

        }
        //入侵
        else if (struEventType == ENUM_VCA_EVENT_INTRUSION)
        {
            //1.触发时间阈值，1-100秒 
            g_StringLanType(szLan, "触发时间阈值:", "Trigger time:");
            m_staticRuleParam3.SetWindowText(szLan);
            m_staticRuleParam3.ShowWindow(SW_SHOW);

            m_editRuleParam3.ShowWindow(SW_SHOW);

            //2.bySensitivity 
            g_StringLanType(szLan, "灵敏度", "Sensitivity");
            m_staticRuleParam5.SetWindowText(szLan);
            m_staticRuleParam5.ShowWindow(SW_SHOW);
            m_editRuleParam5.ShowWindow(SW_SHOW);

            if (g_struDeviceInfo[m_iDevIndex].iDeviceType == IDS6101_HF_A)
            {
//                m_staticAlarmArea.SetWindowText("1-1200s");
            }
            else
            {
//                m_staticAlarmArea.SetWindowText("1-120s");
            }

        }
        else if (struEventType == ENUM_VCA_EVENT_LOITER)  //徘徊
        {
            //1.触发时间阈值，1-100秒 
            g_StringLanType(szLan, "触发时间阈值:", "Trigger time:");
            m_staticRuleParam3.SetWindowText(szLan);
            m_staticRuleParam3.ShowWindow(SW_SHOW);

            m_editRuleParam3.ShowWindow(SW_SHOW);
        }
        else if (struEventType == ENUM_VCA_EVENT_LEFT_TAKE)   //丢包/捡包
        {
            //1.触发丢包/捡包报警的持续时间，1-120秒
            g_StringLanType(szLan, "触发时间阈值:", "Trigger time:");
            m_staticRuleParam3.SetWindowText(szLan);
            m_staticRuleParam3.ShowWindow(SW_SHOW);

            m_editRuleParam3.ShowWindow(SW_SHOW);
        }
        else if (struEventType == ENUM_VCA_EVENT_PARKING)
        {
            //1.触发停车报警的持续时间，1-120秒
            g_StringLanType(szLan, "触发时间阈值:", "Trigger time:");
            m_staticRuleParam3.SetWindowText(szLan);
            m_staticRuleParam3.ShowWindow(SW_SHOW);

            m_editRuleParam3.ShowWindow(SW_SHOW);
        }
        else if (struEventType == ENUM_VCA_EVENT_RUN)
        {
            //1.byMode 距离模式
            g_StringLanType(szLan, "距离模式", "Distance Mode");
            m_staticRuleParam1.SetWindowText(szLan);
            m_staticRuleParam1.ShowWindow(SW_SHOW);

            g_StringLanType(szLan, "像素模式", "Pixes");
            m_comboRuleParam1.InsertString(0, szLan);
            m_comboRuleParam1.SetItemData(0, 0);
            g_StringLanType(szLan, "实际模式", "Metre");
            m_comboRuleParam1.InsertString(1, szLan);
            m_comboRuleParam1.SetItemData(1, 1);
            m_comboRuleParam1.ShowWindow(SW_SHOW);

            //2.fRunDistance 人奔跑最大距离
            g_StringLanType(szLan, "快速移动距离:","Run Distance:");
            m_staticRuleParam3.SetWindowText(szLan);
            m_staticRuleParam3.ShowWindow(SW_SHOW);
            m_editRuleParam3.ShowWindow(SW_SHOW);

            //3.bySensitivity 
            g_StringLanType(szLan, "灵敏度", "Sensitivity");
            m_staticRuleParam5.SetWindowText(szLan);
            m_staticRuleParam5.ShowWindow(SW_SHOW);
            m_editRuleParam5.ShowWindow(SW_SHOW);
            
        }
        else if (struEventType == ENUM_VCA_EVENT_HIGH_DENSITY)
        {
            //fDensity 密度比率
            g_StringLanType(szLan, "密度比率:", "Person density:");
            m_staticRuleParam3.SetWindowText(szLan);
            m_staticRuleParam3.ShowWindow(SW_SHOW);

            m_editRuleParam3.ShowWindow(SW_SHOW);

            //bySensitivity 
            g_StringLanType(szLan, "灵敏度", "Sensitivity");
            m_staticRuleParam5.SetWindowText(szLan);
            m_staticRuleParam5.ShowWindow(SW_SHOW);
            m_editRuleParam5.ShowWindow(SW_SHOW);

        }
        else if (struEventType == ENUM_VCA_EVENT_VIOLENT_MOTION)
        {
            //1.byMode 模式
            g_StringLanType(szLan, "模式", "Mode");
            m_staticRuleParam1.SetWindowText(szLan);
            m_staticRuleParam1.ShowWindow(SW_SHOW);

            m_comboRuleParam1.ResetContent();
            m_comboRuleParam1.InsertString(0, "Video");
            m_comboRuleParam1.SetItemData(0, 0);
            m_comboRuleParam1.InsertString(1, "Video&Audio");
            m_comboRuleParam1.SetItemData(1, 1);
            m_comboRuleParam1.InsertString(2, "Audio");
            m_comboRuleParam1.SetItemData(2, 2);
            m_comboRuleParam1.ShowWindow(SW_SHOW);

            //2.触发事件阈值，1-50秒 
            g_StringLanType(szLan, "触发时间阈值:", "Trigger time:");
            m_staticRuleParam3.SetWindowText(szLan);
            m_staticRuleParam3.ShowWindow(SW_SHOW);

            m_editRuleParam3.ShowWindow(SW_SHOW);

            //3.bySensitivity 
            g_StringLanType(szLan, "灵敏度", "Sensitivity");
            m_staticRuleParam5.SetWindowText(szLan);
            m_staticRuleParam5.ShowWindow(SW_SHOW);
            m_editRuleParam5.ShowWindow(SW_SHOW);

        }
        else if (struEventType == ENUM_VCA_EVENT_GET_UP || 
			     struEventType == ENUM_VCA_EVENT_KEY_PERSON_GET_UP)
        {
            //1.byMode 模式
            g_StringLanType(szLan, "模式", "Mode");
            m_staticRuleParam1.SetWindowText(szLan);
            m_staticRuleParam1.ShowWindow(SW_SHOW);

            m_comboRuleParam1.ResetContent();
            g_StringLanType(szLan, "大床通铺模式", "Wide Bed Mode");
            m_comboRuleParam1.InsertString(0, szLan);
            m_comboRuleParam1.SetItemData(0, 0);
            g_StringLanType(szLan, "高低铺模式", "Bunk Bed Mode");
            m_comboRuleParam1.InsertString(1, szLan);
            m_comboRuleParam1.SetItemData(1, 1);
            g_StringLanType(szLan, "大床通铺坐立起身模式", "Sitting and Getting Up Mode");
            m_comboRuleParam1.InsertString(2, szLan);
            m_comboRuleParam1.SetItemData(2, 2);
            m_comboRuleParam1.ShowWindow(SW_SHOW);

            //2.触发事件阈值，1-50秒 
            g_StringLanType(szLan, "触发时间阈值:", "Trigger time:");
            m_staticRuleParam3.SetWindowText(szLan);
            m_staticRuleParam3.ShowWindow(SW_SHOW);

            m_editRuleParam3.ShowWindow(SW_SHOW);

            //3.bySensitivity 
            g_StringLanType(szLan, "灵敏度", "Sensitivity");
            m_staticRuleParam5.SetWindowText(szLan);
            m_staticRuleParam5.ShowWindow(SW_SHOW);
            m_editRuleParam5.ShowWindow(SW_SHOW);

        }
        else if (struEventType == ENUM_VCA_EVENT_LEFT)
        {
            //1.触发事件阈值，1-50秒 
            g_StringLanType(szLan, "触发时间阈值:", "Trigger time:");
            m_staticRuleParam3.SetWindowText(szLan);
            m_staticRuleParam3.ShowWindow(SW_SHOW);

            m_editRuleParam3.ShowWindow(SW_SHOW);

            //2.bySensitivity 
            g_StringLanType(szLan, "灵敏度", "Sensitivity");
            m_staticRuleParam5.SetWindowText(szLan);
            m_staticRuleParam5.ShowWindow(SW_SHOW);
            m_editRuleParam5.ShowWindow(SW_SHOW);

        }
        else if (struEventType == ENUM_VCA_EVENT_TAKE)
        {
            //1.触发事件阈值，10~100秒 
            g_StringLanType(szLan, "触发时间阈值:", "Trigger time:");
            m_staticRuleParam3.SetWindowText(szLan);
            m_staticRuleParam3.ShowWindow(SW_SHOW);
            m_editRuleParam3.ShowWindow(SW_SHOW);

            //2.bySensitivity 1~5
            g_StringLanType(szLan, "灵敏度", "Sensitivity");
            m_staticRuleParam5.SetWindowText(szLan);
            m_staticRuleParam5.ShowWindow(SW_SHOW);
            m_editRuleParam5.ShowWindow(SW_SHOW);
        }
        else if (struEventType == ENUM_VCA_EVENT_HUMAN_ENTER)
        {

        }
        else if (struEventType == ENUM_VCA_EVENT_OVER_TIME)
        {
            g_StringLanType(szLan, "触发时间阈值:", "Trigger time:");
            m_staticRuleParam3.SetWindowText(szLan);
            m_staticRuleParam3.ShowWindow(SW_SHOW);
            m_editRuleParam3.ShowWindow(SW_SHOW);

        }
        else if (struEventType == ENUM_VCA_EVENT_STICK_UP)
        {
            g_StringLanType(szLan, "触发时间阈值:", "Trigger time:");
            m_staticRuleParam3.SetWindowText(szLan);
            m_staticRuleParam3.ShowWindow(SW_SHOW);
            m_editRuleParam3.ShowWindow(SW_SHOW);

            //2.bySensitivity 1~5
            g_StringLanType(szLan, "灵敏度", "Sensitivity");
            m_staticRuleParam5.SetWindowText(szLan);
            m_staticRuleParam5.ShowWindow(SW_SHOW);
            m_editRuleParam5.ShowWindow(SW_SHOW);
        }
        else if (struEventType == ENUM_VCA_EVENT_INSTALL_SCANNER)
        {
            g_StringLanType(szLan, "触发时间阈值:", "Trigger time:");
            m_staticRuleParam3.SetWindowText(szLan);
            m_staticRuleParam3.ShowWindow(SW_SHOW);
            m_editRuleParam3.ShowWindow(SW_SHOW);

            //2.bySensitivity 1~5
            g_StringLanType(szLan, "灵敏度", "Sensitivity");
            m_staticRuleParam5.SetWindowText(szLan);
            m_staticRuleParam5.ShowWindow(SW_SHOW);
            m_editRuleParam5.ShowWindow(SW_SHOW);
        }
        else if (struEventType == ENUM_VCA_EVENT_LEAVE_POSITION)
        {
            //1.byMode 模式
            g_StringLanType(szLan, "模式", "Mode");
            m_staticRuleParam1.SetWindowText(szLan);
            m_staticRuleParam1.ShowWindow(SW_SHOW);

            m_comboRuleParam1.ResetContent();
            g_StringLanType(szLan, "岗位状态事件", "Absence");
            m_comboRuleParam1.InsertString(0, szLan);
            m_comboRuleParam1.SetItemData(0, 0);
            g_StringLanType(szLan, "岗位状态事件", "Sleep On Duty");
            m_comboRuleParam1.InsertString(1, szLan);
            m_comboRuleParam1.SetItemData(1, 1);
            g_StringLanType(szLan, "岗位状态事件", "Absence or Sleep On Duty");
            m_comboRuleParam1.InsertString(2, szLan);
            m_comboRuleParam1.SetItemData(2, 2);
            m_comboRuleParam1.ShowWindow(SW_SHOW);

            //2.byPersonType 
            g_StringLanType(szLan, "值岗人数", "PersonType");
            m_staticRuleParam2.SetWindowText(szLan);
            m_staticRuleParam2.ShowWindow(SW_SHOW);

            m_comboRuleParam2.ResetContent();
            g_StringLanType(szLan, "单人值岗", "Single");
            m_comboRuleParam2.InsertString(0, szLan);
            m_comboRuleParam2.SetItemData(0, 0);
            g_StringLanType(szLan, "双人值岗", "Double");
            m_comboRuleParam2.InsertString(1, szLan);
            m_comboRuleParam2.SetItemData(1, 1);
            m_comboRuleParam2.ShowWindow(SW_SHOW);

            //3.无人报警时间，单位：秒，取值范围：1~1800 
            g_StringLanType(szLan, "无人报警时间:", "LeaveDelay:");
            m_staticRuleParam3.SetWindowText(szLan);
            m_staticRuleParam3.ShowWindow(SW_SHOW);
            m_editRuleParam3.ShowWindow(SW_SHOW);

            //4.睡觉报警时间，单位：秒，取值范围：1~1800 
            g_StringLanType(szLan, "睡觉报警时间:", "SleepDelay:");
            m_staticRuleParam4.SetWindowText(szLan);
            m_staticRuleParam4.ShowWindow(SW_SHOW);
            m_editRuleParam4.ShowWindow(SW_SHOW);

            //5.bySensitivity 
            g_StringLanType(szLan, "灵敏度", "Sensitivity");
            m_staticRuleParam5.SetWindowText(szLan);
            m_staticRuleParam5.ShowWindow(SW_SHOW);
            m_editRuleParam5.ShowWindow(SW_SHOW);

            //6.在岗人数，取值范围：1-10，默认1 
            g_StringLanType(szLan, "在岗人数", "PeopleNums");
            m_staticRuleParam6.SetWindowText(szLan);
            m_staticRuleParam6.ShowWindow(SW_SHOW);
            m_editRuleParam6.ShowWindow(SW_SHOW);
        }
        else if (struEventType == ENUM_VCA_EVENT_TRAIL)
        {
            //1.bySensitivity 
            g_StringLanType(szLan, "灵敏度", "Sensitivity");
            m_staticRuleParam3.SetWindowText(szLan);
            m_staticRuleParam3.ShowWindow(SW_SHOW);
            m_editRuleParam3.ShowWindow(SW_SHOW);
        }
        else if (struEventType == ENUM_VCA_EVENT_FALL_DOWN)
        {
            //1.触发事件阈值，单位：秒，取值范围：[1,3600] 
            g_StringLanType(szLan, "触发时间阈值:", "Trigger time:");
            m_staticRuleParam3.SetWindowText(szLan);
            m_staticRuleParam3.ShowWindow(SW_SHOW);
            m_editRuleParam3.ShowWindow(SW_SHOW);

            //2.高度阈值 取值范围：[0,250]，默认：90，单位：厘米 
            g_StringLanType(szLan, "高度阈值", "HeightThreshold");
            m_staticRuleParam4.SetWindowText(szLan);
            m_staticRuleParam4.ShowWindow(SW_SHOW);
            m_editRuleParam4.ShowWindow(SW_SHOW);

            //2.bySensitivity 1~5
            g_StringLanType(szLan, "灵敏度", "Sensitivity");
            m_staticRuleParam5.SetWindowText(szLan);
            m_staticRuleParam5.ShowWindow(SW_SHOW);
            m_editRuleParam5.ShowWindow(SW_SHOW);

        }
        else if (struEventType == ENUM_VCA_EVENT_AUDIO_ABNORMAL)
        {
            //1.AudioMode 模式
            g_StringLanType(szLan, "声音检测模式", "AudioMode");
            m_staticRuleParam1.SetWindowText(szLan);
            m_staticRuleParam1.ShowWindow(SW_SHOW);

            m_comboRuleParam1.ResetContent();
            g_StringLanType(szLan, "灵敏度检测", "Sensitivity");
            m_comboRuleParam1.InsertString(0, szLan);
            m_comboRuleParam1.SetItemData(0, 0);
            g_StringLanType(szLan, "分贝阈值检测", "Decibel");
            m_comboRuleParam1.InsertString(1, szLan);
            m_comboRuleParam1.SetItemData(1, 1);
            g_StringLanType(szLan, "都启用", "Both");
            m_comboRuleParam1.InsertString(2, szLan);
            m_comboRuleParam1.SetItemData(2, 2);
            m_comboRuleParam1.ShowWindow(SW_SHOW);

            //2.是否开启声强陡升侦测功能：0- 否，1- 是  
            g_StringLanType(szLan, "声强陡升侦测", "AudioSteepEnabled");
            m_staticRuleParam2.SetWindowText(szLan);
            m_staticRuleParam2.ShowWindow(SW_SHOW);

            m_comboRuleParam2.ResetContent();
            g_StringLanType(szLan, "否", "No");
            m_comboRuleParam2.InsertString(0, szLan);
            m_comboRuleParam2.SetItemData(0, 0);
            g_StringLanType(szLan, "是", "Yes");
            m_comboRuleParam2.InsertString(1, szLan);
            m_comboRuleParam2.SetItemData(1, 1);
            m_comboRuleParam2.ShowWindow(SW_SHOW);

            //3.无人报警时间，单位：秒，取值范围：1~1800 
            g_StringLanType(szLan, "声音强度:", "Decibel:");
            m_staticRuleParam3.SetWindowText(szLan);
            m_staticRuleParam3.ShowWindow(SW_SHOW);
            m_editRuleParam3.ShowWindow(SW_SHOW);

            //4.bySensitivity 1~100
            g_StringLanType(szLan, "灵敏度", "Sensitivity");
            m_staticRuleParam4.SetWindowText(szLan);
            m_staticRuleParam4.ShowWindow(SW_SHOW);
            m_editRuleParam4.ShowWindow(SW_SHOW);

           //5.byThreshold  声音强度阈值，取值范围：[1,100] 
            g_StringLanType(szLan, "声音强调阈值", "AudioThreshold");
            m_staticRuleParam5.SetWindowText(szLan);
            m_staticRuleParam5.ShowWindow(SW_SHOW);
            m_editRuleParam5.ShowWindow(SW_SHOW);

        }
        else if (struEventType == ENUM_VCA_EVENT_ADV_REACH_HEIGHT)
        {
            //跨越方向 dwCrossDirection 
            g_StringLanType(szLan, "跨越方向", "CrossDirection");
            m_staticRuleParam1.SetWindowText(szLan);
            m_staticRuleParam1.ShowWindow(SW_SHOW);

            m_comboRuleParam1.ResetContent();
            g_StringLanType(szLan, "双向", "Bi-Direction");
            m_comboRuleParam1.InsertString(0, szLan);
            m_comboRuleParam1.SetItemData(0, 0);
            g_StringLanType(szLan, "由左至右", "Left2Right");
            m_comboRuleParam1.InsertString(1, szLan);
            m_comboRuleParam1.SetItemData(1, 1);
            g_StringLanType(szLan, "由右至左", "Right2Left");
            m_comboRuleParam1.InsertString(2, szLan);
            m_comboRuleParam1.SetItemData(2, 2);
            m_comboRuleParam1.ShowWindow(SW_SHOW);
        }
        else if (struEventType == ENUM_VCA_EVENT_TOILET_TARRY)
        {
            //1.区域滞留超时时间，单位：秒，取值范围：[1,3600]
            g_StringLanType(szLan, "区域滞留超时时间:", "Delay Time:");
            m_staticRuleParam3.SetWindowText(szLan);
            m_staticRuleParam3.ShowWindow(SW_SHOW);
            m_editRuleParam3.ShowWindow(SW_SHOW);
        }
        else if (struEventType == ENUM_VCA_EVENT_YARD_TARRY)
        {
            //1.放风场滞留时间，单位：秒，取值范围：[1,120] 
            g_StringLanType(szLan, "滞留时间(s):", "Delay Time:");
            m_staticRuleParam3.SetWindowText(szLan);
            m_staticRuleParam3.ShowWindow(SW_SHOW);
            m_editRuleParam3.ShowWindow(SW_SHOW);
        }
        else if (struEventType == ENUM_VCA_EVENT_ADV_TRAVERSE_PLANE)
        {
            //跨越方向 dwCrossDirection 
            g_StringLanType(szLan, "跨越方向", "CrossDirection");
            m_staticRuleParam1.SetWindowText(szLan);
            m_staticRuleParam1.ShowWindow(SW_SHOW);

            m_comboRuleParam1.ResetContent();
            g_StringLanType(szLan, "双向", "Bi-Direction");
            m_comboRuleParam1.InsertString(0, szLan);
            m_comboRuleParam1.SetItemData(0, 0);
            g_StringLanType(szLan, "由左至右", "Left2Right");
            m_comboRuleParam1.InsertString(1, szLan);
            m_comboRuleParam1.SetItemData(1, 1);
            g_StringLanType(szLan, "由右至左", "Right2Left");
            m_comboRuleParam1.InsertString(2, szLan);
            m_comboRuleParam1.SetItemData(2, 2);
            m_comboRuleParam1.ShowWindow(SW_SHOW);

            //4.bySensitivity 1~100
            g_StringLanType(szLan, "灵敏度", "Sensitivity");
            m_staticRuleParam3.SetWindowText(szLan);
            m_staticRuleParam3.ShowWindow(SW_SHOW);
            m_editRuleParam3.ShowWindow(SW_SHOW);
        }
        else if (struEventType == ENUM_VCA_EVENT_STANDUP)
        {
            //1.触发事件阈值，单位：秒，取值范围：[1,3600] 
            g_StringLanType(szLan, "触发时间阈值:", "Trigger time:");
            m_staticRuleParam3.SetWindowText(szLan);
            m_staticRuleParam3.ShowWindow(SW_SHOW);
            m_editRuleParam3.ShowWindow(SW_SHOW);

            //2.高度阈值，取值范围：[0,250]，默认：130，单位：厘米 
            g_StringLanType(szLan, "高度阈值", "HeightThreshold");
            m_staticRuleParam4.SetWindowText(szLan);
            m_staticRuleParam4.ShowWindow(SW_SHOW);
            m_editRuleParam4.ShowWindow(SW_SHOW);

            //2.bySensitivity 1~5
            g_StringLanType(szLan, "灵敏度", "Sensitivity");
            m_staticRuleParam5.SetWindowText(szLan);
            m_staticRuleParam5.ShowWindow(SW_SHOW);
            m_editRuleParam5.ShowWindow(SW_SHOW);

        }
        else if (struEventType == ENUM_VCA_EVENT_PEOPLENUM_CHANGE)
        {
            //1.触发事件阈值，单位：秒，取值范围：[1,3600] 
            g_StringLanType(szLan, "触发时间阈值:", "Trigger time:");
            m_staticRuleParam3.SetWindowText(szLan);
            m_staticRuleParam3.ShowWindow(SW_SHOW);
            m_editRuleParam3.ShowWindow(SW_SHOW);

            //2.人数阈值，取值范围：[0,5]，默认：1 
            g_StringLanType(szLan, "人数阈值", "PeopleNumThreshold");
            m_staticRuleParam4.SetWindowText(szLan);
            m_staticRuleParam4.ShowWindow(SW_SHOW);
            m_editRuleParam4.ShowWindow(SW_SHOW);

            //2.bySensitivity 1~5
            g_StringLanType(szLan, "灵敏度", "Sensitivity");
            m_staticRuleParam5.SetWindowText(szLan);
            m_staticRuleParam5.ShowWindow(SW_SHOW);
            m_editRuleParam5.ShowWindow(SW_SHOW);

            //无人状态是否有效：0- 无效，1- 有效  
            g_StringLanType(szLan, "无人状态使能", "NoneStateEffective");
            m_staticRuleParam1.SetWindowText(szLan);
            m_staticRuleParam1.ShowWindow(SW_SHOW);

            m_comboRuleParam1.ResetContent();
            g_StringLanType(szLan, "否", "No");
            m_comboRuleParam1.InsertString(0, szLan);
            m_comboRuleParam1.SetItemData(0, 0);
            g_StringLanType(szLan, "是", "Yes");
            m_comboRuleParam1.InsertString(1, szLan);
            m_comboRuleParam1.SetItemData(1, 1);
            m_comboRuleParam1.ShowWindow(SW_SHOW);

            //4.byDetectMode 
            g_StringLanType(szLan, "检测方式", "DetectMode");
            m_staticRuleParam2.SetWindowText(szLan);
            m_staticRuleParam2.ShowWindow(SW_SHOW);

            m_comboRuleParam2.ResetContent();
            g_StringLanType(szLan, "大于", "More than");
            m_comboRuleParam2.InsertString(0, szLan);
            m_comboRuleParam2.SetItemData(0, 0);
            g_StringLanType(szLan, "小于", "Less than");
            m_comboRuleParam2.InsertString(1, szLan);
            m_comboRuleParam2.SetItemData(1, 1);
            g_StringLanType(szLan, "等于", "Equal to");
            m_comboRuleParam2.InsertString(2, szLan);
            m_comboRuleParam2.SetItemData(2, 2);
            g_StringLanType(szLan, "不等于", "Not equal to");
            m_comboRuleParam2.InsertString(3, szLan);
            m_comboRuleParam2.SetItemData(3, 3);
            m_comboRuleParam2.ShowWindow(SW_SHOW);
        }
        else if (struEventType == ENUM_VCA_EVENT_SPACING_CHANGE)
        {
            //1.触发事件阈值，单位：秒，取值范围：[1,3600] 
            g_StringLanType(szLan, "触发时间阈值:", "Trigger time:");
            m_staticRuleParam3.SetWindowText(szLan);
            m_staticRuleParam3.ShowWindow(SW_SHOW);
            m_editRuleParam3.ShowWindow(SW_SHOW);

            //2.间距阈值，取值范围：[0,10.0]，默认：1.0，单位：米 
            g_StringLanType(szLan, "间距阈值:", "SpacingThreshold:");
            m_staticRuleParam4.SetWindowText(szLan);
            m_staticRuleParam4.ShowWindow(SW_SHOW);
            m_editRuleParam4.ShowWindow(SW_SHOW);

            //2.bySensitivity 1~5
            g_StringLanType(szLan, "灵敏度", "Sensitivity");
            m_staticRuleParam5.SetWindowText(szLan);
            m_staticRuleParam5.ShowWindow(SW_SHOW);
            m_editRuleParam5.ShowWindow(SW_SHOW);

            //检测方式，与人数阈值相比较：1- 大于，2- 小于 
            g_StringLanType(szLan, "检测模式", "DetectMode");
            m_staticRuleParam1.SetWindowText(szLan);
            m_staticRuleParam1.ShowWindow(SW_SHOW);

            m_comboRuleParam1.ResetContent();
            g_StringLanType(szLan, "大于", "More than");
            m_comboRuleParam1.InsertString(0, szLan);
            m_comboRuleParam1.SetItemData(0, 0);
            g_StringLanType(szLan, "小于", "Less than");
            m_comboRuleParam1.InsertString(1, szLan);
            m_comboRuleParam1.SetItemData(1, 1);
            m_comboRuleParam1.ShowWindow(SW_SHOW);

        }
        else if (struEventType == ENUM_VCA_EVENT_FAKECARD)
        {
            //1.bySensitivity 1~5
            g_StringLanType(szLan, "灵敏度", "Sensitivity");
            m_staticRuleParam5.SetWindowText(szLan);
            m_staticRuleParam5.ShowWindow(SW_SHOW);
            m_editRuleParam5.ShowWindow(SW_SHOW);

        }
        else
        {
            return FALSE;
        }
    }
    return TRUE;	
}

/*********************************************************
Function:	OnChkRuleActive
Desc:		Invalid or not
Input:	
Output:	
Return:	void
**********************************************************/ 
void CDlgVcaRuleCfgEx::OnChkRuleActive() 
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    OnSelchangeComboRuleType();
    EnableRuleWindow(m_bRuleActive);
    m_struOneRule.byActive = (unsigned char)m_bRuleActive;
    m_struOneRule.wAlarmDelay = m_comboAlarmDelay.GetCurSel();
}

/*********************************************************
Function:	EnableRuleWindow
Desc:	    
Input:	BOOL bEnable
Output:	
Return:	BOOL
**********************************************************/
BOOL CDlgVcaRuleCfgEx::EnableRuleWindow(BOOL bEnable)
{
    
    GetDlgItem(IDC_EDIT_RULE_NAME)->EnableWindow(bEnable);
    GetDlgItem(IDC_COMBO_RULE_TYPE)->EnableWindow(bEnable);
    GetDlgItem(IDC_CHK_DRAW_POLYGON)->EnableWindow(bEnable);
    GetDlgItem(IDC_COMBO_RULE_PRIORITY)->EnableWindow(bEnable);
    GetDlgItem(IDC_COMBO_ALARM_DELAY)->EnableWindow(bEnable);
    GetDlgItem(IDC_CHK_DRAW_LINE)->EnableWindow(bEnable);
    
    EnableRuleUnion(bEnable);

    EnablAlarmTime(bEnable);
    EnableSizeFilter(bEnable);
    EnableHandleException(bEnable);
    return TRUE;
}
//规则联合体中的参数的控件隐藏 /显示
BOOL CDlgVcaRuleCfgEx::EnableRuleUnion(BOOL bEnable)
{
    for (int i = 0; i < RULE_PARAM_IDS_NUM; i++)
    {
        GetDlgItem(m_staticRuleParamIDs[i])->EnableWindow(bEnable);
    }
    return TRUE;
}

BOOL CDlgVcaRuleCfgEx::EnablAlarmTime(BOOL bEnable)
{
    GetDlgItem(IDC_EDITHOUR11)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDITMIN11)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDITHOUR12)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDITMIN12)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDITHOUR21)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDITMIN21)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDITHOUR22)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDITMIN22)->EnableWindow(bEnable);

	GetDlgItem(IDC_EDITHOUR31)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDITMIN31)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDITHOUR32)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDITMIN32)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDITHOUR41)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDITMIN41)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDITHOUR42)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDITMIN42)->EnableWindow(bEnable);

	GetDlgItem(IDC_EDITHOUR51)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDITMIN51)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDITHOUR52)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDITMIN52)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDITHOUR61)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDITMIN61)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDITHOUR62)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDITMIN62)->EnableWindow(bEnable);

	GetDlgItem(IDC_EDITHOUR71)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDITMIN71)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDITHOUR72)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDITMIN72)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDITHOUR81)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDITMIN81)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDITHOUR82)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDITMIN82)->EnableWindow(bEnable);

    GetDlgItem(IDC_COMBOWEEKDAY)->EnableWindow(bEnable);
    GetDlgItem(IDC_COMBOCOPYTIME)->EnableWindow(bEnable);
    GetDlgItem(IDC_TIME_COPY)->EnableWindow(bEnable);
    GetDlgItem(IDC_TIME_OK)->EnableWindow(bEnable);
    return TRUE;
}

BOOL CDlgVcaRuleCfgEx::EnableHandleException(BOOL bEnable)
{
    GetDlgItem(IDC_CHECKHANDLETYPE1)->EnableWindow(bEnable);
    GetDlgItem(IDC_CHECKHANDLETYPE2)->EnableWindow(bEnable);
    GetDlgItem(IDC_CHECKHANDLETYPE3)->EnableWindow(bEnable);
    GetDlgItem(IDC_CHECKHANDLETYPE4)->EnableWindow(bEnable);
    GetDlgItem(IDC_CHECKHANDLETYPE5)->EnableWindow(bEnable);
    GetDlgItem(IDC_LIST_RECORD_CHAN)->EnableWindow(bEnable);
    return TRUE;
}

BOOL CDlgVcaRuleCfgEx::EnableSizeFilter(BOOL bEnable)
{
    GetDlgItem(IDC_CHECK_FILTER_ACTIVE)->EnableWindow(bEnable);
    GetDlgItem(IDC_CHECK_SET_MINI_FILTER)->EnableWindow(bEnable & m_bFilterActive);
    GetDlgItem(IDC_CHECK_SET_MAX_FILTER)->EnableWindow(bEnable & m_bFilterActive);
    return TRUE;
}

/*********************************************************
Function:	OnSelchangeComboRuleType
Desc:		Modify Rule Type 
Input:	
Output:	
Return:	void
**********************************************************/
void CDlgVcaRuleCfgEx::OnSelchangeComboRuleType() 
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    VCA_RULE_EVENT_TYPE_EX eEventType;
    eEventType = (VCA_RULE_EVENT_TYPE_EX)GetEventTypeFromWnd();
	m_dwEventType = eEventType;
    ShowRuleTypeWnd(eEventType);
    if (eEventType == m_struVcaRule.m_struRuleCfg.struRule[m_iCurRuleID].wEventType)
    {
        SetEventUnionToWnd(m_struVcaRule.m_struRuleCfg.struRule[m_iCurRuleID].uEventParam, eEventType);
    }
    else
    {
        NET_VCA_EVENT_UNION uEventParam;
        memset(&uEventParam, 0, sizeof(NET_VCA_EVENT_UNION));
        SetEventUnionToWnd(uEventParam, eEventType);
    }
    m_struOneRule.wEventType = eEventType; 
    UpdateData(FALSE);
}



/*********************************************************
Function:	F_DrawFun
Desc:		Draw
Input:	lPlayHandle, player handle
hDc : DC handle
dwUser: User data to distinguish different channels
Output:	
Return:	void
**********************************************************/
void CDlgVcaRuleCfgEx::F_DrawFun(long lPlayHandle, HDC hDc, DWORD dwUser)
{
    COLORREF color;
    color = RGB(255, 255, 0);//Yellow
    if(	m_struOneRule.wEventType== ENUM_VCA_EVENT_ENTER_AREA
        || m_struOneRule.wEventType == ENUM_VCA_EVENT_EXIT_AREA
        || m_struOneRule.wEventType == ENUM_VCA_EVENT_INTRUSION
        || m_struOneRule.wEventType == ENUM_VCA_EVENT_LOITER
        || m_struOneRule.wEventType == ENUM_VCA_EVENT_LEFT_TAKE
        || m_struOneRule.wEventType == ENUM_VCA_EVENT_PARKING
        || m_struOneRule.wEventType == ENUM_VCA_EVENT_RUN
        || m_struOneRule.wEventType == ENUM_VCA_EVENT_HIGH_DENSITY
        || m_struOneRule.wEventType == ENUM_VCA_EVENT_VIOLENT_MOTION
        /*|| m_struVcaRuleCfg.struRule[m_iCurRuleID].dwEventType == VCA_REACH_HIGHT*/
        || m_struOneRule.wEventType == ENUM_VCA_EVENT_GET_UP
		|| m_struOneRule.wEventType == ENUM_VCA_EVENT_KEY_PERSON_GET_UP
        || m_struOneRule.wEventType == ENUM_VCA_EVENT_LEFT
        || m_struOneRule.wEventType == ENUM_VCA_EVENT_TAKE
        || m_struOneRule.wEventType == ENUM_VCA_EVENT_OVER_TIME
        || m_struOneRule.wEventType == ENUM_VCA_EVENT_STICK_UP
        || m_struOneRule.wEventType == ENUM_VCA_EVENT_INSTALL_SCANNER
		|| m_struOneRule.wEventType == ENUM_VCA_EVENT_LEAVE_POSITION
		|| m_struOneRule.wEventType == ENUM_VCA_EVENT_TRAIL
		|| m_struOneRule.wEventType == ENUM_VCA_EVENT_FALL_DOWN
		|| m_struOneRule.wEventType == ENUM_VCA_EVENT_ADV_REACH_HEIGHT
		|| m_struOneRule.wEventType == ENUM_VCA_EVENT_TOILET_TARRY
		|| m_struOneRule.wEventType == ENUM_VCA_EVENT_YARD_TARRY
		|| m_struOneRule.wEventType == ENUM_VCA_EVENT_ADV_TRAVERSE_PLANE
		|| m_struOneRule.wEventType == ENUM_VCA_EVENT_STANDUP
		|| m_struOneRule.wEventType == ENUM_VCA_EVENT_PEOPLENUM_CHANGE
		|| m_struOneRule.wEventType == ENUM_VCA_EVENT_SPACING_CHANGE
        || m_struOneRule.wEventType == ENUM_VCA_EVENT_FAKECARD
        )
    {
        F_DrawFrame(hDc);
    }
    else if (m_struOneRule.wEventType == VCA_TRAVERSE_PLANE
        || m_struOneRule.wEventType == VCA_REACH_HIGHT)
    {
        F_DrawLine(hDc);
    }
    
    F_DrawFilter(hDc);
}

/*********************************************************
Function:	F_DrawFrame
Desc:	
Input:	
hDc : DC handle
Output:	
Return:	 void
**********************************************************/
void CDlgVcaRuleCfgEx::F_DrawFrame(HDC hDc)
{
    DRAW_POLYGON_PARAM struPolygonParam;
    memset(&struPolygonParam, 0, sizeof(struPolygonParam));
    struPolygonParam.color = RGB(255, 255, 0);
    memcpy(&struPolygonParam.rcWnd, &m_rcWnd, sizeof(m_rcWnd));
    memcpy(&struPolygonParam.struVcaPolygon, &m_struVcaPolygon[m_iCurRuleID], sizeof(struPolygonParam.struVcaPolygon));
    struPolygonParam.bCloseIn = m_bCloseIn[m_iCurRuleID];
    struPolygonParam.bMouseMove = m_bMouseMove[m_iCurRuleID];
    
    CDrawFun::DrawVcaPolygon(hDc, &struPolygonParam);
}

/*********************************************************
Function:	F_DrawFilter
Desc:	Rule filter
Input:
hDc :DC handle
Output:	
Return:	 void
**********************************************************/
void CDlgVcaRuleCfgEx::F_DrawFilter(HDC hDc)
{
    if (m_bFilterActive)
    {
        DRAW_RECT_PARAM struDrawRect;
        memset(&struDrawRect, 0, sizeof(struDrawRect));
        struDrawRect.color = RGB(0, 255, 125);
        memcpy(&struDrawRect.rcWnd, &m_rcWnd, sizeof(struDrawRect.rcWnd));
        memcpy(&struDrawRect.struVcaRect, &m_struSizeFilter.struMaxRect, sizeof(struDrawRect.struVcaRect));
        CDrawFun::DrawVcaRect(hDc, &struDrawRect);
        
        memset(&struDrawRect, 0, sizeof(struDrawRect));
        struDrawRect.color = RGB(0, 255, 125);
        memcpy(&struDrawRect.rcWnd, &m_rcWnd, sizeof(struDrawRect.rcWnd));
        memcpy(&struDrawRect.struVcaRect, &m_struSizeFilter.struMiniRect, sizeof(struDrawRect.struVcaRect));
        CDrawFun::DrawVcaRect(hDc, &struDrawRect);
    }
}


/*********************************************************
Function:	F_DrawLine
Desc:	
Input:	
HDC hDc

  Output:	
  Return:	void 
**********************************************************/
void CDlgVcaRuleCfgEx::F_DrawLine(HDC hDc)
{
    DRAW_TRAVERSE_PLANE_PARAM struTraversePlane;
    memset(&struTraversePlane, 0, sizeof(struTraversePlane));
    struTraversePlane.color = RGB(255, 255, 0);
    memcpy(&struTraversePlane.rcWnd, m_rcWnd, sizeof(struTraversePlane.rcWnd));
    memcpy(&struTraversePlane.struVcaLine, &m_struVcaLine[m_iCurRuleID], sizeof(struTraversePlane.struVcaLine));
    struTraversePlane.dwCrossDirection = m_struOneRule.uEventParam.struTraversePlane.dwCrossDirection;
    CDrawFun::DrawTraversePlane(hDc, &struTraversePlane);
}

/*********************************************************
Function:	OnChkDrawPolygon
Desc:		Draw Polygon or not. 
Input:	
Output:	
Return:	void 
**********************************************************/
void CDlgVcaRuleCfgEx::OnChkDrawPolygon() 
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    if(m_bDrawPolygon)
    {
        m_bNeedRedraw[m_iCurRuleID] = TRUE;//Draw polygon
        m_bSetMaxFilter = FALSE;//While drawing polygon, can't draw filter
        m_bSetMiniFilter = FALSE;
        
    }
    else
    {
        if (m_bNeedRedraw[m_iCurRuleID] && !m_bCloseIn[m_iCurRuleID])
        {
            if (m_bMouseMove[m_iCurRuleID])
            {
                m_bMouseMove[m_iCurRuleID] = FALSE;
                m_struVcaPolygon[m_iCurRuleID].dwPointNum--;
            }
            m_bCloseIn[m_iCurRuleID] = TRUE;
        }		
    }	
    UpdateData(FALSE);
    
}

/*********************************************************
Function:	OnCheckFilterActive
Desc:		Check if the size filter is valid
Input:	
Output:	
Return:	void 
**********************************************************/
void CDlgVcaRuleCfgEx::OnCheckFilterActive() 
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    GetDlgItem(IDC_CHECK_SET_MAX_FILTER)->EnableWindow(m_bFilterActive);
    GetDlgItem(IDC_CHECK_SET_MINI_FILTER)->EnableWindow(m_bFilterActive);
}

/*********************************************************
Function:	OnCheckSetMiniFilter
Desc:		Set the minimal filter area
Input:	
Output:	
Return:	void 
**********************************************************/
void CDlgVcaRuleCfgEx::OnCheckSetMiniFilter() 
{
    // TODO: Add your control notification handler code here
    if (m_lPlayHandle < 0)
    {
        return;
    }
    UpdateData(TRUE);
    if (m_bSetMiniFilter)
    {
        m_bSetMaxFilter = FALSE;
        m_bDrawPolygon = FALSE;
        m_bDrawLine	= FALSE;
        UpdateData(FALSE);
    }
}

/*********************************************************
Function:	OnCheckSetMaxFilter
Desc:		Set the maximal filter area.
Input:	
Output:	
Return:	void 
**********************************************************/
void CDlgVcaRuleCfgEx::OnCheckSetMaxFilter() 
{
    // TODO: Add your control notification handler code here
    if (m_lPlayHandle < 0)
    {
        return;
    }
    UpdateData(TRUE);
    if (m_bSetMaxFilter)
    {
        m_bSetMiniFilter = FALSE;
        m_bDrawPolygon = FALSE;
        m_bDrawLine = FALSE;
        UpdateData(FALSE);
    }
}

/*********************************************************
Function:	OnChkDrawLine
Desc:		Check control for Drawing line
Input:	
Output:	
Return: void 	
**********************************************************/
void CDlgVcaRuleCfgEx::OnChkDrawLine() 
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    if(m_bDrawLine)//drawing
    {
        if (m_bMouseMove[m_iCurRuleID])
        {
            m_bMouseMove[m_iCurRuleID] = FALSE;
        }
        m_bSetMaxFilter = FALSE;
        m_bSetMiniFilter = FALSE;
        UpdateData(FALSE);
    }
    else
    {
        m_bMouseMove[m_iCurRuleID] = TRUE;
    }	
    
}


/*********************************************************
Function:	OnTimeOk
Desc:		
Input:	
Output:	
Return:	void 
**********************************************************/
void CDlgVcaRuleCfgEx::OnTimeOk() 
{
    UpdateData(TRUE);
    if (!F_TimeTest())
    {
        return;
    }
    int	i = m_comboWeekday.GetCurSel();	
    GetAlarmTimeFromWnd(&m_struVcaRule.m_struRuleCfg.struRule[m_iCurRuleID].struAlarmTime[0][0], i);

}
/*********************************************************
Function:	OnTimeCopy
Desc:		
Input:	
Output:	
Return:	void 
**********************************************************/
void CDlgVcaRuleCfgEx::OnTimeCopy() 
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    int j,k;
    int i = m_comboWeekday.GetCurSel();
    int m_nCopyTime = m_comboCopyTime.GetCurSel() - 1;
    
    if(m_nCopyTime == -1)
    {
        for(j=0; j<MAX_DAYS; j++)
        {
            if(j == i)
                continue;
            for(k=0; k<MAX_TIMESEGMENT_V30/*MAX_TIMESEGMENT_2*/; k++)
            {
                memcpy(&(m_struVcaRule.m_struRuleCfg.struRule[m_iCurRuleID].struAlarmTime[j][k]), &(m_struVcaRule.m_struRuleCfg.struRule[m_iCurRuleID].struAlarmTime[i][k]), sizeof(NET_DVR_SCHEDTIME));
            }
        }
    }
    else
    {
        for(k=0; k<MAX_TIMESEGMENT_V30/*MAX_TIMESEGMENT_2*/; k++)
        {
            memcpy(&(m_struVcaRule.m_struRuleCfg.struRule[m_iCurRuleID].struAlarmTime[m_nCopyTime][k]), &(m_struVcaRule.m_struRuleCfg.struRule[m_iCurRuleID].struAlarmTime[i][k]), sizeof(NET_DVR_SCHEDTIME));
        }
    }
    
}

/*********************************************************
Function:	F_TimeTest
Desc:		Check Fortify time
Input:	
Output:	
Return:	BOOL
**********************************************************/
BOOL CDlgVcaRuleCfgEx::F_TimeTest()
{
	int i = 0;
	int j = 0;
	WORD wStartTime[MAX_TIMESEGMENT_V30], wStopTime[MAX_TIMESEGMENT_V30];
	char szLan[128] = {0};
	UpdateData(TRUE);
	if ((m_iHour11 < 0) || (m_iHour11 > 24) || (m_iHour12 < 0) || (m_iHour12 > 24) ||	\
		(m_iHour21 < 0) || (m_iHour21 > 24) || (m_iHour22 < 0) || (m_iHour22 > 24) ||	\
		(m_iHour31 < 0) || (m_iHour31 > 24) || (m_iHour32 < 0) || (m_iHour32 > 24) ||	\
		(m_iHour41 < 0) || (m_iHour41 > 24) || (m_iHour42 < 0) || (m_iHour42 > 24) ||   \
		(m_iHour51 < 0) || (m_iHour51 > 24) || (m_iHour52 < 0) || (m_iHour52 > 24) ||	\
		(m_iHour61 < 0) || (m_iHour61 > 24) || (m_iHour62 < 0) || (m_iHour62 > 24) ||	\
		(m_iHour71 < 0) || (m_iHour71 > 24) || (m_iHour72 < 0) || (m_iHour72 > 24) ||	\
		(m_iHour81 < 0) || (m_iHour81 > 24) || (m_iHour82 < 0) || (m_iHour82 > 24))
	{
		g_StringLanType(szLan, "小时范围在0 -- 24之间", "Hour between 0 - 24 ");
		AfxMessageBox(szLan);
		return FALSE;
	}
	if ((m_iMin11 < 0) || (m_iMin11 > 60) || (m_iMin12 < 0) || (m_iMin12 > 60) ||	\
		(m_iMin21 < 0) || (m_iMin21 > 60) || (m_iMin22 < 0) || (m_iMin22 > 60) ||	\
		(m_iMin31 < 0) || (m_iMin31 > 60) || (m_iMin32 < 0) || (m_iMin32 > 60) ||	\
		(m_iMin41 < 0) || (m_iMin41 > 60) || (m_iMin42 < 0) || (m_iMin42 > 60) ||   \
		(m_iMin51 < 0) || (m_iMin51 > 60) || (m_iMin52 < 0) || (m_iMin52 > 60) ||	\
		(m_iMin61 < 0) || (m_iMin61 > 60) || (m_iMin62 < 0) || (m_iMin62 > 60) ||	\
		(m_iMin71 < 0) || (m_iMin71 > 60) || (m_iMin72 < 0) || (m_iMin72 > 60) ||	\
		(m_iMin81 < 0) || (m_iMin81 > 60) || (m_iMin82 < 0) || (m_iMin82 > 60))
	{
		g_StringLanType(szLan, "分钟范围在0 -- 60之间", "Minute between 0 - 60 ");
		AfxMessageBox(szLan);
		return FALSE;
	}
	wStartTime[0] = (WORD)(m_iHour11 * 100 + m_iMin11);
	wStartTime[1] = (WORD)(m_iHour21 * 100 + m_iMin21);
	wStartTime[2] = (WORD)(m_iHour31 * 100 + m_iMin31);
	wStartTime[3] = (WORD)(m_iHour41 * 100 + m_iMin41);
	wStopTime[0] = (WORD)(m_iHour12 * 100 + m_iMin12);
	wStopTime[1] = (WORD)(m_iHour22 * 100 + m_iMin22);
	wStopTime[2] = (WORD)(m_iHour32 * 100 + m_iMin32);
	wStopTime[3] = (WORD)(m_iHour42 * 100 + m_iMin42);
	wStartTime[4] = (WORD)(m_iHour51 * 100 + m_iMin51);
	wStartTime[5] = (WORD)(m_iHour61 * 100 + m_iMin61);
	wStartTime[6] = (WORD)(m_iHour71 * 100 + m_iMin71);
	wStartTime[7] = (WORD)(m_iHour81 * 100 + m_iMin81);
	wStopTime[4] = (WORD)(m_iHour52 * 100 + m_iMin52);
	wStopTime[5] = (WORD)(m_iHour62 * 100 + m_iMin62);
	wStopTime[6] = (WORD)(m_iHour72 * 100 + m_iMin72);
	wStopTime[7] = (WORD)(m_iHour82 * 100 + m_iMin82);
	for (i = 0; i < MAX_TIMESEGMENT_V30; i++)
	{
		if ((wStartTime[i] > 2400) || (wStopTime[i] > 2400))
		{
			g_StringLanType(szLan, "布防时间不能超过24:00", "Fortify time can not be more than 24:00");
			AfxMessageBox(szLan);
			return FALSE;
		}
		if (wStartTime[i] > wStopTime[i])
		{
			g_StringLanType(szLan, "布防起始时间不能大于停止时间", "Fortify start time can not be larger than the record to stop time");
			AfxMessageBox(szLan);
			return FALSE;
		}
		for (j = 0; j < MAX_TIMESEGMENT_V30; j++)
		{
			if (i == j)
			{
				continue;
			}
			if ((wStartTime[i] > wStartTime[j]) && (wStartTime[i] < wStopTime[j]) ||	\
				(wStopTime[i] > wStartTime[j]) && (wStopTime[i] < wStopTime[j]))
			{
				g_StringLanType(szLan, "时间段不能重复", "time can not be repeated");
				AfxMessageBox(szLan);
				return FALSE;
			}
		}
	}
	return TRUE;
}

/*********************************************************
Function:	OnSelchangeComboweekday
Desc:		
Input:	
Output:	
Return:	void 
**********************************************************/
void CDlgVcaRuleCfgEx::OnSelchangeComboweekday() 
{
    // TODO: Add your control notification handler code here
    int	i = m_comboWeekday.GetCurSel();	
    SetAlarmTimeToWnd(&m_struVcaRule.m_struRuleCfg.struRule[m_iCurRuleID].struAlarmTime[0][0], i);
    UpdateData(FALSE);
}

/*********************************************************
Function:	OnCheckhandletype4
Desc:		
Input:	
Output:	
Return:	void 
**********************************************************/
void CDlgVcaRuleCfgEx::OnCheckhandletype4() 
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    m_listAlarmOut.EnableWindow(m_bHandleType4);
    UpdateData(FALSE);
    
}

/*********************************************************
Function:	OnChangeEditRuleName
Desc:		Modify rule name
Input:	
Output:	
Return:	void 
**********************************************************/
void CDlgVcaRuleCfgEx::OnChangeEditRuleName() 
{
    // TODO: If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialog::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.
    
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    memset(&m_struOneRule.byRuleName[0], 0, sizeof(BYTE)*NAME_LEN);
    strncpy((char*)&m_struOneRule.byRuleName[0], m_csRuleName.GetBuffer(0), (NAME_LEN - 1));
}

void CDlgVcaRuleCfgEx::SetVcaRuleToWnd(NET_VCA_RULECFG_V42 const &struVcaRuleCfg)
{
    SetUploadCfgToWnd(struVcaRuleCfg);
    SetPtzPositionToWnd(struVcaRuleCfg.struPTZPosition);
    //将当前ruleID的规则显示到界面
    memcpy(&m_struOneRule, &struVcaRuleCfg.struRule[m_comboRuleID.GetCurSel()], sizeof(m_struOneRule));
    SetOneVcaRuleToWnd(m_struOneRule);
    EnableRuleWindow(m_bRuleActive);
}

void CDlgVcaRuleCfgEx::GetVcaRuleFromWnd(NET_VCA_RULECFG_V42 &struVcaRuleCfg)
{

}

void CDlgVcaRuleCfgEx::SetOneVcaRuleToWnd(NET_VCA_ONE_RULE_V42 const &struVcaOneRule)
{
    m_bRuleActive = struVcaOneRule.byActive;
    m_csRuleName = struVcaOneRule.byRuleName;
    
    SetEventTypeToWnd(struVcaOneRule.wEventType);
    m_combEventPriority.SetCurSel(struVcaOneRule.byEventPriority);
    m_comboAlarmDelay.SetCurSel(struVcaOneRule.wAlarmDelay);
    
    //参数设置到界面(显示参数控件，然后赋值给界面，顺序不能换 )
    ShowRuleTypeWnd((VCA_RULE_EVENT_TYPE_EX)struVcaOneRule.wEventType);
    SetEventUnionToWnd(struVcaOneRule.uEventParam, struVcaOneRule.wEventType);
    

    
    //报警处理方式
    SetHandleExceptionToWnd(struVcaOneRule.struAlarmHandleType);
    //布防时间
    int nDay = m_comboWeekday.GetCurSel();
    SetAlarmTimeToWnd(&struVcaOneRule.struAlarmTime[0][0], nDay);
   
    //录像关联通道
    SetRelRecordChanToWnd(struVcaOneRule.dwRelRecordChan, g_struDeviceInfo[m_iDevIndex].iDeviceChanNum);

    //尺寸过滤器 
    SetSizeFilterToWnd(struVcaOneRule.struSizeFilter);
    //尺寸过滤策略
	SetFilterStrategyToWnd(struVcaOneRule.struFilterStrategy);
    //规则触发参数
	SetRuleTriggerParamToWnd(struVcaOneRule.struTriggerParam);
}

void CDlgVcaRuleCfgEx::GetOneVcaRuleFromWnd(NET_VCA_ONE_RULE_V42 &struOneRule)
{   
    struOneRule.byActive = m_bRuleActive;
    strncpy((char*)struOneRule.byRuleName, m_csRuleName.GetBuffer(0), sizeof(struOneRule.byRuleName));

    struOneRule.wEventType = GetEventTypeFromWnd();
    struOneRule.byEventPriority = m_combEventPriority.GetCurSel();
	struOneRule.wAlarmDelay = m_comboAlarmDelay.GetCurSel();
    
    GetEventUnionFromWnd(struOneRule.uEventParam, struOneRule.wEventType);
    //报警处理方式
    GetHandleExceptionFromWnd(struOneRule.struAlarmHandleType);
    //布防时间
    int nDay = m_comboWeekday.GetCurSel();
    GetAlarmTimeFromWnd(&struOneRule.struAlarmTime[0][0], m_comboWeekday.GetCurSel());

    //录像关联通道
    GetRelRecordChanFromWnd(struOneRule.dwRelRecordChan, g_struDeviceInfo[m_iDevIndex].iDeviceChanNum);

    //尺寸过滤器 
    GetSizeFilterFromWnd(struOneRule.struSizeFilter);
    //尺寸过滤策略
    GetFilterStrategyFromWnd(&struOneRule.struFilterStrategy);
    //规则触发参数
    GetRuleTriggerParamFromWnd(&struOneRule.struTriggerParam);
}

void CDlgVcaRuleCfgEx::SetEventTypeToWnd(DWORD dwEventType)
{
    for (int i = 0; i < m_comboRuleType.GetCount(); i++)
    {
        if (dwEventType == m_comboRuleType.GetItemData(i))
        {
            m_comboRuleType.SetCurSel(i);
            break;
        }
    }
}

DWORD CDlgVcaRuleCfgEx::GetEventTypeFromWnd()
{
    return m_comboRuleType.GetItemData(m_comboRuleType.GetCurSel());
}

//获取的规则参数放到界面-规则参数-中
void CDlgVcaRuleCfgEx::SetEventUnionToWnd(NET_VCA_EVENT_UNION const & uEventParam, DWORD dwEventType)
{
    BYTE bySensitivity = 0;
    switch (dwEventType)
    {
    case ENUM_VCA_EVENT_TRAVERSE_PLANE:
        memcpy(&m_struVcaLine[m_iCurRuleID], &uEventParam.struTraversePlane.struPlaneBottom, sizeof(NET_VCA_LINE));
        m_comboRuleParam1.SetCurSel(uEventParam.struTraversePlane.dwCrossDirection);
        m_iRuleParam3 = uEventParam.struTraversePlane.bySensitivity;
        break;
    case ENUM_VCA_EVENT_REACH_HIGHT:
        memcpy(&m_struVcaLine[m_iCurRuleID], &uEventParam.struReachHight.struVcaLine, sizeof(NET_VCA_LINE));
        m_iRuleParam3 = uEventParam.struReachHight.wDuration;
        break;
    case ENUM_VCA_EVENT_ENTER_AREA:
        memcpy(&m_struVcaPolygon[m_iCurRuleID], &uEventParam.struArea.struRegion, sizeof(NET_VCA_POLYGON));
        break;
    case ENUM_VCA_EVENT_EXIT_AREA:
        memcpy(&m_struVcaPolygon[m_iCurRuleID], &uEventParam.struArea.struRegion, sizeof(NET_VCA_POLYGON));
        break;
    case ENUM_VCA_EVENT_INTRUSION:
        memcpy(&m_struVcaPolygon[m_iCurRuleID], &uEventParam.struIntrusion.struRegion, sizeof(NET_VCA_POLYGON));
        m_iRuleParam3 = uEventParam.struIntrusion.wDuration;
        m_iRuleParam5 = uEventParam.struIntrusion.bySensitivity;
        break;
    case ENUM_VCA_EVENT_LOITER:
        memcpy(&m_struVcaPolygon[m_iCurRuleID], &uEventParam.struLoiter.struRegion, sizeof(NET_VCA_POLYGON));
        m_iRuleParam3 = uEventParam.struLoiter.wDuration;
        break;
    case ENUM_VCA_EVENT_LEFT_TAKE:
        memcpy(&m_struVcaPolygon[m_iCurRuleID], &uEventParam.struTakeTeft.struRegion, sizeof(NET_VCA_POLYGON));
        m_iRuleParam3 = uEventParam.struTakeTeft.wDuration;
        break;
    case ENUM_VCA_EVENT_PARKING:
        memcpy(&m_struVcaPolygon[m_iCurRuleID], &uEventParam.struParking.struRegion, sizeof(NET_VCA_POLYGON));
        m_iRuleParam3 = uEventParam.struParking.wDuration;
        break;
    case ENUM_VCA_EVENT_RUN:
        memcpy(&m_struVcaPolygon[m_iCurRuleID], &uEventParam.struRun.struRegion, sizeof(NET_VCA_POLYGON));
        m_iRuleParam3 = uEventParam.struRun.fRunDistance;
        m_comboRuleParam1.SetCurSel(uEventParam.struRun.byMode);
        m_iRuleParam5 = uEventParam.struRun.bySensitivity;
        break;
    case ENUM_VCA_EVENT_HIGH_DENSITY:
        memcpy(&m_struVcaPolygon[m_iCurRuleID], &uEventParam.struHighDensity.struRegion, sizeof(NET_VCA_POLYGON));
        m_iRuleParam3 = uEventParam.struHighDensity.fDensity;
        m_iRuleParam5 = uEventParam.struHighDensity.bySensitivity;
        break;
    case ENUM_VCA_EVENT_VIOLENT_MOTION:
        memcpy(&m_struVcaPolygon[m_iCurRuleID], &uEventParam.struViolentMotion.struRegion, sizeof(NET_VCA_POLYGON));
        m_iRuleParam3 = uEventParam.struViolentMotion.wDuration;
        m_iRuleParam5 = uEventParam.struViolentMotion.bySensitivity;
        m_comboRuleParam1.SetCurSel(uEventParam.struViolentMotion.byMode);
        break;
    case ENUM_VCA_EVENT_GET_UP:
	case ENUM_VCA_EVENT_KEY_PERSON_GET_UP:
        memcpy(&m_struVcaPolygon[m_iCurRuleID], &uEventParam.struGetUp.struRegion, sizeof(NET_VCA_POLYGON));
        m_iRuleParam3 = uEventParam.struGetUp.wDuration;
        m_comboRuleParam1.SetCurSel(uEventParam.struGetUp.byMode);
        m_iRuleParam5 = uEventParam.struGetUp.bySensitivity;
        break;
    case ENUM_VCA_EVENT_LEFT:
        memcpy(&m_struVcaPolygon[m_iCurRuleID], &uEventParam.struLeft.struRegion, sizeof(NET_VCA_POLYGON));
        m_iRuleParam3 = uEventParam.struLeft.wDuration;
        m_iRuleParam5 = uEventParam.struLeft.bySensitivity;
        break;
    case ENUM_VCA_EVENT_TAKE:
        memcpy(&m_struVcaPolygon[m_iCurRuleID], &uEventParam.struTake.struRegion, sizeof(NET_VCA_POLYGON));
        m_iRuleParam3 = uEventParam.struTake.wDuration;
        m_iRuleParam5 = uEventParam.struTake.bySensitivity;
        break;

    case ENUM_VCA_EVENT_OVER_TIME:
//         NET_VCA_POLYGON     struRegion;    // 区域范围
//         HPR_UINT16  wDuration;  // 操作报警时间阈值
//         HPR_UINT8   byRes[6];   // 保留字节
//     }NET_VCA_OVER_TIME, *LPNET_VCA_OVER_TIME;
        memcpy(&m_struVcaPolygon[m_iCurRuleID], &uEventParam.struOvertime.struRegion, sizeof(NET_VCA_POLYGON));
        m_iRuleParam3 = uEventParam.struOvertime.wDuration;
        break;
    case ENUM_VCA_EVENT_STICK_UP:
        memcpy(&m_struVcaPolygon[m_iCurRuleID], &uEventParam.struStickUp.struRegion, sizeof(NET_VCA_POLYGON));
        m_iRuleParam3 = uEventParam.struStickUp.wDuration;
        m_iRuleParam5 = uEventParam.struStickUp.bySensitivity;
        break;

    case ENUM_VCA_EVENT_INSTALL_SCANNER:
        memcpy(&m_struVcaPolygon[m_iCurRuleID], &uEventParam.struScanner.struRegion, sizeof(NET_VCA_POLYGON));
        m_iRuleParam3 = uEventParam.struScanner.wDuration;
        m_iRuleParam5 = uEventParam.struScanner.bySensitivity;
        break;
	case ENUM_VCA_EVENT_LEAVE_POSITION:
		memcpy(&m_struVcaPolygon[m_iCurRuleID], &uEventParam.struLeavePos.struRegion, sizeof(NET_VCA_POLYGON));
        m_iRuleParam3 = uEventParam.struLeavePos.wLeaveDelay;
        m_iRuleParam4 = uEventParam.struLeavePos.wStaticDelay;
        m_comboRuleParam1.SetCurSel(uEventParam.struLeavePos.byMode);
        m_comboRuleParam2.SetCurSel(uEventParam.struLeavePos.byPersonType);
		
        m_iRuleParam5 = uEventParam.struLeavePos.bySensitivity;
        m_iRuleParam6 = uEventParam.struLeavePos.byOnPosition;
		break;
	case ENUM_VCA_EVENT_TRAIL:
        memcpy(&m_struVcaPolygon[m_iCurRuleID], &uEventParam.struTrail.struRegion, sizeof(NET_VCA_POLYGON));
        m_iRuleParam3 = uEventParam.struTrail.bySensitivity;
		break;
	case ENUM_VCA_EVENT_FALL_DOWN:
        memcpy(&m_struVcaPolygon[m_iCurRuleID], &uEventParam.struFallDown.struRegion, sizeof(NET_VCA_POLYGON));
        m_iRuleParam4 = uEventParam.struFallDown.byHeightThreshold;
        m_iRuleParam3 = uEventParam.struFallDown.wDuration;
        m_iRuleParam5 = uEventParam.struFallDown.bySensitivity;
		break;

    case ENUM_VCA_EVENT_AUDIO_ABNORMAL:
        m_comboRuleParam1.SetCurSel(uEventParam.struAudioAbnormal.byAudioMode);
        m_comboRuleParam2.SetCurSel(uEventParam.struAudioAbnormal.byEnable);
        m_iRuleParam3 = uEventParam.struAudioAbnormal.wDecibel;
        m_iRuleParam4 = uEventParam.struAudioAbnormal.bySensitivity;
        m_iRuleParam5 = uEventParam.struAudioAbnormal.byThreshold;
       
        break;
    case ENUM_VCA_EVENT_ADV_REACH_HEIGHT:
        memcpy(&m_struVcaPolygon[m_iCurRuleID], &uEventParam.struReachHeight.struRegion, sizeof(NET_VCA_POLYGON));
        m_comboRuleParam1.SetCurSel(uEventParam.struReachHeight.dwCrossDirection);
        break;
    case ENUM_VCA_EVENT_TOILET_TARRY:
        memcpy(&m_struVcaPolygon[m_iCurRuleID], &uEventParam.struToiletTarry.struRegion, sizeof(NET_VCA_POLYGON));
        m_iRuleParam3 = uEventParam.struToiletTarry.wDelay;
        break;
    case ENUM_VCA_EVENT_YARD_TARRY:
        memcpy(&m_struVcaPolygon[m_iCurRuleID], &uEventParam.struYardTarry.struRegion, sizeof(NET_VCA_POLYGON));
        m_iRuleParam3 = uEventParam.struYardTarry.wDelay;
        break;
    case ENUM_VCA_EVENT_ADV_TRAVERSE_PLANE:
        memcpy(&m_struVcaPolygon[m_iCurRuleID], &uEventParam.struAdvTraversePlane.struRegion, sizeof(NET_VCA_POLYGON));
        m_comboRuleParam1.SetCurSel(uEventParam.struAdvTraversePlane.dwCrossDirection);
        m_iRuleParam3 = uEventParam.struAdvTraversePlane.bySensitivity;
        break;
	case ENUM_VCA_EVENT_STANDUP:
		memcpy(&m_struVcaPolygon[m_iCurRuleID], &uEventParam.struStandUp.struRegion, sizeof(NET_VCA_POLYGON));
        m_iRuleParam5 = uEventParam.struStandUp.bySensitivity;
        m_iRuleParam3 = uEventParam.struStandUp.wDuration;
        m_iRuleParam4 = uEventParam.struStandUp.byHeightThreshold;
		break;
	case ENUM_VCA_EVENT_PEOPLENUM_CHANGE:
		memcpy(&m_struVcaPolygon[m_iCurRuleID], &uEventParam.struPeopleNumChange.struRegion, sizeof(NET_VCA_POLYGON));
        m_iRuleParam5 = uEventParam.struPeopleNumChange.bySensitivity;
        m_iRuleParam4 = uEventParam.struPeopleNumChange.byPeopleNumThreshold;
        m_comboRuleParam2.SetCurSel(uEventParam.struPeopleNumChange.byDetectMode - 1);
        m_comboRuleParam1.SetCurSel(uEventParam.struPeopleNumChange.byNoneStateEffective);
        m_iRuleParam3 = uEventParam.struPeopleNumChange.wDuration;
		break;
	case ENUM_VCA_EVENT_SPACING_CHANGE:
		memcpy(&m_struVcaPolygon[m_iCurRuleID], &uEventParam.struSpacingChange.struRegion, sizeof(NET_VCA_POLYGON));
        m_iRuleParam5 = uEventParam.struSpacingChange.bySensitivity;
        m_comboRuleParam1.SetCurSel(uEventParam.struSpacingChange.byDetectMode - 1);
        m_iRuleParam4 = uEventParam.struSpacingChange.fSpacingThreshold;
        m_iRuleParam3 = uEventParam.struSpacingChange.wDuration;
		break;
    case ENUM_VCA_EVENT_FAKECARD:
        memcpy(&m_struVcaPolygon[m_iCurRuleID], &uEventParam.struFakeCard.struRegion, sizeof(NET_VCA_POLYGON));
        m_iRuleParam5 = uEventParam.struFakeCard.bySensitivity;
        break;
    default:
        break;
    }
    m_dwPosNum = m_struVcaPolygon[m_iCurRuleID].dwPointNum;

}
//从界面-规则参数-中获取规则参数
void CDlgVcaRuleCfgEx::GetEventUnionFromWnd(NET_VCA_EVENT_UNION &uEventParam, DWORD dwEventType)
{
	UpdateData(TRUE);
    BYTE bySensity = 0;
    switch (dwEventType)
    {
    case ENUM_VCA_EVENT_TRAVERSE_PLANE:
        memcpy(&uEventParam.struTraversePlane.struPlaneBottom, &m_struVcaLine[m_iCurRuleID], sizeof(NET_VCA_LINE));
        uEventParam.struTraversePlane.dwCrossDirection = (VCA_CROSS_DIRECTION)m_comboRuleParam1.GetCurSel();
        if (m_iRuleParam3 < 0 || m_iRuleParam3>255)
        {
            AfxMessageBox("灵敏度参数范围超过一字节无符号类型（0-255）");
        }
        else
        {
            uEventParam.struTraversePlane.bySensitivity = (BYTE)m_iRuleParam3; 
        }
        uEventParam.struTraversePlane.byDetectionTarget = (BYTE)m_comboRuleParam2.GetCurSel();
        break;

    case ENUM_VCA_EVENT_REACH_HIGHT:
        uEventParam.struReachHight.wDuration = m_iRuleParam3;
        memcpy(&uEventParam.struReachHight.struVcaLine, &m_struVcaLine[m_iCurRuleID], sizeof(NET_VCA_LINE));
        break;

    case ENUM_VCA_EVENT_ENTER_AREA:
        memcpy(&uEventParam.struArea.struRegion, &m_struVcaPolygon[m_iCurRuleID], sizeof(NET_VCA_POLYGON));
        if (m_iRuleParam3 < 0 || m_iRuleParam3>255)
        {
            AfxMessageBox("灵敏度参数越界，取值范围（0~255）");
        }
        else
        {
            uEventParam.struArea.bySensitivity = m_iRuleParam3;
        }
        
        break;
    case ENUM_VCA_EVENT_EXIT_AREA:
        memcpy(&uEventParam.struArea.struRegion, &m_struVcaPolygon[m_iCurRuleID], sizeof(NET_VCA_POLYGON));
        if (m_iRuleParam3 < 0 || m_iRuleParam3>255)
        {
            AfxMessageBox("灵敏度参数越界，取值范围（0~255）");
        }
        else
        {
            uEventParam.struArea.bySensitivity = m_iRuleParam3;
        }
        break;
    case ENUM_VCA_EVENT_INTRUSION:
        uEventParam.struIntrusion.wDuration = (WORD)m_iRuleParam3;
        memcpy(&uEventParam.struIntrusion.struRegion, &m_struVcaPolygon[m_iCurRuleID], sizeof(NET_VCA_POLYGON));
        if (m_iRuleParam5 < 0 || m_iRuleParam5>255)
        {
            AfxMessageBox("灵敏度参数越界，取值范围（0~255）");
        }
        else
        {
            uEventParam.struIntrusion.bySensitivity = (BYTE)m_iRuleParam5;
        }
        break;
    case ENUM_VCA_EVENT_LOITER:
        uEventParam.struLoiter.wDuration = (WORD)m_iRuleParam3;
        memcpy(&uEventParam.struLoiter.struRegion, &m_struVcaPolygon[m_iCurRuleID], sizeof(NET_VCA_POLYGON));
        break;
    case ENUM_VCA_EVENT_LEFT_TAKE:
        uEventParam.struTakeTeft.wDuration = (WORD)m_iRuleParam3;
        memcpy(&uEventParam.struTakeTeft.struRegion, &m_struVcaPolygon[m_iCurRuleID], sizeof(NET_VCA_POLYGON));
        break;
    case ENUM_VCA_EVENT_PARKING:
        uEventParam.struParking.wDuration = (WORD)m_iRuleParam3;
        memcpy(&uEventParam.struParking.struRegion, &m_struVcaPolygon[m_iCurRuleID], sizeof(NET_VCA_POLYGON));
        break;
    case ENUM_VCA_EVENT_RUN:
        uEventParam.struRun.fRunDistance = m_iRuleParam3;
        memcpy(&uEventParam.struRun.struRegion, &m_struVcaPolygon[m_iCurRuleID], sizeof(NET_VCA_POLYGON));
        uEventParam.struRun.byMode = m_comboRuleParam1.GetCurSel();
        if (m_iRuleParam5 < 0 || m_iRuleParam5>255)
        {
            AfxMessageBox("灵敏度参数越界，取值范围（0~255）");
        }
        else
        {
            uEventParam.struRun.bySensitivity = m_iRuleParam5;
        }
        break;
    case ENUM_VCA_EVENT_HIGH_DENSITY:
        uEventParam.struHighDensity.fDensity = m_iRuleParam3;
        if (m_iRuleParam5 < 0 || m_iRuleParam5>255)
        {
            AfxMessageBox("灵敏度参数越界，取值范围（0~255）");
        }
        else
        {
            uEventParam.struHighDensity.bySensitivity = m_iRuleParam5;
        }
        memcpy(&uEventParam.struHighDensity.struRegion, &m_struVcaPolygon[m_iCurRuleID], sizeof(NET_VCA_POLYGON));
        break;
    case ENUM_VCA_EVENT_VIOLENT_MOTION:
        uEventParam.struViolentMotion.wDuration = m_iRuleParam3;
        memcpy(&uEventParam.struViolentMotion.struRegion, &m_struVcaPolygon[m_iCurRuleID], sizeof(NET_VCA_POLYGON));
        if (m_iRuleParam5 < 0 || m_iRuleParam5>255)
        {
            AfxMessageBox("灵敏度参数越界，取值范围（0~255）");
        }
        else
        {
            uEventParam.struViolentMotion.bySensitivity = (BYTE)m_iRuleParam5;
        }
        uEventParam.struViolentMotion.byMode = m_comboRuleParam1.GetCurSel();
        break;
    case ENUM_VCA_EVENT_GET_UP:
	case ENUM_VCA_EVENT_KEY_PERSON_GET_UP:
        uEventParam.struGetUp.wDuration = m_iRuleParam3;
        memcpy(&uEventParam.struGetUp.struRegion, &m_struVcaPolygon[m_iCurRuleID], sizeof(NET_VCA_POLYGON));
        uEventParam.struGetUp.byMode = m_comboRuleParam1.GetCurSel();
        if (m_iRuleParam5 < 0 || m_iRuleParam5>255)
        {
            AfxMessageBox("灵敏度参数越界，取值范围（0~255）");
        }
        else
        {
            uEventParam.struGetUp.bySensitivity = (BYTE)m_iRuleParam5;
        }
        break;
    case ENUM_VCA_EVENT_LEFT:
        uEventParam.struLeft.wDuration = m_iRuleParam3;
        memcpy(&uEventParam.struLeft.struRegion, &m_struVcaPolygon[m_iCurRuleID], sizeof(NET_VCA_POLYGON));
        if (m_iRuleParam5 < 0 || m_iRuleParam5>255)
        {
            AfxMessageBox("灵敏度参数越界，取值范围（0~255）");
        }
        else
        {
            uEventParam.struLeft.bySensitivity = (BYTE)m_iRuleParam5;
        }
        break;
    case ENUM_VCA_EVENT_TAKE:
        uEventParam.struTake.wDuration = m_iRuleParam3;
        memcpy(&uEventParam.struTake.struRegion, &m_struVcaPolygon[m_iCurRuleID], sizeof(NET_VCA_POLYGON));
        if (m_iRuleParam5 < 0 || m_iRuleParam5>255)
        {
            AfxMessageBox("灵敏度参数越界，取值范围（0~255）");
        }
        else
        {
            uEventParam.struTake.bySensitivity = (BYTE)m_iRuleParam5;
        }
        break;
    case ENUM_VCA_EVENT_OVER_TIME:
        memcpy(&uEventParam.struOvertime.struRegion, &m_struVcaPolygon[m_iCurRuleID], sizeof(NET_VCA_POLYGON));
        uEventParam.struOvertime.wDuration = (WORD)m_iRuleParam3;
        break;
    case ENUM_VCA_EVENT_STICK_UP:
        memcpy(&uEventParam.struStickUp.struRegion, &m_struVcaPolygon[m_iCurRuleID], sizeof(NET_VCA_POLYGON));
        uEventParam.struStickUp.wDuration = (WORD)m_iRuleParam3;
        if (m_iRuleParam5 < 0 || m_iRuleParam5>255)
        {
            AfxMessageBox("灵敏度参数越界，取值范围（0~255）");
        }
        else
        {
            uEventParam.struStickUp.bySensitivity = (BYTE)m_iRuleParam5;
        }
        break;
    case ENUM_VCA_EVENT_INSTALL_SCANNER:
        memcpy(&uEventParam.struScanner.struRegion, &m_struVcaPolygon[m_iCurRuleID], sizeof(NET_VCA_POLYGON));
        uEventParam.struScanner.wDuration = (WORD)m_iRuleParam3;
        if (m_iRuleParam5 < 0 || m_iRuleParam5>255)
        {
            AfxMessageBox("灵敏度参数越界，取值范围（0~255）");
        }
        else
        {
            uEventParam.struScanner.bySensitivity = (BYTE)m_iRuleParam5;
        }
        break;
	case ENUM_VCA_EVENT_LEAVE_POSITION:
        memcpy(&uEventParam.struLeavePos.struRegion, &m_struVcaPolygon[m_iCurRuleID], sizeof(NET_VCA_POLYGON));
        uEventParam.struLeavePos.wLeaveDelay = m_iRuleParam3;
        uEventParam.struLeavePos.wStaticDelay = m_iRuleParam4;
        uEventParam.struLeavePos.byMode = m_comboRuleParam1.GetCurSel();
        uEventParam.struLeavePos.byPersonType = m_comboRuleParam2.GetCurSel();
        if (m_iRuleParam5 < 0 || m_iRuleParam5>255)
        {
            AfxMessageBox("灵敏度参数越界，取值范围（0~255）");
        }
        else
        {
            uEventParam.struLeavePos.bySensitivity = (BYTE)m_iRuleParam5;
        }
        uEventParam.struLeavePos.byOnPosition = (BYTE)m_iRuleParam6;
		break;
	case ENUM_VCA_EVENT_TRAIL:
        memcpy(&uEventParam.struTrail.struRegion, &m_struVcaPolygon[m_iCurRuleID], sizeof(NET_VCA_POLYGON));
        if (m_iRuleParam3 < 0 || m_iRuleParam3>255)
        {
            AfxMessageBox("灵敏度参数越界，取值范围（0~255）");
        }
        else
        {
            uEventParam.struTrail.bySensitivity = (BYTE)m_iRuleParam3;
        }
		break;
	case ENUM_VCA_EVENT_FALL_DOWN:
        memcpy(&uEventParam.struFallDown.struRegion, &m_struVcaPolygon[m_iCurRuleID], sizeof(NET_VCA_POLYGON));
        uEventParam.struFallDown.byHeightThreshold = m_iRuleParam4;
        uEventParam.struFallDown.wDuration = m_iRuleParam3;
        if (m_iRuleParam5 < 0 || m_iRuleParam5>255)
        {
            AfxMessageBox("灵敏度参数越界，取值范围（0~255）");
        }
        else
        {
            uEventParam.struFallDown.bySensitivity = (BYTE)m_iRuleParam5;
        }
		break;
    case ENUM_VCA_EVENT_AUDIO_ABNORMAL:
        uEventParam.struAudioAbnormal.byAudioMode = m_comboRuleParam1.GetCurSel();
        uEventParam.struAudioAbnormal.byEnable = m_comboRuleParam2.GetCurSel();
        if (m_iRuleParam4 < 0 || m_iRuleParam4>255)
        {
            AfxMessageBox("灵敏度参数越界，取值范围（0~255）");
        }
        else
        {
            uEventParam.struAudioAbnormal.bySensitivity = (BYTE)m_iRuleParam4;
        }
        uEventParam.struAudioAbnormal.wDecibel = m_iRuleParam3;
        uEventParam.struAudioAbnormal.byThreshold = m_iRuleParam5;
        break;
    case ENUM_VCA_EVENT_ADV_REACH_HEIGHT:
        uEventParam.struReachHeight.dwCrossDirection = (VCA_CROSS_DIRECTION)m_comboRuleParam1.GetCurSel();
        memcpy(&uEventParam.struReachHeight.struRegion, &m_struVcaPolygon[m_iCurRuleID], sizeof(NET_VCA_POLYGON));
        break;
    case ENUM_VCA_EVENT_TOILET_TARRY:
        memcpy(&uEventParam.struToiletTarry.struRegion, &m_struVcaPolygon[m_iCurRuleID], sizeof(NET_VCA_POLYGON));
        uEventParam.struToiletTarry.wDelay = (WORD)m_iRuleParam3;
        break;
    case ENUM_VCA_EVENT_YARD_TARRY:
        memcpy(&uEventParam.struYardTarry.struRegion, &m_struVcaPolygon[m_iCurRuleID], sizeof(NET_VCA_POLYGON));
        uEventParam.struYardTarry.wDelay = m_iRuleParam3;
        break;
    case ENUM_VCA_EVENT_ADV_TRAVERSE_PLANE:
        memcpy(&uEventParam.struAdvTraversePlane.struRegion, &m_struVcaPolygon[m_iCurRuleID], sizeof(NET_VCA_POLYGON));
        uEventParam.struAdvTraversePlane.dwCrossDirection = (VCA_CROSS_DIRECTION)m_comboRuleParam1.GetCurSel();
        if (m_iRuleParam3 < 0 || m_iRuleParam3>255)
        {
            AfxMessageBox("灵敏度参数越界，取值范围（0~255）");
        }
        else
        {
            uEventParam.struAdvTraversePlane.bySensitivity = (BYTE)m_iRuleParam3;
        }
        break;
	case ENUM_VCA_EVENT_STANDUP:
		memcpy(&uEventParam.struStandUp.struRegion, &m_struVcaPolygon[m_iCurRuleID], sizeof(NET_VCA_POLYGON));
        if (m_iRuleParam5 < 0 || m_iRuleParam5>255)
        {
            AfxMessageBox("灵敏度参数越界，取值范围（0~255）");
        }
        else
        {
            uEventParam.struStandUp.bySensitivity = (BYTE)m_iRuleParam5;
        }
        uEventParam.struStandUp.wDuration = m_iRuleParam3;
        uEventParam.struStandUp.byHeightThreshold = m_iRuleParam4;
		break;
	case ENUM_VCA_EVENT_PEOPLENUM_CHANGE:
		memcpy(&uEventParam.struPeopleNumChange.struRegion, &m_struVcaPolygon[m_iCurRuleID], sizeof(NET_VCA_POLYGON));
        if (m_iRuleParam5 < 0 || m_iRuleParam5>255)
        {
            AfxMessageBox("灵敏度参数越界，取值范围（0~255）");
        }
        else
        {
            uEventParam.struPeopleNumChange.bySensitivity = (BYTE)m_iRuleParam5;
        }
        uEventParam.struPeopleNumChange.byPeopleNumThreshold = m_iRuleParam4;
        uEventParam.struPeopleNumChange.byDetectMode = (BYTE)m_comboRuleParam2.GetCurSel()+1;
        uEventParam.struPeopleNumChange.byNoneStateEffective = m_comboRuleParam1.GetCurSel();
        uEventParam.struPeopleNumChange.wDuration = m_iRuleParam3;
		break;
	case ENUM_VCA_EVENT_SPACING_CHANGE:
		memcpy(&uEventParam.struSpacingChange.struRegion, &m_struVcaPolygon[m_iCurRuleID], sizeof(NET_VCA_POLYGON));
        if (m_iRuleParam5 < 0 || m_iRuleParam5>255)
        {
            AfxMessageBox("灵敏度参数越界，取值范围（0~255）");
        }
        else
        {
            uEventParam.struSpacingChange.bySensitivity = (BYTE)m_iRuleParam5;
        }
        uEventParam.struSpacingChange.byDetectMode = (BYTE)(m_comboRuleParam1.GetCurSel() + 1);
        uEventParam.struSpacingChange.fSpacingThreshold = m_iRuleParam4;
        uEventParam.struSpacingChange.wDuration = m_iRuleParam3;
		break;
    case ENUM_VCA_EVENT_FAKECARD:
        memcpy(&uEventParam.struFakeCard.struRegion, &m_struVcaPolygon[m_iCurRuleID], sizeof(NET_VCA_POLYGON));
        if (m_iRuleParam5 < 1 || m_iRuleParam5>5)
        {
            AfxMessageBox("灵敏度参数越界，取值范围（1~5）");
        }
        else
        {
            uEventParam.struFakeCard.bySensitivity = (BYTE)m_iRuleParam5;
        }
        break;
    default:
        break;
    }

}

void CDlgVcaRuleCfgEx::SetSizeFilterToWnd(NET_VCA_SIZE_FILTER const &struSizeFilter)
{
    m_bFilterActive = struSizeFilter.byActive;
    m_comboFilterMode.SetCurSel(struSizeFilter.byMode);
    OnSelchangeComboFilterMode();
    if (IMAGE_PIX_MODE == struSizeFilter.byMode)
    {
        memcpy(&m_struSizeFilter.struMaxRect, &struSizeFilter.struMaxRect, sizeof(m_struSizeFilter.struMaxRect));
        memcpy(&m_struSizeFilter.struMiniRect, &struSizeFilter.struMiniRect, sizeof(m_struSizeFilter.struMiniRect));
    }
    else if (REAL_WORLD_MODE == struSizeFilter.byMode)
    {
        m_fMinHeight = struSizeFilter.struMiniRect.fHeight;
        m_fMinWidth = struSizeFilter.struMiniRect.fWidth;
        m_fMaxHeight = struSizeFilter.struMaxRect.fHeight;
        m_fMaxWidth = struSizeFilter.struMaxRect.fWidth;
    }
    else if (DEFAULT_MODE == struSizeFilter.byMode)
    {

    }
}

void CDlgVcaRuleCfgEx::GetSizeFilterFromWnd(NET_VCA_SIZE_FILTER &struSizeFilter)
{
    struSizeFilter.byActive = m_bFilterActive;
    struSizeFilter.byMode = m_comboFilterMode.GetItemData(m_comboFilterMode.GetCurSel());
    if (IMAGE_PIX_MODE == struSizeFilter.byMode)
    {
        memcpy(&struSizeFilter.struMaxRect, &m_struSizeFilter.struMaxRect, sizeof(struSizeFilter.struMaxRect));
        memcpy(&struSizeFilter.struMiniRect, &m_struSizeFilter.struMiniRect, sizeof(struSizeFilter.struMiniRect));
    }
    else if (REAL_WORLD_MODE == struSizeFilter.byMode)
    {
        struSizeFilter.struMaxRect.fHeight = m_fMaxHeight;
        struSizeFilter.struMaxRect.fWidth = m_fMaxWidth;
        struSizeFilter.struMiniRect.fHeight = m_fMinHeight;
        struSizeFilter.struMiniRect.fWidth = m_fMinWidth;
    }

//     struSizeFilter.byMode = m_comboFilterMode.GetCurSel();
//     if (REAL_WORLD_MODE == struSizeFilter.byMode)
//     {
//         struSizeFilter.struMiniRect.fHeight = m_fMinHeight;
//         struSizeFilter.struMiniRect.fWidth = m_fMinWidth;
//         struSizeFilter.struMaxRect.fHeight = m_fMaxHeight;
//         struSizeFilter.struMaxRect.fWidth = m_fMaxWidth;
//     }

}

void CDlgVcaRuleCfgEx::SetHandleExceptionToWnd(NET_DVR_HANDLEEXCEPTION_V40 const &struHandleException)
{
    m_bHandleType1 = (struHandleException.dwHandleType) &0x01;
    m_bHandleType2 = (struHandleException.dwHandleType>>1)&0x01;
    m_bHandleType3 = (struHandleException.dwHandleType>>2)&0x01;
    m_bHandleType4 = (struHandleException.dwHandleType>>3)&0x01;
    m_bHandleType5 = (struHandleException.dwHandleType>>4)&0x01;
    
    //报警输出通道
    char szLan[128] = {0};
    int i=0;

    m_listAlarmOut.SetExtendedStyle(LVS_EX_CHECKBOXES);
    m_listAlarmOut.DeleteAllItems();

    int nIndex = 0;
    for (nIndex = 0; nIndex < struHandleException.dwMaxRelAlarmOutChanNum; nIndex++)
    {
        sprintf(szLan, "AlarmOut%d", nIndex);
        m_listAlarmOut.InsertItem(nIndex, szLan);
        m_listAlarmOut.SetItemData(nIndex, nIndex+1);//报警输出通道号从1开始
    }
    
    for (i = 0; i<struHandleException.dwMaxRelAlarmOutChanNum; i++)
	{
        int nCurChan = struHandleException.dwRelAlarmOut[i];
        if (nCurChan != 0xffffffff)
        {
            for (nIndex = 0; nIndex <m_listAlarmOut.GetItemCount(); nIndex++)
            {
                if (nCurChan == m_listAlarmOut.GetItemData(nIndex))
                {
                    m_listAlarmOut.SetCheck(nIndex);
                }
            }
        }
	}
    m_listAlarmOut.EnableWindow(m_bHandleType4);
    UpdateData(FALSE);
}

void CDlgVcaRuleCfgEx::GetHandleExceptionFromWnd(NET_DVR_HANDLEEXCEPTION_V40 &struHandleException)
{

    UpdateData(TRUE);
    struHandleException.dwHandleType = 0;
    struHandleException.dwHandleType |= (m_bHandleType1 << 0);
    struHandleException.dwHandleType |= (m_bHandleType2 << 1);
    struHandleException.dwHandleType |= (m_bHandleType3 << 2);
    struHandleException.dwHandleType |= (m_bHandleType4 << 3);
    struHandleException.dwHandleType |= (m_bHandleType5 << 4);
    
    //关联报警输出通道
    struHandleException.dwRelAlarmOutChanNum = 0;
    memset(&struHandleException.dwRelAlarmOut, 0xffffffff, sizeof(struHandleException.dwRelAlarmOut));
    if (m_bHandleType4)
    {
        int iChannelIndex = 0;
        for (int i = 0; i < m_listAlarmOut.GetItemCount(); i++)
        {
            if (m_listAlarmOut.GetCheck(i))
            {
                struHandleException.dwRelAlarmOut[iChannelIndex++] = m_listAlarmOut.GetItemData(i);
                struHandleException.dwRelAlarmOutChanNum++;
            }
        }
    }
}

void CDlgVcaRuleCfgEx::SetRelRecordChanToWnd(DWORD const *dwRelRecordChan, int nChanNum)
{
    m_listRecordChan.SetExtendedStyle(LVS_EX_CHECKBOXES);
    char szLan[128] = {0};
    int i;
    m_listRecordChan.DeleteAllItems();
    
	int nIndex = 0;
	for (i = g_struDeviceInfo[m_iDevIndex].iStartChan; i  <= g_struDeviceInfo[m_iDevIndex].iAnalogChanNum ; i++)
	{
		sprintf(szLan, "Analog Chan%d", i);
		m_listRecordChan.InsertItem(nIndex,szLan);
		m_listRecordChan.SetItemData(nIndex, i);
		nIndex++;
	}
    
    for (i = 0; i  < g_struDeviceInfo[m_iDevIndex].pStruIPParaCfgV40->dwDChanNum ; i++)
	{
		sprintf(szLan, "IP Chan%d", i + 1);
		m_listRecordChan.InsertItem(nIndex,szLan);
		m_listRecordChan.SetItemData(nIndex, i + g_struDeviceInfo[m_iDevIndex].pStruIPParaCfgV40->dwStartDChan);
		nIndex++;
	}

    for (int j = 0; j < MAX_CHANNUM_V30; j++)
    {
        //如果关联通道号为0xffffffff，则表示不关联
        if (dwRelRecordChan[j] != 0xffffffff)
        {
            for (i = 0; i < m_listRecordChan.GetItemCount(); i++)
            {
                int nChanNumber = m_listRecordChan.GetItemData(i);
                if (dwRelRecordChan[j] == nChanNumber)
                {
                    m_listRecordChan.SetCheck(i);
                }
            }
        }
    }
}
void CDlgVcaRuleCfgEx::GetRelRecordChanFromWnd(DWORD  *dwRelRecordChan, int nChanNum)
{   
    memset(dwRelRecordChan, 0xffffffff, MAX_CHANNUM_V30);
    int iChaneIndex = 0;
    for (int i = 0; i < m_listRecordChan.GetItemCount(); i++)
    {   
		if (m_listRecordChan.GetCheck(i))
		{
			int nChanNumber = m_listRecordChan.GetItemData(i);
            dwRelRecordChan[iChaneIndex++] = nChanNumber; //通道号从1开始
		}
    }
}

void CDlgVcaRuleCfgEx::SetAlarmTimeToWnd(NET_DVR_SCHEDTIME const *pAlarmTime, int nDay)
{
    m_iHour11 = pAlarmTime[nDay*8+0].byStartHour;
    m_iMin11 = pAlarmTime[nDay*8+0].byStartMin;
    m_iHour12 = pAlarmTime[nDay*8+0].byStopHour;
    m_iMin12 = pAlarmTime[nDay*8+0].byStopMin;
    m_iHour21 = pAlarmTime[nDay*8+1].byStartHour;
    m_iMin21 = pAlarmTime[nDay*8+1].byStartMin;
    m_iHour22 = pAlarmTime[nDay*8+1].byStopHour;
    m_iMin22 = pAlarmTime[nDay*8+1].byStopMin;
	
	m_iHour31 = pAlarmTime[nDay*8+2].byStartHour;
	m_iMin31 = pAlarmTime[nDay*8+2].byStartMin;
	m_iHour32 = pAlarmTime[nDay*8+2].byStopHour;
	m_iMin32 = pAlarmTime[nDay*8+2].byStopMin;
	m_iHour41 = pAlarmTime[nDay*8+3].byStartHour;
	m_iMin41 = pAlarmTime[nDay*8+3].byStartMin;
	m_iHour42 = pAlarmTime[nDay*8+3].byStopHour;
	m_iMin42 = pAlarmTime[nDay*8+3].byStopMin;
	
	m_iHour51 = pAlarmTime[nDay*8+4].byStartHour;
	m_iMin51 = pAlarmTime[nDay*8+4].byStartMin;
	m_iHour52 = pAlarmTime[nDay*8+4].byStopHour;
	m_iMin52 = pAlarmTime[nDay*8+4].byStopMin;
	m_iHour61 = pAlarmTime[nDay*8+5].byStartHour;
	m_iMin61 = pAlarmTime[nDay*8+5].byStartMin;
	m_iHour62 = pAlarmTime[nDay*8+5].byStopHour;
	m_iMin62 = pAlarmTime[nDay*8+5].byStopMin;
	
	m_iHour71 = pAlarmTime[nDay*8+6].byStartHour;
	m_iMin71 = pAlarmTime[nDay*8+6].byStartMin;
	m_iHour72 = pAlarmTime[nDay*8+6].byStopHour;
	m_iMin72 = pAlarmTime[nDay*8+6].byStopMin;
	m_iHour81 = pAlarmTime[nDay*8+7].byStartHour;
	m_iMin81 = pAlarmTime[nDay*8+7].byStartMin;
	m_iHour82 = pAlarmTime[nDay*8+7].byStopHour;
	m_iMin82 = pAlarmTime[nDay*8+7].byStopMin;
    UpdateData(FALSE);
}

void CDlgVcaRuleCfgEx::GetAlarmTimeFromWnd(NET_DVR_SCHEDTIME *pAlarmTime, int nDay)
{
    pAlarmTime[nDay*8+0].byStartHour = (BYTE)m_iHour11;
    pAlarmTime[nDay*8+0].byStartMin = (BYTE)m_iMin11;
    pAlarmTime[nDay*8+0].byStopHour = (BYTE)m_iHour12;
    pAlarmTime[nDay*8+0].byStopMin = (BYTE)m_iMin12;
    pAlarmTime[nDay*8+1].byStartHour = (BYTE)m_iHour21;
    pAlarmTime[nDay*8+1].byStartMin = (BYTE)m_iMin21;
    pAlarmTime[nDay*8+1].byStopHour = (BYTE)m_iHour22;
    pAlarmTime[nDay*8+1].byStopMin = (BYTE)m_iMin22;
	
	pAlarmTime[nDay*8+2].byStartHour = (BYTE)m_iHour31;
    pAlarmTime[nDay*8+2].byStartMin = (BYTE)m_iMin31;
    pAlarmTime[nDay*8+2].byStopHour = (BYTE)m_iHour32;
    pAlarmTime[nDay*8+2].byStopMin = (BYTE)m_iMin32;
    pAlarmTime[nDay*8+3].byStartHour = (BYTE)m_iHour41;
    pAlarmTime[nDay*8+3].byStartMin = (BYTE)m_iMin41;
    pAlarmTime[nDay*8+3].byStopHour = (BYTE)m_iHour42;
    pAlarmTime[nDay*8+3].byStopMin = (BYTE)m_iMin42;
	
	pAlarmTime[nDay*8+4].byStartHour = (BYTE)m_iHour51;
    pAlarmTime[nDay*8+4].byStartMin = (BYTE)m_iMin51;
    pAlarmTime[nDay*8+4].byStopHour = (BYTE)m_iHour52;
    pAlarmTime[nDay*8+4].byStopMin = (BYTE)m_iMin52;
    pAlarmTime[nDay*8+5].byStartHour = (BYTE)m_iHour61;
    pAlarmTime[nDay*8+5].byStartMin = (BYTE)m_iMin61;
    pAlarmTime[nDay*8+5].byStopHour = (BYTE)m_iHour62;
    pAlarmTime[nDay*8+5].byStopMin = (BYTE)m_iMin62;
	
	pAlarmTime[nDay*8+6].byStartHour = (BYTE)m_iHour71;
    pAlarmTime[nDay*8+6].byStartMin = (BYTE)m_iMin71;
    pAlarmTime[nDay*8+6].byStopHour = (BYTE)m_iHour72;
    pAlarmTime[nDay*8+6].byStopMin = (BYTE)m_iMin72;
    pAlarmTime[nDay*8+7].byStartHour = (BYTE)m_iHour81;
    pAlarmTime[nDay*8+7].byStartMin = (BYTE)m_iMin81;
    pAlarmTime[nDay*8+7].byStopHour = (BYTE)m_iHour82;
    pAlarmTime[nDay*8+7].byStopMin = (BYTE)m_iMin82;
}

void CDlgVcaRuleCfgEx::SetFilterStrategyToWnd(NET_VCA_FILTER_STRATEGY const& struFilterStrategy)
{
	int nCount = m_comboFilterStrategy.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		if (m_comboFilterStrategy.GetItemData(i) == struFilterStrategy.byStrategy)
		{
			m_comboFilterStrategy.SetCurSel(i);
			break;
		}
	}
    UpdateData(FALSE);
}

void CDlgVcaRuleCfgEx::GetFilterStrategyFromWnd(NET_VCA_FILTER_STRATEGY* pFilterStrategy)
{
	int nCurSel = m_comboFilterStrategy.GetCurSel();
	if (nCurSel != CB_ERR)
	{
		pFilterStrategy->byStrategy = m_comboFilterStrategy.GetItemData(nCurSel);
	}
}

void CDlgVcaRuleCfgEx::SetRuleTriggerParamToWnd(NET_VCA_RULE_TRIGGER_PARAM const& struRuleTriggerParam)
{   
	int i = 0;
	DWORD dwTriggerMode = struRuleTriggerParam.byTriggerMode;

	int nTriggerModeCount = m_comboTriggerMode.GetCount();
	for (i = 0; i < nTriggerModeCount; i++)
	{
		if (m_comboTriggerMode.GetItemData(i) == struRuleTriggerParam.byTriggerMode)
		{
			m_comboTriggerMode.SetCurSel(i);
			break;
		}
	}

	if (dwTriggerMode == TRIGGER_MODE_TRACKPOINT)
	{
		int nTriggerPtCount = m_comboTriggerPoint.GetCount();
		for (i = 0; i < nTriggerPtCount; i++)
		{
			if (m_comboTriggerPoint.GetItemData(i) == struRuleTriggerParam.byTriggerPoint)
			{
				m_comboTriggerPoint.SetCurSel(i);
				break;
			}
		}
	}
	else if (dwTriggerMode == TRIGGER_MODE_TARGETAREA)
	{
		m_fTriggerArea = struRuleTriggerParam.fTriggerArea;
	}
    UpdateData(FALSE);
}

void CDlgVcaRuleCfgEx::GetRuleTriggerParamFromWnd(NET_VCA_RULE_TRIGGER_PARAM* pRuleTriggerParam)
{   
	DWORD dwTriggerMode    = TRIGGER_MODE_NONE;
	int nTriggerModeIndex = m_comboTriggerMode.GetCurSel();
	if (nTriggerModeIndex != CB_ERR)
	{   
		dwTriggerMode = m_comboTriggerMode.GetItemData(nTriggerModeIndex);
		pRuleTriggerParam->byTriggerMode = dwTriggerMode;
	}

	if (dwTriggerMode == TRIGGER_MODE_TRACKPOINT)
	{
		int nTrackPtIndex = m_comboTriggerPoint.GetCurSel();
		if (nTrackPtIndex != CB_ERR)
		{
			pRuleTriggerParam->byTriggerPoint = m_comboTriggerPoint.GetItemData(nTrackPtIndex);
		}
	}
	else if (dwTriggerMode == TRIGGER_MODE_TARGETAREA)
	{
		pRuleTriggerParam->fTriggerArea = m_fTriggerArea;
	}
}

LONG CDlgVcaRuleCfgEx::StartPlay()
{
    //  如果能去前端流设备
	/*
    if (GetPuStreamCfg(m_struPuStream))
    {
        NET_DVR_DEVICEINFO_V30 struDeviceInfo = {0};
        
        m_lPUServerID = NET_DVR_Login_V30(m_struPuStream.struDevChanInfo.struIP.sIpV4, m_struPuStream.struDevChanInfo.wDVRPort,
            (char*)m_struPuStream.struDevChanInfo.sUserName, (char*)m_struPuStream.struDevChanInfo.sPassword, &struDeviceInfo);
        if (m_lPUServerID < 0)
        {
            AfxMessageBox("Fail to login front device");
            return m_lPUServerID;
        }
        
        NET_DVR_CLIENTINFO struPlay;
        struPlay.hPlayWnd 	= GetDlgItem(IDC_PLAYWND)->GetSafeHwnd();
        struPlay.lChannel 	= m_struPuStream.struDevChanInfo.byChannel; 
        struPlay.lLinkMode 	= 0;
        struPlay.sMultiCastIP = "";	
        
        m_lPlayHandle = NET_DVR_RealPlay_V30(m_lPUServerID, &struPlay, NULL, NULL, TRUE); 
        
        BOOL bRet = FALSE;
        if (m_lPlayHandle < 0)
        {
            g_pMainDlg->AddLog(-1, OPERATION_FAIL_T, "NET_DVR_RealPlay_V30");
            AfxMessageBox("NET_DVR_RealPlay_V30 FAIL");		
        }
        else
        {
            g_pMainDlg->AddLog(-1, OPERATION_SUCC_T, "NET_DVR_RealPlay_V30");
            bRet = NET_DVR_RigisterDrawFun(m_lPlayHandle, DrawAlertShow, 0);//DC Draw callback
        }
        return m_lPlayHandle;
    }
	*/
//   else // 到设备去取流预览
    {
    NET_DVR_CLIENTINFO struPlay;
    struPlay.hPlayWnd 	= GetDlgItem(IDC_PLAYWND)->GetSafeHwnd();
    struPlay.lChannel = m_cmbVCAChannelID.GetItemData(m_cmbVCAChannelID.GetCurSel());
    struPlay.lLinkMode 	= 0;
    struPlay.sMultiCastIP = "";	
    
    //先关闭已有的播放
    if (m_lPlayHandle >= 0)
    {
        NET_DVR_StopRealPlay(m_lPlayHandle);
        m_lPlayHandle = -1;
    }
    m_lPlayHandle = NET_DVR_RealPlay_V30(m_lServerID, &struPlay, NULL, NULL, TRUE); 
    BOOL bRet = FALSE;
    if (m_lPlayHandle < 0)
    {
        g_pMainDlg->AddLog(-1, OPERATION_FAIL_T, "NET_DVR_RealPlay_V30");
        AfxMessageBox("NET_DVR_RealPlay_V30 FAIL");		
    }
    else
    {
        g_pMainDlg->AddLog(-1, OPERATION_SUCC_T, "NET_DVR_RealPlay_V30");
        bRet = NET_DVR_RigisterDrawFun(m_lPlayHandle, DrawAlertShowEx, 0);//DC Draw callback
    }

    return m_lPlayHandle;
}

}


BOOL CDlgVcaRuleCfgEx::StopPlay()
{
    if (m_lPlayHandle >= 0)
    {
        NET_DVR_StopRealPlay(m_lPlayHandle);
        m_lPlayHandle = -1;
    }

    if (m_lPUServerID >= 0)
    {
        NET_DVR_Logout_V30(m_lPUServerID);
        m_lPUServerID = -1;
    }
    return TRUE;
}

void CDlgVcaRuleCfgEx::PostNcDestroy() 
{
    if (m_lPlayHandle >= 0)
    {
        NET_DVR_StopRealPlay(m_lPlayHandle);
        m_lPlayHandle = -1;
    }
    g_pDlgVcaRuleCfgEx = NULL;
	
	CDialog::PostNcDestroy();
}

void CDlgVcaRuleCfgEx::OnSelchangeComboFilterMode() 
{
    DWORD dwFilterMode = m_comboFilterMode.GetItemData(m_comboFilterMode.GetCurSel());
    ShowDefaultMode(SW_HIDE);
    if (IMAGE_PIX_MODE == dwFilterMode)
    {
        ShowPixMode(SW_SHOW);
    }
    else if (REAL_WORLD_MODE == dwFilterMode)
    {
        ShowRealMode(SW_SHOW);
    }
    else if (DEFAULT_MODE == dwFilterMode)
    {
        
    }
}

void CDlgVcaRuleCfgEx::ShowPixMode(int nCmdShow)
{
    GetDlgItem(IDC_CHECK_SET_MINI_FILTER)->ShowWindow(nCmdShow);
    GetDlgItem(IDC_CHECK_SET_MAX_FILTER)->ShowWindow(nCmdShow);
}
void CDlgVcaRuleCfgEx::ShowRealMode(int nCmdShow)
{
    GetDlgItem(IDC_STATIC_MIN)->ShowWindow(nCmdShow);
    GetDlgItem(IDC_STATIC_MIN_H)->ShowWindow(nCmdShow);
    GetDlgItem(IDC_STATIC_MIN_W)->ShowWindow(nCmdShow);
    GetDlgItem(IDC_STATIC_MAX)->ShowWindow(nCmdShow);
    GetDlgItem(IDC_STATIC_MAX_W)->ShowWindow(nCmdShow);
    GetDlgItem(IDC_STATIC_MAX_H)->ShowWindow(nCmdShow);
    GetDlgItem(IDC_EDIT_MIN_HEIGHT)->ShowWindow(nCmdShow);
    GetDlgItem(IDC_EDIT_MIN_WIDTH)->ShowWindow(nCmdShow);
    GetDlgItem(IDC_EDIT_MAX_HEIGHT)->ShowWindow(nCmdShow);
    GetDlgItem(IDC_EDIT_MAX_WIDTH)->ShowWindow(nCmdShow);
}

void CDlgVcaRuleCfgEx::ShowDefaultMode(int nCmdShow)
{
    ShowRealMode(nCmdShow);
    ShowPixMode(nCmdShow);
}

BOOL CDlgVcaRuleCfgEx::GetPuStreamCfg(NET_DVR_PU_STREAM_CFG &struPUStream)
{
    DWORD dwReturn = 0;
    if (NET_DVR_GetDVRConfig(m_lServerID, NET_DVR_GET_PU_STREAMCFG,m_iVcaChannel, &struPUStream, sizeof(struPUStream), &dwReturn))
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_GET_PU_STREAMCFG");
        return TRUE;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_GET_PU_STREAMCFG");
        return FALSE;
    }
}

void CDlgVcaRuleCfgEx::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDialog::OnPaint();
	// Do not call CDialog::OnPaint() for painting messages
}


/*********************************************************
Function:	CreateTree
Desc:		Create alarm output-video channel connection
Input:	
Output:	
Return:	
**********************************************************/

void CDlgVcaRuleCfgEx::OnSelchangeComboTriggermode()
{
    int nCurSel = m_comboTriggerMode.GetCurSel();
    if (nCurSel != CB_ERR)
    {
        DWORD dwTriggerMode = m_comboTriggerMode.GetItemData(nCurSel);

        m_comboTriggerPoint.EnableWindow(dwTriggerMode == TRIGGER_MODE_TRACKPOINT);
        GetDlgItem(IDC_EDIT_TRIGGERAREA)->EnableWindow(dwTriggerMode == TRIGGER_MODE_TARGETAREA);
    }
}

//保存当前规则内容
void CDlgVcaRuleCfgEx::OnBnClickedBtnSaveOneRule()
{
    // TODO:  
    UpdateData(TRUE);
    GetOneVcaRuleFromWnd(m_struVcaRule.m_struRuleCfg.struRule[m_iCurRuleID]);
}

void CDlgVcaRuleCfgEx::OnBnClickedCancel()
{
    // TODO:  在此添加控件通知处理程序代码
    if (m_lPlayHandle >= 0)
    {
        NET_DVR_StopRealPlay(m_lPlayHandle);
        m_lPlayHandle = -1;
    }
    CDialog::OnCancel();
}
/*获取界面上-智能通道配置--信息*/
void CDlgVcaRuleCfgEx::GetGroupInfoFromWnd()
{
    UpdateData(TRUE);
    memset(&m_struChanGroup, 0, sizeof(m_struChanGroup));
    m_struChanGroup.dwSize = sizeof(m_struChanGroup);
    m_struChanGroup.dwChannel = m_cmbVCAChannelID.GetItemData(m_cmbVCAChannelID.GetCurSel());
    m_struChanGroup.dwGroup = m_cmbVCAChannelGroupNo.GetCurSel();
    m_struChanGroup.byID = 1; // m_comboPositionIndex.GetCurSel() + 1;
    m_struChanGroup.dwPositionNo = 0; // m_comboPositionIndex.GetCurSel() + 1;

}
//从界面的--报警上传配置---中获取参数
void    CDlgVcaRuleCfgEx::GetUploadCfgFromWnd()
{
    UpdateData(TRUE);
    m_struVcaRule.m_struRuleCfg.byPicProType = (unsigned char)m_comboProPicType.GetCurSel();
    m_struVcaRule.m_struRuleCfg.byUpLastAlarm = (unsigned char)m_cmUpLastAlarm.GetCurSel();
    m_struVcaRule.m_struRuleCfg.byPicRecordEnable = m_bChkPicRecordEn;
    m_struVcaRule.m_struRuleCfg.struPicParam.wPicSize = (unsigned short)m_comboResolution.GetItemData(m_comboResolution.GetCurSel());
    m_struVcaRule.m_struRuleCfg.struPicParam.wPicQuality = (unsigned short)m_comboPicQuality.GetCurSel();
    m_struVcaRule.m_struRuleCfg.wRelSnapChan[0] = m_comboRelSnapChan1.GetItemData(m_comboRelSnapChan1.GetCurSel());
    m_struVcaRule.m_struRuleCfg.wRelSnapChan[1] = m_comboRelSnapChan2.GetItemData(m_comboRelSnapChan2.GetCurSel());
    m_struVcaRule.m_struRuleCfg.wRelSnapChan[2] = m_comboRelSnapChan3.GetItemData(m_comboRelSnapChan3.GetCurSel());
}
//设置参数到界面中的--报警上传配置---
void    CDlgVcaRuleCfgEx::SetUploadCfgToWnd(NET_VCA_RULECFG_V42 const &struVcaRuleCfg)
{
    m_cmUpLastAlarm.SetCurSel(struVcaRuleCfg.byUpLastAlarm);
    m_bChkPicRecordEn = struVcaRuleCfg.byPicRecordEnable;
    m_comboProPicType.SetCurSel(struVcaRuleCfg.byPicProType);
    m_comboPicQuality.SetCurSel(struVcaRuleCfg.struPicParam.wPicQuality);
    //找到对应的分辨率索引,并显示
    for (int i = 0; i < m_comboResolution.GetCount(); i++)
    {
        if (m_comboResolution.GetItemData(i) == struVcaRuleCfg.struPicParam.wPicSize)
        {
            m_comboResolution.SetCurSel(i);
        }
    }
    //找到对应的关联通道
    for (int i = 0; i < g_struDeviceInfo[m_iDevIndex].iDeviceChanNum; i++)
    {
        if (m_comboRelSnapChan1.GetItemData(i) == struVcaRuleCfg.wRelSnapChan[0])
        {
            m_comboRelSnapChan1.SetCurSel(i);
        }
        if (m_comboRelSnapChan2.GetItemData(i) == struVcaRuleCfg.wRelSnapChan[1])
        {
            m_comboRelSnapChan2.SetCurSel(i);
        }
        if (m_comboRelSnapChan3.GetItemData(i) == struVcaRuleCfg.wRelSnapChan[2])
        {
            m_comboRelSnapChan3.SetCurSel(i);
        }
    }
    UpdateData(FALSE);
}
//从界面 获取场景位置信息
void  CDlgVcaRuleCfgEx::GetPtzPositionFromWnd()
{
    //TODO 目前无需求
}
//将场景位置信息显示到界面
void  CDlgVcaRuleCfgEx::SetPtzPositionToWnd(NET_DVR_PTZ_POSITION const &struPTZPosition)
{
    //TODO 目前无需求
}

void CDlgVcaRuleCfgEx::OnBnClickedBtnGetRuleCfg()
{
    // TODO:  在此添加控件通知处理程序代码
    //获取通道信息
    GetGroupInfoFromWnd();
    //设置规则信息
    if (!GetVcaRuleCfg())
    {
        char szLan[128] = "";
        sprintf(szLan, "Fail to Get Channal Group[%d]  Channel[%d] VCA behavior rule",\
            m_struChanGroup.dwGroup, m_struChanGroup.dwChannel);
        AfxMessageBox(szLan);
    }
    //获取&显示获取到的参数
    SetVcaRuleToWnd(m_struVcaRule.m_struRuleCfg);
}


void CDlgVcaRuleCfgEx::OnCbnSelchangeComboChannelList()
{
    // TODO:  在此添加控件通知处理程序代码
    StartPlay();
    ChangeResolutionItem();
}
//根据设备返回的能力，修改分辨率选项
void  CDlgVcaRuleCfgEx::ChangeResolutionItem()
{
    GetVcaChanAbilityXml();
    m_comboResolution.ResetContent();
    for (int i = 0; i < m_struVcaChanAbilityXml.iAlarmPicResolutionListLen; i++)
    {
        ALARM_PIC_RESOLUTION struAlarmPicRes = { 0 };
        memcpy(&struAlarmPicRes, &m_struVcaChanAbilityXml.struAlarmPicResolutionList[i], sizeof(struAlarmPicRes));
        m_comboResolution.InsertString(i, struAlarmPicRes.szResolution);
        m_comboResolution.SetItemData(i, struAlarmPicRes.iIndex);
    }
    m_comboResolution.SetCurSel(0);
}
