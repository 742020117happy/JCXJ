using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Configuration;
using System.Runtime.InteropServices;
using Configuration.Language;

namespace Configuration.ProductAcs
{
    public partial class DoorConfig : Form
    {
        public int m_lUserId = -1;
        public DoorConfig()
        {
            InitializeComponent();
            MultiLanguage.LoadLanguage(this);
        }
        private void btnUploadSet_Click(object sender, EventArgs e)
        {
            string StressPass = string.Empty;
            string UnlockPass = string.Empty;
            string byRes2 = string.Empty;
            string byStressPassword = string.Empty;
            string byUnlockPassword = string.Empty;
            CHCNetSDK.NET_DVR_DOOR_CFG struDoorCfg = new CHCNetSDK.NET_DVR_DOOR_CFG();
            struDoorCfg.dwSize = (uint)Marshal.SizeOf(struDoorCfg);
            string doorName = DoorName.Text;
            struDoorCfg.byDoorName = System.Text.Encoding.Default.GetBytes(doorName.Trim().PadRight(32, '\0').ToCharArray());
            struDoorCfg.byMagneticType = Convert.ToByte(DoorContactCom.SelectedIndex);
            struDoorCfg.byOpenButtonType = Convert.ToByte(SubChannelCom.SelectedIndex);
            struDoorCfg.byEnableDoorLock = 0;
            struDoorCfg.byEnableLeaderCard = 0;
            struDoorCfg.byLeaderCardMode = 0;
            struDoorCfg.byOpenDuration = Convert.ToByte(OpenDuration.Text);
            struDoorCfg.byDisabledOpenDuration = 15;
            struDoorCfg.dwLeaderCardOpenDuration = 10;
            struDoorCfg.byMagneticAlarmTimeout = Convert.ToByte(DoorLeftTime.Text);
            struDoorCfg.byStressPassword = System.Text.Encoding.Default.GetBytes(byStressPassword.Trim().PadRight(8, '\0').ToCharArray());
            struDoorCfg.bySuperPassword = System.Text.Encoding.Default.GetBytes(SuperPasswd.Text.Trim().PadRight(8, '\0').ToCharArray());
            struDoorCfg.byUnlockPassword = System.Text.Encoding.Default.GetBytes(byUnlockPassword.Trim().PadRight(8, '\0').ToCharArray());
            struDoorCfg.bySuperPassword = System.Text.Encoding.Default.GetBytes(StressPass.PadRight(8, '\0').ToCharArray());
            struDoorCfg.byUnlockPassword = System.Text.Encoding.Default.GetBytes(UnlockPass.PadRight(8, '\0').ToCharArray());
            struDoorCfg.byRes2 = System.Text.Encoding.Default.GetBytes(byRes2.PadRight(43, '\0').ToCharArray());

            struDoorCfg.byUseLocalController = 0;
            struDoorCfg.wLocalControllerID = 0;
            struDoorCfg.wLocalControllerDoorNumber = 0;
            struDoorCfg.wLocalControllerStatus = 0;
            struDoorCfg.byLockInputCheck = 0;
            struDoorCfg.byLockInputType = 0;
            struDoorCfg.byDoorTerminalMode = 0;
            struDoorCfg.byOpenButton = 0;
            struDoorCfg.byLadderControlDelayTime = 0;
            IntPtr struDoorPtr = Marshal.AllocHGlobal(Marshal.SizeOf(struDoorCfg));
            Marshal.StructureToPtr(struDoorCfg, struDoorPtr, true);
            if (!CHCNetSDK.NET_DVR_SetDVRConfig(m_lUserId, CHCNetSDK.NET_DVR_SET_DOOR_CFG, 1, struDoorPtr, (uint)Marshal.SizeOf(struDoorCfg)))
            {
                MessageBox.Show("NET_DVR_SET_DOOR_CFG, Error Code: " + CHCNetSDK.NET_DVR_GetLastError());
            }
            else
            {
                MessageBox.Show("NET_DVR_SET_DOOR_CFG Success");
            }
            Marshal.FreeHGlobal(struDoorPtr);
        }
    }
}
