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

using FingerPrintMgr.Language;

namespace FingerPrintManagement
{
    public partial class FingerPrintMgr : Form
    {
        public FingerPrintMgr()
        {
            InitializeComponent();
            CHCNetSDK.NET_DVR_Init();
            CHCNetSDK.NET_DVR_SetLogToFile(3, "./SdkLog/", true);
            comboBoxLanguage.SelectedIndex = 0;
        }

        private int m_UserID = -1;
        public int m_lGetFingerPrintCfgHandle = -1;
        public int m_lSetFingerPrintCfgHandle = -1;
        public int m_lDelFingerPrintCfHandle = -1;
        public int m_lCapFingerPrintCfHandle = -1;

        private void button1_Click(object sender, EventArgs e)
        {
            AddDev deviceAdd = new AddDev();
            deviceAdd.ShowDialog();
            m_UserID = deviceAdd.m_iUserID;
            deviceAdd.Dispose();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.InitialDirectory = Environment.CurrentDirectory;
            openFileDialog.Filter = "Fingerprint file|*.dat|All documents|*.*";
            openFileDialog.RestoreDirectory = true;
            openFileDialog.FilterIndex = 1;
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                textBoxFingerData.Text = openFileDialog.FileName;
            }
        }

        private void button5_Click(object sender, EventArgs e)
        {
            if(m_lSetFingerPrintCfgHandle!=-1)
            {
                CHCNetSDK.NET_DVR_StopRemoteConfig((int)m_lSetFingerPrintCfgHandle);
                m_lSetFingerPrintCfgHandle = -1;
            }

            CHCNetSDK.NET_DVR_FINGERPRINT_COND strupond = new CHCNetSDK.NET_DVR_FINGERPRINT_COND();
            strupond.init();
            int dwSize = Marshal.SizeOf(strupond);
            strupond.dwSize = dwSize;
            byte.TryParse(textBoxFingerID.Text, out strupond.byFingerPrintID);
            int.TryParse(textBoxCardReaderNo.Text,out strupond.dwEnableReaderNo);
            strupond.dwFingerprintNum = 1;//指纹数量写死的
            byte[] byTempptrRec = System.Text.Encoding.UTF8.GetBytes(textBoxCardNo.Text);
            for (int i = 0; i < byTempptrRec.Length; i++)
            {
                strupond.byCardNo[i] = byTempptrRec[i];
            }

            IntPtr ptrStrucond = Marshal.AllocHGlobal(dwSize);
            Marshal.StructureToPtr(strupond, ptrStrucond, false);

            m_lSetFingerPrintCfgHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_UserID, CHCNetSDK.NET_DVR_SET_FINGERPRINT, ptrStrucond, dwSize,null,IntPtr.Zero);
            if(-1==m_lSetFingerPrintCfgHandle)
            {
                Marshal.FreeHGlobal(ptrStrucond);
                MessageBox.Show("NET_DVR_SET_FINGERPRINT_CFG_V50 FAIL, ERROR CODE" + CHCNetSDK.NET_DVR_GetLastError().ToString(), "Error", MessageBoxButtons.OK);
                return;
            }

            Boolean Flag = true;
            int dwStatus = 0;
            CHCNetSDK.NET_DVR_FINGERPRINT_RECORD StruRecord = new CHCNetSDK.NET_DVR_FINGERPRINT_RECORD();
            StruRecord.init();
            int dwInBuffSize = Marshal.SizeOf(StruRecord);
            StruRecord.dwSize = dwInBuffSize;
            byte.TryParse(textBoxFingerID.Text, out StruRecord.byFingerPrintID);
            int.TryParse(textBoxCardReaderNo.Text,out StruRecord.dwEnableReaderNo);

            byte[] byTemp = System.Text.Encoding.UTF8.GetBytes(textBoxCardNo.Text);
            for (int i = 0; i < byTemp.Length; i++)
            {
                StruRecord.byCardNo[i] = byTemp[i];
            }
            ReadFingerData(ref StruRecord);

            CHCNetSDK.NET_DVR_FINGERPRINT_STATUS StruStatus = new CHCNetSDK.NET_DVR_FINGERPRINT_STATUS();
            StruStatus.init();
            int dwOutBuffSize = Marshal.SizeOf(StruStatus);
            StruStatus.dwSize = dwOutBuffSize;
            IntPtr ptrOutDataLen=Marshal.AllocHGlobal(sizeof(int));


            while(Flag)
            {
                dwStatus = CHCNetSDK.NET_DVR_SendWithRecvRemoteConfig(m_lSetFingerPrintCfgHandle, ref StruRecord, dwInBuffSize, ref StruStatus, dwOutBuffSize, ptrOutDataLen);
                switch(dwStatus)
                {
                    case CHCNetSDK.NET_SDK_GET_NEXT_STATUS_SUCCESS://成功读取到数据，处理完本次数据后需调用next
                        ProcessSetFingerData(ref StruStatus, ref Flag);
                        break;
                    case CHCNetSDK.NET_SDK_GET_NEXT_STATUS_NEED_WAIT:
                        break;
                    case CHCNetSDK.NET_SDK_GET_NEXT_STATUS_FAILED:
                        CHCNetSDK.NET_DVR_StopRemoteConfig(m_lSetFingerPrintCfgHandle);
                        MessageBox.Show("NET_SDK_GET_NEXT_STATUS_FAILED" + CHCNetSDK.NET_DVR_GetLastError().ToString(), "Error", MessageBoxButtons.OK);
                        Flag = false;
                        break;
                    case CHCNetSDK.NET_SDK_GET_NEXT_STATUS_FINISH:
                        CHCNetSDK.NET_DVR_StopRemoteConfig(m_lSetFingerPrintCfgHandle);
                        Flag = false;
                        break;
                    default:
                        MessageBox.Show("NET_SDK_GET_NEXT_STATUS_UNKOWN" + CHCNetSDK.NET_DVR_GetLastError().ToString(), "Error", MessageBoxButtons.OK);
                        Flag = false;
                        CHCNetSDK.NET_DVR_StopRemoteConfig(m_lSetFingerPrintCfgHandle);
                        break;
                }
            }

            Marshal.FreeHGlobal(ptrStrucond);
            Marshal.FreeHGlobal(ptrOutDataLen);
        }


        private void ProcessSetFingerData(ref CHCNetSDK.NET_DVR_FINGERPRINT_STATUS ststus,ref bool  flag)
        {
            switch(ststus.byRecvStatus)
            {
                case 0:
                    MessageBox.Show("SetFingegDataSuccessful", "Succeed", MessageBoxButtons.OK);
                    break;
                default:
                    flag = false;
                    MessageBox.Show("NET_SDK_SET_FINGER_DATA_FAILED" +ststus.byRecvStatus.ToString(), "ERROR", MessageBoxButtons.OK);
                    break;
            }
        }
        private void ReadFingerData(ref CHCNetSDK.NET_DVR_FINGERPRINT_RECORD Record)
        {
            try
            {
                using (FileStream fs = new FileStream(textBoxFingerData.Text, FileMode.OpenOrCreate))
                {
                    if (0 == fs.Length)
                    {
                        Record.byFingerData[0] = 0;
                        fs.Close();
                    }
                    Record.dwFingerPrintLen = (int)fs.Length;
                    BinaryReader objBinaryReader = new BinaryReader(fs);
                    if (Record.dwFingerPrintLen > CHCNetSDK.MAX_FINGER_PRINT_LEN)
                    {
                        MessageBox.Show("FingerPrintLen is too long");
                        return;
                    }
                    for (int i = 0; i < Record.dwFingerPrintLen; i++)
                    {
                        if (i >= fs.Length)
                        {
                            break;
                        }
                        Record.byFingerData[i] = objBinaryReader.ReadByte();
                    }
                    fs.Close();
                }
            }
            catch
            {
                if(m_lSetFingerPrintCfgHandle!=-1)
                {
                    CHCNetSDK.NET_DVR_StopRemoteConfig(m_lSetFingerPrintCfgHandle);
                }
                MessageBox.Show("FingerDataPath may be wrong", "Error", MessageBoxButtons.OK);
            }

        }

        private void btnGet_Click(object sender, EventArgs e)
        {
            if(m_lGetFingerPrintCfgHandle!=-1)
            {
                CHCNetSDK.NET_DVR_StopRemoteConfig((int)m_lGetFingerPrintCfgHandle);
                m_lGetFingerPrintCfgHandle = -1;
            }
            textBoxFingerData.Text = "";

            CHCNetSDK.NET_DVR_FINGERPRINT_COND struCond = new CHCNetSDK.NET_DVR_FINGERPRINT_COND();
            struCond.init();
            struCond.dwSize = Marshal.SizeOf(struCond);
            struCond.dwFingerprintNum = 1;
            byte.TryParse(textBoxFingerID.Text, out struCond.byFingerPrintID);
            byte[] byTemp = System.Text.Encoding.UTF8.GetBytes(textBoxCardNo.Text);
            for (int i = 0; i < byTemp.Length; i++)
            {
                struCond.byCardNo[i] = byTemp[i];
            }
            int.TryParse(textBoxCardReaderNo.Text,out struCond.dwEnableReaderNo);
            int dwSize = Marshal.SizeOf(struCond);
            IntPtr ptrStruCond = Marshal.AllocHGlobal(dwSize);
            Marshal.StructureToPtr(struCond, ptrStruCond, false);
            m_lGetFingerPrintCfgHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_UserID, CHCNetSDK.NET_DVR_GET_FINGERPRINT, ptrStruCond, dwSize, null, IntPtr.Zero);
            if(-1==m_lGetFingerPrintCfgHandle)
            {
                Marshal.FreeHGlobal(ptrStruCond);
                MessageBox.Show("NET_DVR_GET_FINGERPRINT_CFG_V50 FAIL, ERROR CODE"+CHCNetSDK.NET_DVR_GetLastError().ToString(), "Error", MessageBoxButtons.OK);
                return;
            }

            Boolean Flag = true;
            CHCNetSDK.NET_DVR_FINGERPRINT_RECORD struOutBuff = new CHCNetSDK.NET_DVR_FINGERPRINT_RECORD();
            struOutBuff.init();
            int dWsize = Marshal.SizeOf(struOutBuff);
            int dwStatus = 0;
            
            while(Flag)
            {
                dwStatus = CHCNetSDK.NET_DVR_GetNextRemoteConfig(m_lGetFingerPrintCfgHandle, ref struOutBuff, dWsize);
                switch(dwStatus)
                {
                    case CHCNetSDK.NET_SDK_GET_NEXT_STATUS_SUCCESS://成功读取到数据，处理完本次数据后需调用next
                        ProcessFingerData(ref struOutBuff, ref Flag);
                        break;
                    case CHCNetSDK.NET_SDK_GET_NEXT_STATUS_NEED_WAIT:
                        break;
                    case CHCNetSDK.NET_SDK_GET_NEXT_STATUS_FAILED:
                        CHCNetSDK.NET_DVR_StopRemoteConfig(m_lGetFingerPrintCfgHandle);
                        MessageBox.Show("NET_SDK_GET_NEXT_STATUS_FAILED"+CHCNetSDK.NET_DVR_GetLastError().ToString(), "Error", MessageBoxButtons.OK);
                        Flag = false;
                        break;
                    case CHCNetSDK.NET_SDK_GET_NEXT_STATUS_FINISH:
                        MessageBox.Show("NET_SDK_GET_NEXT_STATUS_FINISH", "Tips", MessageBoxButtons.OK);
                        CHCNetSDK.NET_DVR_StopRemoteConfig(m_lGetFingerPrintCfgHandle);
                        Flag = false;
                        break;
                    default:
                        MessageBox.Show("NET_SDK_GET_NEXT_STATUS_UNKOWN" + CHCNetSDK.NET_DVR_GetLastError().ToString(), "Error", MessageBoxButtons.OK);
                        CHCNetSDK.NET_DVR_StopRemoteConfig(m_lGetFingerPrintCfgHandle);
                        Flag = false;
                        break;
                }
            }
            Marshal.FreeHGlobal(ptrStruCond);
        }
        private void ProcessFingerData(ref CHCNetSDK.NET_DVR_FINGERPRINT_RECORD struOutBuff,ref bool flag)
        {
            string strpath = null;
            DateTime dt = DateTime.Now;
            strpath = string.Format("{0}\\fingerprint.dat", Environment.CurrentDirectory);
            try
            {
                using (FileStream fs = new FileStream(strpath, FileMode.OpenOrCreate))
                {
                    if (!File.Exists(strpath))
                    {
                        MessageBox.Show("Fingerprint storage file creat failed！");
                    }
                    BinaryWriter objBinaryWrite = new BinaryWriter(fs);
                    fs.Write(struOutBuff.byFingerData, 0, struOutBuff.dwFingerPrintLen);
                    fs.Close();
                }
                textBoxFingerData.Text = strpath;
                MessageBox.Show("Fingerprint GET SUCCEED", "SUCCESSFUL", MessageBoxButtons.OK);
            }
            catch
            {
                MessageBox.Show("Fingerprint process failed");
                flag = false;
            }
        }

        private void btnDel_Click(object sender, EventArgs e)
        {
            if(-1!=m_lDelFingerPrintCfHandle)
            {
                CHCNetSDK.NET_DVR_StopRemoteConfig(m_lDelFingerPrintCfHandle);
                m_lDelFingerPrintCfHandle = -1;
            }


            //这边是联合体，暂收默认卡号人员ID方式删除
            CHCNetSDK.NET_DVR_FINGER_PRINT_INFO_CTRL_V50_ByCardNo struCardNo = new CHCNetSDK.NET_DVR_FINGER_PRINT_INFO_CTRL_V50_ByCardNo();
            struCardNo.init();
            struCardNo.byMode = 0;

            byte[] byTempCardNo = System.Text.Encoding.UTF8.GetBytes(textBoxCardNo.Text);
            ByteCopy(ref byTempCardNo, ref struCardNo.struProcessMode.byCardNo);

            int dwFingerID = 0;
            int.TryParse(textBoxFingerID.Text, out dwFingerID);
            if (dwFingerID > 0 && dwFingerID <= 10)
            {
                struCardNo.struProcessMode.byFingerPrintID[dwFingerID-1] = 1;
            }

            struCardNo.dwSize = Marshal.SizeOf(struCardNo);
            int dwSize = struCardNo.dwSize;

            int dwEnableReaderNo = 1;
            int.TryParse(textBoxCardReaderNo.Text,out dwEnableReaderNo);
            if (dwEnableReaderNo <= 0) dwEnableReaderNo = 1;
            
            // 使能读卡器参数byEnableCardReader[下发的读卡器编号-1] = 1，保证和下发的是同一个读卡器
            struCardNo.struProcessMode.byEnableCardReader[dwEnableReaderNo - 1] = 1;
            IntPtr ptrStruCardNo = Marshal.AllocHGlobal(dwSize);
            Marshal.StructureToPtr(struCardNo, ptrStruCardNo, false);
            m_lDelFingerPrintCfHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_UserID,CHCNetSDK.NET_DVR_DEL_FINGERPRINT,ptrStruCardNo,dwSize,null,IntPtr.Zero);

            if (-1 == m_lDelFingerPrintCfHandle)
            {
                Marshal.FreeHGlobal(ptrStruCardNo);
                MessageBox.Show("NET_DVR_DEL_FINGERPRINT FAIL, ERROR CODE" + CHCNetSDK.NET_DVR_GetLastError().ToString(), "Error", MessageBoxButtons.OK);
                return;
            }


            Boolean Flag=true;
            int dwStatus = 0;
            CHCNetSDK.NET_DVR_FINGER_PRINT_INFO_STATUS_V50 struStatus=new CHCNetSDK.NET_DVR_FINGER_PRINT_INFO_STATUS_V50();
            struStatus.init();
            struStatus.dwSize=Marshal.SizeOf(struStatus);
            int struSize=struStatus.dwSize;
            while (Flag)
            {
                dwStatus = CHCNetSDK.NET_DVR_GetNextRemoteConfig(m_lDelFingerPrintCfHandle, ref struStatus,struSize);
                switch (dwStatus)
                {
                    case CHCNetSDK.NET_SDK_GET_NEXT_STATUS_SUCCESS://成功读取到数据，处理完本次数据后需调用next
                        ProcessDelDataRes(ref struStatus,ref Flag);
                        break;
                    case CHCNetSDK.NET_SDK_GET_NEXT_STATUS_NEED_WAIT:
                        break;
                    case CHCNetSDK.NET_SDK_GET_NEXT_STATUS_FAILED:
                        CHCNetSDK.NET_DVR_StopRemoteConfig(m_lDelFingerPrintCfHandle);
                        m_lDelFingerPrintCfHandle = -1;
                        MessageBox.Show("NET_SDK_GET_NEXT_STATUS_FAILED" + CHCNetSDK.NET_DVR_GetLastError().ToString(), "Error", MessageBoxButtons.OK);
                        Flag = false;
                        break;
                    case CHCNetSDK.NET_SDK_GET_NEXT_STATUS_FINISH:
                        CHCNetSDK.NET_DVR_StopRemoteConfig(m_lDelFingerPrintCfHandle);
                        m_lDelFingerPrintCfHandle = -1;
                        MessageBox.Show("NET_SDK_GET_NEXT_STATUS_FINISH");
                        Flag = false;
                        break;
                    default:
                        MessageBox.Show("NET_SDK_GET_NEXT_STATUS_UNKOWN" + CHCNetSDK.NET_DVR_GetLastError().ToString(), "Error", MessageBoxButtons.OK);
                        Flag = false;
                        break;
                }
            }

            Marshal.FreeHGlobal(ptrStruCardNo);
        }

        private void ProcessDelDataRes(ref CHCNetSDK.NET_DVR_FINGER_PRINT_INFO_STATUS_V50 struStatus,ref bool flag)
        {
            switch(struStatus.byStatus)
            {
                case 0:
                    MessageBox.Show("DelFp Invalid");
                    break;
                case 1:
                    MessageBox.Show("DelFp is Processing");
                    break;
                case 2:
                    MessageBox.Show("DelFp failed");
                    break;
                case 3:
                    MessageBox.Show("DelFp succeed");
                    break;
                default:
                    flag = false;
                    break;
            }
        }

        private void ByteCopy(ref byte[] source,ref byte[] Target)
        {
            for(int i=0;i<source.Length;++i)
            {
                if(i>Target.Length)
                {
                    break;
                }
                Target[i] = source[i];
            }
        }

        private void btnCap_Click(object sender, EventArgs e)
        {
            if(m_lCapFingerPrintCfHandle!=-1)
            {
                CHCNetSDK.NET_DVR_StopRemoteConfig(m_lCapFingerPrintCfHandle);
                m_lCapFingerPrintCfHandle = -1;
            }

            CHCNetSDK.NET_DVR_CAPTURE_FINGERPRINT_COND struCond = new CHCNetSDK.NET_DVR_CAPTURE_FINGERPRINT_COND();
            struCond.init();
            struCond.dwSize = Marshal.SizeOf(struCond);
            int dwInBufferSize=struCond.dwSize;
            struCond.byFingerPrintPicType = 1; //指纹图片类型是什么暂定1
            struCond.byFingerNo = 1;
            IntPtr ptrStruCond = Marshal.AllocHGlobal(struCond.dwSize);
            Marshal.StructureToPtr(struCond, ptrStruCond, false);

            m_lCapFingerPrintCfHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_UserID, CHCNetSDK.NET_DVR_CAPTURE_FINGERPRINT_INFO, ptrStruCond, dwInBufferSize,null,IntPtr.Zero);
            if(-1==m_lCapFingerPrintCfHandle)
            {
                Marshal.FreeHGlobal(ptrStruCond);
                MessageBox.Show("NET_DVR_CAP_FINGERPRINT FAIL, ERROR CODE" + CHCNetSDK.NET_DVR_GetLastError().ToString(), "Error", MessageBoxButtons.OK);
            }

            bool flag=true;
            int dwStatus=0;

            CHCNetSDK.NET_DVR_CAPTURE_FINGERPRINT_CFG struCFG=new CHCNetSDK.NET_DVR_CAPTURE_FINGERPRINT_CFG();
            struCFG.init();
            struCFG.dwSize=Marshal.SizeOf(struCFG);
            int dwOutBuffSize=struCFG.dwSize;
            while(flag)
            {
                dwStatus=CHCNetSDK.NET_DVR_GetNextRemoteConfig(m_lCapFingerPrintCfHandle,ref struCFG, dwOutBuffSize);
                switch(dwStatus)
                {
                    case CHCNetSDK.NET_SDK_GET_NEXT_STATUS_SUCCESS://成功读取到数据，处理完本次数据后需调用next
                        ProcessCapFingerData(ref struCFG, ref flag);
                        break;
                    case CHCNetSDK.NET_SDK_GET_NEXT_STATUS_NEED_WAIT:
                        break;
                    case CHCNetSDK.NET_SDK_GET_NEXT_STATUS_FAILED:
                        CHCNetSDK.NET_DVR_StopRemoteConfig(m_lCapFingerPrintCfHandle);
                        MessageBox.Show("NET_SDK_GET_NEXT_STATUS_FAILED" + CHCNetSDK.NET_DVR_GetLastError().ToString(), "Error", MessageBoxButtons.OK);
                        flag = false;
                        break;
                    case CHCNetSDK.NET_SDK_GET_NEXT_STATUS_FINISH:
                        CHCNetSDK.NET_DVR_StopRemoteConfig(m_lCapFingerPrintCfHandle);
                        flag = false;
                        break;
                    default:
                        MessageBox.Show("NET_SDK_GET_NEXT_STATUS_UNKOWN" + CHCNetSDK.NET_DVR_GetLastError().ToString(), "Error", MessageBoxButtons.OK);
                        flag = false;
                        CHCNetSDK.NET_DVR_StopRemoteConfig(m_lCapFingerPrintCfHandle);
                        break;
                }
            }
            Marshal.FreeHGlobal(ptrStruCond);
        }

        private void ProcessCapFingerData(ref CHCNetSDK.NET_DVR_CAPTURE_FINGERPRINT_CFG struCFG,ref bool flag)
        {
            string strpath = null;
            DateTime dt = DateTime.Now;
            strpath = string.Format("capFingerPrint.dat", Environment.CurrentDirectory);
            try
            {
                using (FileStream fs = new FileStream(strpath, FileMode.OpenOrCreate))
                {
                    fs.Write(struCFG.byFingerData, 0, struCFG.dwFingerPrintDataSize);
                    fs.Close();
                }
                textBoxFingerData.Text = strpath;
                MessageBox.Show("FingerPrint Cap SUCCEED", "SUCCEED", MessageBoxButtons.OK);
            }
            catch
            {
                MessageBox.Show("CapFingerprint process failed");
                flag = false;
            }
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


    }
}
