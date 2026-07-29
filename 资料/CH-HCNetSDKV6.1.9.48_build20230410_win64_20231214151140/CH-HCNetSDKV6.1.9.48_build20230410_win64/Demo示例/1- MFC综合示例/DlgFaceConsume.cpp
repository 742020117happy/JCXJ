// DlgFaceConsume.cpp : 实现文件
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "DlgFaceConsume.h"
#include "afxdialogex.h"
#include "DlgConsumeSimulate.h"

// CDlgFaceConsume 对话框

IMPLEMENT_DYNAMIC(CDlgFaceConsume, CDialogEx)

CDlgFaceConsume::CDlgFaceConsume(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgFaceConsume::IDD, pParent)
{

}

CDlgFaceConsume::~CDlgFaceConsume()
{
}

void CDlgFaceConsume::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgFaceConsume, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_CONSUME_SIMULATE, &CDlgFaceConsume::OnBnClickedBtnConsumeSimulate)
END_MESSAGE_MAP()


// CDlgFaceConsume 消息处理程序


void CDlgFaceConsume::OnBnClickedBtnConsumeSimulate()
{
    // TODO:  在此添加控件通知处理程序代码
    CDlgConsumeSimulate dlg;
    g_pDlgConsumeSimulate = &dlg;
    dlg.m_lUserID = m_lUserID;
    dlg.m_iDeviceIndex = m_iDeviceIndex;
    dlg.DoModal();
    g_pDlgConsumeSimulate = NULL;
}
