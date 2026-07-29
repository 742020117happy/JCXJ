// DlgSenceControl.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgSenceControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSenceControl dialog


CDlgSenceControl::CDlgSenceControl(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSenceControl::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSenceControl)
	m_dwCopyScene = 0;
	m_csSceneName = _T("");
	m_dwSceneNum = 0;
	m_dwSceneNumOperate = 0;
    m_csSceneFileSavePath = _T("");
    m_csSceneFileSeletePath = _T("");
    m_iUpdownloadHandle = -1;
    m_iDownloadHandle = -1;
    m_bDownloading = FALSE;
    m_bUploading = FALSE;
    m_dwSceneFileLen = 0;
	//}}AFX_DATA_INIT
}


void CDlgSenceControl::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgSenceControl)
    DDX_Control(pDX, IDC_LIST_SCENE, m_listScene);
    DDX_Control(pDX, IDC_COMBO_SCENE_OPERATE, m_comboSceneOperate);
    DDX_Text(pDX, IDC_EDIT_COPY_SCENE, m_dwCopyScene);
    DDX_Text(pDX, IDC_EDIT_SCENE_NAME, m_csSceneName);
    DDX_Text(pDX, IDC_EDIT_SCENE_NUM, m_dwSceneNum);
    DDX_Text(pDX, IDC_EDIT_SCENE_NUM_OPERATE, m_dwSceneNumOperate);
    //}}AFX_DATA_MAP
    DDX_Control(pDX, IDC_PROGRESS, m_ctrlProgress);
    DDX_Control(pDX, IDC_STATIC_PROGRESS, m_statProgress);
}


BEGIN_MESSAGE_MAP(CDlgSenceControl, CDialog)
	//{{AFX_MSG_MAP(CDlgSenceControl)
	ON_BN_CLICKED(IDC_BTN_MODE_GET, OnBtnModeGet)
	ON_BN_CLICKED(IDC_BTN_MODE_SET, OnBtnModeSet)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SCENE, OnClickListScene)
	ON_BN_CLICKED(IDC_BTN_SCENE_GET, OnBtnSceneGet)
	ON_CBN_SELCHANGE(IDC_COMBO_SCENE_OPERATE, OnSelchangeComboSceneOperate)
	ON_BN_CLICKED(IDC_BTN_CONTROL, OnBtnControl)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BTN_IMPORT_SCENE_FILE, &CDlgSenceControl::OnBnClickedBtnImportSceneFile)
    ON_BN_CLICKED(IDC_BTN_EXPORT_SCENE_FILE, &CDlgSenceControl::OnBnClickedBtnExportSceneFile)
    ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSenceControl message handlers

BOOL CDlgSenceControl::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

    m_ctrlProgress.SetRange(0, 100);
    m_ctrlProgress.SetStep(1);
    m_ctrlProgress.SetPos(0);
    m_statProgress.SetWindowText(_T("0%"));

	m_dwSetCount = 0;
	m_iCurSel = -1;
	memset(&m_struSenceName, 0, sizeof(m_struSenceName));
	memset(&m_struSenceNameSet, 0, sizeof(m_struSenceNameSet));
	memset(&m_dwStatus, 0, sizeof(m_dwStatus));
	memset(&m_dwSceneNoSet, 0 , sizeof(m_dwSceneNoSet));

	int i = 0;
	for (i = 0; i < SCENE_NUM; i++)
	{
		m_dwSceneNo[i] = i + 1;
	}	

	m_iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
	char szLan[128] = {0};
	m_listScene.SetExtendedStyle(m_listScene.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	g_StringLanType(szLan, "场景号", "Scene No.");
	m_listScene.InsertColumn(0, szLan, LVCFMT_LEFT, 80);
	g_StringLanType(szLan, "场景名", "Scene Name");
	m_listScene.InsertColumn(1, szLan, LVCFMT_LEFT, 200);

	OnBtnModeGet();
	OnSelchangeComboSceneOperate();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSenceControl::OnBtnModeGet() 
{
	// TODO: Add your control notification handler code here
	int i = 0;
	BOOL bOneFail = FALSE;
	char cs[2048] = {0};
	CString csTemp;
	if (!NET_DVR_GetDeviceConfig(m_lUserID, NET_DVR_WALLSCENEPARAM_GET, SCENE_NUM, m_dwSceneNo, SCENE_NUM * 4, m_dwStatus, m_struSenceName, SCENE_NUM * sizeof(NET_DVR_WALLSCENECFG)))
	{
        i = NET_DVR_GetLastError();
		sprintf(cs, "error code: %d", i);
		AfxMessageBox(cs);
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_WALLSCENEPARAM_GET");		
		//return;
	}
	else
	{
		sprintf(cs, "Not succeed:\n");
		for (i = 0; i < SCENE_NUM; i++)
		{
			if (m_dwStatus[i] > 0)
			{
				csTemp = m_listScene.GetItemText(i, 0);
				sprintf(cs, "%sScene No.: %s\n", cs, csTemp);
				bOneFail = TRUE;
			}
		}
		
		if (bOneFail)
		{
			AfxMessageBox(cs);
			m_dwSetCount = 0;		
			memset(m_dwStatus, 0, sizeof(m_dwStatus));
			memset(m_struSenceNameSet, 0, sizeof(m_struSenceNameSet));
		}
		else
		{
			g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_WALLSCENEPARAM_GET");
			m_dwSetCount = 0;		
			memset(m_dwStatus, 0, sizeof(m_dwStatus));
			memset(m_struSenceNameSet, 0, sizeof(m_struSenceNameSet));
		}
		m_listScene.DeleteAllItems();
	    DrawList();
	}	
}

void CDlgSenceControl::DrawList()
{
	int i = 0;
	CString cs;
    for(i = 0; i < SCENE_NUM; i++)
	{
		
		cs.Format("%d",  m_dwSceneNo[i]);
		m_listScene.InsertItem(i, cs, 0);		
		cs.Format("%s", m_struSenceName[i].sSceneName);
		m_listScene.SetItemText(i, 1, cs);			
	}
}

void CDlgSenceControl::OnBtnModeSet() 
{
	// TODO: Add your control notification handler code here
	memset(m_dwStatus, 0, sizeof(m_dwStatus));
	int i = 0;
	int j = 0;
	char cs[128] = {0};
	char szLan[128] = {0};
	if (m_dwSetCount == 0)
	{
		if (!NET_DVR_SetDeviceConfig(m_lUserID, NET_DVR_WALLSCENEPARAM_SET, SCENE_NUM, m_dwSceneNo, SCENE_NUM*4, m_dwStatus, m_struSenceName, SCENE_NUM*sizeof(NET_DVR_WALLSCENECFG)))
		{
			g_StringLanType(szLan, "设置失败", "Failed to set");
			AfxMessageBox(szLan);
			g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_WALLSCENEPARAM_SET");
			return;
		}
		else
		{
			sprintf(cs, "Fail Scene num:\n");
			for(i = 0; i < SCENE_NUM; i++)
			{
				if (m_dwStatus[i] > 0)
				{
					sprintf(cs, "%s %d\n", cs, m_dwSceneNo[i]);
					j++;
				}
			}
			if (j > 0)
			{
				AfxMessageBox(cs);
			}
			else
			{
				g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_WALLSCENEPARAM_SET");
			}
		}
	}
	else
	{
		if (!NET_DVR_SetDeviceConfig(m_lUserID, NET_DVR_WALLSCENEPARAM_SET, m_dwSetCount, m_dwSceneNoSet, 4*m_dwSetCount, m_dwStatus, m_struSenceNameSet, m_dwSetCount*sizeof(NET_DVR_WALLSCENECFG)))
		{
			g_StringLanType(szLan, "设置失败", "Failed to set");
			AfxMessageBox(szLan);
			g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_WALLSCENEPARAM_SET");
			return;
		}
		else
		{
			sprintf(cs, "Fail Scence num:\n");
			for(i = 0; i < SCENE_NUM; i++)
			{
				if (m_dwStatus[i] > 0)
				{
					sprintf(cs, "%s %d\n", cs, m_dwSceneNoSet[i]);
					j++;
				}
			}
			if (j > 0)
			{
				AfxMessageBox(cs);				
			}
			else
			{
				g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_WALLSCENEPARAM_SET");				
			}
			m_dwSetCount = 0;
			memset(m_dwSceneNoSet, 0, sizeof(m_dwSceneNoSet));
			memset(m_struSenceNameSet, 0, sizeof(m_struSenceNameSet));
		}
	}
}

void CDlgSenceControl::OnBtnSave() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szLan[128] = {0};
	if (-1 == m_iCurSel)
	{
		g_StringLanType(szLan, "未选中条目", "Item is not selected");
		AfxMessageBox(szLan);
	}

	int i = 0;
	CString cs;
	for (i = 0; i < SCENE_NUM; i++)
	{
		if (m_dwSceneNoSet[i] == m_iCurSel + 1)
		{
			break;
		}
		if (m_dwSceneNoSet[i] == 0)
		{
			m_dwSceneNoSet[i] = m_iCurSel + 1;
			m_dwSetCount++;
			break;
		}
	}
	
	m_struSenceNameSet[i].dwSize = sizeof(NET_DVR_WALLSCENECFG);
	strncpy((char *)m_struSenceNameSet[i].sSceneName, m_csSceneName, m_csSceneName.GetLength());
	
	cs.Format("%s", m_struSenceNameSet[i].sSceneName);
	m_listScene.SetItemText(m_iCurSel, 1, cs);	

}

void CDlgSenceControl::OnClickListScene(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	POSITION  iPos = m_listScene.GetFirstSelectedItemPosition();
	if (iPos == NULL)
	{
		return;
	}
    m_iCurSel = m_listScene.GetNextSelectedItem(iPos);

	m_csSceneName.Format("%s", m_struSenceNameSet[m_iCurSel].sSceneName);
	UpdateData(FALSE);

	*pResult = 0;
}

void CDlgSenceControl::OnBtnSceneGet() 
{
	// TODO: Add your control notification handler code here
	if (!NET_DVR_MatrixGetCurrentSceneMode(m_lUserID, &m_dwSceneNum))
	{
		MessageBox("NET_DVR_MatrixGetCurrentSceneMode FAILD");
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_MatrixGetCurrentSceneMode");
		return ;
	}
	else
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_MatrixGetCurrentSceneMode");
		UpdateData(FALSE);
	}
}

void CDlgSenceControl::OnSelchangeComboSceneOperate() 
{
	// TODO: Add your control notification handler code here
	if (m_comboSceneOperate.GetCurSel() == 3)
	{
		GetDlgItem(IDC_EDIT_COPY_SCENE)->ShowWindow(SW_HIDE);
	}
	else
	{
		m_dwCopyScene = 0;
		GetDlgItem(IDC_EDIT_COPY_SCENE)->ShowWindow(SW_HIDE);
	}
}

void CDlgSenceControl::OnBtnControl() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szLan[128] = {0};
	DWORD dwSceneCmd = m_comboSceneOperate.GetCurSel() + 1;
	if (dwSceneCmd == 0)
	{
		g_StringLanType(szLan, "请选择操作命令", "Please select the operating command");
		AfxMessageBox(szLan);
		return;
	}
	if (!NET_DVR_MatrixSceneControl(g_struDeviceInfo[m_iDeviceIndex].lLoginID, m_dwSceneNumOperate, dwSceneCmd, 0))
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_MatrixSceneControl");
		MessageBox("NET_DVR_MatrixSceneControl FAILED");
	}
	else
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_MatrixSceneControl");
		MessageBox("NET_DVR_MatrixSceneControl SUCC");
	}
}

void CDlgSenceControl::OnBtnExit() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

//导入场景文件
void CDlgSenceControl::OnBnClickedBtnImportSceneFile()
{
    // TODO:  在此添加控件通知处理程序代码
    //选择文件
    CString strFilter = _T("All Files(*.*)|*.*||");
    CFileDialog fileChose(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);
    if (fileChose.DoModal() != IDOK)
    {
        return;
    }
    m_csSceneFileSeletePath = fileChose.GetPathName();
    CFile file;
    if (!file.Open(m_csSceneFileSeletePath, CFile::modeRead))
    {
        char szLan1[512] = { 0 };
        char szLan2[512] = { 0 };
        g_StringLanType(szLan1, "打开文件失败", "Open file failed.");
        g_StringLanType(szLan2, "场景文件", "secne file");
        MessageBox(szLan1, szLan2, MB_ICONWARNING);
        return;
    }
    m_dwSceneFileLen = file.GetLength();
    file.Close();
    GetDlgItem(IDC_EDIT_SELETE_PATH)->SetWindowText(m_csSceneFileSeletePath);


    //导入
    m_iUpdownloadHandle = NET_DVR_UploadFile_V40(m_lUserID, UPLOAD_SCENE_FILE, NULL, 0, m_csSceneFileSeletePath.GetBuffer(0), NULL, 0);
    if (m_iUpdownloadHandle == -1)
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_UploadFile_V40 UPLOAD_SCENE_FILE");

        return;
    }
    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_UploadFile_V40 UPLOAD_SCENE_FILE");
    // m_dwMaterialID = GetResponseStatusID(szOutputBuff);


    m_bUploading = TRUE;
    m_ctrlProgress.SetPos(0);
    m_statProgress.SetWindowText(_T("0%"));
    SetTimer(TIMER_UPDOWNLOAD_SCENE_FILE_PROGRESS, 100, NULL);
}

//导出场景文件
void CDlgSenceControl::OnBnClickedBtnExportSceneFile()
{
    // TODO:  在此添加控件通知处理程序代码
    CString strFilter = _T("All Files(*.*)|*.*||");
    CFileDialog fileChose(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);

    if (fileChose.DoModal() != IDOK)
    {
        return;
    }
    m_csSceneFileSavePath = fileChose.GetPathName();

    GetDlgItem(IDC_EDIT_SAVE_PATH)->SetWindowText(m_csSceneFileSavePath);

    UpdateData(TRUE);
    m_iUpdownloadHandle = NET_DVR_StartDownload(m_lUserID, NET_SDK_DOWNLOAD_SCENE_FILE, NULL, 0, m_csSceneFileSavePath.GetBuffer(0));
    if (m_iUpdownloadHandle == -1)
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_StartDownload NET_SDK_DOWNLOAD_SCENE_FILE");
        return;
    }
    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_StartDownload NET_SDK_DOWNLOAD_SCENE_FILE");

    m_bDownloading = TRUE;
    m_ctrlProgress.SetPos(0);
    m_statProgress.SetWindowText(_T("0%"));
    SetTimer(TIMER_UPDOWNLOAD_SCENE_FILE_PROGRESS, 100, NULL);

    

}


void CDlgSenceControl::OnTimer(UINT_PTR nIDEvent)
{
    // TODO:  在此添加消息处理程序代码和/或调用默认值

    if (nIDEvent == TIMER_UPDOWNLOAD_SCENE_FILE_PROGRESS)
    {
        DWORD dwProgress = 0;
        LONG lStatus = -1;
        if (m_bUploading)
        {
            lStatus = NET_DVR_GetUploadState(m_iUpdownloadHandle, &dwProgress);
            if (lStatus == -1)
            {
                g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GetUploadState");
                if (!NET_DVR_UploadClose(m_iUpdownloadHandle))
                {
                    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_UploadClose");
                }
                else
                {
                    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_UploadClose");
                }
            }
            else
            {
                g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_GetUploadState");
            }
        }
        else if (m_bDownloading)
        {
            lStatus = NET_DVR_GetDownloadState(m_iUpdownloadHandle, &dwProgress);
            if (lStatus == -1)
            {
                g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GetDownloadState");
                if (!NET_DVR_StopDownload(m_iUpdownloadHandle))
                {
                    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_StopDownload");
                }
                else
                {
                    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_StopDownload");
                }
            }
            else
            {
                g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_GetDownloadState");
            }
        }
        switch (lStatus)
        {
        case 1: //上传或下载成功
        {
            m_ctrlProgress.SetPos(dwProgress);
            CString str;
            str.Format("%d", dwProgress);
            str += _T("%");
            m_statProgress.SetWindowText(str);
            if (m_bUploading)
            {
                if (!NET_DVR_UploadClose(m_iUpdownloadHandle))
                {
                    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_UploadClose");
                }
                else
                {
                    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_UploadClose");
                }
                m_bUploading = FALSE;
            }
            else if (m_bDownloading)
            {
                if (!NET_DVR_StopDownload(m_iUpdownloadHandle))
                {
                    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_StopDownload");
                }
                else
                {
                    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_StopDownload");
                }
                m_bDownloading = FALSE;
            }
            m_iUpdownloadHandle = -1;
            KillTimer(TIMER_UPDOWNLOAD_SCENE_FILE_PROGRESS);
            char sTitle[64] = { 0 };
            char sMsg[64] = { 0 };
            g_StringLanType(sTitle, "场景文件", "scene  file");
            g_StringLanType(sMsg, "上传/下载完成", "Upload/Download finished.");
            MessageBox(sMsg, sTitle, MB_OK | MB_ICONWARNING);
            break;
        }
        case 2: //正在上传或下载
        {
            m_ctrlProgress.SetPos(dwProgress);
            CString str;
            str.Format("%d", dwProgress);
            str += _T("%");
            m_statProgress.SetWindowText(str);
            break;
        }
        case 3: //上传或下载失败
        {
            if (m_bUploading)
            {
                if (!NET_DVR_UploadClose(m_iUpdownloadHandle))
                {
                    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_UploadClose");
                }
                else
                {
                    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_UploadClose");
                }
                m_bUploading = FALSE;
            }
            else if (m_bDownloading)
            {
                if (!NET_DVR_StopDownload(m_iUpdownloadHandle))
                {
                    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_StopDownload");
                }
                else
                {
                    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_StopDownload");
                }
                m_bDownloading = FALSE;
            }
            m_iUpdownloadHandle = -1;
            KillTimer(TIMER_UPDOWNLOAD_SCENE_FILE_PROGRESS);
            char sTitle[64] = { 0 };
            char sMsg[64] = { 0 };
            g_StringLanType(sTitle, "场景文件", "scene file");
            g_StringLanType(sMsg, "上传/下载失败", "Upload/Download failed.");
            MessageBox(sMsg, sTitle, MB_OK | MB_ICONWARNING);
            break;
        }
        case 4: //网络断开
        {
            if (m_bUploading)
            {
                if (!NET_DVR_UploadClose(m_iUpdownloadHandle))
                {
                    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_UploadClose");
                }
                else
                {
                    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_UploadClose");
                }
                m_bUploading = FALSE;
            }
            else if (m_bDownloading)
            {
                if (!NET_DVR_StopDownload(m_iUpdownloadHandle))
                {
                    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_StopDownload");
                }
                else
                {
                    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_StopDownload");
                }
                m_bDownloading = FALSE;
            }
            m_iUpdownloadHandle = -1;
            KillTimer(TIMER_UPDOWNLOAD_SCENE_FILE_PROGRESS);
            char sTitle[64] = { 0 };
            char sMsg[64] = { 0 };
            g_StringLanType(sTitle, "场景", "scene file");
            g_StringLanType(sMsg, "网络断开", "Network disconnection.");
            MessageBox(sMsg, sTitle, MB_OK | MB_ICONWARNING);
            break;
        }
        }
    }

    CDialog::OnTimer(nIDEvent);
}
