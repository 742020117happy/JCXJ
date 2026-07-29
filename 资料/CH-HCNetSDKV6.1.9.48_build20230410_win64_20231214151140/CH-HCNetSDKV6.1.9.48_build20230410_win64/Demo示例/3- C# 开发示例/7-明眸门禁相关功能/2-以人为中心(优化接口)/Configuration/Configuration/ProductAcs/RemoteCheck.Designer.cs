namespace Configuration
{
    partial class RemoteCheck
    {

////Required designer variable.

        private System.ComponentModel.IContainer components = null;


//Clean up any resources being used.

//<param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code


//Required method for Designer support - do not modify
//the contents of this method with the code editor.

        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(RemoteCheck));
            this.label1 = new System.Windows.Forms.Label();
            this.gBox = new System.Windows.Forms.GroupBox();
            this.textBoxChannelIP = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.cbChannelType = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.cbRemoteCheckDoor = new System.Windows.Forms.ComboBox();
            this.btnGet = new System.Windows.Forms.Button();
            this.btnSet = new System.Windows.Forms.Button();
            this.gBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(17, 16);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(91, 14);
            this.label1.TabIndex = 0;
            this.label1.Text = "DoorEnabled:";
            // 
            // gBox
            // 
            this.gBox.Controls.Add(this.textBoxChannelIP);
            this.gBox.Controls.Add(this.label3);
            this.gBox.Controls.Add(this.cbChannelType);
            this.gBox.Controls.Add(this.label2);
            this.gBox.Controls.Add(this.cbRemoteCheckDoor);
            this.gBox.Controls.Add(this.label1);
            this.gBox.Location = new System.Drawing.Point(12, 10);
            this.gBox.Name = "gBox";
            this.gBox.Size = new System.Drawing.Size(268, 117);
            this.gBox.TabIndex = 4;
            this.gBox.TabStop = false;
            // 
            // textBoxChannelIP
            // 
            this.textBoxChannelIP.Location = new System.Drawing.Point(114, 75);
            this.textBoxChannelIP.Name = "textBoxChannelIP";
            this.textBoxChannelIP.Size = new System.Drawing.Size(121, 22);
            this.textBoxChannelIP.TabIndex = 8;
            this.textBoxChannelIP.Text = "10.21.84.18";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(17, 78);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(77, 14);
            this.label3.TabIndex = 7;
            this.label3.Text = "ChannelIP:";
            // 
            // cbChannelType
            // 
            this.cbChannelType.FormattingEnabled = true;
            this.cbChannelType.Items.AddRange(new object[] {
            "PrivateSDK",
            "ISAPI",
            "ISUP",
            "Ezviz"});
            this.cbChannelType.Location = new System.Drawing.Point(114, 44);
            this.cbChannelType.Name = "cbChannelType";
            this.cbChannelType.Size = new System.Drawing.Size(121, 22);
            this.cbChannelType.TabIndex = 6;
            this.cbChannelType.Text = "PrivateSDK";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(17, 47);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(91, 14);
            this.label2.TabIndex = 5;
            this.label2.Text = "ChannelType:";
            // 
            // cbRemoteCheckDoor
            // 
            this.cbRemoteCheckDoor.FormattingEnabled = true;
            this.cbRemoteCheckDoor.Items.AddRange(new object[] {
            "true",
            "false"});
            this.cbRemoteCheckDoor.Location = new System.Drawing.Point(114, 13);
            this.cbRemoteCheckDoor.Name = "cbRemoteCheckDoor";
            this.cbRemoteCheckDoor.Size = new System.Drawing.Size(121, 22);
            this.cbRemoteCheckDoor.TabIndex = 4;
            this.cbRemoteCheckDoor.Text = "true";
            // 
            // btnGet
            // 
            this.btnGet.Location = new System.Drawing.Point(12, 133);
            this.btnGet.Name = "btnGet";
            this.btnGet.Size = new System.Drawing.Size(75, 23);
            this.btnGet.TabIndex = 6;
            this.btnGet.Text = "GET";
            this.btnGet.UseVisualStyleBackColor = true;
            this.btnGet.Click += new System.EventHandler(this.btnGet_Click);
            // 
            // btnSet
            // 
            this.btnSet.Location = new System.Drawing.Point(205, 133);
            this.btnSet.Name = "btnSet";
            this.btnSet.Size = new System.Drawing.Size(75, 23);
            this.btnSet.TabIndex = 7;
            this.btnSet.Text = "SET";
            this.btnSet.UseVisualStyleBackColor = true;
            this.btnSet.Click += new System.EventHandler(this.btnSet_Click);
            // 
            // RemoteCheck
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 14F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(296, 184);
            this.Controls.Add(this.btnSet);
            this.Controls.Add(this.btnGet);
            this.Controls.Add(this.gBox);
            this.Font = new System.Drawing.Font("Consolas", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "RemoteCheck";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "RemoteCheckCfg";
            this.Load += new System.EventHandler(this.RemoteCheck_Load);
            this.gBox.ResumeLayout(false);
            this.gBox.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.GroupBox gBox;
        private System.Windows.Forms.ComboBox cbRemoteCheckDoor;
        private System.Windows.Forms.Button btnGet;
        private System.Windows.Forms.Button btnSet;
        private System.Windows.Forms.ComboBox cbChannelType;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox textBoxChannelIP;
    }
}