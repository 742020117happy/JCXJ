package VideoCalll;

import NetSDKDemo.HCNetSDK;
import com.sun.jna.Pointer;

import java.io.*;
import java.nio.ByteBuffer;

import static VideoCalll.IndoorToClient.lVoiceTransHandle;

/**
 * @author jx
 * @create 2023-01-09-14:50
 */
public class SendAudioRunnable implements Runnable{

    public  static volatile boolean flag = true;

    static File fileEncode = null;
    static FileOutputStream outputStreamG711 = null;
    @Override
    public void run() {
        FileInputStream Voicefile = null;
//            FileOutputStream Encodefile = null;
        int dataLength = 0;

        try {
            //创建从文件读取数据的FileInputStream流
            Voicefile = new FileInputStream(new File(System.getProperty("user.dir") + "\\AudioFile\\temp.pcm"));
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }

        fileEncode = new File(System.getProperty("user.dir") + "\\AudioFile\\EncodeData.g7");  //保存音频编码数据

        if (!fileEncode.exists()) {
            try {
                fileEncode.createNewFile();
            } catch (IOException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        }

        try {
            outputStreamG711 = new FileOutputStream(fileEncode);
        } catch (FileNotFoundException e3) {
            // TODO Auto-generated catch block
            e3.printStackTrace();
        }

        try {

            //返回文件的总字节数
            dataLength = Voicefile.available();
//            System.out.println("文件总长度：\n"+dataLength);
        } catch (IOException e1) {
            e1.printStackTrace();
        }
        if (dataLength < 0) {
            System.out.println("input file dataSize < 0");
        }

        HCNetSDK.BYTE_ARRAY ptrVoiceByte = new HCNetSDK.BYTE_ARRAY(dataLength);
        try {
            Voicefile.read(ptrVoiceByte.byValue);
        } catch (IOException e2) {
            e2.printStackTrace();
        }
        ptrVoiceByte.write();
        int iEncodeSize = 0;
        HCNetSDK.NET_DVR_AUDIOENC_INFO enc_info = new HCNetSDK.NET_DVR_AUDIOENC_INFO();
        enc_info.write();
        Pointer encoder = ViedoCallMain.hCNetSDK.NET_DVR_InitG711Encoder(enc_info);
        while ((dataLength - iEncodeSize) > 0 && flag == true) {
            HCNetSDK.BYTE_ARRAY ptrPcmData = new HCNetSDK.BYTE_ARRAY(640);
            //文件最后一组数据小于640字节，后位需要补0
            if ((dataLength-iEncodeSize)<640)
            {
                System.arraycopy(ptrVoiceByte.byValue, iEncodeSize, ptrPcmData.byValue, 0, dataLength-iEncodeSize);
                ptrPcmData.write();
            }else {

                System.arraycopy(ptrVoiceByte.byValue, iEncodeSize, ptrPcmData.byValue, 0, 640);
                ptrPcmData.write();
            }

            HCNetSDK.BYTE_ARRAY ptrG711Data = new HCNetSDK.BYTE_ARRAY(320);
            ptrG711Data.write();

            HCNetSDK.NET_DVR_AUDIOENC_PROCESS_PARAM struEncParam = new HCNetSDK.NET_DVR_AUDIOENC_PROCESS_PARAM();
            struEncParam.in_buf = ptrPcmData.getPointer();
            struEncParam.out_buf = ptrG711Data.getPointer();
            struEncParam.out_frame_size = 320;
            struEncParam.g711_type = 0;//G711编码类型：0- U law，1- A law
            struEncParam.write();
            if (!ViedoCallMain.hCNetSDK.NET_DVR_EncodeG711Frame(encoder, struEncParam)) {
                System.out.println("NET_DVR_EncodeG711Frame failed, error code:" + ViedoCallMain.hCNetSDK.NET_DVR_GetLastError());
                ViedoCallMain.hCNetSDK.NET_DVR_ReleaseG711Encoder(encoder);
                //	hCNetSDK.NET_DVR_StopVoiceCom(lVoiceHandle);
            }
            struEncParam.read();
            ptrG711Data.read();
            long offsetG711 = 0;
            ByteBuffer buffersG711 = struEncParam.out_buf.getByteBuffer(offsetG711, struEncParam.out_frame_size);
            byte[] bytesG711 = new byte[struEncParam.out_frame_size];
            buffersG711.rewind();
            buffersG711.get(bytesG711);
            try {
                outputStreamG711.write(bytesG711); //这里实现的是将pcm编码成的g711数据写入
                // fileEncode = new File(System.getProperty("user.dir") + "\\AudioFile\\EncodeData.g7")
                //这个文件;（编码数据是否保存对整体功能不影响）
            } catch (IOException e1) {
                // TODO Auto-generated catch block
                e1.printStackTrace();
            }
            iEncodeSize += 640;
//            System.out.println("编码字节数：" + iEncodeSize);

            for (int i = 0; i < struEncParam.out_frame_size / 160; i++) {
                HCNetSDK.BYTE_ARRAY ptrG711Send = new HCNetSDK.BYTE_ARRAY(160);
                System.arraycopy(ptrG711Data.byValue, i * 160, ptrG711Send.byValue, 0, 160);
                ptrG711Send.write();
                if (!ViedoCallMain.hCNetSDK.NET_DVR_VoiceComSendData(lVoiceTransHandle, ptrG711Send.byValue, 160)) {
                    System.out.println("NET_DVR_VoiceComSendData failed, error code:" + ViedoCallMain.hCNetSDK.NET_DVR_GetLastError());
                }

                //需要实时速率发送数据
                try {
                    Thread.sleep(20);
                } catch (InterruptedException e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                }
            }

        }



    }
}

