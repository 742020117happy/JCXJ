using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Globalization;
using System.Reflection;
using System.IO;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using FaceManagement.Language;

namespace FaceManagement
{
    public partial class FaceManagement : Form
    {
        public FaceManagement()
        {
            InitializeComponent();
            CHCNetSDK.NET_DVR_Init();
            CHCNetSDK.NET_DVR_SetLogToFile(3, "./SdkLog/", true);
            comboBoxLanguage.SelectedIndex = 0;
        }

        private int m_UserID = -1;
        private int m_lGetFaceCfgHandle = -1;
        private int m_lSetFaceCfgHandle = -1;
        private int m_lCapFaceCfgHandle = -1;

        private void button1_Click(object sender, EventArgs e)
        {
            AddDev deviceAdd = new AddDev();
            deviceAdd.ShowDialog();
            m_UserID = deviceAdd.m_iUserID;
            deviceAdd.Dispose();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.InitialDirectory = Environment.CurrentDirectory;
            openFileDialog.Filter = "Face file|*.jpg|All documents|*.*";
            openFileDialog.RestoreDirectory = true;
            openFileDialog.FilterIndex = 1;

            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                if (pictureBoxFace.Image != null)
                {
                    pictureBoxFace.Image.Dispose();
                    pictureBoxFace.Image = null;
                }
                textBoxFilePath.Text = openFileDialog.FileName;
                pictureBoxFace.Image = Image.FromFile(textBoxFilePath.Text);
            }
        }

        private void button6_Click(object sender, EventArgs e)
        {
            if(textBoxFilePath.Text=="")
            {
                MessageBox.Show("Please choose human Face path");
                return;
            }

            if (pictureBoxFace.Image != null)
            {
                pictureBoxFace.Image.Dispose();
                pictureBoxFace.Image = null;
            }
            string sURL = "PUT /ISAPI/Intelligent/FDLib/FDSetUp?format=json";
            IntPtr ptrURL = Marshal.StringToHGlobalAnsi(sURL);
            m_lSetFaceCfgHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_UserID, CHCNetSDK.NET_DVR_FACE_DATA_RECORD, ptrURL, sURL.Length, null, IntPtr.Zero);
            if (m_lSetFaceCfgHandle == -1)
            {
                Marshal.FreeHGlobal(ptrURL);
                MessageBox.Show("NET_DVR_StartRemoteConfig fail [url:PUT /ISAPI/Intelligent/FDLib/FDSetUp?format=json] error:" + CHCNetSDK.NET_DVR_GetLastError());
                return;
            }
            Marshal.FreeHGlobal(ptrURL);

            CSetFaceDataCond JsonSetFaceDataCond = new CSetFaceDataCond();
            JsonSetFaceDataCond.faceLibType = "blackFD";
            JsonSetFaceDataCond.FDID = "1";
            JsonSetFaceDataCond.FPID = textBoxEmployeeNo.Text;
            string strJsonSearchFaceDataCond = JsonConvert.SerializeObject(JsonSetFaceDataCond, Formatting.Indented,
                                                        new JsonSerializerSettings { DefaultValueHandling = DefaultValueHandling.Ignore });
            IntPtr ptrJsonSearchFaceDataCond = Marshal.StringToHGlobalAnsi(strJsonSearchFaceDataCond);

            CHCNetSDK.NET_DVR_JSON_DATA_CFG struJsonDataCfg = new CHCNetSDK.NET_DVR_JSON_DATA_CFG();
            struJsonDataCfg.dwSize = (uint)Marshal.SizeOf(struJsonDataCfg);
            struJsonDataCfg.lpJsonData = ptrJsonSearchFaceDataCond;
            struJsonDataCfg.dwJsonDataSize = (uint)strJsonSearchFaceDataCond.Length;
            if (!File.Exists(textBoxFilePath.Text)) 
            {
                MessageBox.Show("The picture does not exist!");
                Marshal.FreeHGlobal(ptrJsonSearchFaceDataCond);
                return;
            }
            FileStream fs = new FileStream(textBoxFilePath.Text, FileMode.OpenOrCreate);
            if (0 == fs.Length)
            {
                MessageBox.Show("The picture is 0k,please input another picture!");
                Marshal.FreeHGlobal(ptrJsonSearchFaceDataCond);
                fs.Close();
                return;
            }
            if (200 * 1024 < fs.Length)
            {
                MessageBox.Show("The picture is larger than 200k,please input another picture!");
                Marshal.FreeHGlobal(ptrJsonSearchFaceDataCond);
                fs.Close();
                return;
            }
            struJsonDataCfg.dwPicDataSize = (uint)fs.Length;
            int iLen = (int)struJsonDataCfg.dwPicDataSize;
            byte[] by = new byte[iLen];
            struJsonDataCfg.lpPicData = Marshal.AllocHGlobal(iLen);
            fs.Read(by, 0, iLen);
            Marshal.Copy(by, 0, struJsonDataCfg.lpPicData, iLen);
            fs.Close();
            IntPtr ptrJsonDataCfg = Marshal.AllocHGlobal((int)struJsonDataCfg.dwSize);
            Marshal.StructureToPtr(struJsonDataCfg, ptrJsonDataCfg, false);

            IntPtr ptrJsonResponseStatus = Marshal.AllocHGlobal(1024);
            for (int i = 0; i < 1024; i++)
            {
                Marshal.WriteByte(ptrJsonResponseStatus, i, 0);
            }
            int dwState = (int)CHCNetSDK.NET_SDK_GET_NEXT_STATUS_SUCCESS;
            uint dwReturned = 0;
            while(true)
            {
                dwState = CHCNetSDK.NET_DVR_SendWithRecvRemoteConfig(m_lSetFaceCfgHandle, ptrJsonDataCfg, struJsonDataCfg.dwSize, ptrJsonResponseStatus, 1024, ref dwReturned);
                string strResponseStatus = Marshal.PtrToStringAnsi(ptrJsonResponseStatus);
                if (dwState == (int)CHCNetSDK.NET_SDK_SENDWITHRECV_STATUS.NET_SDK_CONFIG_STATUS_NEEDWAIT)
                {
                    Thread.Sleep(10);
                    continue;
                }
                else if (dwState == (int)CHCNetSDK.NET_SDK_SENDWITHRECV_STATUS.NET_SDK_CONFIG_STATUS_FAILED)
                {
                    MessageBox.Show("Set Face Error:" + CHCNetSDK.NET_DVR_GetLastError());
                    break;
                }
                else if (dwState == (int)CHCNetSDK.NET_SDK_SENDWITHRECV_STATUS.NET_SDK_CONFIG_STATUS_SUCCESS)
                {
                    CResponseStatus JsonResponseStatus = new CResponseStatus();
                    JsonResponseStatus = JsonConvert.DeserializeObject<CResponseStatus>(strResponseStatus);
                    if (JsonResponseStatus.statusCode == 1)
                    {
                        MessageBox.Show("Set Face Success");
                    }
                    else
                    {
                        MessageBox.Show("Set Face Fail, ResponseStatus.statusCode = " + JsonResponseStatus.statusCode);
                    }
                    break;
                }
                else if (dwState == (int)CHCNetSDK.NET_SDK_SENDWITHRECV_STATUS.NET_SDK_CONFIG_STATUS_EXCEPTION)
                {
                    MessageBox.Show("Set Face Exception Error:" + CHCNetSDK.NET_DVR_GetLastError());
                    break;
                }
                else
                {
                    MessageBox.Show("unknown Status Error:" + CHCNetSDK.NET_DVR_GetLastError());
                    break;
                }
            }
            if (m_lSetFaceCfgHandle > 0)
            {
                CHCNetSDK.NET_DVR_StopRemoteConfig(m_lSetFaceCfgHandle);
                m_lSetFaceCfgHandle = -1;
            }
            Marshal.FreeHGlobal(ptrJsonDataCfg);
            Marshal.FreeHGlobal(ptrJsonResponseStatus);
        }
        
        private void btnGet_Click(object sender, EventArgs e)
        {
            if(m_lGetFaceCfgHandle!=-1)
            {
                CHCNetSDK.NET_DVR_StopRemoteConfig(m_lGetFaceCfgHandle);
                m_lGetFaceCfgHandle = -1;
            }

            if(pictureBoxFace.Image!=null)
            {
                pictureBoxFace.Image.Dispose();
                pictureBoxFace.Image = null;
            }
            string sURL = "POST /ISAPI/Intelligent/FDLib/FDSearch?format=json";
            IntPtr ptrURL = Marshal.StringToHGlobalAnsi(sURL);
            m_lGetFaceCfgHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_UserID, CHCNetSDK.NET_DVR_FACE_DATA_SEARCH, ptrURL, sURL.Length, null, IntPtr.Zero);
            if (m_lGetFaceCfgHandle == -1)
            {
                Marshal.FreeHGlobal(ptrURL);
                MessageBox.Show("NET_DVR_StartRemoteConfig fail [url:PUT /ISAPI/AccessControl/UserInfo/SetUp?format=json] error:" + CHCNetSDK.NET_DVR_GetLastError());
                return;
            }
            Marshal.FreeHGlobal(ptrURL);

            CSearchFaceDataCond JsonSearchFaceDataCond = new CSearchFaceDataCond();
            JsonSearchFaceDataCond.searchResultPosition = 0;
            JsonSearchFaceDataCond.maxResults = 1;
            JsonSearchFaceDataCond.faceLibType = "blackFD";
            JsonSearchFaceDataCond.FDID = "1";
            JsonSearchFaceDataCond.FPID = textBoxEmployeeNo.Text;
            string strJsonSearchFaceDataCond = JsonConvert.SerializeObject(JsonSearchFaceDataCond, Formatting.Indented,
                                                        new JsonSerializerSettings { NullValueHandling = NullValueHandling.Ignore });
            IntPtr ptrJsonSearchFaceDataCond = Marshal.StringToHGlobalAnsi(strJsonSearchFaceDataCond);

            CHCNetSDK.NET_DVR_JSON_DATA_CFG struJsonDataCfg = new CHCNetSDK.NET_DVR_JSON_DATA_CFG();
            struJsonDataCfg.dwSize = (uint)Marshal.SizeOf(struJsonDataCfg);
            IntPtr ptrJsonDataCfg = Marshal.AllocHGlobal((int)struJsonDataCfg.dwSize);
            Marshal.StructureToPtr(struJsonDataCfg, ptrJsonDataCfg, false);

            int dwState = (int)CHCNetSDK.NET_SDK_GET_NEXT_STATUS_SUCCESS;
            uint dwReturned = 0;
            while (true)
            {
                dwState = CHCNetSDK.NET_DVR_SendWithRecvRemoteConfig(m_lGetFaceCfgHandle, ptrJsonSearchFaceDataCond, (uint)strJsonSearchFaceDataCond.Length, ptrJsonDataCfg, (uint)Marshal.SizeOf(struJsonDataCfg), ref dwReturned);
                if (dwState == CHCNetSDK.NET_SDK_GET_NEXT_STATUS_SUCCESS)
                {
                    ProcessFaceData(ptrJsonDataCfg);
                    break;
                }
                else if (dwState == CHCNetSDK.NET_SDK_GET_NEXT_STATUS_FAILED)
                {
                    MessageBox.Show("FAILED error:" + CHCNetSDK.NET_DVR_GetLastError());
                    break;
                }
                else 
                {
                    MessageBox.Show("exception error:" + CHCNetSDK.NET_DVR_GetLastError());
                    break;
                }
            }
            if (m_lGetFaceCfgHandle > 0)
            {
                CHCNetSDK.NET_DVR_StopRemoteConfig(m_lGetFaceCfgHandle);
                m_lGetFaceCfgHandle = -1;
            }
            Marshal.FreeHGlobal(ptrJsonSearchFaceDataCond);
            Marshal.FreeHGlobal(ptrJsonDataCfg);
        }

        private void ProcessFaceData(IntPtr lpBuffer)
        {
            CHCNetSDK.NET_DVR_JSON_DATA_CFG m_struJsonDataCfg = (CHCNetSDK.NET_DVR_JSON_DATA_CFG)Marshal.PtrToStructure(lpBuffer, typeof(CHCNetSDK.NET_DVR_JSON_DATA_CFG));
            string strSearchFaceDataReturn = Marshal.PtrToStringAnsi((IntPtr)m_struJsonDataCfg.lpJsonData, (int)m_struJsonDataCfg.dwJsonDataSize);

            CSearchFaceDataReturn m_JsonSearchFaceDataReturn;
            m_JsonSearchFaceDataReturn = JsonConvert.DeserializeObject<CSearchFaceDataReturn>(strSearchFaceDataReturn);

            if (pictureBoxFace.Image != null)
            {
                pictureBoxFace.Image.Dispose();
                pictureBoxFace.Image = null;
            }

            if (m_JsonSearchFaceDataReturn.totalMatches == 0)
            {
                MessageBox.Show("not exist face");
                return;
            }

            try
            {
                string strpath = string.Format("FacePicture.jpg");
                using (FileStream fs = new FileStream(strpath, FileMode.OpenOrCreate))
                {
                    int FaceLen = (int)m_struJsonDataCfg.dwPicDataSize;
                    byte[] by = new byte[FaceLen];
                    Marshal.Copy(m_struJsonDataCfg.lpPicData, by, 0, FaceLen);
                    fs.Write(by, 0, FaceLen);
                    fs.Close();
                }
                pictureBoxFace.Image = Image.FromFile(strpath);
                textBoxFilePath.Text = string.Format("{0}\\{1}", Environment.CurrentDirectory, strpath);
            }
            catch
            {
                CHCNetSDK.NET_DVR_StopRemoteConfig(m_lGetFaceCfgHandle);
                MessageBox.Show("ProcessFingerData failed", "Error", MessageBoxButtons.OK);
            }
        }

        private void btnCapture_Click(object sender, EventArgs e)
        {
            if(m_lCapFaceCfgHandle!=-1)
            {
                CHCNetSDK.NET_DVR_StopRemoteConfig(m_lCapFaceCfgHandle);
                m_lCapFaceCfgHandle = -1;
            }
            if (pictureBoxFace.Image != null)
            {
                pictureBoxFace.Image.Dispose();
                pictureBoxFace.Image = null;
            }
            textBoxFilePath.Text = "";

            CHCNetSDK.NET_DVR_CAPTURE_FACE_COND struCond = new CHCNetSDK.NET_DVR_CAPTURE_FACE_COND();
            struCond.init();
            struCond.dwSize = Marshal.SizeOf(struCond);
            int dwInBufferSize=struCond.dwSize;
            IntPtr ptrStruCond = Marshal.AllocHGlobal(dwInBufferSize);
            Marshal.StructureToPtr(struCond, ptrStruCond, false);
            m_lCapFaceCfgHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_UserID, CHCNetSDK.NET_DVR_CAPTURE_FACE_INFO,ptrStruCond, dwInBufferSize, null, IntPtr.Zero);
            if(-1==m_lCapFaceCfgHandle)
            {
                Marshal.FreeHGlobal(ptrStruCond);
                MessageBox.Show("NET_DVR_CAP_FACE_FAIL, ERROR CODE" + CHCNetSDK.NET_DVR_GetLastError().ToString(), "Error", MessageBoxButtons.OK);
                return;
            }

            CHCNetSDK.NET_DVR_CAPTURE_FACE_CFG struFaceCfg = new CHCNetSDK.NET_DVR_CAPTURE_FACE_CFG();
            struFaceCfg.init();
            int dwStatus = 0;
            int dwOutBuffSize=Marshal.SizeOf(struFaceCfg);
            bool Flag = true;
            while(Flag)
            {
                dwStatus = CHCNetSDK.NET_DVR_GetNextRemoteConfig(m_lCapFaceCfgHandle, ref struFaceCfg, dwOutBuffSize);
                switch (dwStatus)
                {
                    case CHCNetSDK.NET_SDK_GET_NEXT_STATUS_SUCCESS://成功读取到数据，处理完本次数据后需调用next
                        ProcessCapFaceData(ref struFaceCfg, ref Flag);
                        break;
                    case CHCNetSDK.NET_SDK_GET_NEXT_STATUS_NEED_WAIT:
                        break;
                    case CHCNetSDK.NET_SDK_GET_NEXT_STATUS_FAILED:
                        CHCNetSDK.NET_DVR_StopRemoteConfig(m_lCapFaceCfgHandle);
                        MessageBox.Show("NET_SDK_GET_NEXT_STATUS_FAILED" + CHCNetSDK.NET_DVR_GetLastError().ToString(), "Error", MessageBoxButtons.OK);
                        Flag = false;
                        break;
                    case CHCNetSDK.NET_SDK_GET_NEXT_STATUS_FINISH:
                        CHCNetSDK.NET_DVR_StopRemoteConfig(m_lCapFaceCfgHandle);
                        Flag = false;
                        break;
                    default:
                        MessageBox.Show("NET_SDK_GET_STATUS_UNKOWN" + CHCNetSDK.NET_DVR_GetLastError().ToString(), "Error", MessageBoxButtons.OK);
                        Flag = false;
                        CHCNetSDK.NET_DVR_StopRemoteConfig(m_lCapFaceCfgHandle);
                        break;
                }
            }
            Marshal.FreeHGlobal(ptrStruCond);
        }

        private void ProcessCapFaceData(ref CHCNetSDK.NET_DVR_CAPTURE_FACE_CFG struFaceCfg,ref bool flag)
        {
            if(0==struFaceCfg.dwFacePicSize)
            {
                return;
            }
            string strpath = null;
            DateTime dt = DateTime.Now;
            strpath = string.Format("capture.jpg", Environment.CurrentDirectory);
            try
            {
                using(FileStream fs = new FileStream(strpath, FileMode.OpenOrCreate))
                {
                    int FaceLen = struFaceCfg.dwFacePicSize;
                    byte[] by = new byte[FaceLen];
                    Marshal.Copy(struFaceCfg.pFacePicBuffer, by, 0, FaceLen);
                    fs.Write(by, 0, FaceLen);
                    fs.Close();
                }
                
                pictureBoxFace.Image = Image.FromFile(strpath);
                textBoxFilePath.Text = string.Format("{0}\\{1}", Environment.CurrentDirectory, strpath);
                MessageBox.Show("Capture succeed", "SUCCESSFUL", MessageBoxButtons.OK);
            }
            catch
            {
                flag = false;
                MessageBox.Show("capature data wrong", "Error", MessageBoxButtons.OK);
            }
        }
        private void btnDel_Click(object sender, EventArgs e)
        {

            if (pictureBoxFace.Image != null)
            {
                pictureBoxFace.Image.Dispose();
                pictureBoxFace.Image = null;
            }
            textBoxFilePath.Text = "";
            IntPtr ptrOutBuf = Marshal.AllocHGlobal(1024);
            IntPtr ptrStatusBuffer = Marshal.AllocHGlobal(1024);
            for (int i = 0; i < 1024; i++)
            {
                Marshal.WriteByte(ptrOutBuf, i, 0);
                Marshal.WriteByte(ptrStatusBuffer, i, 0);
            }
            CHCNetSDK.NET_DVR_XML_CONFIG_INPUT struInput = new CHCNetSDK.NET_DVR_XML_CONFIG_INPUT();
            CHCNetSDK.NET_DVR_XML_CONFIG_OUTPUT struOuput = new CHCNetSDK.NET_DVR_XML_CONFIG_OUTPUT();

            string sUrl = "PUT /ISAPI/Intelligent/FDLib/FDSearch/Delete?format=json&FDID=" + "1" + "&faceLibType=" + "blackFD";
            IntPtr ptrURL = Marshal.StringToHGlobalAnsi(sUrl);
            struInput.dwSize = (uint)Marshal.SizeOf(struInput);
            struInput.lpRequestUrl = ptrURL;
            struInput.dwRequestUrlLen = (uint)sUrl.Length;

            CFaceRecordDelete JsonFaceRecordDelete = new CFaceRecordDelete();
            JsonFaceRecordDelete.FPID = new List<CFPID>();
            CFPID singleFPID = new CFPID();
            singleFPID.value = textBoxEmployeeNo.Text;
            JsonFaceRecordDelete.FPID.Add(singleFPID);

            string strFaceRecordDelete = JsonConvert.SerializeObject(JsonFaceRecordDelete);
            IntPtr ptrInBuffer = Marshal.StringToHGlobalAnsi(strFaceRecordDelete);

            struInput.lpInBuffer = ptrInBuffer;
            struInput.dwInBufferSize = (uint)strFaceRecordDelete.Length;

            struOuput.dwSize = (uint)Marshal.SizeOf(struOuput);
            struOuput.lpOutBuffer = ptrOutBuf;
            struOuput.dwOutBufferSize = 1024;
            struOuput.lpStatusBuffer = ptrStatusBuffer;
            struOuput.dwStatusSize = 1024;

            IntPtr ptrInput = Marshal.AllocHGlobal(Marshal.SizeOf(struInput));
            Marshal.StructureToPtr(struInput, ptrInput, false);
            IntPtr ptrOuput = Marshal.AllocHGlobal(Marshal.SizeOf(struOuput));
            Marshal.StructureToPtr(struOuput, ptrOuput, false);
            if (!CHCNetSDK.NET_DVR_STDXMLConfig(m_UserID, ptrInput, ptrOuput))
            {
                string strTemp = string.Format("Delete Face Fail , Error:", CHCNetSDK.NET_DVR_GetLastError());
                MessageBox.Show("Delete Face Fail , Error:" + CHCNetSDK.NET_DVR_GetLastError());
            }
            else
            {
                string strResponseStatus = Marshal.PtrToStringAnsi(struOuput.lpOutBuffer);
                CResponseStatus JsonResponseStatus = new CResponseStatus();
                JsonResponseStatus = JsonConvert.DeserializeObject<CResponseStatus>(strResponseStatus);
                if (JsonResponseStatus.statusCode == 1)
                {
                    MessageBox.Show("Delete Face Success");
                }
                else
                {
                    MessageBox.Show("Delete Face Fail , Error:" + CHCNetSDK.NET_DVR_GetLastError());
                }
            }

            Marshal.FreeHGlobal(ptrOutBuf);
            Marshal.FreeHGlobal(ptrStatusBuffer);
            Marshal.FreeHGlobal(ptrInBuffer);
            Marshal.FreeHGlobal(ptrInput);
            Marshal.FreeHGlobal(ptrOuput);
            Marshal.FreeHGlobal(ptrURL);
        }

        private void comboBoxLanguage_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (comboBoxLanguage.Text != null)
            {
                MultiLanguage.SetDefaultLanguage(comboBoxLanguage.Text);
                foreach (Form form in Application.OpenForms)
                {
                    MultiLanguage.LoadLanguage(form);
                }


                if (comboBoxLanguage.Text == "English")
                {
                    Thread.CurrentThread.CurrentUICulture = new CultureInfo("en-US");
                }
                else if (comboBoxLanguage.Text == "Chinese")
                {
                    Thread.CurrentThread.CurrentUICulture = new CultureInfo("zh-CN");
                }
            }
        }

        private void btnClear_Click(object sender, EventArgs e)
        {
            if (pictureBoxFace.Image != null)
            {
                pictureBoxFace.Image.Dispose();
                pictureBoxFace.Image = null;
            }
            textBoxFilePath.Text = "";
        }
    }
}
