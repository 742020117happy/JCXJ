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
using Configuration.ProductAcs;

namespace Configuration
{
    public partial class Configuration : Form
    {
        public static int m_UserID = -1;

        public Configuration()
        {
            InitializeComponent();
            if (CHCNetSDK.NET_DVR_Init() == false)
            {
                MessageBox.Show("NET_DVR_Init error!");
                return;
            }
            comboBoxLanguage.SelectedIndex = 0;
            MultiLanguage.SetDefaultLanguage(comboBoxLanguage.Text);
            foreach (Form form in Application.OpenForms)
            {
                MultiLanguage.LoadLanguage(form);
            }
        }


        private void btnLogin_Click(object sender, EventArgs e)
        {
            AddDevice addDevice = new AddDevice();
            addDevice.ShowDialog();
            addDevice.Dispose();
        }

        private void btnRemoteCheck_Click(object sender, EventArgs e)
        {
            RemoteCheck dlg = new RemoteCheck();
            if(false == dlg.GetRemoteCfg())
            {
                return;
            }
            dlg.ShowDialog();
            dlg.Dispose();
        }

        private void Configuration_FormClosing(object sender, FormClosingEventArgs e)
        {
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
        private void BtnIsupConfig_Click(object sender, EventArgs e)
        {
            IsupAddrConfigure DlgIsupAddr = new IsupAddrConfigure();
            DlgIsupAddr.m_UserId = m_UserID;
            


            
            DlgIsupAddr.ShowDialog();
            DlgIsupAddr.Dispose();
        }
        //Wlan Config
        private void BtnWlanConfig_Click(object sender, EventArgs e)
        {
            WlanConfig.m_UserID = m_UserID;
            WlanConfig DlgWlanConfig = new WlanConfig();
            DlgWlanConfig.ShowDialog();
            //DlgWlanConfig.m_UserID = m_UserID;
            DlgWlanConfig.Dispose();

        }
        //Upload Config
        private void BtnUploadConfig_Click(object sender, EventArgs e)
        {
            UploadModeConfig DlgUploadConfig = new UploadModeConfig();
            DlgUploadConfig.m_lUserId = m_UserID;
            DlgUploadConfig.ShowDialog();
            DlgUploadConfig.Dispose();
        }
        //Door Config
        private void BtnDoorConfig_Click(object sender, EventArgs e)
        {
            DoorConfig DlgDoorConfig = new DoorConfig();
            DlgDoorConfig.m_lUserId = m_UserID;
            DlgDoorConfig.ShowDialog();
            DlgDoorConfig.Dispose();
        }

        private void BtnTimeConfig_Click(object sender, EventArgs e)
        {
            TimeConfig DlgTimeConfig = new TimeConfig();
            DlgTimeConfig.m_lUserId = m_UserID;
            DlgTimeConfig.ShowDialog();
            DlgTimeConfig.Dispose();
        }
        //Exit
        private void BtnExit_Click(object sender, EventArgs e)
        {
            if (CHCNetSDK.NET_DVR_Logout_V30(m_UserID))
            {
                MessageBox.Show("LoginOut Success!");
            }
        }
    }
}
