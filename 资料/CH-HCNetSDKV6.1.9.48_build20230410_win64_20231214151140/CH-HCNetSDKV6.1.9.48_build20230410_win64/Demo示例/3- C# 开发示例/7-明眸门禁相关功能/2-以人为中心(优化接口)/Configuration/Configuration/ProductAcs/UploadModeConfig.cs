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
    public partial class UploadModeConfig : Form
    {
        public int m_lUserId = -1;
        public int m_CenterNum = 0;
        public byte UploadEnableValue = 0;
        public UploadModeConfig()
        {
            InitializeComponent();
            MultiLanguage.LoadLanguage(this);
            CHCNetSDK.NET_DVR_ALARMHOST_ABILITY struAbility = new CHCNetSDK.NET_DVR_ALARMHOST_ABILITY();
            struAbility.dwSize = (uint)Marshal.SizeOf(struAbility);

            IntPtr InBuffer = IntPtr.Zero;
            IntPtr struAbilityCfg = Marshal.AllocHGlobal(Marshal.SizeOf(struAbility));
            Marshal.StructureToPtr(struAbility, struAbilityCfg, true);

            if (!CHCNetSDK.NET_DVR_GetDeviceAbility(m_lUserId, CHCNetSDK.ALARMHOST_ABILITY, InBuffer, 0, struAbilityCfg, struAbility.dwSize))
            {
                m_CenterNum = 2;
            }
            else
            {
                MessageBox.Show("Get DeviceAbility Success");
                m_CenterNum = struAbility.byCenterGroupNum;
            }
            int i = 0;
            GroupCenterCom.Items.Clear();
            for (i = 0; i < m_CenterNum; i++)
            {
                string csStr = string.Format("Center{0}", i + 1);
                GroupCenterCom.Items.Add(csStr);
            }
        }
        private void btnUploadSet_Click(object sender, EventArgs e)
        {
            uint dwCenterIndex = (uint)GroupCenterCom.SelectedIndex;
            CHCNetSDK.NET_DVR_ALARMHOST_REPORT_CENTER_CFG_V40 struReportMode = new
                CHCNetSDK.NET_DVR_ALARMHOST_REPORT_CENTER_CFG_V40();
            if (dwCenterIndex > 8)
            {
                MessageBox.Show("Please Select right Group Center Value");
                return;
            }
            //Inital Member byte Array
            string byRes = string.Empty;
            string byDealFailCenter = string.Empty;
            string ZoneReport = string.Empty;
            string NoneZoneReport = string.Empty;
            string byAlarmNetCard = string.Empty;
            string byRes2 = string.Empty;
            string byChanAlarmMode = string.Empty;

            struReportMode.byRes = System.Text.Encoding.Default.GetBytes(byRes.PadRight(2, '\0').ToCharArray());
            struReportMode.byChanAlarmMode = System.Text.Encoding.Default.GetBytes(byChanAlarmMode.PadRight(4, '\0').ToCharArray());
            struReportMode.byDealFailCenter = System.Text.Encoding.Default.GetBytes(byDealFailCenter.PadRight(16, '\0').ToCharArray());
            struReportMode.byZoneReport = System.Text.Encoding.Default.GetBytes(ZoneReport.PadRight(512, '\0').ToCharArray());
            struReportMode.byNonZoneReport = System.Text.Encoding.Default.GetBytes(NoneZoneReport.PadRight(32, '\0').ToCharArray());
            struReportMode.byAlarmNetCard = System.Text.Encoding.Default.GetBytes(byAlarmNetCard.PadRight(4, '\0').ToCharArray());
            struReportMode.byRes2 = System.Text.Encoding.Default.GetBytes(byRes2.PadRight(252, '\0').ToCharArray());

            struReportMode.dwSize = (uint)Marshal.SizeOf(struReportMode);
            struReportMode.byDataType = 0;
            struReportMode.byValid = UploadEnableValue;
            struReportMode.byDealFailCenter[0] = 0;
            struReportMode.byDealFailCenter[1] = 0;
            struReportMode.byDealFailCenter[2] = 0;
            struReportMode.byDealFailCenter[3] = 0;
            struReportMode.byDealFailCenter[4] = 0;
            struReportMode.byDealFailCenter[5] = 0;
            for (int i = 0; i < 7; i++)
            {
                struReportMode.byNonZoneReport[i] = 0;
            }

            CHCNetSDK.NET_DVR_ALARMHOST_REPORT_CENTER_CFG_V40[] m_pStruReportCenter = new
                CHCNetSDK.NET_DVR_ALARMHOST_REPORT_CENTER_CFG_V40[7];
            for (int i = 0; i < 7; i++)
            {
                m_pStruReportCenter[i].Init();
            }
            for (int i = 0; i < m_CenterNum; i++)
            {
                m_pStruReportCenter[i].dwSize = (uint)Marshal.SizeOf(struReportMode);
            }
            IntPtr strReportModeCfg = Marshal.AllocHGlobal(Marshal.SizeOf(struReportMode));
            Marshal.StructureToPtr(struReportMode, strReportModeCfg, true);
            m_pStruReportCenter[dwCenterIndex] = (CHCNetSDK.NET_DVR_ALARMHOST_REPORT_CENTER_CFG_V40)Marshal.PtrToStructure(strReportModeCfg,
                typeof(CHCNetSDK.NET_DVR_ALARMHOST_REPORT_CENTER_CFG_V40));
            Marshal.FreeHGlobal(strReportModeCfg);
            int iCenterIndex = 0;
            for (; iCenterIndex < m_CenterNum; iCenterIndex++)
            {
                if (iCenterIndex == 0)
                {
                    m_pStruReportCenter[iCenterIndex].byChanAlarmMode[0] = Convert.ToByte(MainChannelCom.SelectedIndex);
                    m_pStruReportCenter[iCenterIndex].byChanAlarmMode[1] = 0;
                    m_pStruReportCenter[iCenterIndex].byChanAlarmMode[2] = 0;
                    m_pStruReportCenter[iCenterIndex].byChanAlarmMode[3] = 0;
                    m_pStruReportCenter[iCenterIndex].byAlarmNetCard[0] = 0;
                    m_pStruReportCenter[iCenterIndex].byAlarmNetCard[1] = 0;
                    m_pStruReportCenter[iCenterIndex].byAlarmNetCard[2] = 0;
                    m_pStruReportCenter[iCenterIndex].byAlarmNetCard[3] = 0;
                }
                else
                {
                    for (int i = 0; i < 4; i++)
                    {
                        m_pStruReportCenter[iCenterIndex].byChanAlarmMode[i] = 0;
                        m_pStruReportCenter[iCenterIndex].byAlarmNetCard[i] = 0;
                    }
                }
            }
            CHCNetSDK.NET_DVR_INDEX NetIndex = new CHCNetSDK.NET_DVR_INDEX();
            NetIndex.Init();
            IntPtr NetIndexCfg = Marshal.AllocHGlobal(Marshal.SizeOf(NetIndex));
            Marshal.StructureToPtr(NetIndex, NetIndexCfg, true);
            uint InBufferSize = (uint)Marshal.SizeOf(NetIndex);
            CHCNetSDK.NET_DVR_INBUFF InBuff = new CHCNetSDK.NET_DVR_INBUFF();
            InBuff.Init();
            IntPtr StatusPtr = Marshal.AllocHGlobal(Marshal.SizeOf(InBuff));
            Marshal.StructureToPtr(InBuff, StatusPtr, true);
            int ReportCenterSize = Marshal.SizeOf(m_pStruReportCenter[0]);
            IntPtr ReportCenterCfg = Marshal.AllocHGlobal(ReportCenterSize);
            Marshal.StructureToPtr(m_pStruReportCenter[0], ReportCenterCfg, true);
            int ParamSize = m_CenterNum * Marshal.SizeOf(m_pStruReportCenter[0]);
            if (!CHCNetSDK.NET_DVR_SetDeviceConfig(m_lUserId, CHCNetSDK.NET_DVR_SET_ALARMHOST_REPORT_CENTER_V40,
                1, NetIndexCfg, InBufferSize, StatusPtr, ReportCenterCfg, (uint)ParamSize))
            {
                MessageBox.Show("Set UpLoad Mode Failed, " + CHCNetSDK.NET_DVR_GetLastError());
            }
            else
            {
                MessageBox.Show("Set UpLoad Mode Success! ");
            }
            Marshal.FreeHGlobal(ReportCenterCfg);
            Marshal.FreeHGlobal(StatusPtr);
            Marshal.FreeHGlobal(NetIndexCfg);
            return;
        }
        private void UploadEnable_CheckedChanged(object sender, EventArgs e)
        {
            if (UploadEnable.Checked)
            {
                UploadEnableValue = 1;
            }
        }
    }
}
