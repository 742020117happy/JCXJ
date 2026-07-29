namespace Configuration.ProductAcs
{
    partial class UploadModeConfig
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
            this.SubChannelCom = new System.Windows.Forms.ComboBox();
            this.MainChannelCom = new System.Windows.Forms.ComboBox();
            this.GroupCenterCom = new System.Windows.Forms.ComboBox();
            this.UploadEnable = new System.Windows.Forms.CheckBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.btnUploadSet = new System.Windows.Forms.Button();
            this.g_IsupBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // g_IsupBox
            // 
            this.g_IsupBox.Controls.Add(this.SubChannelCom);
            this.g_IsupBox.Controls.Add(this.MainChannelCom);
            this.g_IsupBox.Controls.Add(this.GroupCenterCom);
            this.g_IsupBox.Controls.Add(this.UploadEnable);
            this.g_IsupBox.Controls.Add(this.label5);
            this.g_IsupBox.Controls.Add(this.label4);
            this.g_IsupBox.Controls.Add(this.label3);
            this.g_IsupBox.Location = new System.Drawing.Point(22, 22);
            this.g_IsupBox.Name = "g_IsupBox";
            this.g_IsupBox.Size = new System.Drawing.Size(280, 169);
            this.g_IsupBox.TabIndex = 2;
            this.g_IsupBox.TabStop = false;
            // 
            // SubChannelCom
            // 
            this.SubChannelCom.FormattingEnabled = true;
            this.SubChannelCom.Items.AddRange(new object[] {
            "close",
            "T1",
            "T2",
            "N1",
            "N2",
            "G1",
            "G2",
            "N3",
            "N4"});
            this.SubChannelCom.Location = new System.Drawing.Point(117, 98);
            this.SubChannelCom.Name = "SubChannelCom";
            this.SubChannelCom.Size = new System.Drawing.Size(121, 20);
            this.SubChannelCom.TabIndex = 8;
            // 
            // MainChannelCom
            // 
            this.MainChannelCom.FormattingEnabled = true;
            this.MainChannelCom.Items.AddRange(new object[] {
            "close",
            "T1",
            "T2",
            "N1",
            "N2",
            "G1",
            "G2",
            "N3",
            "N4"});
            this.MainChannelCom.Location = new System.Drawing.Point(117, 65);
            this.MainChannelCom.Name = "MainChannelCom";
            this.MainChannelCom.Size = new System.Drawing.Size(121, 20);
            this.MainChannelCom.TabIndex = 7;
            // 
            // GroupCenterCom
            // 
            this.GroupCenterCom.FormattingEnabled = true;
            this.GroupCenterCom.Location = new System.Drawing.Point(117, 27);
            this.GroupCenterCom.Name = "GroupCenterCom";
            this.GroupCenterCom.Size = new System.Drawing.Size(121, 20);
            this.GroupCenterCom.TabIndex = 6;
            // 
            // UploadEnable
            // 
            this.UploadEnable.AutoSize = true;
            this.UploadEnable.Location = new System.Drawing.Point(27, 134);
            this.UploadEnable.Name = "UploadEnable";
            this.UploadEnable.Size = new System.Drawing.Size(60, 16);
            this.UploadEnable.TabIndex = 3;
            this.UploadEnable.Text = "Enable";
            this.UploadEnable.UseVisualStyleBackColor = true;
            this.UploadEnable.CheckedChanged += new System.EventHandler(this.UploadEnable_CheckedChanged);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(25, 101);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(71, 12);
            this.label5.TabIndex = 5;
            this.label5.Text = "SubChannel:";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(25, 65);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(77, 12);
            this.label4.TabIndex = 4;
            this.label4.Text = "MainChannel:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(25, 30);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(77, 12);
            this.label3.TabIndex = 3;
            this.label3.Text = "GroupCenter:";
            // 
            // btnUploadSet
            // 
            this.btnUploadSet.Location = new System.Drawing.Point(110, 207);
            this.btnUploadSet.Name = "btnUploadSet";
            this.btnUploadSet.Size = new System.Drawing.Size(75, 23);
            this.btnUploadSet.TabIndex = 7;
            this.btnUploadSet.Text = "SET";
            this.btnUploadSet.UseVisualStyleBackColor = true;
            this.btnUploadSet.Click += new System.EventHandler(this.btnUploadSet_Click);
            // 
            // UploadModeConfig
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(325, 247);
            this.Controls.Add(this.btnUploadSet);
            this.Controls.Add(this.g_IsupBox);
            this.MaximizeBox = false;
            this.Name = "UploadModeConfig";
            this.Text = "UploadModeConfig";
            this.g_IsupBox.ResumeLayout(false);
            this.g_IsupBox.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox g_IsupBox;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.CheckBox UploadEnable;
        private System.Windows.Forms.ComboBox GroupCenterCom;
        private System.Windows.Forms.ComboBox SubChannelCom;
        private System.Windows.Forms.ComboBox MainChannelCom;
        private System.Windows.Forms.Button btnUploadSet;
    }
}