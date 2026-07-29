// DlgIntranetSegmentCfg.cpp : implementation file
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "DlgIntranetSegmentCfg.h"
#include "afxdialogex.h"


// CDlgIntranetSegmentCfg dialog

IMPLEMENT_DYNAMIC(CDlgIntranetSegmentCfg, CDialog)

CDlgIntranetSegmentCfg::CDlgIntranetSegmentCfg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgIntranetSegmentCfg::IDD, pParent)
{

}


void CDlgIntranetSegmentCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PHYSICAL_SEGMENT, m_comboPhysicalSegment);
	DDX_Control(pDX, IDC_COMBO_VIRTUAL_SEGMENT, m_comboVirtualSegment);
	DDX_Text(pDX, IDC_EDIT_PHYSICAL_SEGMENT, m_csPhysicalSegment);
	DDX_Text(pDX, IDC_EDIT_VIRTUAL_SEGMENT, m_csVirtualSegment);
}


BEGIN_MESSAGE_MAP(CDlgIntranetSegmentCfg, CDialog)
	ON_BN_CLICKED(ID_GET_CAPACITY, OnBnClickedGetCapacity)
	ON_BN_CLICKED(ID_GET_INTRNET, OnBnClickedGetIntrnet)
	ON_BN_CLICKED(ID_PUT_INTRNET, OnBnClickedPutIntrnet)
	ON_CBN_SELCHANGE(IDC_COMBO_PHYSICAL_SEGMENT, OnCbnSelchangeComboPhysicalSegment)
	ON_CBN_SELCHANGE(IDC_COMBO_VIRTUAL_SEGMENT, OnCbnSelchangeComboVirtualSegment)
END_MESSAGE_MAP()


BOOL CDlgIntranetSegmentCfg::OnInitDialog()
{
	RECT rect;
	CDialog::OnInitDialog();
	GetDlgItem(IDC_EDIT_PHYSICAL_SEGMENT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_VIRTUAL_SEGMENT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PHYSICAL_SEGMENT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_VIRTUAL_SEGMENT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_INTRANET_CAPACITY2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_PHYSICAL_SEGMENT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_VIRTUAL_SEGMENT)->ShowWindow(SW_HIDE);
	GetDlgItem(ID_GET_CAPACITY)->ShowWindow(SW_HIDE);
	GetDlgItem(ID_GET_CAPACITY)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	GetDlgItem(ID_GET_INTRNET)->MoveWindow(&rect);
	OnBnClickedGetCapacity();
	UpdateData(FALSE);
	return TRUE;
}


// CDlgIntranetSegmentCfg message handlers


void CDlgIntranetSegmentCfg::OnBnClickedGetCapacity()
{
	char szLan[128] = { 0 };
	NET_DVR_XML_CONFIG_INPUT    struInput = { 0 };
	NET_DVR_XML_CONFIG_OUTPUT   struOuput = { 0 };
	struInput.dwSize = sizeof(struInput);
	struOuput.dwSize = sizeof(struOuput);
	char szUrl[256] = { 0 };
	sprintf(szUrl, "GET /ISAPI/System/Network/intranetSegment/capabilities?format=json\r\n");
	struInput.lpRequestUrl = szUrl;
	struInput.dwRequestUrlLen = strlen(szUrl);
	DWORD dwOutputLen = 1024;
	char *pOutBuf = new char[dwOutputLen];
	memset(pOutBuf, 0, dwOutputLen);
	struOuput.lpOutBuffer = pOutBuf;
	struOuput.dwOutBufferSize = dwOutputLen;

	if (!NET_DVR_STDXMLConfig(m_lUserID, &struInput, &struOuput))
	{
		g_pMainDlg->AddLog(m_lUserID, OPERATION_FAIL_T, "NET_DVR_STDXMLConfig");
		if (pOutBuf != NULL)
		{
			delete[]pOutBuf;
			pOutBuf = NULL;
		}
		return;
	}
	else
	{
		g_pMainDlg->AddLog(m_lUserID, OPERATION_SUCC_T, "NET_DVR_STDXMLConfig");
	}
	/*std::string str = "{\"IntranetSegmentCap\": {\"ipv4PhysicalSegment\": {\"@opt\": ["
								"\"192.168.243.0-192.168.243.255\", \"192.168.244.0-192.168.244.255\", \"192.168.245.0-192.168.245.255\","
								"\"192.168.246.0-192.168.246.255\", \"192.168.247.0-192.168.247.255\", \"192.168.248.0-192.168.248.255\","
								"\"192.168.249.0-192.168.249.255\", \"192.168.250.0-192.168.250.255\", \"192.168.251.0-192.168.251.255\","
								"\"192.168.252.0-192.168.252.255\", \"192.168.253.0-192.168.253.255\"]},"
							"\"ipv4VirtualSegment\" : {\"@opt\": ["
								"\"192.168.243.0-192.168.243.255\", \"192.168.244.0-192.168.244.255\", \"192.168.245.0-192.168.245.255\","
								"\"192.168.246.0-192.168.246.255\", \"192.168.247.0-192.168.247.255\", \"192.168.248.0-192.168.248.255\","
								"\"192.168.249.0-192.168.249.255\", \"192.168.250.0-192.168.250.255\", \"192.168.251.0-192.168.251.255\","
								"\"192.168.252.0-192.168.252.255\", \"192.168.253.0-192.168.253.255\"]}}}";*/
	m_comboPhysicalSegment.ResetContent();
	m_comboVirtualSegment.ResetContent();

	cJSON* pRoot = cJSON_Parse(pOutBuf);
	cJSON *pSegmentCap = cJSON_GetObjectItem(pRoot, "IntranetSegmentCap");
	if (pSegmentCap != NULL)
	{
		cJSON *pPhysicalSegment = cJSON_GetObjectItem(pSegmentCap, "ipv4PhysicalSegment");
		if (pPhysicalSegment != NULL)
		{
			cJSON *pPhysicalOpt = cJSON_GetObjectItem(pPhysicalSegment, "@opt");
			int iSize = cJSON_GetArraySize(pPhysicalOpt);
			for (int i = 0; i < iSize; ++i)
			{
				cJSON* pPhy = cJSON_GetArrayItem(pPhysicalOpt, i);
				m_comboPhysicalSegment.InsertString(i, pPhy->valuestring);
			}
			m_comboPhysicalSegment.SetCurSel(0);
		}

		cJSON *pVirtualSegment = cJSON_GetObjectItem(pSegmentCap, "ipv4VirtualSegment");
		if (pVirtualSegment != NULL)
		{
			cJSON *pVirtualOpt = cJSON_GetObjectItem(pVirtualSegment, "@opt");
			int iSize = cJSON_GetArraySize(pVirtualOpt);
			for (int i = 0; i < iSize; ++i)
			{
				cJSON* pVir = cJSON_GetArrayItem(pVirtualOpt, i);
				m_comboVirtualSegment.InsertString(i, pVir->valuestring);
			}
			m_comboVirtualSegment.SetCurSel(0);
		}
	}
	if (pOutBuf != NULL)
	{
		delete[]pOutBuf;
		pOutBuf = NULL;
	}
}


void CDlgIntranetSegmentCfg::OnBnClickedGetIntrnet()
{
	char szLan[128] = { 0 };
	NET_DVR_XML_CONFIG_INPUT    struInput = { 0 };
	NET_DVR_XML_CONFIG_OUTPUT   struOuput = { 0 };
	struInput.dwSize = sizeof(struInput);
	struOuput.dwSize = sizeof(struOuput);
	char szUrl[256] = { 0 };
	sprintf(szUrl, "GET /ISAPI/System/Network/intranetSegment?format=json\r\n");
	struInput.lpRequestUrl = szUrl;
	struInput.dwRequestUrlLen = strlen(szUrl);
	DWORD dwOutputLen = 1024;
	char *pOutBuf = new char[dwOutputLen];
	memset(pOutBuf, 0, dwOutputLen);
	struOuput.lpOutBuffer = pOutBuf;
	struOuput.dwOutBufferSize = dwOutputLen;

	if (!NET_DVR_STDXMLConfig(m_lUserID, &struInput, &struOuput))
	{
		g_pMainDlg->AddLog(m_lUserID, OPERATION_FAIL_T, "NET_DVR_STDXMLConfig");
		g_StringLanType(szLan, "获取失败", "Get Failed");
		AfxMessageBox(szLan, MB_ICONERROR);
		if (pOutBuf != NULL)
		{
			delete[]pOutBuf;
			pOutBuf = NULL;
		}
		return;
	}
	else
	{
		g_pMainDlg->AddLog(m_lUserID, OPERATION_SUCC_T, "NET_DVR_STDXMLConfig");
	}
	/*std::string str = "{\"IntranetSegment\": {\"ipv4PhysicalSegment\":  \"192.168.243.0-192.168.243.255\","
			"\"ipv4VirtualSegment\" : \"192.168.244.0-192.168.244.255\"}}";*/

	cJSON* pRoot = cJSON_Parse(pOutBuf);
	cJSON *pSegmentCap = cJSON_GetObjectItem(pRoot, "IntranetSegment");
	if (pSegmentCap != NULL)
	{
		CString str;
		cJSON *pPhysicalSegment = cJSON_GetObjectItem(pSegmentCap, "ipv4PhysicalSegment");
		cJSON *pVirtualSegment = cJSON_GetObjectItem(pSegmentCap, "ipv4VirtualSegment");
		if (pPhysicalSegment != NULL)
		{
			for (int i = 0; i < m_comboPhysicalSegment.GetCount(); i++)
			{
				m_comboPhysicalSegment.GetLBText(i, str);
				if (str == pPhysicalSegment->valuestring)
				{
					m_comboPhysicalSegment.SetCurSel(i);
					break;
				}
			}
		}
		if (pVirtualSegment != NULL)
		{
			for (int i = 0; i < m_comboVirtualSegment.GetCount(); i++)
			{
				m_comboVirtualSegment.GetLBText(i, str);
				if (str == pVirtualSegment->valuestring)
				{
					m_comboVirtualSegment.SetCurSel(i);
					break;
				}
			}
		}
	}

	if (pOutBuf != NULL)
	{
		delete[]pOutBuf;
		pOutBuf = NULL;
	}
	UpdateData(TRUE);
}


void CDlgIntranetSegmentCfg::OnBnClickedPutIntrnet()
{
	char szLan[128] = { 0 };
	if (m_comboPhysicalSegment.GetCurSel() == -1 && m_comboVirtualSegment.GetCurSel() == -1)
	{
		return;
	}
	CString str;
	cJSON *root = cJSON_CreateObject();
	cJSON *pTemp = cJSON_CreateObject();
	if (m_comboPhysicalSegment.GetCurSel() != -1)
	{
		m_comboPhysicalSegment.GetLBText(m_comboPhysicalSegment.GetCurSel(), str);
		cJSON_AddStringToObject(pTemp, "ipv4PhysicalSegment", str);
	}
	if (m_comboVirtualSegment.GetCurSel() != -1)
	{
		m_comboVirtualSegment.GetLBText(m_comboVirtualSegment.GetCurSel(), str);
		cJSON_AddStringToObject(pTemp, "ipv4VirtualSegment", str);
	}
	cJSON_AddItemToObject(root, "IntranetSegment", pTemp);

	NET_DVR_XML_CONFIG_INPUT    struInput = { 0 };
	NET_DVR_XML_CONFIG_OUTPUT   struOuput = { 0 };
	struInput.dwSize = sizeof(struInput);
	struOuput.dwSize = sizeof(struOuput);
	char szUrl[256] = { 0 };
	char *pBuf = cJSON_Print(root);
	sprintf(szUrl, "PUT /ISAPI/System/Network/intranetSegment?format=json\r\n");
	struInput.lpRequestUrl = szUrl;
	struInput.dwRequestUrlLen = strlen(szUrl);
	struInput.lpInBuffer = pBuf;
	struInput.dwInBufferSize = strlen(pBuf);

	DWORD dwOutputLen = 1024;
	char *pOutBuf = new char[dwOutputLen];
	memset(pOutBuf, 0, dwOutputLen);
	struOuput.lpOutBuffer = pOutBuf;
	struOuput.dwOutBufferSize = dwOutputLen;
	if (!NET_DVR_STDXMLConfig(m_lUserID, &struInput, &struOuput))
	{
		g_pMainDlg->AddLog(m_lUserID, OPERATION_FAIL_T, "NET_DVR_STDXMLConfig");
		g_StringLanType(szLan, "设置失败", "Set Failed");
		AfxMessageBox(szLan, MB_ICONERROR);
	}
	else
	{
		cJSON* pRoot = cJSON_Parse(pOutBuf);
		cJSON *pStatusString = cJSON_GetObjectItem(pRoot, "statusString");
		if (pStatusString != NULL)
		{
			CString status = pStatusString->valuestring;
			if (status == "OK")
			{
				//重复设备，直接返回成功
				g_StringLanType(szLan, "设置成功", "Set Success");
				AfxMessageBox(szLan, MB_OK);
			}
			else if (status == "Reboot Required")
			{
				//新设置的值，设备需重启
				g_StringLanType(szLan, "设置成功", "Set Success");
				AfxMessageBox(szLan, MB_OK);
				OnReboot();
			}
			else
			{
				g_StringLanType(szLan, "设置失败", "Set Failed");
				strcat(szLan, ", statusString: ");
				strcat(szLan, status);
				AfxMessageBox(szLan, MB_ICONERROR);
			}
		}
		else
		{
			g_StringLanType(szLan, "设置失败, 未返回状态信息", "Set Failed, no statusString");
			AfxMessageBox(szLan, MB_ICONERROR);
		}
	}

	if (pOutBuf != NULL)
	{
		delete[]pOutBuf;
		pOutBuf = NULL;
	}
}

// 网段设置时，物理网段与虚拟网段需保持能力序号一致（如物理网段选择第n个，虚拟网段也选择第n个）
void CDlgIntranetSegmentCfg::OnCbnSelchangeComboPhysicalSegment()
{
	if (m_comboVirtualSegment.GetCount() > 0)
	{
		m_comboVirtualSegment.SetCurSel(m_comboPhysicalSegment.GetCurSel());
	}
}

void CDlgIntranetSegmentCfg::OnCbnSelchangeComboVirtualSegment()
{
	if (m_comboPhysicalSegment.GetCount() > 0)
	{
		m_comboPhysicalSegment.SetCurSel(m_comboVirtualSegment.GetCurSel());
	}
}

void CDlgIntranetSegmentCfg::OnReboot()
{
	char szLan[128] = { 0 };

	if (NET_DVR_RebootDVR(m_lUserID))
	{
		g_pMainDlg->AddLog(m_lUserID, OPERATION_SUCC_T, "NET_DVR_RebootDVR");
		g_StringLanType(szLan, "设备正在重启", "Device is rebooting");
		AfxMessageBox(szLan);
	}
	else
	{
		g_pMainDlg->AddLog(m_lUserID, OPERATION_FAIL_T, "NET_DVR_RebootDVR");
		g_StringLanType(szLan, "设备重启失败", "Reboot device failed");
		AfxMessageBox(szLan);
	}
}
