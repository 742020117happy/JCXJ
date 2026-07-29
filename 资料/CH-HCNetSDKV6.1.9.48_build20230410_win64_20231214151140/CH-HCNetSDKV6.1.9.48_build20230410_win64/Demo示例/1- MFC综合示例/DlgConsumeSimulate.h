#pragma once
#include "afxcmn.h"
#include <map>
#include "cjson/cJSON.h"

using namespace std;
// CDlgConsumeSimulate 对话框

typedef struct tagCARDNO_STRUCT
{
    string strCardNo;
    int iCurBalance;
    int iCurTimes;
    tagCARDNO_STRUCT()
    {
        strCardNo = "";
        iCurBalance = 0;
        iCurTimes = 0;
    }
    tagCARDNO_STRUCT(string cardNo, int curBalance, int curTimes)
    {
        strCardNo = cardNo;
        iCurBalance = curBalance;
        iCurTimes = curTimes;
    }
}CARDNO_STRUCT, *LPCARDNO_STRUCT; 

typedef struct tagEMPLOTEENO_STRUCT
{
    string strEmployeeNo;
    int iCurBalance;
    int iCurTimes;
    tagEMPLOTEENO_STRUCT()
    {
        strEmployeeNo = "";
        iCurBalance = 0;
        iCurTimes = 0;
    }
    tagEMPLOTEENO_STRUCT(string employeeNo, int curBalance, int curTimes)
    {
        strEmployeeNo = employeeNo;
        iCurBalance = curBalance;
        iCurTimes = curTimes;
    }
}EMPLOTEENO_STRUCT, *LPEMPLOTEENO_STRUCT;

typedef struct tagTRANSRECORD_STRUCT
{
    string strSerialNo;   //必填，integer，交易记录流水号（交易预处理、确认请求流水号保持一致，与交易记录流水号保持一致）
    string strFrontSerialNo;//必填，integer，上一条交易记录流水号
    string strName;//可选，string，姓名（128字节）
    string strEmployeeNo;//可选，string，工号（32字节）（工号和卡号必需至少返回一个，如果两个都返回，上层以工号为准）
    string strCardNo;//可选，string，卡号（32字节）（工号和卡号必需至少返回一个，如果两个都返回，上层以工号为准）
    string strModeType;//可选，string，交易模式类型，current-实时交易，offLine-离线记账交易
    string strType;//可选，string，交易类型（transaction-交易，refund-纠错）
    string strRefundSerialNo;//依赖，integer，需要纠错的流水号（type为refund该字段有效，且必须返回）
    string strVerifyMode;//可选，string，验证方式（card-刷卡，face-刷脸）
    string strMode;//可选，string，消费模式（amount-金额，quota-定额，count-计次）
    int iTotalPayment;//可选，应付金额（单位：分）
    int iActualPayment;//可选，实付金额（单位：分）
    int iBalance;//可选，余额（单位：分）
    int iTimes;//可选，integer，已消费次数
    int iRemainingTimes;//可选，integer，剩余次数
    string strContentType;//依赖，string，资源传输类型，当报文中带有图片时，该节点必须返回：url-url方式传输，binary-二进制方式传输
    string strContent;//依赖，string类型，资源标识ID，当报文中带有图片时，该节点必须返回：当resourcesContentType为binary时，该节点与图片的Content-ID严格对应；当resourcesContentType为url时,该节点填写具体的url
    int iPicturesNumber;//可选，integer，图片数量（后面所带的图片数目，没图片时，该字段为0或不返回）
}TRANSRECORD_STRUCT, *LPTRANSRECORD_STRUCT;

typedef struct tagSERIALNO_STRUCT
{
    string strCancel;
    string strSerialNo;
    string strEmployeeNo;
    string strCardNo;
    string strType;
    string strRefundSerialNo;
    string strMode;
    int iTotalPayment;
    int iCurBalance;
    int iCurTimes;
    tagSERIALNO_STRUCT()
    {
        strCancel = "";
        strSerialNo = "";
        strEmployeeNo = "";
        strCardNo = "";
        strType = "";
        strRefundSerialNo = "";
        strMode = "";
        iTotalPayment = 0;
        iCurBalance = 0;
        iCurTimes = 0;
    }
    
    tagSERIALNO_STRUCT(string cancelType, string SerialNo, string EmployeeNo, string CardNo, string transType, string RefundSerialNo, string mode, int totalPayment)
    {
        strCancel = cancelType;
        strSerialNo = SerialNo;
        strEmployeeNo = EmployeeNo;
        strCardNo = CardNo;
        strType = transType;
        strRefundSerialNo = RefundSerialNo;
        strMode = mode;
        iTotalPayment = totalPayment;
        iCurBalance = 0;
        iCurTimes = 0;
    }
}SERIALNO_STRUCT, *LPSERIALNO_STRUCT;

class CDlgConsumeSimulate : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgConsumeSimulate)

public:
	CDlgConsumeSimulate(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgConsumeSimulate();
    LONG m_lUserID;
    int m_iDeviceIndex;
// 对话框数据
	enum { IDD = IDD_DLG_CONSUME_SIMULATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
    CString m_strEmployeeNo;
    CString m_strCardNo;
    DWORD m_iCurBalance;
    DWORD m_iCurTimes;
    CListCtrl m_personList;
    CListCtrl m_consumeResultList;
    CListCtrl m_transRecordList;
    map<string, CARDNO_STRUCT>     m_employeeToCardMap;  //保存以人为中心的人员信息
    map<string, EMPLOTEENO_STRUCT> m_cardToEmployeeMap;  //保留，暂不使用
    map<string, SERIALNO_STRUCT>   m_serialNoListMap;    //已完成确认的流水号事件信息
    map<string, SERIALNO_STRUCT>   m_serialNoTempListMap;   //完成预处理的流水号事件信息，等待确认事件上报
    LRESULT OnWMProcConsumeAlarm(WPARAM wParam, LPARAM lParam);
    LRESULT OnWMProcTransRecordAlarm(WPARAM wParam, LPARAM lParam);
    afx_msg void OnBnClickedBtnAddPerson();
    afx_msg void OnBnClickedBtnModifyPerson();
    afx_msg void OnBnClickedBtnDeletePerson();
    void ConsumeJsonToStruct(cJSON *pRoot, SERIALNO_STRUCT* pStruSerialNo);
    void TransRecordJsonToStruct(cJSON *pRoot, TRANSRECORD_STRUCT* pStruTransRecord);
};
