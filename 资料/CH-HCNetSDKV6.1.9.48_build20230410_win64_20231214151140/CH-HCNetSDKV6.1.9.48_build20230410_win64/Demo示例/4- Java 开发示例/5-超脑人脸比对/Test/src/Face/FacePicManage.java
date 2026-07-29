package Face;
import Common.CommonMethod;
import NetSDKDemo.HCNetSDK;
import com.sun.jna.Pointer;
import com.sun.jna.ptr.IntByReference;
import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.DocumentHelper;
import org.dom4j.Element;

import java.io.*;
import java.util.Iterator;

/**
 * 功能：人脸库图片上传
 */
public class FacePicManage {
	static String ModeData; //图片建模数据
	
    //上传人脸图片
    public static void uploadPic(int lUserID, String FDID) throws IOException {
        HCNetSDK.NET_DVR_FACELIB_COND struFaceLibCond = new HCNetSDK.NET_DVR_FACELIB_COND();
        struFaceLibCond.read();
        struFaceLibCond.dwSize = struFaceLibCond.size();
        //人脸库ID
        for (int i = 0; i < HCNetSDK.NET_SDK_MAX_FDID_LEN; i++)
        {
        	struFaceLibCond.szFDID[i] = 0;
        }
        System.arraycopy(FDID.getBytes(), 0, struFaceLibCond.szFDID, 0, FDID.length());
        
        struFaceLibCond.byConcurrent = 0; //设备并发处理：0- 不开启(设备自动会建模)，1- 开始(设备不会自动进行建模)
        struFaceLibCond.byCover = 1;  //是否覆盖式导入(人脸库存储满的情况下强制覆盖导入时间最久的图片数据)：0- 否，1- 是
        struFaceLibCond.byCustomFaceLibID = 0;
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
        byte[] picbyte = CommonMethod.toByteArray(".\\pic\\FDLib.jpg");
        HCNetSDK.BYTE_ARRAY arraybyte = new HCNetSDK.BYTE_ARRAY(picbyte.length);
        arraybyte.read();
        arraybyte.byValue = picbyte;
        arraybyte.write();
        struSendParam.pSendData = arraybyte.getPointer();
        struSendParam.dwSendDataLen = picbyte.length;
        struSendParam.byPicType = 1; //图片格式：1- jpg，2- bmp，3- png，4- SWF，5- GIF

        //图片的附加信息缓冲区  图片上添加的属性信息，性别、身份等
        //1:xml文本导入方式
/*        byte[] AppendData = toByteArray("E:\\2.Demo汇总\\JAVA_DEMO\\01-Windows_Demo\\超脑人脸比对\\Test\\pic\\test.xml");
        HCNetSDK.BYTE_ARRAY byteArray = new HCNetSDK.BYTE_ARRAY(AppendData.length);
        byteArray.read();
        byteArray.byValue = AppendData;
        byteArray.write();*/
        //2:包含中文姓名的报文上传
        byte[] byFDLibName = "测试名称".getBytes("UTF-8");
        String strInBuffer1 = new String("<FaceAppendData version=\"2.0\" xmlns=\"http://www.hikvision.com/ver20/XMLSchema\"><bornTime>2014-12-12T00:00:00Z</bornTime><name>");
        String strInBuffer2 = new String("</name><sex>female</sex><province>11</province><city>01</city><certificateType>officerID</certificateType><certificateNumber>1123123123</certificateNumber><PersonInfoExtendList><PersonInfoExtend><id>1</id><enable>true</enable><name>test1</name><value>test2</value></PersonInfoExtend></PersonInfoExtendList></FaceAppendData>");
        int iStringSize = byFDLibName.length + strInBuffer1.length() + strInBuffer2.length();
        HCNetSDK.BYTE_ARRAY ptrByte = new HCNetSDK.BYTE_ARRAY(iStringSize);
        System.arraycopy(strInBuffer1.getBytes(), 0, ptrByte.byValue, 0, strInBuffer1.length());
        System.arraycopy(byFDLibName, 0, ptrByte.byValue, strInBuffer1.length(), byFDLibName.length);
        System.arraycopy(strInBuffer2.getBytes(), 0, ptrByte.byValue, strInBuffer1.length() + byFDLibName.length, strInBuffer2.length());
        ptrByte.write();
        struSendParam.pSendAppendData = ptrByte.getPointer();
        struSendParam.dwSendAppendDataLen = ptrByte.byValue.length;
        struSendParam.write();
        int iSendData = FaceMain.hCNetSDK.NET_DVR_UploadSend(iUploadHandle, struSendParam, Pointer.NULL);
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
                    System.out.println("图片ID：" + new String(struUploadRet.sUrl, "UTF-8").trim());
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

    //上传人脸图片分析，获取建模数据
    public static String analysisImage(int lUserID) throws IOException, DocumentException {  	
    	String sURL = "POST /ISAPI/Intelligent/analysisImage/face";
        HCNetSDK.BYTE_ARRAY stringRequest = new HCNetSDK.BYTE_ARRAY(1024);
        stringRequest.read();
        stringRequest.byValue = sURL.getBytes();
        stringRequest.write();
        
        //读取二进制文件
        byte[] picbyte = CommonMethod.toByteArray(".\\pic\\FDLib.jpg");
        HCNetSDK.BYTE_ARRAY byte_array = new HCNetSDK.BYTE_ARRAY(picbyte.length);
        byte_array.read();
        byte_array.byValue = picbyte;
        byte_array.write();
        
        HCNetSDK.NET_DVR_XML_CONFIG_INPUT struXMLInput = new HCNetSDK.NET_DVR_XML_CONFIG_INPUT();
        struXMLInput.read();
        struXMLInput.dwSize = struXMLInput.size();
        struXMLInput.lpRequestUrl = stringRequest.getPointer();
        struXMLInput.dwRequestUrlLen = sURL.length();
        struXMLInput.lpInBuffer = byte_array.getPointer(); //图片二进制数据
        struXMLInput.dwInBufferSize = byte_array.size();
        struXMLInput.write();
        
        HCNetSDK.BYTE_ARRAY stringXMLOut = new HCNetSDK.BYTE_ARRAY(8 * 1024);
        stringXMLOut.read();
        HCNetSDK.BYTE_ARRAY struXMLStatus = new HCNetSDK.BYTE_ARRAY(1024);
        struXMLStatus.read();
        HCNetSDK.NET_DVR_XML_CONFIG_OUTPUT struXMLOutput = new HCNetSDK.NET_DVR_XML_CONFIG_OUTPUT();
        struXMLOutput.read();
        struXMLOutput.dwSize = struXMLOutput.size();
        struXMLOutput.lpOutBuffer = stringXMLOut.getPointer();
        struXMLOutput.dwOutBufferSize = stringXMLOut.size();
        struXMLOutput.lpStatusBuffer = struXMLStatus.getPointer();
        struXMLOutput.dwStatusSize = struXMLStatus.size();
        struXMLOutput.write();
        if (!FaceMain.hCNetSDK.NET_DVR_STDXMLConfig(lUserID, struXMLInput, struXMLOutput)) {
            int iErr = FaceMain.hCNetSDK.NET_DVR_GetLastError();
            System.err.println("NET_DVR_STDXMLConfig失败，错误号" + iErr);
            return null;
        } else {
            stringXMLOut.read();
            System.out.println("输出文本大小：" + struXMLOutput.dwReturnedXMLSize);
            //打印输出XML文本
            String strOutXML = new String(stringXMLOut.byValue).trim();
            System.out.println(strOutXML);
            Document doc = null;
            doc = DocumentHelper.parseText(strOutXML); // 将字符串转为XML
            Element rootElt = doc.getRootElement(); // 获取根节点
            Iterator iter = rootElt.elementIterator("FaceContrastTarget"); // 获取根节点下的子节点FaceContrastTarget
            while (iter.hasNext()) {
                Element recordEle = (Element) iter.next();
                ModeData = recordEle.elementTextTrim("modeData"); // 拿到FaceContrastTarget节点下的子节点modeData值
                System.out.println("modeData:" + ModeData);
            }
            struXMLStatus.read();
            String strStatus = new String(struXMLStatus.byValue).trim();
            System.out.println(strStatus);
            return ModeData;
        }
    }   
    
    // 创建人脸库输入XML报文
    public static String fDCreate_XmlCreat(String ID, String Name) {
        /*<CreateFDLibList version="2.0" xmlns="http://www.isapi.org/ver20/XMLSchema">
        <!--req,创建人脸比对库输入参数-->
        <CreateFDLib>
        <id>
        <!--req, xs:integer,"表示list中子项个数,从"1"开始赋值,依次增加" -->
        </id>
        <name>
        <!--opt, xs:string,"人脸比对库名称"-->
        </name>
        <thresholdValue>
        <!--opt, xs:integer, "检测阈值,阈值越大检测准确率越低, 范围[0,100]"-->
        </thresholdValue>
        <customInfo>
        <!--opt, xs:string, 人脸库附加信息-->
        </customInfo>
        <customFaceLibID>
        <!--opt, xs:string, "自定义人脸库ID, 由上层下发给设备, 该ID由上层维护并确保唯一性,
        设备侧需将自定义人脸库ID与设备生成的FDID进行关联, 确保上层可通过下发人脸库ID来替代下发FDID进行后续操作"-->
            </customFaceLibID>
        </CreateFDLib>
        </CreateFDLibList>*/
        Document document1;
        Element root = DocumentHelper.createElement("CreateFDLibList");
        document1 = DocumentHelper.createDocument(root);
        Element CreateFDLib = root.addElement("CreateFDLib");
        Element id = CreateFDLib.addElement("id");
        id.setText(ID);
        Element name = CreateFDLib.addElement("name");
        name.setText(Name);
        Element thresholdValue = CreateFDLib.addElement("thresholdValue");
        thresholdValue.setText("70");
        String requestXml = document1.asXML();
        return requestXml;
    }
    
    //查询人脸比对库里的图片输入XML报文
    public static String fDModeDate_XmlCreat(String sFDID, String sModeData) {
        Document document1;
        Element root = DocumentHelper.createElement("FDSearchDescription");
        document1 = DocumentHelper.createDocument(root);
        Element searchID = root.addElement("searchID");
        searchID.setText("C929433A-AD10-0001-CA62-1A701E0015F4");
        Element searchResultPosition = root.addElement("searchResultPosition");
        searchResultPosition.setText("0");
        Element maxResults = root.addElement("maxResults");
        maxResults.setText("50");
        Element FDID = root.addElement("FDID");
        FDID.setText(sFDID);
        Element FaceModeList = root.addElement("FaceModeList");
        Element FaceMode = FaceModeList.addElement("FaceMode");
        Element ModeInfo = FaceMode.addElement("ModeInfo");
        Element similarity = ModeInfo.addElement("similarity");
        similarity.setText("80");
        Element modeData = ModeInfo.addElement("modeData");
        modeData.setText(sModeData);
        String requestXml = document1.asXML();
        System.out.println(requestXml);
        return requestXml;
    }

    //查询的人员输入报文
    //ID:人脸库ID
    // Name： 人员姓名
    public static String searchByName_XmlCreat(String ID, String Name) {
/*        <FDSearchDescription>
        <searchID>C929433A-AD10-0001-CA62-1A701E0015F2</searchID>
        <maxResults>50</maxResults>
        <searchResultPosition>0</searchResultPosition>
        <FDID>1135C03401404CC696F02B03F649ACFE</FDID>
        <name>test</name>
        <sex>male</sex>
        <province>21</province>
        <city>01</city>
        </FDSearchDescription>*/
        Document document1;
        Element root = DocumentHelper.createElement("FDSearchDescription");
        document1 = DocumentHelper.createDocument(root);
        Element searchID = root.addElement("searchID");
        searchID.setText("C929433A-AD10-0001-CA62-1A701E0015F2");
        Element maxResults = root.addElement("maxResults");
        maxResults.setText("50");
        Element searchResultPosition = root.addElement("searchResultPosition");
        searchResultPosition.setText("0");
        Element FDID = root.addElement("FDID");
        FDID.setText(ID);
        Element name = root.addElement("name");
        name.setText(Name);
        Element sex = root.addElement("sex");
        sex.setText("male");
        Element province = root.addElement("province");
        province.setText("11");
        Element city = root.addElement("city");
        city.setText("01");
        String requestXml = document1.asXML();
        System.out.println(requestXml);
        return requestXml;
    }

    //添加人脸附加信息报文
    public static String xmlFaceAppendData() throws UnsupportedEncodingException {
/*        <FDSearchDescription>
        <searchID>C929433A-AD10-0001-CA62-1A701E0015F2</searchID>
        <maxResults>50</maxResults>
        <searchResultPosition>0</searchResultPosition>
        <FDID>1135C03401404CC696F02B03F649ACFE</FDID>
        <name>test</name>
        <sex>male</sex>
        <province>21</province>
        <city>01</city>
        </FDSearchDescription>*/
        Document document1;
        Element root = DocumentHelper.createElement("FaceAppendData");
        document1 = DocumentHelper.createDocument(root);
        Element bornTime = root.addElement("bornTime");
        bornTime.setText("2020-12-12T00:00:00Z");
        Element name = root.addElement("name");
        name.setText("test");
        Element sex = root.addElement("sex");
        sex.setText("male");
        Element province = root.addElement("province");
        province.setText("11");
        Element city = root.addElement("city");
        city.setText("01");
        Element certificateType = root.addElement("certificateType");
        certificateType.setText("officerID");
        Element certificateNumber = root.addElement("certificateNumber");
        certificateNumber.setText("1123123123");
        Element PersonInfoExtendList = root.addElement("PersonInfoExtendList");
        Element PersonInfoExtend = PersonInfoExtendList.addElement("PersonInfoExtend");
        Element id = PersonInfoExtend.addElement("id");
        id.setText("1");
        Element enable = PersonInfoExtend.addElement("enable");
        enable.setText("1");
        Element name1 = PersonInfoExtend.addElement("name");
        name1.setText("1");
        Element value = PersonInfoExtend.addElement("value");
        value.setText("1");
        String requestXml = document1.asXML();
        System.out.println(requestXml);
        return requestXml;
    }
}
