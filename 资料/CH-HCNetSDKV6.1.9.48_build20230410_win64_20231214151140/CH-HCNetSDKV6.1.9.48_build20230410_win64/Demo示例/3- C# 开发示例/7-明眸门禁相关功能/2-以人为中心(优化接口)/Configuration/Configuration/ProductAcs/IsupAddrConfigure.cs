using Configuration.Language;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Configuration.ProductAcs
{
    public partial class IsupAddrConfigure : Form
    {

        public int m_UserId = -1;
        public CHCNetSDK.NET_DVR_NETCFG_V50 m_struNetCfg = new CHCNetSDK.NET_DVR_NETCFG_V50();

        public CHCNetSDK.NET_DVR_ETHERNET_V30[] TempEthernet = new CHCNetSDK.NET_DVR_ETHERNET_V30[2];

        public CHCNetSDK.NET_DVR_NETCFG_V50 m_struNetCfgOut = new CHCNetSDK.NET_DVR_NETCFG_V50();
        public CHCNetSDK.NET_DVR_NETCFG_V50 m_struNetCfgIn = new CHCNetSDK.NET_DVR_NETCFG_V50();
        public byte m_CheckDHCP;
        public byte m_bDNS;

        public CHCNetSDK.NET_DVR_ALARMHOST_NETCFG m_struAlarmCfg = new CHCNetSDK.NET_DVR_ALARMHOST_NETCFG();
        public CHCNetSDK.NET_DVR_ALARMHOST_ABILITY m_struAbility = new CHCNetSDK.NET_DVR_ALARMHOST_ABILITY();

        public CHCNetSDK.NET_DVR_ALARMHOST_NETCFG_V50 m_struNetCfgV50 = new CHCNetSDK.NET_DVR_ALARMHOST_NETCFG_V50();
        public Dictionary<int, Panel> PanelDictory = new Dictionary<int, Panel>();
        public int CurrIndex = 0;
        public byte EnableUploadMode = 0;
        public int m_CenterNum = 0;

        public TreeNode node = new TreeNode();
        public IsupAddrConfigure()
        {
            InitializeComponent();
            MultiLanguage.LoadLanguage(this);
            IntPtr AbilityCfg = Marshal.AllocHGlobal(Marshal.SizeOf(m_struAbility));
            Marshal.StructureToPtr(m_struAbility, AbilityCfg, true);
            uint dwSize = (uint)Marshal.SizeOf(m_struAbility);
            IntPtr InBuffer = IntPtr.Zero;
            GroupCenterCombox.Items.Clear();
            if (!CHCNetSDK.NET_DVR_GetDeviceAbility(m_UserId, CHCNetSDK.ALARMHOST_ABILITY, InBuffer, 0, AbilityCfg, dwSize))
            {
                for (int i = 0; i < 1; i++)
                {
                    string CenterInfo = string.Format("CENTER{0}", i + 1);
                    GroupCenterCombox.Items.Add(CenterInfo);
                }
            }
            else
            {
                for (int i = 0; i < m_struAbility.byNetNum; i++)
                {
                    string CenterInfo = string.Format("CENTER{0}", i + 1);
                    GroupCenterCombox.Items.Add(CenterInfo);
                }
            }
        }
        private void btnGet_Click(object sender, EventArgs e)
        {
            CHCNetSDK.NET_DVR_ALARMHOST_NETCFG_V50 m_struNetCfgInV50 = new CHCNetSDK.NET_DVR_ALARMHOST_NETCFG_V50();
            m_struNetCfgInV50.Init();
            uint dwReturn = 0;
            int iNetType = 1;
            m_struNetCfgInV50.dwSize = (uint)Marshal.SizeOf(m_struNetCfgInV50);
            string IPV6 = string.Empty;
            string IpV4 = string.Empty;
            string Account = string.Empty;
            string ISUPKey = string.Empty;
            string DoMianName = string.Empty;
            for (int i = 0; i < CHCNetSDK.MAX_CENTERNUM; i++)
            {
                m_struNetCfgInV50.struNetCenter[i].struIP.Init();
                m_struNetCfgInV50.struNetCenter[i].struIP.sIpV4 = System.Text.Encoding.Default.GetBytes(IpV4.Trim().PadRight(16, '\0').ToCharArray());
                m_struNetCfgInV50.struNetCenter[i].struIP.byIPv6 = System.Text.Encoding.Default.GetBytes(IPV6.Trim().PadRight(128, '\0').ToCharArray());
                m_struNetCfgInV50.struNetCenter[i].byDevID = System.Text.Encoding.Default.GetBytes(Account.PadRight(32, '\0').ToCharArray());
                m_struNetCfgInV50.struNetCenter[i].byEHomeKey = System.Text.Encoding.Default.GetBytes(ISUPKey.PadRight(32, '\0').ToCharArray());
                m_struNetCfgInV50.struNetCenter[i].byDomainName = System.Text.Encoding.Default.GetBytes(DoMianName.PadRight(64, '\0').ToCharArray());
            }
            string RES1 = string.Empty;
            //Inital Member Data
            m_struNetCfgInV50.byRes1 = System.Text.Encoding.Default.GetBytes(RES1.Trim().PadRight(128, '\0'));
            IntPtr struNetCfgV50 = Marshal.AllocHGlobal(Marshal.SizeOf(m_struNetCfgInV50));
            Marshal.StructureToPtr(m_struNetCfgInV50, struNetCfgV50, true);

            if (!CHCNetSDK.NET_DVR_GetDVRConfig(m_UserId, CHCNetSDK.NET_DVR_GET_ALARMHOST_NETCFG_V50, iNetType, struNetCfgV50,
                (uint)Marshal.SizeOf(m_struNetCfgInV50), ref dwReturn))
            {
                MessageBox.Show("Get ISUP Param Failed: " + CHCNetSDK.NET_DVR_GetLastError());
            }
            else
            {
                CHCNetSDK.NET_DVR_ALARMHOST_NETCFG_V50 m_struNetCfgOutV50 = new CHCNetSDK.NET_DVR_ALARMHOST_NETCFG_V50();
                m_struNetCfgOutV50 = (CHCNetSDK.NET_DVR_ALARMHOST_NETCFG_V50)Marshal.PtrToStructure(struNetCfgV50,
                    typeof(CHCNetSDK.NET_DVR_ALARMHOST_NETCFG_V50));
                GroupCenterCombox.SelectedIndex = 0;
                AddressTypeCombox.SelectedIndex = m_struNetCfgOutV50.struNetCenter[0].byAddressType;
                IPAddress.Text = System.Text.Encoding.Default.GetString(m_struNetCfgOutV50.struNetCenter[0].struIP.sIpV4);
                PortBox.Text = Convert.ToString(m_struNetCfgOutV50.struNetCenter[0].wPort);
                ProtocolTypeCombox.SelectedIndex = m_struNetCfgOutV50.struNetCenter[0].byReportProtocol - 1;
                IsupVersionCombox.SelectedIndex = m_struNetCfgOutV50.struNetCenter[0].byProtocolVersion;
                AccountBox.Text = System.Text.Encoding.Default.GetString(m_struNetCfgOutV50.struNetCenter[0].byDevID);
                MessageBox.Show("Get Isup Net Param Success");
            }
            Marshal.FreeHGlobal(struNetCfgV50);
        }

        private void btnSet_Click(object sender, EventArgs e)
        {
            string EhomeKey = IsupKey.Text;
            if (EhomeKey.Length > 32)
            {
                MessageBox.Show("the length of Isup Key Should not Exceed 32");
                return;
            }
            CHCNetSDK.NET_DVR_ALARMHOST_NETPARAM_V50 m_struParamV50 = new CHCNetSDK.NET_DVR_ALARMHOST_NETPARAM_V50();
            m_struParamV50.Init();
            m_struParamV50.struIP.Init();
            string IPV6 = string.Empty;
            m_struParamV50.struIP.sIpV4 =  System.Text.Encoding.Default.GetBytes(IPAddress.Text.Trim().PadRight(16, '\0').ToCharArray());
            m_struParamV50.struIP.byIPv6 = System.Text.Encoding.Default.GetBytes(IPV6.Trim().PadRight(128, '\0').ToCharArray());
            int Port = int.Parse(PortBox.Text);
            m_struParamV50.wPort = (ushort)Port;
            m_struParamV50.byReportProtocol = Convert.ToByte(ProtocolTypeCombox.SelectedIndex + 1);

            m_struParamV50.byDevID = System.Text.Encoding.Default.GetBytes(AccountBox.Text.Trim().PadRight(32, '\0').ToCharArray());

            m_struParamV50.byEHomeKey = System.Text.Encoding.Default.GetBytes(IsupKey.Text.Trim().PadRight(32, '\0').ToCharArray());

            m_struParamV50.byDomainName = System.Text.Encoding.Default.GetBytes(DoMainName.Text.Trim().PadRight(64, '\0').ToCharArray());

            m_struParamV50.byAddressType = Convert.ToByte(AddressTypeCombox.SelectedIndex);

            m_struParamV50.byEnable = Convert.ToByte(1);

            m_struParamV50.byProtocolVersion = Convert.ToByte(3);

            IntPtr struParamCfgV50 = Marshal.AllocHGlobal(Marshal.SizeOf(m_struParamV50));

            Marshal.StructureToPtr(m_struParamV50, struParamCfgV50, true);
            m_struNetCfgV50.struNetCenter = new CHCNetSDK.NET_DVR_ALARMHOST_NETPARAM_V50[4];

            m_struNetCfgV50.struNetCenter[GroupCenterCombox.SelectedIndex] = (CHCNetSDK.NET_DVR_ALARMHOST_NETPARAM_V50)
                Marshal.PtrToStructure(struParamCfgV50, typeof(CHCNetSDK.NET_DVR_ALARMHOST_NETPARAM_V50));
            m_struNetCfgV50.dwSize = (uint)Marshal.SizeOf(m_struNetCfgV50);
            Marshal.FreeHGlobal(struParamCfgV50);
            IntPtr NetCfgV50 = Marshal.AllocHGlobal(Marshal.SizeOf(m_struNetCfgV50));
            Marshal.StructureToPtr(m_struNetCfgV50, NetCfgV50, true);
            int iNetType = 1;

            if (!CHCNetSDK.NET_DVR_SetDVRConfig(m_UserId, CHCNetSDK.NET_DVR_SET_ALARMHOST_NETCFG_V50, iNetType, NetCfgV50, m_struNetCfgV50.dwSize))
            {
                MessageBox.Show("Set Isup Param  Failed: " + CHCNetSDK.NET_DVR_GetLastError());
            }
            else
            {
                MessageBox.Show("Set Isup Param  Success! ");
            }
            Marshal.FreeHGlobal(NetCfgV50);
            return;
        }
    }
}
