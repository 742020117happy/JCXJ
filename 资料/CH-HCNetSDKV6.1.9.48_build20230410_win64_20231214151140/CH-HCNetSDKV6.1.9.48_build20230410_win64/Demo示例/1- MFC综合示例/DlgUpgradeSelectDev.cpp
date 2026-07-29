// DlgUpgradeSelectDev.cpp : 实现文件
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "DlgUpgradeSelectDev.h"
#include "DlgUpgrade.h"
#include "afxdialogex.h"


// CDlgUpgradeSelectDev 对话框

IMPLEMENT_DYNAMIC(CDlgUpgradeSelectDev, CDialog)

CDlgUpgradeSelectDev::CDlgUpgradeSelectDev(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUpgradeSelectDev::IDD, pParent)
    , m_bHost(FALSE)
    , m_bLamp(FALSE)
    , m_bKey(FALSE)
    , m_bKeyBoard(FALSE)
    , m_bSwipeCard(FALSE)
    , m_bDisplay(FALSE)
{

}

CDlgUpgradeSelectDev::~CDlgUpgradeSelectDev()
{
}

BOOL CDlgUpgradeSelectDev::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  在此添加额外的初始化
    UpdateData(TRUE);
    m_dwAcsNo = 0;
    return FALSE;
}

void CDlgUpgradeSelectDev::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Check(pDX, IDC_CHK_UPGRADE_HOST, m_bHost);
    DDX_Check(pDX, IDC_CHK_UPGRADE_LAMP, m_bLamp);
    DDX_Check(pDX, IDC_CHK_UPGARDE_KEY, m_bKey);
    DDX_Check(pDX, IDC_CHK_UPGRADE_KEYBOARD, m_bKeyBoard);
    DDX_Check(pDX, IDC_CHK_URGRADE_CARD, m_bSwipeCard);
    DDX_Check(pDX, IDC_CHK_UPGRADE_DISPLAY, m_bDisplay);
}


BEGIN_MESSAGE_MAP(CDlgUpgradeSelectDev, CDialog)
    ON_BN_CLICKED(IDC_BTN_EXIT, &CDlgUpgradeSelectDev::OnBnClickedBtnExit)
    ON_BN_CLICKED(IDC_BTN_OK, &CDlgUpgradeSelectDev::OnBnClickedBtnOk)
    ON_BN_CLICKED(IDC_CHK_UPGRADE_HOST, &CDlgUpgradeSelectDev::OnBnClickedChkUpgradeHost)
    ON_BN_CLICKED(IDC_CHK_UPGRADE_DISPLAY, &CDlgUpgradeSelectDev::OnBnClickedChkUpgradeDisplay)
    ON_BN_CLICKED(IDC_CHK_UPGRADE_LAMP, &CDlgUpgradeSelectDev::OnBnClickedChkUpgradeLamp)
    ON_BN_CLICKED(IDC_CHK_UPGARDE_KEY, &CDlgUpgradeSelectDev::OnBnClickedChkUpgardeKey)
    ON_BN_CLICKED(IDC_CHK_UPGRADE_KEYBOARD, &CDlgUpgradeSelectDev::OnBnClickedChkUpgradeKeyboard)
    ON_BN_CLICKED(IDC_CHK_URGRADE_CARD, &CDlgUpgradeSelectDev::OnBnClickedChkUrgradeCard)
END_MESSAGE_MAP()


// CDlgUpgradeSelectDev 消息处理程序


void CDlgUpgradeSelectDev::OnBnClickedBtnExit()
{
    // TODO:  在此添加控件通知处理程序代码
    CDialog::OnCancel();
}

void CDlgUpgradeSelectDev::OnBnClickedBtnOk()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    if (m_bHost)
    {
        m_dwAcsNo = 0;
    }
    if (m_bLamp)
    {
        m_dwAcsNo |= 0x10100;
    }
    if (m_bKey)
    {
        m_dwAcsNo |= 0x10200;
    }
    if (m_bKeyBoard)
    {
        m_dwAcsNo |= 0x10400;
    }
    if (m_bSwipeCard)
    {
        m_dwAcsNo |= 0x10800;
    }
    if (m_bDisplay)
    {
        m_dwAcsNo |= 0x20000;
    }
    CDlgUpgrade *pDlg = (CDlgUpgrade *)this->GetParent();
    pDlg->m_dwAcsNo = m_dwAcsNo;
    CDialog::OnOK();
}

void CDlgUpgradeSelectDev::OnBnClickedChkUpgradeHost()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    if (m_bHost)
    {
        m_bDisplay = FALSE;
        m_bKey = FALSE;
        m_bKeyBoard = FALSE;
        m_bLamp = FALSE;
        m_bSwipeCard = FALSE;
    }
    UpdateData(FALSE);
}


void CDlgUpgradeSelectDev::OnBnClickedChkUpgradeDisplay()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    if (m_bDisplay)
    {
        m_bHost = FALSE;
        m_bKey = FALSE;
        m_bKeyBoard = FALSE;
        m_bLamp = FALSE;
        m_bSwipeCard = FALSE;
    }
    UpdateData(FALSE);
}


void CDlgUpgradeSelectDev::OnBnClickedChkUpgradeLamp()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    if (m_bLamp)
    {
        m_bHost = FALSE;
        m_bDisplay = FALSE;
    }
    UpdateData(FALSE);
}


void CDlgUpgradeSelectDev::OnBnClickedChkUpgardeKey()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    if (m_bKey)
    {
        m_bHost = FALSE;
        m_bDisplay = FALSE;
    }
    UpdateData(FALSE);
}


void CDlgUpgradeSelectDev::OnBnClickedChkUpgradeKeyboard()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    if (m_bKeyBoard)
    {
        m_bHost = FALSE;
        m_bDisplay = FALSE;
    }
    UpdateData(FALSE);
}


void CDlgUpgradeSelectDev::OnBnClickedChkUrgradeCard()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    if (m_bSwipeCard)
    {
        m_bHost = FALSE;
        m_bDisplay = FALSE;
    }
    UpdateData(FALSE);
}
