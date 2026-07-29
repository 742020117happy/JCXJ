using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Runtime.InteropServices;

namespace NVRCsharpDemo
{  
    public partial class MainWindow : Form
    {
        private bool m_bInitSDK = false;
        private uint iLastErr = 0;
        private Int32 m_lUserID = -1;
        private Int32 m_lFindHandle = -1;
        private string str;
        private string str1;
        private string str2;
        private string str3;
        private string sDVRFileName = null;
        private Int32 i = 0;
        private Int32 m_lTree=0;

        private long iSelIndex = 0;
        private uint dwAChanTotalNum = 0;
        private uint dwDChanTotalNum = 0;
        public CHCNetSDK.NET_DVR_DEVICEINFO_V30 DeviceInfo;
        public CHCNetSDK.NET_DVR_IPPARACFG_V40 m_struIpParaCfgV40;
        public CHCNetSDK.NET_DVR_GET_STREAM_UNION m_unionGetStream;
        public CHCNetSDK.NET_DVR_IPCHANINFO m_struChanInfo;

        [MarshalAsAttribute(UnmanagedType.ByValArray, SizeConst = 96, ArraySubType = UnmanagedType.U4)]
        private int[] iChannelNum;

        public MainWindow()
        {
            InitializeComponent();
            m_bInitSDK = CHCNetSDK.NET_DVR_Init();
            if (m_bInitSDK == false)
            {
                MessageBox.Show("NET_DVR_Init error!");
                return;
            }
            else
            {
                //保存SDK日志
                CHCNetSDK.NET_DVR_SetLogToFile(3,"C:\\SdkLog\\", true);
                iChannelNum = new int[96];
            }
        }

        private void btnLogin_Click(object sender, EventArgs e)
        {
            if (textBoxIP.Text == "" || textBoxPort.Text == "" ||
                textBoxUserName.Text == "" || textBoxPassword.Text == "")
            {
                MessageBox.Show("Please input IP, Port, User name and Password!");
                return;
            }
            if (m_lUserID < 0)
            {
                string DVRIPAddress = textBoxIP.Text; //设备IP地址或者域名
                Int16 DVRPortNumber = Int16.Parse(textBoxPort.Text);//设备服务端口号
                string DVRUserName = textBoxUserName.Text;//设备登录用户名
                string DVRPassword = textBoxPassword.Text;//设备登录密码

            //    DeviceInfo = new CHCNetSDK.NET_DVR_DEVICEINFO_V30();

                //登录设备 Login the device
                m_lUserID = CHCNetSDK.NET_DVR_Login_V30(DVRIPAddress, DVRPortNumber, DVRUserName, DVRPassword, ref DeviceInfo);
                if (m_lUserID < 0)
                {
                    iLastErr = CHCNetSDK.NET_DVR_GetLastError();
                    str1 = "NET_DVR_Login_V30 failed, error code= " + iLastErr; //登录失败，输出错误号
                    MessageBox.Show(str1);
                    return;
                }
                else
                {
                    //登录成功
                    MessageBox.Show("Login Success!");
                    btnLogin.Text = "Logout";

                    dwAChanTotalNum = (uint)DeviceInfo.byChanNum;
                    dwDChanTotalNum = (uint)DeviceInfo.byIPChanNum + 256 * (uint)DeviceInfo.byHighDChanNum;

                    if (dwDChanTotalNum > 0)
                    {
                        InfoIPChannel();
                    }
                    else
                    {
                        for (i = 0; i < dwAChanTotalNum; i++)
                        {
                            ListAnalogChannel(i+1, 1);
                            iChannelNum[i] = i + (int)DeviceInfo.byStartChan;
                        }
                       // MessageBox.Show("This device has no IP channel!");
                    }
                }

            }
            else
            {                
                //注销登录 Logout the device
                if (!CHCNetSDK.NET_DVR_Logout(m_lUserID))
                {
                    iLastErr = CHCNetSDK.NET_DVR_GetLastError();
                    str1 = "NET_DVR_Logout failed, error code= " + iLastErr;
                    MessageBox.Show(str1);
                    return;
                }
                listViewIPChannel.Items.Clear();//清空通道列表
                m_lUserID = -1;
                btnLogin.Text = "Login";
            }
            return;
        }

        public void InfoIPChannel()
        {
            uint dwSize = (uint)Marshal.SizeOf(m_struIpParaCfgV40);

            IntPtr ptrIpParaCfgV40 = Marshal.AllocHGlobal((Int32)dwSize);
            Marshal.StructureToPtr(m_struIpParaCfgV40, ptrIpParaCfgV40, false);

            uint dwReturn = 0;
            int iGroupNo = 0; //该Demo仅获取第一组64个通道，如果设备IP通道大于64路，需要按组号0~i多次调用NET_DVR_GET_IPPARACFG_V40获取
            if (!CHCNetSDK.NET_DVR_GetDVRConfig(m_lUserID, CHCNetSDK.NET_DVR_GET_IPPARACFG_V40, iGroupNo, ptrIpParaCfgV40, dwSize, ref dwReturn))
            {
                iLastErr = CHCNetSDK.NET_DVR_GetLastError();
                str1 = "NET_DVR_GET_IPPARACFG_V40 failed, error code= " + iLastErr; //获取IP资源配置信息失败，输出错误号
                MessageBox.Show(str1);
            }
            else
            {
                // succ
                m_struIpParaCfgV40 = (CHCNetSDK.NET_DVR_IPPARACFG_V40)Marshal.PtrToStructure(ptrIpParaCfgV40, typeof(CHCNetSDK.NET_DVR_IPPARACFG_V40));
               
                for (i = 0; i < dwAChanTotalNum; i++)
                {
                    ListAnalogChannel(i+1, m_struIpParaCfgV40.byAnalogChanEnable[i]);
                    iChannelNum[i] = i + (int)DeviceInfo.byStartChan;                     
                }
                
                byte byStreamType;
                uint iDChanNum = 64;

                if (dwDChanTotalNum < 64)
                {
                    iDChanNum = dwDChanTotalNum; //如果设备IP通道小于64路，按实际路数获取
                }

                for (i = 0; i < iDChanNum; i++)
                {
                    iChannelNum[i + dwAChanTotalNum] = i + (int)m_struIpParaCfgV40.dwStartDChan;

                    byStreamType = m_struIpParaCfgV40.struStreamMode[i].byGetStreamType;
                    m_unionGetStream = m_struIpParaCfgV40.struStreamMode[i].uGetStream;

                    switch (byStreamType)
                    {
                        //目前NVR仅支持0- 直接从设备取流一种方式
                        case 0:
                            dwSize = (uint)Marshal.SizeOf(m_unionGetStream);
                            IntPtr ptrChanInfo = Marshal.AllocHGlobal((Int32)dwSize);
                            Marshal.StructureToPtr(m_unionGetStream, ptrChanInfo, false);
                            m_struChanInfo = (CHCNetSDK.NET_DVR_IPCHANINFO)Marshal.PtrToStructure(ptrChanInfo, typeof(CHCNetSDK.NET_DVR_IPCHANINFO));

                            //列出IP通道
                            ListIPChannel(i + 1, m_struChanInfo.byEnable, m_struChanInfo.byIPID);
                            Marshal.FreeHGlobal(ptrChanInfo);
                            break;

                        default:
                            break;
                    }
                }
            }
            Marshal.FreeHGlobal(ptrIpParaCfgV40);
        }
        public void ListIPChannel(Int32 iChanNo, byte byOnline, byte byIPID)
        {
            str1 = String.Format("IPCamera {0}", iChanNo);
            m_lTree++;

            if (byIPID == 0)
            {
                str2 = "X"; //通道空闲，没有添加前端设备                 
            }
            else
            { 
                if(byOnline==0)
                {
                    str2 = "offline"; //通道不在线
                }
                else
                    str2 = "online"; //通道在线
            }
            
            listViewIPChannel.Items.Add(new ListViewItem(new string[] {str1, str2}));//将通道添加到列表中
        }
        public void ListAnalogChannel(Int32 iChanNo, byte byEnable)
        {
            str1 = String.Format("Camera {0}", iChanNo);
            m_lTree++;

            if (byEnable == 0)
            {
                str2 = "Disabled"; //通道已被禁用 This channel has been disabled               
            }
            else
            {
                str2 = "Enabled"; //通道处于启用状态
            }

            listViewIPChannel.Items.Add(new ListViewItem(new string[] {str1, str2 }));//将通道添加到列表中
        }

        private void listViewIPChannel_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
        {
            if (listViewIPChannel.SelectedItems.Count > 0) 
            {
                iSelIndex = listViewIPChannel.SelectedItems[0].Index;  //当前选中的行
            }
        }

        private void btnSearch_Click(object sender, EventArgs e)
        {

            listViewFile.Items.Clear();//清空文件列表 

            CHCNetSDK.NET_DVR_FIND_PICTURE_PARAM struFindParam = new CHCNetSDK.NET_DVR_FIND_PICTURE_PARAM();

            struFindParam.lChannel = iChannelNum[(int)iSelIndex]; //通道号 Channel number
            struFindParam.byFileType = 0xff; //0xff-全部，0-定时录像，1-移动侦测，2-报警触发，...

            //设置录像查找的开始时间 Set the starting time to search video files
            struFindParam.struStartTime.dwYear = dateTimeStart.Value.Year;
            struFindParam.struStartTime.dwMonth = dateTimeStart.Value.Month;
            struFindParam.struStartTime.dwDay = dateTimeStart.Value.Day;
            struFindParam.struStartTime.dwHour = dateTimeStart.Value.Hour;
            struFindParam.struStartTime.dwMinute = dateTimeStart.Value.Minute;
            struFindParam.struStartTime.dwSecond = dateTimeStart.Value.Second;

            //设置录像查找的结束时间 Set the stopping time to search video files
            struFindParam.struStopTime.dwYear = dateTimeEnd.Value.Year;
            struFindParam.struStopTime.dwMonth = dateTimeEnd.Value.Month;
            struFindParam.struStopTime.dwDay = dateTimeEnd.Value.Day;
            struFindParam.struStopTime.dwHour = dateTimeEnd.Value.Hour;
            struFindParam.struStopTime.dwMinute = dateTimeEnd.Value.Minute;
            struFindParam.struStopTime.dwSecond = dateTimeEnd.Value.Second;

            struFindParam.byEventType = 1; //事件类型：0-保留，1-交通事件，2-违章取证，3-其他事件

            //开始录像文件查找 Start to search video files 
            m_lFindHandle = CHCNetSDK.NET_DVR_FindPicture(m_lUserID, ref struFindParam);

            if (m_lFindHandle < 0)
            {
                iLastErr = CHCNetSDK.NET_DVR_GetLastError();
                str = "NET_DVR_FindPicture failed, error code= " + iLastErr; //预览失败，输出错误号
                MessageBox.Show(str);
            }
            else
            {
                CHCNetSDK.NET_DVR_FIND_PICTURE_V50 struFileData = new CHCNetSDK.NET_DVR_FIND_PICTURE_V50(); ;
                while(true)
                {
                    //逐个获取查找到的文件信息 Get file information one by one.
                    int result = CHCNetSDK.NET_DVR_FindNextPicture_V50(m_lFindHandle, ref struFileData);

                    if (result == CHCNetSDK.NET_DVR_ISFINDING)  //正在查找请等待 Searching, please wait
                    {
                        continue;
                    }
                    else if (result == CHCNetSDK.NET_DVR_FILE_SUCCESS) //获取文件信息成功 Get the file information successfully
                    {
                        str1 = System.Text.Encoding.UTF8.GetString(struFileData.sFileName).Trim();

                        str2 = Convert.ToString(struFileData.struTime.dwYear) + "-" +
                            Convert.ToString(struFileData.struTime.dwMonth) + "-" +
                            Convert.ToString(struFileData.struTime.dwDay) + " " +
                            Convert.ToString(struFileData.struTime.dwHour) + ":" +
                            Convert.ToString(struFileData.struTime.dwMinute) + ":" +
                            Convert.ToString(struFileData.struTime.dwSecond);

                        str3 = System.Text.Encoding.GetEncoding("GBK").GetString(struFileData.sLicense).Trim();
                        listViewFile.Items.Add(new ListViewItem(new string[] { str1, str2, str3}));//将查找的录像文件添加到列表中

                    }
                    else if (result == CHCNetSDK.NET_DVR_FILE_NOFIND || result == CHCNetSDK.NET_DVR_NOMOREFILE) 
                    {
                        break; //未查找到文件或者查找结束，退出   No file found or no more file found, search is finished 
                    }
                    else
                    {
                        break;
                    }
                }

                if (!CHCNetSDK.NET_DVR_CloseFindPicture(m_lFindHandle))
                {
                    iLastErr = CHCNetSDK.NET_DVR_GetLastError();
                    str = "NET_DVR_CloseFindPicture failed, error code= " + iLastErr; //预览失败，输出错误号
                    MessageBox.Show(str);
                }
               
            }

        }

        private void listViewFile_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (listViewFile.SelectedItems.Count > 0)
            {
                sDVRFileName = listViewFile.FocusedItem.SubItems[0].Text;
            }
        }

        private void btnDownload_Click(object sender, EventArgs e)
        {

            string sSavedFileName;  //录像文件保存路径和文件名 the path and file name to save      
            sSavedFileName = "Downtest_" + sDVRFileName + ".jpg";

            //按文件名下载 Download by file name
            bool bGetPic = CHCNetSDK.NET_DVR_GetPicture(m_lUserID, sDVRFileName, sSavedFileName);
            if (bGetPic)
            {
                str = "NET_DVR_GetPicture success, file in " + sSavedFileName;
                MessageBox.Show(str);
            }
            else
            {
                iLastErr = CHCNetSDK.NET_DVR_GetLastError();
                str = "NET_DVR_GetPicture failed, error code= " + iLastErr;
                MessageBox.Show(str);
                return;
            }
        }


        private void MainWindow_Load(object sender, EventArgs e)
        {
            //初始化时间
            dateTimeStart.Text = DateTime.Now.ToShortDateString();
            dateTimeEnd.Text = DateTime.Now.ToString();
        }

        private void btn_Exit_Click(object sender, EventArgs e)
        {
            //注销登录 Logout the device
            if (m_lUserID >= 0)
            {
                CHCNetSDK.NET_DVR_Logout(m_lUserID);
                m_lUserID = -1;
            }

            Application.Exit();
        }

        private void label13_Click(object sender, EventArgs e)
        {

        }

        private void label19_Click(object sender, EventArgs e)
        {

        }

        private void label18_Click(object sender, EventArgs e)
        {

        }

        private void dateTimeStart_ValueChanged(object sender, EventArgs e)
        {

        }
    }
}
