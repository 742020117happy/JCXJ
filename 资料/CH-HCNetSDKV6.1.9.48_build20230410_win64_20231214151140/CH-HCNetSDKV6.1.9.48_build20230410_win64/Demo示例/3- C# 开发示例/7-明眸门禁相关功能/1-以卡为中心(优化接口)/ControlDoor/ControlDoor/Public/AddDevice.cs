using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using ControlDoor.Language;

namespace ControlDoor
{
    public partial class AddDevice : Form
    {
        public AddDevice()
        {
            InitializeComponent();
        }

        private void btnAdd_Click(object sender, EventArgs e)
        {
            if (textBoxDeviceAddress.Text.Length <= 0 || textBoxDeviceAddress.Text.Length >128)
            {
                MessageBox.Show(Properties.Resources.deviceAddressTips);
                return;
            }

            int port;
            int.TryParse(textBoxPort.Text, out port);
            if (textBoxPort.Text.Length > 5 || port <= 0)
            {
                MessageBox.Show(Properties.Resources.portTips);
                return;
            }

            if (textBoxUserName.Text.Length > 32 || textBoxPassword.Text.Length > 16)
            {
                MessageBox.Show(Properties.Resources.usernameAndPasswordTips);
                return;
            }

            Login();
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        public void Login()
        {
            if (ControlDoor.m_UserID >= 0)
            {
                CHCNetSDK.NET_DVR_Logout_V30(ControlDoor.m_UserID);
                ControlDoor.m_UserID = -1;
            }
            CHCNetSDK.NET_DVR_USER_LOGIN_INFO struLoginInfo = new CHCNetSDK.NET_DVR_USER_LOGIN_INFO();
            CHCNetSDK.NET_DVR_DEVICEINFO_V40 struDeviceInfoV40 = new CHCNetSDK.NET_DVR_DEVICEINFO_V40();
            struDeviceInfoV40.struDeviceV30.sSerialNumber = new byte[CHCNetSDK.SERIALNO_LEN];

            //设备IP地址或者域名
            byte[] byIP = System.Text.Encoding.Default.GetBytes(textBoxDeviceAddress.Text);
            struLoginInfo.sDeviceAddress = new byte[129];
            byIP.CopyTo(struLoginInfo.sDeviceAddress, 0);

            //设备用户名
            byte[] byUserName = System.Text.Encoding.Default.GetBytes(textBoxUserName.Text);
            struLoginInfo.sUserName = new byte[64];
            byUserName.CopyTo(struLoginInfo.sUserName, 0);

            //设备密码
            byte[] byPassword = System.Text.Encoding.Default.GetBytes(textBoxPassword.Text);
            struLoginInfo.sPassword = new byte[64];
            byPassword.CopyTo(struLoginInfo.sPassword, 0);
            ushort.TryParse(textBoxPort.Text, out struLoginInfo.wPort);

            int lUserID = -1;
            lUserID = CHCNetSDK.NET_DVR_Login_V40(ref struLoginInfo, ref struDeviceInfoV40);
            if (lUserID >= 0)
            {
                ControlDoor.m_UserID = lUserID;
                MessageBox.Show("Login Successful");
                this.Close();
            }
            else
            {
                uint nErr = CHCNetSDK.NET_DVR_GetLastError();
                if (nErr == CHCNetSDK.NET_DVR_PASSWORD_ERROR)
                {
                    MessageBox.Show("user name or password error!");
                    if (1 == struDeviceInfoV40.bySupportLock)
                    {
                        string strTemp1 = string.Format("Left {0} try opportunity", struDeviceInfoV40.byRetryLoginTime);
                        MessageBox.Show(strTemp1);
                    }
                }
                else if (nErr == CHCNetSDK.NET_DVR_USER_LOCKED)
                {
                    if (1 == struDeviceInfoV40.bySupportLock)
                    {
                        string strTemp1 = string.Format("user is locked, the remaining lock time is {0}", struDeviceInfoV40.dwSurplusLockTime);
                        MessageBox.Show(strTemp1);
                    }
                }
                else
                {
                    MessageBox.Show("net error or dvr is busy!");
                }
            }
        }

        private void AddDevice_Load(object sender, EventArgs e)
        {
            MultiLanguage.LoadLanguage(this);
        }
    }
}
