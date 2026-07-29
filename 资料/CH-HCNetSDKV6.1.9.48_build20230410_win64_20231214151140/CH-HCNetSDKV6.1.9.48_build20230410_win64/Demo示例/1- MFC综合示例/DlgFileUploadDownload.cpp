// DlgFileUploadDownload.cpp : 实现文件
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "DlgFileUploadDownload.h"
#include "afxdialogex.h"


// CDlgFileUploadDownload 对话框

IMPLEMENT_DYNAMIC(CDlgFileUploadDownload, CDialogEx)

CDlgFileUploadDownload::CDlgFileUploadDownload(CWnd* pParent /*=NULL*/)
: CDialogEx(CDlgFileUploadDownload::IDD, pParent)
, m_strFileUploadPath(_T(""))
, m_strFileParaPath(_T(""))
, m_bUpLoading(FALSE)
, m_pThread(NULL)
, m_pDownThread(NULL)
, m_strLicenseKeyFilePath(_T(""))
, m_strTaskID(_T(""))
, m_strPID(_T(""))
, m_csDowmloadFilePath(_T(""))
, m_iWallNo(-1)
, m_iWindowNo(-1)
, m_iMaterialNo(-1)
{

}

CDlgFileUploadDownload::~CDlgFileUploadDownload()
{
}

void CDlgFileUploadDownload::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_UPLOAD_TYPE, m_comboUploadType);
    DDX_Text(pDX, IDC_EDIT_UPLOAD_PATH, m_strFileUploadPath);
    DDX_Text(pDX, IDC_EDT_FILE_PATH, m_strFileParaPath);
    DDX_Text(pDX, IDC_EDT_LICENSE_FILE_PATH, m_strLicenseKeyFilePath);
    DDX_Text(pDX, IDC_EDIT_TaskID, m_strTaskID);
    DDX_Text(pDX, IDC_EDIT_PID, m_strPID);
    DDX_Control(pDX, IDC_COMBO_DOWNLOAD_TYPE, m_combDownloadType);
    DDX_Text(pDX, IDC_EDIT_DOWNLOAD_PATH, m_csDowmloadFilePath);
    DDX_Text(pDX, IDC_EDIT_WALL_NUM, m_iWallNo);
    DDX_Control(pDX, IDC_COMBO_WINDOW_TYPE, m_combWindowType);
    DDX_Control(pDX, IDC_COMBO_FILE_TYPE, m_combFileType);
    DDX_Text(pDX, IDC_EDIT_WINDOW_NUM, m_iWindowNo);
    DDX_Text(pDX, IDC_EDIT_MATERIAL_NUM, m_iMaterialNo);
}


BEGIN_MESSAGE_MAP(CDlgFileUploadDownload, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_FILE_UPLOAD, &CDlgFileUploadDownload::OnBnClickedBtnFileUpload)
    ON_BN_CLICKED(IDC_BTN_FILE_PATH_UPLOAD, &CDlgFileUploadDownload::OnBnClickedBtnFilePathUpload)
    ON_BN_CLICKED(IDC_BTN_SELECT_FILE, &CDlgFileUploadDownload::OnBnClickedBtnSelectFile)
    ON_WM_CLOSE()
    ON_MESSAGE(WM_UPDATE_STATUS, &CDlgFileUploadDownload::OnUpdateStatus)
    ON_MESSAGE(WM_UPLOAD_FINISH, &CDlgFileUploadDownload::OnUploadFinish)
    ON_BN_CLICKED(IDC_BTN_SELECT_LicenseKey_FILE, &CDlgFileUploadDownload::OnBnClickedBtnSelectLicensekeyFile)
    ON_BN_CLICKED(IDC_BTN_FILE_DOWNLOAD, &CDlgFileUploadDownload::OnBnClickedBtnFileDownload)
    ON_BN_CLICKED(IDC_BTN_FILE_PATH_DOWNLOAD, &CDlgFileUploadDownload::OnBnClickedBtnFilePathDownload)
    ON_MESSAGE(WM_UPDATE_DOWNDLOAD_STATUS, &CDlgFileUploadDownload::OnUpdateDownloadStatus)
    ON_MESSAGE(WM_DOWNLOAD_FINISH, &CDlgFileUploadDownload::OnDownloadFinish)
END_MESSAGE_MAP()


UINT __cdecl GetFileUpLoadThread(LPVOID pParam)
{
    THREAD_PARAM* pThreadParam = (THREAD_PARAM*)pParam;
    CDlgFileUploadDownload *pThis = (CDlgFileUploadDownload*)pThreadParam->pThis;

    DWORD dwState = 0;
    DWORD dwProgress = 0;
    char szLan[256] = { 0 };
    bool bUpLoading = TRUE;

    while (!pThreadParam->bExit)
    {
        dwState = NET_DVR_GetUploadState(pThis->m_lUploadHandle, &dwProgress);
        if (dwState == 1)
        {
            g_StringLanType(szLan, "上传成功", "Upload successfully");
            bUpLoading = FALSE;
            SendMessage(pThis->m_hWnd, WM_UPLOAD_FINISH, (WPARAM)szLan, (LPARAM)&bUpLoading);

            //pThis->GetDlgItem(IDC_BTN_FILE_UPLOAD)->SetWindowText(szLan);
            //pThis->m_bUpLoading = FALSE;
            break;
        }
        else if (dwState == 2)
        {
            g_StringLanType(szLan, "正在上传,已上传:", "Is uploading,progress:");
            sprintf(szLan, "%s%d", szLan, dwProgress);
            bUpLoading = TRUE;
            SendMessage(pThis->m_hWnd, WM_UPDATE_STATUS, (WPARAM)szLan, (LPARAM)&bUpLoading);
        }
        else if (dwState == 3)
        {
            g_StringLanType(szLan, "上传失败", "Upload failed");
            bUpLoading = FALSE;
            SendMessage(pThis->m_hWnd, WM_UPDATE_STATUS, (WPARAM)szLan, (LPARAM)&bUpLoading);
            break;
        }
        else if (dwState == 4)
        {
            if (dwProgress == 100)
            {
                g_StringLanType(szLan, "上传成功", "Upload successfully");
                bUpLoading = FALSE;
                SendMessage(pThis->m_hWnd, WM_UPDATE_STATUS, (WPARAM)szLan, (LPARAM)&bUpLoading);
                break;
            }
            else
            {
                g_StringLanType(szLan, "网络断开，状态未知", "Network disconnect, status unknown");
                pThis->GetDlgItem(IDC_BTN_FILE_UPLOAD)->SetWindowText(szLan);
                break;
            }
        }
        if (dwState != 2 && dwState != 5)
        {
            NET_DVR_UploadClose(pThis->m_lUploadHandle);
            g_StringLanType(szLan, "上传", "UpLoad");
            bUpLoading = FALSE;
            SendMessage(pThis->m_hWnd, WM_UPDATE_STATUS, (WPARAM)szLan, (LPARAM)&bUpLoading);
            break;
        }
    }
    return 0;
}

UINT __cdecl GetFileDownLoadThread(LPVOID pParam)
{
    THREAD_PARAM* pThreadParam = (THREAD_PARAM*)pParam;
    CDlgFileUploadDownload *pThis = (CDlgFileUploadDownload*)pThreadParam->pThis;

    DWORD dwState = 0;
    DWORD dwProgress = 0;
    char szLan[256] = { 0 };
    bool bDownLoading = TRUE;

    while (!pThreadParam->bExit)
    {
        dwState = NET_DVR_GetDownloadState(pThis->m_lDownloadHandle, &dwProgress);
        if (dwState == 1)
        {
            g_StringLanType(szLan, "下载成功", "Download successfully");
            bDownLoading = FALSE;
            SendMessage(pThis->m_hWnd, WM_DOWNLOAD_FINISH, (WPARAM)szLan, (LPARAM)&bDownLoading);
            break;
        }
        else if (dwState == 2)
        {
            g_StringLanType(szLan, "正在下载,已下载:", "Is downloading,progress:");
            sprintf(szLan, "%s%d", szLan, dwProgress);
            bDownLoading = TRUE;
            SendMessage(pThis->m_hWnd, WM_UPDATE_DOWNDLOAD_STATUS, (WPARAM)szLan, (LPARAM)&bDownLoading);
        }
        else if (dwState == 3)
        {
            g_StringLanType(szLan, "下载失败", "Download failed");
            bDownLoading = FALSE;
            SendMessage(pThis->m_hWnd, WM_UPDATE_DOWNDLOAD_STATUS, (WPARAM)szLan, (LPARAM)&bDownLoading);
            break;
        }
        else if (dwState == 4)
        {
            if (dwProgress == 100)
            {
                g_StringLanType(szLan, "下载成功", "Download successfully");
                bDownLoading = FALSE;
                SendMessage(pThis->m_hWnd, WM_UPDATE_DOWNDLOAD_STATUS, (WPARAM)szLan, (LPARAM)&bDownLoading);
                break;
            }
            else
            {
                g_StringLanType(szLan, "网络断开，状态未知", "Network disconnect, status unknown");
                pThis->GetDlgItem(IDC_BTN_FILE_DOWNLOAD)->SetWindowText(szLan);
                break;
            }
        }
        if (dwState != 2 && dwState != 5)
        {
            NET_DVR_StopDownload(pThis->m_lDownloadHandle);
            g_StringLanType(szLan, "下载", "DonwLoad");
            bDownLoading = FALSE;
            SendMessage(pThis->m_hWnd, IDC_BTN_FILE_DOWNLOAD, (WPARAM)szLan, (LPARAM)&bDownLoading);
            break;
        }
    }
    return 0;
}

void CDlgFileUploadDownload::OnBnClickedBtnFileUpload()
{
    // TODO:  在此添加控件通知处理程序代码
    char szLan[128] = { 0 };
    NET_DVR_AI_ALGORITHM_MODEL struAIAlgorithmModel = { 0 };
    NET_DVR_AI_PICTUR_UPLOAD struAIPicture = { 0 };
    NET_DVR_RATIOSTITCHING_PARAM struratioStitching = { 0 };
    NET_DVR_VIDEOWALL_MATERIAL_COND struVideowallMaterial = { 0 };
    char *m_pPicInfo = NULL;
    if (m_bUpLoading == FALSE)
    {
        UpdateData(TRUE);
        char szFileName[MAX_PATH];
        strcpy(szFileName, m_strFileUploadPath);
        CFile cFile;
        if (!cFile.Open(szFileName, CFile::modeRead))
        {
            g_StringLanType(szLan, "打开文件失败或无此文件", "Open file failed or no this file");
            AfxMessageBox(szLan);
            return;
        }
        DWORD dwFileSize = (DWORD)cFile.GetLength();
        if (dwFileSize == 0)
        {
            g_StringLanType(szLan, "配置文件为空", "Configure file is empty");
            AfxMessageBox(szLan);
        }
        cFile.Close();

        NET_SDK_UPLOAD_TYPE fileType;
        CString csUploadType;
        switch (m_comboUploadType.GetCurSel())
        {
        case 0:
            fileType = UPLOAD_AI_ALGORITHM_MODEL;
            csUploadType = "UPLOAD_AI_ALGORITHM_MODEL";
            struAIAlgorithmModel.dwSize = sizeof(struAIAlgorithmModel);
            if (m_strFileParaPath != "")
            {
                CFile cPicFile;
                if (!cPicFile.Open(m_strFileParaPath, CFile::modeRead))
                {
                    g_StringLanType(szLan, "打开文件失败或无此文件", "Open file failed or no this file");
                    AfxMessageBox(szLan);
                }
                else
                {
                    struAIAlgorithmModel.dwDescribeLength = (DWORD)cPicFile.GetLength();
                    if (struAIAlgorithmModel.dwDescribeLength == 0)
                    {
                        g_StringLanType(szLan, "AIAlgorithmModel文件为空", "AI Algorithm Model is empty");
                        AfxMessageBox(szLan);
                    }

                    m_pPicInfo = new char[struAIAlgorithmModel.dwDescribeLength];
                    cPicFile.Read(m_pPicInfo, struAIAlgorithmModel.dwDescribeLength);
                    struAIAlgorithmModel.pDescribeBuffer = m_pPicInfo;
                    cPicFile.Close();
                }
            }
            if (m_strLicenseKeyFilePath!="")
            {
                CFile cPicFile;
                if (!cPicFile.Open(m_strLicenseKeyFilePath, CFile::modeRead))
                {
                    g_StringLanType(szLan, "打开文件失败或无此文件", "Open file failed or no this file");
                    AfxMessageBox(szLan);
                }
                else
                {
                    struAIAlgorithmModel.dwLicenseKeyLength = (DWORD)cPicFile.GetLength();
                    if (struAIAlgorithmModel.dwLicenseKeyLength == 0)
                    {
                        g_StringLanType(szLan, "LicenseKey文件为空", "LicenseKey is empty");
                        AfxMessageBox(szLan);
                    }

                    m_pPicInfo = new char[struAIAlgorithmModel.dwLicenseKeyLength];
                    cPicFile.Read(m_pPicInfo, struAIAlgorithmModel.dwLicenseKeyLength);
                    struAIAlgorithmModel.pLicenseKeyBuffer = m_pPicInfo;
                    cPicFile.Close();
                }
            }
            break;
        case 1:
            fileType = UPLOAD_AI_PICTURE;
            csUploadType = "UPLOAD_AI_PICTURE";
            struAIPicture.dwSize = sizeof(NET_DVR_AI_PICTUR_UPLOAD);
            memcpy(&struAIPicture.szTaskID, m_strTaskID, 64);
            memcpy(&struAIPicture.szPID, m_strPID, 64);
            break;
        case 2:
            fileType = UPLOAD_RATIOSTITCHING_FILE;
            csUploadType = "UPLOAD_RATIOSTITCHING_FILE";
            struratioStitching.dwSize = sizeof(NET_DVR_RATIOSTITCHING_PARAM);
            if (m_strFileUploadPath != "")
            {
                CFile cPicFile;
                if (!cPicFile.Open(m_strFileUploadPath, CFile::modeRead))
                {
                    g_StringLanType(szLan, "打开文件失败或无此文件", "Open file failed or no this file");
                    AfxMessageBox(szLan);
                }
                else
                {
                    struratioStitching.dwFileLen = (DWORD)cPicFile.GetLength();
                    if (struratioStitching.dwFileLen == 0)
                    {
                        g_StringLanType(szLan, "ratioStitching文件为空", "ratioStitching is empty");
                        AfxMessageBox(szLan);
                    }
                    struratioStitching.byChannel = m_lChannel;
                    cPicFile.Close();
                }
            }
            break;
        case 3:
            fileType = UPLOAD_VIDEOWALL_MATERIAL_FILE;
            csUploadType = "UPLOAD_VIDEOWALL_MATERIAL_FILE";
            struVideowallMaterial.dwSize = sizeof(NET_DVR_VIDEOWALL_MATERIAL_COND);
            if (m_strFileUploadPath != "")
            {
                CFile cPicFile;
                if (!cPicFile.Open(m_strFileUploadPath, CFile::modeRead))
                {
                    g_StringLanType(szLan, "打开文件失败或无此文件", "Open file failed or no this file");
                    AfxMessageBox(szLan);
                }
                else
                {
                    struVideowallMaterial.byWallNo = m_iWallNo;
                    struVideowallMaterial.byWindowType = m_combWindowType.GetCurSel();
                    struVideowallMaterial.byFileType = m_combFileType.GetCurSel();
                    struVideowallMaterial.dwWindowNo = m_iWindowNo;
                    struVideowallMaterial.dwMaterialNo = m_iMaterialNo;
                    cPicFile.Close();
                }
            }
            break;
        default:
            fileType = UPLOAD_AI_ALGORITHM_MODEL;
            csUploadType = "UPLOAD_AI_ALGORITHM_MODEL";
            break;
        }
//        DWORD dwIndex;
        if (fileType == UPLOAD_AI_ALGORITHM_MODEL)
        {
            m_lUploadHandle = NET_DVR_UploadFile_V40(m_lUserID, fileType, &struAIAlgorithmModel, sizeof(struAIAlgorithmModel), m_strFileUploadPath.GetBuffer(0), NULL, 0);
        }
        else if (fileType == UPLOAD_AI_PICTURE)
        {
            m_lUploadHandle = NET_DVR_UploadFile_V40(m_lUserID, fileType, &struAIPicture, sizeof(struAIPicture), m_strFileUploadPath.GetBuffer(0), NULL, 0);
        }
        else if (fileType == UPLOAD_RATIOSTITCHING_FILE)
        {
            m_lUploadHandle = NET_DVR_UploadFile_V40(m_lUserID, fileType, &struratioStitching, sizeof(struratioStitching), m_strFileUploadPath.GetBuffer(0), NULL, 0);
        }
        else if (fileType == UPLOAD_VIDEOWALL_MATERIAL_FILE)
        {
            m_lUploadHandle = NET_DVR_UploadFile_V40(m_lUserID, fileType, &struVideowallMaterial, sizeof(struVideowallMaterial), m_strFileUploadPath.GetBuffer(0), NULL, 0);
        }
        
        if (m_lUploadHandle < 0)
        {

            g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, csUploadType);
            AfxMessageBox("UPLOAD_AI_ALGORITHM_MODEL Upload Failed");
            m_lUploadHandle = -1;
            return;
        }
        else
        {
            g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, csUploadType);
        }

        if (m_pThread != NULL)
        {
            CloseThread();
        }
        m_ThreadParam.pThis = this;
        m_ThreadParam.bExit = FALSE;
        m_pThread = AfxBeginThread(GetFileUpLoadThread, &m_ThreadParam, THREAD_PRIORITY_ABOVE_NORMAL, 0, CREATE_SUSPENDED);
        //线程结束时不自动删除
        m_pThread->m_bAutoDelete = FALSE;
        //恢复线程运行
        m_pThread->ResumeThread();
        if (m_pThread == NULL)
        {
            char szLan[256] = { 0 };
            g_StringLanType(szLan, "打开上传文件线程失败!", "open UpLoad thread Fail!");
            AfxMessageBox(szLan);
            return;
        }
        m_bUpLoading = TRUE;
    }
    else
    {
        NET_DVR_UploadClose(m_lUploadHandle);
        CloseThread();
        m_bUpLoading = FALSE;
        g_StringLanType(szLan, "上传", "UpLoad");
        GetDlgItem(IDC_BTN_FILE_UPLOAD)->SetWindowText(szLan);
    }
    UpdateData(FALSE);
}


void CDlgFileUploadDownload::OnBnClickedBtnFilePathUpload()
{
    // TODO:  在此添加控件通知处理程序代码
    static char szFilter[] = "All File(*.*)|*.*||";
    CFileDialog dlg(TRUE, "*.*", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
    if (dlg.DoModal() == IDOK)
    {
        m_strFileUploadPath = dlg.GetPathName();
        UpdateData(FALSE);
    }
}


void CDlgFileUploadDownload::OnBnClickedBtnSelectFile()
{
    // TODO:  在此添加控件通知处理程序代码
    static char szFilter[] = "All File(*.*)|*.*||";
    CFileDialog dlg(TRUE, "*.*", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
    if (dlg.DoModal() == IDOK)
    {
        m_strFileParaPath = dlg.GetPathName();
        UpdateData(FALSE);
    }
}

void CDlgFileUploadDownload::OnClose()
{
    // TODO:  在此添加消息处理程序代码和/或调用默认值
    CloseThread();
    CDialogEx::OnClose();
}

void CDlgFileUploadDownload::CloseThread()
{
    BOOL bResult = TRUE;
    if (m_bUpLoading == TRUE && m_lUploadHandle != NULL)
    {
        NET_DVR_UploadClose(m_lUploadHandle);
    }
    if (m_pThread != NULL)
    {
        m_ThreadParam.bExit = TRUE;
        DWORD dwResult = WaitForSingleObject(m_pThread->m_hThread, 1 * 1000);
        if (dwResult == WAIT_TIMEOUT)
        {
            DWORD dwExitCode = 0;
            if (GetExitCodeThread(m_pThread->m_hThread, &dwExitCode) && dwExitCode == STILL_ACTIVE)
            {
                bResult = TerminateThread(m_pThread->m_hThread, 0);
            }
        }
        if (bResult)
        {
            if (!m_pThread->m_bAutoDelete)
            {
                delete m_pThread;
                m_pThread = NULL;
            }
        }
    }
}

void CDlgFileUploadDownload::CloseDownLoadThread()
{
    BOOL bResult = TRUE;
    if (m_bDownLoading == TRUE && m_lDownloadHandle != NULL)
    {
        NET_DVR_StopDownload(m_lDownloadHandle);
    }
    if (m_pDownThread != NULL)
    {
        m_DownThreadParam.bExit = TRUE;
        DWORD dwResult = WaitForSingleObject(m_pDownThread->m_hThread, 1 * 1000);
        if (dwResult == WAIT_TIMEOUT)
        {
            DWORD dwExitCode = 0;
            if (GetExitCodeThread(m_pDownThread->m_hThread, &dwExitCode) && dwExitCode == STILL_ACTIVE)
            {
                bResult = TerminateThread(m_pDownThread->m_hThread, 0);
            }
        }
        if (bResult)
        {
            if (!m_pDownThread->m_bAutoDelete)
            {
                delete m_pDownThread;
                m_pDownThread = NULL;
            }
        }
    }
}




afx_msg LRESULT CDlgFileUploadDownload::OnUpdateStatus(WPARAM wParam, LPARAM lParam)
{
    if (wParam != NULL && lParam!= NULL)
    {
        GetDlgItem(IDC_BTN_FILE_UPLOAD)->SetWindowText((LPCTSTR)wParam);
        m_bUpLoading = *(bool*)lParam;
    }
    return 0;
}


afx_msg LRESULT CDlgFileUploadDownload::OnUploadFinish(WPARAM wParam, LPARAM lParam)
{
    if (wParam != NULL && lParam != NULL)
    {
        GetDlgItem(IDC_BTN_FILE_UPLOAD)->SetWindowText((LPCTSTR)wParam);
        m_bUpLoading = *(bool*)lParam;
    }
    CloseThread();
    return 0;
}

afx_msg LRESULT CDlgFileUploadDownload::OnUpdateDownloadStatus(WPARAM wParam, LPARAM lParam)
{
    if (wParam != NULL && lParam != NULL)
    {
        GetDlgItem(IDC_BTN_FILE_DOWNLOAD)->SetWindowText((LPCTSTR)wParam);
        m_bDownLoading = *(bool*)lParam;
    }
    return 0;
}


afx_msg LRESULT CDlgFileUploadDownload::OnDownloadFinish(WPARAM wParam, LPARAM lParam)
{
    if (wParam != NULL && lParam != NULL)
    {
        GetDlgItem(IDC_BTN_FILE_DOWNLOAD)->SetWindowText((LPCTSTR)wParam);
        m_bDownLoading = *(bool*)lParam;
    }
    CloseDownLoadThread();
    return 0;
}




void CDlgFileUploadDownload::OnBnClickedBtnSelectLicensekeyFile()
{
    // TODO:  在此添加控件通知处理程序代码
    static char szFilter[] = "All File(*.*)|*.*||";
    CFileDialog dlg(TRUE, "*.*", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
    if (dlg.DoModal() == IDOK)
    {
        m_strLicenseKeyFilePath = dlg.GetPathName();
        UpdateData(FALSE);
    }
}

void CDlgFileUploadDownload::OnBnClickedBtnFileDownload()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);

    char szLan[128] = { 0 };
    NET_SDK_DOWNLOAD_TYPE fileType;
    NET_DVR_RATIOSTITCHING_PARAM struratioStitching = { 0 };
    NET_DVR_VIDEOWALL_MATERIAL_COND struVideowallMaterial = { 0 };
    CString csDownloadType;
    switch (m_combDownloadType.GetCurSel())
    {
    case 0:
        fileType = NET_SDK_DOWNLOAD_DARK_FIGHTER_X_CORRECT_TABLE_MAIN;
        csDownloadType = "NET_SDK_DOWNLOAD_DARK_FIGHTER_X_CORRECT_TABLE_MAIN";
        break;
    case 1:
        fileType = NET_SDK_DOWNLOAD_DARK_FIGHTER_X_CORRECT_TABLE_BACKUP;
        csDownloadType = "NET_SDK_DOWNLOAD_DARK_FIGHTER_X_CORRECT_TABLE_BACKUP";
        break;
    case 2:
        fileType = NET_SDK_DOWNLOAD_RATIOSTITCHING_FILE;
        csDownloadType = "NET_SDK_DOWNLOAD_RATIOSTITCHING_FILE";
        struratioStitching.dwSize = sizeof(struratioStitching);
        struratioStitching.byChannel = m_lChannel;
        break;
    case 3:
        fileType = NET_SDK_DOWNLOAD_VIDEOWALL_MATERIAL_FILE;
        csDownloadType = "NET_SDK_DOWNLOAD_VIDEOWALL_MATERIAL_FILE";
        struVideowallMaterial.dwSize = sizeof(struVideowallMaterial);
        struVideowallMaterial.byWallNo = m_iWallNo;
        struVideowallMaterial.byWindowType = m_combWindowType.GetCurSel();
        struVideowallMaterial.byFileType = m_combFileType.GetCurSel();
        struVideowallMaterial.dwWindowNo = m_iWindowNo;
        struVideowallMaterial.dwMaterialNo = m_iMaterialNo;
        break;
    default:
        fileType = NET_SDK_DOWNLOAD_RATIOSTITCHING_FILE;
        csDownloadType = "NET_SDK_DOWNLOAD_RATIOSTITCHING_FILE";
        break;
    }

    if (NET_SDK_DOWNLOAD_RATIOSTITCHING_FILE == fileType)
    {
        m_lDownloadHandle = NET_DVR_StartDownload(m_lUserID, fileType, &struratioStitching, sizeof(struratioStitching), m_csDowmloadFilePath.GetBuffer(0));
    }
    else if (NET_SDK_DOWNLOAD_VIDEOWALL_MATERIAL_FILE == fileType)
    {
        m_lDownloadHandle = NET_DVR_StartDownload(m_lUserID, fileType, &struVideowallMaterial, sizeof(struVideowallMaterial), m_csDowmloadFilePath.GetBuffer(0));
    }
    else
    {
        m_lDownloadHandle = NET_DVR_StartDownload(m_lUserID, fileType, NULL, 0, m_csDowmloadFilePath.GetBuffer(0));
    }
   
    if (m_lDownloadHandle == -1)
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, csDownloadType);
        AfxMessageBox("NET_DVR_StartDownload failed");
    }
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, csDownloadType);
    }

    if (m_pDownThread != NULL)
    {
        CloseDownLoadThread();
    }
    m_DownThreadParam.pThis = this;
    m_DownThreadParam.bExit = FALSE;
    m_pDownThread = AfxBeginThread(GetFileDownLoadThread, &m_DownThreadParam, THREAD_PRIORITY_ABOVE_NORMAL, 0, CREATE_SUSPENDED);
    //线程结束时不自动删除
    m_pDownThread->m_bAutoDelete = FALSE;
    //恢复线程运行
    m_pDownThread->ResumeThread();
    if (m_pDownThread == NULL)
    {
        char szLan[256] = { 0 };
        g_StringLanType(szLan, "打开下载文件线程失败!", "open DownLoad thread Fail!");
        AfxMessageBox(szLan);
        return;
    }
    m_bDownLoading = TRUE;

    UpdateData(FALSE);
}


void CDlgFileUploadDownload::OnBnClickedBtnFilePathDownload()
{
    // TODO:  在此添加控件通知处理程序代码
    CString strFilter = _T("All Files(*.*)|*.*||");
    CFileDialog fileChose(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);
    if (fileChose.DoModal() != IDOK)
    {
        return;
    }
    m_csDowmloadFilePath = fileChose.GetPathName();
    GetDlgItem(IDC_EDIT_DOWNLOAD_PATH)->SetWindowText(m_csDowmloadFilePath);
}
