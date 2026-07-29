package VideoCalll;

import NetSDKDemo.HCNetSDK;
import NetSDKDemo.PlayCtrl;
import com.sun.jna.Native;
import com.sun.jna.Pointer;
import com.sun.jna.Union;
import com.sun.jna.examples.win32.W32API;
import com.sun.jna.ptr.ByteByReference;
import com.sun.jna.ptr.IntByReference;

import javax.swing.*;
import java.awt.*;
import java.io.*;
import java.nio.ByteBuffer;
import java.sql.SQLOutput;
import java.text.BreakIterator;

/**
 * @author jiangxin
 * @create 2022-11-09-11:21
 */
public class IndoorToClient {


    static ViedoCallSingalCallBacak videoCallSingalCallback = null;
    static FRealDataCallBack fRealDataCallBack;//预览回调函数实现
    static VoiceDataCallBack voiceDatacallback = null;
    static int cmdType; //请求类型
    static int lPlay; //预览句柄
    static int videocallHandle = -1; //可视对讲长连接句柄
    static int lVoiceComHandle = -1; //语音对讲句柄
    static int lVoiceTransHandle = -1; //语音转发句柄
    static File file = null;
    static File filePcm = null;
    static FileOutputStream outputStream = null;
    static FileOutputStream outputStreamPcm = null;

    static File fileEncode = null;
    static FileOutputStream outputStreamG711 = null;
    static Pointer pDecHandle = null;
    static cbVoiceDataCallBack_MR_V30 cbVoiceDataCallBack = null;
    static IntByReference m_lPort = new IntByReference(-1);
    public static JFrame jf = new JFrame("预览窗口");
    public static JFrame jFrameBtn = new JFrame("操作窗口");
    public static java.awt.Panel panelRealplay;


    //可视对讲回调函数
    class ViedoCallSingalCallBacak implements HCNetSDK.FRemoteConfigCallBack {

        public void invoke(int dwType, Pointer pBuffer, int dwBufLen, Pointer pUser) {
            switch (dwType) {
                case 0:
                    HCNetSDK.REMOTECONFIGSTATUS struCfgStatus = new HCNetSDK.REMOTECONFIGSTATUS();
                    struCfgStatus.write();
                    Pointer pCfgStatus = struCfgStatus.getPointer();
                    pCfgStatus.write(0, pBuffer.getByteArray(0, struCfgStatus.size()), 0, struCfgStatus.size());
                    struCfgStatus.read();

                    int iStatus = 0;
                    for (int i = 0; i < 4; i++) {
                        int ioffset = i * 8;
                        int iByte = struCfgStatus.byStatus[i] & 0xff;
                        iStatus = iStatus + (iByte << ioffset);
                    }

                    switch (iStatus) {
                        case 1000:// NET_SDK_CALLBACK_STATUS_SUCCESS
                            System.out.println("可视对讲回调成功,dwStatus:" + iStatus);
                            break;
                        case 1001:
                            System.out.println("正在获取可视对讲命令回调数据中,dwStatus:" + iStatus);
                            break;
                        case 1002:
                            int iErrorCode = 0;
                            for (int i = 0; i < 4; i++) {
                                int ioffset = i * 8;
                                int iByte = struCfgStatus.byErrorCode[i] & 0xff;
                                iErrorCode = iErrorCode + (iByte << ioffset);
                            }
                            System.out.println("获取可视对讲命令数据失败, dwStatus:" + iStatus + "错误号:" + iErrorCode);
                            break;
                    }

                    break;
                case 1:
                    break;
                case 2:
                    HCNetSDK.NET_DVR_VIDEO_CALL_PARAM m_struVideoCallParm = new HCNetSDK.NET_DVR_VIDEO_CALL_PARAM();
                    m_struVideoCallParm.write();
                    Pointer pInfoV30 = m_struVideoCallParm.getPointer();
                    pInfoV30.write(0, pBuffer.getByteArray(0, m_struVideoCallParm.size()), 0, m_struVideoCallParm.size());
                    m_struVideoCallParm.read();
                    cmdType = m_struVideoCallParm.dwCmdType;
                    System.out.println("信令类型:" + cmdType);
                    //0- 请求呼叫
                    if (cmdType == 0) {
                        JOptionPane.showMessageDialog(null, "设备呼叫中，请点击接听！");
                    }
                    //5- 结束本次通话
                    if (cmdType == 5) {
                        //停止预览和语音对讲(转发)
                        if (lPlay >= 0) {
                            stopPlay();
                        }
                        if (lVoiceComHandle >= 0) {
                            stopVoiceCom(lVoiceComHandle);
                        }
                        if (lVoiceTransHandle >= 0) {
                            SendAudioRunnable.flag =false;
                            stopVoiceG711Trans(lVoiceTransHandle);
                        }
                        JPanelDemo.flashWindows();
                    }

                    break;
                default:
                    break;

            }

        }


    }

    static class FRealDataCallBack implements HCNetSDK.FRealDataCallBack_V30 {
        //预览回调
        public void invoke(int lRealHandle, int dwDataType, Pointer pBuffer, int dwBufSize, Pointer pUser) {
//            System.out.println("码流数据回调...dwBufSize="+dwBufSize);
            //播放库解码
            switch (dwDataType) {
                case HCNetSDK.NET_DVR_SYSHEAD: //系统头
                    if (!ViedoCallMain.playCtrl.PlayM4_GetPort(m_lPort)) //获取播放库未使用的通道号
                    {
                        break;
                    }
                    if (dwBufSize > 0) {
                        if (!ViedoCallMain.playCtrl.PlayM4_SetStreamOpenMode(m_lPort.getValue(), PlayCtrl.STREAME_REALTIME))  //设置实时流播放模式
                        {
                            break;
                        }
                        if (!ViedoCallMain.playCtrl.PlayM4_OpenStream(m_lPort.getValue(), pBuffer, dwBufSize, 1024 * 1024)) //打开流接口
                        {
                            break;
                        }
                        W32API.HWND hwnd = new W32API.HWND(Native.getComponentPointer(panelRealplay));
                        if (!ViedoCallMain.playCtrl.PlayM4_Play(m_lPort.getValue(), hwnd)) //播放开始
                        {
                            break;
                        }

                    }
                case HCNetSDK.NET_DVR_STREAMDATA:   //码流数据
                    if ((dwBufSize > 0) && (m_lPort.getValue() != -1)) {
                        if (!ViedoCallMain.playCtrl.PlayM4_InputData(m_lPort.getValue(), pBuffer, dwBufSize))  //输入流数据
                        {
                            break;
                        }
                    }
            }
        }
    }


    static class VoiceDataCallBack implements HCNetSDK.FVoiceDataCallBack_V30 {
        @Override
        public void invoke(int lVoiceComHandle, Pointer pRecvDataBuffer, int dwBufSize, byte byAudioFlag, int pUser) {
            //回调函数里保存语音对讲中双方通话语音数据
            System.out.println("语音对讲数据回调....");
        }
    }


    static class cbVoiceDataCallBack_MR_V30 implements HCNetSDK.FVoiceDataCallback_MR_V30 {
        public void invoke(int lVoiceComHandle, Pointer pRecvDataBuffer, int dwBufSize, byte byAudioFlag, Pointer pUser) {
            //语音回调函数，实现的是接收设备那边传过来的音频数据（g711编码），如果只需要平台发送音频到设备，不需要接收设备发送的音频，
            // 回调函数里什么都不实现
            //不影响业务功能
//            System.out.println("-----=cbVoiceDataCallBack_MR_V30 enter---------");
            if (byAudioFlag == 1) {
                System.out.println("设备发过来的语音");
                //设备发送过来的语音数据
                try {
                    //将设备发送过来的语音数据写入文件
                    long offset = 0;
                    ByteBuffer buffers = pRecvDataBuffer.getByteBuffer(offset, dwBufSize);
                    byte[] bytes = new byte[dwBufSize];
                    buffers.rewind();
                    buffers.get(bytes);
                    outputStream.write(bytes);  //这里实现的是将设备发送的g711音频数据写入文件
                    //解码
                    if (pDecHandle == null) {
                        pDecHandle = ViedoCallMain.hCNetSDK.NET_DVR_InitG711Decoder();
                    }
                    HCNetSDK.NET_DVR_AUDIODEC_PROCESS_PARAM struAudioParam = new HCNetSDK.NET_DVR_AUDIODEC_PROCESS_PARAM();
                    struAudioParam.in_buf = pRecvDataBuffer;
                    struAudioParam.in_data_size = dwBufSize;
                    HCNetSDK.BYTE_ARRAY ptrVoiceData = new HCNetSDK.BYTE_ARRAY(320);
                    ptrVoiceData.write();
                    struAudioParam.out_buf = ptrVoiceData.getPointer();
                    struAudioParam.out_frame_size = 320;
                    struAudioParam.g711_type = 0; //G711编码类型：0- U law，1- A law
                    struAudioParam.write();
                    if (!ViedoCallMain.hCNetSDK.NET_DVR_DecodeG711Frame(pDecHandle, struAudioParam)) {
                        System.out.println("NET_DVR_DecodeG711Frame failed, error code:" + ViedoCallMain.hCNetSDK.NET_DVR_GetLastError());
                    }
                    struAudioParam.read();
                    //将解码之后PCM音频数据写入文件
                    long offsetPcm = 0;
                    ByteBuffer buffersPcm = struAudioParam.out_buf.getByteBuffer(offsetPcm, struAudioParam.out_frame_size);
                    byte[] bytesPcm = new byte[struAudioParam.out_frame_size];
                    buffersPcm.rewind();
                    buffersPcm.get(bytesPcm);
                    outputStreamPcm.write(bytesPcm);  //这里实现的是将设备发送的pcm音频数据写入文件，（前面的代码实现的就是将g711解码成pcm音频）

                } catch (Exception e) {
                    e.printStackTrace();
                }

            } else if (byAudioFlag == 0) {
                System.out.println("客户端发送音频数据");
            }
        }
    }


    /**
     * 开启预览
     *
     * @param userID
     */
    public static void RealPlay(int userID) {
        if (userID == -1) {
            System.out.println("请先注册");
            return;
        }
        HCNetSDK.NET_DVR_PREVIEWINFO strClientInfo = new HCNetSDK.NET_DVR_PREVIEWINFO();
        strClientInfo.read();
        W32API.HWND hwnd = new W32API.HWND(Native.getComponentPointer(panelRealplay));
        strClientInfo.hPlayWnd = hwnd;  //窗口句柄，从回调取流不显示一般设置为空
        strClientInfo.lChannel = 1;  //通道号
        strClientInfo.dwStreamType = 0; //0-主码流，1-子码流，2-三码流，3-虚拟码流，以此类推
        strClientInfo.dwLinkMode = 0; //连接方式：0- TCP方式，1- UDP方式，2- 多播方式，3- RTP方式，4- RTP/RTSP，5- RTP/HTTP，6- HRUDP（可靠传输） ，7- RTSP/HTTPS，8- NPQ
        strClientInfo.write();
//        if (fRealDataCallBack == null) {
//            fRealDataCallBack = new FRealDataCallBack();
//        }
        lPlay = ViedoCallMain.hCNetSDK.NET_DVR_RealPlay_V40(userID, strClientInfo, null, null);
        if (lPlay == -1) {
            int iErr = ViedoCallMain.hCNetSDK.NET_DVR_GetLastError();
            System.out.println("取流失败" + iErr);
            return;
        }
        System.out.println("取流成功");

        try {
            Thread.sleep(3000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

    }


    /**
     * 停止预览
     */
    public static void stopPlay() {
        if (lPlay >= 0) {
            if (ViedoCallMain.hCNetSDK.NET_DVR_StopRealPlay(lPlay)) {
                System.out.println("停止预览成功");
                return;
            }
        }

    }

    /**
     * 开启语音对讲
     *
     * @param userID
     */
    public static boolean startVoiceCom(int userID) {

        if (voiceDatacallback == null) {
            voiceDatacallback = new VoiceDataCallBack();
        }
        int voiceChannel = 1; //语音通道号。对于设备本身的语音对讲通道，从1开始；对于设备的IP通道，为登录返回的
        // 起始对讲通道号(byStartDTalkChan) + IP通道索引 - 1，例如客户端通过NVR跟其IP Channel02所接前端IPC进行对讲，则dwVoiceChan=byStartDTalkChan + 1
        boolean bret = true;  //需要回调的语音数据类型：0- 编码后的语音数据，1- 编码前的PCM原始数据
        lVoiceComHandle = ViedoCallMain.hCNetSDK.NET_DVR_StartVoiceCom_V30(userID, voiceChannel, bret, voiceDatacallback, null);
        if (lVoiceComHandle <= -1) {
            System.out.println("语音对讲开启失败，错误码为" + ViedoCallMain.hCNetSDK.NET_DVR_GetLastError());
            return false;
        }
        System.out.println("语音对讲开始成功！");
        return true;

    }


    /**
     * 停止语音对讲
     *
     * @return
     */
    public static boolean stopVoiceCom(int lVoiceComHandle) {
        if (!ViedoCallMain.hCNetSDK.NET_DVR_StopVoiceCom(lVoiceComHandle)) {
            System.out.println("停止对讲失败，错误码为" + ViedoCallMain.hCNetSDK.NET_DVR_GetLastError());
            return false;
        }
        System.out.println("语音对讲停止成功！");
        return true;

    }


    /**
     * 开启语音转发
     * 设备音频编码格式G711u
     *
     * @return
     */
    public static boolean startVoiceG711Trans(int userID) {

        //获取设备音频编码格式
        HCNetSDK.NET_DVR_COMPRESSION_AUDIO compression_audio=new HCNetSDK.NET_DVR_COMPRESSION_AUDIO();
        boolean b_AudioCom=ViedoCallMain.hCNetSDK.NET_DVR_GetCurrentAudioCompress(userID,compression_audio);
        if (b_AudioCom==false)
        {
            System.out.println("NET_DVR_GetCurrentAudioCompress fail err="+ViedoCallMain.hCNetSDK.NET_DVR_GetLastError());
        }
        //音频编码类型：0- G722，1- G711_U，2- G711_A，5- MP2L2，6- G726，7- AAC，8- PCM，9-G722，10-G723，11-G729，12-AAC_LC，13-AAC_LD，14-Opus，15-MP3，16-ADPCM
        //音频采样率：0- 默认，1- 16kHZ，2- 32kHZ，3- 48kHZ，4- 44.1kHZ，5- 8kHZ
        //不同音频编码方式语音转发接口流程不同，Demo中使用1- G711_U编码方式作为示例
        System.out.println("设备音频编码格式："+compression_audio.byAudioEncType+"音频采样率："+compression_audio.byAudioSamplingRate);


        //设置语音对讲函数回调
        file = new File(System.getProperty("user.dir") + "\\AudioFile\\DeviceSaveData.g7");  //保存回调函数的音频数据

        if (!file.exists()) {
            try {
                file.createNewFile();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        try {
            outputStream = new FileOutputStream(file);
        } catch (FileNotFoundException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }

        //保存解码后音频数据
        filePcm = new File(System.getProperty("user.dir") + "\\AudioFile\\DecodeSaveData.pcm");  //保存回调函数的音频数据

        if (!filePcm.exists()) {
            try {
                filePcm.createNewFile();
            } catch (IOException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        }
        try {
            outputStreamPcm = new FileOutputStream(filePcm);
        } catch (FileNotFoundException e3) {
            // TODO Auto-generated catch block
            e3.printStackTrace();
        }
        //设置语音回调函数
        if (cbVoiceDataCallBack == null) {
            cbVoiceDataCallBack = new cbVoiceDataCallBack_MR_V30();
        }
        int voiceChannel = 1; //语音通道号。对于设备本身的语音对讲通道，从1开始；对于设备的IP通道，为登录返回的
        // 起始对讲通道号(byStartDTalkChan) + IP通道索引 - 1，例如客户端通过NVR跟其IP Channel02所接前端IPC进行对讲，则dwVoiceChan=byStartDTalkChan + 1
        lVoiceTransHandle = ViedoCallMain.hCNetSDK.NET_DVR_StartVoiceCom_MR_V30(userID, voiceChannel, cbVoiceDataCallBack, null);
        if (lVoiceTransHandle == -1) {
            System.out.println("语音转发启动失败,err=" + ViedoCallMain.hCNetSDK.NET_DVR_GetLastError());
            return false;
        }
        System.out.println("语音转发开启成功");

        //先测试接收设备发送音频数据
/*        try {
            Thread.sleep(1000);
        } catch (InterruptedException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }*/
        //以下代码是读取本地音频文件发送给设备
        return true;
    }


    /**
     * 停止语音转发
     *
     * @param Handle
     */
    public static boolean stopVoiceG711Trans(int Handle) {


        if (pDecHandle != null) {
            ViedoCallMain.hCNetSDK.NET_DVR_ReleaseG711Decoder(pDecHandle);
        }
        if (!ViedoCallMain.hCNetSDK.NET_DVR_StopVoiceCom(Handle)) {
            System.out.println("停止语音转发失败，错误码为" + ViedoCallMain.hCNetSDK.NET_DVR_GetLastError());
            return false;
        }
        System.out.println("语音转发停止成功");
        if (outputStream != null) {
            try {
                outputStream.close();
            } catch (IOException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        }
        if (outputStreamPcm != null) {
            try {
                outputStreamPcm.close();
            } catch (IOException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        }
        return true;
    }


    /**
     * 开启可视对讲长连接
     *
     * @param userID
     */
    public void getCallSingal(int userID) {
        HCNetSDK.NET_DVR_VIDEO_CALL_COND strVideoCallCond = new HCNetSDK.NET_DVR_VIDEO_CALL_COND();
        strVideoCallCond.dwSize = strVideoCallCond.size();
        strVideoCallCond.write();
        Pointer pUserData = null;
        if (videoCallSingalCallback == null) {
            videoCallSingalCallback = new ViedoCallSingalCallBacak();
        }
        videocallHandle = ViedoCallMain.hCNetSDK.NET_DVR_StartRemoteConfig(userID, 16032, strVideoCallCond.getPointer(),
                strVideoCallCond.size(), videoCallSingalCallback, pUserData);
        if (videocallHandle <= -1) {
            System.out.println("NET_DVR_StartRemoteConfig failed errCode:" + ViedoCallMain.hCNetSDK.NET_DVR_GetLastError());
        }
        System.out.println("可视对讲长连接建立成功");
        return;
        
    }

    /**
     * 断开可视对讲长连接
     *
     * @param VideocallHandle
     */
    public void stopCallSingal(int VideocallHandle) {
        if (VideocallHandle >= 0) {
            boolean b_Ret = ViedoCallMain.hCNetSDK.NET_DVR_StopRemoteConfig(VideocallHandle);
            if (b_Ret == false) {
                System.out.println("NET_DVR_StopRemoteConfig failed errCode:" + ViedoCallMain.hCNetSDK.NET_DVR_GetLastError());
                return;
            }
            System.out.println("断开可视对讲长连接建立成功");
            return;

        }

    }

}
