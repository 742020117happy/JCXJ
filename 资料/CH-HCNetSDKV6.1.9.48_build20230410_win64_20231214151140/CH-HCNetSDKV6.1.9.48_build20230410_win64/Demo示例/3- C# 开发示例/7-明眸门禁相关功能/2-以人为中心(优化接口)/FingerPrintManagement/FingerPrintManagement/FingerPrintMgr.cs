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
            string sURL = "POST /ISAPI/AccessControl/FingerPrint/SetUp?format=json";
            IntPtr ptrURL = Marshal.StringToHGlobalAnsi(sURL);
            m_lSetFingerPrintCfgHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_UserID, CHCNetSDK.NET_DVR_JSON_CONFIG, ptrURL, sURL.Length, null, IntPtr.Zero);
            if (m_lSetFingerPrintCfgHandle < 0)
            {
                MessageBox.Show("NET_DVR_StartRemoteConfig fail [url:POST /ISAPI/AccessControl/FingerPrint/SetUp?format=json] error:" + CHCNetSDK.NET_DVR_GetLastError());
                Marshal.FreeHGlobal(ptrURL);
                return;
            }
            else
            {
                CFingerPrintCfgCfg JsonFingerPrintCfg = new CFingerPrintCfgCfg();
                JsonFingerPrintCfg.FingerPrintCfg = new CFingerPrintCfg();
                JsonFingerPrintCfg.FingerPrintCfg.employeeNo = textBoxEmployeeNo.Text;
                JsonFingerPrintCfg.FingerPrintCfg.enableCardReader = new int[1];
                JsonFingerPrintCfg.FingerPrintCfg.enableCardReader[0] = 1;
                JsonFingerPrintCfg.FingerPrintCfg.fingerPrintID = 1;
                JsonFingerPrintCfg.FingerPrintCfg.fingerType = "normalFP";
                string strPath = textBoxFingerData.Text;
                if(!File.Exists(strPath))
                {
                    MessageBox.Show("not find FingerData");
                    Marshal.FreeHGlobal(ptrURL);
                    return;
                }
                FileStream fs = File.OpenRead(strPath); // OpenRead
                int filelen = 0;
                filelen = (int)fs.Length;
                byte[] byteFp = new byte[filelen];
                fs.Read(byteFp, 0, filelen);
                fs.Close();
                string sFpData = null;
                try
                {
                    sFpData = Convert.ToBase64String(byteFp);
                }
                catch
                {
                    sFpData = null;
                }
                JsonFingerPrintCfg.FingerPrintCfg.fingerData = sFpData;

                string strJsonFingerPrintCfg = JsonConvert.SerializeObject(JsonFingerPrintCfg, Formatting.Indented,
                                                            new JsonSerializerSettings { DefaultValueHandling = DefaultValueHandling.Ignore });
                IntPtr ptrJsonFingerPrintCfg = Marshal.StringToHGlobalAnsi(strJsonFingerPrintCfg);

                IntPtr ptrJsonData = Marshal.AllocHGlobal(1024);
                for (int i = 0; i < 1024; i++)
                {
                    Marshal.WriteByte(ptrJsonData, i, 0);
                }

                int dwState = 0;
                uint dwReturned = 0;
                while (true)
                {
                    dwState = CHCNetSDK.NET_DVR_SendWithRecvRemoteConfig(m_lSetFingerPrintCfgHandle, ptrJsonFingerPrintCfg, (uint)strJsonFingerPrintCfg.Length, ptrJsonData, 1024, ref dwReturned);
                    string strJsonData = Marshal.PtrToStringAnsi(ptrJsonData);
                    if (dwState == (int)CHCNetSDK.NET_SDK_SENDWITHRECV_STATUS.NET_SDK_CONFIG_STATUS_NEEDWAIT)
                    {
                        Thread.Sleep(10);
                        continue;
                    }
                    else if (dwState == (int)CHCNetSDK.NET_SDK_SENDWITHRECV_STATUS.NET_SDK_CONFIG_STATUS_FAILED)
                    {
                        MessageBox.Show("Set Card Fail error:" + CHCNetSDK.NET_DVR_GetLastError());
                        break;
                    }
                    else if (dwState == (int)CHCNetSDK.NET_SDK_SENDWITHRECV_STATUS.NET_SDK_CONFIG_STATUS_SUCCESS)
                    {
                        CFingerPrintStatusCfg JsonFingerPrintStatusCfg = new CFingerPrintStatusCfg();
                        JsonFingerPrintStatusCfg = JsonConvert.DeserializeObject<CFingerPrintStatusCfg>(strJsonData); ;
                        if (JsonFingerPrintStatusCfg.FingerPrintStatus == null)
                        {
                            CResponseStatus JsonResponseStatus = new CResponseStatus();
                            JsonResponseStatus = JsonConvert.DeserializeObject<CResponseStatus>(strJsonData);
                            if (JsonResponseStatus.statusCode == 1)
                            {
                                MessageBox.Show("Set FingerPrint Success");
                            }
                            else
                            {
                                MessageBox.Show("Set FingerPrint Fail, ResponseStatus.statusCode:" + JsonResponseStatus.statusCode);
                            }
                        }
                        else
                        {
                            for (int i = 0; i < JsonFingerPrintStatusCfg.FingerPrintStatus.StatusList.Count;  )
                            {
                                if (JsonFingerPrintStatusCfg.FingerPrintStatus.StatusList[i].cardReaderRecvStatus == 1)
                                {
                                    MessageBox.Show("Set FingerPrint Success");
                                }
                                else
                                {
                                    MessageBox.Show("Set FingerPrint Fail, cardReaderRecvStatus:" + JsonFingerPrintStatusCfg.FingerPrintStatus.StatusList[i].cardReaderRecvStatus);
                                }
                                break; //界面上只能显示一个指纹，直接break
                            }
                        }
                        break;
                    }
                    else if (dwState == (int)CHCNetSDK.NET_SDK_SENDWITHRECV_STATUS.NET_SDK_CONFIG_STATUS_EXCEPTION)
                    {
                        MessageBox.Show("Set FingerPrint Exception error:" + CHCNetSDK.NET_DVR_GetLastError());
                        break;
                    }
                    else
                    {
                        MessageBox.Show("unknown Status error:" + CHCNetSDK.NET_DVR_GetLastError());
                        break;
                    }
                }
                if (m_lSetFingerPrintCfgHandle != -1)
                {
                    CHCNetSDK.NET_DVR_StopRemoteConfig((int)m_lSetFingerPrintCfgHandle);
                    m_lSetFingerPrintCfgHandle = -1;
                }
                Marshal.FreeHGlobal(ptrJsonFingerPrintCfg);
                Marshal.FreeHGlobal(ptrJsonData);
                Marshal.FreeHGlobal(ptrURL);
            }
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
            string sURL = "POST /ISAPI/AccessControl/FingerPrintUpload?format=json";
            IntPtr ptrURL = Marshal.StringToHGlobalAnsi(sURL);
            m_lGetFingerPrintCfgHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_UserID, CHCNetSDK.NET_DVR_JSON_CONFIG, ptrURL, sURL.Length, null, IntPtr.Zero);
            if (m_lGetFingerPrintCfgHandle < 0)
            {
                MessageBox.Show("NET_DVR_StartRemoteConfig fail [url:POST /ISAPI/AccessControl/FingerPrintUpload?format=json] error:" + CHCNetSDK.NET_DVR_GetLastError());
                Marshal.FreeHGlobal(ptrURL);
                return;
            }
            else
            {
                CFingerPrintCondCfg JsonFingerPrintCondCfg = new CFingerPrintCondCfg();
                JsonFingerPrintCondCfg.FingerPrintCond = new CFingerPrintCond();
                JsonFingerPrintCondCfg.FingerPrintCond.searchID = "1";
                JsonFingerPrintCondCfg.FingerPrintCond.employeeNo = textBoxEmployeeNo.Text;
                int cardReaderNo = 0;
                int.TryParse(textBoxFingerID.Text, out cardReaderNo);
                JsonFingerPrintCondCfg.FingerPrintCond.cardReaderNo = cardReaderNo;
                int figerPrintID = 0;
                int.TryParse(textBoxFingerID.Text, out figerPrintID);
                JsonFingerPrintCondCfg.FingerPrintCond.fingerPrintID = figerPrintID;

                string strFingerPrintCondCfg = JsonConvert.SerializeObject(JsonFingerPrintCondCfg, Formatting.Indented,
                                                        new JsonSerializerSettings { DefaultValueHandling = DefaultValueHandling.Ignore });
                IntPtr ptrFingerPrintCondCfg = Marshal.StringToHGlobalAnsi(strFingerPrintCondCfg);

                IntPtr ptrJsonData = Marshal.AllocHGlobal(1024);
                for (int i = 0; i < 1024; i++)
                {
                    Marshal.WriteByte(ptrJsonData, i, 0);
                }

                int dwState = 0;
                uint dwReturned = 0;
                while (true)
                {
                    dwState = CHCNetSDK.NET_DVR_SendWithRecvRemoteConfig(m_lGetFingerPrintCfgHandle, ptrFingerPrintCondCfg, (uint)strFingerPrintCondCfg.Length, ptrJsonData, 1024, ref dwReturned);
                    string strJsonData = Marshal.PtrToStringAnsi(ptrJsonData);
                    if (dwState == (int)CHCNetSDK.NET_SDK_SENDWITHRECV_STATUS.NET_SDK_CONFIG_STATUS_NEEDWAIT)
                    {
                        Thread.Sleep(10);
                        continue;
                    }
                    else if (dwState == (int)CHCNetSDK.NET_SDK_SENDWITHRECV_STATUS.NET_SDK_CONFIG_STATUS_FAILED)
                    {
                        MessageBox.Show("Get Card Fail error:" + CHCNetSDK.NET_DVR_GetLastError());
                    }
                    else if (dwState == (int)CHCNetSDK.NET_SDK_SENDWITHRECV_STATUS.NET_SDK_CONFIG_STATUS_SUCCESS)
                    {
                        CFingerPrintInfoCfg JsonFingerPrintInfoCfg = new CFingerPrintInfoCfg();
                        JsonFingerPrintInfoCfg = JsonConvert.DeserializeObject<CFingerPrintInfoCfg>(strJsonData);
                        if (JsonFingerPrintInfoCfg.FingerPrintInfo == null)
                        {
                            //null说明返回的Json报文不是FingerPrintInfo，而是ResponseStatus
                            CResponseStatus JsonResponseStatus = new CResponseStatus();
                            JsonResponseStatus = JsonConvert.DeserializeObject<CResponseStatus>(strJsonData);
                            if (JsonResponseStatus.statusCode == 1)
                            {
                                MessageBox.Show("Get Finger Success");
                            }
                            else
                            {
                                MessageBox.Show("Get FingerPrint Fail, ResponseStatus.statusCode" + JsonResponseStatus.statusCode);
                            }
                        }
                        else
                        {
                            if (JsonFingerPrintInfoCfg.FingerPrintInfo.status == "NoFP")
                            {
                                MessageBox.Show("not exist fingerPrint");
                                break;
                            }
                            for (int i = 0; i < JsonFingerPrintInfoCfg.FingerPrintInfo.FingerPrintList.Count; )
                            {
                                byte[] bytes = Convert.FromBase64String(JsonFingerPrintInfoCfg.FingerPrintInfo.FingerPrintList[i].fingerData);
                                string strPath = string.Format("{0}\\{1}_{2}_{3} fingerprint.dat", Environment.CurrentDirectory, textBoxEmployeeNo.Text,
                                                            textBoxCardReaderNo.Text, textBoxFingerID.Text);
                                textBoxFingerData.Text = strPath;
                                try
                                {
                                    using (FileStream fs = new FileStream(strPath, FileMode.OpenOrCreate))
                                    {
                                        if (!File.Exists(strPath))
                                        {
                                            MessageBox.Show("FingerPrint storage file creat failed！");
                                        }
                                        BinaryWriter objBinaryWrite = new BinaryWriter(fs);
                                        fs.Write(bytes, 0, bytes.Length);
                                        fs.Close();
                                    }
                                }
                                catch
                                {

                                }
                                break;//循环一次就break吧，界面比较简单不能都显示出来
                            }
                            MessageBox.Show("Get FingerPrint Success");
                        }
                        break;
                    }
                    else if (dwState == (int)CHCNetSDK.NET_SDK_SENDWITHRECV_STATUS.NET_SDK_CONFIG_STATUS_EXCEPTION)
                    {
                        MessageBox.Show("Get FingerPrint Exception error:" + CHCNetSDK.NET_DVR_GetLastError());
                        break;
                    }
                    else
                    {
                        MessageBox.Show("unknown Status Error:" + CHCNetSDK.NET_DVR_GetLastError());
                        break;
                    }
                }
                if (m_lGetFingerPrintCfgHandle != -1)
                {
                    CHCNetSDK.NET_DVR_StopRemoteConfig((int)m_lGetFingerPrintCfgHandle);
                    m_lGetFingerPrintCfgHandle = -1;
                }
                Marshal.FreeHGlobal(ptrFingerPrintCondCfg);
                Marshal.FreeHGlobal(ptrJsonData);
                Marshal.FreeHGlobal(ptrURL);
            }
        }

        private void btnDel_Click(object sender, EventArgs e)
        {
            if(-1!=m_lDelFingerPrintCfHandle)
            {
                CHCNetSDK.NET_DVR_StopRemoteConfig(m_lDelFingerPrintCfHandle);
                m_lDelFingerPrintCfHandle = -1;
            }

            //这边是联合体，默认卡号人员ID方式删除
            CHCNetSDK.NET_DVR_FINGER_PRINT_INFO_CTRL_V50_ByCardNo struCardNo = new CHCNetSDK.NET_DVR_FINGER_PRINT_INFO_CTRL_V50_ByCardNo();
            struCardNo.init();
            struCardNo.byMode = 0;

            byte[] byTempCardNo = System.Text.Encoding.UTF8.GetBytes(textBoxEmployeeNo.Text);
            ByteCopy(ref byTempCardNo, ref struCardNo.struProcessMode.byEmployeeNo);
            int dwFingerID = 0;
            int.TryParse(textBoxFingerID.Text, out dwFingerID);
            if (dwFingerID > 0 && dwFingerID <= 10)
            {
                struCardNo.struProcessMode.byFingerPrintID[dwFingerID - 1] = 1;
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
            struCond.byFingerPrintPicType = 0;
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
