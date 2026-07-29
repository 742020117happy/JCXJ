
// SoundIn.h: interface for the CSoundIn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOUNDIN_H__BE5802CB_02A3_4784_90D5_5C32B23055A8__INCLUDED_)
#define AFX_SOUNDIN_H__BE5802CB_02A3_4784_90D5_5C32B23055A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <mmsystem.h>

class CSoundIn  
{
public:
	CSoundIn();
	virtual ~CSoundIn();
	
	virtual void Stop();
	virtual BOOL Start(WAVEFORMATEX* format,DWORD nBufNum,DWORD nBufSize);
    void SetSoundInDataCB(void(__stdcall * DataFromSoundIn)(char* pBuf, DWORD nSize, void *pUser), void *pUser);

	static void waveInErrorMsg(MMRESULT result, LPCTSTR addstr);	
    static void CALLBACK waveInProc(HWAVEOUT hwo, UINT uMsg, void *pInstance, void *pParam1, void *pParam2);
    static DWORD WINAPI SoundInDataThread(LPVOID pUserData);

protected:
	int AddInputBufferToQueue(WAVEHDR *pHdr);
	
protected:	
	WAVEHDR* m_pWaveHead;
	BOOL m_bRecording;
	HWAVEIN m_hRecord;
	int m_QueuedBuffers;
	WAVEFORMATEX m_Format;
	
	DWORD m_nBufNum;
	DWORD m_nBufSize;	
    HANDLE m_hSoundIn;
    DWORD m_dwThreadID;
	// pointer to callback function
    void (CALLBACK *DataFromSoundIn)(char* pBuf, DWORD nSize, void *pUser);
    void *m_pUser;
};

#endif // !defined(AFX_SOUNDIN_H__BE5802CB_02A3_4784_90D5_5C32B23055A8__INCLUDED_)
