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
    public partial class TimeConfig : Form
    {
        public int m_lUserId = -1;
        public TimeConfig()
        {
            InitializeComponent();
            MultiLanguage.LoadLanguage(this);
        }
        private void btnSet_Click(object sender, EventArgs e)
        {
            CHCNetSDK.NET_DVR_TIME CurTime = new CHCNetSDK.NET_DVR_TIME();
            CurTime.dwYear = DeviceDateTimePicker.Value.Year;
            CurTime.dwMonth = DeviceDateTimePicker.Value.Month;
            CurTime.dwDay = DeviceDateTimePicker.Value.Day;
            CurTime.dwHour = int.Parse(HOUR.Text);
            CurTime.dwMinute = int.Parse(MINIUTE.Text);
            CurTime.dwSecond = int.Parse(SECONDS.Text);
            IntPtr DeviceTimePtr = Marshal.AllocHGlobal(Marshal.SizeOf(CurTime));
            Marshal.StructureToPtr(CurTime, DeviceTimePtr, true);
            if (!CHCNetSDK.NET_DVR_SetDVRConfig(m_lUserId, CHCNetSDK.NET_DVR_SET_TIMECFG, 0, DeviceTimePtr, (uint)Marshal.SizeOf(CurTime)))
            {
                MessageBox.Show("Check Time Fialed: " + CHCNetSDK.NET_DVR_GetLastError());
            }
            else
            {
                MessageBox.Show("Check Time Success!");
            }
            Marshal.FreeHGlobal(DeviceTimePtr);
            return;
        }
    }
}
