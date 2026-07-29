package VideoCalll;

import NetSDKDemo.HCNetSDK;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import static VideoCalll.IndoorToClient.*;
import static VideoCalll.ViedoCallMain.hCNetSDK;
import static VideoCalll.ViedoCallMain.lUserID;

public class JPanelDemo  {

    static JButton acceptBtn =new JButton("接听");//实例化button并添加标题
    static JButton cancelBtn =new JButton("拒接");//实例化button并添加标题
    static JButton hangUpBtn =new JButton("挂断");//实例化button并添加标题

    public  static void jRealWinInit()
    {
            //创建一个JFrame对象
        IndoorToClient.jf.setBounds(700, 200, 600, 400);    //设置窗口大小和位置
        IndoorToClient.panelRealplay=new Panel();    //创建一个JPanel对象
        IndoorToClient.panelRealplay.setBounds(0,0,300,200);
        IndoorToClient.panelRealplay.setBackground(Color.BLUE);    //设置背景色
        IndoorToClient.jf.add(IndoorToClient.panelRealplay);    //将面板添加到窗口
       IndoorToClient.jFrameBtn.setBounds(1300,200,50,400);
        IndoorToClient.jFrameBtn.setLayout(null);

        IndoorToClient.jFrameBtn.add(acceptBtn);  //添加button 到窗口中
        IndoorToClient.jFrameBtn.add(cancelBtn);
        IndoorToClient.jFrameBtn.add(hangUpBtn);

        acceptCall();
        cancelCall();
        hangUpCall();
        acceptBtn.setBounds(15,10,80,20);    //  参数分别为坐标，控件的规格
        cancelBtn.setBounds(15,40,80,20);
        hangUpBtn.setBounds(15,70,80,20);

        IndoorToClient.jf.setVisible(true);    //设置窗口可见
        IndoorToClient.jFrameBtn.setVisible(true);
    }

    /**
     * 客户端接听
     */
    public static void acceptCall()
    {
        acceptBtn.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e)
            {
                //接听
                System.out.println("接听按钮被点击");
                HCNetSDK.NET_DVR_VIDEO_CALL_PARAM m_struVideoCallParm = new HCNetSDK.NET_DVR_VIDEO_CALL_PARAM();
                m_struVideoCallParm.read();
                m_struVideoCallParm.dwSize=m_struVideoCallParm.size();
                m_struVideoCallParm.dwCmdType = 2; //接听本次呼叫
                m_struVideoCallParm.write();
                boolean b_ret=hCNetSDK.NET_DVR_SendRemoteConfig(videocallHandle,0,m_struVideoCallParm.getPointer(),m_struVideoCallParm.dwSize);
                if (b_ret == false)
                {
                    System.out.println("NET_DVR_SendRemoteConfig，错误码为" + hCNetSDK.NET_DVR_GetLastError());
                    return;
                }
                //开启预览
                IndoorToClient.RealPlay(lUserID);
                //开启语音对讲或者转发，开发客户端程序使用语言对讲，开发web服务，使用语言转发
                //1.开启语音对讲
//                IndoorToClient.startVoiceCom(lUserID);

                //2.开启语音转发
                IndoorToClient.startVoiceG711Trans(lUserID);
                //语音转发发送音频数据线程
                SendAudioRunnable sendAudioRunnable=new SendAudioRunnable();
                SendAudioRunnable.flag=true;
                new Thread(sendAudioRunnable).start();
            }
        } );
    }


    /**
     * 客户端拒接
     */
    public static void cancelCall()
    {
        cancelBtn.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                //拒接
                System.out.println("取消接听按钮被点击");
                HCNetSDK.NET_DVR_VIDEO_CALL_PARAM m_struVideoCallParm = new HCNetSDK.NET_DVR_VIDEO_CALL_PARAM();
                m_struVideoCallParm.read();
                m_struVideoCallParm.dwSize=m_struVideoCallParm.size();
                m_struVideoCallParm.dwCmdType = 3; //3- 拒绝本地来电呼叫
                m_struVideoCallParm.write();
                boolean b_ret = hCNetSDK.NET_DVR_SendRemoteConfig(videocallHandle, 0, m_struVideoCallParm.getPointer(), m_struVideoCallParm.size());
                if (b_ret == false) {
                    System.out.println("NET_DVR_SendRemoteConfig，错误码为" + hCNetSDK.NET_DVR_GetLastError());
                    return;
                }
                return;
            }
        });
        return;
    }

    /**
     * 客户端挂断
     */
    public static void hangUpCall()
    {
        hangUpBtn.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                //挂断
                System.out.println("挂断按钮被点击");
                HCNetSDK.NET_DVR_VIDEO_CALL_PARAM m_struVideoCallParm = new HCNetSDK.NET_DVR_VIDEO_CALL_PARAM();
                m_struVideoCallParm.read();
                m_struVideoCallParm.dwSize=m_struVideoCallParm.size();
                m_struVideoCallParm.dwCmdType = 5; //5- 结束本次通话
                m_struVideoCallParm.write();
                boolean b_ret = hCNetSDK.NET_DVR_SendRemoteConfig(videocallHandle, 0, m_struVideoCallParm.getPointer(), m_struVideoCallParm.size());
                if (b_ret == false) {
                    System.out.println("NET_DVR_SendRemoteConfig，错误码为" + hCNetSDK.NET_DVR_GetLastError());
                    return;
                }
                //客户端主动停止预览和语音对讲
                if (lPlay>=0)
                {
                    IndoorToClient.stopPlay();
                }
                if (lVoiceComHandle>=0)
                {
                    IndoorToClient.stopVoiceCom(lVoiceComHandle);
                }
                if (lVoiceTransHandle >=0)
                {
                    SendAudioRunnable.flag =false;
                    IndoorToClient.stopVoiceG711Trans(lVoiceTransHandle);

                }
                JPanelDemo.flashWindows();
                return;

            }
        });
        return;
    }

    public  static void flashWindows()
    {
        IndoorToClient.panelRealplay.removeAll();
        IndoorToClient.panelRealplay.repaint();
    }



    public static void jRealWinDestory()
    {
        IndoorToClient.jf.dispose();
        IndoorToClient.jFrameBtn.dispose();
    }


}
