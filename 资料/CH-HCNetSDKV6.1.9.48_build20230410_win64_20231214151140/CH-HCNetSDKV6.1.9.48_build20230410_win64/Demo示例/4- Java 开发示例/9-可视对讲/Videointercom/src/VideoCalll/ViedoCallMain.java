package VideoCalll;

import NetSDKDemo.FMSGCallBack_V31;
import NetSDKDemo.HCNetSDK;
import NetSDKDemo.PlayCtrl;
import Utils.osSelect;
import com.sun.jna.Native;
import com.sun.jna.Pointer;

import java.util.Scanner;

import static VideoCalll.IndoorToClient.videocallHandle;

/**
 * @author jX
 * @create 2022-11-09-11:10
 */
public class ViedoCallMain {

    static HCNetSDK hCNetSDK = null;
    public static PlayCtrl playCtrl = null;
    static int lUserID = -1;//用户句柄
    public static int lAlarmHandle = -1; //布防句柄
    public static int lListenHandle = -1;
    public static FMSGCallBack_V31 fMSFCallBack_V31 = null;


    /**
     * 根据不同操作系统选择不同的库文件和库路径
     * @return
     */
    private static boolean createSDKInstance()
    {
        if(hCNetSDK == null)
        {
            synchronized (HCNetSDK.class)
            {
                String strDllPath = "";
                try
                {
                    //System.setProperty("jna.debug_load", "true");
                    if(osSelect.isWindows())
                        //win系统加载库路径
                        strDllPath = System.getProperty("user.dir") + "\\lib\\HCNetSDK.dll";
                    else if(osSelect.isLinux())
                        //Linux系统加载库路径
                        strDllPath =  System.getProperty("user.dir") + "/lib/libhcnetsdk.so";
                    hCNetSDK = (HCNetSDK) Native.loadLibrary(strDllPath, HCNetSDK.class);
                }catch (Exception ex) {
                    System.out.println("loadLibrary: " + strDllPath + " Error: " + ex.getMessage());
                    return false;
                }
            }
        }
        return true;
    }

    /**
     * 播放库加载
     *
     * @return
     */
    private static boolean createPlayInstance() {
        if (playCtrl == null) {
            synchronized (PlayCtrl.class) {
                String strPlayPath = "";
                try {
                    if (osSelect.isWindows())
                        //win系统加载库路径(路径不要带中文)
                        strPlayPath = System.getProperty("user.dir") + "\\lib\\PlayCtrl.dll";
                    else if (osSelect.isLinux())
                        //Linux系统加载库路径(路径不要带中文)
                        strPlayPath = System.getProperty("user.dir")+"/lib/libPlayCtrl.so";
                    playCtrl = (PlayCtrl) Native.loadLibrary(strPlayPath, PlayCtrl.class);

                } catch (Exception ex) {
                    System.out.println("loadLibrary: " + strPlayPath + " Error: " + ex.getMessage());
                    return false;
                }
            }
        }
        return true;
    }
    public static void main(String[] args) throws InterruptedException {


        if(hCNetSDK == null)
        {
            if(!createSDKInstance())
            {
                System.out.println("Load SDK fail");
                return;
            }
        }
        if (playCtrl == null) {
            if (!createPlayInstance()) {
                System.out.println("Load PlayCtrl fail");
                return;
            }

        }
        //linux系统建议调用以下接口加载组件库
        if (osSelect.isLinux())
        {
            HCNetSDK.BYTE_ARRAY ptrByteArray1 = new HCNetSDK.BYTE_ARRAY(256);
            HCNetSDK.BYTE_ARRAY ptrByteArray2 = new HCNetSDK.BYTE_ARRAY(256);
            //这里是库的绝对路径，请根据实际情况修改，注意改路径必须有访问权限
            String strPath1 = System.getProperty("user.dir") + "/lib/libcrypto.so.1.1";
            String strPath2 = System.getProperty("user.dir") + "/lib/libssl.so.1.1";

            System.arraycopy(strPath1.getBytes(), 0, ptrByteArray1.byValue, 0, strPath1.length());
            ptrByteArray1.write();
            hCNetSDK.NET_DVR_SetSDKInitCfg(3, ptrByteArray1.getPointer());

            System.arraycopy(strPath2.getBytes(), 0, ptrByteArray2.byValue, 0, strPath2.length());
            ptrByteArray2.write();
            hCNetSDK.NET_DVR_SetSDKInitCfg(4, ptrByteArray2.getPointer());

            String strPathCom = System.getProperty("user.dir") + "/lib/";
            HCNetSDK.NET_DVR_LOCAL_SDK_PATH struComPath = new HCNetSDK.NET_DVR_LOCAL_SDK_PATH();
            System.arraycopy(strPathCom.getBytes(), 0, struComPath.sPath, 0, strPathCom.length());
            struComPath.write();
            hCNetSDK.NET_DVR_SetSDKInitCfg(2, struComPath.getPointer());
        }

        hCNetSDK.NET_DVR_Init();
        boolean i= hCNetSDK.NET_DVR_SetLogToFile(3, "./sdklog", false);
        //设置报警回调函数
        if (fMSFCallBack_V31 == null) {
            fMSFCallBack_V31 = new FMSGCallBack_V31();
            Pointer pUser = null;
            if (!ViedoCallMain.hCNetSDK.NET_DVR_SetDVRMessageCallBack_V31(fMSFCallBack_V31, pUser)) {
                System.out.println("设置回调函数失败!");
            } else {
                System.out.println("设置回调函数成功!");
            }
        }

        //设备登录
        Login("10.19.34.12","admin","hik12345",(short) 8000);	//登陆

        //建立可视对讲长连接
        JPanelDemo.jRealWinInit();
        IndoorToClient indoorToClient=new IndoorToClient();
        indoorToClient.getCallSingal(lUserID);

        while (true) {
            //这里加入控制台输入控制，是为了保持连接状态，当输入Y表示布防结束
            System.out.print("请选择是否退出程序(Y/N)：\n");
            Scanner input = new Scanner(System.in);
            String str = input.next();
            if (str.equals("Y")) {
                break;
            }
        }
        //停止可视对讲长连接
        indoorToClient.stopCallSingal(videocallHandle);

        //设备注销,进程退出前调用，
        Logout();
        JPanelDemo.jRealWinDestory();


    }




    /**
     * 设备登录
     * @param ipadress IP地址
     * @param user  用户名
     * @param psw  密码
     * @param port 端口，默认8000
     */
    public static void Login(String ipadress, String user, String psw, short port) {
        //注册
        HCNetSDK.NET_DVR_USER_LOGIN_INFO m_strLoginInfo = new HCNetSDK.NET_DVR_USER_LOGIN_INFO();//设备登录信息
        String m_sDeviceIP = ipadress;//设备ip地址
        m_strLoginInfo.sDeviceAddress = new byte[HCNetSDK.NET_DVR_DEV_ADDRESS_MAX_LEN];
        System.arraycopy(m_sDeviceIP.getBytes(), 0, m_strLoginInfo.sDeviceAddress, 0, m_sDeviceIP.length());
        String m_sUsername = user;//设备用户名
        m_strLoginInfo.sUserName = new byte[HCNetSDK.NET_DVR_LOGIN_USERNAME_MAX_LEN];
        System.arraycopy(m_sUsername.getBytes(), 0, m_strLoginInfo.sUserName, 0, m_sUsername.length());
        String m_sPassword = psw;//设备密码
        m_strLoginInfo.sPassword = new byte[HCNetSDK.NET_DVR_LOGIN_PASSWD_MAX_LEN];
        System.arraycopy(m_sPassword.getBytes(), 0, m_strLoginInfo.sPassword, 0, m_sPassword.length());
        m_strLoginInfo.wPort = port; //sdk端口
        m_strLoginInfo.bUseAsynLogin = false; //是否异步登录：0- 否，1- 是
        m_strLoginInfo.write();
        HCNetSDK.NET_DVR_DEVICEINFO_V40 m_strDeviceInfo = new HCNetSDK.NET_DVR_DEVICEINFO_V40();//设备信息
        lUserID = hCNetSDK.NET_DVR_Login_V40(m_strLoginInfo, m_strDeviceInfo);
        if (lUserID == -1) {
            System.out.println("登录失败，错误码为" + hCNetSDK.NET_DVR_GetLastError());
            return;
        } else {
            System.out.println("登录成功！");
            m_strDeviceInfo.read();
            return;

        }
    }


/*    public static class RunnableDemo implements Runnable {

        @Override
        public void run() {

            JPanelDemo.jRealWinInit();


        }
    }*/


    /**
     * 登出操作
     *
     */
    public static void Logout(){

        /**登出和清理，释放SDK资源*/
        if (lUserID>=0)
        {
            if (!hCNetSDK.NET_DVR_Logout(lUserID))
            {
                System.out.println("设备注销失败，错误码：" + hCNetSDK.NET_DVR_GetLastError());
                return;
            }
            System.out.println("设备注销成功！！！");
        }

    }







}
