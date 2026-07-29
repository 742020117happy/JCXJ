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

using ControlDoor.Language;

namespace ControlDoor
{
    public partial class ControlDoor : Form
    {
        public static int m_UserID = -1;

        public ControlDoor()
        {
            InitializeComponent();
            if (CHCNetSDK.NET_DVR_Init() == false)
            {
                MessageBox.Show("NET_DVR_Init error!");
                return;
            }
            comboBoxLanguage.SelectedIndex = 0;
        }
        
        //0-close, 1-open, 2-stay open, 3-stay close
        private void btnLogin_Click(object sender, EventArgs e)
        {
            AddDevice dlg = new AddDevice();
            dlg.ShowDialog();
            dlg.Dispose();
        }

        private void btnOpen_Click(object sender, EventArgs e)
        {
            if (CHCNetSDK.NET_DVR_ControlGateway(m_UserID, 1, 1))
            {
                MessageBox.Show("NET_DVR_ControlGateway: open door succeed");
            }
            else
            {
                MessageBox.Show("NET_DVR_ControlGateway: open door failed error:" + CHCNetSDK.NET_DVR_GetLastError());
            }
        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            if (CHCNetSDK.NET_DVR_ControlGateway(m_UserID, 1, 0))
            {
                MessageBox.Show("NET_DVR_ControlGateway: close door succeed");
            }
            else
            {
                MessageBox.Show("NET_DVR_ControlGateway: close door failed error:" + CHCNetSDK.NET_DVR_GetLastError());
            }
        }

        private void btnStayOpen_Click(object sender, EventArgs e)
        {
            if (CHCNetSDK.NET_DVR_ControlGateway(m_UserID, 1, 3))
            {
                MessageBox.Show("NET_DVR_ControlGateway: stay close door succeed");
            }
            else
            {
                MessageBox.Show("NET_DVR_ControlGateway:  stay close door failed error:" + CHCNetSDK.NET_DVR_GetLastError());
            }
        }

        private void btnStayClose_Click(object sender, EventArgs e)
        {
            if (CHCNetSDK.NET_DVR_ControlGateway(m_UserID, 1, 2))
            {
                MessageBox.Show("NET_DVR_ControlGateway: stay open door succeed");
            }
            else
            {
                MessageBox.Show("NET_DVR_ControlGateway:  stay open door failed error:" + CHCNetSDK.NET_DVR_GetLastError());
            }
        }

        private void ControlDoor_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (m_UserID >= 0)
            {
                CHCNetSDK.NET_DVR_Logout_V30(m_UserID);
                m_UserID = -1;
            }
            CHCNetSDK.NET_DVR_Cleanup();
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
