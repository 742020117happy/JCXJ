#pragma once


// CDlgFaceConsume 对话框

class CDlgFaceConsume : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFaceConsume)

public:
	CDlgFaceConsume(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgFaceConsume();
    LONG m_lUserID;
    int m_iDeviceIndex;
// 对话框数据
	enum { IDD = IDD_DLG_FACE_CONSUME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnConsumeSimulate();
};
