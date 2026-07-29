package Face;

import Common.CommonMethod;
import NetSDKDemo.HCNetSDK;
import com.sun.jna.Pointer;
import com.sun.jna.ptr.IntByReference;

import java.io.IOException;
import java.io.UnsupportedEncodingException;

/**
 * 自定义人脸库和自定义人脸ID集成流程
 * 自定义人脸图片所在的人脸库必须为私密库，人脸库增删时必须使用identityKey字段，不使用的话目前设备不支持增加，会导致无法检索和删除。
 */
public  class FacePicManagebyCustom {

    /**
     * 自定义ID创建一个人脸库
     * @param lUserID
     * @param identityKey  交互口令
     * @param name  人脸库名称
     * @param customlibid 人脸库自定义ID
     * @throws UnsupportedEncodingException 
     *
     */
    public static void creatCustomFaceLib(int lUserID, String identityKey,String name, String customlibid ) throws UnsupportedEncodingException
    {
    	 String result=ISAPI.sdk_isapi(lUserID,"POST /ISAPI/Intelligent/FDLib?FDType=custom&&identityKey="+identityKey,CommonMethod.xmlCreatCustomID("1",name,customlibid));
         System.out.println("创建自定义人脸库结果："+result);
    }

    /**
     * 查询自定义人脸库信息
     * @param lUserID
     * @param identityKey
     * @param customlibid
     * @throws UnsupportedEncodingException 
     */
    public static void getCustomFaceLib(int lUserID, String identityKey,String customlibid) throws UnsupportedEncodingException
    {
        String result=ISAPI.sdk_isapi(lUserID,"GET /ISAPI/Intelligent/FDLib/"+customlibid+"?FDType=custom&&identityKey="+identityKey, "");
        System.out.println("获取自定义人脸库结果:"+result);
    }

    public static void delCustomFaceLib(int lUserID,String identityKey,String customlibid) throws UnsupportedEncodingException
    {
        String result =ISAPI.sdk_isapi(lUserID,"DELETE /ISAPI/Intelligent/FDLib/"+customlibid+"?FDType=custom&&identityKey="+identityKey, "");
        System.out.println("删除自定义人脸库结果："+result);

    }


    /**
     * 自定义人脸库中添加自定义人脸ID
     * @param lUserID
     * @param customFDID
     * @param IdentityKey
     * @throws IOException
     */
    public static void uploadPicbycustomID(int lUserID, String customFDID,String IdentityKey ) throws IOException {
        HCNetSDK.NET_DVR_FACELIB_COND struFaceLibCond = new HCNetSDK.NET_DVR_FACELIB_COND();
        struFaceLibCond.read();
        struFaceLibCond.dwSize = struFaceLibCond.size();
        struFaceLibCond.szFDID = customFDID.getBytes(); //人脸库ID
        struFaceLibCond.byCustomFaceLibID=1;  //人脸库ID是否是自定义：0- 不是，1- 是
        struFaceLibCond.byIdentityKey= IdentityKey.getBytes();  //交互操作口令  和自定义添加人脸库的IdentityKey保持一致
        struFaceLibCond.byConcurrent = 0; //设备并发处理：0- 不开启(设备自动会建模)，1- 开始(设备不会自动进行建模)
        struFaceLibCond.byCover = 1;  //是否覆盖式导入(人脸库存储满的情况下强制覆盖导入时间最久的图片数据)：0- 否，1- 是
        struFaceLibCond.write();
        Pointer pStruFaceLibCond = struFaceLibCond.getPointer();
        int iUploadHandle = FaceMain.hCNetSDK.NET_DVR_UploadFile_V40(lUserID, HCNetSDK.IMPORT_DATA_TO_FACELIB, pStruFaceLibCond,
                struFaceLibCond.size(), null, Pointer.NULL, 0);
        if (iUploadHandle <= -1) {
            int iErr = FaceMain.hCNetSDK.NET_DVR_GetLastError();
            System.err.println("NET_DVR_UploadFile_V40失败，错误号" + iErr);
            return;
        } else {
            System.out.println("NET_DVR_UploadFile_V40成功");
        }
        HCNetSDK.NET_DVR_SEND_PARAM_IN struSendParam = new HCNetSDK.NET_DVR_SEND_PARAM_IN();
        struSendParam.read();
        //本地jpg图片转成二进制byte数组
        byte[] picbyte = CommonMethod.toByteArray(".\\pic\\test1.jpg");
        HCNetSDK.BYTE_ARRAY arraybyte = new HCNetSDK.BYTE_ARRAY(picbyte.length);
        arraybyte.read();
        arraybyte.byValue = picbyte;
        arraybyte.write();
        struSendParam.pSendData = arraybyte.getPointer();
        struSendParam.dwSendDataLen = picbyte.length;
        struSendParam.byPicType = 1; //图片格式：1- jpg，2- bmp，3- png，4- SWF，5- GIF
        struSendParam.sPicName = "test01".getBytes(); //图片名称
        //图片的附加信息缓冲区  图片上添加的属性信息，性别、身份等
        //1:xml文本导入方式
/**        byte[] AppendData = CommonMethod.toByteArray("..\\pic\\test.xml");
        HCNetSDK.BYTE_ARRAY byteArray = new HCNetSDK.BYTE_ARRAY(AppendData.length);
        byteArray.read();
        byteArray.byValue = AppendData;
        byteArray.write();*/
        /**2:包含中文姓名的报文上传
        <customHumanID>ID20220109</customHumanID> 表示自定义人脸ID*/
        byte[] byFDLibName = "测试名称".getBytes("UTF-8");
        String strInBuffer1 = new String("<FaceAppendData version=\"2.0\" xmlns=\"http://www.hikvision.com/ver20/XMLSchema\"><bornTime>2014-12-12T00:00:00Z</bornTime><name>");
        String strInBuffer2 = new String("</name><sex>female</sex><province>11</province><city>01</city><certificateType>officerID</certificateType><certificateNumber>1123123123</certificateNumber><PersonInfoExtendList><PersonInfoExtend><id>1</id><enable>false</enable><name>test1</name><value>test2</value></PersonInfoExtend></PersonInfoExtendList><customHumanID>ID20220109</customHumanID></FaceAppendData>");
        int iStringSize = byFDLibName.length + strInBuffer1.length() + strInBuffer2.length();
        HCNetSDK.BYTE_ARRAY ptrByte = new HCNetSDK.BYTE_ARRAY(iStringSize);
        System.arraycopy(strInBuffer1.getBytes(), 0, ptrByte.byValue, 0, strInBuffer1.length());
        System.arraycopy(byFDLibName, 0, ptrByte.byValue, strInBuffer1.length(), byFDLibName.length);
        System.arraycopy(strInBuffer2.getBytes(), 0, ptrByte.byValue, strInBuffer1.length() + byFDLibName.length, strInBuffer2.length());
        ptrByte.write();
        struSendParam.pSendAppendData = ptrByte.getPointer();
        struSendParam.dwSendAppendDataLen = ptrByte.byValue.length;
        struSendParam.write();
        int iSendData = FaceMain.hCNetSDK.NET_DVR_UploadSend( iUploadHandle, struSendParam, Pointer.NULL);
        if (iSendData <= -1) {
            int iErr = FaceMain.hCNetSDK.NET_DVR_GetLastError();
            System.err.println("NET_DVR_UploadSend失败，错误号" + iErr);
            return;
        }
        while (true) {
            IntByReference Pint = new IntByReference(0);
            int state = FaceMain.hCNetSDK.NET_DVR_GetUploadState(iUploadHandle, Pint.getPointer());
            if (state == 1) {
                System.out.println("上传成功");
                //获取图片ID
                HCNetSDK.NET_DVR_UPLOAD_FILE_RET struUploadRet = new HCNetSDK.NET_DVR_UPLOAD_FILE_RET();
                boolean bUploadResult = FaceMain.hCNetSDK.NET_DVR_GetUploadResult(iUploadHandle, struUploadRet.getPointer(), struUploadRet.size());
                if (!bUploadResult) {
                    int iErr = FaceMain.hCNetSDK.NET_DVR_GetLastError();
                    System.err.println("NET_DVR_GetUploadResult失败，错误号" + iErr);
                } else {
                    struUploadRet.read();
                    System.out.println("图片ID：" + new String(struUploadRet.sUrl, "UTF-8"));
                }
                break;
            } else if (state == 2) {
                System.out.println("进度：" + Pint.getValue());
                continue;
            }
            System.err.println("返回值" + state);
            break;
        }
        //关闭图片上传连接
        boolean b_Close = FaceMain.hCNetSDK.NET_DVR_UploadClose(iUploadHandle);
        if (!b_Close) {
            int iErr = FaceMain.hCNetSDK.NET_DVR_GetLastError();
            System.err.println("NET_DVR_UploadSend失败，错误号" + iErr);
            return;
        }
    }

    /**
     * 根据自定义人脸ID查询人脸图片
     * @param lUserID
     * @param customID  自定义人脸库ID
     * @param customHumanID 自定义人脸ID
     *@param identityKey 创建自定义人脸的交互口令
     * @throws UnsupportedEncodingException 
     */
    public static void getFacePicBycustomID(int lUserID, String customID, String customHumanID,String identityKey) throws UnsupportedEncodingException {

        String requestUrl = "GET /ISAPI/Intelligent/FDLib/" + customID + "/picture/" + customHumanID+"?FDType=custom&&identityKey="+identityKey;
        String result=ISAPI.sdk_isapi(lUserID, requestUrl, "");
        System.out.println("查询自定义人脸图片结果:"+result);
    }


    public static void delFacePicBycustomID(int lUserID, String customID, String customHumanID,String identityKey) throws UnsupportedEncodingException
    {
        String requestUrl = "DELETE /ISAPI/Intelligent/FDLib/" + customID + "/picture/" + customHumanID+"?FDType=custom&&identityKey="+identityKey;
        String result=ISAPI.sdk_isapi(lUserID, requestUrl, "");
        System.out.println("删除自定义人脸图片结果:"+result);


    }







}
