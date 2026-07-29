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
using System.Threading.Tasks;
using System.Diagnostics;
using System.IO;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System.Text.RegularExpressions;
using UserManagement.Language;

namespace UserManagement
{
    public partial class UserManagement : Form
    {
        public static int m_UserID = -1;
        public Int32 m_lGetUserCfgHandle = -1;
        public Int32 m_lSetUserCfgHandle = -1;
        public Int32 m_lDelUserCfgHandle = -1;

        public UserManagement()
        {
            InitializeComponent();
            if (CHCNetSDK.NET_DVR_Init() == false)
            {
                MessageBox.Show("NET_DVR_Init error!");
                return;
            }
            comboBoxLanguage.SelectedIndex = 0;
            CHCNetSDK.NET_DVR_SetLogToFile(3, "./", false);
        }

        private void btnLogin_Click(object sender, EventArgs e)
        {
            AddDevice dlg = new AddDevice();
            dlg.ShowDialog();
            dlg.Dispose();
        }

        private void UserManagement_FormClosing(object sender, FormClosingEventArgs e)
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
            if(m_lSetUserCfgHandle != -1)
            {
                if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lSetUserCfgHandle))
                {
                    m_lSetUserCfgHandle = -1;
                }
            }

            string sURL = "PUT /ISAPI/AccessControl/UserInfo/SetUp?format=json";
            IntPtr ptrURL = Marshal.StringToHGlobalAnsi(sURL);
            m_lSetUserCfgHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_UserID, CHCNetSDK.NET_DVR_JSON_CONFIG, ptrURL, sURL.Length, null, IntPtr.Zero);
            if (m_lSetUserCfgHandle < 0)
            {
                MessageBox.Show("NET_DVR_StartRemoteConfig fail [url:PUT /ISAPI/AccessControl/UserInfo/SetUp?format=json] error:" + CHCNetSDK.NET_DVR_GetLastError());
                Marshal.FreeHGlobal(ptrURL);
                return;
            }
            else 
            {
                SendUserInfo();
                Marshal.FreeHGlobal(ptrURL);
            }
        }

        private void SendUserInfo()
        {
            CUserInfoCfg JsonUserInfo = new CUserInfoCfg();
            JsonUserInfo.UserInfo = new CUserInfo();
            JsonUserInfo.UserInfo.employeeNo = textBoxEmployeeNo.Text;
            JsonUserInfo.UserInfo.name = textBoxName.Text;
            JsonUserInfo.UserInfo.userType = "normal";
            JsonUserInfo.UserInfo.Valid = new CValid();
            JsonUserInfo.UserInfo.Valid.enable = true;
            JsonUserInfo.UserInfo.Valid.beginTime = "2017-08-01T17:30:08";
            JsonUserInfo.UserInfo.Valid.endTime = "2037-12-12T23:59:59";
            JsonUserInfo.UserInfo.Valid.timeType = "local";

            JsonUserInfo.UserInfo.doorRight = "1";
            JsonUserInfo.UserInfo.RightPlan = new List<CRightPlan>();
            CRightPlan JsonRightPlan = new CRightPlan();
            JsonRightPlan.doorNo = 1;
            JsonRightPlan.planTemplateNo = textBoxRightPlan.Text;
            JsonUserInfo.UserInfo.RightPlan.Add(JsonRightPlan);
       //     JsonUserInfo.UserInfo.userVerifyMode = "face";
            string strJsonUserInfo = JsonConvert.SerializeObject(JsonUserInfo, Formatting.Indented,
                                                        new JsonSerializerSettings { DefaultValueHandling = DefaultValueHandling.Ignore });

            byte[] byJsonUserInfo = System.Text.Encoding.UTF8.GetBytes(strJsonUserInfo);
            IntPtr ptrJsonUserInfo = Marshal.AllocHGlobal(byJsonUserInfo.Length);
            Marshal.Copy(byJsonUserInfo, 0, ptrJsonUserInfo, byJsonUserInfo.Length);

            IntPtr ptrJsonData = Marshal.AllocHGlobal(1024);
            for (int i = 0; i < 1024; i++)
            {
                Marshal.WriteByte(ptrJsonData, i, 0);
            }

            int dwState = (int)CHCNetSDK.NET_SDK_SENDWITHRECV_STATUS.NET_SDK_CONFIG_STATUS_SUCCESS;
            uint dwReturned = 0;
            while (true)
            {
                dwState = CHCNetSDK.NET_DVR_SendWithRecvRemoteConfig(m_lSetUserCfgHandle, ptrJsonUserInfo, (uint)byJsonUserInfo.Length, ptrJsonData, 1024, ref dwReturned);
                string strJsonData = Marshal.PtrToStringAnsi(ptrJsonData);
                if (dwState == (int)CHCNetSDK.NET_SDK_SENDWITHRECV_STATUS.NET_SDK_CONFIG_STATUS_NEEDWAIT)
                {
                    Thread.Sleep(10);
                    continue;
                }
                else if (dwState == (int)CHCNetSDK.NET_SDK_SENDWITHRECV_STATUS.NET_SDK_CONFIG_STATUS_FAILED)
                {
                    MessageBox.Show("Set User Fail error:" + CHCNetSDK.NET_DVR_GetLastError());
                    break;
                }
                else if (dwState == (int)CHCNetSDK.NET_SDK_SENDWITHRECV_STATUS.NET_SDK_CONFIG_STATUS_SUCCESS)
                {
                    //返回NET_SDK_CONFIG_STATUS_SUCCESS代表流程走通了，但并不代表下发成功，比如有些设备可能因为人员已存在等原因下发失败，所以需要解析Json报文
                    CResponseStatus JsonResponseStatus = new CResponseStatus();
                    JsonResponseStatus = JsonConvert.DeserializeObject<CResponseStatus>(strJsonData);

                    if (JsonResponseStatus.statusCode == 1)
                    {
                        MessageBox.Show("Set User Success");
                    }
                    else
                    {
                        MessageBox.Show("Set User Fail, ResponseStatus.statusCode" + JsonResponseStatus.statusCode);
                    }
                    break;
                }
                else if (dwState == (int)CHCNetSDK.NET_SDK_SENDWITHRECV_STATUS.NET_SDK_CONFIG_STATUS_FINISH)
                {
                    //下发人员时：dwState其实不会走到这里，因为设备不知道我们会下发多少个人，所以长连接需要我们主动关闭
                    MessageBox.Show("Set User Finish");
                    break;
                }
                else if (dwState == (int)CHCNetSDK.NET_SDK_SENDWITHRECV_STATUS.NET_SDK_CONFIG_STATUS_EXCEPTION)
                {
                    MessageBox.Show("Set User Exception error:" + CHCNetSDK.NET_DVR_GetLastError());
                    break;
                }
                else
                {
                    MessageBox.Show("unknown Status error:" + CHCNetSDK.NET_DVR_GetLastError());
                    break;
                }
            }
            if (m_lSetUserCfgHandle != -1)
            {
                if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lSetUserCfgHandle))
                {
                    m_lSetUserCfgHandle = -1;
                }
            }
            Marshal.FreeHGlobal(ptrJsonUserInfo);
            Marshal.FreeHGlobal(ptrJsonData);
        }

        private void btnGet_Click(object sender, EventArgs e)
        {
            if (m_lGetUserCfgHandle != -1)
            {
                if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lGetUserCfgHandle))
                {
                    m_lGetUserCfgHandle = -1;
                }
            }

            string sURL = "POST /ISAPI/AccessControl/UserInfo/Search?format=json";
            IntPtr ptrURL = Marshal.StringToHGlobalAnsi(sURL);
            m_lSetUserCfgHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_UserID, CHCNetSDK.NET_DVR_JSON_CONFIG, ptrURL, sURL.Length, null, IntPtr.Zero);
            if (m_lSetUserCfgHandle < 0)
            {
                MessageBox.Show("NET_DVR_StartRemoteConfig fail [url:POST /ISAPI/AccessControl/UserInfo/Search?format=json] error:" + CHCNetSDK.NET_DVR_GetLastError());
                Marshal.FreeHGlobal(ptrURL);
                return;
            }
            else
            {
                CUserInfoSearchCondCfg JsonUserInfoSearchCondCfg = new CUserInfoSearchCondCfg();
                JsonUserInfoSearchCondCfg.UserInfoSearchCond = new CUserInfoSearchCond();
                JsonUserInfoSearchCondCfg.UserInfoSearchCond.searchID = "1";
                JsonUserInfoSearchCondCfg.UserInfoSearchCond.searchResultPosition = 0;
                JsonUserInfoSearchCondCfg.UserInfoSearchCond.maxResults = 10;
                JsonUserInfoSearchCondCfg.UserInfoSearchCond.EmployeeNoList = new List<CEmployeeNoList>();
                CEmployeeNoList singleEmployeeNoList = new CEmployeeNoList();
                singleEmployeeNoList.employeeNo = textBoxEmployeeNo.Text;
                JsonUserInfoSearchCondCfg.UserInfoSearchCond.EmployeeNoList.Add(singleEmployeeNoList);
                string strUserInfoSearchCondCfg = JsonConvert.SerializeObject(JsonUserInfoSearchCondCfg);
                IntPtr ptrUserInfoSearchCondCfg = Marshal.StringToHGlobalAnsi(strUserInfoSearchCondCfg);

                IntPtr ptrJsonData = Marshal.AllocHGlobal(1024);
                for (int i = 0; i < 1024; i++)
                {
                    Marshal.WriteByte(ptrJsonData, i, 0);
                }

                int dwState = (int)CHCNetSDK.NET_SDK_SENDWITHRECV_STATUS.NET_SDK_CONFIG_STATUS_SUCCESS;
                uint dwReturned = 0;
                while (true)
                {
                    dwState = CHCNetSDK.NET_DVR_SendWithRecvRemoteConfig(m_lSetUserCfgHandle, ptrUserInfoSearchCondCfg, (uint)strUserInfoSearchCondCfg.Length, ptrJsonData, 1024, ref dwReturned);

                    if (dwState == (int)CHCNetSDK.NET_SDK_SENDWITHRECV_STATUS.NET_SDK_CONFIG_STATUS_NEEDWAIT)
                    {
                        Thread.Sleep(10);
                        continue;
                    }
                    else if (dwState == (int)CHCNetSDK.NET_SDK_SENDWITHRECV_STATUS.NET_SDK_CONFIG_STATUS_FAILED)
                    {
                        MessageBox.Show("Get User Fail error:" + CHCNetSDK.NET_DVR_GetLastError());
                    }
                    else if (dwState == (int)CHCNetSDK.NET_SDK_SENDWITHRECV_STATUS.NET_SDK_CONFIG_STATUS_SUCCESS)
                    {
                        byte[] bUserInfoSearch = new byte[1024 * 10];
                        Marshal.Copy(ptrJsonData, bUserInfoSearch, 0, (int)dwReturned);
                        string strUserInfoSearch = System.Text.Encoding.UTF8.GetString(bUserInfoSearch);

                        CUserInfoSearchCfg JsonUserInfoSearchCfg = new CUserInfoSearchCfg();
                        JsonUserInfoSearchCfg = JsonConvert.DeserializeObject<CUserInfoSearchCfg>(strUserInfoSearch);
                        if (JsonUserInfoSearchCfg.UserInfoSearch == null)
                        {
                            //null说明返回的Json报文不是UserInfoSearch，而是ResponseStatus
                            CResponseStatus JsonResponseStatus = new CResponseStatus();
                            JsonResponseStatus = JsonConvert.DeserializeObject<CResponseStatus>(strUserInfoSearch);
                            if (JsonResponseStatus.statusCode == 1)
                            {
                                MessageBox.Show("Get User Success");
                            }
                            else
                            {
                                MessageBox.Show("Get User Fail, ResponseStatus.statusCode" + JsonResponseStatus.statusCode);
                            }
                        }
                        else
                        {
                            //解析UserInfoSearch报文
                            if (JsonUserInfoSearchCfg.UserInfoSearch.totalMatches == 0)
                            {
                                MessageBox.Show("no this EmployeeNo person");
                                break;
                            }
                            for (int i = 0; i < JsonUserInfoSearchCfg.UserInfoSearch.numOfMatches; )
                            {
                                textBoxEmployeeNo.Text = JsonUserInfoSearchCfg.UserInfoSearch.UserInfo[i].employeeNo;
                                if (JsonUserInfoSearchCfg.UserInfoSearch.UserInfo[i].name != null)
                                {
                                    textBoxName.Text = JsonUserInfoSearchCfg.UserInfoSearch.UserInfo[i].name;
                                }
                                if (JsonUserInfoSearchCfg.UserInfoSearch.UserInfo[i].RightPlan != null)
                                {
                                    textBoxRightPlan.Text = JsonUserInfoSearchCfg.UserInfoSearch.UserInfo[i].RightPlan[0].planTemplateNo;
                                }
                                break;//循环一次就break吧，界面比较简单不能都显示出来
                            }
                            MessageBox.Show("Get User Success");
                        }
                        break;
                    }
                    else if (dwState == (int)CHCNetSDK.NET_SDK_SENDWITHRECV_STATUS.NET_SDK_CONFIG_STATUS_FINISH)
                    {
                        MessageBox.Show("Get User Finish");
                        break;
                    }
                    else if (dwState == (int)CHCNetSDK.NET_SDK_SENDWITHRECV_STATUS.NET_SDK_CONFIG_STATUS_EXCEPTION)
                    {
                        MessageBox.Show("Get User Exception error:" + CHCNetSDK.NET_DVR_GetLastError());
                        break;
                    }
                    else
                    {
                        MessageBox.Show("unknown status error:" + CHCNetSDK.NET_DVR_GetLastError());
                        break;
                    }
                }
                Marshal.FreeHGlobal(ptrUserInfoSearchCondCfg);
                Marshal.FreeHGlobal(ptrJsonData);
            }
            if (m_lGetUserCfgHandle != -1)
            {
                if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lGetUserCfgHandle))
                {
                    m_lSetUserCfgHandle = -1;
                }
            }
            Marshal.FreeHGlobal(ptrURL);
        }

        private void btnDelete_Click(object sender, EventArgs e)
        {
            IntPtr ptrOutBuf = Marshal.AllocHGlobal(1024);
            IntPtr ptrStatusBuffer = Marshal.AllocHGlobal(1024);
            for (int i = 0; i < 1024; i++)
            {
                Marshal.WriteByte(ptrOutBuf, i, 0);
                Marshal.WriteByte(ptrStatusBuffer, i, 0);
            }

            CHCNetSDK.NET_DVR_XML_CONFIG_INPUT struInput = new CHCNetSDK.NET_DVR_XML_CONFIG_INPUT();
            CHCNetSDK.NET_DVR_XML_CONFIG_OUTPUT struOuput = new CHCNetSDK.NET_DVR_XML_CONFIG_OUTPUT();

            string sUrl = "PUT /ISAPI/AccessControl/UserInfoDetail/Delete?format=json";
            IntPtr ptrURL = Marshal.StringToHGlobalAnsi(sUrl);
            struInput.dwSize = (uint)Marshal.SizeOf(struInput);
            struInput.lpRequestUrl = ptrURL;
            struInput.dwRequestUrlLen = (uint)sUrl.Length;

            CUserInfoDetailCfg JsonUserInfoDetailCfg = new CUserInfoDetailCfg();
            JsonUserInfoDetailCfg.UserInfoDetail = new CUserInfoDetail();
            JsonUserInfoDetailCfg.UserInfoDetail.mode = "byEmployeeNo";
            JsonUserInfoDetailCfg.UserInfoDetail.EmployeeNoList = new List<CEmployeeNoList>();
            CEmployeeNoList singleEmployeeNoList = new CEmployeeNoList();
            singleEmployeeNoList.employeeNo = textBoxEmployeeNo.Text;
            JsonUserInfoDetailCfg.UserInfoDetail.EmployeeNoList.Add(singleEmployeeNoList);
            string strUserInfoDetailCfg = JsonConvert.SerializeObject(JsonUserInfoDetailCfg);
            IntPtr ptrUserInfoDetailCfg = Marshal.StringToHGlobalAnsi(strUserInfoDetailCfg);

            struInput.lpInBuffer = ptrUserInfoDetailCfg;
            struInput.dwInBufferSize = (uint)strUserInfoDetailCfg.Length;

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
                Marshal.FreeHGlobal(ptrUserInfoDetailCfg);
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

                Marshal.FreeHGlobal(ptrOutBuf);
                Marshal.FreeHGlobal(ptrStatusBuffer);
                Marshal.FreeHGlobal(ptrUserInfoDetailCfg);
                Marshal.FreeHGlobal(ptrInput);
                Marshal.FreeHGlobal(ptrOuput);
                Marshal.FreeHGlobal(ptrURL);
            }

            if (-1 != m_lDelUserCfgHandle)
            {
                if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lDelUserCfgHandle))
                {
                    m_lDelUserCfgHandle = -1;
                }
            }
            string sUrlDeleteProcess = "GET /ISAPI/AccessControl/UserInfoDetail/DeleteProcess?format=json";
            IntPtr ptrUrlDeleteProcess = Marshal.StringToHGlobalAnsi(sUrlDeleteProcess);
            m_lDelUserCfgHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_UserID, CHCNetSDK.NET_DVR_JSON_CONFIG, ptrUrlDeleteProcess, sUrlDeleteProcess.Length, null, IntPtr.Zero);
            if (m_lDelUserCfgHandle < 0)
            {
                MessageBox.Show("NET_DVR_StartRemoteConfig fail [url:GET /ISAPI/AccessControl/UserInfoDetail/DeleteProcess?format=json] error:" + CHCNetSDK.NET_DVR_GetLastError());
                Marshal.FreeHGlobal(ptrUrlDeleteProcess);
                return;
            }
            else
            {
                IntPtr ptrJsonData = Marshal.AllocHGlobal(1024);
                for (int i = 0; i < 1024; i++)
                {
                    Marshal.WriteByte(ptrJsonData, i, 0);
                }
                int dwState = (int)CHCNetSDK.NET_SDK_SENDWITHRECV_STATUS.NET_SDK_CONFIG_STATUS_SUCCESS;
                while (true)
                {
                    dwState = CHCNetSDK.NET_DVR_GetNextRemoteConfig(m_lDelUserCfgHandle, ptrJsonData, 1024);
                    if (dwState == -1)
                    {
                        uint a = CHCNetSDK.NET_DVR_GetLastError();
                    }
                    string strJsonData = Marshal.PtrToStringAnsi(ptrJsonData);
                    if (dwState == (int)CHCNetSDK.NET_SDK_SENDWITHRECV_STATUS.NET_SDK_CONFIG_STATUS_NEEDWAIT)
                    {
                        Thread.Sleep(10);
                        continue;
                    }
                    else if (dwState == (int)CHCNetSDK.NET_SDK_SENDWITHRECV_STATUS.NET_SDK_CONFIG_STATUS_FAILED)
                    {
                        MessageBox.Show("Get DelUser Process Fail error:" + CHCNetSDK.NET_DVR_GetLastError());
                    }
                    else if (dwState == (int)CHCNetSDK.NET_SDK_SENDWITHRECV_STATUS.NET_SDK_CONFIG_STATUS_SUCCESS)
                    {
                        CUserInfoDetailDeleteProcessCfg JsonUserInfoSearchCfg = new CUserInfoDetailDeleteProcessCfg();
                        JsonUserInfoSearchCfg = JsonConvert.DeserializeObject<CUserInfoDetailDeleteProcessCfg>(strJsonData);
                        if (JsonUserInfoSearchCfg.UserInfoDetailDeleteProcess == null)
                        {
                            //null说明返回的Json报文不是UserInfoSearch，而是ResponseStatus
                            CResponseStatus JsonResponseStatus = new CResponseStatus();
                            JsonResponseStatus = JsonConvert.DeserializeObject<CResponseStatus>(strJsonData);
                            if (JsonResponseStatus.statusCode == 1)
                            {
                                //不会走到这里
                                MessageBox.Show("Get DelUser Process Success");
                            }
                            else
                            {
                                MessageBox.Show("Get DelUser Process Fail, ResponseStatus.statusCode" + JsonResponseStatus.statusCode);
                            }
                        }
                        else
                        {
                            //解析UserInfoDetailDeleteProcess报文
                            if (JsonUserInfoSearchCfg.UserInfoDetailDeleteProcess.status == "success")
                            {
                                MessageBox.Show("Del User Success");
                            }
                            else if (JsonUserInfoSearchCfg.UserInfoDetailDeleteProcess.status == "failed")
                            {
                                MessageBox.Show("Del User Failed"); 
                            }
                            else if (JsonUserInfoSearchCfg.UserInfoDetailDeleteProcess.status == "processing")
                            {
                                MessageBox.Show("Del User processing"); 
                            }
                        }
                        break;
                    }
                    else if (dwState == (int)CHCNetSDK.NET_SDK_SENDWITHRECV_STATUS.NET_SDK_CONFIG_STATUS_FINISH)
                    {
                        MessageBox.Show("Get DelUser Process Finish");
                        break;
                    }
                    else if (dwState == (int)CHCNetSDK.NET_SDK_SENDWITHRECV_STATUS.NET_SDK_CONFIG_STATUS_EXCEPTION)
                    {
                        MessageBox.Show("Get DelUser Process Exception error:" + CHCNetSDK.NET_DVR_GetLastError());
                        break;
                    }
                    else
                    {
                        MessageBox.Show("unknown Status Error:" + CHCNetSDK.NET_DVR_GetLastError());
                        break;
                    }
                }
                Marshal.FreeHGlobal(ptrJsonData);
            }
            if (-1 != m_lDelUserCfgHandle)
            {
                if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lDelUserCfgHandle))
                {
                    m_lDelUserCfgHandle = -1;
                }
            }
            Marshal.FreeHGlobal(ptrUrlDeleteProcess);
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
