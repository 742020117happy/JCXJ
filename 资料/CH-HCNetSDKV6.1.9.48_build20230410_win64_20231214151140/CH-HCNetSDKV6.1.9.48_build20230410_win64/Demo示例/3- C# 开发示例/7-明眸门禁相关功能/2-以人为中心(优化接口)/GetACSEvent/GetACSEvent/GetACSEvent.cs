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

using GetACSEvent.Language;

namespace GetACSEvent
{
    public partial class GetACSEvent : Form
    {
        public int m_UserID = -1;
        private string CsTemp = null;
        private int m_lLogNum = 0;
        private string MinorType = null;
        private string MajorType = null;
        public string IpAddr = string.Empty;
        public int m_lGetAcsEventHandle = -1;
        public GetACSEvent()
        {
            InitializeComponent();
            CHCNetSDK.NET_DVR_Init();
            CHCNetSDK.NET_DVR_SetLogToFile(3, "./SdkLog/", true);
            comboBoxLanguage.SelectedIndex = 0;
        }

        private void btnLogin_Click(object sender, EventArgs e)
        {
            AddDevice deviceAdd = new AddDevice();
            deviceAdd.ShowDialog();
            m_UserID = deviceAdd.m_iUserID;
            IpAddr = deviceAdd.m_DevIp;
            deviceAdd.Dispose();
        }

        //存储门禁事件对应的可见光图和热度图
        int SavePictureToLocal(ref CHCNetSDK.NET_DVR_ACS_EVENT_CFG AcsEventCfg, string PictureType)
        {
            int iPictureLength = 0;
            byte[] pPictureData = new byte[10];
            if (PictureType == "AcsEventPicture")
            {
                pPictureData = new byte[AcsEventCfg.dwPicDataLen];
                if ((AcsEventCfg.dwPicDataLen > 0) && (AcsEventCfg.pPicData != null))
                {
                    Marshal.Copy(AcsEventCfg.pPicData, pPictureData, 0, (int)AcsEventCfg.dwPicDataLen);
                }
                iPictureLength = (int)AcsEventCfg.dwPicDataLen;
                if (iPictureLength > 0 && pPictureData.Length > 10)
                {
                    string PicName = string.Empty;
                    string FilePath = "C:/Picture/" + IpAddr + "/";
                    if (!Directory.Exists(FilePath))
                    {
                        Directory.CreateDirectory(FilePath);
                    }
                    string StrCurTime = AcsEventCfg.struTime.dwYear + "_" + AcsEventCfg.struTime.dwMonth + "_" + AcsEventCfg.struTime.dwMonth + "_" +
                        AcsEventCfg.struTime.dwHour + "_" + AcsEventCfg.struTime.dwMinute + "_" + AcsEventCfg.struTime.dwSecond + "_";
                    FilePath += StrCurTime;
                    PicName = FilePath;
                    PicName += "Major[" + AcsEventCfg.dwMajor + "]_" + "Minor[" + AcsEventCfg.dwMinor + "]_AcsEvent.jpg";
                    try
                    {
                        using (FileStream fs = new FileStream(PicName, FileMode.OpenOrCreate, FileAccess.Read | FileAccess.Write))
                        {
                            byte[] DataStream = new byte[iPictureLength];
                            Marshal.Copy(AcsEventCfg.pPicData, DataStream, 0, iPictureLength);
                            fs.Write(DataStream, 0, iPictureLength);
                            fs.Flush();
                            fs.Close();
                        }
                    }
                    catch (Exception e)
                    {
                        MessageBox.Show("Store Picture Error");
                    }
                    return 1;
                }
            }
            else if (PictureType == "QRCodeInfo")
            {

                pPictureData = new byte[AcsEventCfg.dwQRCodeInfoLen];
                if ((AcsEventCfg.dwPicDataLen > 0) && (AcsEventCfg.pPicData != null))
                {
                    Marshal.Copy(AcsEventCfg.pQRCodeInfo, pPictureData, 0, (int)AcsEventCfg.dwQRCodeInfoLen);
                }
                iPictureLength = (int)AcsEventCfg.dwPicDataLen;
                if (iPictureLength > 0 && pPictureData.Length > 10)
                {
                    string PicName = string.Empty;
                    string FilePath = "C:/Picture/" + IpAddr + "/";
                    if (!Directory.Exists(FilePath))
                    {
                        Directory.CreateDirectory(FilePath);
                    }
                    string StrCurTime = AcsEventCfg.struTime.dwYear + "_" + AcsEventCfg.struTime.dwMonth + "_" + AcsEventCfg.struTime.dwMonth + "_" +
                        AcsEventCfg.struTime.dwHour + "_" + AcsEventCfg.struTime.dwMinute + "_" + AcsEventCfg.struTime.dwSecond + "_";
                    FilePath += StrCurTime;
                    PicName = FilePath;
                    PicName += "Major[" + AcsEventCfg.dwMajor + "]_" + "Minor[" + AcsEventCfg.dwMinor + "]_QRCodeInfo.jpg";
                    try
                    {
                        using (FileStream fs = new FileStream(PicName, FileMode.OpenOrCreate, FileAccess.Read | FileAccess.Write))
                        {
                            byte[] DataStream = new byte[iPictureLength];
                            Marshal.Copy(AcsEventCfg.pQRCodeInfo, DataStream, 0, iPictureLength);
                            fs.Write(DataStream, 0, iPictureLength);
                            fs.Flush();
                            fs.Close();
                        }
                    }
                    catch (Exception e)
                    {
                        MessageBox.Show("Store Picture Error");
                    }
                    return 1;
                }
            }
            else if (PictureType == "VisibileLightData")
            {
                pPictureData = new byte[AcsEventCfg.dwVisibleLightDataLen];
                if (AcsEventCfg.dwVisibleLightDataLen > 0 && AcsEventCfg.pVisibleLightData != null)
                {
                    Marshal.Copy(AcsEventCfg.pVisibleLightData, pPictureData, 0, (int)AcsEventCfg.dwVisibleLightDataLen);
                }
                iPictureLength = (int)AcsEventCfg.dwVisibleLightDataLen;
                if (iPictureLength > 0 && pPictureData.Length > 10)
                {
                    string PicName = string.Empty;
                    string FilePath = "C:/Picture/" + IpAddr + "/";
                    if (!Directory.Exists(FilePath))
                    {
                        Directory.CreateDirectory(FilePath);
                    }
                    string StrCurTime = AcsEventCfg.struTime.dwYear + "_" + AcsEventCfg.struTime.dwMonth + "_" + AcsEventCfg.struTime.dwMonth + "_" +
                        AcsEventCfg.struTime.dwHour + "_" + AcsEventCfg.struTime.dwMinute + "_" + AcsEventCfg.struTime.dwSecond + "_";
                    FilePath += StrCurTime;
                    PicName = FilePath;
                    PicName += "Major[" + AcsEventCfg.dwMajor + "]_" + "Minor[" + AcsEventCfg.dwMinor + "]_VisibileLight.jpg";
                    try
                    {
                        using (FileStream fs = new FileStream(PicName, FileMode.OpenOrCreate, FileAccess.Read | FileAccess.Write))
                        {
                            byte[] DataStream = new byte[iPictureLength];
                            Marshal.Copy(AcsEventCfg.pVisibleLightData, DataStream, 0, iPictureLength);
                            fs.Write(DataStream, 0, iPictureLength);
                            fs.Flush();
                            fs.Close();
                        }
                    }
                    catch (Exception e)
                    {
                        MessageBox.Show("Store Picture Error");
                    }
                    return 1;
                }
            }
            else if (PictureType == "ThermalData")
            {
                pPictureData = new byte[AcsEventCfg.dwThermalDataLen];
                if (AcsEventCfg.dwThermalDataLen > 0 && AcsEventCfg.pThermalData != null)
                {
                    Marshal.Copy(AcsEventCfg.pThermalData, pPictureData, 0, (int)AcsEventCfg.dwThermalDataLen);
                }
                iPictureLength = (int)AcsEventCfg.dwThermalDataLen;
                if (iPictureLength > 0 && pPictureData.Length > 10)
                {
                    string PicName = string.Empty;
                    string FilePath = "C:/Picture/" + IpAddr + "/";
                    if (!Directory.Exists(FilePath))
                    {
                        Directory.CreateDirectory(FilePath);
                    }
                    string StrCurTime = AcsEventCfg.struTime.dwYear + "_" + AcsEventCfg.struTime.dwMonth + "_" + AcsEventCfg.struTime.dwMonth + "_" +
                        AcsEventCfg.struTime.dwHour + "_" + AcsEventCfg.struTime.dwMinute + "_" + AcsEventCfg.struTime.dwSecond + "_";
                    FilePath += StrCurTime;
                    PicName = FilePath;
                    PicName += "Major[" + AcsEventCfg.dwMajor + "]_" + "Minor[" + AcsEventCfg.dwMinor + "]_Thermal.jpg";
                    try
                    {
                        using (FileStream fs = new FileStream(PicName, FileMode.OpenOrCreate, FileAccess.Read | FileAccess.Write))
                        {
                            byte[] DataStream = new byte[iPictureLength];
                            Marshal.Copy(AcsEventCfg.pThermalData, DataStream, 0, iPictureLength);
                            fs.Write(DataStream, 0, iPictureLength);
                            fs.Flush();
                            fs.Close();
                        }
                    }
                    catch (Exception e)
                    {
                        MessageBox.Show("Store Picture Error");
                    }
                    return 1;
                }
            }
            return 0;
        }


        private void btnSearch_Click(object sender, EventArgs e)
        {
            listViewEvent.Items.Clear();
            m_lLogNum = 0;
            CHCNetSDK.NET_DVR_ACS_EVENT_COND struCond = new CHCNetSDK.NET_DVR_ACS_EVENT_COND();
            struCond.Init();
            struCond.dwSize =(uint)Marshal.SizeOf(struCond);

            MajorType = comboBoxMainType.SelectedItem.ToString();
            struCond.dwMajor=GetAcsEventType.ReturnMajorTypeValue(ref MajorType);

            MinorType = comboBoxSecondType.SelectedItem.ToString();
            struCond.dwMinor =GetAcsEventType.ReturnMinorTypeValue(ref MinorType);


            struCond.struStartTime.dwYear = dateTimeStart.Value.Year;
            struCond.struStartTime.dwMonth = dateTimeStart.Value.Month;
            struCond.struStartTime.dwDay = dateTimeStart.Value.Day;
            struCond.struStartTime.dwHour = dateTimeStart.Value.Hour;
            struCond.struStartTime.dwMinute = dateTimeStart.Value.Minute;
            struCond.struStartTime.dwSecond = dateTimeStart.Value.Second;

            struCond.struEndTime.dwYear = dateTimeEnd.Value.Year;
            struCond.struEndTime.dwMonth = dateTimeEnd.Value.Month;
            struCond.struEndTime.dwDay = dateTimeEnd.Value.Day;
            struCond.struEndTime.dwHour = dateTimeEnd.Value.Hour;
            struCond.struEndTime.dwMinute = dateTimeEnd.Value.Minute;
            struCond.struEndTime.dwSecond = dateTimeEnd.Value.Second;

            //if (!StrToByteArray(ref struCond.byCardNo, textBoxCardNo.Text))
            //{
            //    return;
            //}

            //if (!StrToByteArray(ref struCond.byName, textBoxName.Text))
            //{
            //    return;
            //}
            //struCond.dwBeginSerialNo = 0;
            //struCond.dwEndSerialNo = 0;

            uint dwSize = struCond.dwSize;
            IntPtr ptrCond = Marshal.AllocHGlobal((int)dwSize);
            Marshal.StructureToPtr(struCond, ptrCond, false);
            m_lGetAcsEventHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_UserID, CHCNetSDK.NET_DVR_GET_ACS_EVENT, ptrCond, (int)dwSize, null, IntPtr.Zero);
            if (-1 == m_lGetAcsEventHandle)
            {
                Marshal.FreeHGlobal(ptrCond);
                MessageBox.Show("NET_DVR_StartRemoteConfig FAIL, ERROR CODE" + CHCNetSDK.NET_DVR_GetLastError().ToString(), "Error", MessageBoxButtons.OK);
                return;
            }
            int dwStatus = 0;
            Boolean Flag = true;
            CHCNetSDK.NET_DVR_ACS_EVENT_CFG struCFG=new CHCNetSDK.NET_DVR_ACS_EVENT_CFG();
            struCFG.dwSize=(uint)Marshal.SizeOf(struCFG);
            int dwOutBuffSize=(int)struCFG.dwSize;
            struCFG.init();
            while (Flag)
            {
                dwStatus = CHCNetSDK.NET_DVR_GetNextRemoteConfig(m_lGetAcsEventHandle, ref struCFG, dwOutBuffSize);
                switch (dwStatus)
                {
                    case CHCNetSDK.NET_SDK_GET_NEXT_STATUS_SUCCESS://成功读取到数据，处理完本次数据后需调用next
                        ProcessAcsEvent(ref struCFG, ref Flag);
                        break;
                    case CHCNetSDK.NET_SDK_GET_NEXT_STATUS_NEED_WAIT:
                        Thread.Sleep(200);
                        break;
                    case CHCNetSDK.NET_SDK_GET_NEXT_STATUS_FAILED:
                        CHCNetSDK.NET_DVR_StopRemoteConfig(m_lGetAcsEventHandle);
                        MessageBox.Show("NET_SDK_GET_NEXT_STATUS_FAILED" + CHCNetSDK.NET_DVR_GetLastError().ToString(), "Error", MessageBoxButtons.OK);
                        Flag = false;
                        break;
                    case CHCNetSDK.NET_SDK_GET_NEXT_STATUS_FINISH:
                        CHCNetSDK.NET_DVR_StopRemoteConfig(m_lGetAcsEventHandle);
                        Flag = false;
                        break;
                    default:
                        MessageBox.Show("NET_SDK_GET_NEXT_STATUS_UNKOWN" + CHCNetSDK.NET_DVR_GetLastError().ToString(), "Error", MessageBoxButtons.OK);
                        Flag = false;
                        CHCNetSDK.NET_DVR_StopRemoteConfig(m_lGetAcsEventHandle);
                        break;
                }
            }
            Marshal.FreeHGlobal(ptrCond);
        }
        private void ProcessAcsEvent(ref CHCNetSDK.NET_DVR_ACS_EVENT_CFG struCFG, ref bool flag)
        {
            try
            {
                AddAcsEventToList(ref struCFG);
                if (struCFG.dwPicDataLen != 0)
                {
                    byte[] pPicData = new byte[struCFG.dwPicDataLen];
                    if (struCFG.pPicData == null)
                    {
                        return;
                    }
                    //Save Picture To Local
                    SavePictureToLocal(ref struCFG, "AcsEventPicture");
                    //SavePictureToLocal(ref struCFG, "QRCodeInfo");
                    SavePictureToLocal(ref struCFG, "VisibileLightData");
                    SavePictureToLocal(ref struCFG, "ThermalData");
                }
            }
            catch
            {
                MessageBox.Show("AddAcsEventToList Failed", "Error", MessageBoxButtons.OK);
                flag = false;
            }
        }
        private Boolean StrToByteArray(ref byte[] destination, string data)
        {
            if (data != "")
            {
                byte[] source = System.Text.Encoding.Default.GetBytes(data);
                if (source.Length > destination.Length)
                {
                    MessageBox.Show("The length of num is exceeding");
                    return false;
                }
                else
                {
                    for (int i = 0; i < source.Length; ++i)
                    {
                        destination[i] = source[i];
                    }
                    return true;
                }
            }
            return true;
        }
 

        private string ProcessMajorType(ref uint dwMajor)
        {
            string res = null;
            switch (dwMajor)
            {
                case 1:
                    res = "Alarm";
                    break;
                case 2:
                    res = "Exception";
                    break;
                case 3:
                    res = "Operation";
                    break;
                case 5:
                    res = "Event";
                    break;
                default:
                    res = "Unknown";
                    break;
            }
            return res;
        }

        private void AlarmMinorTypeMap(ref CHCNetSDK.NET_DVR_ACS_EVENT_CFG struEventCfg)
        {
            switch (struEventCfg.dwMinor)
            {
                case CHCNetSDK.MINOR_ALARMIN_SHORT_CIRCUIT:
                    CsTemp = "ALARMIN_SHORT_CIRCUIT";
                    break;
                case CHCNetSDK.MINOR_ALARMIN_BROKEN_CIRCUIT:
                    CsTemp = "ALARMIN_BROKEN_CIRCUIT";
                    break;
                case CHCNetSDK.MINOR_ALARMIN_EXCEPTION:
                    CsTemp = "ALARMIN_EXCEPTION";
                    break;
                case CHCNetSDK.MINOR_ALARMIN_RESUME:
                    CsTemp = "ALARMIN_RESUME";
                    break;
                case CHCNetSDK.MINOR_HOST_DESMANTLE_ALARM:
                    CsTemp = "HOST_DESMANTLE_ALARM";
                    break;
                case CHCNetSDK.MINOR_HOST_DESMANTLE_RESUME:
                    CsTemp = "HOST_DESMANTLE_RESUME";
                    break;
                case CHCNetSDK.MINOR_CARD_READER_DESMANTLE_ALARM:
                    CsTemp = "CARD_READER_DESMANTLE_ALARM";
                    break;
                case CHCNetSDK.MINOR_CARD_READER_DESMANTLE_RESUME:
                    CsTemp = "CARD_READER_DESMANTLE_RESUME";
                    break;
                case CHCNetSDK.MINOR_CASE_SENSOR_ALARM:
                    CsTemp = "CASE_SENSOR_ALARM";
                    break;
                case CHCNetSDK.MINOR_CASE_SENSOR_RESUME:
                    CsTemp = "CASE_SENSOR_RESUME";
                    break;
                case CHCNetSDK.MINOR_STRESS_ALARM:
                    CsTemp = "STRESS_ALARM";
                    break;
                case CHCNetSDK.MINOR_OFFLINE_ECENT_NEARLY_FULL:
                    CsTemp = "OFFLINE_ECENT_NEARLY_FULL";
                    break;
                case CHCNetSDK.MINOR_CARD_MAX_AUTHENTICATE_FAIL:
                    CsTemp = "CARD_MAX_AUTHENTICATE_FAIL";
                    break;
                case CHCNetSDK.MINOR_SD_CARD_FULL:
                    CsTemp = "MINOR_SD_CARD_FULL";
                    break;
                case CHCNetSDK.MINOR_LINKAGE_CAPTURE_PIC:
                    CsTemp = "MINOR_LINKAGE_CAPTURE_PIC";
                    break;
                case CHCNetSDK.MINOR_SECURITY_MODULE_DESMANTLE_ALARM:
                    CsTemp = "MINOR_SECURITY_MODULE_DESMANTLE_ALARM";
                    break;
                case CHCNetSDK.MINOR_SECURITY_MODULE_DESMANTLE_RESUME:
                    CsTemp = "MINOR_SECURITY_MODULE_DESMANTLE_RESUME";
                    break;
                case CHCNetSDK.MINOR_POS_START_ALARM:
                    CsTemp = "MINOR_POS_START_ALARM";
                    break;
                case CHCNetSDK.MINOR_POS_END_ALARM:
                    CsTemp = "MINOR_POS_END_ALARM";
                    break;
                case CHCNetSDK.MINOR_FACE_IMAGE_QUALITY_LOW:
                    CsTemp = "MINOR_FACE_IMAGE_QUALITY_LOW";
                    break;
                case CHCNetSDK.MINOR_FINGE_RPRINT_QUALITY_LOW:
                    CsTemp = "MINOR_FINGE_RPRINT_QUALITY_LOW";
                    break;
                case CHCNetSDK.MINOR_FIRE_IMPORT_SHORT_CIRCUIT:
                    CsTemp = "MINOR_FIRE_IMPORT_SHORT_CIRCUIT";
                    break;
                case CHCNetSDK.MINOR_FIRE_IMPORT_BROKEN_CIRCUIT:
                    CsTemp = "MINOR_FIRE_IMPORT_BROKEN_CIRCUIT";
                    break;
                case CHCNetSDK.MINOR_FIRE_IMPORT_RESUME:
                    CsTemp = "MINOR_FIRE_IMPORT_RESUME";
                    break;
                case CHCNetSDK.MINOR_FIRE_BUTTON_TRIGGER:
                    CsTemp = "FIRE_BUTTON_TRIGGER";
                    break;
                case CHCNetSDK.MINOR_FIRE_BUTTON_RESUME:
                    CsTemp = "FIRE_BUTTON_RESUME";
                    break;
                case CHCNetSDK.MINOR_MAINTENANCE_BUTTON_TRIGGER:
                    CsTemp = "MAINTENANCE_BUTTON_TRIGGER";
                    break;
                case CHCNetSDK.MINOR_MAINTENANCE_BUTTON_RESUME:
                    CsTemp = "MAINTENANCE_BUTTON_RESUME";
                    break;
                case CHCNetSDK.MINOR_EMERGENCY_BUTTON_TRIGGER:
                    CsTemp = "EMERGENCY_BUTTON_TRIGGER";
                    break;
                case CHCNetSDK.MINOR_EMERGENCY_BUTTON_RESUME:
                    CsTemp = "EMERGENCY_BUTTON_RESUME";
                    break;
                case CHCNetSDK.MINOR_DISTRACT_CONTROLLER_ALARM:
                    CsTemp = "DISTRACT_CONTROLLER_ALARM";
                    break;
                case CHCNetSDK.MINOR_DISTRACT_CONTROLLER_RESUME:
                    CsTemp = "DISTRACT_CONTROLLER_RESUME";
                    break;
                case CHCNetSDK.MINOR_CHANNEL_CONTROLLER_DESMANTLE_ALARM:
                    CsTemp = "MINOR_CHANNEL_CONTROLLER_DESMANTLE_ALARM";
                    break;
                case CHCNetSDK.MINOR_CHANNEL_CONTROLLER_DESMANTLE_RESUME:
                    CsTemp = "MINOR_CHANNEL_CONTROLLER_DESMANTLE_RESUME";
                    break;
                case CHCNetSDK.MINOR_CHANNEL_CONTROLLER_FIRE_IMPORT_ALARM:
                    CsTemp = "MINOR_CHANNEL_CONTROLLER_FIRE_IMPORT_ALARM";
                    break;
                case CHCNetSDK.MINOR_CHANNEL_CONTROLLER_FIRE_IMPORT_RESUME:
                    CsTemp = "MINOR_CHANNEL_CONTROLLER_FIRE_IMPORT_RESUME";
                    break;
                case CHCNetSDK.MINOR_LEGAL_EVENT_NEARLY_FULL:
                    CsTemp = "MINOR_LEGAL_EVENT_NEARLY_FULL";
                    break;
                default:
                    CsTemp = Convert.ToString(struEventCfg.dwMinor, 16);
                    break;
            }
        }

        private void ExceptionMinorTypeMap(ref CHCNetSDK.NET_DVR_ACS_EVENT_CFG struEventCfg)
        {
            switch (struEventCfg.dwMinor)
            {
                case CHCNetSDK.MINOR_NET_BROKEN:
                    CsTemp = "NET_BROKEN";
                    break;
                case CHCNetSDK.MINOR_RS485_DEVICE_ABNORMAL:
                    CsTemp = "RS485_DEVICE_ABNORMAL";
                    break;
                case CHCNetSDK.MINOR_RS485_DEVICE_REVERT:
                    CsTemp = "RS485_DEVICE_REVERT";
                    break;
                case CHCNetSDK.MINOR_DEV_POWER_ON:
                    CsTemp = "DEV_POWER_ON";
                    break;
                case CHCNetSDK.MINOR_DEV_POWER_OFF:
                    CsTemp = "DEV_POWER_OFF";
                    break;
                case CHCNetSDK.MINOR_WATCH_DOG_RESET:
                    CsTemp = "WATCH_DOG_RESET";
                    break;
                case CHCNetSDK.MINOR_LOW_BATTERY:
                    CsTemp = "LOW_BATTERY";
                    break;
                case CHCNetSDK.MINOR_BATTERY_RESUME:
                    CsTemp = "BATTERY_RESUME";
                    break;
                case CHCNetSDK.MINOR_AC_OFF:
                    CsTemp = "AC_OFF";
                    break;
                case CHCNetSDK.MINOR_AC_RESUME:
                    CsTemp = "AC_RESUME";
                    break;
                case CHCNetSDK.MINOR_NET_RESUME:
                    CsTemp = "NET_RESUME";
                    break;
                case CHCNetSDK.MINOR_FLASH_ABNORMAL:
                    CsTemp = "FLASH_ABNORMAL";
                    break;
                case CHCNetSDK.MINOR_CARD_READER_OFFLINE:
                    CsTemp = "CARD_READER_OFFLINE";
                    break;
                case CHCNetSDK.MINOR_CARD_READER_RESUME:
                    CsTemp = "CARD_READER_RESUME";
                    break;
                case CHCNetSDK.MINOR_INDICATOR_LIGHT_OFF:
                    CsTemp = "INDICATOR_LIGHT_OFF";
                    break;
                case CHCNetSDK.MINOR_INDICATOR_LIGHT_RESUME:
                    CsTemp = "INDICATOR_LIGHT_RESUME";
                    break;
                case CHCNetSDK.MINOR_CHANNEL_CONTROLLER_OFF:
                    CsTemp = "CHANNEL_CONTROLLER_OFF";
                    break;
                case CHCNetSDK.MINOR_CHANNEL_CONTROLLER_RESUME:
                    CsTemp = "CHANNEL_CONTROLLER_RESUME";
                    break;
                case CHCNetSDK.MINOR_SECURITY_MODULE_OFF:
                    CsTemp = "SECURITY_MODULE_OFF";
                    break;
                case CHCNetSDK.MINOR_SECURITY_MODULE_RESUME:
                    CsTemp = "SECURITY_MODULE_RESUME";
                    break;
                case CHCNetSDK.MINOR_BATTERY_ELECTRIC_LOW:
                    CsTemp = "BATTERY_ELECTRIC_LOW";
                    break;
                case CHCNetSDK.MINOR_BATTERY_ELECTRIC_RESUME:
                    CsTemp = "BATTERY_ELECTRIC_RESUME";
                    break;
                case CHCNetSDK.MINOR_LOCAL_CONTROL_NET_BROKEN:
                    CsTemp = "LOCAL_CONTROL_NET_BROKEN";
                    break;
                case CHCNetSDK.MINOR_LOCAL_CONTROL_NET_RSUME:
                    CsTemp = "LOCAL_CONTROL_NET_RSUME";
                    break;
                case CHCNetSDK.MINOR_MASTER_RS485_LOOPNODE_BROKEN:
                    CsTemp = "MASTER_RS485_LOOPNODE_BROKEN";
                    break;
                case CHCNetSDK.MINOR_MASTER_RS485_LOOPNODE_RESUME:
                    CsTemp = "MASTER_RS485_LOOPNODE_RESUME";
                    break;
                case CHCNetSDK.MINOR_LOCAL_CONTROL_OFFLINE:
                    CsTemp = "LOCAL_CONTROL_OFFLINE";
                    break;
                case CHCNetSDK.MINOR_LOCAL_CONTROL_RESUME:
                    CsTemp = "LOCAL_CONTROL_RESUME";
                    break;
                case CHCNetSDK.MINOR_LOCAL_DOWNSIDE_RS485_LOOPNODE_BROKEN:
                    CsTemp = "LOCAL_DOWNSIDE_RS485_LOOPNODE_BROKEN";
                    break;
                case CHCNetSDK.MINOR_LOCAL_DOWNSIDE_RS485_LOOPNODE_RESUME:
                    CsTemp = "LOCAL_DOWNSIDE_RS485_LOOPNODE_RESUME";
                    break;
                case CHCNetSDK.MINOR_DISTRACT_CONTROLLER_ONLINE:
                    CsTemp = "DISTRACT_CONTROLLER_ONLINE";
                    break;
                case CHCNetSDK.MINOR_DISTRACT_CONTROLLER_OFFLINE:
                    CsTemp = "DISTRACT_CONTROLLER_OFFLINE";
                    break;
                case CHCNetSDK.MINOR_ID_CARD_READER_NOT_CONNECT:
                    CsTemp = "ID_CARD_READER_NOT_CONNECT";
                    break;
                case CHCNetSDK.MINOR_ID_CARD_READER_RESUME:
                    CsTemp = "ID_CARD_READER_RESUME";
                    break;
                case CHCNetSDK.MINOR_FINGER_PRINT_MODULE_NOT_CONNECT:
                    CsTemp = "FINGER_PRINT_MODULE_NOT_CONNECT";
                    break;
                case CHCNetSDK.MINOR_FINGER_PRINT_MODULE_RESUME:
                    CsTemp = "FINGER_PRINT_MODULE_RESUME";
                    break;
                case CHCNetSDK.MINOR_CAMERA_NOT_CONNECT:
                    CsTemp = "CAMERA_NOT_CONNECT";
                    break;
                case CHCNetSDK.MINOR_CAMERA_RESUME:
                    CsTemp = "CAMERA_RESUME";
                    break;
                case CHCNetSDK.MINOR_COM_NOT_CONNECT:
                    CsTemp = "COM_NOT_CONNECT";
                    break;
                case CHCNetSDK.MINOR_COM_RESUME:
                    CsTemp = "COM_RESUME";
                    break;
                case CHCNetSDK.MINOR_DEVICE_NOT_AUTHORIZE:
                    CsTemp = "DEVICE_NOT_AUTHORIZE";
                    break;
                case CHCNetSDK.MINOR_PEOPLE_AND_ID_CARD_DEVICE_ONLINE:
                    CsTemp = "PEOPLE_AND_ID_CARD_DEVICE_ONLINE";
                    break;
                case CHCNetSDK.MINOR_PEOPLE_AND_ID_CARD_DEVICE_OFFLINE:
                    CsTemp = "PEOPLE_AND_ID_CARD_DEVICE_OFFLINE";
                    break;
                case CHCNetSDK.MINOR_LOCAL_LOGIN_LOCK:
                    CsTemp = "LOCAL_LOGIN_LOCK";
                    break;
                case CHCNetSDK.MINOR_LOCAL_LOGIN_UNLOCK:
                    CsTemp = "LOCAL_LOGIN_UNLOCK";
                    break;
                case CHCNetSDK.MINOR_SUBMARINEBACK_COMM_BREAK:
                    CsTemp = "SUBMARINEBACK_COMM_BREAK";
                    break;
                case CHCNetSDK.MINOR_SUBMARINEBACK_COMM_RESUME:
                    CsTemp = "SUBMARINEBACK_COMM_RESUME";
                    break;
                case CHCNetSDK.MINOR_MOTOR_SENSOR_EXCEPTION:
                    CsTemp = "MOTOR_SENSOR_EXCEPTION";
                    break;
                case CHCNetSDK.MINOR_CAN_BUS_EXCEPTION:
                    CsTemp = "CAN_BUS_EXCEPTION";
                    break;
                case CHCNetSDK.MINOR_CAN_BUS_RESUME:
                    CsTemp = "CAN_BUS_RESUME";
                    break;
                case CHCNetSDK.MINOR_GATE_TEMPERATURE_OVERRUN:
                    CsTemp = "GATE_TEMPERATURE_OVERRUN";
                    break;
                case CHCNetSDK.MINOR_IR_EMITTER_EXCEPTION:
                    CsTemp = "IR_EMITTER_EXCEPTION";
                    break;
                case CHCNetSDK.MINOR_IR_EMITTER_RESUME:
                    CsTemp = "IR_EMITTER_RESUME";
                    break;
                case CHCNetSDK.MINOR_LAMP_BOARD_COMM_EXCEPTION:
                    CsTemp = "LAMP_BOARD_COMM_EXCEPTION";
                    break;
                case CHCNetSDK.MINOR_LAMP_BOARD_COMM_RESUME:
                    CsTemp = "LAMP_BOARD_COMM_RESUME";
                    break;
                case CHCNetSDK.MINOR_IR_ADAPTOR_COMM_EXCEPTION:
                    CsTemp = "IR_ADAPTOR_COMM_EXCEPTION";
                    break;
                case CHCNetSDK.MINOR_IR_ADAPTOR_COMM_RESUME:
                    CsTemp = "IR_ADAPTOR_COMM_RESUME";
                    break;
                default:
                    CsTemp = Convert.ToString(struEventCfg.dwMinor, 16);
                    break;
            }
        }

        private void OperationMinorTypeMap(ref CHCNetSDK.NET_DVR_ACS_EVENT_CFG struEventCfg)
        {
            switch (struEventCfg.dwMinor)
            {
                case CHCNetSDK.MINOR_LOCAL_UPGRADE:
                    CsTemp = "LOCAL_UPGRADE";
                    break;
                case CHCNetSDK.MINOR_REMOTE_LOGIN:
                    CsTemp = "REMOTE_LOGIN";
                    break;
                case CHCNetSDK.MINOR_REMOTE_LOGOUT:
                    CsTemp = "REMOTE_LOGOUT";
                    break;
                case CHCNetSDK.MINOR_REMOTE_ARM:
                    CsTemp = "REMOTE_ARM";
                    break;
                case CHCNetSDK.MINOR_REMOTE_DISARM:
                    CsTemp = "REMOTE_DISARM";
                    break;
                case CHCNetSDK.MINOR_REMOTE_REBOOT:
                    CsTemp = "REMOTE_REBOOT";
                    break;
                case CHCNetSDK.MINOR_REMOTE_UPGRADE:
                    CsTemp = "REMOTE_UPGRADE";
                    break;
                case CHCNetSDK.MINOR_REMOTE_CFGFILE_OUTPUT:
                    CsTemp = "REMOTE_CFGFILE_OUTPUT";
                    break;
                case CHCNetSDK.MINOR_REMOTE_CFGFILE_INTPUT:
                    CsTemp = "REMOTE_CFGFILE_INTPUT";
                    break;
                case CHCNetSDK.MINOR_REMOTE_ALARMOUT_OPEN_MAN:
                    CsTemp = "REMOTE_ALARMOUT_OPEN_MAN";
                    break;
                case CHCNetSDK.MINOR_REMOTE_ALARMOUT_CLOSE_MAN:
                    CsTemp = "REMOTE_ALARMOUT_CLOSE_MAN";
                    break;
                case CHCNetSDK.MINOR_REMOTE_OPEN_DOOR:
                    CsTemp = "REMOTE_OPEN_DOOR";
                    break;
                case CHCNetSDK.MINOR_REMOTE_CLOSE_DOOR:
                    CsTemp = "REMOTE_CLOSE_DOOR";
                    break;
                case CHCNetSDK.MINOR_REMOTE_ALWAYS_OPEN:
                    CsTemp = "REMOTE_ALWAYS_OPEN";
                    break;
                case CHCNetSDK.MINOR_REMOTE_ALWAYS_CLOSE:
                    CsTemp = "REMOTE_ALWAYS_CLOSE";
                    break;
                case CHCNetSDK.MINOR_REMOTE_CHECK_TIME:
                    CsTemp = "REMOTE_CHECK_TIME";
                    break;
                case CHCNetSDK.MINOR_NTP_CHECK_TIME:
                    CsTemp = "NTP_CHECK_TIME";
                    break;
                case CHCNetSDK.MINOR_REMOTE_CLEAR_CARD:
                    CsTemp = "REMOTE_CLEAR_CARD";
                    break;
                case CHCNetSDK.MINOR_REMOTE_RESTORE_CFG:
                    CsTemp = "REMOTE_RESTORE_CFG";
                    break;
                case CHCNetSDK.MINOR_ALARMIN_ARM:
                    CsTemp = "ALARMIN_ARM";
                    break;
                case CHCNetSDK.MINOR_ALARMIN_DISARM:
                    CsTemp = "ALARMIN_DISARM";
                    break;
                case CHCNetSDK.MINOR_LOCAL_RESTORE_CFG:
                    CsTemp = "LOCAL_RESTORE_CFG";
                    break;
                case CHCNetSDK.MINOR_REMOTE_CAPTURE_PIC:
                    CsTemp = "REMOTE_CAPTURE_PIC";
                    break;
                case CHCNetSDK.MINOR_MOD_NET_REPORT_CFG:
                    CsTemp = "MOD_NET_REPORT_CFG";
                    break;
                case CHCNetSDK.MINOR_MOD_GPRS_REPORT_PARAM:
                    CsTemp = "MOD_GPRS_REPORT_PARAM";
                    break;
                case CHCNetSDK.MINOR_MOD_REPORT_GROUP_PARAM:
                    CsTemp = "MOD_REPORT_GROUP_PARAM";
                    break;
                case CHCNetSDK.MINOR_UNLOCK_PASSWORD_OPEN_DOOR:
                    CsTemp = "UNLOCK_PASSWORD_OPEN_DOOR";
                    break;
                case CHCNetSDK.MINOR_AUTO_RENUMBER:
                    CsTemp = "AUTO_RENUMBER";
                    break;
                case CHCNetSDK.MINOR_AUTO_COMPLEMENT_NUMBER:
                    CsTemp = "AUTO_COMPLEMENT_NUMBER";
                    break;
                case CHCNetSDK.MINOR_NORMAL_CFGFILE_INPUT:
                    CsTemp = "NORMAL_CFGFILE_INPUT";
                    break;
                case CHCNetSDK.MINOR_NORMAL_CFGFILE_OUTTPUT:
                    CsTemp = "NORMAL_CFGFILE_OUTTPUT";
                    break;
                case CHCNetSDK.MINOR_CARD_RIGHT_INPUT:
                    CsTemp = "CARD_RIGHT_INPUT";
                    break;
                case CHCNetSDK.MINOR_CARD_RIGHT_OUTTPUT:
                    CsTemp = "CARD_RIGHT_OUTTPUT";
                    break;
                case CHCNetSDK.MINOR_LOCAL_USB_UPGRADE:
                    CsTemp = "LOCAL_USB_UPGRADE";
                    break;
                case CHCNetSDK.MINOR_REMOTE_VISITOR_CALL_LADDER:
                    CsTemp = "REMOTE_VISITOR_CALL_LADDER";
                    break;
                case CHCNetSDK.MINOR_REMOTE_HOUSEHOLD_CALL_LADDER:
                    CsTemp = "REMOTE_HOUSEHOLD_CALL_LADDER";
                    break;
                case CHCNetSDK.MINOR_REMOTE_ACTUAL_GUARD:
                    CsTemp = "REMOTE_ACTUAL_GUARD";
                    break;
                case CHCNetSDK.MINOR_REMOTE_ACTUAL_UNGUARD:
                    CsTemp = "REMOTE_ACTUAL_UNGUARD";
                    break;
                case CHCNetSDK.MINOR_REMOTE_CONTROL_NOT_CODE_OPER_FAILED:
                    CsTemp = "REMOTE_CONTROL_NOT_CODE_OPER_FAILED";
                    break;
                case CHCNetSDK.MINOR_REMOTE_CONTROL_CLOSE_DOOR:
                    CsTemp = "REMOTE_CONTROL_CLOSE_DOOR";
                    break;
                case CHCNetSDK.MINOR_REMOTE_CONTROL_OPEN_DOOR:
                    CsTemp = "REMOTE_CONTROL_OPEN_DOOR";
                    break;
                case CHCNetSDK.MINOR_REMOTE_CONTROL_ALWAYS_OPEN_DOOR:
                    CsTemp = "REMOTE_CONTROL_ALWAYS_OPEN_DOOR";
                    break;
                default:
                    CsTemp = Convert.ToString(struEventCfg.dwMinor, 16);
                    break;
            }
        }

        private void EventMinorTypeMap(ref CHCNetSDK.NET_DVR_ACS_EVENT_CFG struEventCfg)
        {
            switch (struEventCfg.dwMinor)
            {
                case CHCNetSDK.MINOR_LEGAL_CARD_PASS:
                    CsTemp = "LEGAL_CARD_PASS";
                    break;
                case CHCNetSDK.MINOR_CARD_AND_PSW_PASS:
                    CsTemp = "CARD_AND_PSW_PASS";
                    break;
                case CHCNetSDK.MINOR_CARD_AND_PSW_FAIL:
                    CsTemp = "CARD_AND_PSW_FAIL";
                    break;
                case CHCNetSDK.MINOR_CARD_AND_PSW_TIMEOUT:
                    CsTemp = "CARD_AND_PSW_TIMEOUT";
                    break;
                case CHCNetSDK.MINOR_CARD_AND_PSW_OVER_TIME:
                    CsTemp = "CARD_AND_PSW_OVER_TIME";
                    break;
                case CHCNetSDK.MINOR_CARD_NO_RIGHT:
                    CsTemp = "CARD_NO_RIGHT";
                    break;
                case CHCNetSDK.MINOR_CARD_INVALID_PERIOD:
                    CsTemp = "CARD_INVALID_PERIOD";
                    break;
                case CHCNetSDK.MINOR_CARD_OUT_OF_DATE:
                    CsTemp = "CARD_OUT_OF_DATE";
                    break;
                case CHCNetSDK.MINOR_INVALID_CARD:
                    CsTemp = "INVALID_CARD";
                    break;
                case CHCNetSDK.MINOR_ANTI_SNEAK_FAIL:
                    CsTemp = "ANTI_SNEAK_FAIL";
                    break;
                case CHCNetSDK.MINOR_INTERLOCK_DOOR_NOT_CLOSE:
                    CsTemp = "INTERLOCK_DOOR_NOT_CLOSE";
                    break;
                case CHCNetSDK.MINOR_NOT_BELONG_MULTI_GROUP:
                    CsTemp = "NOT_BELONG_MULTI_GROUP";
                    break;
                case CHCNetSDK.MINOR_INVALID_MULTI_VERIFY_PERIOD:
                    CsTemp = "INVALID_MULTI_VERIFY_PERIOD";
                    break;
                case CHCNetSDK.MINOR_MULTI_VERIFY_SUPER_RIGHT_FAIL:
                    CsTemp = "MULTI_VERIFY_SUPER_RIGHT_FAIL";
                    break;
                case CHCNetSDK.MINOR_MULTI_VERIFY_REMOTE_RIGHT_FAIL:
                    CsTemp = "MULTI_VERIFY_REMOTE_RIGHT_FAIL";
                    break;
                case CHCNetSDK.MINOR_MULTI_VERIFY_SUCCESS:
                    CsTemp = "MULTI_VERIFY_SUCCESS";
                    break;
                case CHCNetSDK.MINOR_LEADER_CARD_OPEN_BEGIN:
                    CsTemp = "LEADER_CARD_OPEN_BEGIN";
                    break;
                case CHCNetSDK.MINOR_LEADER_CARD_OPEN_END:
                    CsTemp = "LEADER_CARD_OPEN_END";
                    break;
                case CHCNetSDK.MINOR_ALWAYS_OPEN_BEGIN:
                    CsTemp = "ALWAYS_OPEN_BEGIN";
                    break;
                case CHCNetSDK.MINOR_ALWAYS_OPEN_END:
                    CsTemp = "ALWAYS_OPEN_END";
                    break;
                case CHCNetSDK.MINOR_LOCK_OPEN:
                    CsTemp = "LOCK_OPEN";
                    break;
                case CHCNetSDK.MINOR_LOCK_CLOSE:
                    CsTemp = "LOCK_CLOSE";
                    break;
                case CHCNetSDK.MINOR_DOOR_BUTTON_PRESS:
                    CsTemp = "DOOR_BUTTON_PRESS";
                    break;
                case CHCNetSDK.MINOR_DOOR_BUTTON_RELEASE:
                    CsTemp = "DOOR_BUTTON_RELEASE";
                    break;
                case CHCNetSDK.MINOR_DOOR_OPEN_NORMAL:
                    CsTemp = "DOOR_OPEN_NORMAL";
                    break;
                case CHCNetSDK.MINOR_DOOR_CLOSE_NORMAL:
                    CsTemp = "DOOR_CLOSE_NORMAL";
                    break;
                case CHCNetSDK.MINOR_DOOR_OPEN_ABNORMAL:
                    CsTemp = "DOOR_OPEN_ABNORMAL";
                    break;
                case CHCNetSDK.MINOR_DOOR_OPEN_TIMEOUT:
                    CsTemp = "DOOR_OPEN_TIMEOUT";
                    break;
                case CHCNetSDK.MINOR_ALARMOUT_ON:
                    CsTemp = "ALARMOUT_ON";
                    break;
                case CHCNetSDK.MINOR_ALARMOUT_OFF:
                    CsTemp = "ALARMOUT_OFF";
                    break;
                case CHCNetSDK.MINOR_ALWAYS_CLOSE_BEGIN:
                    CsTemp = "ALWAYS_CLOSE_BEGIN";
                    break;
                case CHCNetSDK.MINOR_ALWAYS_CLOSE_END:
                    CsTemp = "ALWAYS_CLOSE_END";
                    break;
                case CHCNetSDK.MINOR_MULTI_VERIFY_NEED_REMOTE_OPEN:
                    CsTemp = "MULTI_VERIFY_NEED_REMOTE_OPEN";
                    break;
                case CHCNetSDK.MINOR_MULTI_VERIFY_SUPERPASSWD_VERIFY_SUCCESS:
                    CsTemp = "MULTI_VERIFY_SUPERPASSWD_VERIFY_SUCCESS";
                    break;
                case CHCNetSDK.MINOR_MULTI_VERIFY_REPEAT_VERIFY:
                    CsTemp = "MULTI_VERIFY_REPEAT_VERIFY";
                    break;
                case CHCNetSDK.MINOR_MULTI_VERIFY_TIMEOUT:
                    CsTemp = "MULTI_VERIFY_TIMEOUT";
                    break;
                case CHCNetSDK.MINOR_DOORBELL_RINGING:
                    CsTemp = "DOORBELL_RINGING";
                    break;
                case CHCNetSDK.MINOR_FINGERPRINT_COMPARE_PASS:
                    CsTemp = "FINGERPRINT_COMPARE_PASS";
                    break;
                case CHCNetSDK.MINOR_FINGERPRINT_COMPARE_FAIL:
                    CsTemp = "FINGERPRINT_COMPARE_FAIL";
                    break;
                case CHCNetSDK.MINOR_CARD_FINGERPRINT_VERIFY_PASS:
                    CsTemp = "CARD_FINGERPRINT_VERIFY_PASS";
                    break;
                case CHCNetSDK.MINOR_CARD_FINGERPRINT_VERIFY_FAIL:
                    CsTemp = "CARD_FINGERPRINT_VERIFY_FAIL";
                    break;
                case CHCNetSDK.MINOR_CARD_FINGERPRINT_VERIFY_TIMEOUT:
                    CsTemp = "CARD_FINGERPRINT_VERIFY_TIMEOUT";
                    break;
                case CHCNetSDK.MINOR_CARD_FINGERPRINT_PASSWD_VERIFY_PASS:
                    CsTemp = "CARD_FINGERPRINT_PASSWD_VERIFY_PASS";
                    break;
                case CHCNetSDK.MINOR_CARD_FINGERPRINT_PASSWD_VERIFY_FAIL:
                    CsTemp = "CARD_FINGERPRINT_PASSWD_VERIFY_FAIL";
                    break;
                case CHCNetSDK.MINOR_CARD_FINGERPRINT_PASSWD_VERIFY_TIMEOUT:
                    CsTemp = "CARD_FINGERPRINT_PASSWD_VERIFY_TIMEOUT";
                    break;
                case CHCNetSDK.MINOR_FINGERPRINT_PASSWD_VERIFY_PASS:
                    CsTemp = "FINGERPRINT_PASSWD_VERIFY_PASS";
                    break;
                case CHCNetSDK.MINOR_FINGERPRINT_PASSWD_VERIFY_FAIL:
                    CsTemp = "FINGERPRINT_PASSWD_VERIFY_FAIL";
                    break;
                case CHCNetSDK.MINOR_FINGERPRINT_PASSWD_VERIFY_TIMEOUT:
                    CsTemp = "FINGERPRINT_PASSWD_VERIFY_TIMEOUT";
                    break;
                case CHCNetSDK.MINOR_FINGERPRINT_INEXISTENCE:
                    CsTemp = "FINGERPRINT_INEXISTENCE";
                    break;
                case CHCNetSDK.MINOR_CARD_PLATFORM_VERIFY:
                    CsTemp = "CARD_PLATFORM_VERIFY";
                    break;
                case CHCNetSDK.MINOR_MAC_DETECT:
                    CsTemp = "MINOR_MAC_DETECT";
                    break;
                case CHCNetSDK.MINOR_LEGAL_MESSAGE:
                    CsTemp = "MINOR_LEGAL_MESSAGE";
                    break;
                case CHCNetSDK.MINOR_ILLEGAL_MESSAGE:
                    CsTemp = "MINOR_ILLEGAL_MESSAGE";
                    break;
                case CHCNetSDK.MINOR_DOOR_OPEN_OR_DORMANT_FAIL:
                    CsTemp = "DOOR_OPEN_OR_DORMANT_FAIL";
                    break;
                case CHCNetSDK.MINOR_AUTH_PLAN_DORMANT_FAIL:
                    CsTemp = "AUTH_PLAN_DORMANT_FAIL";
                    break;
                case CHCNetSDK.MINOR_CARD_ENCRYPT_VERIFY_FAIL:
                    CsTemp = "CARD_ENCRYPT_VERIFY_FAIL";
                    break;
                case CHCNetSDK.MINOR_SUBMARINEBACK_REPLY_FAIL:
                    CsTemp = "SUBMARINEBACK_REPLY_FAIL";
                    break;
                case CHCNetSDK.MINOR_DOOR_OPEN_OR_DORMANT_OPEN_FAIL:
                    CsTemp = "DOOR_OPEN_OR_DORMANT_OPEN_FAIL";
                    break;
                case CHCNetSDK.MINOR_DOOR_OPEN_OR_DORMANT_LINKAGE_OPEN_FAIL:
                    CsTemp = "DOOR_OPEN_OR_DORMANT_LINKAGE_OPEN_FAIL";
                    break;
                case CHCNetSDK.MINOR_TRAILING:
                    CsTemp = "TRAILING";
                    break;
                case CHCNetSDK.MINOR_REVERSE_ACCESS:
                    CsTemp = "REVERSE_ACCESS";
                    break;
                case CHCNetSDK.MINOR_FORCE_ACCESS:
                    CsTemp = "FORCE_ACCESS";
                    break;
                case CHCNetSDK.MINOR_CLIMBING_OVER_GATE:
                    CsTemp = "CLIMBING_OVER_GATE";
                    break;
                case CHCNetSDK.MINOR_PASSING_TIMEOUT:
                    CsTemp = "PASSING_TIMEOUT";
                    break;
                case CHCNetSDK.MINOR_INTRUSION_ALARM:
                    CsTemp = "INTRUSION_ALARM";
                    break;
                case CHCNetSDK.MINOR_FREE_GATE_PASS_NOT_AUTH:
                    CsTemp = "FREE_GATE_PASS_NOT_AUTH";
                    break;
                case CHCNetSDK.MINOR_DROP_ARM_BLOCK:
                    CsTemp = "DROP_ARM_BLOCK";
                    break;
                case CHCNetSDK.MINOR_DROP_ARM_BLOCK_RESUME:
                    CsTemp = "DROP_ARM_BLOCK_RESUME";
                    break;
                case CHCNetSDK.MINOR_LOCAL_FACE_MODELING_FAIL:
                    CsTemp = "LOCAL_FACE_MODELING_FAIL";
                    break;
                case CHCNetSDK.MINOR_STAY_EVENT:
                    CsTemp = "STAY_EVENT";
                    break;
                case CHCNetSDK.MINOR_PASSWORD_MISMATCH:
                    CsTemp = "PASSWORD_MISMATCH";
                    break;
                case CHCNetSDK.MINOR_EMPLOYEE_NO_NOT_EXIST:
                    CsTemp = "EMPLOYEE_NO_NOT_EXIST";
                    break;
                case CHCNetSDK.MINOR_COMBINED_VERIFY_PASS:
                    CsTemp = "COMBINED_VERIFY_PASS";
                    break;
                case CHCNetSDK.MINOR_COMBINED_VERIFY_TIMEOUT:
                    CsTemp = "COMBINED_VERIFY_TIMEOUT";
                    break;
                case CHCNetSDK.MINOR_VERIFY_MODE_MISMATCH:
                    CsTemp = "VERIFY_MODE_MISMATCH";
                    break;
                default:
                    CsTemp = Convert.ToString(struEventCfg.dwMinor, 16);
                    break;
            }
        }

        private void ProcessMinorType(ref CHCNetSDK.NET_DVR_ACS_EVENT_CFG struEventCfg)
        {
            switch (struEventCfg.dwMajor)
            {
                case CHCNetSDK.MAJOR_ALARM:
                    AlarmMinorTypeMap(ref struEventCfg);
                    break;
                case CHCNetSDK.MAJOR_EXCEPTION:
                    ExceptionMinorTypeMap(ref struEventCfg);
                    break;
                case CHCNetSDK.MAJOR_OPERATION:
                    OperationMinorTypeMap(ref struEventCfg);
                    break;
                case CHCNetSDK.MAJOR_EVENT:
                    EventMinorTypeMap(ref struEventCfg);
                    break;
                default:
                    CsTemp = "Unknown";
                    break;
            }
        }

        private void CardTypeMap(ref CHCNetSDK.NET_DVR_ACS_EVENT_CFG struEventCfg)
        {
            switch (struEventCfg.struAcsEventInfo.byCardType)
            {
                case 1:
                    CsTemp = "Ordinary Card";
                    break;
                case 2:
                    CsTemp = "Disabled Card";
                    break;
                case 3:
                    CsTemp = "Block List Card";
                    break;
                case 4:
                    CsTemp = "Patrol Card";
                    break;
                case 5:
                    CsTemp = "Stress Card";
                    break;
                case 6:
                    CsTemp = "Super Card";
                    break;
                case 7:
                    CsTemp = "Guest Card";
                    break;
                case 8:
                    CsTemp = "Release Card";
                    break;
                default:
                    CsTemp = "No effect";
                    break;
            }
        }

        private void ProcessReportChannel(ref CHCNetSDK.NET_DVR_ACS_EVENT_CFG EventCfg)
        {
            switch (EventCfg.struAcsEventInfo.byReportChannel)
            {
                case 1:
                    CsTemp = "Upload";
                    break;
                case 2:
                    CsTemp = "Center 1 Upload";
                    break;
                case 3:
                    CsTemp = "Center 2 Upload";
                    break;
                default:
                    CsTemp = "No effect";
                    break;
            }
        }

        private void ProcessCardReader(ref CHCNetSDK.NET_DVR_ACS_EVENT_CFG struEventCfg)
        {
            switch (struEventCfg.struAcsEventInfo.byCardReaderKind)
            {
                case 1:
                    CsTemp = "IC Reader";
                    break;
                case 2:
                    CsTemp = "Certificate Reader";
                    break;
                case 3:
                    CsTemp = "Two-dimension Reader";
                    break;
                case 4:
                    CsTemp = "Finger Print Head";
                    break;
                default:
                    CsTemp = "No effect";
                    break;
            }
        }

        private void ProcessByType(ref CHCNetSDK.NET_DVR_ACS_EVENT_CFG struEventCfg)
        {
            switch (struEventCfg.struAcsEventInfo.byType)
            {
                case 0:
                    CsTemp = "Instant Zone";
                    break;
                case 1:
                    CsTemp = "24 Hour Zone";
                    break;
                case 2:
                    CsTemp = "Delay Zone";
                    break;
                case 3:
                    CsTemp = "Internal Zone";
                    break;
                case 4:
                    CsTemp = "Key Zone";
                    break;
                case 5:
                    CsTemp = "Fire Zone";
                    break;
                case 6:
                    CsTemp = "Perimeter Zone";
                    break;
                case 7:
                    CsTemp = "24 Hour Silent Zone";
                    break;
                case 8:
                    CsTemp = "24 Hour Auxiliary Zone";
                    break;
                case 9:
                    CsTemp = "24 Hour Vibration Zone";
                    break;
                case 10:
                    CsTemp = "Acs Emergency Open Zone";
                    break;
                case 11:
                    CsTemp = "Acs Emergency Close Zone";
                    break;
                default:
                    CsTemp = "No Effect";
                    break;
            }
        }
        private void ProcessInternatAccess(ref CHCNetSDK.NET_DVR_ACS_EVENT_CFG struEventCfg)
        {
            switch (struEventCfg.struAcsEventInfo.byInternetAccess)
            {
                case 1:
                    CsTemp = "Up Network Port 1";
                    break;
                case 2:
                    CsTemp = "Up Network Port 2";
                    break;
                case 3:
                    CsTemp = "Down Network Port 1";
                    break;
                default:
                    CsTemp = "No effect";
                    break;
            }
        }

        private void ProcessSwipeCard(ref CHCNetSDK.NET_DVR_ACS_EVENT_CFG struEventCfg)
        {
            if (struEventCfg.struAcsEventInfo.bySwipeCardType == 1)
            {
                CsTemp = "QR Code";
            }
            else
            {
                CsTemp = "No effect";
            }
        }

        private void ProcessMacAdd(ref CHCNetSDK.NET_DVR_ACS_EVENT_CFG struEventCfg)
        {
            if (struEventCfg.struAcsEventInfo.byMACAddr[0] != 0)
            {
                CsTemp = struEventCfg.struAcsEventInfo.byMACAddr[0].ToString() + ":" +
                    struEventCfg.struAcsEventInfo.byMACAddr[1].ToString() + ":" +
                    struEventCfg.struAcsEventInfo.byMACAddr[2].ToString() + ":" +
                    struEventCfg.struAcsEventInfo.byMACAddr[3].ToString() + ":" +
                    struEventCfg.struAcsEventInfo.byMACAddr[4].ToString() + ":" +
                    struEventCfg.struAcsEventInfo.byMACAddr[5].ToString();
            }
            else
            {
                CsTemp = "No Effect";
            }
        }

        private string GetStrLogTime(ref CHCNetSDK.NET_DVR_TIME time)
        {
            string res = time.dwYear.ToString() + ":" + time.dwMonth.ToString() + ":"
                + time.dwDay.ToString() + ":" + time.dwHour.ToString() + ":" + time.dwMinute.ToString()
                + ":" + time.dwSecond.ToString();
            return res;
        }

        private string ProcessMajorType(ref int dwMajor)
        {
            string res = null;
            switch (dwMajor)
            {
                case 1:
                    res = "Alarm";
                    break;
                case 2:
                    res = "Exception";
                    break;
                case 3:
                    res = "Operation";
                    break;
                case 5:
                    res = "Event";
                    break;
                default:
                    res = "Unknown";
                    break;
            }
            return res;
        }


        private void AddAcsEventToList(ref CHCNetSDK.NET_DVR_ACS_EVENT_CFG struEventCfg)
        {
            this.listViewEvent.BeginUpdate();
            ListViewItem Item = new ListViewItem();
            Item.Text = (++m_lLogNum).ToString();

            string LogTime = GetStrLogTime(ref struEventCfg.struTime);
            Item.SubItems.Add(LogTime);

            string Major = ProcessMajorType(ref struEventCfg.dwMajor);
            Item.SubItems.Add(Major);

            ProcessMinorType(ref struEventCfg);
            Item.SubItems.Add(CsTemp);

            CsTemp = System.Text.Encoding.UTF8.GetString(struEventCfg.struAcsEventInfo.byCardNo);
            Item.SubItems.Add(CsTemp);

            CardTypeMap(ref struEventCfg);
            Item.SubItems.Add(CsTemp);

            Item.SubItems.Add(struEventCfg.struAcsEventInfo.byAllowListNo.ToString());//WhiteList

            ProcessReportChannel(ref struEventCfg);
            Item.SubItems.Add(CsTemp);

            ProcessCardReader(ref struEventCfg);
            Item.SubItems.Add(CsTemp);

            CsTemp = struEventCfg.struAcsEventInfo.dwCardReaderNo.ToString();
            Item.SubItems.Add(CsTemp);

            Item.SubItems.Add(struEventCfg.struAcsEventInfo.dwDoorNo.ToString());

            Item.SubItems.Add(struEventCfg.struAcsEventInfo.dwVerifyNo.ToString());

            Item.SubItems.Add(struEventCfg.struAcsEventInfo.dwAlarmInNo.ToString());

            Item.SubItems.Add(struEventCfg.struAcsEventInfo.dwAlarmOutNo.ToString());

            Item.SubItems.Add(struEventCfg.struAcsEventInfo.dwCaseSensorNo.ToString());

            Item.SubItems.Add(struEventCfg.struAcsEventInfo.dwRs485No.ToString());

            Item.SubItems.Add(struEventCfg.struAcsEventInfo.dwMultiCardGroupNo.ToString());

            Item.SubItems.Add(struEventCfg.struAcsEventInfo.wAccessChannel.ToString());

            Item.SubItems.Add(struEventCfg.struAcsEventInfo.byDeviceNo.ToString());

            Item.SubItems.Add(struEventCfg.struAcsEventInfo.dwEmployeeNo.ToString());

            Item.SubItems.Add(struEventCfg.struAcsEventInfo.byDistractControlNo.ToString());

            Item.SubItems.Add(struEventCfg.struAcsEventInfo.wLocalControllerID.ToString());

            ProcessInternatAccess(ref struEventCfg);
            Item.SubItems.Add(CsTemp);

            ProcessByType(ref struEventCfg);
            Item.SubItems.Add(CsTemp);

            ProcessMacAdd(ref struEventCfg);
            Item.SubItems.Add(CsTemp);

            ProcessSwipeCard(ref struEventCfg);
            Item.SubItems.Add(CsTemp);

            Item.SubItems.Add(struEventCfg.struAcsEventInfo.dwSerialNo.ToString());

            Item.SubItems.Add(struEventCfg.struAcsEventInfo.byChannelControllerID.ToString());

            Item.SubItems.Add(struEventCfg.struAcsEventInfo.byChannelControllerLampID.ToString());

            Item.SubItems.Add(struEventCfg.struAcsEventInfo.byChannelControllerIRAdaptorID.ToString());

            Item.SubItems.Add(struEventCfg.struAcsEventInfo.byChannelControllerIREmitterID.ToString());

            if (struEventCfg.wInductiveEventType < (ushort)GetAcsEventType.NumOfInductiveEvent())
            {
                Item.SubItems.Add(GetAcsEventType.FindKeyOfInductive(struEventCfg.wInductiveEventType));
            }
            else
            {
                Item.SubItems.Add("Invalid");
            }

            Item.SubItems.Add("0");//RecordChannelNum

            ProcessbyUserType(ref struEventCfg);
            Item.SubItems.Add("0");

            ProcessVerifyMode(ref struEventCfg);
            Item.SubItems.Add("0");

            CsTemp = System.Text.Encoding.UTF8.GetString(struEventCfg.struAcsEventInfo.byEmployeeNo);
            Item.SubItems.Add(CsTemp);

            string strTemperature = struEventCfg.struAcsEventInfo.fCurrTemperature.ToString();
            Item.SubItems.Add(strTemperature);
            //增加考勤状态
            //0-undefined, 1-checkIn, 2-checkOut, 3-breakOut, 4-breakIn, 5-overtimeIn, 6-overtimeOut 
            if (struEventCfg.struAcsEventInfo.byAttendanceStatus == 0)
            {
                Item.SubItems.Add("undefined");
            }
            else if (struEventCfg.struAcsEventInfo.byAttendanceStatus == 1)
            {
                Item.SubItems.Add("checkIn");
            }
            else if (struEventCfg.struAcsEventInfo.byAttendanceStatus == 2)
            {
                Item.SubItems.Add("checkOut");
            }
            else if (struEventCfg.struAcsEventInfo.byAttendanceStatus == 3)
            {
                Item.SubItems.Add("breakOut");
            }
            else if (struEventCfg.struAcsEventInfo.byAttendanceStatus == 4)
            {
                Item.SubItems.Add("breakIn");
            }
            else if (struEventCfg.struAcsEventInfo.byAttendanceStatus == 5)
            {
                Item.SubItems.Add("overtimeIn");
            }
            else if (struEventCfg.struAcsEventInfo.byAttendanceStatus == 6)
            {
                Item.SubItems.Add("overtimeOut");
            }
            CsTemp = null;
            this.listViewEvent.Items.Add(Item);
            this.listViewEvent.EndUpdate();
        }

        private void ProcessPicData(ref CHCNetSDK.NET_DVR_ACS_EVENT_CFG struEventCfg, string path)
        {
            if (struEventCfg.dwPicDataLen > 0 && struEventCfg.pPicData != IntPtr.Zero)
            {

                using (FileStream fs = new FileStream(path, FileMode.Create))
                {
                    int iLen = (int)struEventCfg.dwPicDataLen;
                    byte[] by = new byte[iLen];
                    Marshal.Copy(struEventCfg.pPicData, by, 0, iLen);
                    fs.Write(by, 0, iLen);
                    fs.Close();
                }
            }
        }

        private void ProcessbyUserType(ref CHCNetSDK.NET_DVR_ACS_EVENT_CFG struEventCfg)
        {
            switch (struEventCfg.struAcsEventInfo.byUserType)
            {
                case 1:
                    CsTemp = "host";
                    break;
                case 2:
                    CsTemp = "visitor";
                    break;
                case 3:
                    CsTemp = "block list";
                    break;
                case 4:
                    CsTemp = "administrator";
                    break;
                default:
                    CsTemp = "No Effect";
                    break;
            }
        }

        private void ProcessVerifyMode(ref CHCNetSDK.NET_DVR_ACS_EVENT_CFG struEventCfg)
        {
            switch (struEventCfg.struAcsEventInfo.byCurrentVerifyMode)
            {
                case 1:
                    CsTemp = "dormancy";
                    break;
                case 2:
                    CsTemp = "cardAndPw";
                    break;
                case 3:
                    CsTemp = "card";
                    break;
                case 4:
                    CsTemp = "cardOrPw";
                    break;
                case 5:
                    CsTemp = "fp";
                    break;
                case 6:
                    CsTemp = "fpAndPw";
                    break;
                case 7:
                    CsTemp = "fpOrCard";
                    break;
                case 8:
                    CsTemp = "fpAndCard";
                    break;
                case 9:
                    CsTemp = "fpAndCardAndPw";
                    break;
                case 10:
                    CsTemp = "faceOrFpOrCardOrPw";
                    break;
                case 11:
                    CsTemp = "faceAndFp";
                    break;
                case 12:
                    CsTemp = "faceAndPw";
                    break;
                case 13:
                    CsTemp = "faceAndCard";
                    break;
                case 14:
                    CsTemp = "face";
                    break;
                case 15:
                    CsTemp = "employeeNoAndPw";
                    break;
                case 16:
                    CsTemp = "fpOrPw";
                    break;
                case 17:
                    CsTemp = "employeeNoAndFp";
                    break;
                case 18:
                    CsTemp = "employeeNoAndFpAndPw";
                    break;
                case 19:
                    CsTemp = "faceAndFpAndCard";
                    break;
                case 20:
                    CsTemp = "faceAndPwAndFp";
                    break;
                case 21:
                    CsTemp = "employeeNoAndFace";
                    break;
                case 22:
                    CsTemp = "faceOrfaceAndCard";
                    break;
                case 23:
                    CsTemp = "fpOrface";
                    break;
                case 24:
                    CsTemp = "cardOrfaceOrPw";
                    break;
                default:
                    CsTemp = "No effect";
                    break;
            }
        }

        private void GetACSEvent_FormClosing(object sender, FormClosingEventArgs e)
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

        private void listViewEvent_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

    }
}
