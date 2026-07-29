#if !defined(AFX_DLGVCARULECFGEX_H__B678DBAB_24C9_4063_A990_784B443D35E7__INCLUDED_)
#define AFX_DLGVCARULECFGEX_H__B678DBAB_24C9_4063_A990_784B443D35E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgVcaRuleCfgEx.h : header file
//
# define RULE_PARAM_IDS_NUM 12  //规则参数控件数

/////////////////////////////////////////////////////////////////////////////
// CDlgVcaRuleCfgEx dialog
#include "DrawFun.h"
#include "afxwin.h"
#include "afxcmn.h"
class CDlgVcaRuleCfgEx : public CDialog
{
// Construction
public:
	CDlgVcaRuleCfgEx(CWnd* pParent = NULL);   // standard constructor
    enum
	{
		TRIGGER_MODE_NONE       = 0,
		TRIGGER_MODE_TRACKPOINT = 1,
		TRIGGER_MODE_TARGETAREA = 2
	};

	enum
	{
		TRIGGER_POINT_CENTER = 0,
		TRIGGER_POINT_UP     = 1,
		TRIGGER_POINT_DOWN   = 2,
	};
	enum
	{
		FILTER_STRATEGY_NONE             = 0,
		FILTER_STRATEGY_WIDTH_AND_HEIGHT = 1,
		FILTER_STRATEGY_AERA             = 2
	};
// Dialog Data
	//{{AFX_DATA(CDlgVcaRuleCfgEx)
	enum { IDD = IDD_DLG_VCA_RULE_CFG_EX};
	CComboBox	m_comDetectMode;
	CComboBox	m_comboRelSnapChan3;
	CComboBox	m_comboRelSnapChan2;
	CComboBox	m_comboRelSnapChan1;
	CComboBox	m_comboFilterStrategy;
	CComboBox	m_comboTriggerPoint;
	CComboBox	m_comboTriggerMode;
	CStatic	m_staticDelayTime;
	CComboBox	m_comboAlarmDelay;
	CComboBox	m_cmUpLastAlarm;
	CComboBox	m_comboFilterMode;
	CListCtrl	m_listRecordChan;
	CComboBox	m_comboWeekday;
	CComboBox	m_comboCopyTime;
	CComboBox	m_comboRuleID;
    CComboBox m_comboRuleType;
    CComboBox m_comboPicQuality;
    CComboBox m_comboProPicType;
	BOOL	m_bFilterActive;
	BOOL	m_bSetMaxFilter;
	BOOL	m_bSetMiniFilter;
	BOOL	m_bHandleType1;
	BOOL	m_bHandleType2;
	BOOL	m_bHandleType3;
	BOOL	m_bHandleType4;
	BOOL	m_bHandleType5;
	BOOL	m_bDrawPolygon;
	BOOL	m_bRuleActive;
	CString	m_csRuleName;

	int		m_iHour11;
	int		m_iHour12;
	int		m_iHour21;
	int		m_iHour22;
	int		m_iMin11;
	int		m_iMin12;
	int		m_iMin21;
	int		m_iMin22;
	BOOL	m_bDrawLine;
	float	m_fMinHeight;
	float	m_fMinWidth;
	float	m_fMaxHeight;
	float	m_fMaxWidth;
	BOOL	m_bChkPicRecordEn;
	int		m_iHour31;
	int		m_iHour32;
	int		m_iHour41;
	int		m_iHour42;
	int		m_iHour51;
	int		m_iHour52;
	int		m_iHour61;
	int		m_iHour62;
	int		m_iHour71;
	int		m_iHour72;
	int		m_iHour81;
	int		m_iHour82;
	int		m_iMin31;
	int		m_iMin32;
	int		m_iMin41;
	int		m_iMin42;
	int		m_iMin51;
	int		m_iMin52;
	int		m_iMin61;
	int		m_iMin62;
	int		m_iMin71;
	int		m_iMin72;
	int		m_iMin81;
	int		m_iMin82;
	float	m_fTriggerArea;

    CEdit m_editRuleParam4;
    CEdit m_editRuleParam5;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgVcaRuleCfgEx)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgVcaRuleCfgEx)
	virtual BOOL OnInitDialog();
    afx_msg void OnBtnSetOneRule();
	afx_msg void OnSelchangeComboRuleId();
	afx_msg void OnChkRuleActive();
	afx_msg void OnSelchangeComboRuleType();
	afx_msg void OnChkDrawPolygon();
	afx_msg void OnCheckFilterActive();
	afx_msg void OnCheckSetMiniFilter();
	afx_msg void OnCheckSetMaxFilter();
	afx_msg void OnChkDrawLine();
	afx_msg void OnTimeOk();
	afx_msg void OnTimeCopy();
	afx_msg void OnSelchangeComboweekday();
	afx_msg void OnCheckhandletype4();
	afx_msg void OnChangeEditRuleName();
	afx_msg void OnSelchangeComboFilterMode();
	afx_msg void OnPaint();
	afx_msg void OnSelchangeComboTriggermode();
    afx_msg void OnBnClickedBtnSaveOneRule();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
        
       

public:
    //类的结构体
    struct VCA_RULECFG{
        NET_VCA_RULECFG_V42 m_struRuleCfg;
    };
    
    VCA_RULECFG m_struVcaRule;  //保存界面规则参数
    NET_DVR_CHANNEL_GROUP  m_struChanGroup;//保存界面通道参数
    UINT m_staticRuleParamIDs[RULE_PARAM_IDS_NUM];   //参数控件 用于批量隐藏
    NET_VCA_ONE_RULE_V42 m_struOneRule;

    //报警图片分辨率 结构体
    struct ALARM_PIC_RESOLUTION
    {
        int iIndex;
        char szName[20];
        char szResolution[10];
    };
    //DEVICE_ABILITY_INFO VcaChanAbility能力结构体
    struct VCA_CHAN_ABILITY_XML{
        int iAlarmPicResolutionListLen; //报警图片分辨率能力数字大小
        ALARM_PIC_RESOLUTION struAlarmPicResolutionList[100]; //具体的报警图片分辨率能力
    };
    VCA_CHAN_ABILITY_XML m_struVcaChanAbilityXml;

	//Data
	LONG	m_lServerID;
	int     m_iDevIndex;
	int		m_iVcaChannel; 


	//NET_VCA_RULECFG_V41 m_struRuleCfg;  
	NET_VCA_BEHAVIOR_ABILITY m_struBehaviorAbility;
    NET_VCA_CHAN_IN_PARAM m_struVcaChanInParam;
    NET_VCA_CTRLCFG     m_struVcaCtrlCfg;
    NET_VCA_SIZE_FILTER m_struSizeFilter;
    NET_DVR_PU_STREAM_CFG m_struPuStream;
	DWORD m_dwEventType;

    
	CRect m_rcWnd; 
	long m_lPlayHandle; 

	DWORD m_dwPosNum;  
    NET_VCA_POLYGON m_struVcaPolygon[MAX_RULE_NUM_V42];
    NET_VCA_LINE m_struVcaLine[MAX_RULE_NUM_V42];
    BOOL m_bCloseIn[MAX_RULE_NUM_V42];
    BOOL m_bNeedRedraw[MAX_RULE_NUM_V42];
    BOOL m_bMouseMove[MAX_RULE_NUM_V42];
	BOOL m_bAlarmOut[MAX_ALARMOUT_V30];

	int  m_iCurRuleID; 

    void F_DrawFun(long lPlayHandle, HDC hDc, DWORD dwUser);

    LONG m_lPUServerID;

	BYTE m_byVCAType;
	BYTE m_byMode;
private:
    BOOL    GetVcaCtrlCfg();
    BOOL    GetVcaChanAbility();
    BOOL    GetVcaRuleCfg();
    BOOL    SetVcaRuleCfg();
    BOOL    GetVcaParam();
    BOOL    GetVcaChanAbilityXml(); //获取DEVICE_ABILITY_INFO中VcaChanAbility

    

    void InitWnd();
    BOOL InitRuleType(int iBehaviorAbilityType, int iBehaviorAbilityTypeEx);  
    BOOL InitRuleID(int RuleNum); 
    void   GetGroupInfoFromWnd();  //从界面获取通道信息
    void   GetUploadCfgFromWnd(); //从界面的--报警上传配置---中获取参数
    void   SetUploadCfgToWnd(NET_VCA_RULECFG_V42 const &struVcaRuleCfg);//设置参数到界面中的--报警上传配置---
    void   GetPtzPositionFromWnd(); //从界面 获取场景位置信息
    void   SetPtzPositionToWnd(NET_DVR_PTZ_POSITION const &struPTZPosition); //将场景位置信息显示到界面
    void   ChangeResolutionItem(); //根据设备返回的能力，修改分辨率选项
    void SetVcaRuleToWnd(NET_VCA_RULECFG_V42 const &struVcaRuleCfg);
    void GetVcaRuleFromWnd(NET_VCA_RULECFG_V42 &struVcaRuleCfg);
    void SetOneVcaRuleToWnd(NET_VCA_ONE_RULE_V42 const &struVcaOneRule);
    void GetOneVcaRuleFromWnd(NET_VCA_ONE_RULE_V42 &struOneRule);
    void SetEventTypeToWnd(DWORD dwEventType);
    DWORD GetEventTypeFromWnd();
    void SetEventUnionToWnd(NET_VCA_EVENT_UNION const & uEventParam, DWORD dwEventType);
    void GetEventUnionFromWnd(NET_VCA_EVENT_UNION &uEventParam, DWORD dwEventType);
    void SetSizeFilterToWnd(NET_VCA_SIZE_FILTER const &struSizeFilter);
    void GetSizeFilterFromWnd(NET_VCA_SIZE_FILTER &struSizeFilter);
    void SetHandleExceptionToWnd(NET_DVR_HANDLEEXCEPTION_V40 const &struHandleException);
    void GetHandleExceptionFromWnd(NET_DVR_HANDLEEXCEPTION_V40 &struHandleException);
    void SetRelRecordChanToWnd(DWORD const *dwRelRecordChan, int nChanNum);
    void GetRelRecordChanFromWnd(DWORD *dwRelRecordChan, int nChanNum);
    void SetAlarmTimeToWnd(NET_DVR_SCHEDTIME const *pAlarmTime, int nDay);
    void GetAlarmTimeFromWnd(NET_DVR_SCHEDTIME *pAlarmTime, int nDay);
	void SetFilterStrategyToWnd(NET_VCA_FILTER_STRATEGY const&struFilterStrategy);
	void GetFilterStrategyFromWnd(NET_VCA_FILTER_STRATEGY* pFilterStrategy);
	void SetRuleTriggerParamToWnd(NET_VCA_RULE_TRIGGER_PARAM const &struRuleTriggerParam);
	void GetRuleTriggerParamFromWnd(NET_VCA_RULE_TRIGGER_PARAM* pRuleTriggerParam);

    LONG StartPlay();
    BOOL StopPlay();

    BOOL EnableRuleWindow(BOOL bEnable);
    BOOL EnableRuleUnion(BOOL bEnable); //规则联合体中的参数的控件使能
    BOOL EnablAlarmTime(BOOL bEnable);
    BOOL EnableHandleException(BOOL bEnable);
    BOOL EnableSizeFilter(BOOL bEnable);
    BOOL ShowRuleTypeWnd(VCA_RULE_EVENT_TYPE_EX struEventType); 

    void F_DrawFrame(HDC hDc);
    void F_DrawFilter(HDC hDc);
    void F_DrawLine(HDC hDc);
    BOOL F_TimeTest(); 

    void ShowPixMode(int nCmdShow);
    void ShowRealMode(int nCmdShow);
    void ShowDefaultMode(int nCmdShow);

    BOOL GetPuStreamCfg(NET_DVR_PU_STREAM_CFG &struPUStream);
public:
    CStatic m_staticRuleParam1;
    CStatic m_staticRuleParam2;
    CStatic m_staticRuleParam3;
    CStatic m_staticRuleParam4;
    CStatic m_staticRuleParam5;
    CComboBox m_cmbVCAChannelID;
    CComboBox m_cmbVCAChannelGroupNo;
    int m_iRuleParam3;
    int m_iRuleParam4;
    int m_iRuleParam5;
    CEdit m_editRuleParam6;
    int m_iRuleParam6;
    CStatic m_staticRuleParam6;
    
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedBtnGetRuleCfg();
    
    CComboBox m_comboRuleParam1;
    CComboBox m_comboRuleParam2;
    CEdit m_editRuleParam3;
    CComboBox m_comboResolution;
    CComboBox m_combEventPriority;
    CListCtrl m_listAlarmOut;
    afx_msg void OnCbnSelchangeComboChannelList();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGVCARULECFGEX_H__B678DBAB_24C9_4063_A990_784B443D35E7__INCLUDED_)
