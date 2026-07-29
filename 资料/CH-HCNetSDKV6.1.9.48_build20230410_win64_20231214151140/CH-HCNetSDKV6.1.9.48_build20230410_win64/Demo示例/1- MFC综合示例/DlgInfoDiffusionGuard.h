#pragma once


// CDlgInfoDiffusionGuard 对话框

void CALLBACK RemoteConfigCallback(DWORD dwType, void* lpBuffer, DWORD dwBufLen, void* pUserData);

class CDlgInfoDiffusionGuard : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgInfoDiffusionGuard)

public:
	CDlgInfoDiffusionGuard(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgInfoDiffusionGuard();
    virtual BOOL OnInitDialog();
// 对话框数据
	enum { IDD = IDD_DLG_INFO_DIFFUSION_GAURD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    int m_dwCommand;
    int m_dwLinkHandle;
    CString m_strInbuffer1;
    int m_dwDataType;
    CString m_strInbuffer2;
    LONG m_lUserID;
    int m_iDeviceIndex;
    void processLinkData(DWORD dwType, void* lpBuffer, DWORD dwBufLen);
    afx_msg void OnBnClickedBtnStartlink();
    afx_msg void OnBnClickedBtnSenddata();
    afx_msg void OnBnClickedBtnCloselink();
    afx_msg LRESULT OnMsgUpdateOutBuffer(WPARAM wParam, LPARAM lParam);
    CString m_strOutBuffer;
    //CString m_strStatus;
    bool m_bStatusIndex;
    bool m_bDataIndex;
};
