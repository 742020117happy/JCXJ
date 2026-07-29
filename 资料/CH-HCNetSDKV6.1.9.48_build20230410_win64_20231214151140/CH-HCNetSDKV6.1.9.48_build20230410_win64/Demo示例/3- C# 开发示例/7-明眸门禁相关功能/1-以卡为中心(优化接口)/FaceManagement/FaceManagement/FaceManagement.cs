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

            CHCNetSDK.NET_DVR_FACE_COND struCond = new CHCNetSDK.NET_DVR_FACE_COND();
            struCond.init();
            struCond.dwSize = Marshal.SizeOf(struCond);
            struCond.dwFaceNum = 1;
            int.TryParse(textBoxCardReaderNo.Text.ToString(), out struCond.dwEnableReaderNo);
            byte[] byTemp = System.Text.Encoding.UTF8.GetBytes(textBoxCardNo.Text);
            for (int i = 0; i < byTemp.Length; i++)
            {
                struCond.byCardNo[i] = byTemp[i];
            }

            int dwInBufferSize=struCond.dwSize;
            IntPtr ptrstruCond=Marshal.AllocHGlobal(dwInBufferSize);
            Marshal.StructureToPtr(struCond,ptrstruCond,false);
            m_lSetFaceCfgHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_UserID, CHCNetSDK.NET_DVR_SET_FACE, ptrstruCond, dwInBufferSize, null, IntPtr.Zero);
            if(-1==m_lSetFaceCfgHandle)
            {
                Marshal.FreeHGlobal(ptrstruCond);
                MessageBox.Show("NET_DVR_SET_FACE_FAIL, ERROR CODE" + CHCNetSDK.NET_DVR_GetLastError().ToString(), "Error", MessageBoxButtons.OK);
                return;
            }

            CHCNetSDK.NET_DVR_FACE_RECORD struRecord = new CHCNetSDK.NET_DVR_FACE_RECORD();
            struRecord.init();
            struRecord.dwSize = Marshal.SizeOf(struRecord);

            byte[] byRecordNo = System.Text.Encoding.UTF8.GetBytes(textBoxCardNo.Text);
            for (int i = 0; i < byRecordNo.Length; i++)
            {
                struRecord.byCardNo[i] = byRecordNo[i];
            }

            ReadFaceData(ref struRecord);
            int dwInBuffSize=Marshal.SizeOf(struRecord);
            int dwStatus = 0;
            
            CHCNetSDK.NET_DVR_FACE_STATUS struStatus=new CHCNetSDK.NET_DVR_FACE_STATUS();
            struStatus.init();
            struStatus.dwSize=Marshal.SizeOf(struStatus);
            int dwOutBuffSize=struStatus.dwSize;
            IntPtr ptrOutDataLen=Marshal.AllocHGlobal(sizeof(int));
            bool Flag = true;
            while(Flag)
            {
                dwStatus = CHCNetSDK.NET_DVR_SendWithRecvRemoteConfig(m_lSetFaceCfgHandle, ref struRecord, dwInBuffSize, ref struStatus, dwOutBuffSize, ptrOutDataLen);
                switch (dwStatus)
                {
                    case CHCNetSDK.NET_SDK_GET_NEXT_STATUS_SUCCESS://成功读取到数据，处理完本次数据后需调用next
                        ProcessSetFaceData(ref struStatus, ref Flag);
                        break;
                    case CHCNetSDK.NET_SDK_GET_NEXT_STATUS_NEED_WAIT:
                        break;
                    case CHCNetSDK.NET_SDK_GET_NEXT_STATUS_FAILED:
                        CHCNetSDK.NET_DVR_StopRemoteConfig(m_lSetFaceCfgHandle);
                        MessageBox.Show("NET_SDK_GET_NEXT_STATUS_FAILED" + CHCNetSDK.NET_DVR_GetLastError().ToString(), "Error", MessageBoxButtons.OK);
                        Flag = false;
                        break;
                    case CHCNetSDK.NET_SDK_GET_NEXT_STATUS_FINISH:
                        CHCNetSDK.NET_DVR_StopRemoteConfig(m_lSetFaceCfgHandle);
                        Flag = false;
                        break;
                    default:
                        MessageBox.Show("NET_SDK_GET_NEXT_STATUS_UNKOWN" + CHCNetSDK.NET_DVR_GetLastError().ToString(), "Error", MessageBoxButtons.OK);
                        Flag = false;
                        CHCNetSDK.NET_DVR_StopRemoteConfig(m_lSetFaceCfgHandle);
                        break;
                }
            }

            Marshal.FreeHGlobal(ptrstruCond);
            Marshal.FreeHGlobal(ptrOutDataLen);
        }

        private void ProcessSetFaceData(ref CHCNetSDK.NET_DVR_FACE_STATUS struStatus ,ref bool flag)
        {
            switch(struStatus.byRecvStatus)
            {
                case 1:
                    MessageBox.Show("SetFaceDataSuccessful", "Succeed", MessageBoxButtons.OK);
                    break;
                default:
                    flag = false;
                    MessageBox.Show("NET_SDK_SET_Face_DATA_FAILED" + struStatus.byRecvStatus.ToString(), "ERROR", MessageBoxButtons.OK);
                    break;
            }
            
        }

        private void ReadFaceData(ref CHCNetSDK.NET_DVR_FACE_RECORD struRecord)
        {
            if (!File.Exists(textBoxFilePath.Text))
            {
                MessageBox.Show("The face picture does not exist!");
                return;
            }
            FileStream fs = new FileStream(textBoxFilePath.Text, FileMode.OpenOrCreate);
            if (0 == fs.Length)
            {
                MessageBox.Show("The face picture is 0k,please input another picture!");
                return;
            }
            if (200 * 1024 < fs.Length)
            {
                MessageBox.Show("The face picture is larger than 200k,please input another picture!");
                return;
            }
            try
            {
                int.TryParse(fs.Length.ToString(),out struRecord.dwFaceLen);
                int iLen = struRecord.dwFaceLen;
                byte[] by = new byte[iLen];
                struRecord.pFaceBuffer = Marshal.AllocHGlobal(iLen);
                fs.Read(by, 0, iLen);
                Marshal.Copy(by, 0, struRecord.pFaceBuffer, iLen);
                fs.Close();
                textBoxFilePath.Text = "";
            }
            catch
            {
                MessageBox.Show("Read Face Data failed");
                fs.Close();
                return;
            }
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

            CHCNetSDK.NET_DVR_FACE_COND struCond = new CHCNetSDK.NET_DVR_FACE_COND();
            struCond.init();
            struCond.dwSize = Marshal.SizeOf(struCond);
            int dwSize = struCond.dwSize;
            if (textBoxCardReaderNo.Text.ToString()=="")
            {
                struCond.dwEnableReaderNo =0;
            }
            else
            {
                 int.TryParse(textBoxCardReaderNo.Text.ToString(),out struCond.dwEnableReaderNo);
            }
            struCond.dwFaceNum = 1;//人脸数量是1
            byte[] byTemp = System.Text.Encoding.UTF8.GetBytes(textBoxCardNo.Text);
            for (int i = 0; i < byTemp.Length; i++)
            {
                struCond.byCardNo[i] = byTemp[i];
            }

            IntPtr ptrStruCond = Marshal.AllocHGlobal(dwSize);
            Marshal.StructureToPtr(struCond, ptrStruCond, false);

            m_lGetFaceCfgHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_UserID, CHCNetSDK.NET_DVR_GET_FACE, ptrStruCond, dwSize, null, IntPtr.Zero);
            if(m_lGetFaceCfgHandle==-1)
            {
                Marshal.FreeHGlobal(ptrStruCond);
                MessageBox.Show("NET_DVR_GET_FACE_FAIL, ERROR CODE" + CHCNetSDK.NET_DVR_GetLastError().ToString(), "Error", MessageBoxButtons.OK);
                return;
            }

            bool Flag = true;
            int dwStatus = 0;

            CHCNetSDK.NET_DVR_FACE_RECORD struRecord=new CHCNetSDK.NET_DVR_FACE_RECORD();
            struRecord.init();
            struRecord.dwSize=Marshal.SizeOf(struRecord);
            int dwOutBuffSize=struRecord.dwSize;
            while(Flag)
            {
                dwStatus = CHCNetSDK.NET_DVR_GetNextRemoteConfig(m_lGetFaceCfgHandle, ref struRecord, dwOutBuffSize);
                switch(dwStatus)
                {
                    case CHCNetSDK.NET_SDK_GET_NEXT_STATUS_SUCCESS://成功读取到数据，处理完本次数据后需调用next
                        ProcessFaceData(ref struRecord, ref Flag);
                        break;
                    case CHCNetSDK.NET_SDK_GET_NEXT_STATUS_NEED_WAIT:
                        break;
                    case CHCNetSDK.NET_SDK_GET_NEXT_STATUS_FAILED:
                        CHCNetSDK.NET_DVR_StopRemoteConfig(m_lGetFaceCfgHandle);
                        MessageBox.Show("NET_SDK_GET_NEXT_STATUS_FAILED" + CHCNetSDK.NET_DVR_GetLastError().ToString(), "Error", MessageBoxButtons.OK);
                        Flag = false;
                        break;
                    case CHCNetSDK.NET_SDK_GET_NEXT_STATUS_FINISH:
                        MessageBox.Show("NET_SDK_GET_NEXT_STATUS_FINISH", "Tips", MessageBoxButtons.OK);
                        CHCNetSDK.NET_DVR_StopRemoteConfig(m_lGetFaceCfgHandle);
                        Flag = false;
                        break;
                    default:
                        MessageBox.Show("NET_SDK_GET_STATUS_UNKOWN" + CHCNetSDK.NET_DVR_GetLastError().ToString(), "Error", MessageBoxButtons.OK);
                        Flag = false;
                        CHCNetSDK.NET_DVR_StopRemoteConfig(m_lGetFaceCfgHandle);
                        break;
                }
            }

            Marshal.FreeHGlobal(ptrStruCond);
        }

        private void ProcessFaceData(ref CHCNetSDK.NET_DVR_FACE_RECORD struRecord,ref Boolean Flag)
        {
            string strpath = null;
            DateTime dt = DateTime.Now;
            strpath = string.Format("FacePicture.jpg");

            if(0==struRecord.dwFaceLen)
            {
                return;
            }

            if (pictureBoxFace.Image != null)
            {
                pictureBoxFace.Image.Dispose();
                pictureBoxFace.Image = null;
            }

            try
            {
                using (FileStream fs = new FileStream(strpath, FileMode.OpenOrCreate))
                {
                    int FaceLen = struRecord.dwFaceLen;
                    byte[] by = new byte[FaceLen];
                    Marshal.Copy(struRecord.pFaceBuffer, by, 0, FaceLen);
                    fs.Write(by, 0, FaceLen);
                    fs.Close();
                }
                pictureBoxFace.Image = Image.FromFile(strpath);
                textBoxFilePath.Text = string.Format("{0}\\{1}", Environment.CurrentDirectory, strpath);
            }
            catch
            {
                Flag = false;
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
            CHCNetSDK.NET_DVR_FACE_PARAM_CTRL_CARDNO struCardNo = new CHCNetSDK.NET_DVR_FACE_PARAM_CTRL_CARDNO();
            struCardNo.Init();
            struCardNo.dwSize = Marshal.SizeOf(struCardNo);
            struCardNo.byMode = 0;
            int dwSize = struCardNo.dwSize;
            byte[] byCardNo = System.Text.Encoding.UTF8.GetBytes(textBoxCardNo.Text);
            for (int i = 0; i < byCardNo.Length; i++)
            {
                struCardNo.struByCard.byCardNo[i] = byCardNo[i];
            }

            int dwEnableReaderNo = 1;
            int.TryParse(textBoxCardReaderNo.Text, out dwEnableReaderNo);
            if (dwEnableReaderNo <= 0) dwEnableReaderNo = 1;

            struCardNo.struByCard.byEnableCardReader[dwEnableReaderNo-1] = 1;

            for(int i=0;i<CHCNetSDK.MAX_FACE_NUM;++i)
            {
                struCardNo.struByCard.byFaceID[i] = 1;//全部写1删除人脸
            }

            if(false==CHCNetSDK.NET_DVR_RemoteControl(m_UserID,CHCNetSDK.NET_DVR_DEL_FACE_PARAM_CFG,ref struCardNo,dwSize))
            {
                MessageBox.Show("NET_SDK_DEL_FACE_FAILED" + CHCNetSDK.NET_DVR_GetLastError().ToString(), "Error", MessageBoxButtons.OK);
            }
            else
            {
                MessageBox.Show("NET_SDK_DEL_FACE_SUCCEED", "succeed", MessageBoxButtons.OK);
            }
        }

        private void textBoxCardReaderNo_TextChanged(object sender, EventArgs e)
        {

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
