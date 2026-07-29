// DlgInfoDiffusionXmlCfg.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgInfoDiffusionXmlCfg.h"
#include "DlgInfoGerenal.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgInfoDiffusionXmlCfg dialog


CDlgInfoDiffusionXmlCfg::CDlgInfoDiffusionXmlCfg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInfoDiffusionXmlCfg::IDD, pParent)
    , m_binputFileData(FALSE)
    , m_strFilePath(_T(""))
    , m_dwRecvTimeOut(5000)
    , m_bChanConvert(FALSE)
    , m_bIsFormdata(FALSE)
    , m_lpOutputXml(nullptr)
	, m_bForceEncrypt(FALSE)
{
	//{{AFX_DATA_INIT(CDlgInfoDiffusionXmlCfg)
	m_szCommandStr = _T("");
	m_szInputParam = _T("");
	m_szOutputParam = _T("");
	//}}AFX_DATA_INIT
	m_iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
	m_lUserID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;
	memset(m_szCommandBuf, 0, sizeof(m_szCommandBuf));
	m_lpOutputXml = new char[MAX_LEN_XML];
	memset(m_lpOutputXml, 0, MAX_LEN_XML);
    m_lpOutBin = new char[MAX_LEN_XML];
    memset(m_lpOutBin, 0, MAX_LEN_XML);
    m_lHandle = -1;
    m_strFilePathMult[0] = _T("");
    m_strFilePathMult[1] = _T("");
}


void CDlgInfoDiffusionXmlCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgInfoDiffusionXmlCfg)
	DDX_Control(pDX, IDC_COMBO_OPERATE_TYPE, m_cmbOperateType);
	DDX_Text(pDX, IDC_EDIT_COMMAND, m_szCommandStr);
	DDX_Text(pDX, IDC_EDIT_PARAM_INPUT, m_szInputParam);
	DDX_Text(pDX, IDC_EDIT_PARAM_OUTPUT, m_szOutputParam);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHECK1, m_binputFileData);
	DDX_Text(pDX, IDC_EDT_FILE_PATH, m_strFilePath);
	DDX_Text(pDX, IDC_EDIT_RECV_TIME_OUT, m_dwRecvTimeOut);
	DDV_MinMaxUInt(pDX, m_dwRecvTimeOut, 0, ULONG_MAX);
	DDX_Check(pDX, IDC_CHECK_CHAN_CONVERT, m_bChanConvert);
	DDX_Check(pDX, IDC_CHECK_FORMDATA, m_bIsFormdata);
	DDX_Control(pDX, IDC_COMBO_FILE_NUM, m_cmbFileNum);
	DDX_Check(pDX, IDC_CHECK_FORCE_ENCRYPT, m_bForceEncrypt);
}


BEGIN_MESSAGE_MAP(CDlgInfoDiffusionXmlCfg, CDialog)
	//{{AFX_MSG_MAP(CDlgInfoDiffusionXmlCfg)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	ON_BN_CLICKED(IDC_BTN_GET, OnBtnGet)
	ON_BN_CLICKED(IDC_BTN_SET, OnBtnSet)
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BTN_SELECT_FILE, &CDlgInfoDiffusionXmlCfg::OnBnClickedBtnSelectFile)
    ON_BN_CLICKED(IDC_BUTTON_TEST_MIME, &CDlgInfoDiffusionXmlCfg::OnBnClickedButtonTestMime)
    ON_BN_CLICKED(IDC_BTN_LONG_LINK_OPERATE, &CDlgInfoDiffusionXmlCfg::OnBnClickedBtnLongLinkOperate)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgInfoDiffusionXmlCfg message handlers

void CDlgInfoDiffusionXmlCfg::OnBtnExit() 
{
    if (m_lpOutputXml != nullptr)
    {
        delete[] m_lpOutputXml;
    }
    if (m_lpOutBin != nullptr)
    {
        delete[] m_lpOutBin;
    }
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}

//透传表单格式
void CDlgInfoDiffusionXmlCfg::TransferFormData()
{
    char szLan[128] = { 0 };
    BYTE *m_pPicInfo = NULL;

    //if (!(g_struDeviceInfo[m_iDeviceIndex].bySupport4 & 0x02))  //这个能力不是实时获取的，导致只有第一次能力是对，之后都会从错误，所以注释掉
    //{
    //    //如果设备不支持，在返回不支持
    //    g_StringLanType(szLan, "设备不支持透传表单格式", "Device not support Trafer formdata");
    //    AfxMessageBox(szLan);
    //    return;
    //}
    NET_DVR_XML_CONFIG_INPUT struInputParam = { 0 };
    struInputParam.dwSize = sizeof(struInputParam);
    int nSel = m_cmbOperateType.GetCurSel();
    if (nSel == CB_ERR)
    {
        return;
    }
    CString szCommand = _T("");
    m_cmbOperateType.GetLBText(nSel, szCommand);

    if (0 == strcmp(szCommand, _T("PUT")) || 0 == strcmp(szCommand, _T("POST")))
    {
        int iNumofMime = 0;
        NET_DVR_MIME_UNIT struUnit[2] = { 0 };
        if (m_szInputParam.GetLength() > 0) //如果有输入参数有值
        {
            struUnit[iNumofMime].pContent = m_szInputParam.GetBuffer(0);
            struUnit[iNumofMime].dwContentLen = m_szInputParam.GetLength();

            if (strcmp(m_szInputParam, _T("{")))
            {
                memcpy(struUnit[iNumofMime].szContentType, _T("text/json"), strlen(_T("text/json")));
            }
            else
            {
                memcpy(struUnit[iNumofMime].szContentType, _T("text/xml"), strlen(_T("text/xml")));
            }

            memcpy(struUnit[iNumofMime].szName, _T("test"), strlen(_T("test")));
            memcpy(struUnit[iNumofMime].szFilename, _T("\\test.bat"), strlen(_T("\\test.bat")));

            iNumofMime += 1;
        }
        if (m_strFilePath.GetLength() > 0) //2.如果”文件全路径“有值，则添加图片
        {
            if (m_binputFileData) //2.1如果输出是文件数据，则传文件数据
            {
                FILE *fPicFile = fopen(m_strFilePath, "rb");
                if (NULL == fPicFile)
                {
                    g_StringLanType(szLan, "打开文件失败或无此文件", "Open file failed or no this file");
                    AfxMessageBox(szLan);
                    return;
                }
                fseek(fPicFile, 0, SEEK_END);
                int iFileSize = ftell(fPicFile);
                if (iFileSize == 0)
                {
                    g_StringLanType(szLan, "Pic文件为空", "Pic file is empty");
                    AfxMessageBox(szLan);
                    return;
                }
                fseek(fPicFile, 0, SEEK_SET);
                m_pPicInfo = new BYTE[iFileSize];
                if (fread(m_pPicInfo, 1, iFileSize, fPicFile) != iFileSize) {
                    if (NULL != m_pPicInfo)
                    {
                        delete[]m_pPicInfo;
                        m_pPicInfo = NULL;
                    }
                    g_StringLanType(szLan, "Pic文件读取失败", "Pic file read failed！");
                    AfxMessageBox(szLan);
                    return;
                }
                struUnit[iNumofMime].pContent = (char*)m_pPicInfo;
                struUnit[iNumofMime].dwContentLen = iFileSize;
                struUnit[iNumofMime].bySelfRead = 0;
                memcpy(struUnit[iNumofMime].szContentType, _T("image/jpeg"), strlen(_T("image/jpeg")));
                //memcpy(struUnit[iNumofMime].szContentType, _T("application/octet-stream"), strlen(_T("application/octet-stream")));
                memcpy(struUnit[iNumofMime].szName, _T("updateFile"), strlen(_T("updateFile")));
                memcpy(struUnit[iNumofMime].szFilename, m_strFilePath, m_strFilePath.GetLength());
                fclose(fPicFile);
                iNumofMime += 1;
                
            }
            else //2.2如果是传路径，则不用读取文件
            {
                struUnit[iNumofMime].pContent = NULL;
                struUnit[iNumofMime].dwContentLen = 0;
                struUnit[iNumofMime].bySelfRead = 1;
                memcpy(struUnit[iNumofMime].szContentType, _T("image/jpeg"), strlen(_T("image/jpeg")));
                memcpy(struUnit[iNumofMime].szName, _T("test"), strlen(_T("test")));
                memcpy(struUnit[iNumofMime].szFilename, m_strFilePath, m_strFilePath.GetLength());
                iNumofMime += 1;
            }
        }
        struInputParam.lpInBuffer = (char*)struUnit;
        struInputParam.dwInBufferSize = iNumofMime * sizeof(NET_DVR_MIME_UNIT);
        struInputParam.byNumOfMultiPart = iNumofMime;
    }
    else
    {
        g_StringLanType(szLan, "GET 或者 DELETE 方法不支持透传表单.", "GET or DELETE not support transfer formdata.");
        AfxMessageBox(szLan);
        return;
    }
    szCommand = szCommand + _T(" ") + m_szCommandStr + _T("\r\n");
    memset(m_szCommandBuf, 0, sizeof(m_szCommandBuf));
    sprintf(m_szCommandBuf, "%s", szCommand);
    struInputParam.lpRequestUrl = m_szCommandBuf;
    struInputParam.dwRequestUrlLen = strlen(m_szCommandBuf);
    struInputParam.dwRecvTimeOut = m_dwRecvTimeOut;


    char szStatusBuff[1024] = { 0 };
    NET_DVR_XML_CONFIG_OUTPUT struOutputParam = { 0 };
    struOutputParam.dwSize = sizeof(struOutputParam);
    struOutputParam.lpOutBuffer = m_lpOutputXml;
    struOutputParam.dwOutBufferSize = MAX_LEN_XML;
    struOutputParam.lpStatusBuffer = szStatusBuff;
    struOutputParam.dwStatusSize = sizeof(szStatusBuff);

    if (!NET_DVR_STDXMLConfig(m_lUserID, &struInputParam, &struOutputParam))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_STDXMLConfig");
        string str_gb2312 = UTF2GB(szStatusBuff);
        m_szOutputParam = str_gb2312.c_str();
        UpdateData(FALSE);
        if (NULL != m_pPicInfo)
        {
            delete[]m_pPicInfo;
            m_pPicInfo = NULL;
        }
        return;
    }
    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_STDXMLConfig");

    string str_gb2312 = UTF2GB(m_lpOutputXml);
    m_szOutputParam = str_gb2312.c_str();
    if (NULL != m_pPicInfo)
    {
        delete[]m_pPicInfo;
        m_pPicInfo = NULL;
    }
    UpdateData(FALSE);

}
void CDlgInfoDiffusionXmlCfg::OnBtnGet() 
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    //如果是表单格式，则走表单格式透传，否则就按照普通透传
    if (m_bIsFormdata)
    {
        TransferFormData();
        return;
    }

    BYTE *m_pPicInfo = NULL;
    char szLan[128] = { 0 };
    string utf_8;
    NET_DVR_XML_CONFIG_INPUT struInputParam = {0};
    struInputParam.dwSize = sizeof(struInputParam);
    int nSel = m_cmbOperateType.GetCurSel();
    if (nSel == CB_ERR)
    {
        return;
    }
    CString szCommand = _T("");
    m_cmbOperateType.GetLBText(nSel, szCommand);
    
    szCommand = szCommand + _T(" ") + m_szCommandStr;
    memset(m_szCommandBuf, 0, sizeof(m_szCommandBuf));
    sprintf(m_szCommandBuf, "%s", szCommand);
	struInputParam.byForceEncrpt = m_bForceEncrypt;
    struInputParam.lpRequestUrl = m_szCommandBuf;
    struInputParam.dwRequestUrlLen = strlen(m_szCommandBuf);
    struInputParam.dwRecvTimeOut = m_dwRecvTimeOut;
    
    char szStatusBuff[1024] = {0};
    NET_DVR_XML_CONFIG_OUTPUT struOutputParam = {0};
    struOutputParam.dwSize = sizeof(struOutputParam);

    if (m_binputFileData)
    {
        CFile cPicFile;
        if (!cPicFile.Open(m_strFilePath, CFile::modeRead))
        {
            g_StringLanType(szLan, "打开文件失败或无此文件", "Open file failed or no this file");
            AfxMessageBox(szLan);
        }
        else
        {
            struInputParam.dwInBufferSize = (DWORD)cPicFile.GetLength();
            if (struInputParam.dwInBufferSize == 0)
            {
                g_StringLanType(szLan, "Pic文件为空", "Pic file is empty");
                AfxMessageBox(szLan);
            }

            m_pPicInfo = new BYTE[struInputParam.dwInBufferSize];
            cPicFile.Read(m_pPicInfo, struInputParam.dwInBufferSize);
            struInputParam.lpInBuffer = m_pPicInfo;

            cPicFile.Close();
        }

    }
    else
    {
        struInputParam.dwInBufferSize = m_szInputParam.GetLength();
        if (struInputParam.dwInBufferSize != 0)
        {
            utf_8 = GB2UTF(m_szInputParam.GetBuffer(0));
            struInputParam.lpInBuffer = (void*)utf_8.c_str();
            struInputParam.dwInBufferSize = utf_8.length();
        }

    }

    memset(m_lpOutputXml, 0, MAX_LEN_XML);
    struOutputParam.lpOutBuffer = m_lpOutputXml;
    struOutputParam.dwOutBufferSize = MAX_LEN_XML;
    struOutputParam.lpStatusBuffer = szStatusBuff;
    struOutputParam.dwStatusSize = sizeof(szStatusBuff);
    //启用对xml内容中的通道号节点进行转换，则调用V50接口：NET_DVR_STDXMLConfig_Conv
    if (m_bChanConvert)
    {
        if (!NET_DVR_STDXMLConfig_Conv(m_lUserID, &struInputParam, &struOutputParam))
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_STDXMLConfigV50");
            string str_gb2312 = UTF2GB(szStatusBuff);
            m_szOutputParam = str_gb2312.c_str();
            UpdateData(FALSE);
            if (NULL != m_pPicInfo)
            {
                delete[]m_pPicInfo;
                m_pPicInfo = NULL;
            }
            return;
        }
    }
    else
    {
        if (!NET_DVR_STDXMLConfig(m_lUserID, &struInputParam, &struOutputParam))
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_STDXMLConfig");
            string str_gb2312 = UTF2GB(szStatusBuff);
            m_szOutputParam = str_gb2312.c_str();
            if (struOutputParam.byNumOfMultiPart > 0)
            {
                LPNET_DVR_MIME_DATA lpMimeData = (LPNET_DVR_MIME_DATA)(struOutputParam.lpOutBuffer);

                if (lpMimeData != NULL)
                {
                    //解析第一个MIME结构体数据（json）
                    memcpy(m_lpOutputXml, lpMimeData->lpContent, lpMimeData->dwContentSize);
                    string str_gb2312 = UTF2GB(m_lpOutputXml);
                    m_szOutputParam = str_gb2312.c_str();

                    //循环解析第二个及之后的MIME结构体数据
                    for (int i = 1; i < struOutputParam.byNumOfMultiPart; i++)
                    {
                        lpMimeData += 1;

                        if (lpMimeData != NULL)
                        {
                            CString strContentType = "";
                            if (lpMimeData->byContentType == 1)
                            {
                                strContentType = "json";
                            }
                            else if (lpMimeData->byContentType == 2)
                            {
                                strContentType = "bmp";
                            }
                            //保存图片
                            SavePics(lpMimeData->lpContent, lpMimeData->dwContentSize, lpMimeData->sContentID, strContentType);
                        }
                    }
                }
            }
            UpdateData(FALSE);
            if (NULL != m_pPicInfo)
            {
                delete[]m_pPicInfo;
                m_pPicInfo = NULL;
            }
            return;
        }
    }
    
    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_STDXMLConfig");
    string str_gb2312 = UTF2GB(m_lpOutputXml);
    if (str_gb2312.length() == 0)
    {
        str_gb2312 = UTF2GB(szStatusBuff);
    }
    m_szOutputParam = str_gb2312.c_str();
    m_szOutputParam.Replace("\n", "\r\n");
    
	UpdateData(FALSE);
    if (NULL != m_pPicInfo)
    {
        delete[]m_pPicInfo;
        m_pPicInfo = NULL;
    }
    //仅用于测试结构化后图片接收
    if (struOutputParam.byNumOfMultiPart > 0)
    {
        NET_DVR_MIME_UNIT *struTemp = { 0 };
        struTemp = (NET_DVR_MIME_UNIT*)struOutputParam.lpOutBuffer;
        str_gb2312 += "\r\nContentType:";
        str_gb2312 += struTemp->szContentType;
        str_gb2312 += "\r\nFilename:";
        str_gb2312 += struTemp->szFilename;
        str_gb2312 += "\r\nName:";
        str_gb2312 += struTemp->szName;
        str_gb2312 += "\r\nContentLen:";
        char cLen[32];
        ultoa(struTemp->dwContentLen, cLen, 10);
        str_gb2312 += cLen;
        str_gb2312 += "\r\n";
        if (str_gb2312.length() == 0)
        {
            str_gb2312 = UTF2GB(szStatusBuff);
        }
        m_szOutputParam = str_gb2312.c_str();
        m_szOutputParam.Replace("\n", "\r\n");
        UpdateData(FALSE);

        if (strcmp(struTemp->szContentType, "image/jpeg") != 0)
        {
            return;
        }
        if (struOutputParam.lpDataBuffer != NULL && struTemp->dwContentLen > 0)
        {
            FILE* pFile = NULL;
            fopen_s(&pFile, "C:\\NET_DVR_STDXMLConfig_BinaryTest.jpg", "wb+");
            if (pFile)
            {
                fwrite(struOutputParam.lpDataBuffer, struTemp->dwContentLen, 1, pFile);
                fclose(pFile);
            }
        }
    }
}

void CDlgInfoDiffusionXmlCfg::OnBtnSet() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	NET_DVR_XML_CONFIG_INPUT struInputParam = {0};
	struInputParam.dwSize = sizeof(struInputParam);
	int nSel = m_cmbOperateType.GetCurSel();
	if (nSel == CB_ERR)
	{
		return;
	}
	CString szCommand = _T("");
	m_cmbOperateType.GetLBText(nSel, szCommand);
	if (0 == strcmp(szCommand, _T("GET")))
	{
		return;
	}
	else if (strcmp(szCommand, _T("DELETE")))
	{
		struInputParam.lpInBuffer = m_szInputParam.GetBuffer(0);
		struInputParam.dwInBufferSize = m_szInputParam.GetLength();
	}
	szCommand = szCommand + _T(" ") + m_szCommandStr + _T("\r\n");
	memset(m_szCommandBuf, 0, sizeof(m_szCommandBuf));
	sprintf(m_szCommandBuf, "%s", szCommand);
	struInputParam.lpRequestUrl = m_szCommandBuf;
	struInputParam.dwRequestUrlLen = strlen(m_szCommandBuf);
    struInputParam.dwRecvTimeOut = m_dwRecvTimeOut;
	
	char szStatusBuff[1024] = {0};
	NET_DVR_XML_CONFIG_OUTPUT struOutputParam = {0};
	struOutputParam.dwSize = sizeof(struOutputParam);
	struOutputParam.lpStatusBuffer = szStatusBuff;
	struOutputParam.dwStatusSize = sizeof(szStatusBuff);
	
    SYSTEMTIME struTime1 = { 0 };
    SYSTEMTIME struTime2 = { 0 };
    ::GetSystemTime(&struTime1);
	if (!NET_DVR_STDXMLConfig(m_lUserID, &struInputParam, &struOutputParam))
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_STDXMLConfig");
		return;
	}
    ::GetSystemTime(&struTime2);
	g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_STDXMLConfig, timeDiff[%d:%d]", (struTime2.wSecond-struTime1.wSecond),
        (struTime2.wMilliseconds-struTime1.wMilliseconds));
	
	UpdateData(FALSE);
}

BOOL CDlgInfoDiffusionXmlCfg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_cmbOperateType.SetCurSel(0);
    m_cmbFileNum.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgInfoDiffusionXmlCfg::OnBnClickedBtnSelectFile()
{
    UpdateData(TRUE);

    char szLan[1024] = { 0 };
    static char szFilter[] = "All File(*.*)|*.*||";
    if (m_cmbFileNum.GetCurSel() == 0)
    {
        CFileDialog dlg(TRUE, "*.*", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
        if (dlg.DoModal() == IDOK)
        {
            m_strFilePath = dlg.GetPathName();
            SetDlgItemText(IDC_EDT_FILE_PATH, m_strFilePath);
        }
    }
    else if (m_cmbFileNum.GetCurSel() == 1)
    {
        int i = 0;
        CFileDialog dlg(TRUE, "*.*", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT | OFN_EXPLORER, szFilter);
        if (dlg.DoModal() == IDOK)
        {
            CString strPath;
            POSITION pos = dlg.GetStartPosition();
            while (pos != NULL)
            {
                strPath = dlg.GetNextPathName(pos);
                m_strFilePathMult[i] = strPath;
                i++;
            }
            CString str = m_strFilePathMult[0] + ";" + m_strFilePathMult[1];
            SetDlgItemText(IDC_EDT_FILE_PATH, str);
        }
    }
}


void CDlgInfoDiffusionXmlCfg::OnBnClickedButtonTestMime()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);

    NET_DVR_XML_CONFIG_INPUT struInputParam = { 0 };
    struInputParam.dwSize = sizeof(struInputParam);
    int nSel = m_cmbOperateType.GetCurSel();
    if (nSel == CB_ERR)
    {
        return;
    }
    CString szCommand = _T("");
    m_cmbOperateType.GetLBText(nSel, szCommand);
    if (strcmp(szCommand, _T("PUT")) && strcmp(szCommand, _T("POST")))
    {
        return;
    }
    else if (strcmp(szCommand, _T("DELETE")))
    {
        NET_DVR_MIME_UNIT struUnit[2] = { 0 };
        if (m_binputFileData)
        {
            char szLan[128] = { 0 };
            CFile cPicFile;
            if (!cPicFile.Open(m_strFilePath, CFile::modeRead))
            {
                g_StringLanType(szLan, "打开文件失败或无此文件", "Open file failed or no this file");
                AfxMessageBox(szLan);
            }
            else
            {
                cPicFile.Close();
                struUnit[0].pContent = NULL;
                struUnit[0].dwContentLen = 0;
                struUnit[0].bySelfRead = 1;
                //memcpy(struUnit[0].szContentType, _T("image/jpeg"), strlen(_T("application/octet-stream""image/jpeg")));
                memcpy(struUnit[0].szContentType, _T("application/octet-stream"), strlen(_T("application/octet-stream")));
                memcpy(struUnit[0].szName, _T("updateFile"), strlen(_T("updateFile")));
                memcpy(struUnit[0].szFilename, m_strFilePath, m_strFilePath.GetLength());
                struInputParam.lpInBuffer = (void*)struUnit;
                struInputParam.dwInBufferSize = 1 * sizeof(NET_DVR_MIME_UNIT);
                struInputParam.byNumOfMultiPart = 1;
            }
        }
        else
        {
            struUnit[0].pContent = m_szInputParam.GetBuffer(0);
            struUnit[0].dwContentLen = m_szInputParam.GetLength();

            if (strcmp(szCommand, _T("{")))
            {
                memcpy(struUnit[0].szContentType, _T("text/json"), strlen(_T("text/json")));
            }
            else
            {
                memcpy(struUnit[0].szContentType, _T("text/xml"), strlen(_T("text/xml")));
            }

            memcpy(struUnit[0].szName, _T("test"), strlen(_T("test")));
            memcpy(struUnit[0].szFilename, _T("\\test.bat"), strlen(_T("\\test.bat")));


            struUnit[1].pContent = NULL;
            struUnit[1].dwContentLen = 0;
            struUnit[1].bySelfRead = 1;
            memcpy(struUnit[1].szContentType, _T("image/jpeg"), strlen(_T("image/jpeg")));
            memcpy(struUnit[1].szName, _T("test"), strlen(_T("test")));
            memcpy(struUnit[1].szFilename, m_strFilePath, m_strFilePath.GetLength());
            struInputParam.lpInBuffer = (void*)struUnit;
            struInputParam.dwInBufferSize = 2 * sizeof(NET_DVR_MIME_UNIT);
            struInputParam.byNumOfMultiPart = 2;
        }


        //struUnit[2].pContent = _T("a@@#$%^&*()_.............1234...^&*()");
        //struUnit[2].dwContentLen = strlen(_T("a@@#$%^&*()_.............1234...^&*()"));
        //memcpy(struUnit[2].szContentType, _T("image/jpeg"), strlen(_T("image/jpeg")));
        //memcpy(struUnit[2].szName, _T("test2"), strlen(_T("test2")));
        //memcpy(struUnit[2].szFilename, _T("C:\\test\\test2.jpg"), strlen(_T("C:\\test\\test2.jpg")));
    }
    szCommand = szCommand + _T(" ") + m_szCommandStr + _T("\r\n");
    memset(m_szCommandBuf, 0, sizeof(m_szCommandBuf));
    sprintf(m_szCommandBuf, "%s", szCommand);
    struInputParam.lpRequestUrl = m_szCommandBuf;
    struInputParam.dwRequestUrlLen = strlen(m_szCommandBuf);
    struInputParam.dwRecvTimeOut = m_dwRecvTimeOut;


    char szStatusBuff[1024] = { 0 };
    NET_DVR_XML_CONFIG_OUTPUT struOutputParam = { 0 };
    struOutputParam.dwSize = sizeof(struOutputParam);
    struOutputParam.lpOutBuffer = m_lpOutputXml;
    struOutputParam.dwOutBufferSize = MAX_LEN_XML;
    struOutputParam.lpStatusBuffer = szStatusBuff;
    struOutputParam.dwStatusSize = sizeof(szStatusBuff);

    if (!NET_DVR_STDXMLConfig(m_lUserID, &struInputParam, &struOutputParam))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_STDXMLConfig");
        string str_gb2312 = UTF2GB(szStatusBuff);
        m_szOutputParam = str_gb2312.c_str();
        UpdateData(FALSE);
        return;
    }
    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_STDXMLConfig");

    string str_gb2312 = UTF2GB(m_lpOutputXml);
    m_szOutputParam = str_gb2312.c_str();

    UpdateData(FALSE);
}


void CDlgInfoDiffusionXmlCfg::OnBnClickedBtnLongLinkOperate()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);

    int nSel = m_cmbOperateType.GetCurSel();
    if (nSel == CB_ERR)
    {
        return;
    }
    CString szCommand = _T("");
    m_cmbOperateType.GetLBText(nSel, szCommand);
    szCommand = szCommand + _T(" ") + m_szCommandStr;
    memset(m_szCommandBuf, 0, sizeof(m_szCommandBuf));
    sprintf_s(m_szCommandBuf, "%s", szCommand);

    if (szCommand == "POST /ISAPI/AccessControl/IrisInfo/record?format=json" ||
        szCommand == "PUT /ISAPI/AccessControl/IrisInfo/modify?format=json" ||
        szCommand == "PUT /ISAPI/AccessControl/IrisInfo/setup?format=json")
    {
        m_lHandle = NET_DVR_StartRemoteConfig(m_lUserID, NET_DVR_SET_FORM_DATA, m_szCommandBuf, strlen(m_szCommandBuf), NULL, NULL);

        if (m_lHandle >= 0)
        {
            SendFormData();
        }
        else
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_SET_FORM_DATA");
            return;
        }
    }
    else if (szCommand == "POST /ISAPI/AccessControl/IrisInfo/singleSearch?format=json" ||
        szCommand == "GET /ISAPI/AccessControl/captureIrisData/progress?format=json")
    {
        m_lHandle = NET_DVR_StartRemoteConfig(m_lUserID, NET_DVR_GET_FORM_DATA, m_szCommandBuf, strlen(m_szCommandBuf), NULL, NULL);
        if (m_lHandle >= 0)
        {
            GetFormData();
        }
        else
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GET_FORM_DATA");
            return;
        }
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "url not support");
    }
    UpdateData(FALSE);
}

void CDlgInfoDiffusionXmlCfg::SendFormData()
{
    char szLan[128] = { 0 };
    char szTemp[16] = { 0 };
    BYTE *m_pPicInfo[2] = { NULL };
    char szStatusBuff[5000] = { 0 };
    DWORD dwOutLen = 0;
    NET_DVR_FORM_DATA_CFG struFormData = { 0 };
    struFormData.dwSize = sizeof(struFormData);
    NET_DVR_MIME_DATA struMimeData[3] = { 0 };
    struMimeData[0].byContentType = 1;
    struMimeData[0].lpContent = m_szInputParam.GetBuffer(0);
    struMimeData[0].dwContentSize = m_szInputParam.GetLength();

    if (m_cmbFileNum.GetCurSel() == 0)
    {
        if (m_strFilePath.GetLength() > 0)
        {
            FILE *fPicFile = fopen(m_strFilePath, "rb");
            if (NULL == fPicFile)
            {
                g_StringLanType(szLan, "打开文件失败或无此文件", "Open file failed or no this file");
                AfxMessageBox(szLan);
                return;
            }
            fseek(fPicFile, 0, SEEK_END);
            int iFileSize = ftell(fPicFile);
            if (iFileSize == 0)
            {
                g_StringLanType(szLan, "Pic文件为空", "Pic file is empty");
                AfxMessageBox(szLan);
                return;
            }
            fseek(fPicFile, 0, SEEK_SET);
            m_pPicInfo[1] = new BYTE[iFileSize];
            if (fread(m_pPicInfo[1], 1, iFileSize, fPicFile) != iFileSize) {
                if (NULL != m_pPicInfo[1])
                {
                    delete[]m_pPicInfo[1];
                    m_pPicInfo[1] = NULL;
                }
                g_StringLanType(szLan, "Pic文件读取失败", "Pic file read failed！");
                AfxMessageBox(szLan);
                return;
            }
            struMimeData[1].byContentType = 2;
            struMimeData[1].lpContent = (char*)m_pPicInfo[1];
            struMimeData[1].dwContentSize = iFileSize;
            memcpy(struMimeData[1].sContentID, "irisPic1", strlen("irisPic1"));
        }
        struFormData.byNumOfMultiPart = 2;
        struFormData.lpBuffer = (char*)struMimeData;
        struFormData.dwBufferSize = 2 * sizeof(NET_DVR_MIME_DATA);
    }
    else if (m_cmbFileNum.GetCurSel() == 1)
    {
        for (int i = 0; i < 2; i++)
        {
            if (m_strFilePathMult[i].GetLength() > 0)
            {
                FILE *fPicFile = fopen(m_strFilePathMult[i], "rb");
                if (NULL == fPicFile)
                {
                    g_StringLanType(szLan, "打开文件失败或无此文件", "Open file failed or no this file");
                    AfxMessageBox(szLan);
                    return;
                }
                fseek(fPicFile, 0, SEEK_END);
                int iFileSize = ftell(fPicFile);
                if (iFileSize == 0)
                {
                    g_StringLanType(szLan, "Pic文件为空", "Pic file is empty");
                    AfxMessageBox(szLan);
                    return;
                }
                fseek(fPicFile, 0, SEEK_SET);
                m_pPicInfo[i] = new BYTE[iFileSize];
                if (fread(m_pPicInfo[i], 1, iFileSize, fPicFile) != iFileSize) {
                    if (NULL != m_pPicInfo[i])
                    {
                        delete[]m_pPicInfo[i];
                        m_pPicInfo[i] = NULL;
                    }
                    g_StringLanType(szLan, "Pic文件读取失败", "Pic file read failed！");
                    AfxMessageBox(szLan);
                    return;
                }
                struMimeData[i + 1].byContentType = 2;
                struMimeData[i + 1].lpContent = (char*)m_pPicInfo[i];
                struMimeData[i + 1].dwContentSize = iFileSize;
                sprintf_s(szTemp, "irisPic%d", (i + 1));
                memcpy(struMimeData[i + 1].sContentID, szTemp, strlen(szTemp));
            }
        }
        struFormData.byNumOfMultiPart = 3;
        struFormData.lpBuffer = (char*)struMimeData;
        struFormData.dwBufferSize = 3 * sizeof(NET_DVR_MIME_DATA);
    }

    int iStatus = NET_DVR_SendWithRecvRemoteConfig(m_lHandle, &struFormData, sizeof(struFormData), szStatusBuff, sizeof(szStatusBuff), &dwOutLen);
    if (iStatus < 0)
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "SendFormData, NET_DVR_SendWithRecvRemoteConfig failed");
        string str_gb2312 = UTF2GB(szStatusBuff);
        m_szOutputParam = str_gb2312.c_str();
        UpdateData(FALSE);
        for (int i = 0; i < 2; i++)
        {
            if (NULL != m_pPicInfo[i])
            {
                delete[] m_pPicInfo[i];
                m_pPicInfo[i] = NULL;
            }
        }
        return;
    }
    else
    {
        switch (iStatus)
        {
        case NET_SDK_CONFIG_STATUS_SUCCESS:
        {
            string str_gb2312 = UTF2GB(szStatusBuff);
            m_szOutputParam = str_gb2312.c_str();
        }
            break;
        default:
            break;
        }
    }
    for (int i = 0; i < 2; i++)
    {
        if (NULL != m_pPicInfo[i])
        {
            delete[] m_pPicInfo[i];
            m_pPicInfo[i] = NULL;
        }
    }
    UpdateData(FALSE);
}

void CDlgInfoDiffusionXmlCfg::GetFormData()
{
    char *szBuf = new char[2048];
    memset(szBuf, 0, 2048);
    DWORD dwOutLen = 0;
    int iStatus = NET_DVR_SendWithRecvRemoteConfig(m_lHandle, m_szInputParam.GetBuffer(0), m_szInputParam.GetLength(), szBuf, 2048, &dwOutLen);
    if (iStatus < 0)
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "GetFormData, NET_DVR_SendWithRecvRemoteConfig failed");
        delete[] szBuf;
        szBuf = NULL;
        return;
    }
    else
    {
        LPNET_DVR_FORM_DATA_CFG lpFormData = (LPNET_DVR_FORM_DATA_CFG)szBuf;
        LPNET_DVR_MIME_DATA lpMimeData = (LPNET_DVR_MIME_DATA)(lpFormData->lpBuffer);
        switch (iStatus)
        {
        case NET_SDK_CONFIG_STATUS_SUCCESS:
        {
            if (lpFormData->byNumOfMultiPart == 1)
            {
                memcpy(m_lpOutputXml, lpMimeData->lpContent, lpMimeData->dwContentSize);
                string str_gb2312 = UTF2GB(m_lpOutputXml);
                m_szOutputParam = str_gb2312.c_str();
            }

            else
            {
                if (lpMimeData != NULL)
                {
                    //解析第一个MIME结构体数据（json）
                    memcpy(m_lpOutputXml, lpMimeData->lpContent, lpMimeData->dwContentSize);
                    string str_gb2312 = UTF2GB(m_lpOutputXml);
                    m_szOutputParam = str_gb2312.c_str();
                    m_szOutputParam.Replace("\n", "\r\n");

                    //循环解析第二个及之后的MIME结构体数据
                    for (int i = 1; i < lpFormData->byNumOfMultiPart; i++)
                    {
                        lpMimeData += 1;

                        if (lpMimeData != NULL)
                        {
                            CString strContentType = "";
                            if (lpMimeData->byContentType == 1)
                            {
                                strContentType = "json";
                            }
                            else if (lpMimeData->byContentType == 2)
                            {
                                strContentType = "bmp";
                            }
                            //保存图片
                            SavePics(lpMimeData->lpContent, lpMimeData->dwContentSize, lpMimeData->sContentID, strContentType);
                        }
                    }
                }
                g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_GET_FORM_DATA SUCC");
            }

//             Sleep(10);
//             memset(szBuf, 0, 512);
//             int iStatus = NET_DVR_SendWithRecvRemoteConfig(m_lHandle, m_szInputParam.GetBuffer(0), m_szInputParam.GetLength(), szBuf, 512, &dwOutLen);
//             if (iStatus > 0)
//             {
//                 switch (iStatus)
//                 {
//                 case NET_SDK_CONFIG_STATUS_SUCCESS:
//                 {
//                     if (lpFormData->byNumOfMultiPart == 1)
//                     {
//                         memcpy(m_lpOutputXml, lpMimeData->lpContent, lpMimeData->dwContentSize);
//                         string str_gb2312 = UTF2GB(m_lpOutputXml);
//                         m_szOutputParam = str_gb2312.c_str();
//                     }
//                 }
//                     break;
//                 case NET_SDK_CONFIG_STATUS_NEEDWAIT:
//                     break;
//                 case NET_SDK_CONFIG_STATUS_FINISH:
//                     NET_DVR_StopRemoteConfig(m_lHandle);
//                     break;
//                 case NET_SDK_CONFIG_STATUS_FAILED:
//                     break;
//                 case NET_SDK_CONFIG_STATUS_EXCEPTION:
//                     NET_DVR_StopRemoteConfig(m_lHandle);
//                     break;
//                 default:
//                     break;
//                 }
//             }
        }
            break;
        case NET_SDK_CONFIG_STATUS_NEEDWAIT:
            break;
        case NET_SDK_CONFIG_STATUS_FINISH:
            if (!NET_DVR_StopRemoteConfig(m_lHandle))
            {
                g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_StopRemoteConfig failed[%d]", NET_DVR_GetLastError());
            }
            break;
        case NET_SDK_CONFIG_STATUS_FAILED:
            break;
        case NET_SDK_CONFIG_STATUS_EXCEPTION:
            if (!NET_DVR_StopRemoteConfig(m_lHandle))
            {
                g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_StopRemoteConfig failed[%d]", NET_DVR_GetLastError());
            }
            break;
        default:
            break;
        }

        if (szBuf != NULL)
        {
            delete[] szBuf;
            szBuf = NULL;
        }

    }
}

void CDlgInfoDiffusionXmlCfg::SavePics(void* lpInBuf, DWORD dwInBufSize, char* szID, CString strType)
{
    char cFilename[256] = { 0 };
    HANDLE hFile;
    DWORD dwReturn;
    sprintf(cFilename, "%s\\%s", g_struLocalParam.chPictureSavePath, g_struDeviceInfo[m_iDeviceIndex].chDeviceIPInFileName);

    if (GetFileAttributes(cFilename) != FILE_ATTRIBUTE_DIRECTORY)
    {
        CreateDirectory(cFilename, NULL);
    }

    SYSTEMTIME t;
    GetLocalTime(&t);
    char chTime[128] = { 0 };
    sprintf(chTime, "%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d%3.3d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
    sprintf(cFilename, "%s\\%s[%s].%s", cFilename, szID, chTime, strType);

    hFile = CreateFile(cFilename, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        return;
    }
    WriteFile(hFile, lpInBuf, dwInBufSize, &dwReturn, NULL);
    CloseHandle(hFile);
    hFile = NULL;
}
