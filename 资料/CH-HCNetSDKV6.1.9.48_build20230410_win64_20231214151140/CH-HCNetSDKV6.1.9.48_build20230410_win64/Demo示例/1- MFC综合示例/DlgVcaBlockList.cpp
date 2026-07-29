// DlgVcaBlockList.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgVcaBlockList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgVcaBlockList dialog
CDlgVcaBlockList *g_pVcaBlockList = NULL;
UINT GetBlockListThread(LPVOID pParam)
{
	
    UNREFERENCED_PARAMETER(pParam);
	
	LONG bRet = -1;
	NET_VCA_BLOCKLIST_INFO struBlockListData;
    memset(&struBlockListData, 0, sizeof(struBlockListData));
	CString csTmp;
	char szLan[128] = {0};
	while (1) 
	{
		bRet = NET_DVR_FindNextBlockList(g_pVcaBlockList->m_lFindHandle, &struBlockListData);
        if (bRet == NET_DVR_FILE_SUCCESS)
		{
			csTmp.Format("%d", g_pVcaBlockList->m_lBlockListNum+1);
			g_pVcaBlockList->m_listBlockList.InsertItem(g_pVcaBlockList->m_lBlockListNum, csTmp,0);

			switch (struBlockListData.byType)
			{
			case 0:
				g_StringLanType(szLan, "全部", "ALL");
				break;
			case 1:
				g_StringLanType(szLan, "允许名单", "Allow list");
				break;
			case 2:
				g_StringLanType(szLan, "禁止名单", "Block list");
				break;
			default:
                g_StringLanType(szLan, "未知", "Unknow");
				break;
			}
            csTmp.Format("%s", szLan);
			g_pVcaBlockList->m_listBlockList.SetItemText(g_pVcaBlockList->m_lBlockListNum, 1, csTmp);

            csTmp.Format("%s", struBlockListData.struAttribute.byName);
			g_pVcaBlockList->m_listBlockList.SetItemText(g_pVcaBlockList->m_lBlockListNum, 2, csTmp);

			switch (struBlockListData.struAttribute.bySex)
			{
			case 0:
				g_StringLanType(szLan, "无", "No");
				break;
			case 1:
				g_StringLanType(szLan, "男", "Man");
				break;
			case 2:
				g_StringLanType(szLan, "女", "Woman");
				break;
			default:
                g_StringLanType(szLan, "未知", "Unknow");
				break;
			}
			csTmp.Format("%s", szLan);
			g_pVcaBlockList->m_listBlockList.SetItemText(g_pVcaBlockList->m_lBlockListNum, 3, csTmp);

			csTmp.Format("%s", struBlockListData.struAttribute.byBirthDate);
			g_pVcaBlockList->m_listBlockList.SetItemText(g_pVcaBlockList->m_lBlockListNum, 4, csTmp);

			csTmp.Format("%d", struBlockListData.struAttribute.struNativePlace.wCityID);
			g_pVcaBlockList->m_listBlockList.SetItemText(g_pVcaBlockList->m_lBlockListNum, 5, csTmp);

            csTmp.Format("%d", struBlockListData.dwRegisterID);
			g_pVcaBlockList->m_listBlockList.SetItemText(g_pVcaBlockList->m_lBlockListNum, 6, csTmp);

			csTmp.Format("%d", struBlockListData.dwGroupNo);
			g_pVcaBlockList->m_listBlockList.SetItemText(g_pVcaBlockList->m_lBlockListNum, 7, csTmp);

			switch (struBlockListData.struAttribute.byCertificateType)
			{
			case 0:
				g_StringLanType(szLan, "无", "No");
				break;
			case 1:
				g_StringLanType(szLan, "身份证", "identification card");
				break;
			case 2:
				g_StringLanType(szLan, "警官证", "Officers card");
				break;
			default:
                g_StringLanType(szLan, "未知", "Unknow");
				break;
			}
			csTmp.Format("%s", szLan);
			g_pVcaBlockList->m_listBlockList.SetItemText(g_pVcaBlockList->m_lBlockListNum, 8, csTmp);

			csTmp.Format("%s", struBlockListData.struAttribute.byCertificateNumber);
			g_pVcaBlockList->m_listBlockList.SetItemText(g_pVcaBlockList->m_lBlockListNum, 9, csTmp);

			csTmp.Format("%s", struBlockListData.byRemark);
			g_pVcaBlockList->m_listBlockList.SetItemText(g_pVcaBlockList->m_lBlockListNum, 10, csTmp);

			switch (struBlockListData.byLevel)
			{
			case 0:
				g_StringLanType(szLan, "全部", "ALL");
				break;
			case 1:
				g_StringLanType(szLan, "低", "LOW");
				break;
			case 2:
				g_StringLanType(szLan, "中", "Middle");
				break;
			case 3:
				g_StringLanType(szLan, "高", "High");
				break;
			default:
                g_StringLanType(szLan, "未知", "Unknow");
				break;
			}
			csTmp.Format("%s", szLan);
			g_pVcaBlockList->m_listBlockList.SetItemText(g_pVcaBlockList->m_lBlockListNum, 11, csTmp);
			
			g_pVcaBlockList->m_lBlockListNum++;
		}
		else
		{
			if (bRet == NET_DVR_ISFINDING)
			{
				g_pVcaBlockList->GetDlgItem(IDC_STATIC_SEARCH)->ShowWindow(SW_SHOW);
				g_pVcaBlockList->GetDlgItem(IDC_STATIC_SEARCH)->SetWindowText("Searching......");
				Sleep(5);
				continue;
			}
			if ((bRet == NET_DVR_NOMOREFILE) || (bRet == NET_DVR_FILE_NOFIND))
			{
				g_StringLanType(szLan, "搜索", "search");
				g_pVcaBlockList->GetDlgItem(IDC_BTN_SEARCH)->SetWindowText(szLan);
				//				g_pPDCInfoSearch->m_bSearch = FALSE;
				g_pVcaBlockList->GetDlgItem(IDC_STATIC_SEARCH)->ShowWindow(SW_HIDE);
				g_StringLanType(szLan, "搜索禁止名单结束!", "Search blocklist Ending");
				AfxMessageBox(szLan);
				break;
			}
			else
			{
				g_StringLanType(szLan, "搜索", "Search");
				g_pVcaBlockList->GetDlgItem(IDC_BTN_SEARCH)->SetWindowText(szLan);
				//				g_pPDCInfoSearch->m_bSearch = FALSE;
				g_StringLanType(szLan, "由于服务器忙,或网络故障,搜索异常终止!",\
					"Since the server is busy, or network failure, search abnormal termination");
				AfxMessageBox(szLan);
				break;
			}
		}
	}
	CloseHandle(g_pVcaBlockList->m_hFindThread);
	g_pVcaBlockList->m_hFindThread = NULL;
	NET_DVR_FindBlockListClose(g_pVcaBlockList->m_lFindHandle);
	
	return 0;
}


CDlgVcaBlockList::CDlgVcaBlockList(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgVcaBlockList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgVcaBlockList)
	m_csCertificateNumber = _T("");
	m_dwGroupID = 0;
	m_csName = _T("");
	m_csRemark = _T("");
	m_dwRegisterID = 0;
	m_csBirthDate = _T("");
	m_csFastRegPicPath = _T("");
	m_bChkImportPicModel = FALSE;
	//}}AFX_DATA_INIT
	m_lServerID = -1;
    m_lChannel  = -1;
    m_iDevIndex = -1;
	m_lFindHandle = -1;
	m_hFindThread = NULL;
	m_lBlockListNum = 0;
	memset(&m_struBlockListPara, 0, sizeof(m_struBlockListPara));
	memset(&m_struBlockListPic, 0, sizeof(m_struBlockListPic));
	memset(&m_struBlFastPara, 0, sizeof(m_struBlFastPara));
}


void CDlgVcaBlockList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgVcaBlockList)
	DDX_Control(pDX, IDC_COMBO_LEVEL, m_comboLevel);
    DDX_Control(pDX, IDC_LIST_BLOCKLIST, m_listBlockList);
	DDX_Control(pDX, IDC_COMBO_NATIVE_PLACE, m_comboNativePlace);
	DDX_Control(pDX, IDC_COMBO_CERFITICATE_TYPE, m_comboCerfiticateType);
    DDX_Control(pDX, IDC_COMBO_BLOCKLIST_TYPE, m_comboBlockListType);
	DDX_Control(pDX, IDC_COMBO_SEX, m_comboSex);
	DDX_Text(pDX, IDC_EDIT_CERTIFICATE_NUMBER, m_csCertificateNumber);
	DDX_Text(pDX, IDC_EDIT_GROUP, m_dwGroupID);
	DDX_Text(pDX, IDC_EDIT_NAME, m_csName);
	DDX_Text(pDX, IDC_EDIT_REMARK, m_csRemark);
	DDX_Text(pDX, IDC_EDIT_REGISTER_ID, m_dwRegisterID);
	DDX_Text(pDX, IDC_EDIT_BIRTHDATE, m_csBirthDate);
	DDX_Text(pDX, IDC_EDIT_FASTREGISTER_PICPATH, m_csFastRegPicPath);
	DDX_Check(pDX, IDC_CHK_IMPORT_PIC_MODEL, m_bChkImportPicModel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgVcaBlockList, CDialog)
	//{{AFX_MSG_MAP(CDlgVcaBlockList)
	ON_BN_CLICKED(IDC_BTN_REGISTER, OnBtnRegister)
	ON_BN_CLICKED(IDC_BTN_SEARCH, OnBtnSearch)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
    ON_NOTIFY(NM_DBLCLK, IDC_LIST_BLOCKLIST, OnDblclkListBlocklist)
	ON_BN_CLICKED(IDC_BTN_UPDATE, OnBtnUpdate)
	ON_BN_CLICKED(IDC_BTN_DELETE, OnBtnDelete)
    ON_BN_CLICKED(IDC_BTN_GET_BLOCKLIST_PIC, OnBtnGetBlocklistPic)
	ON_BN_CLICKED(IDC_BTN_FAST_SCAN, OnBtnFastScan)
	ON_BN_CLICKED(IDC_BTN_FAST_REGISTER, OnBtnFastRegister)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgVcaBlockList message handlers
BOOL CDlgVcaBlockList::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_comboBlockListType.SetCurSel(0);
	m_comboCerfiticateType.SetCurSel(0);
	m_comboNativePlace.SetCurSel(0);
	m_comboSex.SetCurSel(0);
	m_comboLevel.SetCurSel(0);

	g_pVcaBlockList = this;

	char szLan[128] = {0};
    
    m_listBlockList.SetExtendedStyle(m_listBlockList.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_SUBITEMIMAGES);
    g_StringLanType(szLan, "序号", "NO.");
    m_listBlockList.InsertColumn(0, szLan, LVCFMT_RIGHT, 60, -1);
	g_StringLanType(szLan, "类型", "Type");
	m_listBlockList.InsertColumn(1, szLan, LVCFMT_LEFT, 80, -1);
	g_StringLanType(szLan, "姓名", "Name");
    m_listBlockList.InsertColumn(2, szLan, LVCFMT_LEFT, 80, -1);
	g_StringLanType(szLan, "性别", "Sex");
    m_listBlockList.InsertColumn(3, szLan, LVCFMT_LEFT,60, -1);
    g_StringLanType(szLan, "出生年月", "Age");
    m_listBlockList.InsertColumn(4, szLan, LVCFMT_LEFT, 80, -1);   
	g_StringLanType(szLan, "籍贯", "Native Place");
	m_listBlockList.InsertColumn(5, szLan, LVCFMT_LEFT, 80, -1);
    g_StringLanType(szLan, "禁止名单ID", "BlockList ID");
    m_listBlockList.InsertColumn(6, szLan, LVCFMT_LEFT, 100, -1);
	g_StringLanType(szLan, "分组号", "BlockList Group");
    m_listBlockList.InsertColumn(7, szLan, LVCFMT_LEFT, 60, -1);
	g_StringLanType(szLan, "证件类型", "Certificate Type");
	m_listBlockList.InsertColumn(8, szLan, LVCFMT_LEFT, 80, -1);
	g_StringLanType(szLan, "证件号", "NO.");
	m_listBlockList.InsertColumn(9, szLan, LVCFMT_LEFT, 140, -1);
	g_StringLanType(szLan, "备注", "Remark");
	m_listBlockList.InsertColumn(10, szLan, LVCFMT_LEFT, 200, -1);
	g_StringLanType(szLan, "等级", "Level");
	m_listBlockList.InsertColumn(11, szLan, LVCFMT_LEFT, 80, -1);
/*
	CString csTmp;
//	char szLan[128] = {0};
	NET_VCA_BLOCKLIST_INFO struBlockListData;
    memset(&struBlockListData, 0, sizeof(struBlockListData));

	csTmp.Format("%d", g_pVcaBlockList->m_lBlockListNum+1);
	g_pVcaBlockList->m_listBlockList.InsertItem(g_pVcaBlockList->m_lBlockListNum, csTmp,0);
	
	switch (struBlockListData.byType)
	{
	case 0:
		g_StringLanType(szLan, "禁止名单", "Allow list");
		break;
	case 1:
		g_StringLanType(szLan, "禁止名单", "Block list");
	default:
		g_StringLanType(szLan, "未知", "Unknow");
		break;
	}
	csTmp.Format("%s", szLan);
	g_pVcaBlockList->m_listBlockList.SetItemText(g_pVcaBlockList->m_lBlockListNum, 1, csTmp);
	
	csTmp.Format("%s", struBlockListData.struAttribute.byName);
	g_pVcaBlockList->m_listBlockList.SetItemText(g_pVcaBlockList->m_lBlockListNum, 2, csTmp);
	
	switch (struBlockListData.struAttribute.bySex)
	{
	case 0:
		g_StringLanType(szLan, "女", "Man");
		break;
	case 1:
		g_StringLanType(szLan, "女", "Woman");
		break;
	default:
		g_StringLanType(szLan, "未知", "Unknow");
		break;
	}
	csTmp.Format("%s", szLan);
	g_pVcaBlockList->m_listBlockList.SetItemText(g_pVcaBlockList->m_lBlockListNum, 3, csTmp);
	
	csTmp.Format("%d", 20);
	g_pVcaBlockList->m_listBlockList.SetItemText(g_pVcaBlockList->m_lBlockListNum, 4, csTmp);
	
	csTmp.Format("%d", struBlockListData.struAttribute.dwNativePlace);
	g_pVcaBlockList->m_listBlockList.SetItemText(g_pVcaBlockList->m_lBlockListNum, 5, csTmp);
	
	csTmp.Format("%d", 50);
	g_pVcaBlockList->m_listBlockList.SetItemText(g_pVcaBlockList->m_lBlockListNum, 6, csTmp);
	
	csTmp.Format("%d", 20);
	g_pVcaBlockList->m_listBlockList.SetItemText(g_pVcaBlockList->m_lBlockListNum, 7, csTmp);
	
	switch (struBlockListData.struAttribute.byCertificateType)
	{
	case 0:
		g_StringLanType(szLan, "警官证", "identification card");
		break;
	case 1:
		g_StringLanType(szLan, "警官证", "Officers card");
		break;
	default:
		g_StringLanType(szLan, "未知", "Unknow");
		break;
	}
	csTmp.Format("%s", szLan);
	g_pVcaBlockList->m_listBlockList.SetItemText(g_pVcaBlockList->m_lBlockListNum, 8, csTmp);
	
	csTmp.Format("%s", "33012001");
	g_pVcaBlockList->m_listBlockList.SetItemText(g_pVcaBlockList->m_lBlockListNum, 9, csTmp);
	
	csTmp.Format("%s", "");
	g_pVcaBlockList->m_listBlockList.SetItemText(g_pVcaBlockList->m_lBlockListNum, 10, csTmp);		
*/	
    UpdateData(FALSE);
	
	return TRUE;
}

void CDlgVcaBlockList::GetBlockListInfoFromWnd(NET_VCA_BLOCKLIST_INFO& struBlockListInfo)
{
	UpdateData(TRUE);
	memcpy(struBlockListInfo.struAttribute.byBirthDate, m_csBirthDate, MAX_HUMAN_BIRTHDATE_LEN);
	struBlockListInfo.struAttribute.bySex = m_comboSex.GetCurSel();
	struBlockListInfo.struAttribute.byCertificateType = m_comboCerfiticateType.GetCurSel();
	struBlockListInfo.struAttribute.struNativePlace.wCityID = m_comboNativePlace.GetCurSel();
	memcpy(struBlockListInfo.struAttribute.byName, m_csName, NAME_LEN);
	memcpy(struBlockListInfo.struAttribute.byCertificateNumber, m_csCertificateNumber, NAME_LEN);
	struBlockListInfo.dwRegisterID = m_dwRegisterID;
	struBlockListInfo.dwGroupNo = m_dwGroupID;
	struBlockListInfo.byType = m_comboBlockListType.GetCurSel();
	struBlockListInfo.byLevel = m_comboLevel.GetCurSel();
	memcpy(struBlockListInfo.byRemark, m_csRemark, NAME_LEN);
}

void CDlgVcaBlockList::OnBtnRegister() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int i=0;
	char szLan[128] = {0};
	memset(&m_struBlockListPara, 0, sizeof(m_struBlockListPara));
	for (i=0; i<MAX_HUMAN_PICTURE_NUM; i++)
	{
        m_struBlockListPara.struRegisterPic[i].pImage = new BYTE[10*1024];
		if (m_struBlockListPara.struRegisterPic[i].pImage == NULL)
		{
			return;
		}
		memset(m_struBlockListPara.struRegisterPic[i].pImage, 0, 10*1024);
		m_struBlockListPara.struRegisterPic[i].pModel = new BYTE[10*1024];
		if (m_struBlockListPara.struRegisterPic[i].pModel == NULL)
		{
			return;
		}
		memset(m_struBlockListPara.struRegisterPic[i].pModel, 0, 10*1024);
	}
	GetBlockListInfoFromWnd(m_struBlockListPara.struBlockListInfo);
	m_struBlockListPara.dwSize = sizeof(m_struBlockListPara);
	for (i=0; i<MAX_HUMAN_PICTURE_NUM; i++)
	{
		if (g_struFaceModel[i].dwFacePicLen>0 && g_struFaceModel[i].dwFaceModelLen>0)
		{
			m_struBlockListPara.struRegisterPic[i].dwImageLen = g_struFaceModel[i].dwFacePicLen;
			m_struBlockListPara.struRegisterPic[i].dwModelLen = g_struFaceModel[i].dwFaceModelLen;
			memcpy(m_struBlockListPara.struRegisterPic[i].pImage, g_struFaceModel[i].byFacePic, g_struFaceModel[i].dwFacePicLen);
			memcpy(m_struBlockListPara.struRegisterPic[i].pModel, g_struFaceModel[i].byModelData, g_struFaceModel[i].dwFaceModelLen);
			m_struBlockListPara.dwRegisterPicNum++;
		}
	}
	if (!NET_DVR_AddBlockList(m_lServerID, m_lChannel, &m_struBlockListPara))
	{
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_AddBlockList m_lChannel[%d] ",m_lChannel);
        g_StringLanType(szLan, "注册禁止名单失败", "Fail to register blocklist");
        AfxMessageBox(szLan);
    }
    else
    {
		g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_AddBlockList m_lChannel[%d] ",m_lChannel);
		g_StringLanType(szLan, "注册禁止名单成功", "Succ to register blocklist");
        AfxMessageBox(szLan);
    }

	for (i=0; i<MAX_HUMAN_PICTURE_NUM; i++)
	{
		delete []m_struBlockListPara.struRegisterPic[i].pImage;
		m_struBlockListPara.struRegisterPic[i].pImage = NULL;
		delete []m_struBlockListPara.struRegisterPic[i].pModel;
		m_struBlockListPara.struRegisterPic[i].pModel = NULL;
	}
}

void CDlgVcaBlockList::OnBtnSearch() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szLan[128] = {0};

    NET_VCA_BLOCKLIST_COND struFindCond = {0};
	struFindCond.dwGroupNo = m_dwGroupID;
	struFindCond.byLevel = m_comboLevel.GetCurSel();
	struFindCond.byType = m_comboBlockListType.GetCurSel();
	struFindCond.lChannel = m_lChannel;
	struFindCond.struAttribute.bySex = m_comboSex.GetCurSel();
	struFindCond.struAttribute.struNativePlace.wCityID = m_comboNativePlace.GetCurSel();
	struFindCond.struAttribute.byCertificateType = m_comboCerfiticateType.GetCurSel();
	memcpy(struFindCond.struAttribute.byCertificateNumber, m_csCertificateNumber, NAME_LEN);
	memcpy(struFindCond.struAttribute.byName, m_csName, NAME_LEN);
	memcpy(struFindCond.struAttribute.byBirthDate, m_csBirthDate, MAX_HUMAN_BIRTHDATE_LEN);

	m_lFindHandle = NET_DVR_FindBlockList(m_lServerID, &struFindCond);
	if (m_lFindHandle < 0)
	{
		g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_FindBlockList");
		
		g_StringLanType(szLan, "搜索禁止名单失败", "Search blocklist failed");
		AfxMessageBox(szLan);
        return;
	}
	else
	{
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_FindBlockList");
		
		g_StringLanType(szLan, "搜索禁止名单成功", "Search blocklist successfully");
		AfxMessageBox(szLan);
	}

	m_lBlockListNum = 0;
	m_listBlockList.DeleteAllItems();
	DWORD dwThreadId = 0;
	if (m_hFindThread == NULL)
	{
		m_hFindThread = CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(GetBlockListThread), this, 0, &dwThreadId);
	}

	if (m_hFindThread  == NULL)
	{
		g_StringLanType(szLan, "打开线程失败", "Open thread failed");
		AfxMessageBox(szLan);
		return;
    }

	g_StringLanType(szLan, "搜索", "search");
	g_pVcaBlockList->GetDlgItem(IDC_BTN_SEARCH)->SetWindowText(szLan);
    
}

void CDlgVcaBlockList::OnBtnExit() 
{
	// TODO: Add your control notification handler code here
	int i;
	if (m_hFindThread != NULL)
	{
		TerminateThread(m_hFindThread, 0);
		CloseHandle(m_hFindThread);
		m_hFindThread = NULL;
		NET_DVR_FindBlockListClose(m_lFindHandle);
	}

	for (i=0; i<MAX_HUMAN_PICTURE_NUM; i++)
	{
		g_struFacePic[i].dwImageID = 0;
		g_struFacePic[i].dwFaceScore = 0;
		memset(&g_struFacePic[i].struVcaRect, 0, sizeof(NET_VCA_RECT));
	}

	CDialog::OnCancel();
}

void CDlgVcaBlockList::OnDblclkListBlocklist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int iItemSel = 0;
	CString csTxt;
//	CString csTemp;
	char szTemp[128] = {0};
//	DWORD dwDevPort;
	
	POSITION  iPos = m_listBlockList.GetFirstSelectedItemPosition();
	if (iPos == NULL)
	{
		return;
	}
	iItemSel = m_listBlockList.GetNextSelectedItem(iPos);
	csTxt.Format("%s", m_listBlockList.GetItemText(iItemSel, 1));
    strcpy(szTemp, csTxt);
	if (!strcmp(szTemp, "允许名单"))
	{
		m_comboBlockListType.SetCurSel(1);
	}
	else if (!strcmp(szTemp, "禁止名单"))
	{
		m_comboBlockListType.SetCurSel(2);
	}

	m_csName.Format("%s", m_listBlockList.GetItemText(iItemSel, 2));
	
    csTxt.Format("%s", m_listBlockList.GetItemText(iItemSel, 3));
    strcpy(szTemp, csTxt);
	if (!strcmp(szTemp, "男"))
	{
		m_comboSex.SetCurSel(1);
	}
	else if (!strcmp(szTemp, "女"))
	{
		m_comboSex.SetCurSel(2);
	}
	else
	{
        m_comboSex.SetCurSel(0);
	}

	m_csBirthDate.Format("%s", m_listBlockList.GetItemText(iItemSel, 4));

	csTxt.Format("%s", m_listBlockList.GetItemText(iItemSel, 5));
	m_comboNativePlace.SetCurSel(CStringTodwIP(csTxt));

	csTxt.Format("%s", m_listBlockList.GetItemText(iItemSel, 6));
	m_dwRegisterID = CStringTodwIP(csTxt);

	csTxt.Format("%s", m_listBlockList.GetItemText(iItemSel, 7));
	m_dwGroupID = CStringTodwIP(csTxt);

    csTxt.Format("%s", m_listBlockList.GetItemText(iItemSel, 8));
    strcpy(szTemp, csTxt);
	if (!strcmp(szTemp, "身份证"))
	{
		m_comboCerfiticateType.SetCurSel(1);
	}
	else if (!strcmp(szTemp, "警官证"))
	{
		m_comboCerfiticateType.SetCurSel(2);
	}
	else
	{
        m_comboCerfiticateType.SetCurSel(0);
	}

    m_csCertificateNumber.Format("%s", m_listBlockList.GetItemText(iItemSel, 9));

	m_csRemark.Format("%s", m_listBlockList.GetItemText(iItemSel, 10));

	csTxt.Format("%s", m_listBlockList.GetItemText(iItemSel, 11));
    strcpy(szTemp, csTxt);
	if (!strcmp(szTemp, "高"))
	{
		m_comboLevel.SetCurSel(2);
	}
	else if (!strcmp(szTemp, "中"))
	{
		m_comboLevel.SetCurSel(1);
	}
	else
	{
        m_comboLevel.SetCurSel(0);
	}

	*pResult = 0;
	UpdateData(FALSE);
}

void CDlgVcaBlockList::OnBtnUpdate() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int i;
	char szLan[128] = {0};
	NET_VCA_BLOCKLIST_PARA struBlockListPara = {0};
	for (i=0; i<MAX_HUMAN_PICTURE_NUM; i++)
	{
        struBlockListPara.struRegisterPic[i].pImage = new BYTE[10*1024];
		if (struBlockListPara.struRegisterPic[i].pImage == NULL)
		{
			return;
		}
		struBlockListPara.struRegisterPic[i].pModel = new BYTE[10*1024];
		if (struBlockListPara.struRegisterPic[i].pModel == NULL)
		{
			return;
		}
	}
	GetBlockListInfoFromWnd(struBlockListPara.struBlockListInfo);
	struBlockListPara.dwSize = sizeof(struBlockListPara);
	for (i=0; i<MAX_HUMAN_PICTURE_NUM; i++)
	{
		if (g_struFaceModel[i].dwFacePicLen > 0)
		{
			struBlockListPara.struRegisterPic[i].dwImageLen = g_struFaceModel[i].dwFacePicLen;
			struBlockListPara.struRegisterPic[i].dwModelLen = g_struFaceModel[i].dwFaceModelLen;
			memcpy(struBlockListPara.struRegisterPic[i].pImage, g_struFaceModel[i].byFacePic, g_struFaceModel[i].dwFacePicLen);
			memcpy(struBlockListPara.struRegisterPic[i].pModel, g_struFaceModel[i].byModelData, g_struFaceModel[i].dwFaceModelLen);
		    struBlockListPara.dwRegisterPicNum++;
		}
	}

	if (!NET_DVR_UpdateBlockList(m_lServerID, m_lChannel, &struBlockListPara))
	{
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_UpdateBlockList m_lChannel[%d] ",m_lChannel);
        g_StringLanType(szLan, "修改禁止名单失败", "Fail to Update blocklist");
        AfxMessageBox(szLan);
    }
    else
    {
		g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_UpdateBlockList m_lChannel[%d] ",m_lChannel);
		g_StringLanType(szLan, "修改禁止名单成功", "Succ to Update blocklist");
        AfxMessageBox(szLan);
    }

	for (i=0; i<MAX_HUMAN_PICTURE_NUM; i++)
	{
		delete []struBlockListPara.struRegisterPic[i].pImage;
		struBlockListPara.struRegisterPic[i].pImage= NULL;
		delete []struBlockListPara.struRegisterPic[i].pModel;
		struBlockListPara.struRegisterPic[i].pModel = NULL;
	}
}

void CDlgVcaBlockList::OnBtnDelete() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szLan[128] = {0};
	
	if (!NET_DVR_DelBlockList(m_lServerID, m_lChannel, m_dwRegisterID))
	{
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_DelBlockList m_lChannel[%d] ",m_lChannel);
        g_StringLanType(szLan, "删除禁止名单失败", "Fail to Delete blocklist");
        AfxMessageBox(szLan);
    }
    else
    {
		g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_DelBlockList m_lChannel[%d] ",m_lChannel);
		g_StringLanType(szLan, "删除禁止名单成功", "Succ to Delete blocklist");
        AfxMessageBox(szLan);
    }
}

void CDlgVcaBlockList::OnBtnGetBlocklistPic() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
    char szLan[128] = {0};
	int i;
	NET_VCA_BLOCKLIST_PIC struBlockListPic = {0};
	for (i=0; i<MAX_HUMAN_PICTURE_NUM; i++)
	{
		struBlockListPic.struBlockListPic[i].pImage = new BYTE[10 * 1024];
		memset(struBlockListPic.struBlockListPic[i].pImage, 0, 10*1024);
		struBlockListPic.struBlockListPic[i].pModel = new BYTE[10*1024];
		memset(struBlockListPic.struBlockListPic[i].pModel, 0, 10*1024);
	}
	
	if (!NET_DVR_GetBlockListPicture(m_lServerID, m_dwRegisterID, &struBlockListPic))
	{
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_GetBlockListPicture m_dwRegisterID[%d] ",m_dwRegisterID);
        g_StringLanType(szLan, "下载禁止名单图片失败", "Fail to Get blocklist picture");
        AfxMessageBox(szLan);

		for (i=0; i<MAX_HUMAN_PICTURE_NUM;i++)
		{
			delete []struBlockListPic.struBlockListPic[i].pImage;
			struBlockListPic.struBlockListPic[i].pImage = NULL;
			delete []struBlockListPic.struBlockListPic[i].pModel;
			struBlockListPic.struBlockListPic[i].pModel = NULL;
		}
		return;
    }
    else
    {
		g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_GetBlockListPicture m_dwRegisterID[%d] ",m_dwRegisterID);
		g_StringLanType(szLan, "下载禁止名单图片成功", "Succ to Get blocklist picture");
        AfxMessageBox(szLan);
    }

    for (i=0; i<MAX_HUMAN_PICTURE_NUM;i++)
	{
		if (struBlockListPic.struBlockListPic[i].dwImageLen > 0 && struBlockListPic.struBlockListPic[i].pImage != NULL)
		{
			char cFilename[256] = {0};
			HANDLE hFile;
			DWORD dwReturn;
			
			SYSTEMTIME t;
			GetLocalTime(&t);
			
			sprintf(cFilename, "%s\\%s", g_struLocalParam.chPictureSavePath, g_struDeviceInfo[m_iDevIndex].chDeviceIP);
			if (GetFileAttributes(cFilename) != FILE_ATTRIBUTE_DIRECTORY)
			{
				CreateDirectory(cFilename, NULL);
			}
			
			sprintf(cFilename, "%s\\BlockList Pic[ID%d][No%d].jpg",cFilename, m_dwRegisterID,i);
			hFile = CreateFile(cFilename, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE)
			{
				return;
			}
			WriteFile(hFile, struBlockListPic.struBlockListPic[i].pImage, struBlockListPic.struBlockListPic[i].dwImageLen, &dwReturn, NULL);
			CloseHandle(hFile);
            hFile = NULL;
		}
	}

	for (i=0; i<MAX_HUMAN_PICTURE_NUM;i++)
	{
       
        delete []struBlockListPic.struBlockListPic[i].pImage;
		struBlockListPic.struBlockListPic[i].pImage = NULL;
	    delete []struBlockListPic.struBlockListPic[i].pModel;
		struBlockListPic.struBlockListPic[i].pModel = NULL;
	}

	UpdateData(FALSE);
}

void CDlgVcaBlockList::OnBtnFastScan() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	char szLan[128] = {0};
    OPENFILENAME ofn = {0};
	CRect struRect;
	
    memset(m_chFilename, 0, MAX_PATH);
    
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner   = this->GetSafeHwnd();
    ofn.lpstrFilter = "All Files\0*.*\0\0";
    ofn.lpstrFile   = m_chFilename;
    ofn.nMaxFile    = MAX_PATH;
    ofn.Flags       = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
    
    if (GetOpenFileName(&ofn))
    {
        m_csFastRegPicPath.Format("%s", m_chFilename);
    }
	
	if (strlen(m_chFilename) == 0)
	{
		return;
	}
	
	UpdateData(FALSE);
}

void CDlgVcaBlockList::OnBtnFastRegister() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szLan[128] = {0};
	CFile file;
	char *sFileBuf;
	BOOL bRet = TRUE;
	DWORD dwFileSize = 0;
	sFileBuf = NULL;
	if (!file.Open(m_chFilename, CFile::shareDenyNone))
	{
		file.Close();
		return;
	}
	file.Seek(0, CFile::begin);
	dwFileSize = (DWORD)(file.GetLength());
	if (dwFileSize == 0)
	{
		file.Close();
		return;
	}
    m_dwFileSize = dwFileSize;
	
	sFileBuf = new char[dwFileSize + 4];
	if (sFileBuf == NULL)
	{
		return;
	}
	file.Read(sFileBuf, dwFileSize);
	file.Close();
	
	m_struBlFastPara.pImage = new BYTE[m_dwFileSize];
	if (m_struBlFastPara.pImage == NULL)
	{
		return;
	}
	memset(m_struBlFastPara.pImage, 0, m_dwFileSize);
	memcpy(m_struBlFastPara.pImage, sFileBuf, m_dwFileSize);
	m_struBlFastPara.dwSize = sizeof(m_struBlFastPara);

	GetBlockListInfoFromWnd(m_struBlFastPara.struBlockListInfo);
	m_struBlFastPara.dwImageLen = m_dwFileSize;
	if (!NET_DVR_FastAddBlockList(m_lServerID, m_lChannel, &m_struBlFastPara))
	{
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_FastAddBlockList m_lChannel[%d] ",m_lChannel);
        g_StringLanType(szLan, "快速注册禁止名单失败", "Fail to register blocklist");
        AfxMessageBox(szLan);
    }
    else
    {
		g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_FastAddBlockList m_lChannel[%d] ",m_lChannel);
		g_StringLanType(szLan, "快速注册禁止名单成功", "Succ to register blocklist");
        AfxMessageBox(szLan);
    }

	delete []m_struBlFastPara.pImage;
	m_struBlFastPara.pImage = NULL;
}
