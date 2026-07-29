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
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

using CardManagement.Language;


namespace CardManagement
{
    public partial class CardManagement : Form
    {
        public static int m_UserID = -1;
        public Int32 m_lGetCardCfgHandle = -1;
        public Int32 m_lSetCardCfgHandle = -1;
        public Int32 m_lDelCardCfgHandle = -1;

        public CardManagement()
        {
            InitializeComponent();
            if (CHCNetSDK.NET_DVR_Init() == false)
            {
                MessageBox.Show("NET_DVR_Init error!");
                return;
            }
            comboBoxLanguage.SelectedIndex = 0;
            comboBoxCardType.SelectedIndex = 0;
            CHCNetSDK.NET_DVR_SetLogToFile(3, "./", false);
        }

        private void btnLogin_Click(object sender, EventArgs e)
        {
            AddDevice dlg = new AddDevice();
            dlg.ShowDialog();
            dlg.Dispose();
        }

        private void CardManagement_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (m_UserID >= 0)
            {
                CHCNetSDK.NET_DVR_Logout_V30(m_UserID);
                m_UserID = -1;
            }
            CHCNetSDK.NET_DVR_Cleanup();
        }

        private void btnSet_Click(object sender, EventArgs e)
        {
            if(m_lSetCardCfgHandle != -1)
            {
                if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lSetCardCfgHandle))
                {
                    m_lSetCardCfgHandle = -1;
                }
            }

            string sURL = "PUT /ISAPI/AccessControl/CardInfo/SetUp?format=json";
            IntPtr ptrURL = Marshal.StringToHGlobalAnsi(sURL);
            m_lSetCardCfgHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_UserID, CHCNetSDK.NET_DVR_JSON_CONFIG, ptrURL, sURL.Length, null, IntPtr.Zero);
            if (m_lSetCardCfgHandle < 0)
            {
                MessageBox.Show("NET_DVR_StartRemoteConfig fail [url:PUT /ISAPI/AccessControl/CardInfo/SetUp?format=json] error:" + CHCNetSDK.NET_DVR_GetLastError());
                Marshal.FreeHGlobal(ptrURL);
                return;
            }
            else
            {
                SendCardData();
                Marshal.FreeHGlobal(ptrURL);
            }
        }

        private void SendCardData()
        {
            CCardInfoCfg JsonCardInfo = new CCardInfoCfg();
            JsonCardInfo.CardInfo = new CCardInfo();
            JsonCardInfo.CardInfo.employeeNo = textBoxEmployeeNo.Text;
            JsonCardInfo.CardInfo.cardNo = textBoxCardNo.Text;
            JsonCardInfo.CardInfo.cardType = comboBoxCardType.Text;
            string strJsonCardInfo = JsonConvert.SerializeObject(JsonCardInfo, Formatting.Indented,
                                                        new JsonSerializerSettings { DefaultValueHandling = DefaultValueHandling.Ignore });
            IntPtr ptrJsonCardInfo = Marshal.StringToHGlobalAnsi(strJsonCardInfo);

            IntPtr ptrJsonData = Marshal.AllocHGlobal(1024);
            for (int i = 0; i < 1024; i++)
            {
                Marshal.WriteByte(ptrJsonData, i, 0);
            }

            int dwState = 0;
            uint dwReturned = 0;
            while (true)
            {
                dwState = CHCNetSDK.NET_DVR_SendWithRecvRemoteConfig(m_lSetCardCfgHandle, ptrJsonCardInfo, (uint)strJsonCardInfo.Length, ptrJsonData, 1024, ref dwReturned);
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
                    CResponseStatus JsonResponseStatus = new CResponseStatus();
                    JsonResponseStatus = JsonConvert.DeserializeObject<CResponseStatus>(strJsonData);
                    if (JsonResponseStatus.statusCode == 1)
                    {
                        MessageBox.Show("Set Card Success");
                    }
                    else
                    {
                        MessageBox.Show("Set Card Fail, ResponseStatus.statusCode:" + JsonResponseStatus.statusCode);
                    }
                    break;
                }
                else if (dwState == (int)CHCNetSDK.NET_SDK_SENDWITHRECV_STATUS.NET_SDK_CONFIG_STATUS_EXCEPTION)
                {
                    MessageBox.Show("Set Card Exception error:" + CHCNetSDK.NET_DVR_GetLastError());
                    break;
                }
                else
                {
                    MessageBox.Show("unknown Status error:" + CHCNetSDK.NET_DVR_GetLastError());
                    break;
                }
            }
            if (m_lSetCardCfgHandle != -1)
            {
                if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lSetCardCfgHandle))
                {
                    m_lSetCardCfgHandle = -1;
                }
            }
            Marshal.FreeHGlobal(ptrJsonCardInfo);
            Marshal.FreeHGlobal(ptrJsonData);
        }

        private void btnGet_Click(object sender, EventArgs e)
        {
            if (m_lGetCardCfgHandle != -1)
            {
                if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lGetCardCfgHandle))
                {
                    m_lGetCardCfgHandle = -1;
                }
            }
            string sURL = "POST /ISAPI/AccessControl/CardInfo/Search?format=json";
            IntPtr ptrURL = Marshal.StringToHGlobalAnsi(sURL);
            m_lGetCardCfgHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_UserID, CHCNetSDK.NET_DVR_JSON_CONFIG, ptrURL, sURL.Length, null, IntPtr.Zero);
            if (m_lGetCardCfgHandle < 0)
            {
                MessageBox.Show("NET_DVR_StartRemoteConfig fail [url:POST /ISAPI/AccessControl/CardInfo/Search?format=json] error:" + CHCNetSDK.NET_DVR_GetLastError());
                Marshal.FreeHGlobal(ptrURL);
                return;
            }
            else
            {
                CCardInfoSearchCondCfg JsonCardInfoSearchCondCfg = new CCardInfoSearchCondCfg();
                JsonCardInfoSearchCondCfg.CardInfoSearchCond = new CCardInfoSearchCond();
                JsonCardInfoSearchCondCfg.CardInfoSearchCond.searchID = "1";
                JsonCardInfoSearchCondCfg.CardInfoSearchCond.searchResultPosition = 0;
                JsonCardInfoSearchCondCfg.CardInfoSearchCond.maxResults = 10;
                JsonCardInfoSearchCondCfg.CardInfoSearchCond.EmployeeNoList = new List<CEmployeeNoList>();
                CEmployeeNoList singleEmployeeNoList = new CEmployeeNoList();
                singleEmployeeNoList.employeeNo = textBoxEmployeeNo.Text;
                JsonCardInfoSearchCondCfg.CardInfoSearchCond.EmployeeNoList.Add(singleEmployeeNoList);
                //JsonCardInfoSearchCondCfg.CardInfoSearchCond.CardNoList = new List<CCardNoList>();
                //CCardNoList singleCardNoList = new CCardNoList();
                //singleCardNoList.cardNo = textBoxCardNo.Text;
                //JsonCardInfoSearchCondCfg.CardInfoSearchCond.CardNoList.Add(singleCardNoList);

                string strCardInfoSearchCondCfg = JsonConvert.SerializeObject(JsonCardInfoSearchCondCfg, Formatting.Indented,
                                                        new JsonSerializerSettings { DefaultValueHandling = DefaultValueHandling.Ignore });
                IntPtr ptrCardInfoSearchCondCfg = Marshal.StringToHGlobalAnsi(strCardInfoSearchCondCfg);

                IntPtr ptrJsonData = Marshal.AllocHGlobal(1024);
                for (int i = 0; i < 1024; i++)
                {
                    Marshal.WriteByte(ptrJsonData, i, 0);
                }

                int dwState = 0;
                uint dwReturned = 0;
                while (true)
                {
                    dwState = CHCNetSDK.NET_DVR_SendWithRecvRemoteConfig(m_lGetCardCfgHandle, ptrCardInfoSearchCondCfg, (uint)strCardInfoSearchCondCfg.Length, ptrJsonData, 1024, ref dwReturned);
                    string strJsonData = Marshal.PtrToStringAnsi(ptrJsonData);
                    if (dwState == (int)CHCNetSDK.NET_SDK_SENDWITHRECV_STATUS.NET_SDK_CONFIG_STATUS_NEEDWAIT)
                    {
                        Thread.Sleep(10);
                        continue;
                    }
                    else if (dwState == (int)CHCNetSDK.NET_SDK_SENDWITHRECV_STATUS.NET_SDK_CONFIG_STATUS_FAILED)
                    {
                        MessageBox.Show("Get Card Fail error:" + CHCNetSDK.NET_DVR_GetLastError());
                        break;
                    }
                    else if (dwState == (int)CHCNetSDK.NET_SDK_SENDWITHRECV_STATUS.NET_SDK_CONFIG_STATUS_SUCCESS)
                    {
                        CCardInfoSearchCfg JsonCardInfoSearchCfg = new CCardInfoSearchCfg();
                        JsonCardInfoSearchCfg = JsonConvert.DeserializeObject<CCardInfoSearchCfg>(strJsonData);
                        if (JsonCardInfoSearchCfg.CardInfoSearch == null)
                        {
                            //null说明返回的Json报文不是UserInfoSearch，而是ResponseStatus
                            CResponseStatus JsonResponseStatus = new CResponseStatus();
                            JsonResponseStatus = JsonConvert.DeserializeObject<CResponseStatus>(strJsonData);
                            if (JsonResponseStatus.statusCode == 1)
                            {
                                MessageBox.Show("Get Card Success");
                            }
                            else
                            {
                                MessageBox.Show("Get Card Fail, ResponseStatus.statusCode" + JsonResponseStatus.statusCode);
                            }
                        }
                        else
                        {
                            //解析CardInfoSearch报文
                            if (JsonCardInfoSearchCfg.CardInfoSearch.totalMatches == 0)
                            {
                                MessageBox.Show("There is no this card");
                                break;
                            }

                            for (int i = 0; i < JsonCardInfoSearchCfg.CardInfoSearch.numOfMatches; )
                            {
                                textBoxEmployeeNo.Text = JsonCardInfoSearchCfg.CardInfoSearch.CardInfo[i].employeeNo;
                                textBoxCardNo.Text = JsonCardInfoSearchCfg.CardInfoSearch.CardInfo[i].cardNo;
                                if (JsonCardInfoSearchCfg.CardInfoSearch.CardInfo[i].cardType == "normalCard")
                                {
                                    comboBoxCardType.SelectedIndex = 0;
                                }
                                else if (JsonCardInfoSearchCfg.CardInfoSearch.CardInfo[i].cardType == "patrolCard")
                                {
                                    comboBoxCardType.SelectedIndex = 1;
                                }
                                else if (JsonCardInfoSearchCfg.CardInfoSearch.CardInfo[i].cardType == "hijackCard")
                                {
                                    comboBoxCardType.SelectedIndex = 2;
                                }
                                else if (JsonCardInfoSearchCfg.CardInfoSearch.CardInfo[i].cardType == "superCard")
                                {
                                    comboBoxCardType.SelectedIndex = 3;
                                }
                                else if (JsonCardInfoSearchCfg.CardInfoSearch.CardInfo[i].cardType == "dismissingCard")
                                {
                                    comboBoxCardType.SelectedIndex = 4;
                                }
                                break;//循环一次就break吧，界面比较简单不能都显示出来
                            }
                            MessageBox.Show("Get Card Success");
                        }
                        break;
                    }
                    else if (dwState == (int)CHCNetSDK.NET_SDK_SENDWITHRECV_STATUS.NET_SDK_CONFIG_STATUS_EXCEPTION)
                    {
                        MessageBox.Show("Get Card Exception error:" + CHCNetSDK.NET_DVR_GetLastError());
                        break;
                    }
                    else
                    {
                        MessageBox.Show("unknown Status Error:" + CHCNetSDK.NET_DVR_GetLastError());
                        break;
                    }
                }
                if (m_lGetCardCfgHandle != -1)
                {
                    CHCNetSDK.NET_DVR_StopRemoteConfig(m_lGetCardCfgHandle);
                    m_lGetCardCfgHandle = -1;
                }
                Marshal.FreeHGlobal(ptrCardInfoSearchCondCfg);
                Marshal.FreeHGlobal(ptrJsonData);
                Marshal.FreeHGlobal(ptrURL);
            }
        }

        private void btnDelete_Click(object sender, EventArgs e)
        {
            if (m_lDelCardCfgHandle != -1)
            {
                if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lDelCardCfgHandle))
                {
                    m_lDelCardCfgHandle = -1;
                }
            }
            IntPtr ptrOutBuf = Marshal.AllocHGlobal(1024);
            IntPtr ptrStatusBuffer = Marshal.AllocHGlobal(1024);
            for (int i = 0; i < 1024; i++)
            {
                Marshal.WriteByte(ptrOutBuf, i, 0);
                Marshal.WriteByte(ptrStatusBuffer, i, 0);
            }

            CHCNetSDK.NET_DVR_XML_CONFIG_INPUT struInput = new CHCNetSDK.NET_DVR_XML_CONFIG_INPUT();
            CHCNetSDK.NET_DVR_XML_CONFIG_OUTPUT struOuput = new CHCNetSDK.NET_DVR_XML_CONFIG_OUTPUT();

            string sUrl = "PUT /ISAPI/AccessControl/CardInfo/Delete?format=json";
            IntPtr ptrURL = Marshal.StringToHGlobalAnsi(sUrl);
            struInput.dwSize = (uint)Marshal.SizeOf(struInput);
            struInput.lpRequestUrl = ptrURL;
            struInput.dwRequestUrlLen = (uint)sUrl.Length;

            CCardInfoDelCondCfg JsonCardInfoDelCondCfg = new CCardInfoDelCondCfg();
            JsonCardInfoDelCondCfg.CardInfoDelCond = new CCardInfoDelCond();
            JsonCardInfoDelCondCfg.CardInfoDelCond.EmployeeNoList = new List<CEmployeeNoList>();
            CEmployeeNoList singleEmployeeNoList = new CEmployeeNoList();
            singleEmployeeNoList.employeeNo = textBoxEmployeeNo.Text;
            JsonCardInfoDelCondCfg.CardInfoDelCond.EmployeeNoList.Add(singleEmployeeNoList);
            //JsonCardInfoDelCondCfg.CardInfoDelCond.CardNoList = new List<CCardNoList>();
            //CCardNoList singleCardNoList = new CCardNoList();
            //singleCardNoList.cardNo = textBoxCardNo.Text;
            //JsonCardInfoDelCondCfg.CardInfoDelCond.CardNoList.Add(singleCardNoList);
            string strCardInfoDelCondCfg = JsonConvert.SerializeObject(JsonCardInfoDelCondCfg, Formatting.Indented,
                                                        new JsonSerializerSettings { DefaultValueHandling = DefaultValueHandling.Ignore });
            IntPtr ptrCardInfoDelCondCfg = Marshal.StringToHGlobalAnsi(strCardInfoDelCondCfg);

            struInput.lpInBuffer = ptrCardInfoDelCondCfg;
            struInput.dwInBufferSize = (uint)strCardInfoDelCondCfg.Length;

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
                MessageBox.Show("NET_DVR_STDXMLConfig fail [url:PUT /ISAPI/AccessControl/UserInfoDetail/Delete?format=json] error:" + CHCNetSDK.NET_DVR_GetLastError());
                Marshal.FreeHGlobal(ptrOutBuf);
                Marshal.FreeHGlobal(ptrStatusBuffer);
                Marshal.FreeHGlobal(ptrCardInfoDelCondCfg);
                Marshal.FreeHGlobal(ptrInput);
                Marshal.FreeHGlobal(ptrOuput);
                Marshal.FreeHGlobal(ptrURL);
                return;
            }
            else
            {
                string strResponseStatus = Marshal.PtrToStringAnsi(struOuput.lpOutBuffer);
                CResponseStatus JsonResponseStatus = new CResponseStatus();
                JsonResponseStatus = JsonConvert.DeserializeObject<CResponseStatus>(strResponseStatus);
                if (JsonResponseStatus.statusCode != 1)
                {
                    MessageBox.Show("NET_DVR_STDXMLConfig Return ResponseStatus.statusCode:" + JsonResponseStatus.statusCode);
                }
                else
                {
                    MessageBox.Show("Del Card Success");
                }

                Marshal.FreeHGlobal(ptrOutBuf);
                Marshal.FreeHGlobal(ptrStatusBuffer);
                Marshal.FreeHGlobal(ptrCardInfoDelCondCfg);
                Marshal.FreeHGlobal(ptrInput);
                Marshal.FreeHGlobal(ptrOuput);
                Marshal.FreeHGlobal(ptrURL);
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
