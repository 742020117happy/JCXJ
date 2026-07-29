using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using PlanTemplate.ProductAcs;
using System.Runtime.InteropServices;
using System.IO;
using System.Threading;
using System.Globalization;

using PlanTemplate.Language;

namespace PlanTemplate
{
    public partial class PlanTemplate : Form
    {
        public static int m_UserID = -1;

        public PlanTemplate()
        {
            InitializeComponent();
            if (CHCNetSDK.NET_DVR_Init() == false)
            {
                MessageBox.Show("NET_DVR_Init error!");
                return;
            }
            comboBoxLanguage.SelectedIndex = 0;
        }

        private void btnLogin_Click(object sender, EventArgs e)
        {
            AddDevice addDevice = new AddDevice();
            addDevice.ShowDialog();
            addDevice.Dispose();
        }

        private void btnWeekPlan_Click(object sender, EventArgs e)
        {
            WeekPlan dlg = new WeekPlan();
            dlg.ShowDialog();
            dlg.Dispose();
        }

        private void btnHolidayPlan_Click(object sender, EventArgs e)
        {
            HolidayPlan dlg = new HolidayPlan();
            dlg.ShowDialog();
            dlg.Dispose();
        }

        private void btnHolidayGroup_Click(object sender, EventArgs e)
        {
            HolidayGroupPlan dlg = new HolidayGroupPlan();
            dlg.ShowDialog();
            dlg.Dispose();
        }

        private void btnPlanTemplate_Click(object sender, EventArgs e)
        {
            PlanTemplateM dlg = new PlanTemplateM();
            dlg.ShowDialog();
            dlg.Dispose();
        }

        private void PlanTemplate_FormClosing(object sender, FormClosingEventArgs e)
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
