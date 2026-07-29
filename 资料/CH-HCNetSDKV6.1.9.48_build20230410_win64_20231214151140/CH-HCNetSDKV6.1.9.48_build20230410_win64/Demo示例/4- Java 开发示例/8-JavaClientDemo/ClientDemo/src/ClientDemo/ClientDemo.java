/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * ClientDemo.java
 *
 * Created on 2009-9-14, 19:31:34
 */
/**
 * @author Xubinfeng
 */

package ClientDemo;

import Commom.osSelect;
import com.sun.jna.Native;
import com.sun.jna.NativeLong;
import com.sun.jna.Pointer;
import com.sun.jna.examples.win32.W32API.HWND;
import com.sun.jna.ptr.ByteByReference;
import com.sun.jna.ptr.IntByReference;
import com.sun.jna.ptr.NativeLongByReference;

import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JPopupMenu;
import javax.swing.JWindow;
import javax.swing.plaf.basic.BasicTextAreaUI;
import javax.swing.table.DefaultTableModel;
import javax.swing.tree.*;
import java.awt.Color;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.Toolkit;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

/*****************************************************************************
 * 主类 ：ClientDemo
 * 用途 ：用户注册，预览，参数配置菜单
 * 容器：Jframe
 ****************************************************************************/
public class ClientDemo extends javax.swing.JFrame {
    /*************************************************
     * 函数:      主类构造函数
     * 函数描述:	初始化成员
     *************************************************/

    public ClientDemo() {
        JPopupMenu.setDefaultLightWeightPopupEnabled(false);//防止被播放窗口(AWT组件)覆盖
        initComponents();
        int lUserID = -1;
        int lPreviewHandle =-1;
        int lAlarmHandle = -1;
        int lListenHandle = -1;
        int g_lVoiceHandle = -1;
        m_lPort= new IntByReference(-1);
        fMSFCallBack = null;
        fRealDataCallBack = new FRealDataCallBack();
        fExceptionCallBack = new FExceptionCallBack_Imp();
        m_iTreeNodeNum = 0;
    }

    static HCNetSDK hCNetSDK = null;
    static PlayCtrl playControl = null;

    public static int g_lVoiceHandle;//全局的语音对讲句柄

    static HCNetSDK.NET_DVR_DEVICEINFO_V30 m_strDeviceInfo;//设备信息
    static HCNetSDK.NET_DVR_IPPARACFG m_strIpparaCfg;//IP参数
    static HCNetSDK.NET_DVR_CLIENTINFO m_strClientInfo;//用户参数

    boolean bRealPlay;//是否在预览.
    String m_sDeviceIP;//已登录设备的IP地址

    int  lUserID;//用户句柄
    int  lPreviewHandle;//预览句柄
    IntByReference m_lPort;//回调预览时播放库端口指针

    int  lAlarmHandle;//报警布防句柄
    int lListenHandle;//报警监听句柄

    static FMSGCallBack fMSFCallBack;//报警回调函数实现
    static FRealDataCallBack fRealDataCallBack;//预览回调函数实现
    static FExceptionCallBack_Imp fExceptionCallBack;

    JFramePTZControl framePTZControl;//云台控制窗口

    int m_iTreeNodeNum;//通道树节点数目
    DefaultMutableTreeNode m_DeviceRoot;//通道树根节点

    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jSplitPaneHorizontal = new javax.swing.JSplitPane();
        jPanelUserInfo = new javax.swing.JPanel();
        jButtonRealPlay = new javax.swing.JButton();
        jButtonLogin = new javax.swing.JButton();
        jLabelUserName = new javax.swing.JLabel();
        jLabelIPAddress = new javax.swing.JLabel();
        jTextFieldPortNumber = new javax.swing.JTextField();
        jTextFieldIPAddress = new javax.swing.JTextField();
        jLabelPortNumber = new javax.swing.JLabel();
        jLabelPassWord = new javax.swing.JLabel();
        jPasswordFieldPassword = new javax.swing.JPasswordField();
        jTextFieldUserName = new javax.swing.JTextField();
        jButtonExit = new javax.swing.JButton();
        jScrollPaneTree = new javax.swing.JScrollPane();
        jTreeDevice = new javax.swing.JTree();
        jComboBoxCallback = new javax.swing.JComboBox();
        jSplitPaneVertical = new javax.swing.JSplitPane();
        jPanelRealplayArea = new javax.swing.JPanel();
        panelRealplay = new java.awt.Panel();
        jScrollPanelAlarmList = new javax.swing.JScrollPane();
        jTableAlarm = new javax.swing.JTable();
        jMenuBarConfig = new javax.swing.JMenuBar();
        jMenuConfig = new javax.swing.JMenu();
        jMenuItemBasicConfig = new javax.swing.JMenuItem();
        jMenuItemNetwork = new javax.swing.JMenuItem();
        jMenuItemChannel = new javax.swing.JMenuItem();
        jMenuItemAlarmCfg = new javax.swing.JMenuItem();
        jMenuItemSerialCfg = new javax.swing.JMenuItem();
        jMenuItemUserConfig = new javax.swing.JMenuItem();
        jMenuItemIPAccess = new javax.swing.JMenuItem();
        jMenuPlayBack = new javax.swing.JMenu();
        jMenuItemPlayBackRemote = new javax.swing.JMenuItem();
        jMenuItemPlayTime = new javax.swing.JMenuItem();
        jMenuSetAlarm = new javax.swing.JMenu();
        jRadioButtonMenuSetAlarm = new javax.swing.JRadioButtonMenuItem();
        jRadioButtonMenuListen = new javax.swing.JRadioButtonMenuItem();
        jMenuItemRemoveAlarm = new javax.swing.JMenuItem();
        jMenuManage = new javax.swing.JMenu();
        jMenuItemCheckTime = new javax.swing.JMenuItem();
        jMenuItemFormat = new javax.swing.JMenuItem();
        jMenuItemUpgrade = new javax.swing.JMenuItem();
        jSeparator1 = new javax.swing.JSeparator();
        jMenuItemReboot = new javax.swing.JMenuItem();
        jMenuItemShutDown = new javax.swing.JMenuItem();
        jSeparator2 = new javax.swing.JSeparator();
        jMenuItemDefault = new javax.swing.JMenuItem();
        jMenuItemDeviceState = new javax.swing.JMenuItem();
        jMenuVoice = new javax.swing.JMenu();
        jMenuItemVoiceCom = new javax.swing.JMenuItem();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setTitle("ClientDemo");
        setFont(new java.awt.Font("宋体", 0, 10));

        jSplitPaneHorizontal.setBorder(javax.swing.BorderFactory.createEmptyBorder(1, 1, 1, 1));
        jSplitPaneHorizontal.setDividerLocation(155);
        jSplitPaneHorizontal.setDividerSize(2);

        jPanelUserInfo.setBorder(javax.swing.BorderFactory.createEtchedBorder(new java.awt.Color(204, 255, 255), null));

        jButtonRealPlay.setText("预览");
        jButtonRealPlay.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButtonRealPlayActionPerformed(evt);
            }
        });

        jButtonLogin.setText("注册");
        jButtonLogin.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButtonLoginActionPerformed(evt);
            }
        });

        jLabelUserName.setText("用户名");

        jLabelIPAddress.setText("IP地址");

        jTextFieldPortNumber.setText("8000");

        jTextFieldIPAddress.setText("10.17.36.31");

        jLabelPortNumber.setText("端口");

        jLabelPassWord.setText("密码");

        jPasswordFieldPassword.setText("hik12345");

        jTextFieldUserName.setText("admin");

        jButtonExit.setText("退出");
        jButtonExit.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButtonExitActionPerformed(evt);
            }
        });

        jTreeDevice.setModel(this.initialTreeModel());
        jScrollPaneTree.setViewportView(jTreeDevice);

        jComboBoxCallback.setModel(new javax.swing.DefaultComboBoxModel(new String[]{"直接预览", "回调预览"}));

        javax.swing.GroupLayout jPanelUserInfoLayout = new javax.swing.GroupLayout(jPanelUserInfo);
        jPanelUserInfo.setLayout(jPanelUserInfoLayout);
        jPanelUserInfoLayout.setHorizontalGroup(
                jPanelUserInfoLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                        .addGroup(jPanelUserInfoLayout.createSequentialGroup()
                                .addGap(10, 10, 10)
                                .addComponent(jLabelIPAddress)
                                .addGap(14, 14, 14)
                                .addComponent(jTextFieldIPAddress, javax.swing.GroupLayout.PREFERRED_SIZE, 80, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addGroup(jPanelUserInfoLayout.createSequentialGroup()
                                .addGap(10, 10, 10)
                                .addComponent(jLabelUserName)
                                .addGap(14, 14, 14)
                                .addComponent(jTextFieldUserName, javax.swing.GroupLayout.PREFERRED_SIZE, 80, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addGroup(jPanelUserInfoLayout.createSequentialGroup()
                                .addGap(10, 10, 10)
                                .addComponent(jLabelPassWord)
                                .addGap(26, 26, 26)
                                .addComponent(jPasswordFieldPassword, javax.swing.GroupLayout.PREFERRED_SIZE, 80, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addGroup(jPanelUserInfoLayout.createSequentialGroup()
                                .addContainerGap()
                                .addComponent(jButtonLogin)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                                .addComponent(jButtonRealPlay))
                        .addGroup(jPanelUserInfoLayout.createSequentialGroup()
                                .addGap(10, 10, 10)
                                .addComponent(jLabelPortNumber)
                                .addGap(26, 26, 26)
                                .addComponent(jTextFieldPortNumber, javax.swing.GroupLayout.PREFERRED_SIZE, 80, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanelUserInfoLayout.createSequentialGroup()
                                .addContainerGap(83, Short.MAX_VALUE)
                                .addComponent(jButtonExit)
                                .addContainerGap())
                        .addComponent(jScrollPaneTree, javax.swing.GroupLayout.DEFAULT_SIZE, 150, Short.MAX_VALUE)
                        .addGroup(jPanelUserInfoLayout.createSequentialGroup()
                                .addContainerGap()
                                .addComponent(jComboBoxCallback, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addContainerGap(66, Short.MAX_VALUE))
        );
        jPanelUserInfoLayout.setVerticalGroup(
                jPanelUserInfoLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                        .addGroup(jPanelUserInfoLayout.createSequentialGroup()
                                .addGap(18, 18, 18)
                                .addGroup(jPanelUserInfoLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                        .addComponent(jLabelIPAddress)
                                        .addComponent(jTextFieldIPAddress, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                                .addGap(9, 9, 9)
                                .addGroup(jPanelUserInfoLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                        .addComponent(jLabelUserName)
                                        .addComponent(jTextFieldUserName, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                                .addGap(9, 9, 9)
                                .addGroup(jPanelUserInfoLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                        .addComponent(jLabelPassWord)
                                        .addComponent(jPasswordFieldPassword, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                                .addGap(9, 9, 9)
                                .addGroup(jPanelUserInfoLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                        .addComponent(jLabelPortNumber)
                                        .addComponent(jTextFieldPortNumber, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                                .addGroup(jPanelUserInfoLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                        .addComponent(jButtonLogin)
                                        .addComponent(jButtonRealPlay))
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                                .addComponent(jScrollPaneTree, javax.swing.GroupLayout.PREFERRED_SIZE, 404, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addGap(18, 18, 18)
                                .addComponent(jComboBoxCallback, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addGap(10, 10, 10)
                                .addComponent(jButtonExit)
                                .addContainerGap())
        );

        jSplitPaneHorizontal.setLeftComponent(jPanelUserInfo);

        jSplitPaneVertical.setDividerLocation(579);
        jSplitPaneVertical.setDividerSize(2);
        jSplitPaneVertical.setOrientation(javax.swing.JSplitPane.VERTICAL_SPLIT);

        jPanelRealplayArea.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(153, 255, 102)));

        panelRealplay.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mousePressed(java.awt.event.MouseEvent evt) {
                panelRealplayMousePressed(evt);
            }
        });

        javax.swing.GroupLayout panelRealplayLayout = new javax.swing.GroupLayout(panelRealplay);
        panelRealplay.setLayout(panelRealplayLayout);
        panelRealplayLayout.setHorizontalGroup(
                panelRealplayLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                        .addGap(0, 704, Short.MAX_VALUE)
        );
        panelRealplayLayout.setVerticalGroup(
                panelRealplayLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                        .addGap(0, 576, Short.MAX_VALUE)
        );

        javax.swing.GroupLayout jPanelRealplayAreaLayout = new javax.swing.GroupLayout(jPanelRealplayArea);
        jPanelRealplayArea.setLayout(jPanelRealplayAreaLayout);
        jPanelRealplayAreaLayout.setHorizontalGroup(
                jPanelRealplayAreaLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                        .addComponent(panelRealplay, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
        );
        jPanelRealplayAreaLayout.setVerticalGroup(
                jPanelRealplayAreaLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                        .addComponent(panelRealplay, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
        );

        jSplitPaneVertical.setTopComponent(jPanelRealplayArea);

        jScrollPanelAlarmList.setBorder(javax.swing.BorderFactory.createEtchedBorder());

        jTableAlarm.setModel(this.initialTableModel());
        jScrollPanelAlarmList.setViewportView(jTableAlarm);

        jSplitPaneVertical.setRightComponent(jScrollPanelAlarmList);

        jSplitPaneHorizontal.setRightComponent(jSplitPaneVertical);

        jMenuBarConfig.setBorder(new javax.swing.border.SoftBevelBorder(javax.swing.border.BevelBorder.RAISED));

        jMenuConfig.setText("配置");

        jMenuItemBasicConfig.setText("基本信息");
        jMenuItemBasicConfig.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mousePressed(java.awt.event.MouseEvent evt) {
                jMenuItemBasicConfigMousePressed(evt);
            }
        });
        jMenuConfig.add(jMenuItemBasicConfig);

        jMenuItemNetwork.setText("网络参数");
        jMenuItemNetwork.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mousePressed(java.awt.event.MouseEvent evt) {
                jMenuItemNetworkMousePressed(evt);
            }
        });
        jMenuConfig.add(jMenuItemNetwork);

        jMenuItemChannel.setText("通道参数");
        jMenuItemChannel.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mousePressed(java.awt.event.MouseEvent evt) {
                jMenuItemChannelMousePressed(evt);
            }
        });
        jMenuConfig.add(jMenuItemChannel);

        jMenuItemAlarmCfg.setText("报警参数");
        jMenuItemAlarmCfg.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mousePressed(java.awt.event.MouseEvent evt) {
                jMenuItemAlarmCfgMousePressed(evt);
            }
        });
        jMenuConfig.add(jMenuItemAlarmCfg);

        jMenuItemSerialCfg.setText("串口参数");
        jMenuItemSerialCfg.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mousePressed(java.awt.event.MouseEvent evt) {
                jMenuItemSerialCfgMousePressed(evt);
            }
        });
        jMenuConfig.add(jMenuItemSerialCfg);

        jMenuItemUserConfig.setText("用户配置");
        jMenuItemUserConfig.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mousePressed(java.awt.event.MouseEvent evt) {
                jMenuItemUserConfigMousePressed(evt);
            }
        });
        jMenuConfig.add(jMenuItemUserConfig);

        jMenuItemIPAccess.setText("IP接入配置");
        jMenuItemIPAccess.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jMenuItemIPAccessActionPerformed(evt);
            }
        });
        jMenuConfig.add(jMenuItemIPAccess);

        jMenuBarConfig.add(jMenuConfig);

        jMenuPlayBack.setText("回放");

        jMenuItemPlayBackRemote.setText("按文件");
        jMenuItemPlayBackRemote.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mousePressed(java.awt.event.MouseEvent evt) {
                jMenuItemPlayBackRemoteMousePressed(evt);
            }
        });
        jMenuPlayBack.add(jMenuItemPlayBackRemote);

        jMenuItemPlayTime.setText("按时间");
        jMenuItemPlayTime.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mousePressed(java.awt.event.MouseEvent evt) {
                jMenuItemPlayTimeMousePressed(evt);
            }
        });
        jMenuPlayBack.add(jMenuItemPlayTime);

        jMenuBarConfig.add(jMenuPlayBack);

        jMenuSetAlarm.setBorder(null);
        jMenuSetAlarm.setText("报警");

        jRadioButtonMenuSetAlarm.setText("〇布防");
        jRadioButtonMenuSetAlarm.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jRadioButtonMenuSetAlarmActionPerformed(evt);
            }
        });
        jMenuSetAlarm.add(jRadioButtonMenuSetAlarm);

        jRadioButtonMenuListen.setText("〇监听");
        jRadioButtonMenuListen.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jRadioButtonMenuListenActionPerformed(evt);
            }
        });
        jMenuSetAlarm.add(jRadioButtonMenuListen);

        jMenuItemRemoveAlarm.setText("清空报警信息");
        jMenuItemRemoveAlarm.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mousePressed(java.awt.event.MouseEvent evt) {
                jMenuItemRemoveAlarmMousePressed(evt);
            }
        });
        jMenuSetAlarm.add(jMenuItemRemoveAlarm);

        jMenuBarConfig.add(jMenuSetAlarm);

        jMenuManage.setText("管理");

        jMenuItemCheckTime.setText("校时");
        jMenuItemCheckTime.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mousePressed(java.awt.event.MouseEvent evt) {
                jMenuItemCheckTimeMousePressed(evt);
            }
        });
        jMenuManage.add(jMenuItemCheckTime);

        jMenuItemFormat.setText("格式化");
        jMenuItemFormat.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mousePressed(java.awt.event.MouseEvent evt) {
                jMenuItemFormatMousePressed(evt);
            }
        });
        jMenuManage.add(jMenuItemFormat);

        jMenuItemUpgrade.setText("升级");
        jMenuItemUpgrade.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mousePressed(java.awt.event.MouseEvent evt) {
                jMenuItemUpgradeMousePressed(evt);
            }
        });
        jMenuManage.add(jMenuItemUpgrade);
        jMenuManage.add(jSeparator1);

        jMenuItemReboot.setText("重启");
        jMenuItemReboot.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mousePressed(java.awt.event.MouseEvent evt) {
                jMenuItemRebootMousePressed(evt);
            }
        });
        jMenuManage.add(jMenuItemReboot);

        jMenuItemShutDown.setText("关闭");
        jMenuItemShutDown.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mousePressed(java.awt.event.MouseEvent evt) {
                jMenuItemShutDownMousePressed(evt);
            }
        });
        jMenuManage.add(jMenuItemShutDown);
        jMenuManage.add(jSeparator2);

        jMenuItemDefault.setText("恢复默认参数");
        jMenuItemDefault.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mousePressed(java.awt.event.MouseEvent evt) {
                jMenuItemDefaultMousePressed(evt);
            }
        });
        jMenuManage.add(jMenuItemDefault);

        jMenuItemDeviceState.setText("设备状态");
        jMenuItemDeviceState.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mousePressed(java.awt.event.MouseEvent evt) {
                jMenuItemDeviceStateMousePressed(evt);
            }
        });
        jMenuManage.add(jMenuItemDeviceState);

        jMenuBarConfig.add(jMenuManage);

        jMenuVoice.setText("语音");

        jMenuItemVoiceCom.setText("语音对讲");
        jMenuItemVoiceCom.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mousePressed(java.awt.event.MouseEvent evt) {
                jMenuItemVoiceComMousePressed(evt);
            }
        });
        jMenuVoice.add(jMenuItemVoiceCom);

        jMenuBarConfig.add(jMenuVoice);

        setJMenuBar(jMenuBarConfig);

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
                layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                        .addComponent(jSplitPaneHorizontal, javax.swing.GroupLayout.DEFAULT_SIZE, 866, Short.MAX_VALUE)
        );
        layout.setVerticalGroup(
                layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                        .addComponent(jSplitPaneHorizontal, javax.swing.GroupLayout.DEFAULT_SIZE, 671, Short.MAX_VALUE)
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    /*************************************************
     * 函数:      "注册"  按钮单击响应函数
     * 函数描述:	注册登录设备
     *************************************************/
    private void jButtonLoginActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButtonLoginActionPerformed
        //注册之前先注销已注册的用户,预览情况下不可注销
        if (bRealPlay) {
            JOptionPane.showMessageDialog(this, "注册新用户请先停止当前预览!");
            return;
        }

        if (lUserID > -1) {
            //先注销
            hCNetSDK.NET_DVR_Logout_V30(lUserID);
            lUserID = -1;
            m_iTreeNodeNum = 0;
            m_DeviceRoot.removeAllChildren();
        }
        //注册
        m_sDeviceIP = jTextFieldIPAddress.getText();//设备ip地址
        m_strDeviceInfo = new HCNetSDK.NET_DVR_DEVICEINFO_V30();
        int iPort = Integer.parseInt(jTextFieldPortNumber.getText());
        lUserID = hCNetSDK.NET_DVR_Login_V30(m_sDeviceIP,
                (short) iPort, jTextFieldUserName.getText(), new String(jPasswordFieldPassword.getPassword()), m_strDeviceInfo);

        long userID = lUserID;
        if (userID == -1) {
            m_sDeviceIP = "";//登录未成功,IP置为空
            int error;
            error=hCNetSDK.NET_DVR_GetLastError();
            JOptionPane.showMessageDialog(ClientDemo.this, "注册失败,错误码："+error);

        } else {
            JOptionPane.showMessageDialog(ClientDemo.this, "注册成功");
            CreateDeviceTree();
        }
    }//GEN-LAST:event_jButtonLoginActionPerformed

    /*************************************************
     * 函数:      initialTableModel
     * 函数描述:	初始化报警信息列表,写入列名称
     *************************************************/
    public DefaultTableModel initialTableModel() {
        String tabeTile[];
        tabeTile = new String[]{"时间", "报警信息", "设备信息"};
        DefaultTableModel alarmTableModel = new DefaultTableModel(tabeTile, 0);
        return alarmTableModel;
    }

    /*************************************************
     * 函数:      initialTreeModel
     * 函数描述:  初始化设备树
     *************************************************/
    private DefaultTreeModel initialTreeModel() {
        m_DeviceRoot = new DefaultMutableTreeNode("Device");
        DefaultTreeModel myDefaultTreeModel = new DefaultTreeModel(m_DeviceRoot);//使用根节点创建模型
        return myDefaultTreeModel;
    }

    /*************************************************
     * 函数:      " 退出"  按钮响应函数
     * 函数描述:	注销并退出
     *************************************************/
    private void jButtonExitActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButtonExitActionPerformed
        //如果在预览,先停止预览, 释放句柄
        if (lPreviewHandle > -1) {
            hCNetSDK.NET_DVR_StopRealPlay(lPreviewHandle);
            if (framePTZControl != null) {
                framePTZControl.dispose();
            }
        }

        //报警撤防
        if (lAlarmHandle != -1) {
            hCNetSDK.NET_DVR_CloseAlarmChan_V30(lAlarmHandle);
        }
        //停止监听
        if (lListenHandle != -1) {
            hCNetSDK.NET_DVR_StopListen_V30(lListenHandle);
        }

        //如果已经注册,注销
        if (lUserID > -1) {
            hCNetSDK.NET_DVR_Logout_V30(lUserID);
        }
        //cleanup SDK
        hCNetSDK.NET_DVR_Cleanup();
        this.dispose();
    }//GEN-LAST:event_jButtonExitActionPerformed

    /*************************************************
     * 函数:       "清空报警信息"  菜单项响应函数
     * 函数描述:	单击清空信息列表
     *************************************************/
    private void jMenuItemRemoveAlarmMousePressed(java.awt.event.MouseEvent evt)//GEN-FIRST:event_jMenuItemRemoveAlarmMousePressed
    {//GEN-HEADEREND:event_jMenuItemRemoveAlarmMousePressed
        //删除所有行
        ((DefaultTableModel) jTableAlarm.getModel()).getDataVector().removeAllElements();
        //把改变显示到列表控件
        ((DefaultTableModel) jTableAlarm.getModel()).fireTableStructureChanged();
    }//GEN-LAST:event_jMenuItemRemoveAlarmMousePressed

    /*************************************************
     * 函数:      "报警监听"  菜单项响应函数
     * 函数描述:   选中开始监听,取消结束监听
     *************************************************/
    private void jRadioButtonMenuListenActionPerformed(java.awt.event.ActionEvent evt)//GEN-FIRST:event_jRadioButtonMenuListenActionPerformed
    {//GEN-HEADEREND:event_jRadioButtonMenuListenActionPerformed
        if (jRadioButtonMenuListen.isSelected() == true)//选择监听
        {
            if (lListenHandle == -1)
            //尚未监听,开始监听
            {
                if (fMSFCallBack == null) {
                    fMSFCallBack = new FMSGCallBack();
                }
                Pointer pUser = null;
                if (!hCNetSDK.NET_DVR_SetDVRMessageCallBack_V31(fMSFCallBack, pUser)) {
                    System.out.println("设置回调函数失败!");
                }

                //本地IP地址置为null时自动获取本地IP
                lListenHandle = hCNetSDK.NET_DVR_StartListen_V30(null, (short) 7200, fMSFCallBack, null);
                if (lListenHandle == -1) {
                    JOptionPane.showMessageDialog(this, "开始监听失败");
                    jRadioButtonMenuListen.setSelected(false);
                }
            }
        } else
        //停止监听
        {
            if (lListenHandle != -1) {
                if (!hCNetSDK.NET_DVR_StopListen_V30(lListenHandle)) {
                    JOptionPane.showMessageDialog(this, "停止监听失败");
                    jRadioButtonMenuListen.setSelected(true);
                    lListenHandle = -1;
                } else {
                    lListenHandle = -1;
                }
            }
        }
    }//GEN-LAST:event_jRadioButtonMenuListenActionPerformed

    /*************************************************
     * 函数:      "报警布防"  菜单项响应函数
     * 函数描述:	选中布防听,取消撤防
     *************************************************/
    private void jRadioButtonMenuSetAlarmActionPerformed(java.awt.event.ActionEvent evt)//GEN-FIRST:event_jRadioButtonMenuSetAlarmActionPerformed
    {//GEN-HEADEREND:event_jRadioButtonMenuSetAlarmActionPerformed
        if (lUserID == -1) {
            JOptionPane.showMessageDialog(this, "请先注册");
            return;
        }

        if (jRadioButtonMenuSetAlarm.isSelected() == true)
        //已选择布防
        {
            if (lAlarmHandle == -1)//尚未布防,需要布防
            {
                if (fMSFCallBack == null) {
                    fMSFCallBack = new FMSGCallBack();
                    Pointer pUser = null;
                    if (!hCNetSDK.NET_DVR_SetDVRMessageCallBack_V31(fMSFCallBack, pUser)) {
                        System.out.println("设置回调函数失败!");
                    }
                }
                lAlarmHandle = hCNetSDK.NET_DVR_SetupAlarmChan_V30(lUserID);
                if (lAlarmHandle == -1) {
                    JOptionPane.showMessageDialog(this, "布防失败");
                    jRadioButtonMenuSetAlarm.setSelected(false);
                }
            }
        } else
        //未选择布防
        {
            if (lAlarmHandle != -1) {
                if (!hCNetSDK.NET_DVR_CloseAlarmChan_V30(lAlarmHandle)) {
                    JOptionPane.showMessageDialog(this, "撤防失败");
                    jRadioButtonMenuSetAlarm.setSelected(true);
                    lAlarmHandle = -1;
                } else {
                    lAlarmHandle = -1;
                }
            }
        }
    }//GEN-LAST:event_jRadioButtonMenuSetAlarmActionPerformed

    /*************************************************
     * 函数:      "预览"  按钮单击响应函数
     * 函数描述:	获取通道号,打开播放窗口,开始此通道的预览
     *************************************************/
    private void jButtonRealPlayActionPerformed(java.awt.event.ActionEvent evt)//GEN-FIRST:event_jButtonRealPlayActionPerformed
    {//GEN-HEADEREND:event_jButtonRealPlayActionPerformed
//        System.out.println(panelRealplay.getWidth());
//        System.out.println(panelRealplay.getHeight());
        if (lUserID == -1) {
            JOptionPane.showMessageDialog(this, "请先注册");
            return;
        }

        //如果预览窗口没打开,不在预览
        if (bRealPlay == false) {
            //获取窗口句柄
            HWND hwnd = new HWND(Native.getComponentPointer(panelRealplay));

            //获取通道号
            int iChannelNum = getChannelNumber();//通道号
            if (iChannelNum == -1) {
                JOptionPane.showMessageDialog(this, "请选择要预览的通道");
                return;
            }

//            m_strClientInfo = new HCNetSDK.NET_DVR_CLIENTINFO();
//            m_strClientInfo.lChannel = new NativeLong(iChannelNum);
            HCNetSDK.NET_DVR_PREVIEWINFO strClientInfo = new HCNetSDK.NET_DVR_PREVIEWINFO();
            strClientInfo.read();
//            strClientInfo.hPlayWnd = null;  //窗口句柄，从回调取流不显示一般设置为空
            strClientInfo.lChannel = iChannelNum;  //通道号
            strClientInfo.dwStreamType=0; //0-主码流，1-子码流，2-三码流，3-虚拟码流，以此类推
            strClientInfo.dwLinkMode=0; //连接方式：0- TCP方式，1- UDP方式，2- 多播方式，3- RTP方式，4- RTP/RTSP，5- RTP/HTTP，6- HRUDP（可靠传输） ，7- RTSP/HTTPS，8- NPQ
            strClientInfo.bBlocked=1;  //0- 非阻塞取流，1- 阻塞取流

            //在此判断是否回调预览,0,不回调 1 回调
            if (jComboBoxCallback.getSelectedIndex() == 0) {
                strClientInfo.hPlayWnd = hwnd;
                strClientInfo.write();
                lPreviewHandle = hCNetSDK.NET_DVR_RealPlay_V40(lUserID, strClientInfo, null , null);
                if (lPreviewHandle<=-1)
                {
                    int error;
                    error=hCNetSDK.NET_DVR_GetLastError();
                    JOptionPane.showMessageDialog(ClientDemo.this, "预览失败,错误码："+error);
                    return;

                }

                try {
                    Thread.sleep(2000);
                } catch (InterruptedException e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                }


                String path = ".\\pic\\savevideo.mp4";
                Boolean bSaveVideo = hCNetSDK.NET_DVR_SaveRealData_V30(lPreviewHandle,0x2,path);
                if (bSaveVideo == false) {
                    int iErr = hCNetSDK.NET_DVR_GetLastError();
                    System.out.println("保存录像失败" + iErr);
                    return;
                }
                System.out.println("保存录像成功");




            } else if (jComboBoxCallback.getSelectedIndex() == 1) {
                strClientInfo.hPlayWnd = null;
                strClientInfo.write();
                lPreviewHandle = hCNetSDK.NET_DVR_RealPlay_V40(lUserID,
                        strClientInfo, fRealDataCallBack, null);
                if (lPreviewHandle<=-1)
                {
                    int error;
                    error=hCNetSDK.NET_DVR_GetLastError();
                    JOptionPane.showMessageDialog(ClientDemo.this, "预览失败,错误码："+error);
                    return;
                }
            }

            long previewSucValue = lPreviewHandle;

            //预览失败时:
            if (previewSucValue == -1) {
                int error;
                error=hCNetSDK.NET_DVR_GetLastError();
                JOptionPane.showMessageDialog(this, "预览失败,错误码："+error);
                return;
            }

            //预览成功的操作
            jButtonRealPlay.setText("停止");
            bRealPlay = true;

            //显示云台控制窗口
            framePTZControl = new JFramePTZControl(lPreviewHandle);
            framePTZControl.setLocation(this.getX() + this.getWidth(), this.getY());
            framePTZControl.setVisible(true);
        }

        //如果在预览,停止预览,关闭窗口
        else {

            Boolean bStopSaveVideo = hCNetSDK.NET_DVR_StopSaveRealData(lPreviewHandle);
            if (bStopSaveVideo == false) {
                int iErr = hCNetSDK.NET_DVR_GetLastError();
                System.out.println("停止录像失败" + iErr);
                return;
            }
            System.out.println("停止录像成功");


            hCNetSDK.NET_DVR_StopRealPlay(lPreviewHandle);
            jButtonRealPlay.setText("预览");
            bRealPlay = false;
            if (m_lPort.getValue() != -1) {
                playControl.PlayM4_Stop(m_lPort.getValue());
                m_lPort.setValue(-1);
            }
            framePTZControl.dispose();

            panelRealplay.repaint();
        }
    }//GEN-LAST:event_jButtonRealPlayActionPerformed

    /*************************************************
     * 函数:       "串口"  菜单项响应函数
     * 函数描述:	新建窗体显示串口
     *************************************************/
    private void jMenuItemSerialCfgMousePressed(java.awt.event.MouseEvent evt)//GEN-FIRST:event_jMenuItemSerialCfgMousePressed
    {//GEN-HEADEREND:event_jMenuItemSerialCfgMousePressed
        if (lUserID == -1) {
            JOptionPane.showMessageDialog(this, "请先注册");
            return;
        }
        //打开JDialog
        JDialogSerialCfg dlgSerialCfg = new JDialogSerialCfg(this, false, lUserID, m_strDeviceInfo);
        centerWindow(dlgSerialCfg);
        dlgSerialCfg.setVisible(true);
    }//GEN-LAST:event_jMenuItemSerialCfgMousePressed

    /*************************************************
     * 函数:       "报警参数"  菜单项响应函数
     * 函数描述:	新建窗体显示报警参数
     *************************************************/
    private void jMenuItemAlarmCfgMousePressed(java.awt.event.MouseEvent evt)//GEN-FIRST:event_jMenuItemAlarmCfgMousePressed
    {//GEN-HEADEREND:event_jMenuItemAlarmCfgMousePressed
        if (lUserID == -1) {
            JOptionPane.showMessageDialog(this, "请先注册");
            return;
        }
        //打开JDialog
        JDialogAlarmCfg dlgAlarmCfg = new JDialogAlarmCfg(this, false, lUserID, m_strDeviceInfo);
        dlgAlarmCfg.setLocation(this.getX(), this.getY());
        dlgAlarmCfg.setVisible(true);
    }//GEN-LAST:event_jMenuItemAlarmCfgMousePressed

    /*************************************************
     * 函数:  "通道配置"  菜单项响应函数
     * 函数描述:点击显示通道参数配置
     *************************************************/
    private void jMenuItemChannelMousePressed(java.awt.event.MouseEvent evt)//GEN-FIRST:event_jMenuItemChannelMousePressed
    {//GEN-HEADEREND:event_jMenuItemChannelMousePressed
        if (lUserID == -1) {
            JOptionPane.showMessageDialog(this, "请先注册");
            return;
        }
        //打开JDialog
        JDialogChannelConfig dialogChannelConfig = new JDialogChannelConfig(this, false, lUserID, m_strDeviceInfo);//无模式对话框
        dialogChannelConfig.setBounds(this.getX(), this.getY(), 670, 640);
        dialogChannelConfig.setVisible(true);
        //传参数
        int iStartChan = m_strDeviceInfo.byStartChan;
        int iChannum = m_strDeviceInfo.byChanNum;
        //初始化通道数组合框
        for (int i = 0; i < iChannum; i++) {
            dialogChannelConfig.jComboBoxChannelNumber.addItem("Camera" + (i + iStartChan));
        }
        for (int i = 0; i < HCNetSDK.MAX_IP_CHANNEL; i++) {
            if (m_strIpparaCfg.struIPChanInfo[i].byEnable == 1) {
                dialogChannelConfig.jComboBoxChannelNumber.addItem("IPCamara" + (i + iStartChan));
            }
        }
    }//GEN-LAST:event_jMenuItemChannelMousePressed

    /*************************************************
     * 函数:      "网络参数"  菜单项响应函数
     * 函数描述:	点击显示网络参数配置
     *************************************************/
    private void jMenuItemNetworkMousePressed(java.awt.event.MouseEvent evt)//GEN-FIRST:event_jMenuItemNetworkMousePressed
    {//GEN-HEADEREND:event_jMenuItemNetworkMousePressed
        if (lUserID == -1) {
            JOptionPane.showMessageDialog(this, "请先注册");
            return;
        }
        //打开Jframe
        JFrameNetWorkConfig frameNetwork = new JFrameNetWorkConfig(lUserID);
        frameNetwork.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        frameNetwork.setSize(550, 380);
        centerWindow(frameNetwork);
        frameNetwork.setVisible(true);
    }//GEN-LAST:event_jMenuItemNetworkMousePressed

    /*************************************************
     * 函数:       "基本信息"  菜单项响应函数
     * 函数描述:	新建窗体,显示设备基本信息
     *************************************************/
    private void jMenuItemBasicConfigMousePressed(java.awt.event.MouseEvent evt)//GEN-FIRST:event_jMenuItemBasicConfigMousePressed
    {//GEN-HEADEREND:event_jMenuItemBasicConfigMousePressed
        if (lUserID == -1) {
            JOptionPane.showMessageDialog(this, "请先注册");
            return;
        }
        //打开JDialog
        JDialogBasicConfig dlgBasicConfig = new JDialogBasicConfig(this, false, lUserID);
        dlgBasicConfig.setSize(507, 400);
        centerWindow(dlgBasicConfig);
        dlgBasicConfig.setVisible(true);
    }//GEN-LAST:event_jMenuItemBasicConfigMousePressed

    /*************************************************
     * 函数:       "设备状态"  菜单项响应函数
     * 函数描述:	新建窗体显示设备状态
     *************************************************/
    private void jMenuItemDeviceStateMousePressed(java.awt.event.MouseEvent evt)//GEN-FIRST:event_jMenuItemDeviceStateMousePressed
    {//GEN-HEADEREND:event_jMenuItemDeviceStateMousePressed
        if (lUserID == -1) {
            JOptionPane.showMessageDialog(this, "请先注册");
            return;
        }
        //打开JDialog
        JDialogDeviceState dlgDeviceState = new JDialogDeviceState(this, false, lUserID, m_strDeviceInfo, m_sDeviceIP);
        dlgDeviceState.setSize(680, 715);
        centerWindow(dlgDeviceState);
        dlgDeviceState.setVisible(true);
    }//GEN-LAST:event_jMenuItemDeviceStateMousePressed

    /*************************************************
     * 函数:       "恢复默认参数"  菜单项响应函数
     * 函数描述:	弹出确认框,是否恢复默认参数
     *************************************************/
    private void jMenuItemDefaultMousePressed(java.awt.event.MouseEvent evt)//GEN-FIRST:event_jMenuItemDefaultMousePressed
    {//GEN-HEADEREND:event_jMenuItemDefaultMousePressed
        if (lUserID == -1) {
            JOptionPane.showMessageDialog(this, "请先注册");
            return;
        }

        int iResponse = JOptionPane.showConfirmDialog(this, "确定恢复默认参数?", "恢复默认参数", JOptionPane.OK_CANCEL_OPTION);
        if (iResponse == 0)
        //确认
        {
            if (!hCNetSDK.NET_DVR_RestoreConfig(lUserID)) {
                JOptionPane.showMessageDialog(this, "恢复默认参数失败");
                return;
            }
        }
        if (iResponse == 2)
        //取消
        {
            return;
        }
    }//GEN-LAST:event_jMenuItemDefaultMousePressed

    /*************************************************
     * 函数:       "关闭"  菜单项响应函数
     * 函数描述:	弹出确认框询问是否关机
     *************************************************/
    private void jMenuItemShutDownMousePressed(java.awt.event.MouseEvent evt)//GEN-FIRST:event_jMenuItemShutDownMousePressed
    {//GEN-HEADEREND:event_jMenuItemShutDownMousePressed
        if (lUserID == -1) {
            JOptionPane.showMessageDialog(this, "请先注册");
            return;
        }

        int iResponse = JOptionPane.showConfirmDialog(this, "确定关闭设备?", "关机", JOptionPane.OK_CANCEL_OPTION);
        //确认
        if (iResponse == 0) {
            if (!hCNetSDK.NET_DVR_ShutDownDVR(lUserID)) {
                JOptionPane.showMessageDialog(this, "关闭设备失败");
                return;
            }
        }
        //取消
        if (iResponse == 2) {
            return;
        }
    }//GEN-LAST:event_jMenuItemShutDownMousePressed

    /*************************************************
     * 函数:       "重启"  菜单项响应函数
     * 函数描述:	弹出确认框询问是否重启设备
     *************************************************/
    private void jMenuItemRebootMousePressed(java.awt.event.MouseEvent evt)//GEN-FIRST:event_jMenuItemRebootMousePressed
    {//GEN-HEADEREND:event_jMenuItemRebootMousePressed
        if (lUserID == -1) {
            JOptionPane.showMessageDialog(this, "请先注册");
            return;
        }

        int iResponse = JOptionPane.showConfirmDialog(this, "确定重启设备?", "重启", JOptionPane.OK_CANCEL_OPTION);
        //确认
        if (iResponse == 0) {
            if (!hCNetSDK.NET_DVR_RebootDVR(lUserID)) {
                JOptionPane.showMessageDialog(this, "设备重启失败");
                return;
            }
        }
        //取消
        if (iResponse == 2) {
            return;
        }
    }//GEN-LAST:event_jMenuItemRebootMousePressed

    /*************************************************
     * 函数:       "升级"  菜单项响应函数
     * 函数描述:	新建窗体,显示升级选项
     *************************************************/
    private void jMenuItemUpgradeMousePressed(java.awt.event.MouseEvent evt)//GEN-FIRST:event_jMenuItemUpgradeMousePressed
    {//GEN-HEADEREND:event_jMenuItemUpgradeMousePressed
        if (lUserID == -1) {
            JOptionPane.showMessageDialog(this, "请先注册");
            return;
        }
        //打开JDialog
        JDialogUpGrade dlgUpgrade = new JDialogUpGrade(this, false, lUserID);
        dlgUpgrade.setSize(440, 265);
        centerWindow(dlgUpgrade);
        dlgUpgrade.setVisible(true);
    }//GEN-LAST:event_jMenuItemUpgradeMousePressed

    /*************************************************
     * 函数:       "格式化"  菜单项响应函数
     * 函数描述:	新建窗体,显示格式化选项
     *************************************************/
    private void jMenuItemFormatMousePressed(java.awt.event.MouseEvent evt)//GEN-FIRST:event_jMenuItemFormatMousePressed
    {//GEN-HEADEREND:event_jMenuItemFormatMousePressed
        if (lUserID == -1) {
            JOptionPane.showMessageDialog(this, "请先注册");
            return;
        }
        //打开JDialog
        JDialogFormatDisk dlgFormatDisk = new JDialogFormatDisk(this, false, lUserID);
        centerWindow(dlgFormatDisk);
        dlgFormatDisk.setVisible(true);
    }//GEN-LAST:event_jMenuItemFormatMousePressed

    /*************************************************
     * 函数:       "校时"  菜单项响应函数
     * 函数描述:	新建窗体,显示校时选项
     *************************************************/
    private void jMenuItemCheckTimeMousePressed(java.awt.event.MouseEvent evt)//GEN-FIRST:event_jMenuItemCheckTimeMousePressed
    {//GEN-HEADEREND:event_jMenuItemCheckTimeMousePressed
        if (lUserID == -1) {
            JOptionPane.showMessageDialog(this, "请先注册");
            return;
        }
        //打开JDialog
        JDialogCheckTime dlgCheckTime = new JDialogCheckTime(this, false, lUserID);
        centerWindow(dlgCheckTime);
        dlgCheckTime.setVisible(true);
    }//GEN-LAST:event_jMenuItemCheckTimeMousePressed

    /*************************************************
     * 函数:       "时间回放"  菜单项响应函数
     * 函数描述:	新建窗体时间回放
     *************************************************/
    private void jMenuItemPlayTimeMousePressed(java.awt.event.MouseEvent evt)//GEN-FIRST:event_jMenuItemPlayTimeMousePressed
    {//GEN-HEADEREND:event_jMenuItemPlayTimeMousePressed
        if (lUserID == -1) {
            JOptionPane.showMessageDialog(this, "请先注册");
            return;
        }
        //打开JDialog
        JDialogPlayBackByTime dlgPlayTime = new JDialogPlayBackByTime(this, false, lUserID, m_sDeviceIP);
        centerWindow(dlgPlayTime);
        dlgPlayTime.setVisible(true);
    }//GEN-LAST:event_jMenuItemPlayTimeMousePressed

    /*************************************************
     * 函数:      "回放"  按文件  菜单项响应函数
     * 函数描述:	点击打开回放界面
     *************************************************/
    private void jMenuItemPlayBackRemoteMousePressed(java.awt.event.MouseEvent evt)//GEN-FIRST:event_jMenuItemPlayBackRemoteMousePressed
    {//GEN-HEADEREND:event_jMenuItemPlayBackRemoteMousePressed
        if (lUserID == -1) {
            JOptionPane.showMessageDialog(this, "请先注册");
            return;
        }
        //打开JDialog
        //无模式对话框
        JDialogPlayBack dialogPlayBack = new JDialogPlayBack(this, false, lUserID);
        dialogPlayBack.setBounds(this.getX(), this.getY(), 730, 650);
        centerWindow(dialogPlayBack);
        dialogPlayBack.setVisible(true);
    }//GEN-LAST:event_jMenuItemPlayBackRemoteMousePressed

    /*************************************************
     * 函数:      "用户配置"  按文件  菜单项响应函数
     * 函数描述:   点击打开对话框,开始用户配置
     *************************************************/
    private void jMenuItemUserConfigMousePressed(java.awt.event.MouseEvent evt)//GEN-FIRST:event_jMenuItemUserConfigMousePressed
    {//GEN-HEADEREND:event_jMenuItemUserConfigMousePressed
        if (lUserID == -1) {
            JOptionPane.showMessageDialog(this, "请先注册");
            return;
        }

        JDialogUserConfig dlgUserConfig = new JDialogUserConfig(this, false, lUserID, m_strDeviceInfo, m_strIpparaCfg);
        centerWindow(dlgUserConfig);
        dlgUserConfig.setVisible(true);
    }//GEN-LAST:event_jMenuItemUserConfigMousePressed

    /*************************************************
     * 函数:      "语音对讲"  按文件  菜单项响应函数
     * 函数描述:   点击打开对话框,开始语音对讲相关操作
     *************************************************/
    private void jMenuItemVoiceComMousePressed(java.awt.event.MouseEvent evt)//GEN-FIRST:event_jMenuItemVoiceComMousePressed
    {//GEN-HEADEREND:event_jMenuItemVoiceComMousePressed
        if (lUserID == -1) {
            JOptionPane.showMessageDialog(this, "请先注册");
            return;
        }

        JDialogVoiceTalk dlgVoiceTalk = new JDialogVoiceTalk(this, false, lUserID, m_strDeviceInfo);
        centerWindow(dlgVoiceTalk);
        dlgVoiceTalk.setVisible(true);
    }//GEN-LAST:event_jMenuItemVoiceComMousePressed


    /*************************************************
     * 函数:      "Ip接入"  按文件  菜单项响应函数
     * 函数描述:   点击打开对话框,IP接入配置
     *************************************************/
    private void jMenuItemIPAccessActionPerformed(java.awt.event.ActionEvent evt)//GEN-FIRST:event_jMenuItemIPAccessActionPerformed
    {//GEN-HEADEREND:event_jMenuItemIPAccessActionPerformed
        if (lUserID == -1) {
            JOptionPane.showMessageDialog(this, "请先注册");
            return;
        }

        JDialogIPAccessCfg dlgIPAccess = new JDialogIPAccessCfg(this, false, lUserID, m_strDeviceInfo);
        centerWindow(dlgIPAccess);
        dlgIPAccess.setVisible(true);
    }//GEN-LAST:event_jMenuItemIPAccessActionPerformed

    /*************************************************
     * 函数:      "播放窗口"  双击响应函数
     * 函数描述:   双击全屏预览当前预览通道
     *************************************************/
    private void panelRealplayMousePressed(java.awt.event.MouseEvent evt)//GEN-FIRST:event_panelRealplayMousePressed
    {//GEN-HEADEREND:event_panelRealplayMousePressed
        if (!bRealPlay) {
            return;
        }
        //鼠标单击事件为双击
        if (evt.getClickCount() == 2) {
            //新建JWindow 全屏预览
            final JWindow wnd = new JWindow();
            //获取屏幕尺寸
            Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
            wnd.setSize(screenSize);
            wnd.setVisible(true);

            final HWND hwnd = new HWND(Native.getComponentPointer(wnd));
            m_strClientInfo.hPlayWnd = hwnd;
            final int lRealHandle = hCNetSDK.NET_DVR_RealPlay_V30(lUserID,
                    m_strClientInfo, null, null, true);

            //JWindow增加双击响应函数,双击时停止预览,退出全屏
            wnd.addMouseListener(new java.awt.event.MouseAdapter() {
                public void mousePressed(java.awt.event.MouseEvent evt) {
                    if (evt.getClickCount() == 2) {
                        //停止预览
                        hCNetSDK.NET_DVR_StopRealPlay(lRealHandle);
                        wnd.dispose();
                    }
                }
            });

        }
    }//GEN-LAST:event_panelRealplayMousePressed

    /*************************************************
     * 函数:    centerWindow
     * 函数描述:窗口置中
     *************************************************/
    public static void centerWindow(Container window) {
        Dimension dim = Toolkit.getDefaultToolkit().getScreenSize();
        int w = window.getSize().width;
        int h = window.getSize().height;
        int x = (dim.width - w) / 2;
        int y = (dim.height - h) / 2;
        window.setLocation(x, y);
    }

    /*************************************************
     * 函数:    CreateDeviceTree
     * 函数描述:建立设备通道树
     *************************************************/
    private void CreateDeviceTree() {
        //ibrBytesReturned 实际收到的数据长度指针，不能为NULL
        int maxIpChannelNum ;
        if (m_strDeviceInfo.byHighDChanNum == 0)
        {
            maxIpChannelNum = m_strDeviceInfo.byIPChanNum & 0xff;
            System.out.println("设备数组通道总数："+maxIpChannelNum);
        }else
        {
            maxIpChannelNum = (int)((m_strDeviceInfo.byHighDChanNum & 0xff) << 8);
            System.out.println("设备数组通道总数："+maxIpChannelNum);
        }
        int group = maxIpChannelNum/64<=0?0:maxIpChannelNum/64-1;
        System.out.println(group);
        for (int i=0;i<=group;i++)
        {
            IntByReference ibrBytesReturned = new IntByReference(0);//获取IP接入配置参数
            boolean bRet;

            HCNetSDK.NET_DVR_IPPARACFG_V40 m_strIpparaCfg = new HCNetSDK.NET_DVR_IPPARACFG_V40();
            m_strIpparaCfg.write();
            //lpIpParaConfig 接收数据的缓冲指针
            Pointer lpIpParaConfig = m_strIpparaCfg.getPointer();
            bRet = hCNetSDK.NET_DVR_GetDVRConfig(lUserID, HCNetSDK.NET_DVR_GET_IPPARACFG_V40, i, lpIpParaConfig, m_strIpparaCfg.size(), ibrBytesReturned);
            m_strIpparaCfg.read();

            DefaultTreeModel TreeModel = ((DefaultTreeModel) jTreeDevice.getModel());//获取树模型
            DefaultTreeCellRenderer render = new DefaultTreeCellRenderer();
            if (!bRet) {
                //设备不支持,则表示没有IP通道
                for (int iChannum = 0; iChannum < m_strDeviceInfo.byChanNum; iChannum++) {
                    DefaultMutableTreeNode newNode = new DefaultMutableTreeNode("Camera" + (iChannum + m_strDeviceInfo.byStartChan));
                    TreeModel.insertNodeInto(newNode, m_DeviceRoot, iChannum);
                }
            } else {
                //设备支持IP通道
                for (int iChannum = 0; iChannum < ((maxIpChannelNum<=32)?32:64); iChannum++) {
                    m_strIpparaCfg.struStreamMode[iChannum].read();
                    if (m_strIpparaCfg.struStreamMode[iChannum].byGetStreamType == 0) {
                        m_strIpparaCfg.struStreamMode[iChannum].uGetStream.setType(HCNetSDK.NET_DVR_IPCHANINFO.class);
                        m_strIpparaCfg.struStreamMode[iChannum].uGetStream.struChanInfo.read();
                        if (m_strIpparaCfg.struStreamMode[iChannum].uGetStream.struChanInfo.byEnable == 1) {
                            DefaultMutableTreeNode newNode = new DefaultMutableTreeNode("IPCamera_" + (iChannum+64*i + m_strDeviceInfo.byStartDChan)+"_在线");
                            TreeModel.insertNodeInto(newNode, m_DeviceRoot, m_iTreeNodeNum);
                            render.setTextNonSelectionColor(Color.BLUE);
                            jTreeDevice.setCellRenderer(render);
                            m_iTreeNodeNum++;
                        }
                        else {

                            DefaultMutableTreeNode newNode = new DefaultMutableTreeNode("IPCamera" + (iChannum+64*i + m_strDeviceInfo.byStartDChan)+"离线");
                            render.setTextNonSelectionColor(Color.GRAY);
                            TreeModel.insertNodeInto(newNode, m_DeviceRoot, m_iTreeNodeNum);
                            m_iTreeNodeNum++;

                        }

                    }

                }

            }
            TreeModel.reload();//将添加的节点显示到界面
        }

        jTreeDevice.setSelectionInterval(1, 1);//选中第一个节点
    }

    /*************************************************
     * 函数:    getChannelNumber
     * 函数描述:从设备树获取通道号
     *************************************************/
    int getChannelNumber() {
        int iChannelNum = -1;
        TreePath tp = jTreeDevice.getSelectionPath();//获取选中节点的路径
        if (tp != null)//判断路径是否有效,即判断是否有通道被选中
        {
            //获取选中的通道名,对通道名进行分析:
            String sChannelName = ((DefaultMutableTreeNode) tp.getLastPathComponent()).toString();
            if (sChannelName.charAt(0) == 'C')//Camara开头表示模拟通道
            {
                //子字符串中获取通道号
                iChannelNum = Integer.parseInt(sChannelName.substring(6));
            } else {
                if (sChannelName.charAt(0) == 'I')//IPCamara开头表示IP通道
                {
                    //子字符创中获取通道号,IP通道号要加32
                    //iChannelNum = Integer.parseInt(sChannelName.substring(8)) + m_strDeviceInfo.byStartDChan - 1;
                    iChannelNum = Integer.parseInt(sChannelName.split("_")[1]);
                } else {
                    return -1;
                }
            }
        } else {
            return -1;
        }
        return iChannelNum;
    }


    /**
     * 动态库加载
     *
     * @return
     */
    private static boolean CreateSDKInstance() {
        if (hCNetSDK == null) {
            synchronized (HCNetSDK.class) {
                String strDllPath = "";
                try {
                    if (osSelect.isWindows())
                        //win系统加载库路径
                        strDllPath = System.getProperty("user.dir")+"\\lib\\HCNetSDK.dll";

                    else if (osSelect.isLinux())
                        //Linux系统加载库路径
                        strDllPath = System.getProperty("user.dir")+"/lib/libhcnetsdk.so";
                    hCNetSDK = (HCNetSDK) Native.loadLibrary(strDllPath, HCNetSDK.class);
                } catch (Exception ex) {
                    System.out.println("loadLibrary: " + strDllPath + " Error: " + ex.getMessage());
                    return false;
                }
            }
        }
        return true;
    }

    /**
     * 播放库加载
     *
     * @return
     */
    private static boolean CreatePlayInstance() {
        if (playControl == null) {
            synchronized (PlayCtrl.class) {
                String strPlayPath = "";
                try {
                    if (osSelect.isWindows())
                        //win系统加载库路径
                        strPlayPath = System.getProperty("user.dir")+"\\lib\\PlayCtrl.dll";
                    else if (osSelect.isLinux())
                        //Linux系统加载库路径
                        strPlayPath = System.getProperty("user.dir")+"/lib/libPlayCtrl.so";
                    playControl=(PlayCtrl) Native.loadLibrary(strPlayPath,PlayCtrl.class);

                } catch (Exception ex) {
                    System.out.println("loadLibrary: " + strPlayPath + " Error: " + ex.getMessage());
                    return false;
                }
            }
        }
        return true;
    }

    /*************************************************
     * 函数:       主函数
     * 函数描述:新建ClientDemo窗体并调用接口初始化SDK
     *************************************************/
    public static void main(String args[]) {
        try {
            javax.swing.UIManager.setLookAndFeel("com.sun.java.swing.plaf.windows.WindowsLookAndFeel");
        } catch (Exception e) {
            e.printStackTrace();
        }

        java.awt.EventQueue.invokeLater(new Runnable() {

            public void run() {

                if (hCNetSDK == null&&playControl==null) {
                    if (!CreateSDKInstance()) {
                        System.out.println("Load SDK fail");
                        return;
                    }
                    if (!CreatePlayInstance()) {
                        System.out.println("Load PlayCtrl fail");
                        return;
                    }
                }
                //linux系统建议调用以下接口加载组件库
                if (osSelect.isLinux()) {
                    HCNetSDK.BYTE_ARRAY ptrByteArray1 = new HCNetSDK.BYTE_ARRAY(256);
                    HCNetSDK.BYTE_ARRAY ptrByteArray2 = new HCNetSDK.BYTE_ARRAY(256);
                    //这里是库的绝对路径，请根据实际情况修改，注意改路径必须有访问权限
                    String strPath1 = System.getProperty("user.dir")+"/lib/libcrypto.so.1.1";
                    String strPath2 = System.getProperty("user.dir")+"/lib/libssl.so.1.1";

                    System.arraycopy(strPath1.getBytes(), 0, ptrByteArray1.byValue, 0, strPath1.length());
                    ptrByteArray1.write();
                    hCNetSDK.NET_DVR_SetSDKInitCfg(3, ptrByteArray1.getPointer());

                    System.arraycopy(strPath2.getBytes(), 0, ptrByteArray2.byValue, 0, strPath2.length());
                    ptrByteArray2.write();
                    hCNetSDK.NET_DVR_SetSDKInitCfg(4, ptrByteArray2.getPointer());

                    String strPathCom = System.getProperty("user.dir")+"/lib/";
                    HCNetSDK.NET_DVR_LOCAL_SDK_PATH struComPath = new HCNetSDK.NET_DVR_LOCAL_SDK_PATH();
                    System.arraycopy(strPathCom.getBytes(), 0, struComPath.sPath, 0, strPathCom.length());
                    struComPath.write();
                    hCNetSDK.NET_DVR_SetSDKInitCfg(2, struComPath.getPointer());
                }

                boolean initSuc = hCNetSDK.NET_DVR_Init();
                if (initSuc != true) {
                    JOptionPane.showMessageDialog(null, "初始化失败");
                }
                if(fExceptionCallBack == null)
                {
                    fExceptionCallBack = new FExceptionCallBack_Imp();
                }
                Pointer pUser = null;
                if (!hCNetSDK.NET_DVR_SetExceptionCallBack_V30(0, 0, fExceptionCallBack, pUser)) {
                    return ;
                }
                System.out.println("设置告警回调成功");
                hCNetSDK.NET_DVR_SetLogToFile(3, "./sdklog", false);

                ClientDemo Demo = new ClientDemo();
                centerWindow(Demo);
                Demo.setVisible(true);
            }
        });
    }





    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton jButtonExit;
    private javax.swing.JButton jButtonLogin;
    private javax.swing.JButton jButtonRealPlay;
    private javax.swing.JComboBox jComboBoxCallback;
    private javax.swing.JLabel jLabelIPAddress;
    private javax.swing.JLabel jLabelPassWord;
    private javax.swing.JLabel jLabelPortNumber;
    private javax.swing.JLabel jLabelUserName;
    private javax.swing.JMenuBar jMenuBarConfig;
    private javax.swing.JMenu jMenuConfig;
    private javax.swing.JMenuItem jMenuItemAlarmCfg;
    private javax.swing.JMenuItem jMenuItemBasicConfig;
    private javax.swing.JMenuItem jMenuItemChannel;
    private javax.swing.JMenuItem jMenuItemCheckTime;
    private javax.swing.JMenuItem jMenuItemDefault;
    private javax.swing.JMenuItem jMenuItemDeviceState;
    private javax.swing.JMenuItem jMenuItemFormat;
    private javax.swing.JMenuItem jMenuItemIPAccess;
    private javax.swing.JMenuItem jMenuItemNetwork;
    private javax.swing.JMenuItem jMenuItemPlayBackRemote;
    private javax.swing.JMenuItem jMenuItemPlayTime;
    private javax.swing.JMenuItem jMenuItemReboot;
    private javax.swing.JMenuItem jMenuItemRemoveAlarm;
    private javax.swing.JMenuItem jMenuItemSerialCfg;
    private javax.swing.JMenuItem jMenuItemShutDown;
    private javax.swing.JMenuItem jMenuItemUpgrade;
    private javax.swing.JMenuItem jMenuItemUserConfig;
    private javax.swing.JMenuItem jMenuItemVoiceCom;
    private javax.swing.JMenu jMenuManage;
    private javax.swing.JMenu jMenuPlayBack;
    private javax.swing.JMenu jMenuSetAlarm;
    private javax.swing.JMenu jMenuVoice;
    private javax.swing.JPanel jPanelRealplayArea;
    private javax.swing.JPanel jPanelUserInfo;
    private javax.swing.JPasswordField jPasswordFieldPassword;
    private javax.swing.JRadioButtonMenuItem jRadioButtonMenuListen;
    private javax.swing.JRadioButtonMenuItem jRadioButtonMenuSetAlarm;
    private javax.swing.JScrollPane jScrollPaneTree;
    private javax.swing.JScrollPane jScrollPanelAlarmList;
    private javax.swing.JSeparator jSeparator1;
    private javax.swing.JSeparator jSeparator2;
    private javax.swing.JSplitPane jSplitPaneHorizontal;
    private javax.swing.JSplitPane jSplitPaneVertical;
    private javax.swing.JTable jTableAlarm;
    private javax.swing.JTextField jTextFieldIPAddress;
    private javax.swing.JTextField jTextFieldPortNumber;
    private javax.swing.JTextField jTextFieldUserName;
    private javax.swing.JTree jTreeDevice;
    private java.awt.Panel panelRealplay;
    // End of variables declaration//GEN-END:variables


    /******************************************************************************
     * 内部类:   FMSGCallBack
     * 报警信息回调函数
     ******************************************************************************/
    public class FMSGCallBack implements HCNetSDK.FMSGCallBack_V31 {
        //报警信息回调函数

        public boolean invoke(int lCommand, HCNetSDK.NET_DVR_ALARMER pAlarmer, Pointer pAlarmInfo, int dwBufLen, Pointer pUser) {
            String sAlarmType = new String();
            DefaultTableModel alarmTableModel = ((DefaultTableModel) jTableAlarm.getModel());//获取表格模型

            String[] newRow = new String[3];
            //报警时间
            Date today = new Date();
            DateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd hh:mm:ss");
            String[] sIP = new String[2];

            //lCommand是传的报警类型
            switch (lCommand) {
                //9000报警
                case HCNetSDK.COMM_ALARM_V30:
                    HCNetSDK.NET_DVR_ALARMINFO_V30 strAlarmInfoV30 = new HCNetSDK.NET_DVR_ALARMINFO_V30();
                    strAlarmInfoV30.write();
                    Pointer pInfoV30 = strAlarmInfoV30.getPointer();
                    pInfoV30.write(0, pAlarmInfo.getByteArray(0,strAlarmInfoV30.size()), 0, strAlarmInfoV30.size());
                    strAlarmInfoV30.read();

                    switch (strAlarmInfoV30.dwAlarmType) {
                        case 0:
                            sAlarmType = new String("信号量报警");
                            break;
                        case 1:
                            sAlarmType = new String("硬盘满");
                            break;
                        case 2:
                            sAlarmType = new String("信号丢失");
                            break;
                        case 3:
                            sAlarmType = new String("移动侦测");
                            break;
                        case 4:
                            sAlarmType = new String("硬盘未格式化");
                            break;
                        case 5:
                            sAlarmType = new String("读写硬盘出错");
                            break;
                        case 6:
                            sAlarmType = new String("遮挡报警");
                            break;
                        case 7:
                            sAlarmType = new String("制式不匹配");
                            break;
                        case 8:
                            sAlarmType = new String("非法访问");
                            break;
                    }

                    newRow[0] = dateFormat.format(today);
                    //报警类型
                    newRow[1] = sAlarmType;
                    //报警设备IP地址
                    sIP = new String(pAlarmer.sDeviceIP).split("\0", 2);
                    newRow[2] = sIP[0];
                    alarmTableModel.insertRow(0, newRow);

                    break;

                //8000报警
                case HCNetSDK.COMM_ALARM:
                    HCNetSDK.NET_DVR_ALARMINFO strAlarmInfo = new HCNetSDK.NET_DVR_ALARMINFO();
                    strAlarmInfo.write();
                    Pointer pInfo = strAlarmInfo.getPointer();
                    pInfo.write(0, pAlarmInfo.getByteArray(0,strAlarmInfo.size()), 0, strAlarmInfo.size());
                    strAlarmInfo.read();


                    switch (strAlarmInfo.dwAlarmType) {
                        case 0:
                            sAlarmType = new String("信号量报警");
                            break;
                        case 1:
                            sAlarmType = new String("硬盘满");
                            break;
                        case 2:
                            sAlarmType = new String("信号丢失");
                            break;
                        case 3:
                            sAlarmType = new String("移动侦测");
                            break;
                        case 4:
                            sAlarmType = new String("硬盘未格式化");
                            break;
                        case 5:
                            sAlarmType = new String("读写硬盘出错");
                            break;
                        case 6:
                            sAlarmType = new String("遮挡报警");
                            break;
                        case 7:
                            sAlarmType = new String("制式不匹配");
                            break;
                        case 8:
                            sAlarmType = new String("非法访问");
                            break;
                    }

                    newRow[0] = dateFormat.format(today);
                    //报警类型
                    newRow[1] = sAlarmType;
                    //报警设备IP地址
                    sIP = new String(pAlarmer.sDeviceIP).split("\0", 2);
                    newRow[2] = sIP[0];
                    alarmTableModel.insertRow(0, newRow);

                    break;

                //ATM DVR transaction information
                case HCNetSDK.COMM_TRADEINFO:
                    //处理交易信息报警
                    break;

                //IPC接入配置改变报警
                case HCNetSDK.COMM_IPCCFG:
                    // 处理IPC报警
                    break;

                default:
                    System.out.println("未知报警类型");
                    break;
            }
            return true;
        }

    }

    /******************************************************************************
     * 内部类:   FRealDataCallBack
     * 实现预览回调数据
     ******************************************************************************/
    class FRealDataCallBack implements HCNetSDK.FRealDataCallBack_V30 {
        //预览回调
        public void invoke(int lRealHandle, int dwDataType, ByteByReference pBuffer, int dwBufSize, Pointer pUser) {
            HWND hwnd = new HWND(Native.getComponentPointer(panelRealplay));
            switch (dwDataType) {
                case HCNetSDK.NET_DVR_SYSHEAD: //系统头

                    if (!playControl.PlayM4_GetPort(m_lPort)) //获取播放库未使用的通道号
                    {
                        break;
                    }

                    if (dwBufSize > 0) {
                        if (!playControl.PlayM4_SetStreamOpenMode(m_lPort.getValue(), PlayCtrl.STREAME_REALTIME))  //设置实时流播放模式
                        {
                            break;
                        }

                        if (!playControl.PlayM4_OpenStream(m_lPort.getValue(), pBuffer, dwBufSize, 1024 * 1024)) //打开流接口
                        {
                            break;
                        }

                        if (!playControl.PlayM4_Play(m_lPort.getValue(), hwnd)) //播放开始
                        {
                            break;
                        }
                    }
                case HCNetSDK.NET_DVR_STREAMDATA:   //码流数据
                    if ((dwBufSize > 0) && (m_lPort.getValue() != -1)) {
                        if (!playControl.PlayM4_InputData(m_lPort.getValue(), pBuffer, dwBufSize))  //输入流数据
                        {
                            break;
                        }
                    }
            }
        }
    }

    static class FExceptionCallBack_Imp implements HCNetSDK.FExceptionCallBack {
        public void invoke(int dwType, int lUserID, int lHandle, Pointer pUser) {
            System.out.println("异常事件类型:"+dwType);
            return;
        }
    }

}

