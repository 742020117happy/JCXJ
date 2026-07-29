using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using Configuration.Language;
using Newtonsoft.Json;

namespace Configuration
{
    public partial class RemoteCheck : Form
    {
        public AcsCfgRoot m_struAcsCfg = new AcsCfgRoot();
        public Int32 m_lUserID = Configuration.m_UserID;

        public RemoteCheck()
        {
            m_struAcsCfg.AcsCfg = new AcsCfg();
            InitializeComponent();
        }

        public bool GetRemoteCfg()
        {
            string strOutbound = string.Empty;
            string szMethod = string.Empty;
            string szUrl = "/ISAPI/AccessControl/AcsCfg?format=json";
            szMethod = "GET";           
            DoRequest(szMethod, szUrl, null, out strOutbound);
            if (strOutbound != string.Empty)
            {
                m_struAcsCfg = JsonConvert.DeserializeObject<AcsCfgRoot>(strOutbound);
            }
            else
            {
                return false;
            }
            ///public string remoteCheckDoorEnabled { get; set; }
            if (m_struAcsCfg.AcsCfg.remoteCheckDoorEnabled == true)
            {
                cbRemoteCheckDoor.SelectedIndex = 0;

                //public string checkChannelType { get; set; }
                if (m_struAcsCfg.AcsCfg.checkChannelType.ToLower().Equals("privatesdk"))
                {
                    cbChannelType.SelectedIndex = 0;
                }
                else if (m_struAcsCfg.AcsCfg.checkChannelType.ToLower().Equals("isapi"))
                {
                    cbChannelType.SelectedIndex = 1;
                }
                else if (m_struAcsCfg.AcsCfg.checkChannelType.ToLower().Equals("isup"))
                {
                    cbChannelType.SelectedIndex = 2;
                }
                else if (m_struAcsCfg.AcsCfg.checkChannelType.ToLower().Equals("ezviz"))
                {
                    cbChannelType.SelectedIndex = 3;
                }

                //public string channelIp { get; set; }
                if (m_struAcsCfg.AcsCfg.channelIp != string.Empty && m_struAcsCfg.AcsCfg.channelIp != null)
                {
                    textBoxChannelIP.Text = m_struAcsCfg.AcsCfg.channelIp;
                }
                else
                {
                    textBoxChannelIP.Text = "";
                }
            }
            else
            {
                cbRemoteCheckDoor.SelectedIndex = 1;
                cbChannelType.SelectedIndex = -1; 
                textBoxChannelIP.Text = "";
            }
            return true;
        }

        public void SetRemoteCfg()
        {
            ///public string remoteCheckDoorEnabled { get; set; }
            if (m_struAcsCfg.AcsCfg == null)
            {
                MessageBox.Show("Please get cfg first!");
                return;
            }
            switch (cbRemoteCheckDoor.SelectedIndex)
            {
                case 0:
                    m_struAcsCfg.AcsCfg.remoteCheckDoorEnabled = true;
                    break;
                case 1:
                    m_struAcsCfg.AcsCfg.remoteCheckDoorEnabled = false;
                    break;
            }

            //public string checkChannelType { get; set; }
            switch (cbChannelType.SelectedIndex)
            {
                case 0:
                    m_struAcsCfg.AcsCfg.checkChannelType = "PrivateSDK";
                    break;
                case 1:
                    m_struAcsCfg.AcsCfg.checkChannelType = "ISAPI";
                    break;
                case 2:
                    m_struAcsCfg.AcsCfg.checkChannelType = "ISUP";
                    break;
                case 3:
                    m_struAcsCfg.AcsCfg.checkChannelType = "Ezviz";
                    break;
            }
            //public string channelIp { get; set; }
            m_struAcsCfg.AcsCfg.channelIp = textBoxChannelIP.Text;
            if(cbChannelType.SelectedIndex == 0 && (m_struAcsCfg.AcsCfg.channelIp == "" || m_struAcsCfg.AcsCfg.channelIp == null))
            {
                MessageBox.Show("please input channelIp!");
                return;
            }
            string szUrl = "/ISAPI/AccessControl/AcsCfg?format=json";
            string strOutbound = string.Empty;
            string szMethod = string.Empty;
            string strInput = string.Empty;
            strInput = JsonConvert.SerializeObject(m_struAcsCfg);
            szMethod = "PUT";
            DoRequest(szMethod, szUrl, strInput, out strOutbound);

            if (strOutbound != string.Empty)
            {
                ResponseStatus rs = JsonConvert.DeserializeObject<ResponseStatus>(strOutbound);
                if (rs.statusCode.Equals("1"))
                {
                    MessageBox.Show("Set Data Succ!");
                }
            }
        }

        private void DoRequest(string strMethod, string strUri, string strInput, out string strOutput)
        {
            strOutput = string.Empty;
            CHCNetSDK.NET_DVR_XML_CONFIG_INPUT struInput = new CHCNetSDK.NET_DVR_XML_CONFIG_INPUT();
            struInput.dwSize = (uint)Marshal.SizeOf(struInput);
            string strRequestUrl = strMethod + " " + strUri; //"PUT" + " " + "/ISAPI/AccessControl/remoteCheck?format=json";
            IntPtr ptrUrl = Marshal.StringToCoTaskMemAnsi(strRequestUrl);
            struInput.lpRequestUrl = ptrUrl;
            struInput.dwRequestUrlLen = (uint)strRequestUrl.Length;
            struInput.dwRecvTimeOut = 3000;
            if (strMethod == "PUT" || strMethod == "POST")
            {
                struInput.lpInBuffer = Marshal.StringToCoTaskMemAnsi(strInput);
                struInput.dwInBufferSize = (uint)strInput.Length;
            }
            else
            {
                struInput.lpInBuffer = IntPtr.Zero;
                struInput.dwInBufferSize = 0;
            }

            IntPtr ptrInput = Marshal.AllocHGlobal(Marshal.SizeOf(struInput));
            Marshal.StructureToPtr(struInput, ptrInput, false);

            CHCNetSDK.NET_DVR_XML_CONFIG_OUTPUT struOutput = new CHCNetSDK.NET_DVR_XML_CONFIG_OUTPUT();
            struOutput.dwSize = (uint)Marshal.SizeOf(struOutput);
            const int ciOutSize = 1024 * 1024; //预留1M接收数据
            IntPtr ptrOut = Marshal.AllocHGlobal(ciOutSize);
            struOutput.lpOutBuffer = ptrOut;
            struOutput.dwOutBufferSize = ciOutSize;
            struOutput.lpStatusBuffer = ptrOut;
            struOutput.dwStatusSize = ciOutSize;
            IntPtr ptrOutput = Marshal.AllocHGlobal(Marshal.SizeOf(struOutput));
            Marshal.StructureToPtr(struOutput, ptrOutput, false);
            bool bRet = CHCNetSDK.NET_DVR_STDXMLConfig((int)m_lUserID, ptrInput, ptrOutput);

            if (!bRet)
            {
                Marshal.FreeHGlobal(ptrInput);
                Marshal.FreeHGlobal(ptrOut);
                Marshal.FreeHGlobal(ptrOutput);
                //MessageBox.Show("failed");
            }
            strOutput = Marshal.PtrToStringAnsi(ptrOut);
            Marshal.FreeHGlobal(ptrInput);
            Marshal.FreeHGlobal(ptrOut);
            Marshal.FreeHGlobal(ptrOutput);
            //MessageBox.Show(strOutput);
        }

        private void btnGet_Click(object sender, EventArgs e)
        {
            GetRemoteCfg();
        }

        private void btnSet_Click(object sender, EventArgs e)
        {
            SetRemoteCfg();
        }

        private void RemoteCheck_Load(object sender, EventArgs e)
        {
            MultiLanguage.LoadLanguage(this);
        }
    }
    public class AcsCfg
    {
        /// <summary>
        /// 
        /// </summary>
        public bool RS485Backup { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public bool showCapPic { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public bool showUserInfo { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public bool overlayUserInfo { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public bool voicePrompt { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public bool uploadCapPic { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public bool saveCapPic { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public bool inputCardNo { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public bool enableWifiDetect { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public bool enable3G4G { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public string protocol { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public bool thermalEnabled { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public bool thermalMode { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public bool thermalPictureEnabled { get; set; }
        /// <summary>
        /// 
        /// </summary>
//         public string thermalIp { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public double highestThermalThreshold { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public double lowestThermalThreshold { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public bool thermalDoorEnabled { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public bool QRCodeEnabled { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public bool remoteCheckDoorEnabled { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public string checkChannelType { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public string channelIp { get; set; }
    }

    public class AcsCfgRoot
    {
        /// <summary>
        /// 
        /// </summary>
        public AcsCfg AcsCfg { get; set; }
    }

}
