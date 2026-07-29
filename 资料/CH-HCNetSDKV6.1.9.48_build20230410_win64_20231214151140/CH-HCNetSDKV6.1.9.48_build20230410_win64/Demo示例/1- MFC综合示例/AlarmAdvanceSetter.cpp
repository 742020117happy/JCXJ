// AlarmAdvanceSetter.cpp : 实现文件
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "AlarmAdvanceSetter.h"
#include "afxdialogex.h"
#include "GeneralDef.h"

const char* szEventTypeZn[] = { "信号量报警", "移动侦测", "视频丢失", "遮挡报警", "人脸侦测",
"虚焦侦测", "音频异常侦测", "场景变更", "区域入侵", "越界侦测（穿越警戒线）",
"进入区域", "离开区域", "徘徊", "聚集", "快速移动", "停车侦测", "物品遗留",
"物品拿取", "PIR", "客流量检测", "测温报警", "测温预警", "温差报警", "火点检测",
"烟雾检测", "烟火检测", "船只检测", "硬盘满", "硬盘错误", "网络断开",
"IP地址冲突", "非法访问", "抓拍", "人脸比对", "允许名单人脸比对（陌生人比对）", "目标抓拍",
"车牌识别", "人员密度", "交通事件检测", "抓拍建模", "名单车辆报警上传", "热度图报警", "车辆图片二次识别",
"人证比对", "岗位状态检测", "人数变化", "人员奔跑", "剧烈运动", "人员倒地", "人员滞留事件", "高频人员报警",
"热备异常", "POE供电异常", "视频异常", "录像异常", "硬盘未初始化", "人脸库硬盘异常", "人脸库变更异常",
"人脸库图片变更", "集群系统存储满异常", "集群版本异常", "掉线异常", "录像周期不足",
"IPC迁移失败", "安全帽检测", "标签信息上传", "基站信息上传", "非视频设备检测事件", "门禁主机检测事件",
"可视对讲设备事件上传", "可视对讲设备报警上传", "排队时长检测", "排队人数检测", "人员排队检测实时数据上报",
"混合目标检测", "智慧城管", "视频异常", "VQD诊断" };

const char* szEventType[] = { "IO", "VMD", "videoloss", "shelteralarm", "facedetection",
"defocus", "audioexception", "scenechangedetection", "fielddetection", "linedetection",
"regionEntrance", "regionExiting", "loitering", "group", "rapidMove", "parking", "unattendedBaggage",
"attendedBaggage", "PIR", "PeopleCounting", "TMA", "TMPA", "temperature", "fireDetection",
"smokeDetection", "smokeAndFireDetection", "shipsDetection", "diskfull", "diskerror", "nicbroken",
"ipconflict", "illaccess", "faceCapture", "alarmResult", "whiteFaceContrast", "targetCapture",
"ANPR", "personDensityDetection", "AID", "faceSnapModeling", "vehicleMatchResult", "heatmap", "vehicleRcogResult",
"cardMatch", "leavePosition", "peopleNumChange", "running", "violentMotion", "failDown", "retention", "HFPD",
"hotSpare", "poe", "videoException", "recordException", "diskUnformat", "facelibDiskError", "facelibModifyError",
"facelibPictureError", "sysStorFull", "versionAbnormal", "nodeOffline", "recordCycleAbnormal",
"ipcTransferAbnormal", "safetyHelmetDetection", "tagInfo", "baseStationInfo", "IOTD", "AccessControllerEvent",
"VideoIntercomEvent", "VideoIntercomAlarm", "personQueueTime", "personQueueCounting", "personQueueRealTime",
"mixedTargetDetection", "cityManagement", "pocException", "VQDDiagnose" };

// CAlarmAdvanceSetter 对话框

IMPLEMENT_DYNAMIC(CAlarmAdvanceSetter, CDialogEx)

DWORD CAlarmAdvanceSetter::GetAbilityThread(LPVOID pParam)
{
    CAlarmAdvanceSetter *pThis = (CAlarmAdvanceSetter*)pParam;
    pThis->setDeviceAbility();
    return 0;
}

CAlarmAdvanceSetter::CAlarmAdvanceSetter(CWnd* pParent /*=NULL*/)
    : CDialogEx(CAlarmAdvanceSetter::IDD, pParent)
    , m_bChePaiBroken(FALSE)
    , m_bKeLiuTongJiBroken(FALSE)
    , m_bHotMapBroken(FALSE)
    , m_bFaceSanpBroken(FALSE)
    , m_bFaceMatchBroken(FALSE)
    , m_bCopilotFace(FALSE)
    , m_iTaskNo(0)
    , m_bConfirmMechanism(FALSE)
    , m_pData(NULL)
    , m_bIsSupoortSubscribe(FALSE)
    , m_iDeviceIndex(-1)
    , m_bJsonPass(FALSE)
    , m_bIsSupportBehaviorUploadByCloudStorageURL(FALSE)
    , m_bChannelEnable(FALSE)
    , m_strSubscribe(_T(""))
    , m_bUploadAll(FALSE)
    , m_bUploadChannel(FALSE)
    , m_bUploadHd(FALSE)
    , m_bUploadCapability(FALSE)
    , m_bCheckSubcribeCap(FALSE)
    , m_bEnableXml(FALSE)
    , m_bChkHeatMapDurationPDC(FALSE)
    , m_bChkHeatMapIntersection(FALSE)
    , m_bRetAlarmTypeV40(FALSE)
    , m_bNVRAlarmIPC(FALSE)
    , m_bVMDPic(FALSE)
    , m_bAidBroken(FALSE)
    , m_bTfsBroken(FALSE)
    , m_bTPsStatBroken(FALSE)
    , m_bRuleBroken(FALSE)
    , m_hAbilityThread(NULL)
{
    //m_bIsSupportAlarmLevel = FALSE;
    //m_bIsSupportITSUploadType = FALSE;
    //m_bIsSupportAlarmTypeV40 = FALSE;
    //m_bIsSupportDevInfoVersion = FALSE;
    m_bIsSupportFaceSnap = FALSE;
    m_bIsSupportFaceDetection = FALSE;
    //m_bIsSupportChePaiBroken = FALSE;
    m_iRowCount = 0;
    m_iSubItem = -1;
    m_iLastItem = -1;

    memset(m_listSubcirbeEventCap, 0, MAX_EVENT_TYPE_NUM * sizeof(SubcirbeEventCap));

    m_strAllPictureUrlType = _T("");

    //m_bIsSupportFaceSanpBroken = FALSE;
    //m_bIsSupportFaceMatchBroken = TRUE;
}

CAlarmAdvanceSetter::~CAlarmAdvanceSetter()
{
    CloseAbilityThread();
}

void CAlarmAdvanceSetter::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_ALARM_RANK, m_cmbAlarmLevel);
    DDX_Control(pDX, IDC_COMBO_ALARM_TYPE, m_cmbAlarmType);
    DDX_Control(pDX, IDC_COMBO_FACE_DETECT_TYPE, m_cmbFaceDetectType);
    DDX_Check(pDX, IDC_CHECK_FACE_MATCH_BROKEN, m_bFaceMatchBroken);
    DDX_Control(pDX, IDC_COMBO_ITS_UPLOAD_TYPE, m_cmbITSUploadType);
    DDX_Check(pDX, IDC_CHECK_CHEPAI_BROKEN, m_bChePaiBroken);
    DDX_Check(pDX, IDC_CHECK_KELIUTONGJI_BROKEN, m_bKeLiuTongJiBroken);
    DDX_Check(pDX, IDC_CHECK_HotMap_BROKEN, m_bHotMapBroken);
    DDX_Check(pDX, IDC_CHECK_FACE_SNAP_BROKEN, m_bFaceSanpBroken);
    DDX_Control(pDX, IDC_COMBO_FACE_SNAP_DATA_TYPE, m_cmbFaceSnapDataType);
    DDX_Control(pDX, IDC_COMBO_FACE_MATCH_DATA_TYPE, m_cmbFaceMatchDataType);
    DDX_Control(pDX, IDC_COMBO_EVENT_JSON_DATA_TYPE, m_cmbEventJsonDataType);
    DDX_Check(pDX, IDC_CHECK_COPILOT_FACE, m_bCopilotFace);
    DDX_Text(pDX, IDC_EDIT_TASK_NUMBER, m_iTaskNo);
    DDX_Control(pDX, IDC_COMBO_ALARM_SUBSCRIBE_INFO, m_cmbAlarmSubscribeInfo);
    DDX_Check(pDX, IDC_CHECK_CONFIRM_MECHANISM, m_bConfirmMechanism);
    DDX_Check(pDX, IDC_CHECK_JSON_PASS, m_bJsonPass);
    DDX_Control(pDX, IDC_COMBO_BEHAVIOR, m_cmbBehavior);
    DDX_Control(pDX, IDC_LIST_CHAN, m_listChan);
    DDX_Check(pDX, IDC_CHECK_CHANNEL_ENABLE, m_bChannelEnable);
    DDX_Control(pDX, IDC_COMBO_PICTURE_URL_TYPE, m_comboPictureUrlType);
    DDX_Control(pDX, IDC_COMBO_EVENT_TYPE, m_comboEventType);
    DDX_Control(pDX, IDC_LIST_EVENT_PARAM, m_listEventParam);
    DDX_Check(pDX, IDC_CHECK_UPLOAD_ALL, m_bUploadAll);
    DDX_Check(pDX, IDC_CHECK_UPLOAD_CHANNEL, m_bUploadChannel);
    DDX_Check(pDX, IDC_CHECK_UPLOAD_HD, m_bUploadHd);
    DDX_Check(pDX, IDC_CHECK_UPLOAD_CAPABILITY, m_bUploadCapability);
    DDX_Check(pDX, IDC_CHECK_SUBCRIBE_CAP, m_bCheckSubcribeCap);
    DDX_Check(pDX, IDC_CHK_ENABLE_XML, m_bEnableXml);
    DDX_Check(pDX, IDC_CHECK_HEATMAP_PDC_DURATION_BROKEN, m_bChkHeatMapDurationPDC);
    DDX_Check(pDX, IDC_CHECK_INTERSECTION_ANALYSIS_BROKEN, m_bChkHeatMapIntersection);
    DDX_Check(pDX, IDC_CHECK_ALARMTYPE_V40, m_bRetAlarmTypeV40);
    DDX_Check(pDX, IDC_CHECK_ALL_ALARM, m_bNVRAlarmIPC);
    DDX_Check(pDX, IDC_CHECK_VMD, m_bVMDPic);
    DDX_Check(pDX, IDC_CHECK_AID_BROKEN, m_bAidBroken);
    DDX_Check(pDX, IDC_CHECK_TFS_BROKEN, m_bTfsBroken);
    DDX_Check(pDX, IDC_CHECK_TPS_STATISTIC_BROKEN, m_bTPsStatBroken);
    DDX_Check(pDX, IDC_CHECK_RULE_BROKEN, m_bRuleBroken);
}


BEGIN_MESSAGE_MAP(CAlarmAdvanceSetter, CDialogEx)
    ON_CBN_SELCHANGE(IDC_COMBO_ALARM_SUBSCRIBE_INFO, &CAlarmAdvanceSetter::OnCbnSelchangeComboAlarmSubscribeInfo)
    ON_BN_CLICKED(IDC_BUTTON_CONFIRM, &CAlarmAdvanceSetter::OnBnClickedButtonConfirm)
    ON_BN_CLICKED(IDC_BTN_XMLSUB, &CAlarmAdvanceSetter::OnBnClickedBtnXmlsub)
    ON_BN_CLICKED(IDC_BUTTON_GET_SUBSCRIBE, &CAlarmAdvanceSetter::OnBnClickedButtonGetSubscribe)
    ON_BN_CLICKED(IDC_BUTTON_SET_SUBSCRIBE, &CAlarmAdvanceSetter::OnBnClickedButtonSetSubscribe)
    ON_BN_CLICKED(IDC_BUTTON_ADD_PARAM, &CAlarmAdvanceSetter::OnBnClickedButtonAddParam)
    ON_BN_CLICKED(IDC_BUTTON_DELETE, &CAlarmAdvanceSetter::OnBnClickedButtonDeleteParam)
    ON_NOTIFY(NM_CLICK, IDC_LIST_EVENT_PARAM, &CAlarmAdvanceSetter::OnNMClickListEventParam)
    ON_BN_CLICKED(IDC_CHECK_CHANNEL_ENABLE, &CAlarmAdvanceSetter::OnBnClickedCheckChannelEnable)
    ON_CBN_SELCHANGE(IDC_COMBO_EVENT_TYPE, &CAlarmAdvanceSetter::OnCbnSelchangeComboEventType)
    ON_BN_CLICKED(IDC_CHECK_SUBCRIBE_CAP, &CAlarmAdvanceSetter::OnBnClickedCheckSubcribeCap)
    ON_WM_CLOSE()
END_MESSAGE_MAP()

BOOL CAlarmAdvanceSetter::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化

    //设置下拉框的默认选项
    m_cmbAlarmLevel.SetCurSel(0);
    m_cmbAlarmType.SetCurSel(0);  //默认为客户端布防
    m_cmbFaceDetectType.SetCurSel(1);  //默认为人脸侦测报警
    m_cmbITSUploadType.SetCurSel(1);//默认为新报警
    m_cmbAlarmSubscribeInfo.SetCurSel(0);  //默认为All
    m_cmbFaceMatchDataType.SetCurSel(0);   //默认为二进制
    m_cmbFaceSnapDataType.SetCurSel(0);    //默认为二进制
    m_cmbEventJsonDataType.SetCurSel(0);   //默认为二进制
    m_cmbBehavior.SetCurSel(0);   //默认为二进制

    //为复选框设置默认值
    m_bFaceSanpBroken = FALSE;
    m_bFaceMatchBroken = FALSE;
    m_bHotMapBroken = FALSE;
    m_bKeLiuTongJiBroken = FALSE;
    m_bChePaiBroken = FALSE;
    m_bConfirmMechanism = FALSE;
    m_bCopilotFace = FALSE;
    m_iTaskNo = FALSE;
    m_bJsonPass = FALSE;
    m_bChkHeatMapDurationPDC = FALSE;
    m_bChkHeatMapIntersection = FALSE;

    m_bAidBroken = FALSE;
    m_bTfsBroken = FALSE;
    m_bTPsStatBroken = FALSE;
    m_bRuleBroken = FALSE;

    //默认可以设置所有报警信息
    m_cmbAlarmLevel.EnableWindow(TRUE);
    m_cmbAlarmType.EnableWindow(TRUE);
    m_cmbFaceDetectType.EnableWindow(TRUE);
    m_cmbITSUploadType.EnableWindow(TRUE);
    m_cmbFaceMatchDataType.EnableWindow(TRUE);
    m_cmbFaceSnapDataType.EnableWindow(TRUE);
    m_cmbEventJsonDataType.EnableWindow(TRUE);

    m_bChannelEnable = FALSE;

    GetDlgItem(IDC_COMBO_FACE_SNAP_DATA_TYPE)->EnableWindow(TRUE);
    GetDlgItem(IDC_COMBO_FACE_MATCH_DATA_TYPE)->EnableWindow(TRUE);
    GetDlgItem(IDC_COMBO_EVENT_JSON_DATA_TYPE)->EnableWindow(TRUE);
    GetDlgItem(IDC_COMBO_BEHAVIOR)->EnableWindow(TRUE);
    
    GetDlgItem(IDC_CHECK_CONFIRM_MECHANISM)->EnableWindow(TRUE);
    GetDlgItem(IDC_CHECK_CHEPAI_BROKEN)->EnableWindow(TRUE);
    GetDlgItem(IDC_CHECK_KELIUTONGJI_BROKEN)->EnableWindow(TRUE);
    GetDlgItem(IDC_CHECK_HotMap_BROKEN)->EnableWindow(TRUE);
    GetDlgItem(IDC_CHECK_FACE_SNAP_BROKEN)->EnableWindow(TRUE);
    GetDlgItem(IDC_CHECK_FACE_MATCH_BROKEN)->EnableWindow(TRUE);
    GetDlgItem(IDC_CHECK_COPILOT_FACE)->EnableWindow(TRUE);
    GetDlgItem(IDC_EDIT_TASK_NUMBER)->EnableWindow(TRUE);

    GetDlgItem(IDC_CHECK_AID_BROKEN)->EnableWindow(TRUE);
    GetDlgItem(IDC_CHECK_TFS_BROKEN)->EnableWindow(TRUE);
    GetDlgItem(IDC_CHECK_TPS_STATISTIC_BROKEN)->EnableWindow(TRUE);
    GetDlgItem(IDC_CHECK_RULE_BROKEN)->EnableWindow(TRUE);
    if (!m_bIsSupoortSubscribe)
    {
        m_cmbAlarmSubscribeInfo.EnableWindow(FALSE);
        GetDlgItem(IDC_CHECK_CHANNEL_ENABLE)->EnableWindow(FALSE);
        GetDlgItem(IDC_COMBO_EVENT_TYPE)->EnableWindow(FALSE);
        GetDlgItem(IDC_COMBO_PICTURE_URL_TYPE)->EnableWindow(FALSE);
        GetDlgItem(IDC_CHK_ENABLE_XML)->EnableWindow(FALSE);
    }

    m_bIsSupportFaceSnap = FALSE;
    m_bIsSupportFaceDetection = FALSE;

    OnBnClickedCheckChannelEnable();
    OnCbnSelchangeComboAlarmSubscribeInfo();

    InitEventType();

    //隐藏LIST上报信息
    //GetDlgItem(IDC_STATIC_LIST_SUBSCRIBE_LIST)->EnableWindow(FALSE);

    //在Fortify.cpp文件中, 如果选择多个设备，则设置为m_iDeviceIndex=-1
    if (m_iDeviceIndex != -1)
    {
        //使用线程的方式，后端设备64个通道，每个通道都获取能力集，设备返回很慢，一个能力获取需要10s以上时间，多个能力获取卡死
        CreateAbilityThread();
        GetSubcribeCapability();
        m_bCheckSubcribeCap = TRUE;
        GetDlgItem(IDC_CHECK_SUBCRIBE_CAP)->EnableWindow(TRUE);
    }
    else
    {
        GetDlgItem(IDC_CHECK_SUBCRIBE_CAP)->EnableWindow(FALSE);
    }

    m_listChan.SetExtendedStyle(m_listChan.GetExtendedStyle() | LVS_EX_CHECKBOXES);
    UpdateChanStatus();

    m_listEventParam.SetExtendedStyle(m_listEventParam.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);
    m_listEventParam.DeleteAllItems();
    char szLan[128] = { 0 };
    g_StringLanType(szLan, "序号", "Index");
    m_listEventParam.InsertColumn(0, szLan, LVCFMT_LEFT, 60, -1);

    g_StringLanType(szLan, "事件类型", "EventType");
    m_listEventParam.InsertColumn(1, szLan, LVCFMT_LEFT, 120, -1);

    g_StringLanType(szLan, "通道号", "Channels");
    m_listEventParam.InsertColumn(2, szLan, LVCFMT_LEFT, 120, -1);

    g_StringLanType(szLan, "图片类型", "PictureType");
    m_listEventParam.InsertColumn(3, szLan, LVCFMT_LEFT, 120, -1);

    if (g_struDeviceInfo[m_iDeviceIndex].lFortifyHandle >= 0)
    {
        GetDlgItem(IDC_BUTTON_GET_SUBSCRIBE)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_SET_SUBSCRIBE)->EnableWindow(TRUE);
    }
    else
    {
        GetDlgItem(IDC_BUTTON_GET_SUBSCRIBE)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_SET_SUBSCRIBE)->EnableWindow(FALSE);
    }

    UpdateData(FALSE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常:  OCX 属性页应返回 FALSE
}


/////////////////////////////////////////////////////////////////////////////
/*********************************************************
Function:  CheckITDeviceAbility
Desc:      Get the ITDeviceAbility ability based on the DevieceIndex
           maximize the alarm ability(one channel supports means all channel support)
Input:     none
Output:    none
Return:    none
**********************************************************/
void CAlarmAdvanceSetter::CheckITDeviceAbility()
{
    //查询 ITDeviceAbility
    /*布防等级配置： 1.接口调用成功；2.判断主节点<ITDeviceAbility>中的<ITCAbility>，只要存在就表示支持布防等级配置；
      是否上传副驾驶人脸子图 :  1.接口调用成功；2.判断主节点<ITDeviceAbility>中的<ITCAbility>，表示这个设备是交通设备，只有交通设备才支持该功能，；
      智能交通报警信息上传类型: 1.接口调用成功； 2.判断主节点<ITDeviceAbility>中的<TrafficDataUpload>*/
    DWORD dwAbilityType = DEVICE_ABILITY_INFO;
    char pInBuf[512] = "";
    DWORD dwOutputLen = 1024 * 1024;
    char *pOutBuf = new char[dwOutputLen];

    BOOL bIsSupportAlarmLevel = FALSE;
    BOOL bIsSupportITSUploadType = FALSE;
    BOOL bIsSupportCopilotFace = FALSE;

    for (int iChannelindex = 0; iChannelindex < g_struDeviceInfo[m_iDeviceIndex].iDeviceChanNum; iChannelindex++)
    {
        if (g_struDeviceInfo[m_iDeviceIndex].pStruChanInfo[iChannelindex].bEnable == FALSE)
        {
            continue;
        }
        sprintf(pInBuf, "<ITDeviceAbility version = \"2.0\">\r\n \
                         <channelNO>%d</channelNO>\r\n \
                         </ITDeviceAbility>", iChannelindex + 1);
        memset(pOutBuf, 0, dwOutputLen);
        if (NET_DVR_GetDeviceAbility(g_struDeviceInfo[m_iDeviceIndex].lLoginID, dwAbilityType, pInBuf, strlen(pInBuf), pOutBuf, dwOutputLen))
        {
            //解析xmlConfigOutput参数
            CXmlBase xmlBase;
            xmlBase.Parse(pOutBuf);
            if (xmlBase.FindElem("ITDeviceAbility") && xmlBase.IntoElem() && xmlBase.FindElem("ITCAbility"))
            {
                bIsSupportAlarmLevel = TRUE;
                bIsSupportCopilotFace = TRUE;
                if ((xmlBase.IntoElem() && xmlBase.FindElem("TrafficDataUpload")))
                {
                    bIsSupportITSUploadType = TRUE;
                    break;
                }
            }
        }
    }
    m_cmbAlarmLevel.EnableWindow(bIsSupportAlarmLevel);
    m_cmbITSUploadType.EnableWindow(bIsSupportITSUploadType);
    GetDlgItem(IDC_CHECK_COPILOT_FACE)->EnableWindow(bIsSupportCopilotFace);

    delete []pOutBuf;
    pOutBuf = NULL;
}

/////////////////////////////////////////////////////////////////////////////
/*********************************************************
Function:  CheckEventAbility
Desc:      Get the EventAbility ability based on the DevieceIndex
maximize the alarm ability(one channel supports means all channel support)
Input:     none
Output:    none
Return:    none
**********************************************************/
void CAlarmAdvanceSetter::CheckEventAbility()
{
    //查询EventAbility
    /*人脸报警信息类型-人脸侦测:1.接口调用成功；2.判断<EventAbility>中的<FaceDetection>，如果节点存在，表示设备支持“人脸侦测”*/

    DWORD dwAbilityType = DEVICE_ABILITY_INFO;
    char pInBuf[512] = "";
    DWORD dwOutputLen = 1024 * 1024;
    char *pOutBuf = new char[dwOutputLen];

    for (int iChannelindex = 0; iChannelindex < g_struDeviceInfo[m_iDeviceIndex].iDeviceChanNum; iChannelindex++)
    {
        if (g_struDeviceInfo[m_iDeviceIndex].pStruChanInfo[iChannelindex].bEnable == FALSE)
        {
            continue;
        }
        sprintf(pInBuf, "<EventAbility version=\"2.0\">\r\n<channelNO>%d</channelNO>\r\n</EventAbility>", iChannelindex + 1);
        memset(pOutBuf, 0, dwOutputLen);
        if (NET_DVR_GetDeviceAbility(g_struDeviceInfo[m_iDeviceIndex].lLoginID, dwAbilityType, pInBuf, strlen(pInBuf), pOutBuf, dwOutputLen))
        {
            CXmlBase xmlBase;
            xmlBase.Parse(pOutBuf);

            if (xmlBase.FindElem("EventAbility") && xmlBase.IntoElem() && xmlBase.FindElem("FaceDetection"))
            {
                m_bIsSupportFaceDetection = TRUE;
                break;
            }
        }
    }

    delete[]pOutBuf;
    pOutBuf = NULL;
    return;
}


/////////////////////////////////////////////////////////////////////////////
/*********************************************************
Function:  CheckVcaChanAbility
Desc:      Get the VcaChanAbility ability based on the DevieceIndex
maximize the alarm ability(one channel supports means all channel support)
Input:     none
Output:    none
Return:    none
**********************************************************/
void CAlarmAdvanceSetter::CheckVcaChanAbility()
{
    //查询VcaChanAbility 
    /*人脸报警信息类型-抓拍: 1.接口调用成功；2.判断<VcaChanAbility>中的<Face>中的<FaceSnap>，如果节点存在，表示设备支持“抓拍”
      抓拍续传：1.接口调用成功；2.判断<VcaChanAbility>中的<Face>中的<FaceSnap>中的<brokenNetHttp>，如果节点存在，表示设备支持“抓拍断网续传” */
    DWORD dwAbilityType = DEVICE_ABILITY_INFO;
    char pInBuf[512] = "";
    DWORD dwOutputLen = 1024 * 1024;
    char *pOutBuf = new char[dwOutputLen];

    BOOL bIsSupportFaceSnapBroken = FALSE;

    for (int iChannelindex = 0; iChannelindex < g_struDeviceInfo[m_iDeviceIndex].iDeviceChanNum; iChannelindex++)
    {
        if (g_struDeviceInfo[m_iDeviceIndex].pStruChanInfo[iChannelindex].bEnable == FALSE)
        {
            continue;
        }
        sprintf(pInBuf, "<VcaChanAbility version=\"2.0\">\r\n<channelNO>%d</channelNO>\r\n</VcaChanAbility>", iChannelindex + 1);
        memset(pOutBuf, 0, dwOutputLen);
        if (NET_DVR_GetDeviceAbility(g_struDeviceInfo[m_iDeviceIndex].lLoginID, dwAbilityType, pInBuf, strlen(pInBuf), pOutBuf, dwOutputLen))
        {
            CXmlBase xmlBase;
            xmlBase.Parse(pOutBuf);

            if (xmlBase.FindElem("VcaChanAbility") && xmlBase.IntoElem() && xmlBase.FindElem("Face") && xmlBase.IntoElem() && xmlBase.FindElem("FaceSnap"))
            {
                m_bIsSupportFaceSnap = TRUE;
                //2.抓拍断网续传
                if ((xmlBase.IntoElem() && xmlBase.FindElem("brokenNetHttp")))
                {
                    bIsSupportFaceSnapBroken = TRUE;
                    break;
                }
            }
        }
    }

    GetDlgItem(IDC_CHECK_FACE_SNAP_BROKEN)->EnableWindow(bIsSupportFaceSnapBroken);
    delete []pOutBuf;
    pOutBuf = NULL;
    
}

/////////////////////////////////////////////////////////////////////////////
/*********************************************************
Function:  CheckBasicCapability
Desc:      Get the BasicCapability ability based on the DevieceIndex
maximize the alarm ability(one channel supports means all channel support)
Input:     none
Output:    none
Return:    none
**********************************************************/
void CAlarmAdvanceSetter::CheckBasicCapability()
{
    //查询断网续传类型: 1.车牌，客流量，热度图断网续传,JSON报警透传（IPC）
    /*车牌，客流量，热度图断网续传: 1.接口调用成功；2.判断<BasicCapability>中的<SoftwareCapability>中的<BrokenNetHttpSupport>，如果节点存在，表示支持断网续传机制；
        同时断网续传是针对功能 
        postMPR - 车牌检测
        PDC - 客流统计
        heatMapDetection - 热度图
    JSON报警透传（IPC）:1.接口调用成功；2.判断<BasicCapability>中的<SoftwareCapability>中的<isSupportAlarmJSONBrokenNetHttp>是否存在，如果存在就表示支持；
    */
    

    DWORD dwAbilityType = DEVICE_SOFTHARDWARE_ABILITY;
    char pInBuf[512] = "";
    DWORD dwOutputLen = 1024 * 1024;
    char *pOutBuf = new char[dwOutputLen];
    memset(pOutBuf, 0, dwOutputLen);

    BOOL bIsSupportChePaiBroken = FALSE;
    BOOL bIsSupportKeLiuTongJiBroken = FALSE;
    BOOL bIsSupportHotMapBroken = FALSE;
    BOOL bIsSupportJsonBroken = FALSE;
    BOOL bIsSupportConfirmMechanism = FALSE;

    BOOL bIsSupportAIDBroken = FALSE;
    BOOL bIsSupportTFSBroken = FALSE;
    BOOL bIsSupportTPSStatBroken = FALSE;
    BOOL bIsSupportRuleBroken = FALSE;
    if (NET_DVR_GetDeviceAbility(g_struDeviceInfo[m_iDeviceIndex].lLoginID, dwAbilityType, NULL, 0, pOutBuf, dwOutputLen))
    {
        CXmlBase xmlBase;
        xmlBase.Parse(pOutBuf);

        if (xmlBase.FindElem("BasicCapability") && xmlBase.IntoElem() && xmlBase.FindElem("SoftwareCapability") && xmlBase.IntoElem())
        {
            if (xmlBase.FindElem("BrokenNetHttpSupport"))
            {
                string sOpt = xmlBase.GetAttributeValue("opt");
                if (sOpt.find("postMPR") != string::npos)
                {
                    bIsSupportChePaiBroken = TRUE;
                }
                if (sOpt.find("PDC") != string::npos)
                {
                    bIsSupportKeLiuTongJiBroken = TRUE;
                }
                if (sOpt.find("heatMapDetection") != string::npos)
                {
                    bIsSupportHotMapBroken = TRUE;
                }
            }

            if (xmlBase.FindElem("isSupportConfirmMechanism"))
            {
                //确认机制
                bIsSupportConfirmMechanism = TRUE;
            }

            if (xmlBase.FindElem("isSupportAlarmJSONBrokenNetHttp"))
            {
                //JSON报警透传（IPC）
                bIsSupportJsonBroken = TRUE;
            }

            if (xmlBase.FindElem("isSupportAIDAlarmBrokenNetHttp"))
            {
                //AID
                bIsSupportAIDBroken = TRUE;
            }

            if (xmlBase.FindElem("isSupportAlarmJSONBrokenNetHttp"))
            {
                //TFS
                bIsSupportTFSBroken = TRUE;
            }

            if (xmlBase.FindElem("isSupportTPSStatisticsAlarmBrokenNetHttp"))
            {
                //TPS统计过车事件
                bIsSupportTPSStatBroken = TRUE;
            }

            if (xmlBase.FindElem("isSupportRuleAlarmBrokenNetHttp"))
            {
                //异常行为检测
                bIsSupportRuleBroken = TRUE;
            }

            if (xmlBase.FindElem("isSupportBehaviorUploadByCloudStorageURL"))
            {
                //是否支持支持异常行为检测事件通过武汉云url上报
                m_bIsSupportBehaviorUploadByCloudStorageURL = TRUE;
            }
        }
    }
    GetDlgItem(IDC_CHECK_CHEPAI_BROKEN)->EnableWindow(bIsSupportChePaiBroken);
    GetDlgItem(IDC_CHECK_KELIUTONGJI_BROKEN)->EnableWindow(bIsSupportKeLiuTongJiBroken);
    GetDlgItem(IDC_CHECK_HotMap_BROKEN)->EnableWindow(bIsSupportHotMapBroken);
    GetDlgItem(IDC_CHECK_JSON_PASS)->EnableWindow(bIsSupportJsonBroken);
    GetDlgItem(IDC_CHECK_CONFIRM_MECHANISM)->EnableWindow(bIsSupportConfirmMechanism);

    GetDlgItem(IDC_CHECK_AID_BROKEN)->EnableWindow(bIsSupportAIDBroken);
    GetDlgItem(IDC_CHECK_TFS_BROKEN)->EnableWindow(bIsSupportTFSBroken);
    GetDlgItem(IDC_CHECK_TPS_STATISTIC_BROKEN)->EnableWindow(bIsSupportTPSStatBroken);
    GetDlgItem(IDC_CHECK_RULE_BROKEN)->EnableWindow(bIsSupportRuleBroken);

    delete []pOutBuf;
    pOutBuf = NULL;
}

/////////////////////////////////////////////////////////////////////////////
/*********************************************************
Function:  CheckFaceContrast
Desc:      Get the FaceContrast Broken Upload ability based on the DevieceIndex
maximize the alarm ability(one channel supports means all channel support)
Input:     none
Output:    none
Return:    none
**********************************************************/
void CAlarmAdvanceSetter::CheckFaceContrast()
{
    //查询人脸对比断网续传
    /*
    1.接口调用成功；
    2.判断<FaceContrastCap>中的<brokenNetHttp>，节点存在表示支持
    */
    NET_DVR_XML_CONFIG_INPUT  xmlConfigInput = { 0 };
    NET_DVR_XML_CONFIG_OUTPUT  xmlCongfigOutput = { 0 };
    xmlConfigInput.dwSize = sizeof(xmlConfigInput);
    xmlCongfigOutput.dwSize = sizeof(xmlCongfigOutput);

    char szUrl[256] = ""; 
    DWORD dwOutputLen = 1024 * 1024;
    char *pOutBuf = new char[dwOutputLen];

    BOOL bIsSupportFaceMatchBroken = FALSE;

    for (int iChannelindex = 0; iChannelindex < g_struDeviceInfo[m_iDeviceIndex].iDeviceChanNum; iChannelindex++)
    {
        if (g_struDeviceInfo[m_iDeviceIndex].pStruChanInfo[iChannelindex].bEnable == FALSE)
        {
            continue;
        }
        sprintf(szUrl, "GET /ISAPI/Intelligent/channels/%d/faceContrast/capabilities", iChannelindex + 1);
        xmlConfigInput.lpRequestUrl = szUrl;
        xmlConfigInput.dwRequestUrlLen = strlen(szUrl);

        memset(pOutBuf, 0, dwOutputLen);
        xmlCongfigOutput.dwOutBufferSize = dwOutputLen;
        xmlCongfigOutput.lpOutBuffer = pOutBuf;

        if (NET_DVR_STDXMLConfig(g_struDeviceInfo[m_iDeviceIndex].lLoginID, &xmlConfigInput, &xmlCongfigOutput))
        {
            CXmlBase xmlBase;
            xmlBase.Parse(pOutBuf);

            if ((xmlBase.FindElem("FaceContrastCap") && xmlBase.IntoElem() && xmlBase.FindElem("brokenNetHttp")))
            {
                bIsSupportFaceMatchBroken = TRUE;
                break;
            }
        }
    }
    GetDlgItem(IDC_CHECK_FACE_MATCH_BROKEN)->EnableWindow(bIsSupportFaceMatchBroken);
    delete []pOutBuf;
    pOutBuf = NULL;
}

/////////////////////////////////////////////////////////////////////////////
/*********************************************************
Function:  CheckVehicleRecogAbility
Desc:      Get the VehicleRecogAbility ability based on the DevieceIndex
maximize the alarm ability(one channel supports means all channel support)
Input:     none
Output:    none
Return:    none
**********************************************************/
void CAlarmAdvanceSetter::CheckVehicleRecogAbility()
{
    //查询VehicleRecogAbility  
    /*任务处理号:1.接口调用成功；2.判断<VehicleRecogAbility>中的<taskNo>是否存在，如果不存在就表示不支持
    */
    DWORD dwAbilityType = DEVICE_ABILITY_INFO;
    char pInBuf[512] = "";
    DWORD dwOutputLen = 1024 * 1024;
    char *pOutBuf = new char[dwOutputLen];

    BOOL bIsSupportTaskNum = FALSE;

    for (int iChannelindex = 0; iChannelindex < g_struDeviceInfo[m_iDeviceIndex].iDeviceChanNum; iChannelindex++)
    {
        if (g_struDeviceInfo[m_iDeviceIndex].pStruChanInfo[iChannelindex].bEnable == FALSE)
        {
            continue;
        }
        sprintf(pInBuf, "<VehicleRecogAbility version=\"2.0\">\r\n<channelNO>%d</channelNO>\r\n</VehicleRecogAbility>", iChannelindex + 1);
        memset(pOutBuf, 0, dwOutputLen);
        if (NET_DVR_GetDeviceAbility(g_struDeviceInfo[m_iDeviceIndex].lLoginID, dwAbilityType, pInBuf, strlen(pInBuf), pOutBuf, dwOutputLen))
        {
            CXmlBase xmlBase;
            xmlBase.Parse(pOutBuf);

            if ((xmlBase.FindElem("VehicleRecogAbility") && xmlBase.IntoElem() && xmlBase.FindElem("taskNo")))
            {
                bIsSupportTaskNum = TRUE;
                break;
            }
        }
    }
    GetDlgItem(IDC_EDIT_TASK_NUMBER)->EnableWindow(bIsSupportTaskNum);
    delete[]pOutBuf;
    pOutBuf = NULL;

}

/////////////////////////////////////////////////////////////////////////////
/*********************************************************
Function:  CheckVehicleRecogAbility
Desc:      Get the VehicleRecogAbility ability based on the DevieceIndex
maximize the alarm ability(one channel supports means all channel support)
Input:     none
Output:    none
Return:    none
**********************************************************/
void CAlarmAdvanceSetter::CheckCloudStorageCfg()
{
    //报警图片数据类型
    /*1、接口调用成功；2、判断byEnable 是否等于 1，如果等于1，那就说明设备支持云存储，并且云存储已经开启，默认所有的URI走云存储方式；
    这个时候可以显示两个选择项（0-二进制传输；1-URL传输）
    */
    DWORD dwCommand = NET_DVR_GET_CLOUDSTORAGE_CFG;
    DWORD dwCount = 1;
    NET_DVR_CLOUDSTORAGE_COND struStorageCond = { 0 };
    struStorageCond.dwSize = sizeof(struStorageCond);
    struStorageCond.dwChannel = 1;
    NET_DVR_CLOUDSTORAGE_CFG struStorageCfg = { 0 };
    DWORD dwStatusList = 0;

    BOOL bIsSupportCloudStorage = FALSE;

    if (NET_DVR_GetDeviceConfig(g_struDeviceInfo[m_iDeviceIndex].lLoginID, dwCommand, dwCount, \
        &struStorageCond, sizeof(NET_DVR_CLOUDSTORAGE_COND), &dwStatusList, &struStorageCfg, sizeof(NET_DVR_CLOUDSTORAGE_CFG)))
    {
        if (struStorageCfg.byEnable == 1)
        {
            //如果支持url，则直接返回
            bIsSupportCloudStorage = TRUE;
            GetDlgItem(IDC_COMBO_FACE_SNAP_DATA_TYPE)->EnableWindow(bIsSupportCloudStorage);
            GetDlgItem(IDC_COMBO_FACE_MATCH_DATA_TYPE)->EnableWindow(bIsSupportCloudStorage);
            GetDlgItem(IDC_COMBO_EVENT_JSON_DATA_TYPE)->EnableWindow(bIsSupportCloudStorage);
            if (m_bIsSupportBehaviorUploadByCloudStorageURL)//是否支持支持异常行为检测事件通过武汉云url上报
            {
                GetDlgItem(IDC_COMBO_BEHAVIOR)->EnableWindow(bIsSupportCloudStorage);
            }
            else
            {
                GetDlgItem(IDC_COMBO_BEHAVIOR)->EnableWindow(FALSE);
            }
            return;
        }
    }

    //云存储2.0isapi透传协议判断
    char szUrl[512] = "";
    sprintf(szUrl, "GET /ISAPI/ContentMgmt/channels/1/cloudStorage/1");
    DWORD dwOutputLen = 1024 * 1024 * 3 ;
    char *pOutBuf = new char[dwOutputLen];
    if (NULL == pOutBuf)
    {
        AfxMessageBox("Apply Memory Failed!\n");
        return;
    }
    memset(pOutBuf, 0, dwOutputLen);
    NET_DVR_XML_CONFIG_INPUT struXmlCfgIn = { 0 };
    struXmlCfgIn.dwSize = sizeof(struXmlCfgIn);
    struXmlCfgIn.lpRequestUrl = szUrl;
    struXmlCfgIn.dwRequestUrlLen = strlen(szUrl);

    NET_DVR_XML_CONFIG_OUTPUT struXmlCfgOut = { 0 };
    struXmlCfgOut.dwSize = sizeof(struXmlCfgOut);
    struXmlCfgOut.lpOutBuffer = pOutBuf;
    struXmlCfgOut.dwOutBufferSize = dwOutputLen;

    if (NET_DVR_STDXMLConfig(g_struDeviceInfo[m_iDeviceIndex].lLoginID, &struXmlCfgIn, &struXmlCfgOut))
    {
        CXmlBase xmlBase;
        xmlBase.Parse(pOutBuf);
        if (xmlBase.FindElem("CloudStorage") && xmlBase.IntoElem())
        {
            //CloudStorage->enable
            if (xmlBase.FindElem("enable") && xmlBase.GetData() == "true")
            {
                bIsSupportCloudStorage = TRUE;
            }
            //CloudStorage->V2.0->enabled
            if (!bIsSupportCloudStorage && xmlBase.FindElem("V2.0") && xmlBase.IntoElem())
            {
                if (  xmlBase.FindElem("enabled") && xmlBase.GetData() == "true")
                {
                    bIsSupportCloudStorage = TRUE;
                }
                else 
                {
                    xmlBase.OutOfElem();
                }
            }
            //CloudStorage->ImageAndVideoCloud->enabled
            if (!bIsSupportCloudStorage && xmlBase.FindElem("ImageAndVideoCloud") && xmlBase.IntoElem())
            {
                if(xmlBase.FindElem("enabled") && xmlBase.GetData() == "true")
                {
                    bIsSupportCloudStorage = TRUE;
                }
            }
        }
    }
    //用于调试
    //else
    //{
    //    char szErrorMsg[512]= { 0 };
    //    sprintf(szErrorMsg,"ErrrorCode:%d .\n Error Msg: %s.", (char*)NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
    //    MessageBox(szErrorMsg);
    //}
    GetDlgItem(IDC_COMBO_FACE_SNAP_DATA_TYPE)->EnableWindow(bIsSupportCloudStorage);
    GetDlgItem(IDC_COMBO_FACE_MATCH_DATA_TYPE)->EnableWindow(bIsSupportCloudStorage);
    GetDlgItem(IDC_COMBO_EVENT_JSON_DATA_TYPE)->EnableWindow(bIsSupportCloudStorage);
    if (m_bIsSupportBehaviorUploadByCloudStorageURL)//是否支持支持异常行为检测事件通过武汉云url上报
    {
        GetDlgItem(IDC_COMBO_BEHAVIOR)->EnableWindow(bIsSupportCloudStorage);
    }
    else
    {
        GetDlgItem(IDC_COMBO_BEHAVIOR)->EnableWindow(FALSE);
    }

    if (pOutBuf != NULL)
    {
        delete[] pOutBuf;
        pOutBuf = NULL;
    }
}

/////////////////////////////////////////////////////////////////////////////
/*********************************************************
Function:  setDeviceAbility
Desc:      Get the alarm ability (ITDeviceAbility, EventAbility, VcaChanAbility, VehicleRecogAbility and so on) based on the DevieceIndex
           maximize the alarm ability(one channel supports means all channel support)
Input:     none
Output:    none
Return:    none
**********************************************************/
BOOL CAlarmAdvanceSetter::setDeviceAbility()
{
   
    //check that whether the device supports AlarmLevel, ITSUploadType, Copilot Face ability
    CheckITDeviceAbility();

    //check that whether the device supports FaceDetectionType:FaceDetection ability
    CheckEventAbility();  
  
    //check that whether the device supports FaceDetectionType:FaceSnaphot ability
    CheckVcaChanAbility();  


    //To Judge the FaceDetectionType
    if (m_bIsSupportFaceSnap && m_bIsSupportFaceDetection)
    {
        m_cmbFaceDetectType.EnableWindow(TRUE);
    }
    else if (m_bIsSupportFaceSnap)
    {
        m_cmbFaceDetectType.SetCurSel(0);
        m_cmbFaceDetectType.EnableWindow(FALSE);
    }
    else
    {
        m_cmbFaceDetectType.SetCurSel(1);
        m_cmbFaceDetectType.EnableWindow(FALSE);
    }

    /*//确认机制 代码有问题，条件永远不成立，通过获取设备能力集判断确认机制是否放开
    if (!(g_struDeviceInfo[m_iDeviceIndex].bySupport2 & 0x08 == 1))
    {
        GetDlgItem(IDC_CHECK_CONFIRM_MECHANISM)->EnableWindow(FALSE);

    }*/
    //支持哪些断网续传类型，确认机制
    CheckBasicCapability();

    //是否支持人脸对比断网续传类型
    CheckFaceContrast();

    //是否支持任务处理号
    CheckVehicleRecogAbility();

    //报警图片数据传输存储类型
    CheckCloudStorageCfg();



    ////查询VideoPicAbility
    //dwAbilityType = DEVICE_VIDEOPIC_ABILITY;
    //sprintf(pInBuf, "<VideoPicAbility version=\"2.0\">\r\n<channelNO>1</channelNO>\r\n</VideoPicAbility>");
    //memset(pOutBuf, 0, dwOutputLen);

    //if (NET_DVR_GetDeviceAbility(g_struDeviceInfo[m_iDeviceIndex].lLoginID, dwAbilityType, pInBuf, strlen(pInBuf), pOutBuf, dwOutputLen))
    //{
    //    CXmlBase xmlBase;
    //    xmlBase.Parse(pOutBuf);

    //    if (xmlBase.FindElem("VideoPicAbility") && xmlBase.IntoElem() && xmlBase.FindElem("MotionDetection"))
    //    {
    //        m_bIsSupportAlarmTypeV40 = TRUE;
    //    }
    //}

    return TRUE;
}

//选择订阅信息的类型，ALL 或者 LIST
void CAlarmAdvanceSetter::OnCbnSelchangeComboAlarmSubscribeInfo()
{
    CString csSelect = "";
    m_cmbAlarmSubscribeInfo.GetLBText(m_cmbAlarmSubscribeInfo.GetCurSel(), csSelect);
    if (m_cmbAlarmSubscribeInfo.GetCurSel() == 0)  //ALL
    {
        //禁用所有的LIST选项
        SetSubscribeItemsEnabled(FALSE);
    }
    else if (m_cmbAlarmSubscribeInfo.GetCurSel() == 1) //LIST
    {
        //启用所有的LIST选项
        SetSubscribeItemsEnabled(TRUE);
    }
    else
    {
        char szLan[1024] = { 0 };
        g_StringLanType(szLan, "未选中订阅信息！", "Pick the Subscription Type!");
        AfxMessageBox(szLan);
    }
}

/////////////////////////////////////////////////////////////////////////////
/*********************************************************
Function:  SetSubscribeItemsEnabled
Desc:      Change the subscribe check box state 
Input:     enabled(the state of subscribe check box)
Output:    none
Return:    none
**********************************************************/
void CAlarmAdvanceSetter::SetSubscribeItemsEnabled(BOOL enabled)
{
    //GetDlgItem(IDC_STATIC_LIST_SUBSCRIBE_LIST)->EnableWindow(enabled);
    GetDlgItem(IDC_COMBO_EVENT_TYPE)->EnableWindow(enabled);
    GetDlgItem(IDC_LIST_EVENT_PARAM)->EnableWindow(enabled);
    GetDlgItem(IDC_BUTTON_ADD_PARAM)->EnableWindow(enabled);
    GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(enabled);
}

/////////////////////////////////////////////////////////////////////////////
/*********************************************************
Function:  OnBnClickedButtonConfirm
Desc:      Save the AlarmParam to m_pStruSetupAlarmParamV50
Input:     none
Output:    none
Return:    none
**********************************************************/
void CAlarmAdvanceSetter::OnBnClickedButtonConfirm()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    GenerateSubcribeParam();
    CloseAbilityThread();
    m_pStruSetupAlarmParamV50->dwSize = sizeof(NET_DVR_SETUPALARM_PARAM_V50);

    m_pStruSetupAlarmParamV50->byLevel = m_cmbAlarmLevel.GetCurSel();
    m_pStruSetupAlarmParamV50->byCustomCtrl = m_bCopilotFace;

    m_pStruSetupAlarmParamV50->byAlarmInfoType = m_cmbITSUploadType.GetCurSel();

    m_pStruSetupAlarmParamV50->byRetAlarmTypeV40 = TRUE;
    m_pStruSetupAlarmParamV50->byRetDevInfoVersion = TRUE;  //CVR上传报警信息类型(仅对接CVR时有效)
    m_pStruSetupAlarmParamV50->byRetVQDAlarmType = TRUE; //Prefer VQD Alarm type of NET_DVR_VQD_ALARM

    //m_bIsSupportFaceSnap       m_bIsSupportFaceDetection = FALSE;
    m_pStruSetupAlarmParamV50->byFaceAlarmDetection = m_cmbFaceDetectType.GetCurSel();

    m_pStruSetupAlarmParamV50->bySupport = 0;
    m_pStruSetupAlarmParamV50->bySupport |= (m_bConfirmMechanism << 1); //有问题
    m_pStruSetupAlarmParamV50->bySupport |= (1 << 2);

    m_pStruSetupAlarmParamV50->byBrokenNetHttp = 0;
    m_pStruSetupAlarmParamV50->byBrokenNetHttp |= (m_bChePaiBroken << 0);
    m_pStruSetupAlarmParamV50->byBrokenNetHttp |= (m_bKeLiuTongJiBroken << 1);
    m_pStruSetupAlarmParamV50->byBrokenNetHttp |= (m_bHotMapBroken << 2);
    m_pStruSetupAlarmParamV50->byBrokenNetHttp |= (m_bFaceSanpBroken << 3);
    m_pStruSetupAlarmParamV50->byBrokenNetHttp |= (m_bFaceMatchBroken << 4);
    m_pStruSetupAlarmParamV50->byBrokenNetHttp |= (m_bJsonPass << 5);
    m_pStruSetupAlarmParamV50->byBrokenNetHttp |= (m_bChkHeatMapDurationPDC << 6);
    m_pStruSetupAlarmParamV50->byBrokenNetHttp |= (m_bChkHeatMapIntersection << 7);

    m_pStruSetupAlarmParamV50->wTaskNo = m_iTaskNo;

    m_pStruSetupAlarmParamV50->byDeployType = m_cmbAlarmType.GetCurSel();

    if (m_bRetAlarmTypeV40 == TRUE) // 指定V40布防
    {
        m_pStruSetupAlarmParamV50->byRetAlarmTypeV40 = FALSE;
    }

    if (m_bNVRAlarmIPC == TRUE)  // NVR全量布防IPC
    {
        m_pStruSetupAlarmParamV50->bySupport |= 0x10;
    }

    if (m_bVMDPic == TRUE) // 移动侦测人车分类传图
    {
        m_pStruSetupAlarmParamV50->bySubScription |= 0x80;
    }

    //设置上传图片采用URL或者二进制
    m_pStruSetupAlarmParamV50->byAlarmTypeURL = 0;
    m_pStruSetupAlarmParamV50->byAlarmTypeURL |= (m_cmbFaceSnapDataType.GetCurSel() << 0);
    m_pStruSetupAlarmParamV50->byAlarmTypeURL |= (m_cmbEventJsonDataType.GetCurSel() << 1);
    m_pStruSetupAlarmParamV50->byAlarmTypeURL |= (m_cmbFaceMatchDataType.GetCurSel() << 2);
    m_pStruSetupAlarmParamV50->byAlarmTypeURL |= (m_cmbBehavior.GetCurSel() << 3);


    if (m_bEnableXml)
    {
        strncpy(m_pData, m_pSub, MAX_SUBSCRIBE_DATA_LENGTH);
    }
    else
    {
        strcpy(m_pData, m_strSubscribe.GetBuffer());
    }

    m_pStruSetupAlarmParamV50->byBrokenNetHttpV60 = 0;
    m_pStruSetupAlarmParamV50->byBrokenNetHttpV60 |= (m_bAidBroken << 0);
    m_pStruSetupAlarmParamV50->byBrokenNetHttpV60 |= (m_bTfsBroken << 1);
    m_pStruSetupAlarmParamV50->byBrokenNetHttpV60 |= (m_bTPsStatBroken << 2);
    m_pStruSetupAlarmParamV50->byBrokenNetHttpV60 |= (m_bRuleBroken << 3);
    //HWND hParentHnd = ::FindWindow(NULL, _T("布防/撤防"));
    //::SendMessage(hParentHnd, WM_ALARM_ADVANCED_SETTER, (DWORD)&struSetupAlarmParamV50, (LONG)&szData);
    OnCancel();

}

/////////////////////////////////////////////////////////////////////////////
/*********************************************************
Function:    setAdvanceAlarmParam
Desc:        Save the pointer of AlarmParam to m_pStruSetupAlarmParamV50
Input:    pStruSetupAlarmParamV50: the pointer of AlarmParam
          pData: the pointer of subscribe info data
Output:    none
Return:    none
**********************************************************/
void CAlarmAdvanceSetter::setAdvanceAlarmParam(NET_DVR_SETUPALARM_PARAM_V50* pStruSetupAlarmParamV50,char *pData)
{
    m_pStruSetupAlarmParamV50 = pStruSetupAlarmParamV50;
    m_pData = pData;
}


void CAlarmAdvanceSetter::OnBnClickedBtnXmlsub()
{
    // TODO:  在此添加控件通知处理程序代码
    m_DlgXmlSub.getXmlSub(m_pSub);
    m_DlgXmlSub.DoModal();
}

/** @fn void CAlarmAdvanceSetter::UpdateChanStatus()
 *  @brief 更新通道列表
 *  @return void
 */

void CAlarmAdvanceSetter::UpdateChanStatus()
{
    int iIndex = 0;
    int i = 0;
    CString csTemp;
    m_listChan.DeleteAllItems();
    //get the whole state of all channels

    if (m_iDeviceIndex < 0)
    {
        return;
    }

    for (i = 0; i < g_struDeviceInfo[m_iDeviceIndex].iDeviceChanNum; i++)
    {
        if (i < g_struDeviceInfo[m_iDeviceIndex].iAnalogChanNum)
        {
            csTemp.Format(ANALOG_C_FORMAT, g_struDeviceInfo[m_iDeviceIndex].iStartChan + i);
            m_listChan.InsertItem(iIndex, csTemp);
            m_listChan.SetItemData(iIndex, i + g_struDeviceInfo[m_iDeviceIndex].iStartChan);
            iIndex++;
        }
        else
        {
            csTemp.Format(DIGITAL_C_FORMAT, i + 1 - g_struDeviceInfo[m_iDeviceIndex].iAnalogChanNum/*g_struDeviceInfo[m_iDeviceIndex].iStartChan-g_struDeviceInfo[m_iDeviceIndex].pStruIPParaCfgV40[0].dwStartDChan*/);
            m_listChan.InsertItem(iIndex, csTemp);
            m_listChan.SetItemData(iIndex, i - g_struDeviceInfo[m_iDeviceIndex].iAnalogChanNum + g_struDeviceInfo[m_iDeviceIndex].pStruIPParaCfgV40[0].dwStartDChan);
            iIndex++;
        }
    }
}

/** @fn void CAlarmAdvanceSetter::OnBnClickedButtonGetSubscribe()
 *  @brief 获取订阅参数
 *  @return void
 */

void CAlarmAdvanceSetter::OnBnClickedButtonGetSubscribe()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    if (m_iDeviceIndex < 0)
    {
        return;
    }

    DWORD dwOutputLen = 1024 * 1024;
    char *pOutBuf = new char[dwOutputLen];
    memset(pOutBuf, 0, dwOutputLen);

    if (NET_DVR_GetAlarmSubscribe(g_struDeviceInfo[m_iDeviceIndex].lFortifyHandle, pOutBuf, dwOutputLen))
    {
        m_strSubscribe = pOutBuf;
        GenerateSubcribeListData();
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_GetAlarmSubscribe");
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GetAlarmSubscribe");
    }

    if (pOutBuf != NULL)
    {
        delete[]pOutBuf;
        pOutBuf = NULL;
    }
    UpdateData(FALSE);
}


/** @fn void CAlarmAdvanceSetter::OnBnClickedButtonSetSubscribe()
 *  @brief 设置订阅参数
 *  @return void
 */

void CAlarmAdvanceSetter::OnBnClickedButtonSetSubscribe()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    if (m_iDeviceIndex < 0)
    {
        return;
    }

    GenerateSubcribeParam();

    DWORD dwInputLen = m_strSubscribe.GetLength();
    char *pInBuf = m_strSubscribe.GetBuffer();

    if (NET_DVR_SetAlarmSubscribe(g_struDeviceInfo[m_iDeviceIndex].lFortifyHandle, pInBuf, dwInputLen))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_SetAlarmSubscribe");
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_SetAlarmSubscribe");
    }
    UpdateData(FALSE);
}

/** @fn void CAlarmAdvanceSetter::GetModeAllSubscribeParam(CString & strData)
 *  @brief 获取eventType为all时订阅参数
 *  @param (in)	CString & strData    
 *  @return void
 */

void CAlarmAdvanceSetter::GetModeAllSubscribeParam(CString & strData)
{
    CString strTemp = _T("");
    CString strChannels = _T("");
    bool bFirst = true;
    if (m_bChannelEnable)
    {
        CString strParam, strChannel = _T("");
        for (int i = 0; i < m_listChan.GetItemCount(); i++)
        {
            if (m_listChan.GetCheck(i))
            {
                int iChannel = -1;
                iChannel = NET_DVR_SDKChannelToISAPI(g_struDeviceInfo[m_iDeviceIndex].lLoginID, m_listChan.GetItemData(i), TRUE);
                if (bFirst)
                {
                    strChannel.Format("%d", iChannel);
                    bFirst = false;
                }
                else
                {
                    strChannel.Format(",%d", iChannel);
                }
                strParam += strChannel;
            }
        }
        strChannels.Format("%s%s%s", "<channels>", strParam, "</channels>\r\n");
    }

    if (m_comboPictureUrlType.GetCurSel() >= 0)
    {
        m_comboPictureUrlType.GetLBText(m_comboPictureUrlType.GetCurSel(), strTemp);
        strData.Format("%s%s%s%s", strChannels, "<pictureURLType>", strTemp, "</pictureURLType>\r\n");
    }
}

/** @fn void CAlarmAdvanceSetter::GetChangedUploadSubParam(CString & strData)
 *  @brief 获取变化上报参数
 *  @param (in)	CString & strData    
 *  @return void
 */

void CAlarmAdvanceSetter::GetChangedUploadSubParam(CString & strData)
{
    strData = _T("");
    if (m_bUploadAll)
    {
        strData += "<ChangedUploadSub>\r\n<StatusSub>\r\n<all>true</all>\r\n</StatusSub>\r\n</ChangedUploadSub>";
    }
    else if (m_bUploadChannel || m_bUploadHd || m_bUploadCapability)
    {
        strData += "<ChangedUploadSub>\r\n<StatusSub>\r\n";
        if (m_bUploadChannel)
        {
            strData += "<channel>true</channel>";
        }
        if (m_bUploadHd)
        {
            strData += "<hd>true</hd>";
        }
        if (m_bUploadCapability)
        {
            strData += "<capability>true</capability>";
        }

        strData += "\r\n</StatusSub>\r\n</ChangedUploadSub>";
    }
}

/** @fn void CAlarmAdvanceSetter::InitEventType()
 *  @brief 初始化订阅事件类型
 *  @return void
 */

void CAlarmAdvanceSetter::InitEventType()
{
    char szLan[128] = { 0 };
    UpdateData(TRUE);
    int iIndex = 0;
    m_comboEventType.ResetContent();
    for (iIndex = 0; iIndex < sizeof(szEventType) / sizeof(char*); iIndex++)
    {
        m_comboEventType.InsertString(iIndex, szEventType[iIndex]);
    }

    m_comboEventType.SetCurSel(0);//默认为移动侦测
    UpdateData(FALSE);
}

/** @fn void CAlarmAdvanceSetter::OnBnClickedButtonAddParam()
 *  @brief 增加订阅事件到表格中
 *  @return void
 */

void CAlarmAdvanceSetter::OnBnClickedButtonAddParam()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    int iRowCount = m_listEventParam.GetItemCount();
    CString strTemp = _T("");
    strTemp.Format("%d", iRowCount);
    m_listEventParam.InsertItem(iRowCount, strTemp);//序号
    strTemp = _T("");
    m_comboEventType.GetLBText(m_comboEventType.GetCurSel(), strTemp);
    m_listEventParam.SetItemText(iRowCount, 1, strTemp);//事件类型

    bool bFirst = true;
    if (m_bChannelEnable)
    {
        CString strChannel = _T("");
        strTemp = _T("");
        for (int i = 0; i < m_listChan.GetItemCount(); i++)
        {
            if (m_listChan.GetCheck(i))
            {
                int iChannel = -1;
                iChannel = NET_DVR_SDKChannelToISAPI(g_struDeviceInfo[m_iDeviceIndex].lLoginID, m_listChan.GetItemData(i), TRUE);
                if (bFirst)
                {
                    strChannel.Format("%d", iChannel);
                    bFirst = false;
                }
                else
                {
                    strChannel.Format(",%d", iChannel);
                }
                strTemp += strChannel;
            }
        }
        m_listEventParam.SetItemText(iRowCount, 2, strTemp);//通道号
    }
    else
    {
        m_listEventParam.SetItemText(iRowCount, 2, "all");//通道号
    }

    if (m_comboPictureUrlType.GetCurSel() >= 0)//图片类型
    {
        m_comboPictureUrlType.GetLBText(m_comboPictureUrlType.GetCurSel(), strTemp);
        m_listEventParam.SetItemText(iRowCount, 3, strTemp);
    }

    UpdateData(FALSE);
}

/** @fn void CAlarmAdvanceSetter::OnBnClickedButtonDeleteParam()
 *  @brief 删除表格中订阅参数
 *  @return void
 */

void CAlarmAdvanceSetter::OnBnClickedButtonDeleteParam()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    m_iRowCount = m_listEventParam.GetItemCount();
    if (m_iLastItem >= 0 && m_iLastItem < m_iRowCount)
    {
        m_listEventParam.DeleteItem(m_iLastItem);
    }
    UpdateData(FALSE);
}

/** @fn void CAlarmAdvanceSetter::OnNMClickListEventParam(NMHDR *pNMHDR, LRESULT *pResult)
 *  @brief 处理表格单击事件
 *  @param (in)	NMHDR * pNMHDR    
 *  @param (in)	LRESULT * pResult    
 *  @return void
 */

void CAlarmAdvanceSetter::OnNMClickListEventParam(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO:  在此添加控件通知处理程序代码
    m_iLastItem = pNMItemActivate->iItem;
    m_iSubItem = pNMItemActivate->iSubItem;
    //m_bSelectItem = TRUE;

    *pResult = 0;
}

/** @fn void CAlarmAdvanceSetter::GenerateSubcribeParam()
 *  @brief 根据控件生成订阅报文
 *  @return void
 */

void CAlarmAdvanceSetter::GenerateSubcribeParam()
{
    UpdateData(TRUE);
    int iIndex = 0;
    char szData[MAX_SUBSCRIBE_DATA_LENGTH] = { 0 };
    CString strTemp = _T("");
    if (m_cmbAlarmSubscribeInfo.GetCurSel() == 0) //ALL
    {
        m_strSubscribe = "<SubscribeEvent version=\"2.0\" xmlns=\"http://www.isapi.org/ver20/XMLSchema\">\r\n<eventMode>all</eventMode>\r\n";
        GetChangedUploadSubParam(strTemp);
        m_strSubscribe += strTemp;
        GetModeAllSubscribeParam(strTemp);
        m_strSubscribe += strTemp;
        m_strSubscribe += "</SubscribeEvent>";
    }
    else
    {
        m_strSubscribe = "<SubscribeEvent version=\"2.0\" xmlns=\"http://www.isapi.org/ver20/XMLSchema\">\r\n<eventMode>list</eventMode>\r\n<EventList>\r\n";
        for (iIndex = 0; iIndex < m_listEventParam.GetItemCount(); iIndex++)
        {
            sprintf(szData, "%s%s%s%s", szData, "<Event>\r\n<type>", m_listEventParam.GetItemText(iIndex, 1), "</type>\r\n");
            if ("def" != m_listEventParam.GetItemText(iIndex, 3))
            {
                sprintf(szData, "%s%s%s%s", szData, "<pictureURLType>", m_listEventParam.GetItemText(iIndex, 3), "</pictureURLType>\r\n");
            }
            if ("all" != m_listEventParam.GetItemText(iIndex, 2))
            {
                sprintf(szData, "%s%s%s%s", szData, "<channels>", m_listEventParam.GetItemText(iIndex, 2), "</channels>\r\n");
            }
            sprintf(szData, "%s%s", szData, "</Event>\r\n");
        }
        m_strSubscribe += szData;
        m_strSubscribe += "</EventList>\r\n";

        GetChangedUploadSubParam(strTemp);
        m_strSubscribe += strTemp;

        m_strSubscribe += "</SubscribeEvent>";
    }
}

/** @fn void CAlarmAdvanceSetter::GenerateSubcribeListData()
 *  @brief 将订阅报文显示到控件
 *  @return void
 */

void CAlarmAdvanceSetter::GenerateSubcribeListData()
{
    m_listEventParam.DeleteAllItems();
    int iIndex = 0;

    CString csTemp;
    CXmlBase xmlBase;
    xmlBase.Parse(m_strSubscribe.GetBuffer(0));

    m_bUploadAll = FALSE;
    m_bUploadChannel = FALSE;
    m_bUploadHd = FALSE;
    m_bUploadCapability = FALSE;

    if (xmlBase.FindElem("SubscribeEvent") && xmlBase.IntoElem())
    {
        if (xmlBase.FindElem("eventMode") && "list" == xmlBase.GetData())
        {
            m_cmbAlarmSubscribeInfo.SetCurSel(1);
            OnCbnSelchangeComboAlarmSubscribeInfo();
            if (xmlBase.FindElem("EventList") && xmlBase.IntoElem())
            {
                do
                {
                    if (xmlBase.FindElem("Event") && xmlBase.IntoElem())
                    {
                        if (xmlBase.FindElem("type"))
                        {
                            csTemp.Format("%d", iIndex);
                            m_listEventParam.InsertItem(iIndex, csTemp);

                            csTemp.Format("%s", xmlBase.GetData().c_str());
                            m_listEventParam.SetItemText(iIndex, 1, csTemp);

                            if (xmlBase.FindElem("pictureURLType"))
                            {
                                csTemp.Format("%s", xmlBase.GetData().c_str());
                                m_listEventParam.SetItemText(iIndex, 3, csTemp);
                            }
                            else
                            {
                                m_listEventParam.SetItemText(iIndex, 3, "def");
                            }

                            if (xmlBase.FindElem("channels"))
                            {
                                csTemp.Format("%s", xmlBase.GetData().c_str());
                                m_listEventParam.SetItemText(iIndex, 2, csTemp);
                            }
                            else
                            {
                                m_listEventParam.SetItemText(iIndex, 2, "all");
                            }

                            iIndex++;
                        }
                        xmlBase.OutOfElem();
                    }
                } while (xmlBase.NextSibElem());

                xmlBase.OutOfElem();
            }
            if (xmlBase.FindElem("ChangedUploadSub") && xmlBase.IntoElem())
            {
                if (xmlBase.FindElem("StatusSub") && xmlBase.IntoElem())
                {
                    if (xmlBase.FindElem("all") && "true" == xmlBase.GetData())
                    {
                        m_bUploadAll = TRUE;
                    }
                    else
                    {
                        m_bUploadAll = FALSE;
                    }

                    if (xmlBase.FindElem("channel") && "true" == xmlBase.GetData())
                    {
                        m_bUploadChannel = TRUE;
                    }
                    else
                    {
                        m_bUploadChannel = FALSE;
                    }

                    if (xmlBase.FindElem("hd") && "true" == xmlBase.GetData())
                    {
                        m_bUploadHd = TRUE;
                    }
                    else
                    {
                        m_bUploadHd = FALSE;
                    }

                    if (xmlBase.FindElem("capability") && "true" == xmlBase.GetData())
                    {
                        m_bUploadCapability = TRUE;
                    }
                    else
                    {
                        m_bUploadCapability = FALSE;
                    }

                    xmlBase.OutOfElem();
                }
                xmlBase.OutOfElem();
            }
        }
        else if (xmlBase.FindElem("eventMode") && "all" == xmlBase.GetData())
        {
            m_cmbAlarmSubscribeInfo.SetCurSel(0);
            OnCbnSelchangeComboAlarmSubscribeInfo();

            if (xmlBase.FindElem("ChangedUploadSub") && xmlBase.IntoElem())
            {
                if (xmlBase.FindElem("StatusSub") && xmlBase.IntoElem())
                {
                    if (xmlBase.FindElem("all") && "true" == xmlBase.GetData())
                    {
                        m_bUploadAll = TRUE;
                    }
                    else
                    {
                        m_bUploadAll = FALSE;
                    }

                    if (xmlBase.FindElem("channel") && "true" == xmlBase.GetData())
                    {
                        m_bUploadChannel = TRUE;
                    }
                    else
                    {
                        m_bUploadChannel = FALSE;
                    }

                    if (xmlBase.FindElem("hd") && "true" == xmlBase.GetData())
                    {
                        m_bUploadHd = TRUE;
                    }
                    else
                    {
                        m_bUploadHd = FALSE;
                    }

                    if (xmlBase.FindElem("capability") && "true" == xmlBase.GetData())
                    {
                        m_bUploadCapability = TRUE;
                    }
                    else
                    {
                        m_bUploadCapability = FALSE;
                    }

                    xmlBase.OutOfElem();
                }
                xmlBase.OutOfElem();
            }

            if (xmlBase.FindElem("channels"))
            {
                m_bChannelEnable = TRUE;
                OnBnClickedCheckChannelEnable();
                for (int i = 0; i < m_listChan.GetItemCount(); i++)
                {
                    m_listChan.SetCheck(i, FALSE);

                    csTemp.Format("%d", m_listChan.GetItemData(i));

                    char *tokenPtr = strtok((char*)xmlBase.GetData().c_str(), ",");
                    while (tokenPtr != NULL)
                    {
                        if (tokenPtr == csTemp)
                        {
                            m_listChan.SetCheck(i, TRUE);
                            break;
                        }
                        tokenPtr = strtok(NULL, ",");
                    }
                }
            }
            else
            {
                m_bChannelEnable = FALSE;
                OnBnClickedCheckChannelEnable();
            }

            if (xmlBase.FindElem("pictureURLType"))
            {
                for (int i = 0; i < m_comboPictureUrlType.GetCount(); i++)
                {
                    m_cmbAlarmSubscribeInfo.GetLBText(i, csTemp);

                    if (csTemp == xmlBase.GetData().c_str())
                    {
                        m_comboPictureUrlType.SetCurSel(i);
                    }
                }
            }
            else
            {
                m_comboPictureUrlType.SetCurSel(0);
            }
        }
        xmlBase.OutOfElem();
    }
}



/** @fn void CAlarmAdvanceSetter::OnBnClickedCheckChannelEnable()
 *  @brief 通道订阅使能单击事件
 *  @return void
 */

void CAlarmAdvanceSetter::OnBnClickedCheckChannelEnable()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    GetDlgItem(IDC_LIST_CHAN)->EnableWindow(m_bChannelEnable);
}

/** @fn void CAlarmAdvanceSetter::GetSubcribeCapability()
 *  @brief 获取报警订阅能力
 *  @return void
 */

void CAlarmAdvanceSetter::GetSubcribeCapability()
{

    NET_DVR_XML_CONFIG_INPUT  xmlConfigInput = { 0 };
    NET_DVR_XML_CONFIG_OUTPUT  xmlCongfigOutput = { 0 };
    xmlConfigInput.dwSize = sizeof(xmlConfigInput);
    xmlCongfigOutput.dwSize = sizeof(xmlCongfigOutput);

    char szUrl[256] = "";
    DWORD dwOutputLen = 1024 * 1024;
    char *pOutBuf = new char[dwOutputLen];

    BOOL bIsSupportFaceMatchBroken = FALSE;

    sprintf(szUrl, "GET /ISAPI/Event/notification/subscribeEventCap");
    xmlConfigInput.lpRequestUrl = szUrl;
    xmlConfigInput.dwRequestUrlLen = strlen(szUrl);

    memset(pOutBuf, 0, dwOutputLen);
    xmlCongfigOutput.dwOutBufferSize = dwOutputLen;
    xmlCongfigOutput.lpOutBuffer = pOutBuf;

    if (NET_DVR_STDXMLConfig(g_struDeviceInfo[m_iDeviceIndex].lLoginID, &xmlConfigInput, &xmlCongfigOutput))
    {
        CXmlBase xmlBase;
        xmlBase.Parse(pOutBuf);

        if ((xmlBase.FindElem("SubscribeEventCap") && xmlBase.IntoElem()))
        {
            //判断是否支持通道订阅
            if (xmlBase.FindElem("channelMode"))
            {
                GetDlgItem(IDC_CHECK_CHANNEL_ENABLE)->EnableWindow(FALSE);
                if (xmlBase.GetAttributeValue("opt").find("list"))
                {
                    GetDlgItem(IDC_CHECK_CHANNEL_ENABLE)->EnableWindow(TRUE);
                }
            }
            //判断是否支持事件订阅
            if (xmlBase.FindElem("eventMode"))
            {
                m_cmbAlarmSubscribeInfo.ResetContent();
                int iIndex = 0;
                CString strTmp = strtok((char*)xmlBase.GetAttributeValue("opt").c_str(), ",");
                while (strTmp != _T(""))
                {
                    strTmp.TrimLeft();
                    m_cmbAlarmSubscribeInfo.InsertString(iIndex, strTmp);
                    iIndex++;

                    strTmp = strtok(NULL, ",");
                }

                m_cmbAlarmSubscribeInfo.SetCurSel(0);
            }
            //解析支持订阅事件类型
            if (xmlBase.FindElem("EventList") && xmlBase.IntoElem())
            {
                m_comboEventType.ResetContent();
                int iIndex = 0;
                do
                {
                    if (xmlBase.FindElem("Event") && xmlBase.IntoElem())
                    {
                        if (xmlBase.FindElem("type"))
                        {
                            m_listSubcirbeEventCap[iIndex].strEventType = xmlBase.GetData();
                            m_comboEventType.InsertString(iIndex, xmlBase.GetData().c_str());

                            if (xmlBase.FindElem("pictureURLType"))
                            {
                                m_listSubcirbeEventCap[iIndex].strPictureUrlType = xmlBase.GetAttributeValue("opt");
                                m_listSubcirbeEventCap[iIndex].strDefaultPictureUrlType = xmlBase.GetAttributeValue("def");
                            }
                            iIndex++;
                        }
                        xmlBase.OutOfElem();
                    }
                } while (xmlBase.NextSibElem());

                m_comboEventType.SetCurSel(0);

                xmlBase.OutOfElem();
            }
            //判断是否支持图片格式订阅
            if (xmlBase.FindElem("pictureURLType"))
            {
                m_comboPictureUrlType.ResetContent();
                m_strAllPictureUrlType = xmlBase.GetAttributeValue("opt");

                int iIndex = 0;
                CString strTmp = strtok((char *)m_strAllPictureUrlType.c_str(), ",");
                while (strTmp != _T(""))
                {
                    strTmp.TrimLeft();
                    m_comboPictureUrlType.InsertString(iIndex, strTmp);
                    iIndex++;

                    strTmp = strtok(NULL, ",");
                }
                m_comboPictureUrlType.SetCurSel(0);
                //暂时不使用默认的图片url格式
            }

            xmlBase.OutOfElem();
        }
    }
    else
    {
        m_cmbAlarmSubscribeInfo.EnableWindow(FALSE);
        GetDlgItem(IDC_CHECK_CHANNEL_ENABLE)->EnableWindow(FALSE);
        GetDlgItem(IDC_COMBO_EVENT_TYPE)->EnableWindow(FALSE);
        GetDlgItem(IDC_COMBO_PICTURE_URL_TYPE)->EnableWindow(FALSE);
    }
    if (pOutBuf != NULL)
    {
        delete[]pOutBuf;
        pOutBuf = NULL;
    }
}



/** @fn void CAlarmAdvanceSetter::OnCbnSelchangeComboEventType()
 *  @brief 事件类型切换响应
 *  @return void
 */

void CAlarmAdvanceSetter::OnCbnSelchangeComboEventType()
{
    // TODO:  在此添加控件通知处理程序代码
    if (!m_bCheckSubcribeCap)//不校验能力集时默认切换时显示所有图片URL类型
    {
        return;
    }
    CString strSubscribeType = _T("");
    m_cmbAlarmSubscribeInfo.GetLBText(m_cmbAlarmSubscribeInfo.GetCurSel(), strSubscribeType);
    if (strSubscribeType == "all")
    {
        //清除原有图片url类型列表
        m_comboPictureUrlType.ResetContent();
        m_comboPictureUrlType.SetWindowText("");
        //根据所有事件图片url能力显示界面
        if (m_strAllPictureUrlType != _T(""))
        {
            if (m_strAllPictureUrlType.find(","))
            {
                int iIndex = 0;
                CString strTmp = strtok((char*)m_strAllPictureUrlType.c_str(), ",");
                while (strTmp != _T(""))
                {
                    strTmp.TrimLeft();
                    m_comboPictureUrlType.InsertString(iIndex, strTmp);
                    iIndex++;

                    strTmp = strtok(NULL, ",");
                }
            }
            else
            {
                m_comboPictureUrlType.InsertString(0, m_strAllPictureUrlType.c_str());
            }
            m_comboPictureUrlType.SetCurSel(0);
        }

    }
    else if (strSubscribeType == "list")
    {
        //清除原有图片url类型列表
        m_comboPictureUrlType.ResetContent();
        m_comboPictureUrlType.SetWindowText("");
        //根据当前事件图片url能力显示界面
        CString strEventType = _T("");
        m_comboEventType.GetLBText(m_comboEventType.GetCurSel(), strEventType);
        for (int i = 0; i < MAX_EVENT_TYPE_NUM; i++)
        {
            if (m_listSubcirbeEventCap[i].strEventType.c_str() == strEventType)
            {
                if (m_listSubcirbeEventCap[i].strPictureUrlType != _T(""))
                {
                    if (m_listSubcirbeEventCap[i].strPictureUrlType.find(","))
                    {
                        int iIndex = 0;
                        CString strTmp = strtok((char*)m_listSubcirbeEventCap[i].strPictureUrlType.c_str(), ",");
                        while (strTmp != _T(""))
                        {
                            strTmp.TrimLeft();
                            m_comboPictureUrlType.InsertString(iIndex, strTmp);
                            iIndex++;

                            strTmp = strtok(NULL, ",");
                        }
                    }
                    else
                    {
                        m_comboPictureUrlType.InsertString(0, m_listSubcirbeEventCap[i].strPictureUrlType.c_str());
                    }
                    m_comboPictureUrlType.SetCurSel(0);
                }
            }
        }
    }
}


void CAlarmAdvanceSetter::OnBnClickedCheckSubcribeCap()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    if (m_bCheckSubcribeCap && m_iDeviceIndex != -1)
    {
        GetSubcribeCapability();
    }
    else
    {
        InitEventType();
        m_comboPictureUrlType.ResetContent();
        m_comboPictureUrlType.InsertString(0, "def");
        m_comboPictureUrlType.InsertString(1, "binary");
        m_comboPictureUrlType.InsertString(2, "localURL");
        m_comboPictureUrlType.InsertString(3, "cloudStorageURL");
        m_comboPictureUrlType.SetCurSel(0);
    }
    UpdateData(FALSE);
}

void CAlarmAdvanceSetter::CreateAbilityThread()
{
    char szLan[128] = { 0 };
    DWORD dwThreadId = 0;
    if (NULL == m_hAbilityThread)
    {
        m_hAbilityThread = CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(GetAbilityThread), this, 0, &dwThreadId);
    }
        
    if (NULL == m_hAbilityThread)
    {
        g_StringLanType(szLan, "创建线程失败", "Open thread failed!");
        AfxMessageBox(szLan);
        return;
    }

    //设置断网续传初始化状态FALSE
    SetBrokenStatus(FALSE);
    return;
}

void CAlarmAdvanceSetter::CloseAbilityThread()
{
    if (NULL != m_hAbilityThread)
    {
        TerminateThread(m_hAbilityThread, 0);
        CloseHandle(m_hAbilityThread);
        m_hAbilityThread = NULL;
    }
}

void CAlarmAdvanceSetter::SetBrokenStatus(BOOL bStatus)
{
    GetDlgItem(IDC_CHECK_CHEPAI_BROKEN)->EnableWindow(bStatus);
    GetDlgItem(IDC_CHECK_KELIUTONGJI_BROKEN)->EnableWindow(bStatus);
    GetDlgItem(IDC_CHECK_HotMap_BROKEN)->EnableWindow(bStatus);
    GetDlgItem(IDC_CHECK_FACE_SNAP_BROKEN)->EnableWindow(bStatus);
    GetDlgItem(IDC_CHECK_FACE_MATCH_BROKEN)->EnableWindow(bStatus);
    GetDlgItem(IDC_CHECK_JSON_PASS)->EnableWindow(bStatus);
    GetDlgItem(IDC_CHECK_CONFIRM_MECHANISM)->EnableWindow(bStatus);
    //GetDlgItem(IDC_CHECK_HEATMAP_PDC_DURATION_BROKEN)->EnableWindow(bStatus);
    //GetDlgItem(IDC_CHECK_INTERSECTION_ANALYSIS_BROKEN)->EnableWindow(bStatus);

    GetDlgItem(IDC_CHECK_AID_BROKEN)->EnableWindow(bStatus);
    GetDlgItem(IDC_CHECK_TFS_BROKEN)->EnableWindow(bStatus);
    GetDlgItem(IDC_CHECK_TPS_STATISTIC_BROKEN)->EnableWindow(bStatus);
    GetDlgItem(IDC_CHECK_RULE_BROKEN)->EnableWindow(bStatus);
}


void CAlarmAdvanceSetter::OnClose()
{
    // TODO:  在此添加消息处理程序代码和/或调用默认值
    CloseAbilityThread();

    CDialogEx::OnClose();
}

