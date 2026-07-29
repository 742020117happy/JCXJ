namespace Configuration.ProductAcs
{
    partial class WlanConfig
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
            this.label3 = new System.Windows.Forms.Label();
            this.g_IsupBox = new System.Windows.Forms.GroupBox();
            this.DNSEnable = new System.Windows.Forms.CheckBox();
            this.DHCPEnable = new System.Windows.Forms.CheckBox();
            this.DNS2Address = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.DNS1Address = new System.Windows.Forms.TextBox();
            this.MacAddress = new System.Windows.Forms.TextBox();
            this.GateWay = new System.Windows.Forms.TextBox();
            this.SubNetMask = new System.Windows.Forms.TextBox();
            this.IPAddress = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.btnSave = new System.Windows.Forms.Button();
            this.g_IsupBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(29, 69);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(65, 12);
            this.label3.TabIndex = 3;
            this.label3.Text = "IPAddress:";
            // 
            // g_IsupBox
            // 
            this.g_IsupBox.Controls.Add(this.DNSEnable);
            this.g_IsupBox.Controls.Add(this.DHCPEnable);
            this.g_IsupBox.Controls.Add(this.DNS2Address);
            this.g_IsupBox.Controls.Add(this.label1);
            this.g_IsupBox.Controls.Add(this.DNS1Address);
            this.g_IsupBox.Controls.Add(this.MacAddress);
            this.g_IsupBox.Controls.Add(this.GateWay);
            this.g_IsupBox.Controls.Add(this.SubNetMask);
            this.g_IsupBox.Controls.Add(this.IPAddress);
            this.g_IsupBox.Controls.Add(this.label9);
            this.g_IsupBox.Controls.Add(this.label8);
            this.g_IsupBox.Controls.Add(this.label5);
            this.g_IsupBox.Controls.Add(this.label4);
            this.g_IsupBox.Controls.Add(this.label3);
            this.g_IsupBox.Location = new System.Drawing.Point(24, 26);
            this.g_IsupBox.Name = "g_IsupBox";
            this.g_IsupBox.Size = new System.Drawing.Size(280, 299);
            this.g_IsupBox.TabIndex = 1;
            this.g_IsupBox.TabStop = false;
            // 
            // DNSEnable
            // 
            this.DNSEnable.AutoSize = true;
            this.DNSEnable.Location = new System.Drawing.Point(164, 29);
            this.DNSEnable.Name = "DNSEnable";
            this.DNSEnable.Size = new System.Drawing.Size(42, 16);
            this.DNSEnable.TabIndex = 22;
            this.DNSEnable.Text = "DNS";
            this.DNSEnable.UseVisualStyleBackColor = true;
            this.DNSEnable.CheckedChanged += new System.EventHandler(this.DNSEnable_CheckedChanged);
            // 
            // DHCPEnable
            // 
            this.DHCPEnable.AutoSize = true;
            this.DHCPEnable.Location = new System.Drawing.Point(31, 29);
            this.DHCPEnable.Name = "DHCPEnable";
            this.DHCPEnable.Size = new System.Drawing.Size(48, 16);
            this.DHCPEnable.TabIndex = 21;
            this.DHCPEnable.Text = "DHCP";
            this.DHCPEnable.UseVisualStyleBackColor = true;
            this.DHCPEnable.CheckedChanged += new System.EventHandler(this.DHCPEnable_CheckedChanged_1);
            // 
            // DNS2Address
            // 
            this.DNS2Address.Location = new System.Drawing.Point(145, 257);
            this.DNS2Address.Name = "DNS2Address";
            this.DNS2Address.Size = new System.Drawing.Size(113, 21);
            this.DNS2Address.TabIndex = 20;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(29, 260);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(65, 12);
            this.label1.TabIndex = 19;
            this.label1.Text = "DNS2 Addr:";
            // 
            // DNS1Address
            // 
            this.DNS1Address.Location = new System.Drawing.Point(145, 218);
            this.DNS1Address.Name = "DNS1Address";
            this.DNS1Address.Size = new System.Drawing.Size(113, 21);
            this.DNS1Address.TabIndex = 18;
            // 
            // MacAddress
            // 
            this.MacAddress.Location = new System.Drawing.Point(145, 176);
            this.MacAddress.Name = "MacAddress";
            this.MacAddress.Size = new System.Drawing.Size(113, 21);
            this.MacAddress.TabIndex = 17;
            // 
            // GateWay
            // 
            this.GateWay.Location = new System.Drawing.Point(145, 137);
            this.GateWay.Name = "GateWay";
            this.GateWay.Size = new System.Drawing.Size(113, 21);
            this.GateWay.TabIndex = 16;
            // 
            // SubNetMask
            // 
            this.SubNetMask.Location = new System.Drawing.Point(145, 101);
            this.SubNetMask.Name = "SubNetMask";
            this.SubNetMask.Size = new System.Drawing.Size(113, 21);
            this.SubNetMask.TabIndex = 15;
            // 
            // IPAddress
            // 
            this.IPAddress.Location = new System.Drawing.Point(145, 66);
            this.IPAddress.Name = "IPAddress";
            this.IPAddress.Size = new System.Drawing.Size(113, 21);
            this.IPAddress.TabIndex = 14;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(29, 221);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(65, 12);
            this.label9.TabIndex = 9;
            this.label9.Text = "DNS1 Addr:";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(29, 179);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(71, 12);
            this.label8.TabIndex = 8;
            this.label8.Text = "MacAddress:";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(29, 140);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(95, 12);
            this.label5.TabIndex = 5;
            this.label5.Text = "DefaultGateWay:";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(29, 104);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(71, 12);
            this.label4.TabIndex = 4;
            this.label4.Text = "SubNetMask:";
            // 
            // btnSave
            // 
            this.btnSave.Location = new System.Drawing.Point(110, 340);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(75, 23);
            this.btnSave.TabIndex = 8;
            this.btnSave.Text = "SAVE";
            this.btnSave.UseVisualStyleBackColor = true;
            this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
            // 
            // WlanConfig
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(330, 375);
            this.Controls.Add(this.btnSave);
            this.Controls.Add(this.g_IsupBox);
            this.MaximizeBox = false;
            this.Name = "WlanConfig";
            this.Text = "WlanConfig";
            this.g_IsupBox.ResumeLayout(false);
            this.g_IsupBox.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.GroupBox g_IsupBox;
        private System.Windows.Forms.TextBox DNS1Address;
        private System.Windows.Forms.TextBox MacAddress;
        private System.Windows.Forms.TextBox GateWay;
        private System.Windows.Forms.TextBox SubNetMask;
        private System.Windows.Forms.TextBox IPAddress;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox DNS2Address;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnSave;
        private System.Windows.Forms.CheckBox DNSEnable;
        private System.Windows.Forms.CheckBox DHCPEnable;
    }
}