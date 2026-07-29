using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.IO;
using System.Threading;
using System.Globalization;
using Configuration.Language;
namespace Configuration.ProductAcs
{
    public partial class WlanConfig : Form
    {
        public static int m_UserID = -1;
        public CHCNetSDK.NET_DVR_NETCFG_V50 m_struNetCfg = new CHCNetSDK.NET_DVR_NETCFG_V50();
        public CHCNetSDK.NET_DVR_NETCFG_V50 m_struNetCfgOut = new CHCNetSDK.NET_DVR_NETCFG_V50();
        public byte m_CheckDHCP;
        public byte m_bDNS;
        public WlanConfig()
        {
            InitializeComponent();
            MultiLanguage.LoadLanguage(this);
            m_struNetCfg.Init();
            IntPtr NetCfg = Marshal.AllocHGlobal(Marshal.SizeOf(m_struNetCfg));
            Marshal.StructureToPtr(m_struNetCfg, NetCfg, true);
            uint dwReturned = 0;
            uint dwSize = (uint)Marshal.SizeOf(m_struNetCfg);
            if (!CHCNetSDK.NET_DVR_GetDVRConfig(m_UserID, CHCNetSDK.NET_DVR_GET_NETCFG_V50, 0, NetCfg, dwSize, ref dwReturned))
            {
                MessageBox.Show("Error: " + CHCNetSDK.NET_DVR_GetLastError());
                Marshal.FreeHGlobal(NetCfg);
                return;
            }
            m_struNetCfgOut = (CHCNetSDK.NET_DVR_NETCFG_V50)Marshal.PtrToStructure(NetCfg, typeof(CHCNetSDK.NET_DVR_NETCFG_V50));
            IPAddress.Text = System.Text.Encoding.Default.GetString(m_struNetCfgOut.struEtherNet[0].struDVRIP.sIpV4);
            SubNetMask.Text = System.Text.Encoding.Default.GetString(m_struNetCfgOut.struEtherNet[0].struDVRIPMask.sIpV4);
            GateWay.Text =  System.Text.Encoding.Default.GetString(m_struNetCfgOut.struGatewayIpAddr.sIpV4);
            string MacAddr = string.Empty;
            if (m_struNetCfgOut.struEtherNet[0].byMACAddr != null)
            {
                for (int i = 0; i < m_struNetCfgOut.struEtherNet[0].byMACAddr.Length; i++)
                {

                    MacAddr += m_struNetCfgOut.struEtherNet[0].byMACAddr[i].ToString("x2");
                    if (i < m_struNetCfgOut.struEtherNet[0].byMACAddr.Length - 1)
                    {
                        MacAddr += ":";
                    }
                }
            }
            MacAddress.Text = MacAddr;
            DNS1Address.Text = System.Text.Encoding.Default.GetString(m_struNetCfgOut.struDnsServer1IpAddr.sIpV4);
            DNS2Address.Text = System.Text.Encoding.Default.GetString(m_struNetCfgOut.struDnsServer2IpAddr.sIpV4); 
            if (m_struNetCfgOut.byUseDhcp == 0xff)
            {
                DHCPEnable.Enabled = false;
                m_CheckDHCP = 0;
            }
            else
            {
                m_CheckDHCP = m_struNetCfgOut.byUseDhcp;
                EnableDhcp();
            }
            if (m_struNetCfg.byEnableDNS == 1)
            {
                m_bDNS = 1;
            }
            else if (m_struNetCfg.byEnableDNS == 2)
            {
                m_bDNS = 0;
            }
            EnableDNS();
            Marshal.FreeHGlobal(NetCfg);
        }
        private void btnSave_Click(object sender, EventArgs e)
        {
            MultiLanguage.LoadLanguage(this);
            //检查输入的IP地址是否合法
            string IP = IPAddress.Text;
            string IPMask = SubNetMask.Text;
            string GateWayValue = GateWay.Text;
            string DNS1Addr = DNS1Address.Text;
            string DNS2Addr = DNS2Address.Text;
            if (!CheckIPStr(IP) || IP.Length == 0)
            {
                MessageBox.Show("非法的IP地址");
                return;
            }
            if (!CheckIPStr(IPMask) || IPMask.Length == 0)
            {
                MessageBox.Show("非法的子网掩码");
                return;
            }
            if (!CheckIPStr(GateWayValue) || GateWayValue.Length == 0)
            {
                MessageBox.Show("非法的网关地址");
                return;
            }
            m_struNetCfgOut.struEtherNet[0].struDVRIP.sIpV4 = System.Text.Encoding.Default.GetBytes(IPAddress.Text.Trim().PadRight(16, '\0').ToCharArray());
            m_struNetCfgOut.byEnableOnvifMulticastDiscovery = 2;
            m_struNetCfgOut.byEnablePrivateMulticastDiscovery = 2;
            if (m_bDNS == 1)
            {
                m_struNetCfgOut.byEnableDNS = 1;
            }
            else
            {
                m_struNetCfgOut.byEnableDNS = 2;
            }

            if (m_CheckDHCP == 1)
            {
                m_struNetCfgOut.byUseDhcp = 1;
            }
            else
            {
                m_struNetCfgOut.byUseDhcp = 0;
            }
            IntPtr NetCfgIn = Marshal.AllocHGlobal(Marshal.SizeOf(m_struNetCfgOut));
            Marshal.StructureToPtr(m_struNetCfgOut, NetCfgIn, true);

            uint dwSize = (uint)Marshal.SizeOf(m_struNetCfgOut);

            if (!CHCNetSDK.NET_DVR_SetDVRConfig(m_UserID, CHCNetSDK.NET_DVR_SET_NETCFG_V50, 0, NetCfgIn, dwSize))
            {
                MessageBox.Show("Save Net Configure Param Failed: " + CHCNetSDK.NET_DVR_GetLastError());
                Marshal.FreeHGlobal(NetCfgIn);
                return;
            }
            Marshal.FreeHGlobal(NetCfgIn);
            MessageBox.Show("Save Net Configure Param Success,Please Relogin Device，Because Device IP Has Changed!");
            return;
        }
        public Boolean CheckIPStr(string ip)
        {
            char[] str = new char[64];
            str = ip.ToCharArray();
            char a;
            int dot = 0;
            int a3, a2, a1, a0;
            a3 = a2 = a1 = a0 = -1;
            int[] PointArr = new int[3];
            int index = 0;
            for (int j = 0; j < str.Length; j++)
            {
                a = str[j];

                if ((a == ' ') || (a == '.') || ((a >= '0') && (a <= '9')))
                {
                    if (a == '.')
                    {
                        PointArr[index++] = j;
                        dot++;
                    }
                }
                else
                {
                    return false;
                }
            }
            if (dot != 3)
            {
                return false;
            }
            else
            {
                a0 = int.Parse(ip.Substring(0, PointArr[0]));
                a1 = int.Parse(ip.Substring(PointArr[0] + 1, PointArr[1] - PointArr[0] - 1));
                a2 = int.Parse(ip.Substring(PointArr[1] + 1, PointArr[2] - PointArr[1] - 1));
                a3 = int.Parse(ip.Substring(PointArr[2] + 1, ip.Length - PointArr[2] - 1));
                if ((a0 > 255) || (a1 > 255) || (a2 > 255) || (a3 > 255))
                {
                    return false;
                }
                if ((a0 < 0) || (a1 < 0) || (a2 < 0) || (a3 < 0))
                {
                    return false;
                }
            }
            return true;
        }
        public void EnableDhcp()
        {
            if (m_CheckDHCP == 1)
            {
                IPAddress.Enabled = false;
                SubNetMask.Enabled = false;
                GateWay.Enabled = false;
                DHCPEnable.Checked = true;
            }
            else
            {
                IPAddress.Enabled = true;
                SubNetMask.Enabled = true;
                GateWay.Enabled = true;
                DNS1Address.Enabled = true;
                DNS2Address.Enabled = true;
                m_bDNS = 0;
                DNSEnable.Visible = false;
                DHCPEnable.Visible = true;
            }
        }
        public void EnableDNS()
        {
            if (DNSEnable.Checked)
            {
                DNS1Address.Enabled = true;
                DNS2Address.Enabled = true;
                m_bDNS = 1;
            }
            else
            {
                m_bDNS = 0;
            }
        }
        private void DHCPEnable_CheckedChanged_1(object sender, EventArgs e)
        {
            if (DHCPEnable.Checked)
            {
                IPAddress.Enabled = false;
                SubNetMask.Enabled = false;
                GateWay.Enabled = false;
                m_CheckDHCP = 1;
            }
            else
            {
                IPAddress.Enabled = true;
                SubNetMask.Enabled = true;
                GateWay.Enabled = true;
                m_CheckDHCP = 0;
            }
        }

        private void DNSEnable_CheckedChanged(object sender, EventArgs e)
        {
            EnableDNS();
        }
    }
}
