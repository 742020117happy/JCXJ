namespace Configuration.ProductAcs
{
    partial class IsupAddrConfigure
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
            this.AccountBox = new System.Windows.Forms.TextBox();
            this.IsupKey = new System.Windows.Forms.TextBox();
            this.DoMainName = new System.Windows.Forms.TextBox();
            this.PortBox = new System.Windows.Forms.TextBox();
            this.IPAddress = new System.Windows.Forms.TextBox();
            this.IsupVersionCombox = new System.Windows.Forms.ComboBox();
            this.ProtocolTypeCombox = new System.Windows.Forms.ComboBox();
            this.AddressTypeCombox = new System.Windows.Forms.ComboBox();
            this.GroupCenterCombox = new System.Windows.Forms.ComboBox();
            this.label9 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.btnGet = new System.Windows.Forms.Button();
            this.btnSet = new System.Windows.Forms.Button();
            this.g_IsupBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // g_IsupBox
            // 
            this.g_IsupBox.Controls.Add(this.AccountBox);
            this.g_IsupBox.Controls.Add(this.IsupKey);
            this.g_IsupBox.Controls.Add(this.DoMainName);
            this.g_IsupBox.Controls.Add(this.PortBox);
            this.g_IsupBox.Controls.Add(this.IPAddress);
            this.g_IsupBox.Controls.Add(this.IsupVersionCombox);
            this.g_IsupBox.Controls.Add(this.ProtocolTypeCombox);
            this.g_IsupBox.Controls.Add(this.AddressTypeCombox);
            this.g_IsupBox.Controls.Add(this.GroupCenterCombox);
            this.g_IsupBox.Controls.Add(this.label9);
            this.g_IsupBox.Controls.Add(this.label8);
            this.g_IsupBox.Controls.Add(this.label7);
            this.g_IsupBox.Controls.Add(this.label6);
            this.g_IsupBox.Controls.Add(this.label5);
            this.g_IsupBox.Controls.Add(this.label4);
            this.g_IsupBox.Controls.Add(this.label3);
            this.g_IsupBox.Controls.Add(this.label2);
            this.g_IsupBox.Controls.Add(this.label1);
            this.g_IsupBox.Location = new System.Drawing.Point(12, 25);
            this.g_IsupBox.Name = "g_IsupBox";
            this.g_IsupBox.Size = new System.Drawing.Size(335, 366);
            this.g_IsupBox.TabIndex = 0;
            this.g_IsupBox.TabStop = false;
            // 
            // AccountBox
            // 
            this.AccountBox.Location = new System.Drawing.Point(122, 330);
            this.AccountBox.Name = "AccountBox";
            this.AccountBox.Size = new System.Drawing.Size(121, 21);
            this.AccountBox.TabIndex = 18;
            // 
            // IsupKey
            // 
            this.IsupKey.Location = new System.Drawing.Point(122, 288);
            this.IsupKey.Name = "IsupKey";
            this.IsupKey.Size = new System.Drawing.Size(121, 21);
            this.IsupKey.TabIndex = 17;
            // 
            // DoMainName
            // 
            this.DoMainName.Location = new System.Drawing.Point(122, 167);
            this.DoMainName.Name = "DoMainName";
            this.DoMainName.Size = new System.Drawing.Size(121, 21);
            this.DoMainName.TabIndex = 16;
            // 
            // PortBox
            // 
            this.PortBox.Location = new System.Drawing.Point(122, 131);
            this.PortBox.Name = "PortBox";
            this.PortBox.Size = new System.Drawing.Size(121, 21);
            this.PortBox.TabIndex = 15;
            // 
            // IPAddress
            // 
            this.IPAddress.Location = new System.Drawing.Point(122, 96);
            this.IPAddress.Name = "IPAddress";
            this.IPAddress.Size = new System.Drawing.Size(121, 21);
            this.IPAddress.TabIndex = 14;
            // 
            // IsupVersionCombox
            // 
            this.IsupVersionCombox.FormattingEnabled = true;
            this.IsupVersionCombox.Items.AddRange(new object[] {
            "None",
            "V2.0",
            "V4.0",
            "V5.0"});
            this.IsupVersionCombox.Location = new System.Drawing.Point(122, 245);
            this.IsupVersionCombox.Name = "IsupVersionCombox";
            this.IsupVersionCombox.Size = new System.Drawing.Size(121, 20);
            this.IsupVersionCombox.TabIndex = 13;
            this.IsupVersionCombox.Text = "v5.0";
            // 
            // ProtocolTypeCombox
            // 
            this.ProtocolTypeCombox.FormattingEnabled = true;
            this.ProtocolTypeCombox.Items.AddRange(new object[] {
            "PRIVATE",
            "NAL2300",
            "EHOME"});
            this.ProtocolTypeCombox.Location = new System.Drawing.Point(122, 206);
            this.ProtocolTypeCombox.Name = "ProtocolTypeCombox";
            this.ProtocolTypeCombox.Size = new System.Drawing.Size(121, 20);
            this.ProtocolTypeCombox.TabIndex = 12;
            this.ProtocolTypeCombox.Text = "EHOME";
            // 
            // AddressTypeCombox
            // 
            this.AddressTypeCombox.FormattingEnabled = true;
            this.AddressTypeCombox.Items.AddRange(new object[] {
            "None",
            "IP/IP6",
            "domain name"});
            this.AddressTypeCombox.Location = new System.Drawing.Point(122, 59);
            this.AddressTypeCombox.Name = "AddressTypeCombox";
            this.AddressTypeCombox.Size = new System.Drawing.Size(121, 20);
            this.AddressTypeCombox.TabIndex = 11;
            this.AddressTypeCombox.Text = "IP/IP6";
            // 
            // GroupCenterCombox
            // 
            this.GroupCenterCombox.FormattingEnabled = true;
            this.GroupCenterCombox.Location = new System.Drawing.Point(122, 25);
            this.GroupCenterCombox.Name = "GroupCenterCombox";
            this.GroupCenterCombox.Size = new System.Drawing.Size(121, 20);
            this.GroupCenterCombox.TabIndex = 10;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(27, 333);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(53, 12);
            this.label9.TabIndex = 9;
            this.label9.Text = "Account:";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(27, 291);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(53, 12);
            this.label8.TabIndex = 8;
            this.label8.Text = "IsupKey:";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(27, 248);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(77, 12);
            this.label7.TabIndex = 7;
            this.label7.Text = "IsupVersion:";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(27, 209);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(83, 12);
            this.label6.TabIndex = 6;
            this.label6.Text = "ProtocolType:";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(27, 170);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(71, 12);
            this.label5.TabIndex = 5;
            this.label5.Text = "DoMainName:";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(27, 134);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(35, 12);
            this.label4.TabIndex = 4;
            this.label4.Text = "Port:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(27, 99);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(65, 12);
            this.label3.TabIndex = 3;
            this.label3.Text = "IPAddress:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(27, 62);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(77, 12);
            this.label2.TabIndex = 2;
            this.label2.Text = "AddressType:";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(27, 28);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(77, 12);
            this.label1.TabIndex = 1;
            this.label1.Text = "CenterGroup:";
            // 
            // btnGet
            // 
            this.btnGet.Location = new System.Drawing.Point(41, 406);
            this.btnGet.Name = "btnGet";
            this.btnGet.Size = new System.Drawing.Size(75, 23);
            this.btnGet.TabIndex = 7;
            this.btnGet.Text = "GET";
            this.btnGet.UseVisualStyleBackColor = true;
            this.btnGet.Click += new System.EventHandler(this.btnGet_Click);
            // 
            // btnSet
            // 
            this.btnSet.Location = new System.Drawing.Point(180, 406);
            this.btnSet.Name = "btnSet";
            this.btnSet.Size = new System.Drawing.Size(75, 23);
            this.btnSet.TabIndex = 8;
            this.btnSet.Text = "SET";
            this.btnSet.UseVisualStyleBackColor = true;
            this.btnSet.Click += new System.EventHandler(this.btnSet_Click);
            // 
            // IsupAddrConfigure
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(364, 453);
            this.Controls.Add(this.btnSet);
            this.Controls.Add(this.btnGet);
            this.Controls.Add(this.g_IsupBox);
            this.MaximizeBox = false;
            this.Name = "IsupAddrConfigure";
            this.Text = "IsupAddrConfigure";
            this.g_IsupBox.ResumeLayout(false);
            this.g_IsupBox.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox g_IsupBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.ComboBox GroupCenterCombox;
        private System.Windows.Forms.ComboBox IsupVersionCombox;
        private System.Windows.Forms.ComboBox ProtocolTypeCombox;
        private System.Windows.Forms.ComboBox AddressTypeCombox;
        private System.Windows.Forms.Button btnGet;
        private System.Windows.Forms.Button btnSet;
        private System.Windows.Forms.TextBox AccountBox;
        private System.Windows.Forms.TextBox IsupKey;
        private System.Windows.Forms.TextBox DoMainName;
        private System.Windows.Forms.TextBox PortBox;
        private System.Windows.Forms.TextBox IPAddress;
    }
}