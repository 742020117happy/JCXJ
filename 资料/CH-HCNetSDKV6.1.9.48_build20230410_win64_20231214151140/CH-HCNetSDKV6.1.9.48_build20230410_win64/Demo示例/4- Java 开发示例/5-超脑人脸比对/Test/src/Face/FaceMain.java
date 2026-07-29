package Face;

import Common.CommonMethod;
import Common.osSelect;
import NetSDKDemo.HCNetSDK;
import com.sun.jna.Native;
import org.dom4j.DocumentException;
import java.io.IOException;
import java.util.Scanner;

public class FaceMain {

    static HCNetSDK hCNetSDK = null;
    static int lUserID = -1;//用户句柄

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

    public static void main(String[] args) throws InterruptedException, IOException, DocumentException {
        if(hCNetSDK == null)
        {
            if(!createSDKInstance())
            {
                System.out.println("Load SDK fail");
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

        /*初始化*/
        hCNetSDK.NET_DVR_Init();
        
        /*加载日志*/
        hCNetSDK.NET_DVR_SetLogToFile(3, "./sdklog", false);

        login_V40("10.17.36.33","admin","hik12345",(short) 8000); //登录设备

        String requestUrl = ""; //透传URL命令
        String strOutXML = ""; //输出结果
        
        //人脸库ID,通过GET /ISAPI/Intelligent/FDLib查询设备当前所有人脸库信息获取人脸库ID

        String FDID = "F57C4AB8522642B09DA8980FB9878C12"; //人脸库ID
        String PID = "4"; //人脸图片ID，可以通过POST /ISAPI/Intelligent/FDLib/FDSearch查询指定人脸库的人脸图片，其中包含ID
            
        /*获取所有人脸库信息:ID*/
        requestUrl = "GET /ISAPI/Intelligent/FDLib";
        strOutXML = ISAPI.sdk_isapi(lUserID, requestUrl, "");
        System.out.println(strOutXML);

        /*获取指定人脸库的信息*/
//        requestUrl = "GET /ISAPI/Intelligent/FDLib/" + FDID;
//        strOutXML = ISAPI.sdk_isapi(lUserID, requestUrl, "");
//        System.out.println(strOutXML);
        
//        /*创建一个人脸库，创建成功的返回结果里面包含FDID*/
//        strOutXML = ISAPI.sdk_isapi(lUserID, "POST /ISAPI/Intelligent/FDLib", FacePicManage.fDCreate_XmlCreat("9", "test02"));
//        System.out.println(strOutXML);
        
        /*删除一个人脸库*/
        /* DELETE /ISAPI/Intelligent/FDLib/<FDID>，FDID为人脸库ID
         * 通过getAllFaceLib里面代码可以查询设备当前所有人脸库信息可以获取人脸库ID
         */
//        requestUrl = "DELETE /ISAPI/Intelligent/FDLib/" + FDID;
//        ISAPI.sdk_isapi(lUserID, requestUrl, "");
//        System.out.println(strOutXML);
        
        /*上传人脸图片到人脸库*/
//        FacePicManage.uploadPic(lUserID, FDID);
//        System.out.println(strOutXML);
        
        /*查询指定人脸库的人脸信息*/
//        requestUrl = "POST /ISAPI/Intelligent/FDLib/FDSearch";
//        String HumanName = "测试人员"; //人员姓名
//        strOutXML = ISAPI.sdk_isapi(lUserID, requestUrl, FacePicManage.searchByName_XmlCreat(FDID, HumanName));
//        System.out.println(strOutXML);
        
        /*获取人脸比对库图片数据附加信息*/ 
//        人脸图片ID，通过searchFaceLibData查询人脸库里面人脸图片信息或者uploadPic上传人脸图片会返回人脸图片ID
//        requestUrl = "GET /ISAPI/Intelligent/FDLib/" + FDID + "/picture/" + PID;
//        strOutXML = ISAPI.sdk_isapi(lUserID, requestUrl, "");
//        System.out.println(strOutXML);
        
        /**删除人脸比对库图片数据(包含附加信息)*/
//        requestUrl = "DELETE /ISAPI/Intelligent/FDLib/" + FDID + "/picture/" + PID;
//        strOutXML = ISAPI.sdk_isapi(lUserID, requestUrl, "");
//        System.out.println(strOutXML);
        
        /**按照图片搜索*/
//        String ModeData = FacePicManage.analysisImage(lUserID);//上传图片分析，分析结果返回模型数据
//        //使用模型数据检索人脸库
//        requestUrl = "POST /ISAPI/Intelligent/FDLib/FDSearch";
//        strOutXML = ISAPI.sdk_isapi(lUserID, requestUrl, FacePicManage.fDModeDate_XmlCreat(FDID, ModeData));
//        System.out.println(strOutXML);
        
        /**查询设备中存储的人脸比对结果信息*/
//        requestUrl = "POST /ISAPI/Intelligent/FDLib/FCSearch";
//        strOutXML = ISAPI.sdk_isapi(lUserID, requestUrl, CommonMethod.fCSearch_XmlCreat());
//        System.out.println(strOutXML);
        
        /**
         * 自定义人脸库和人脸ID相关接口
         */
//        FacePicManagebyCustom.uploadPicbycustomID(lUserID,"1","1");
//        FacePicManagebyCustom.getFacePicBycustomID(lUserID,"1","ID20220109","1");
//        FacePicManagebyCustom.delFacePicBycustomID(lUserID,"1","ID20220109","1");
//        FacePicManagebyCustom.getCustomFaceLib(lUserID,"1","1");
//        FacePicManagebyCustom.delCustomFaceLib(lUserID,"1","1");




        Thread.sleep(3000);

        /*注销登录，程序退出时调用；程序同时对接多台设备时，每一台设备分别调用一次接口，传入不同的登录句柄*/
        if (lUserID>=0)
        {
            hCNetSDK.NET_DVR_Logout(lUserID);
        }
        
        /*SDK反初始化，释放资源，程序退出时调用*/
        hCNetSDK.NET_DVR_Cleanup();
    }


    /**
     * 设备登录
     * @param ipadress IP地址
     * @param user  用户名
     * @param psw  密码
     * @param port 端口，默认8000
     */
    public static void login_V40(String ipadress, String user, String psw, short port) {
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


}
