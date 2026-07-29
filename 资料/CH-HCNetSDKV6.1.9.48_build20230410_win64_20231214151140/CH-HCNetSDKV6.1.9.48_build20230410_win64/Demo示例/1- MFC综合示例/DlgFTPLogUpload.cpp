// DlgFTPLogUpload.cpp : 实现文件
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "DlgFTPLogUpload.h"
#include "afxdialogex.h"
#include "DeviceCfgFile.h"

// CDlgFTPLogUpload 对话框

IMPLEMENT_DYNAMIC(CDlgFTPLogUpload, CDialogEx)

CDlgFTPLogUpload::CDlgFTPLogUpload(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgFTPLogUpload::IDD, pParent)
    , m_lUserID(0)
    , m_iDevIndex(0)
    , m_lChannel(0)
    , m_strConCommand(_T(""))
    , m_strDevCode(_T(""))
    , m_strPassword(_T(""))
    , m_strServerIP(_T(""))
    , m_iServerPort(0)
    , m_strUserName(_T(""))
    , m_strCompkey(_T(""))
    , m_ctEndDate(COleDateTime::GetCurrentTime())
    , m_ctEndTime(COleDateTime::GetCurrentTime())
    , m_ctStartDate(COleDateTime::GetCurrentTime())
    , m_ctStartTime(COleDateTime::GetCurrentTime())
    , m_bEnableFTP(FALSE)
    , m_strDataType(_T(""))
{
    memset(m_szCommandBuf, 0, sizeof(m_szCommandBuf));
    m_lpOutputXml = new char[MAX_LEN_XML];
    memset(m_lpOutputXml, 0, MAX_LEN_XML);
}

CDlgFTPLogUpload::~CDlgFTPLogUpload()
{
    if (NULL != m_lpOutputXml)
    {
        delete[] m_lpOutputXml;
        m_lpOutputXml = NULL;
    }
}

void CDlgFTPLogUpload::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_PROTOCOL_TYPE, m_comboProtocolType);
    DDX_Text(pDX, IDC_EDIT_CONSOLE_PASSWORD, m_strConCommand);
    DDX_Text(pDX, IDC_EDIT_DEVICE_CODE, m_strDevCode);
    DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
    DDX_Text(pDX, IDC_EDIT_SERVER_IP, m_strServerIP);
    DDX_Text(pDX, IDC_EDIT_SERVER_PORT, m_iServerPort);
    DDX_Text(pDX, IDC_EDIT_USER_NAME, m_strUserName);
    DDX_DateTimeCtrl(pDX, IDC_END_DATETIMEPICKER_DATE, m_ctEndDate);
    DDX_DateTimeCtrl(pDX, IDC_END_DATETIMEPICKER_TIME, m_ctEndTime);
    DDX_DateTimeCtrl(pDX, IDC_START_DATETIMEPICKER_DATE, m_ctStartDate);
    DDX_DateTimeCtrl(pDX, IDC_START_DATETIMEPICKER_TIME, m_ctStartTime);
    DDX_Control(pDX, IDC_COMBO_IP_TYPE, m_comboIPType);
    DDX_Text(pDX, IDC_EDIT_COMP_KEY, m_strCompkey);
    DDX_Check(pDX, IDC_CHECK_ENABLE_FTP, m_bEnableFTP);
    DDX_Control(pDX, IDC_COMBO_ADDRTYPE, m_comboAddrType);
    DDX_Text(pDX, IDC_EDIT_DATA_TYPE, m_strDataType);
}


BEGIN_MESSAGE_MAP(CDlgFTPLogUpload, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_TEST, &CDlgFTPLogUpload::OnBnClickedBtnTest)
    ON_BN_CLICKED(IDC_BTN_SAVE, &CDlgFTPLogUpload::OnBnClickedBtnSave)
    ON_BN_CLICKED(IDC_BTN_GENERATE, &CDlgFTPLogUpload::OnBnClickedBtnGenerate)
    ON_BN_CLICKED(IDC_BTN_GET, &CDlgFTPLogUpload::OnBnClickedBtnGet)
    ON_CBN_SELCHANGE(IDC_COMBO_ADDRTYPE, &CDlgFTPLogUpload::OnCbnSelchangeComboAddrtype)
END_MESSAGE_MAP()


// CDlgFTPLogUpload 消息处理程序

BOOL CDlgFTPLogUpload::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    m_comboIPType.SetCurSel(0);
    m_comboProtocolType.SetCurSel(0);
    m_comboAddrType.SetCurSel(1);
    m_iServerPort = 21;
    char sAllIp[16][16] = { 0 };
    DWORD dwIpNum = 0;
    BOOL bBind = FALSE;
    if (!NET_DVR_GetLocalIP(sAllIp, &dwIpNum, &bBind))
    {
        MessageBox(NET_DVR_GetErrorMsg());
        return FALSE;
    }

    m_strServerIP = sAllIp[0];
    m_bEnableFTP = TRUE;
    UpdateData(FALSE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常:  OCX 属性页应返回 FALSE
}


/** @fn void CDlgFTPLogUpload::OnBnClickedBtnTest()
 *  @brief 测试诊断服务器的FTP通信功能
 *  @return void
 */
void CDlgFTPLogUpload::OnBnClickedBtnTest()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);

    cJSON *pRoot = cJSON_CreateObject();
    cJSON *pTemp = cJSON_CreateObject();
    if (pRoot == NULL || pTemp == NULL)
    {
        return;
    }
    CString strProType;
    m_comboProtocolType.GetLBText(m_comboProtocolType.GetCurSel(), strProType);
    cJSON_AddStringToObject(pTemp, "protocol", strProType);

    int iSel = m_comboAddrType.GetCurSel();
    if (iSel == 1) //ipaddress
    {
        cJSON_AddStringToObject(pTemp, "addressingFormatType", "ipaddress");
        iSel = m_comboIPType.GetCurSel();
        string strIPType;
        if (iSel == 0) //IPV4
        {
            strIPType = "ipv4";
            cJSON_AddStringToObject(pTemp, "ipVersion", strIPType.c_str());
            cJSON_AddStringToObject(pTemp, "ipV4Address", m_strServerIP);
        }
        else //IPV6
        {
            strIPType = "ipv6";
            cJSON_AddStringToObject(pTemp, "ipVersion", strIPType.c_str());
            cJSON_AddStringToObject(pTemp, "ipv6Address", m_strServerIP);
        }
    }
    else //hostname
    {
        cJSON_AddStringToObject(pTemp, "addressingFormatType", "hostname");
        cJSON_AddStringToObject(pTemp, "hostName", m_strServerIP);
    }

    cJSON_AddNumberToObject(pTemp, "portNo", m_iServerPort);
    cJSON_AddStringToObject(pTemp, "userName", m_strUserName);
    cJSON_AddStringToObject(pTemp, "password", m_strPassword);

    cJSON_AddItemToObject(pRoot, "TestDescription", pTemp);
    char* strOut = cJSON_Print(pRoot);
    int iMothod = 3; //POST
    CString strCommandStr = "/ISAPI/System/diagnosedData/server/test?format=json";
    CString strInputParam = strOut;
    CString strOutputParam;

    ISAPITransparent(iMothod, strCommandStr, strInputParam, strOutputParam);

    BOOL bRet = FALSE;
    cJSON* pRootResult = cJSON_Parse(strOutputParam);
    if (NULL == pRootResult)
    {
        if (NULL != pRoot)
        {
            cJSON_Delete(pRoot);
            pRoot = NULL;
        }
        free(strOut);
        return;
    }
    do
    {
        cJSON* pTestResult = cJSON_GetObjectItem(pRootResult, "TestResult");
        if (NULL == pTestResult)
        {
            break;
        }

        char zsErrorDescription[50] = {0};
        if (GetNodeVal_JSON(pTestResult, "errorDescription", zsErrorDescription, 50) == FALSE)
        {
            break;
        }
        string strTemp = zsErrorDescription;
        if (strTemp == "ok" || strTemp == "OK")
        {
            bRet = TRUE;
        }

    } while (0);

    if (NULL != pRootResult)
    {
        cJSON_Delete(pRootResult);
        pRootResult = NULL;
    }

    if (NULL != pRoot)
    {
        cJSON_Delete(pRoot);
        pRoot = NULL;
    }
    free(strOut);

    char szLan[128] = { 0 };
    if (!bRet)
    {
        g_StringLanType(szLan, "诊断服务器测试失败！", "Diagnostic server test failed!");
    }
    else
    {
        g_StringLanType(szLan, "诊断服务器测试成功！", "Diagnostic server test sucessed!");
    }
    AfxMessageBox(szLan);
    return;
}

/** @fn void CDlgFTPLogUpload::OnBnClickedBtnGenerate()
 *  @brief 获取特征码
 *  @return void
 */
void CDlgFTPLogUpload::OnBnClickedBtnGenerate()
{
    // TODO:  在此添加控件通知处理程序代码
    int iMothod = 0; //GET
    CString strCommandStr = "/ISAPI/System/deviceInfo/characteristicCode?format=json";
    CString strInputParam;
    CString strOutputParam;

    ISAPITransparent(iMothod, strCommandStr, strInputParam, strOutputParam);

    cJSON *root;
    root = cJSON_Parse(strOutputParam);
    char zsCharacteristic[129] = { 0 };
    if (NULL == root)
    {
        OutputDebugString("get json root failed !\n");
    }
    else
    {
        cJSON *characteristic = cJSON_GetObjectItem(root, "Characteristic");
        if (!characteristic)
        {
            OutputDebugString("get json characteristic failed !\n");
        }
        else
        {
            cJSON *code = cJSON_GetObjectItem(characteristic, "code");
            if (!code)
            {
                return;
            }
            strncpy(zsCharacteristic, code->valuestring, sizeof(zsCharacteristic) - 1);
        }
    }

    if (strlen(zsCharacteristic) > 0)
    {
        m_strDevCode = zsCharacteristic;
    }

    if (NULL != root)
    {
        cJSON_Delete(root);
        root = NULL;
    }

    UpdateData(FALSE);
    return;
}


/** @fn void CDlgFTPLogUpload::OnBnClickedBtnSave()
 *  @brief 设置诊断服务参数
 *  @return void
 */
void CDlgFTPLogUpload::OnBnClickedBtnSave()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    NET_DVR_TIME struStartTime = { 0 };
    struStartTime.dwYear = m_ctStartDate.GetYear();
    struStartTime.dwMonth = m_ctStartDate.GetMonth();
    struStartTime.dwDay = m_ctStartDate.GetDay();
    struStartTime.dwHour = m_ctStartTime.GetHour();
    struStartTime.dwMinute = m_ctStartTime.GetMinute();
    struStartTime.dwSecond = m_ctStartTime.GetSecond();

    NET_DVR_TIME struEndTime = { 0 };
    struEndTime.dwYear = m_ctEndDate.GetYear();
    struEndTime.dwMonth = m_ctEndDate.GetMonth();
    struEndTime.dwDay = m_ctEndDate.GetDay();
    struEndTime.dwHour = m_ctEndTime.GetHour();
    struEndTime.dwMinute = m_ctEndTime.GetMinute();
    struEndTime.dwSecond = m_ctEndTime.GetSecond();
    char szStartTime[128] = { 0 };
    sprintf(szStartTime, "%04d-%02d-%02dT%02d:%02d:%02d+08:00", struStartTime.dwYear, struStartTime.dwMonth, struStartTime.dwDay, struStartTime.dwHour, struStartTime.dwMinute, struStartTime.dwSecond);
    char szEndTime[128] = { 0 };
    sprintf(szEndTime, "%04d-%02d-%02dT%02d:%02d:%02d+08:00", struEndTime.dwYear, struEndTime.dwMonth, struEndTime.dwDay, struEndTime.dwHour, struEndTime.dwMinute, struEndTime.dwSecond);

    cJSON *pRoot = cJSON_CreateObject();
    cJSON *pTemp = cJSON_CreateObject();
    cJSON *pList = cJSON_CreateArray();
    cJSON *pDiagnosedDataUpload = cJSON_CreateObject();
    if (NULL == pRoot || NULL == pTemp || NULL == pList || NULL == pDiagnosedDataUpload)
    {
        return;
    }
    CString strProType;
    m_comboProtocolType.GetLBText(m_comboProtocolType.GetCurSel(), strProType);
    cJSON_AddNumberToObject(pTemp, "id", 1);
    cJSON_AddStringToObject(pTemp, "protocol", strProType);
    cJSON_AddBoolToObject(pTemp, "enabled", m_bEnableFTP);

    int iSel = m_comboAddrType.GetCurSel();
    if (iSel == 1) //ipaddress
    {
        cJSON_AddStringToObject(pTemp, "addressingFormatType", "ipaddress");
        iSel = m_comboIPType.GetCurSel();
        string strIPType;
        if (iSel == 0) //IPV4
        {
            strIPType = "ipv4";
            cJSON_AddStringToObject(pTemp, "ipVersion", strIPType.c_str());
            cJSON_AddStringToObject(pTemp, "ipV4Address", m_strServerIP);
        }
        else //IPV6
        {
            strIPType = "ipv6";
            cJSON_AddStringToObject(pTemp, "ipVersion", strIPType.c_str());
            cJSON_AddStringToObject(pTemp, "ipv6Address", m_strServerIP);
        }
    }
    else //hostname
    {
        cJSON_AddStringToObject(pTemp, "addressingFormatType", "hostname");
        cJSON_AddStringToObject(pTemp, "hostName", m_strServerIP);
    }

    cJSON_AddNumberToObject(pTemp, "portNo", m_iServerPort);
    cJSON_AddStringToObject(pTemp, "userName", m_strUserName);
    cJSON_AddStringToObject(pTemp, "password", m_strPassword);

    cJSON_AddStringToObject(pDiagnosedDataUpload, "consoleCommand", m_strConCommand);
    cJSON_AddStringToObject(pDiagnosedDataUpload, "compressionKey", m_strCompkey);
    cJSON_AddStringToObject(pDiagnosedDataUpload, "startTime", szStartTime);
    cJSON_AddStringToObject(pDiagnosedDataUpload, "endTime", szEndTime);
    cJSON_AddItemToObject(pTemp, "DiagnosedDataUpload", pDiagnosedDataUpload);

    cJSON_AddStringToObject(pTemp, "dataType", m_strDataType);

    cJSON_AddItemToArray(pList, pTemp);
    cJSON_AddItemToObject(pRoot, "DiagnosedDataServerList", pList);
    char* strOut = cJSON_Print(pRoot);
    int iMothod = 1; //PUT
    CString strCommandStr = "/ISAPI/System/diagnosedData/server?format=json";
    CString strInputParam = strOut;
    CString strOutputParam;

    ISAPITransparent(iMothod, strCommandStr, strInputParam, strOutputParam); //透传ISPAI协议请求

    char szLan[128] = { 0 };
    BOOL bRet = FALSE;
    char szSubStatus[1024] = { 0 };
    cJSON* pRootResult = cJSON_Parse(strOutputParam); //解析报文
    if (NULL == pRootResult)
    {
        if (NULL != pRoot)
        {
            cJSON_Delete(pRoot);
            pRoot = NULL;
        }
        free(strOut);
        return;
    }
    do
    {
        if (GetNodeVal_JSON(pRootResult, "subStatusCode", szSubStatus, 1024) == FALSE)
        {
            break;
        }
        string strTemp = szSubStatus;
        if (strTemp == "ok" || strTemp == "OK")
        {
            g_StringLanType(szLan, "诊断信息服务器保存成功！", "Diagnostic server save sucessed!");
        }
        else if (strTemp == "incorrectConsolePassword")
        {
            g_StringLanType(szLan, "诊断信息服务器保存失败,控制台密码错误！", "Diagnostic server save failed. Incorrect console password!");
        }
        else
        {
            g_StringLanType(szLan, "诊断信息服务器保存失败！", "Diagnostic server save failed!");
        }
    } while (0);


    if (NULL != pRootResult)
    {
        cJSON_Delete(pRootResult);
        pRootResult = NULL;
    }

    if (NULL != pRoot)
    {
        cJSON_Delete(pRoot);
        pRoot = NULL;
    }
    free(strOut);

    AfxMessageBox(szLan);
    return;
}

/** @fn void CDlgFTPLogUpload::OnBnClickedBtnGet()
 *  @brief 获取诊断服务器配置参数
 *  @return void
 */
void CDlgFTPLogUpload::OnBnClickedBtnGet()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    NET_DVR_TIME struStartTime = { 0 };
    NET_DVR_TIME struEndTime = { 0 };

    int iMothod = 0; //GET
    CString strCommandStr = "/ISAPI/System/diagnosedData/server?format=json";
    CString strInputParam = "";
    CString strOutputParam = "";

    ISAPITransparent(iMothod, strCommandStr, strInputParam, strOutputParam);//透传ISPAI协议请求

    BOOL bRet = FALSE;
    char szSubStatus[20] = { 0 };
    char szTemp[1024] = { 0 };
    int iTemp = 0;
    cJSON* pRootResult = cJSON_Parse(strOutputParam); //解析报文
    if (NULL == pRootResult)
    {
        return;
    }
    do
    {
        cJSON* pDiagnosedDataServerList = cJSON_GetObjectItem(pRootResult, "DiagnosedDataServerList");
        if (pDiagnosedDataServerList == NULL || pDiagnosedDataServerList->type != cJSON_Array)
        {
            break;
        }

        int nList = cJSON_GetArraySize(pDiagnosedDataServerList);
        string strTemp;
        for (int i = 0; i < nList && i < 1; i++) //只提取数组的第一组数据用来界面显示
        {

            cJSON* pServerList = cJSON_GetArrayItem(pDiagnosedDataServerList, i);
            if (pServerList == NULL || pServerList->type != cJSON_Object)
            {
                break;
            }

            if (GetNodeVal_JSON(pServerList, "protocol", szTemp, 1023) == FALSE)
            {
                break;
            }
            strTemp = szTemp;
            if (strTemp == "FTP")
            {
                m_comboProtocolType.SetCurSel(0);
            }
            else if (strTemp == "SFTP")
            {
                m_comboProtocolType.SetCurSel(1);
            }

            cJSON *pEenabled = cJSON_GetObjectItem(pServerList, "enabled");
            if (!pEenabled)
            {
                return;
            }
            m_bEnableFTP = (BOOL)pEenabled->valueint;

            if (GetNodeVal_JSON(pServerList, "addressingFormatType", szTemp, 1023) == FALSE)
            {
                break;
            }
            strTemp = szTemp;
            if (strTemp == "ipaddress")
            {
                if (GetNodeVal_JSON(pServerList, "ipVersion", szTemp, 1023) == FALSE)
                {
                    break;
                }
                strTemp = szTemp;
                if (strTemp == "ipv4")
                {
                    m_comboIPType.SetCurSel(0);
                    if (GetNodeVal_JSON(pServerList, "ipV4Address", szTemp, 1023) == TRUE)
                    {
                        m_strServerIP = szTemp;
                    }
                    
                }
                else if (strTemp == "ipv6")
                {
                    m_comboIPType.SetCurSel(1);
                    if (GetNodeVal_JSON(pServerList, "ipv6Address", szTemp, 1023) == TRUE)
                    {
                        m_strServerIP = szTemp;
                    }
                }
            }
            else if (strTemp == "hostname")
            {
                if (GetNodeVal_JSON(pServerList, "hostName", szTemp, 1023) == TRUE)
                {
                    m_strServerIP = szTemp;
                }
            }

            if (GetNodeVal_JSON(pServerList, "portNo", &iTemp) == TRUE)
            {
                m_iServerPort = iTemp;
            }

            if (GetNodeVal_JSON(pServerList, "userName", szTemp, 1023) == TRUE)
            {
                m_strUserName = szTemp;
            }

            if (GetNodeVal_JSON(pServerList, "password", szTemp, 1023) == TRUE)
            {
                m_strPassword = szTemp;
            }

            cJSON* pDiagnosedDataUpload = cJSON_GetObjectItem(pServerList, "DiagnosedDataUpload");
            if (pDiagnosedDataUpload == NULL || pDiagnosedDataUpload->type != cJSON_Object)
            {
                break;
            }

            if (GetNodeVal_JSON(pDiagnosedDataUpload, "compressionKey", szTemp, 1023) == TRUE)
            {
                m_strCompkey = szTemp;
            }

            if (GetNodeVal_JSON(pDiagnosedDataUpload, "consoleCommand", szTemp, 1023) == TRUE)
            {
                m_strConCommand = szTemp;
            }
            
            if (GetNodeVal_JSON(pDiagnosedDataUpload, "startTime", szTemp, 1023) == TRUE)
            {
                sscanf(szTemp, "%04d-%02d-%02dT%02d:%02d:%02d%s", &(struStartTime.dwYear), &(struStartTime.dwMonth), &(struStartTime.dwDay), &(struStartTime.dwHour), &(struStartTime.dwMinute), &(struStartTime.dwSecond), szTemp);
                m_ctStartDate.SetDate(struStartTime.dwYear, struStartTime.dwMonth, struStartTime.dwDay);
                m_ctStartTime.SetTime(struStartTime.dwHour, struStartTime.dwMinute, struStartTime.dwSecond);
            }

            if (GetNodeVal_JSON(pDiagnosedDataUpload, "endTime", szTemp, 1023) == TRUE)
            {
                sscanf(szTemp, "%04d-%02d-%02dT%02d:%02d:%02d%s", &(struEndTime.dwYear), &(struEndTime.dwMonth), &(struEndTime.dwDay), &(struEndTime.dwHour), &(struEndTime.dwMinute), &(struEndTime.dwSecond), szTemp);
                m_ctEndDate.SetDate(struEndTime.dwYear, struEndTime.dwMonth, struEndTime.dwDay);
                m_ctEndTime.SetTime(struEndTime.dwHour, struEndTime.dwMinute, struEndTime.dwSecond);
            }

            if (GetNodeVal_JSON(pServerList, "dataType", szTemp, 1023) == TRUE)
            {
                m_strDataType = szTemp;
            }
        }

        bRet = TRUE;
    } while (0);


    if (NULL != pRootResult)
    {
        cJSON_Delete(pRootResult);
        pRootResult = NULL;
    }

    char szLan[128] = { 0 };
    if (!bRet)
    {
        g_StringLanType(szLan, "诊断信息服务器获取失败！", "Diagnostic server get failed!");
    }
    else
    {
        g_StringLanType(szLan, "诊断信息服务器获取成功！", "Diagnostic server get sucessed!");
    }
    AfxMessageBox(szLan);
    UpdateData(FALSE);
    return;
}


/** @fn void CDlgFTPLogUpload::ISAPITransparent(const int iMothod, const CString strCommandStr, CString strInputParam, CString& strOutputParam)
 *  @brief ISAPI透传功能
 *  @param (in)   const int iMothod   URL方法
 *  @param (in)   const CString strCommandStr   URL
 *  @param (in)   CString strInputParam   输入参数
 *  @param (in)   CString & strOutputParam   输出参数
 *  @return void
 */
void CDlgFTPLogUpload::ISAPITransparent(const int iMothod, const CString strCommandStr, CString strInputParam, CString& strOutputParam)
{
    // TODO:  在此添加控件通知处理程序代码
    char szLan[128] = { 0 };
    string utf_8;
    NET_DVR_XML_CONFIG_INPUT struInputParam = { 0 };
    struInputParam.dwSize = sizeof(struInputParam);
    
    CString szCommand = _T("");
    if (iMothod == 0)  //GET
    {
        szCommand = "GET";
    }
    else if (iMothod == 1)  //PUT
    {
        szCommand = "PUT";
    }
    else if (iMothod == 2)  //DELETE
    {
        szCommand = "DELETE";
    }
    else if (iMothod == 3)  //POST
    {
        szCommand = "POST";
    }

    szCommand = szCommand + _T(" ") + strCommandStr;
    memset(m_szCommandBuf, 0, sizeof(m_szCommandBuf));
    sprintf(m_szCommandBuf, "%s", szCommand);
    struInputParam.lpRequestUrl = m_szCommandBuf;
    struInputParam.dwRequestUrlLen = strlen(m_szCommandBuf);
    struInputParam.dwRecvTimeOut = 5000;

    char szStatusBuff[1024] = { 0 };
    NET_DVR_XML_CONFIG_OUTPUT struOutputParam = { 0 };
    struOutputParam.dwSize = sizeof(struOutputParam);

    struInputParam.dwInBufferSize = strInputParam.GetLength();
    if (struInputParam.dwInBufferSize != 0)
    {
        utf_8 = GB2UTF(strInputParam.GetBuffer(0));
        struInputParam.lpInBuffer = (void*)utf_8.c_str();
        struInputParam.dwInBufferSize = utf_8.length();
    }

    memset(m_lpOutputXml, 0, MAX_LEN_XML);
    struOutputParam.lpOutBuffer = m_lpOutputXml;
    struOutputParam.dwOutBufferSize = MAX_LEN_XML;
    struOutputParam.lpStatusBuffer = szStatusBuff;
    struOutputParam.dwStatusSize = sizeof(szStatusBuff);

    if (!NET_DVR_STDXMLConfig(m_lUserID, &struInputParam, &struOutputParam))
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_STDXMLConfig");
        string str_gb2312 = UTF2GB(szStatusBuff);
        strOutputParam = str_gb2312.c_str();
        return;
    }

    g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_STDXMLConfig");
    string str_gb2312 = UTF2GB(m_lpOutputXml);
    if (str_gb2312.length() == 0)
    {
        str_gb2312 = UTF2GB(szStatusBuff);
    }
    strOutputParam = str_gb2312.c_str();
    strOutputParam.Replace("\n", "\r\n");

    return;
}

/** @fn void CDlgFTPLogUpload::OnCbnSelchangeComboAddrtype()
 *  @brief 修改IP地址显示类型
 *  @return void
 */
void CDlgFTPLogUpload::OnCbnSelchangeComboAddrtype()
{
    // TODO:  在此添加控件通知处理程序代码
    int iSel = m_comboAddrType.GetCurSel();
    if (iSel == 0) //hostname
    {
        CString m_text = "域名地址";
        SetDlgItemText(IDC_STATIC_SERVER_ADDRESS, m_text);
        GetDlgItem(IDC_STATIC_IP_TYPE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_COMBO_IP_TYPE)->ShowWindow(SW_HIDE); 
    }
    else if (iSel == 1) //ipaddress
    {
        CString m_text = "服务器地址";
        SetDlgItemText(IDC_STATIC_SERVER_ADDRESS, m_text);
        GetDlgItem(IDC_STATIC_IP_TYPE)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_COMBO_IP_TYPE)->ShowWindow(SW_SHOW);
    }
    UpdateData(FALSE);
}
