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
using System.IO;

namespace DeleteFace
{
    public partial class DeleteFace : Form
    {
        public static int m_UserID = -1;
        public int m_lDelFingerPrintCfHandle = -1;
        public DeleteFace()
        {
            InitializeComponent();
            if (CHCNetSDK.NET_DVR_Init() == false)
            {
                MessageBox.Show("NET_DVR_Init error!");
                return;
            }
            CHCNetSDK.NET_DVR_SetLogToFile(3, "./SdkLog/", true);
            comboBoxdDel.SelectedIndex = 0;
            CreateCheckListBox();
        }

        private void btnLogin_Click(object sender, EventArgs e)
        {
            AddDevice dlg = new AddDevice();
            dlg.ShowDialog();
            dlg.Dispose();
        }

        private void btnDel_Click(object sender, EventArgs e)
        {
            if (m_lDelFingerPrintCfHandle != -1)
            {
                CHCNetSDK.NET_DVR_StopRemoteConfig(m_lDelFingerPrintCfHandle);
            }
            if (comboBoxdDel.SelectedIndex == 0)
            {
                //按卡号方式删除
                CHCNetSDK.NET_DVR_FACE_PARAM_CTRL_ByCard m_struDelByCard = new CHCNetSDK.NET_DVR_FACE_PARAM_CTRL_ByCard();
                m_struDelByCard.Init();
                m_struDelByCard.dwSize = Marshal.SizeOf(m_struDelByCard);
                m_struDelByCard.byMode = (byte)comboBoxdDel.SelectedIndex;

                byte[] byCardNo = System.Text.Encoding.UTF8.GetBytes(textBoxCardNo.Text);
                for (int i = 0; i < byCardNo.Length; i++)
                {
                    m_struDelByCard.struProcessMode.byCardNo[i] = byCardNo[i];
                }
                for (int i = 0; i < CHCNetSDK.MAX_FACE_NUM; ++i)
                {
                    m_struDelByCard.struProcessMode.byFaceID[i] = 1;
                    //之前的算法需要两张人脸（一张带眼镜，一张不带眼镜）所以有了这个数组(现在只需要一张)，删除时把这个数组都赋值为1。
                }
                for (int i = 0; i < 512; i++)
                {
                    if (checkedListBoxCardReader.GetItemChecked(i))
                    {
                        m_struDelByCard.struProcessMode.byEnableCardReader[i] = 1;
                    }
                }
                IntPtr ptrstruDel = Marshal.AllocHGlobal(m_struDelByCard.dwSize);
                Marshal.StructureToPtr(m_struDelByCard, ptrstruDel, false);
                if (CHCNetSDK.NET_DVR_RemoteControl(m_UserID, CHCNetSDK.NET_DVR_DEL_FACE_PARAM_CFG, ptrstruDel, m_struDelByCard.dwSize))
                {
                    MessageBox.Show("NET_SDK_DEL_FACE_Success");
                }
                else
                {
                    MessageBox.Show("NET_SDK_DEL_FACE_FAILED error:" + CHCNetSDK.NET_DVR_GetLastError());
                }
                Marshal.FreeHGlobal(ptrstruDel);
            }
            else 
            {
                //按读卡器方式删除
                CHCNetSDK.NET_DVR_FACE_PARAM_CTRL_ByReader m_struDelByReader = new CHCNetSDK.NET_DVR_FACE_PARAM_CTRL_ByReader();
                m_struDelByReader.Init();
                m_struDelByReader.dwSize = Marshal.SizeOf(m_struDelByReader);
                m_struDelByReader.byMode = (byte)comboBoxdDel.SelectedIndex;

                byte[] byCardNo = System.Text.Encoding.UTF8.GetBytes(textBoxCardNo.Text);
                for (int i = 0; i < byCardNo.Length; i++)
                {
                    m_struDelByReader.struProcessMode.byCardNo[i] = byCardNo[i];
                }
                int.TryParse(textBoxReaderNo.Text, out m_struDelByReader.struProcessMode.dwCardReaderNo);
                if (checkBoxDelAll.Checked)
                {
                    m_struDelByReader.struProcessMode.byClearAllCard = 1;
                }
                else
                {
                    m_struDelByReader.struProcessMode.byClearAllCard = 0;
                }
                IntPtr ptrstruDel = Marshal.AllocHGlobal(m_struDelByReader.dwSize);
                Marshal.StructureToPtr(m_struDelByReader, ptrstruDel, false);
                if (CHCNetSDK.NET_DVR_RemoteControl(m_UserID, CHCNetSDK.NET_DVR_DEL_FACE_PARAM_CFG, ptrstruDel, m_struDelByReader.dwSize))
                {
                    MessageBox.Show("NET_SDK_DEL_FACE_Success");
                }
                else
                {
                    MessageBox.Show("NET_SDK_DEL_FACE_FAILED error:" + CHCNetSDK.NET_DVR_GetLastError());
                }
                Marshal.FreeHGlobal(ptrstruDel);
            }
        }

        private void CreateCheckListBox()
        {
            string strTemp = null;
            for (int i = 0; i < 512; i++)
            {
                strTemp = string.Format("CardReader {0}", i + 1);
                checkedListBoxCardReader.Items.Add(strTemp);
            }
            checkedListBoxCardReader.SetItemChecked(0, true);
        }

        private void comboBoxdDel_SelectedIndexChanged(object sender, EventArgs e)
        {
            if(comboBoxdDel.SelectedIndex == 0)
            {
                checkBoxDelAll.Visible = false;
                label3.Visible = false;
                textBoxReaderNo.Visible = false;
                checkedListBoxCardReader.Visible = true;
            }
            else if (comboBoxdDel.SelectedIndex == 1)
            {
                checkBoxDelAll.Visible = true;
                label3.Visible = true;
                textBoxReaderNo.Visible = true;
                checkedListBoxCardReader.Visible = false;
            }
        }

        private void DeleteFace_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (m_UserID >= 0)
            {
                CHCNetSDK.NET_DVR_Logout_V30(m_UserID);
                m_UserID = -1;
            }
            CHCNetSDK.NET_DVR_Cleanup();
        }
    }
}
