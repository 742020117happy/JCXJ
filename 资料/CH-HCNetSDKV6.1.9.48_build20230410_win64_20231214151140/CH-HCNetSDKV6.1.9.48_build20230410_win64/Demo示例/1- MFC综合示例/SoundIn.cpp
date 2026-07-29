
// SoundIn.cpp: implementation of the CSoundIn class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SoundIn.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSoundIn::CSoundIn()
{
	m_pWaveHead = NULL;
	m_QueuedBuffers = 0;
	m_hRecord = NULL;
	m_bRecording = FALSE;
	DataFromSoundIn = NULL;
    m_hSoundIn = INVALID_HANDLE_VALUE;
    m_dwThreadID = 0;
    m_pUser = NULL;
}

CSoundIn::~CSoundIn()
{
	if (m_bRecording)
	{
		Stop();
	}
}


BOOL CSoundIn::Start(WAVEFORMATEX* format, DWORD nBufNum, DWORD nBufSize)
{
	if (m_bRecording || DataFromSoundIn == NULL)
	{
		TRACE("m_bRecording || DataFromSoundIn == NULL:m_bRecording:%d", m_bRecording);
		return FALSE;
	}

	MMRESULT mmReturn = 0;
	int i = 0;
	
	m_nBufSize = nBufSize;
	m_nBufNum = nBufNum;
	
	if (format != NULL)
	{
		m_Format = *format;
	}

    m_bRecording = TRUE;
    m_hSoundIn = CreateThread(NULL, 0, SoundInDataThread, this, 0, &m_dwThreadID);
    if (INVALID_HANDLE_VALUE == m_hSoundIn)
    {
        return FALSE;
    }

    //这里要通过线程方式，回调函数方式容易导致死锁
    mmReturn = ::waveInOpen(&m_hRecord, WAVE_MAPPER, &m_Format, (DWORD_PTR)m_dwThreadID, (DWORD_PTR)this, CALLBACK_TASK);
    if (mmReturn)
    {
        m_bRecording = FALSE;
        return FALSE;
    }
	
	m_pWaveHead = new WAVEHDR[m_nBufNum];
	if (m_pWaveHead == NULL)
	{
        return FALSE;
	}

	for (i=0; i<(INT)m_nBufNum; i++)
	{
		ZeroMemory(&m_pWaveHead[i], sizeof(WAVEHDR));
		m_pWaveHead[i].lpData = new char [m_nBufSize];
		m_pWaveHead[i].dwBufferLength = m_nBufSize;
		AddInputBufferToQueue(&m_pWaveHead[i]);
	}
	
	mmReturn = ::waveInStart(m_hRecord);
	if (mmReturn)
	{
		waveInErrorMsg(mmReturn, "in Start()");
		return FALSE;
	}

	return TRUE;
}

void CSoundIn::Stop()
{
    if (INVALID_HANDLE_VALUE != m_hSoundIn)
    {
        DWORD dwWaitResult = WAIT_TIMEOUT;
        while (true)
        {
            PostThreadMessage(m_dwThreadID, 0, NULL, NULL);
            dwWaitResult = WaitForSingleObject(m_hSoundIn, 100); //等待线程退出
            if (WAIT_OBJECT_0 == dwWaitResult || WAIT_FAILED == dwWaitResult)
            {
                break;
            }
        }
        CloseHandle(m_hSoundIn);
        m_hSoundIn = INVALID_HANDLE_VALUE;
    }

    MMRESULT mmReturn = MMSYSERR_NOERROR;
    int i = 0;
    if (m_bRecording)
    {
        ::waveInStop(m_hRecord);
        ::waveInReset(m_hRecord);

		if (m_pWaveHead)
		{
			for (i=0; i<(INT)m_nBufNum; i++)
			{				
				mmReturn = ::waveInUnprepareHeader(m_hRecord, &m_pWaveHead[i], sizeof(WAVEHDR));
				if (mmReturn)
				{
					TRACE("waveInUnprepareHeader failed:%d", mmReturn);
				}				
				delete[] m_pWaveHead[i].lpData;
				m_QueuedBuffers--;
			}
			delete[] m_pWaveHead;
			m_pWaveHead=NULL;
		}
		mmReturn = ::waveInClose(m_hRecord);
		if (mmReturn)
		{
			TRACE("waveInClose failed:%d:%d", mmReturn, m_QueuedBuffers);
			waveInErrorMsg(mmReturn, "in Stop()");
		}
	}

    m_bRecording = FALSE;
}


void CALLBACK CSoundIn::waveInProc(HWAVEOUT hwo, UINT uMsg, void * pInstance, void *pParam1, void *pParam2)
{
    ASSERT(pInstance);
    CSoundIn*pOwner = (CSoundIn*)pInstance;
    if (!pOwner->m_bRecording)
    {
        return;
    }

    MMRESULT mmReturn = 0;
    LPWAVEHDR pHdr = (LPWAVEHDR)pParam1;

    switch (uMsg)
    {
    case WIM_DATA:
        //TRACE("waveInProc[%d]\n", pHdr->dwBytesRecorded);
        (pOwner->DataFromSoundIn)(pHdr->lpData, pHdr->dwBytesRecorded, pOwner->m_pUser);

        if (pOwner->m_bRecording)
        {
            // add the input buffer to the queue again
            mmReturn = ::waveInAddBuffer(pOwner->m_hRecord, pHdr, sizeof(WAVEHDR));
            if (mmReturn)
            {
                waveInErrorMsg(mmReturn, "in OnWIM_DATA()");
            }
        }
        break;
    default:
        TRACE("other msg\n");
        break;
    }
}

DWORD WINAPI CSoundIn::SoundInDataThread(LPVOID pUserData)
{
    CSoundIn *pThis = (CSoundIn *)pUserData;
    //OutputDebugString("语音输入线程进入\n");
    while (TRUE)
    {
        MSG struMsg = { 0 };
        if (GetMessage(&struMsg, NULL, 0, 0))
        {
            if (MM_WIM_DATA == struMsg.message)
            {
                WAVEHDR *pStruHdr = (WAVEHDR *)struMsg.lParam;
                (pThis->DataFromSoundIn)(pStruHdr->lpData, pStruHdr->dwBytesRecorded, pThis->m_pUser);

                if (pThis->m_bRecording)
                {
                    // add the input buffer to the queue again
                    MMRESULT mmReturn = ::waveInAddBuffer(pThis->m_hRecord, pStruHdr, sizeof(WAVEHDR));
                    if (mmReturn)
                    {
                        waveInErrorMsg(mmReturn, "in OnWIM_DATA()");
                    }
                }
            }
            else if (MM_WIM_CLOSE == struMsg.message)
            {
                break;
            }
            else if (0 == struMsg.message) //退出消息
            {
                break;
            }
            else
            {
                continue;
            }
        }
        else
        {
            break;
        }
    }
    //OutputDebugString("语音输入线程退出\n");
    return 0;
}

int CSoundIn::AddInputBufferToQueue(WAVEHDR *pHdr)
{
	MMRESULT mmReturn = 0;

	// prepare it
	mmReturn = ::waveInPrepareHeader(m_hRecord, pHdr, sizeof(WAVEHDR));
	if (mmReturn)
	{
		waveInErrorMsg(mmReturn, "in AddInputBufferToQueue()");
		return m_QueuedBuffers;
	}
	
	// add the input buffer to the queue
	mmReturn = ::waveInAddBuffer(m_hRecord, pHdr, sizeof(WAVEHDR));
	if (mmReturn)
	{
		waveInErrorMsg(mmReturn, "in AddInputBufferToQueue()");
		return m_QueuedBuffers;
	}
	// no error
	// increment the number of waiting buffers
	return ++m_QueuedBuffers;
}

void CSoundIn::waveInErrorMsg(MMRESULT result, LPCTSTR addstr)
{
	char errorbuffer[100];
	waveInGetErrorText(result, errorbuffer,100);
}

void CSoundIn::SetSoundInDataCB(void (CALLBACK *SoundInCB)(char*, DWORD, void *), void * pUser)
{
    DataFromSoundIn = SoundInCB;
    m_pUser = pUser;
}