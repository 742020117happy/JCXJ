namespace Configuration
{
    partial class Configuration
    {

///必需的设计器变量。

        private System.ComponentModel.IContainer components = null;


//清理所有正在使用的资源。

//<param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码


//设计器支持所需的方法 - 不要
//使用代码编辑器修改此方法的内容。

        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Configuration));
            this.panel1 = new System.Windows.Forms.Panel();
            this.label1 = new System.Windows.Forms.Label();
            this.btnLogin = new System.Windows.Forms.Button();
            this.btnRemoteCheck = new System.Windows.Forms.Button();
            this.comboBoxLanguage = new System.Windows.Forms.ComboBox();
            this.BtnIsupConfig = new System.Windows.Forms.Button();
            this.BtnWlanConfig = new System.Windows.Forms.Button();
            this.BtnUploadConfig = new System.Windows.Forms.Button();
            this.BtnDoorConfig = new System.Windows.Forms.Button();
            this.BtnTimeConfig = new System.Windows.Forms.Button();
            this.BtnExit = new System.Windows.Forms.Button();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("panel1.BackgroundImage")));
            this.panel1.Controls.Add(this.label1);
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(613, 106);
            this.panel1.TabIndex = 3;
            // 
            // label1
            // 
            this.label1.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.label1.AutoSize = true;
            this.label1.BackColor = System.Drawing.Color.Transparent;
            this.label1.Font = new System.Drawing.Font("Consolas", 26.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.ForeColor = System.Drawing.Color.White;
            this.label1.Location = new System.Drawing.Point(130, 31);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(341, 41);
            this.label1.TabIndex = 1;
            this.label1.Text = "ACS Configuration";
            // 
            // btnLogin
            // 
            this.btnLogin.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnLogin.Location = new System.Drawing.Point(499, 112);
            this.btnLogin.Name = "btnLogin";
            this.btnLogin.Size = new System.Drawing.Size(103, 29);
            this.btnLogin.TabIndex = 4;
            this.btnLogin.Text = "Login";
            this.btnLogin.UseVisualStyleBackColor = true;
            this.btnLogin.Click += new System.EventHandler(this.btnLogin_Click);
            // 
            // btnRemoteCheck
            // 
            this.btnRemoteCheck.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnRemoteCheck.Location = new System.Drawing.Point(12, 112);
            this.btnRemoteCheck.Name = "btnRemoteCheck";
            this.btnRemoteCheck.Size = new System.Drawing.Size(119, 29);
            this.btnRemoteCheck.TabIndex = 53;
            this.btnRemoteCheck.Text = "Remote Check";
            this.btnRemoteCheck.UseVisualStyleBackColor = true;
            this.btnRemoteCheck.Click += new System.EventHandler(this.btnRemoteCheck_Click);
            // 
            // comboBoxLanguage
            // 
            this.comboBoxLanguage.FormattingEnabled = true;
            this.comboBoxLanguage.Items.AddRange(new object[] {
            "English",
            "Chinese"});
            this.comboBoxLanguage.Location = new System.Drawing.Point(499, 205);
            this.comboBoxLanguage.Name = "comboBoxLanguage";
            this.comboBoxLanguage.Size = new System.Drawing.Size(98, 22);
            this.comboBoxLanguage.TabIndex = 61;
            this.comboBoxLanguage.Text = "English";
            this.comboBoxLanguage.SelectedIndexChanged += new System.EventHandler(this.comboBoxLanguage_SelectedIndexChanged);
            // 
            // BtnIsupConfig
            // 
            this.BtnIsupConfig.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.BtnIsupConfig.Location = new System.Drawing.Point(137, 112);
            this.BtnIsupConfig.Name = "BtnIsupConfig";
            this.BtnIsupConfig.Size = new System.Drawing.Size(119, 29);
            this.BtnIsupConfig.TabIndex = 62;
            this.BtnIsupConfig.Text = "Isup Config";
            this.BtnIsupConfig.UseVisualStyleBackColor = true;
            this.BtnIsupConfig.Click += new System.EventHandler(this.BtnIsupConfig_Click);
            // 
            // BtnWlanConfig
            // 
            this.BtnWlanConfig.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.BtnWlanConfig.Location = new System.Drawing.Point(262, 112);
            this.BtnWlanConfig.Name = "BtnWlanConfig";
            this.BtnWlanConfig.Size = new System.Drawing.Size(119, 29);
            this.BtnWlanConfig.TabIndex = 63;
            this.BtnWlanConfig.Text = "Wlan Config";
            this.BtnWlanConfig.UseVisualStyleBackColor = true;
            this.BtnWlanConfig.Click += new System.EventHandler(this.BtnWlanConfig_Click);
            // 
            // BtnUploadConfig
            // 
            this.BtnUploadConfig.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.BtnUploadConfig.Location = new System.Drawing.Point(12, 147);
            this.BtnUploadConfig.Name = "BtnUploadConfig";
            this.BtnUploadConfig.Size = new System.Drawing.Size(119, 29);
            this.BtnUploadConfig.TabIndex = 64;
            this.BtnUploadConfig.Text = "Upload Config";
            this.BtnUploadConfig.UseVisualStyleBackColor = true;
            this.BtnUploadConfig.Click += new System.EventHandler(this.BtnUploadConfig_Click);
            // 
            // BtnDoorConfig
            // 
            this.BtnDoorConfig.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.BtnDoorConfig.Location = new System.Drawing.Point(137, 147);
            this.BtnDoorConfig.Name = "BtnDoorConfig";
            this.BtnDoorConfig.Size = new System.Drawing.Size(119, 29);
            this.BtnDoorConfig.TabIndex = 65;
            this.BtnDoorConfig.Text = "Door Config";
            this.BtnDoorConfig.UseVisualStyleBackColor = true;
            this.BtnDoorConfig.Click += new System.EventHandler(this.BtnDoorConfig_Click);
            // 
            // BtnTimeConfig
            // 
            this.BtnTimeConfig.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.BtnTimeConfig.Location = new System.Drawing.Point(262, 147);
            this.BtnTimeConfig.Name = "BtnTimeConfig";
            this.BtnTimeConfig.Size = new System.Drawing.Size(119, 29);
            this.BtnTimeConfig.TabIndex = 66;
            this.BtnTimeConfig.Text = "Time Config";
            this.BtnTimeConfig.UseVisualStyleBackColor = true;
            this.BtnTimeConfig.Click += new System.EventHandler(this.BtnTimeConfig_Click);
            // 
            // BtnExit
            // 
            this.BtnExit.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.BtnExit.Location = new System.Drawing.Point(499, 147);
            this.BtnExit.Name = "BtnExit";
            this.BtnExit.Size = new System.Drawing.Size(103, 29);
            this.BtnExit.TabIndex = 67;
            this.BtnExit.Text = "EXIT";
            this.BtnExit.UseVisualStyleBackColor = true;
            this.BtnExit.Click += new System.EventHandler(this.BtnExit_Click);
            // 
            // Configuration
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 14F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(609, 237);
            this.Controls.Add(this.BtnExit);
            this.Controls.Add(this.BtnTimeConfig);
            this.Controls.Add(this.BtnDoorConfig);
            this.Controls.Add(this.BtnUploadConfig);
            this.Controls.Add(this.BtnWlanConfig);
            this.Controls.Add(this.BtnIsupConfig);
            this.Controls.Add(this.comboBoxLanguage);
            this.Controls.Add(this.btnRemoteCheck);
            this.Controls.Add(this.btnLogin);
            this.Controls.Add(this.panel1);
            this.Font = new System.Drawing.Font("Consolas", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "Configuration";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "ACS Demo";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Configuration_FormClosing);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnLogin;
        private System.Windows.Forms.Button btnRemoteCheck;
        private System.Windows.Forms.ComboBox comboBoxLanguage;
        private System.Windows.Forms.Button BtnIsupConfig;
        private System.Windows.Forms.Button BtnWlanConfig;
        private System.Windows.Forms.Button BtnUploadConfig;
        private System.Windows.Forms.Button BtnDoorConfig;
        private System.Windows.Forms.Button BtnTimeConfig;
        private System.Windows.Forms.Button BtnExit;
    }
}

