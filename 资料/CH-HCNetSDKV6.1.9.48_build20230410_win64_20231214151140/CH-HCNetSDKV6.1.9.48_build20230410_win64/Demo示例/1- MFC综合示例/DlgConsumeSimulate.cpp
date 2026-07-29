// DlgConsumeSimulate.cpp : 实现文件
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "DlgConsumeSimulate.h"
#include "afxdialogex.h"


// CDlgConsumeSimulate 对话框

IMPLEMENT_DYNAMIC(CDlgConsumeSimulate, CDialogEx)

CDlgConsumeSimulate::CDlgConsumeSimulate(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgConsumeSimulate::IDD, pParent)
    , m_strEmployeeNo(_T(""))
    , m_strCardNo(_T(""))
    , m_iCurBalance(0)
    , m_iCurTimes(0)
{

}

CDlgConsumeSimulate::~CDlgConsumeSimulate()
{
}



void CDlgConsumeSimulate::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_EMPLOYEENO, m_strEmployeeNo);
    DDX_Text(pDX, IDC_EDIT_CARDNO, m_strCardNo);
    DDX_Text(pDX, IDC_EDIT_CUR_BALANCE, m_iCurBalance);
    DDX_Text(pDX, IDC_EDIT_CUR_TIMES, m_iCurTimes);
    DDX_Control(pDX, IDC_LIST_PERSON, m_personList);
    DDX_Control(pDX, IDC_LIST_CONSUME_RESULT, m_consumeResultList);
    DDX_Control(pDX, IDC_LIST_TRANS_RECORD, m_transRecordList);
}


BEGIN_MESSAGE_MAP(CDlgConsumeSimulate, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_ADD_PERSON, &CDlgConsumeSimulate::OnBnClickedBtnAddPerson)
    ON_BN_CLICKED(IDC_BTN_MODIFY_PERSON, &CDlgConsumeSimulate::OnBnClickedBtnModifyPerson)
    ON_BN_CLICKED(IDC_BTN_DELETE_PERSON, &CDlgConsumeSimulate::OnBnClickedBtnDeletePerson)
    ON_MESSAGE(WM_PROC_CONSUME_ALARM, &CDlgConsumeSimulate::OnWMProcConsumeAlarm)
    ON_MESSAGE(WM_PROC_TRANS_RECORD_ALARM, &CDlgConsumeSimulate::OnWMProcTransRecordAlarm)
END_MESSAGE_MAP()


/*********************************************************
Function:	OnInitDialog
Desc:		Initialize the dialog
Input:
Output:
Return:
**********************************************************/
BOOL CDlgConsumeSimulate::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_personList.InsertColumn(0, _T("工号"),LVCFMT_LEFT,80);
    m_personList.InsertColumn(1, _T("卡号"), LVCFMT_LEFT, 80);
    m_personList.InsertColumn(2, _T("当前余额"), LVCFMT_LEFT, 80);
    m_personList.InsertColumn(3, _T("当前余次"), LVCFMT_LEFT, 80);

    m_consumeResultList.InsertColumn(0, _T("流水号"), LVCFMT_LEFT, 80);
    m_consumeResultList.InsertColumn(1, _T("工号"), LVCFMT_LEFT, 80);
    m_consumeResultList.InsertColumn(2, _T("卡号"), LVCFMT_LEFT, 80);
    m_consumeResultList.InsertColumn(3, _T("交易类型"), LVCFMT_LEFT, 80);
    m_consumeResultList.InsertColumn(4, _T("消费模式"), LVCFMT_LEFT, 80);
    m_consumeResultList.InsertColumn(5, _T("原余额"), LVCFMT_LEFT, 80);
    m_consumeResultList.InsertColumn(6, _T("当前余额"), LVCFMT_LEFT, 80);
    m_consumeResultList.InsertColumn(7, _T("原余次"), LVCFMT_LEFT, 80);
    m_consumeResultList.InsertColumn(8, _T("当前余次"), LVCFMT_LEFT, 80);
   

    m_transRecordList.InsertColumn(0, _T("流水号"), LVCFMT_LEFT, 80);
    m_transRecordList.InsertColumn(1, _T("上条流水号"), LVCFMT_LEFT, 80);
    m_transRecordList.InsertColumn(2, _T("工号"), LVCFMT_LEFT, 80);
    m_transRecordList.InsertColumn(3, _T("卡号"), LVCFMT_LEFT, 80);
    m_transRecordList.InsertColumn(4, _T("交易类型"), LVCFMT_LEFT, 80);
    m_transRecordList.InsertColumn(5, _T("应付金额"), LVCFMT_LEFT, 80);
    m_transRecordList.InsertColumn(6, _T("实付金额"), LVCFMT_LEFT, 80);
    m_transRecordList.InsertColumn(7, _T("余额"), LVCFMT_LEFT, 80);
    m_transRecordList.InsertColumn(8, _T("已消费次数"), LVCFMT_LEFT, 80);
    m_transRecordList.InsertColumn(9, _T("剩余次数"), LVCFMT_LEFT, 80);

    return TRUE;
}

void CDlgConsumeSimulate::TransRecordJsonToStruct(cJSON *pRoot, TRANSRECORD_STRUCT* pStruTransRecord)
{
    char byStr[32] = { 0 };

    memset(byStr, 0, 32);
    cJSON* pNode = cJSON_GetObjectItem(pRoot, "serialNo");
    if (pNode != NULL && pNode->type == cJSON_String)
    {
        strncpy(byStr, pNode->valuestring, strlen(pNode->valuestring));
        pStruTransRecord->strSerialNo = byStr;
    }

    memset(byStr, 0, 32);
    pNode = cJSON_GetObjectItem(pRoot, "frontSerialNo");
    if (pNode != NULL && pNode->type == cJSON_String)
    {
        strncpy(byStr, pNode->valuestring, strlen(pNode->valuestring));
        pStruTransRecord->strFrontSerialNo = byStr;
    }

    memset(byStr, 0, 32);
    pNode = cJSON_GetObjectItem(pRoot, "employeeNoString");
    if (pNode != NULL && pNode->type == cJSON_String)
    {
        strncpy(byStr, pNode->valuestring, strlen(pNode->valuestring));
        pStruTransRecord->strEmployeeNo = byStr;
    }


    memset(byStr, 0, 32);
    pNode = cJSON_GetObjectItem(pRoot, "cardNo");
    if (pNode != NULL && pNode->type == cJSON_String)
    {
        strncpy(byStr, pNode->valuestring, strlen(pNode->valuestring));
        pStruTransRecord->strCardNo = byStr;
    }

    memset(byStr, 0, 32);
    pNode = cJSON_GetObjectItem(pRoot, "modeType");
    if (pNode != NULL && pNode->type == cJSON_String)
    {
        strncpy(byStr, pNode->valuestring, strlen(pNode->valuestring));
        pStruTransRecord->strModeType = byStr;
    }

    memset(byStr, 0, 32);
    pNode = cJSON_GetObjectItem(pRoot, "type");
    if (pNode != NULL && pNode->type == cJSON_String)
    {
        strncpy(byStr, pNode->valuestring, strlen(pNode->valuestring));
        pStruTransRecord->strType = byStr;
    }


    memset(byStr, 0, 32);
    pNode = cJSON_GetObjectItem(pRoot, "refundSerialNo");
    if (pNode != NULL && pNode->type == cJSON_String)
    {
        strncpy(byStr, pNode->valuestring, strlen(pNode->valuestring));
        pStruTransRecord->strRefundSerialNo = byStr;
    }

    memset(byStr, 0, 32);
    pNode = cJSON_GetObjectItem(pRoot, "verifyMode");
    if (pNode != NULL && pNode->type == cJSON_String)
    {
        strncpy(byStr, pNode->valuestring, strlen(pNode->valuestring));
        pStruTransRecord->strVerifyMode = byStr;
    }

    memset(byStr, 0, 32);
    pNode = cJSON_GetObjectItem(pRoot, "mode");
    if (pNode != NULL && pNode->type == cJSON_String)
    {
        strncpy(byStr, pNode->valuestring, strlen(pNode->valuestring));
        pStruTransRecord->strMode = byStr;
    }


    memset(byStr, 0, 32);
    pNode = cJSON_GetObjectItem(pRoot, "totalPayment");
    if (pNode != NULL && pNode->type == cJSON_String)
    {
        strncpy(byStr, pNode->valuestring, strlen(pNode->valuestring));
        pStruTransRecord->iTotalPayment = atoi(byStr);
    }

    memset(byStr, 0, 32);
    pNode = cJSON_GetObjectItem(pRoot, "actualPayment");
    if (pNode != NULL && pNode->type == cJSON_String)
    {
        strncpy(byStr, pNode->valuestring, strlen(pNode->valuestring));
        pStruTransRecord->iActualPayment = atoi(byStr);
    }

    memset(byStr, 0, 32);
    pNode = cJSON_GetObjectItem(pRoot, "balance");
    if (pNode != NULL && pNode->type == cJSON_String)
    {
        strncpy(byStr, pNode->valuestring, strlen(pNode->valuestring));
        pStruTransRecord->iBalance = atoi(byStr);
    }

    memset(byStr, 0, 32);
    pNode = cJSON_GetObjectItem(pRoot, "times");
    if (pNode != NULL && pNode->type == cJSON_String)
    {
        strncpy(byStr, pNode->valuestring, strlen(pNode->valuestring));
        pStruTransRecord->iTimes = atoi(byStr);
    }

    memset(byStr, 0, 32);
    pNode = cJSON_GetObjectItem(pRoot, "remainingTimes");
    if (pNode != NULL && pNode->type == cJSON_String)
    {
        strncpy(byStr, pNode->valuestring, strlen(pNode->valuestring));
        pStruTransRecord->iRemainingTimes = atoi(byStr);
    }

    memset(byStr, 0, 32);
    pNode = cJSON_GetObjectItem(pRoot, "resourcesContentType");
    if (pNode != NULL && pNode->type == cJSON_String)
    {
        strncpy(byStr, pNode->valuestring, strlen(pNode->valuestring));
        pStruTransRecord->strContentType = byStr;
    }
    memset(byStr, 0, 32);
    pNode = cJSON_GetObjectItem(pRoot, "resourcesContent");
    if (pNode != NULL && pNode->type == cJSON_String)
    {
        strncpy(byStr, pNode->valuestring, strlen(pNode->valuestring));
        pStruTransRecord->strContent = byStr;
    }
    memset(byStr, 0, 32);
    pNode = cJSON_GetObjectItem(pRoot, "picturesNumber");
    if (pNode != NULL && pNode->type == cJSON_String)
    {
        strncpy(byStr, pNode->valuestring, strlen(pNode->valuestring));
        pStruTransRecord->iPicturesNumber = atoi(byStr);
    }
}

void CDlgConsumeSimulate::ConsumeJsonToStruct(cJSON *pRoot, SERIALNO_STRUCT* pStruSerialNo)
{
    char byStr[32] = { 0 };
    memset(byStr, 0, 32);
    cJSON *pNode = cJSON_GetObjectItem(pRoot, "cancel");
    if (pNode != NULL && pNode->type == cJSON_String)
    {
        strncpy(byStr, pNode->valuestring, strlen(pNode->valuestring));
        pStruSerialNo->strCancel = byStr;
    }

    memset(byStr, 0, 32);
    pNode = cJSON_GetObjectItem(pRoot, "serialNo");
    if (pNode != NULL && pNode->type == cJSON_String)
    {
        strncpy(byStr, pNode->valuestring, strlen(pNode->valuestring));
        pStruSerialNo->strSerialNo = byStr;
    }


    memset(byStr, 0, 32);
    pNode = cJSON_GetObjectItem(pRoot, "employeeNoString");
    if (pNode != NULL && pNode->type == cJSON_String)
    {
        strncpy(byStr, pNode->valuestring, strlen(pNode->valuestring));
        pStruSerialNo->strEmployeeNo = byStr;
    }


    memset(byStr, 0, 32);
    pNode = cJSON_GetObjectItem(pRoot, "cardNo");
    if (pNode != NULL && pNode->type == cJSON_String)
    {
        strncpy(byStr, pNode->valuestring, strlen(pNode->valuestring));
        pStruSerialNo->strCardNo = byStr;
    }


    memset(byStr, 0, 32);
    pNode = cJSON_GetObjectItem(pRoot, "type");
    if (pNode != NULL && pNode->type == cJSON_String)
    {
        strncpy(byStr, pNode->valuestring, strlen(pNode->valuestring));
        pStruSerialNo->strType = byStr;
    }


    memset(byStr, 0, 32);
    pNode = cJSON_GetObjectItem(pRoot, "refundSerialNo");
    if (pNode != NULL && pNode->type == cJSON_String)
    {
        strncpy(byStr, pNode->valuestring, strlen(pNode->valuestring));
        pStruSerialNo->strRefundSerialNo = byStr;
    }


    memset(byStr, 0, 32);
    pNode = cJSON_GetObjectItem(pRoot, "mode");
    if (pNode != NULL && pNode->type == cJSON_String)
    {
        strncpy(byStr, pNode->valuestring, strlen(pNode->valuestring));
        pStruSerialNo->strMode = byStr;
    }


    memset(byStr, 0, 32);
    pNode = cJSON_GetObjectItem(pRoot, "totalPayment");
    if (pNode != NULL && pNode->type == cJSON_String)
    {
        strncpy(byStr, pNode->valuestring, strlen(pNode->valuestring));
        pStruSerialNo->iTotalPayment = atoi(byStr);
    }
}

// CDlgConsumeSimulate 消息处理程序
LRESULT CDlgConsumeSimulate::OnWMProcConsumeAlarm(WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);

    char byMinor[32] = { 0 }; //消费事件次类型
    SERIALNO_STRUCT struSerialNo; //消费事件结构体，存储json转换后的数据
    cJSON *pRoot;
    pRoot = cJSON_Parse(g_struConsumeISAPIAlarm.pAlarmData);
    if (pRoot)
    {
        cJSON *pEventNode = cJSON_GetObjectItem(pRoot, "ConsumptionEvent");
        if (!pEventNode)
        {
            

            //消费ISAPI报文转结构体
            ConsumeJsonToStruct(pEventNode, &struSerialNo);
            
            cJSON *pMinorNode = cJSON_GetObjectItem(pEventNode, "minor");
            if (pMinorNode != NULL && pMinorNode->type == cJSON_String)
            {
                strncpy(byMinor, pMinorNode->valuestring, strlen(pMinorNode->valuestring));
            }
        }
    }


    //如果为消费事件预处理
    //比对当前人员列表中是否存在对应人员，如果不存在则忽略
    //如果存在，则使用流水号列表存取流水号以及相关的金额信息，然后下发事件确认协议
    if (strcmp(byMinor, "transactionPreprocessingRequest") == 0)
    {
        cJSON *pTemp = cJSON_CreateObject();
        if (pTemp == NULL)
        {
            return FALSE;
        }
        cJSON_AddStringToObject(pTemp, "serialNo", struSerialNo.strSerialNo.c_str());
        if (m_employeeToCardMap.find(struSerialNo.strEmployeeNo) != m_employeeToCardMap.end() 
            && (m_serialNoListMap.find(struSerialNo.strSerialNo) == m_serialNoListMap.end())) //还要判断流水号列表中是否存在该流水号，由于是消费预处理，正常不应该存在
        {
            if (struSerialNo.strType == "transaction") //交易类型为交易时的业务处理逻辑
            {
                if (struSerialNo.strMode == "amount" || struSerialNo.strMode == "quota")
                {
                    if (m_employeeToCardMap[struSerialNo.strEmployeeNo].iCurBalance >= struSerialNo.iTotalPayment)
                    {
                        struSerialNo.iCurBalance = m_employeeToCardMap[struSerialNo.strEmployeeNo].iCurBalance - struSerialNo.iTotalPayment;
                        cJSON_AddStringToObject(pTemp, "result", "success");
                        cJSON_AddStringToObject(pTemp, "mode", struSerialNo.strMode.c_str());
                        cJSON_AddStringToObject(pTemp, "actualPayment", to_string(struSerialNo.iTotalPayment).c_str());
                        cJSON_AddStringToObject(pTemp, "balance", to_string(m_employeeToCardMap[struSerialNo.strEmployeeNo].iCurBalance).c_str());
                        cJSON_AddStringToObject(pTemp, "employeeNoString", struSerialNo.strEmployeeNo.c_str());
                        cJSON_AddStringToObject(pTemp, "cardNo", struSerialNo.strCardNo.c_str());
                    }
                    else
                    {
                        struSerialNo.iCurBalance = m_employeeToCardMap[struSerialNo.strEmployeeNo].iCurBalance;
                        cJSON_AddStringToObject(pTemp, "result", "balanceNotEnough");                                    
                    }                                
                }
                else if (struSerialNo.strMode == "count")
                {
                    //NET_DVR_STDXMLConfig
                    if (m_employeeToCardMap[struSerialNo.strEmployeeNo].iCurTimes >= 1)  //还有剩余次数就成功
                    {
                        struSerialNo.iCurTimes = m_employeeToCardMap[struSerialNo.strEmployeeNo].iCurTimes - 1;
                        cJSON_AddStringToObject(pTemp, "result", "success");
                        cJSON_AddStringToObject(pTemp, "mode", struSerialNo.strMode.c_str());
                        cJSON_AddNumberToObject(pTemp, "times", 1);
                        cJSON_AddNumberToObject(pTemp, "remainingTimes", m_employeeToCardMap[struSerialNo.strEmployeeNo].iCurTimes);
                        cJSON_AddStringToObject(pTemp, "employeeNoString", struSerialNo.strEmployeeNo.c_str());
                        cJSON_AddStringToObject(pTemp, "cardNo", struSerialNo.strCardNo.c_str());
                    }
                    else
                    {
                        struSerialNo.iCurTimes = m_employeeToCardMap[struSerialNo.strEmployeeNo].iCurTimes;
                        cJSON_AddStringToObject(pTemp, "result", "remainingTimesNotEnough");
                    }
                }
            }
            else if (struSerialNo.strType == "refund") //交易类型为纠错时的业务处理逻辑
            {
                if (m_serialNoListMap.find(struSerialNo.strRefundSerialNo) != m_serialNoListMap.end() && m_serialNoTempListMap.find(struSerialNo.strRefundSerialNo) != m_serialNoTempListMap.end())   //判断已经交易过的流水号列表中是否有纠错流水号
                {
                    if (m_serialNoTempListMap[struSerialNo.strRefundSerialNo].strMode == "amount" || m_serialNoTempListMap[struSerialNo.strRefundSerialNo].strMode == "quota")
                    {//如果纠错的流水号是以金额交易，那么将纠错流水号的消费金额写为0，并还原余额总数
                        m_serialNoTempListMap[struSerialNo.strRefundSerialNo].iCurBalance = m_serialNoTempListMap[struSerialNo.strRefundSerialNo].iCurBalance + m_serialNoTempListMap[struSerialNo.strRefundSerialNo].iTotalPayment;
                        m_serialNoTempListMap[struSerialNo.strRefundSerialNo].iTotalPayment = 0;
                    }
                    else if (m_serialNoTempListMap[struSerialNo.strRefundSerialNo].strMode == "count")
                    {//如果纠错的流水号是以次数交易，那么将纠错流水号的次数还原
                        m_serialNoTempListMap[struSerialNo.strRefundSerialNo].iCurTimes = m_serialNoTempListMap[struSerialNo.strRefundSerialNo].iCurTimes + 1;
                    }
                    cJSON_AddStringToObject(pTemp, "result", "success");
                }
            }

        }
        else  //查无此人逻辑
        {
            cJSON_AddStringToObject(pTemp, "result", "noSuchPerson");
            //NET_DVR_STDXMLConfig  查无此人
                        
        }

        //下发消费事件确认报文
        cJSON *root = cJSON_CreateObject();
        cJSON_AddItemToObject(root, "ConsumptionEventConfirm", pTemp);

        m_serialNoTempListMap[struSerialNo.strSerialNo] = struSerialNo;  //迁移到后面
        char szUrl[256] = { 0 };
        char *pBuf = cJSON_Print(root);
        sprintf(szUrl, "PUT /ISAPI/Consume/consumptionEventConfirm?format=json\r\n");
        NET_DVR_XML_CONFIG_INPUT    struInput = { 0 };
        NET_DVR_XML_CONFIG_OUTPUT   struOuput = { 0 };
        struInput.dwSize = sizeof(struInput);
        struOuput.dwSize = sizeof(struOuput);
        struInput.lpRequestUrl = szUrl;
        struInput.dwRequestUrlLen = strlen(szUrl);
        struInput.lpInBuffer = pBuf;
        struInput.dwInBufferSize = strlen(pBuf);
        if (!NET_DVR_STDXMLConfig(m_lUserID, &struInput, &struOuput))
        {

        }
        return NULL;


    }
    else if (strcmp(byMinor, "transactionConfirmingRequest") == 0)
    {
        //如果为消费事件确认
        //比对当前人员列表中是否存在对应人员，如果不存在则忽略
        //如果存在，则判断当前流水号是否在流水号列表中，如果不在，则报错（因为该情况不可能出现，所以需要报错）
        //如果流水号存在，则进行扣款相关操作
        if (m_employeeToCardMap.find(struSerialNo.strEmployeeNo) != m_employeeToCardMap.end()
            && (m_serialNoTempListMap.find(struSerialNo.strSerialNo) != m_serialNoTempListMap.end())) //还要判断流水号临时列表中是否存在该流水号，正常应该存在
        {
            if (struSerialNo.strCancel == "false")//交易取消
            {
                m_serialNoTempListMap.erase(struSerialNo.strSerialNo);
            }
            else
            {
                //如果是进行交易：那么修改人员列表控件以及m_employeeToCardMap的信息（余额及余次），并将该流水号增加到交易完成的列表m_serialNoListMap中，最后删除m_serialNoTempListMap中的流水号
                //如果是进行纠错：那么修改人员列表控件以及m_employeeToCardMap的信息（余额及余次），最后删除m_serialNoTempListMap中的流水号，删除m_serialNoListMap中的纠错流水号
                SERIALNO_STRUCT* struLastSerialNo = &m_serialNoTempListMap[struSerialNo.strSerialNo];
                m_employeeToCardMap[struLastSerialNo->strEmployeeNo].iCurBalance = struLastSerialNo->iCurBalance;
                m_employeeToCardMap[struLastSerialNo->strEmployeeNo].iCurTimes = struLastSerialNo->iCurTimes;
                int count = m_personList.GetItemCount();
                for (int i = 0; i < count; i++)
                {
                    CString temp = m_personList.GetItemText(i,0);
                    if (temp.GetBuffer() == struLastSerialNo->strEmployeeNo)
                    {
                        m_personList.SetItemText(i, 2, to_string(struLastSerialNo->iCurBalance).c_str());
                        m_personList.SetItemText(i, 3, to_string(struLastSerialNo->iCurTimes).c_str());
                        break;
                    }
                }
                            

                //在消费控件列表显示出已经确认的消费事件
                if (struLastSerialNo->strType == "transaction")
                {
                    int iConsumeResultSize = m_consumeResultList.GetItemCount();
                    m_consumeResultList.InsertItem(iConsumeResultSize, struLastSerialNo->strSerialNo.c_str());
                    m_consumeResultList.SetItemText(iConsumeResultSize, 1, struLastSerialNo->strEmployeeNo.c_str());
                    m_consumeResultList.SetItemText(iConsumeResultSize, 2, struLastSerialNo->strCardNo.c_str());
                    m_consumeResultList.SetItemText(iConsumeResultSize, 3, struLastSerialNo->strType.c_str());
                    m_consumeResultList.SetItemText(iConsumeResultSize, 4, struLastSerialNo->strMode.c_str());
                    m_consumeResultList.SetItemText(iConsumeResultSize, 5, to_string(struLastSerialNo->iCurBalance + struLastSerialNo->iTotalPayment).c_str());
                    m_consumeResultList.SetItemText(iConsumeResultSize, 6, to_string(struLastSerialNo->iCurBalance).c_str());
                    m_consumeResultList.SetItemText(iConsumeResultSize, 7, to_string(struLastSerialNo->iCurTimes + 1).c_str());
                    m_consumeResultList.SetItemText(iConsumeResultSize, 8, to_string(struLastSerialNo->iCurTimes).c_str());
                                
                }
                else if (struLastSerialNo->strType == "refund")
                {
                    int iConsumeResultSize = m_consumeResultList.GetItemCount();
                    m_consumeResultList.InsertItem(iConsumeResultSize, struLastSerialNo->strSerialNo.c_str());
                    m_consumeResultList.SetItemText(iConsumeResultSize, 1, struLastSerialNo->strEmployeeNo.c_str());
                    m_consumeResultList.SetItemText(iConsumeResultSize, 2, struLastSerialNo->strCardNo.c_str());
                    m_consumeResultList.SetItemText(iConsumeResultSize, 3, struLastSerialNo->strType.c_str());
                    m_consumeResultList.SetItemText(iConsumeResultSize, 4, "");
                    m_consumeResultList.SetItemText(iConsumeResultSize, 5, "");
                    m_consumeResultList.SetItemText(iConsumeResultSize, 6, "");
                    m_consumeResultList.SetItemText(iConsumeResultSize, 7, "");
                    m_consumeResultList.SetItemText(iConsumeResultSize, 8, "");
                                
                }
                //在确认的流水线容器中增加当前消费事件
                m_serialNoListMap[struSerialNo.strSerialNo] = *struLastSerialNo;
            }
        }
        else
        {
            //无操作，应该报错
        }
    }
    else
    {
        return NULL;
    }
    
    return NULL;
}

LRESULT CDlgConsumeSimulate::OnWMProcTransRecordAlarm(WPARAM wParam, LPARAM lParam)
{
    //g_struConsumeISAPIAlarm
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);

    TRANSRECORD_STRUCT struTransRecord; //交易记录事件结构体，存储json转换后的数据
    cJSON *pRoot;
    pRoot = cJSON_Parse(g_struConsumeISAPIAlarm.pAlarmData);
    if (pRoot)
    {
        cJSON *pEventNode = cJSON_GetObjectItem(pRoot, "TransactionRecordEvent");
        if (!pEventNode)
        {
            //消费ISAPI报文转结构体
            TransRecordJsonToStruct(pEventNode, &struTransRecord);
        }
    }
    int iCount = m_transRecordList.GetItemCount();
    m_transRecordList.InsertItem(iCount, struTransRecord.strSerialNo.c_str());
    m_transRecordList.SetItemText(iCount, 1, struTransRecord.strFrontSerialNo.c_str());
    m_transRecordList.SetItemText(iCount, 2, struTransRecord.strEmployeeNo.c_str());
    m_transRecordList.SetItemText(iCount, 3, struTransRecord.strCardNo.c_str());
    m_transRecordList.SetItemText(iCount, 4, struTransRecord.strType.c_str());
    m_transRecordList.SetItemText(iCount, 5, to_string(struTransRecord.iTotalPayment).c_str());
    m_transRecordList.SetItemText(iCount, 6, to_string(struTransRecord.iActualPayment).c_str());
    m_transRecordList.SetItemText(iCount, 7, to_string(struTransRecord.iBalance).c_str());
    m_transRecordList.SetItemText(iCount, 8, to_string(struTransRecord.iTimes).c_str());
    m_transRecordList.SetItemText(iCount, 9, to_string(struTransRecord.iRemainingTimes).c_str());
    //如果为交易记录事件
    //回复确认报文，并将记录进行显示
    //下发消费事件确认报文
    cJSON *pTemp = cJSON_CreateObject();
    if (pTemp == NULL)
    {
        return FALSE;
    }
    cJSON_AddStringToObject(pTemp, "serialNo", struTransRecord.strSerialNo.c_str());
    cJSON_AddStringToObject(pTemp, "result", "success");
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "TransactionRecordEventConfirm", pTemp);

    char szUrl[256] = { 0 };
    char *pBuf = cJSON_Print(root);
    sprintf(szUrl, "/ISAPI/Consume/transactionRecordEventConfirm?format=json\r\n");
    NET_DVR_XML_CONFIG_INPUT    struInput = { 0 };
    NET_DVR_XML_CONFIG_OUTPUT   struOuput = { 0 };
    struInput.dwSize = sizeof(struInput);
    struOuput.dwSize = sizeof(struOuput);
    struInput.lpRequestUrl = szUrl;
    struInput.dwRequestUrlLen = strlen(szUrl);
    struInput.lpInBuffer = pBuf;
    struInput.dwInBufferSize = strlen(pBuf);
    if (!NET_DVR_STDXMLConfig(m_lUserID, &struInput, &struOuput))
    {

    }
    return NULL;
}

void CDlgConsumeSimulate::OnBnClickedBtnAddPerson()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    if (m_strEmployeeNo.IsEmpty() || m_strCardNo.IsEmpty())
    {
        return;
    }

    //auto iter = m_employeeToCardMap.find(m_strEmployeeNo.GetBuffer());
    if (m_employeeToCardMap.find(m_strEmployeeNo.GetBuffer()) != m_employeeToCardMap.end() || m_cardToEmployeeMap.find(m_strCardNo.GetBuffer()) != m_cardToEmployeeMap.end())
    {
        return;
    }
    //auto iter1 = m_cardToEmployeeMap.find(m_strCardNo.GetBuffer());
    EMPLOTEENO_STRUCT struEmployeeNo(m_strEmployeeNo.GetBuffer(), m_iCurBalance, m_iCurTimes);
    CARDNO_STRUCT struCardNo(m_strCardNo.GetBuffer(),m_iCurBalance,m_iCurTimes);
    

    m_employeeToCardMap[m_strEmployeeNo.GetBuffer()] = struCardNo;
    m_cardToEmployeeMap[m_strEmployeeNo.GetBuffer()] = struEmployeeNo;

    int size = m_personList.GetItemCount();
    m_personList.InsertItem(size, m_strEmployeeNo);
    m_personList.SetItemText(size, 1, m_strCardNo);
    CString str;
    str.Format(_T("%d"), m_iCurBalance);
    m_personList.SetItemText(size, 2, str);
    str.Format(_T("%d"), m_iCurTimes);
    m_personList.SetItemText(size, 3, str);
    UpdateData(FALSE);
}


void CDlgConsumeSimulate::OnBnClickedBtnModifyPerson()
{
    // TODO:  在此添加控件通知处理程序代码
}


void CDlgConsumeSimulate::OnBnClickedBtnDeletePerson()
{
    // TODO:  在此添加控件通知处理程序代码
}
