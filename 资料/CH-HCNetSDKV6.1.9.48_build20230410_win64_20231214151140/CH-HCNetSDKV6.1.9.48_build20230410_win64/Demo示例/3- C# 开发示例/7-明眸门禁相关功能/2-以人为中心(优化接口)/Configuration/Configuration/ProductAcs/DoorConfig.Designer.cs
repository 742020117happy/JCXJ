namespace Configuration.ProductAcs
{
    partial class DoorConfig
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.g_IsupBox = new System.Windows.Forms.GroupBox();
            this.DoorName = new System.Windows.Forms.TextBox();
            this.SuperPasswd = new System.Windows.Forms.TextBox();
            this.DoorLeftTime = new System.Windows.Forms.TextBox();
            this.OpenDuration = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.SubChannelCom = new System.Windows.Forms.ComboBox();
            this.DoorContactCom = new System.Windows.Forms.ComboBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.btnUploadSet = new System.Windows.Forms.Button();
            this.g_IsupBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // g_IsupBox
            // 
            this.g_IsupBox.Controls.Add(this.DoorName);
            this.g_IsupBox.Controls.Add(this.SuperPasswd);
            this.g_IsupBox.Controls.Add(this.DoorLeftTime);
            this.g_IsupBox.Controls.Add(this.OpenDuration);
            this.g_IsupBox.Controls.Add(this.label6);
            this.g_IsupBox.Controls.Add(this.label2);
            this.g_IsupBox.Controls.Add(this.label1);
            this.g_IsupBox.Controls.Add(this.SubChannelCom);
            this.g_IsupBox.Controls.Add(this.DoorContactCom);
            this.g_IsupBox.Controls.Add(this.label5);
            this.g_IsupBox.Controls.Add(this.label4);
            this.g_IsupBox.Controls.Add(this.label3);
            this.g_IsupBox.Location = new System.Drawing.Point(24, 23);
            this.g_IsupBox.Name = "g_IsupBox";
            this.g_IsupBox.Size = new System.Drawing.Size(335, 271);
            this.g_IsupBox.TabIndex = 3;
            this.g_IsupBox.TabStop = false;
            // 
            // DoorName
            // 
            this.DoorName.Location = new System.Drawing.Point(191, 27);
            this.DoorName.Name = "DoorName";
            this.DoorName.Size = new System.Drawing.Size(121, 21);
            this.DoorName.TabIndex = 15;
            // 
            // SuperPasswd
            // 
            this.SuperPasswd.Location = new System.Drawing.Point(191, 232);
            this.SuperPasswd.Name = "SuperPasswd";
            this.SuperPasswd.Size = new System.Drawing.Size(121, 21);
            this.SuperPasswd.TabIndex = 14;
            // 
            // DoorLeftTime
            // 
            this.DoorLeftTime.Location = new System.Drawing.Point(191, 193);
            this.DoorLeftTime.Name = "DoorLeftTime";
            this.DoorLeftTime.Size = new System.Drawing.Size(121, 21);
            this.DoorLeftTime.TabIndex = 13;
            // 
            // OpenDuration
            // 
            this.OpenDuration.Location = new System.Drawing.Point(191, 148);
            this.OpenDuration.Name = "OpenDuration";
            this.OpenDuration.Size = new System.Drawing.Size(121, 21);
            this.OpenDuration.TabIndex = 12;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(16, 235);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(77, 12);
            this.label6.TabIndex = 11;
            this.label6.Text = "SuperPasswd:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(16, 196);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(155, 12);
            this.label2.TabIndex = 10;
            this.label2.Text = "DoorLeftOpenTimeoutAlarm:";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(16, 151);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(83, 12);
            this.label1.TabIndex = 9;
            this.label1.Text = "OpenDuration:";
            // 
            // SubChannelCom
            // 
            this.SubChannelCom.FormattingEnabled = true;
            this.SubChannelCom.Items.AddRange(new object[] {
            "Normally Closed",
            "Normally Opened"});
            this.SubChannelCom.Location = new System.Drawing.Point(191, 98);
            this.SubChannelCom.Name = "SubChannelCom";
            this.SubChannelCom.Size = new System.Drawing.Size(121, 20);
            this.SubChannelCom.TabIndex = 8;
            // 
            // DoorContactCom
            // 
            this.DoorContactCom.FormattingEnabled = true;
            this.DoorContactCom.Items.AddRange(new object[] {
            "Normally Closed",
            "Normally Opened"});
            this.DoorContactCom.Location = new System.Drawing.Point(191, 65);
            this.DoorContactCom.Name = "DoorContactCom";
            this.DoorContactCom.Size = new System.Drawing.Size(121, 20);
            this.DoorContactCom.TabIndex = 7;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(16, 101);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(95, 12);
            this.label5.TabIndex = 5;
            this.label5.Text = "ExitButtonType:";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(16, 65);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(77, 12);
            this.label4.TabIndex = 4;
            this.label4.Text = "DoorContact:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(16, 30);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(35, 12);
            this.label3.TabIndex = 3;
            this.label3.Text = "Name:";
            // 
            // btnUploadSet
            // 
            this.btnUploadSet.Location = new System.Drawing.Point(142, 310);
            this.btnUploadSet.Name = "btnUploadSet";
            this.btnUploadSet.Size = new System.Drawing.Size(75, 23);
            this.btnUploadSet.TabIndex = 8;
            this.btnUploadSet.Text = "SET";
            this.btnUploadSet.UseVisualStyleBackColor = true;
            this.btnUploadSet.Click += new System.EventHandler(this.btnUploadSet_Click);
            // 
            // DoorConfig
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(381, 348);
            this.Controls.Add(this.btnUploadSet);
            this.Controls.Add(this.g_IsupBox);
            this.MaximizeBox = false;
            this.Name = "DoorConfig";
            this.Text = "DoorConfig";
            this.g_IsupBox.ResumeLayout(false);
            this.g_IsupBox.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox g_IsupBox;
        private System.Windows.Forms.ComboBox SubChannelCom;
        private System.Windows.Forms.ComboBox DoorContactCom;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox DoorName;
        private System.Windows.Forms.TextBox SuperPasswd;
        private System.Windows.Forms.TextBox DoorLeftTime;
        private System.Windows.Forms.TextBox OpenDuration;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnUploadSet;
    }
}