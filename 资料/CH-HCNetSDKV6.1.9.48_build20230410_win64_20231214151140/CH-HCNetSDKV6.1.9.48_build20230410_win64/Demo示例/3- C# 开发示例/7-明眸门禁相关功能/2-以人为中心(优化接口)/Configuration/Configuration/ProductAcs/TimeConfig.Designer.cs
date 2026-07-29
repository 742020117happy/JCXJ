namespace Configuration.ProductAcs
{
    partial class TimeConfig
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
            this.SECONDS = new System.Windows.Forms.TextBox();
            this.MINIUTE = new System.Windows.Forms.TextBox();
            this.HOUR = new System.Windows.Forms.TextBox();
            this.DeviceDateTimePicker = new System.Windows.Forms.DateTimePicker();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.btnSet = new System.Windows.Forms.Button();
            this.g_IsupBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // g_IsupBox
            // 
            this.g_IsupBox.Controls.Add(this.SECONDS);
            this.g_IsupBox.Controls.Add(this.MINIUTE);
            this.g_IsupBox.Controls.Add(this.HOUR);
            this.g_IsupBox.Controls.Add(this.DeviceDateTimePicker);
            this.g_IsupBox.Controls.Add(this.label2);
            this.g_IsupBox.Controls.Add(this.label1);
            this.g_IsupBox.Location = new System.Drawing.Point(24, 23);
            this.g_IsupBox.Name = "g_IsupBox";
            this.g_IsupBox.Size = new System.Drawing.Size(335, 185);
            this.g_IsupBox.TabIndex = 1;
            this.g_IsupBox.TabStop = false;
            // 
            // SECONDS
            // 
            this.SECONDS.Location = new System.Drawing.Point(226, 120);
            this.SECONDS.Name = "SECONDS";
            this.SECONDS.Size = new System.Drawing.Size(39, 21);
            this.SECONDS.TabIndex = 6;
            // 
            // MINIUTE
            // 
            this.MINIUTE.Location = new System.Drawing.Point(169, 120);
            this.MINIUTE.Name = "MINIUTE";
            this.MINIUTE.Size = new System.Drawing.Size(39, 21);
            this.MINIUTE.TabIndex = 5;
            // 
            // HOUR
            // 
            this.HOUR.Location = new System.Drawing.Point(115, 120);
            this.HOUR.Name = "HOUR";
            this.HOUR.Size = new System.Drawing.Size(39, 21);
            this.HOUR.TabIndex = 4;
            // 
            // DeviceDateTimePicker
            // 
            this.DeviceDateTimePicker.Location = new System.Drawing.Point(115, 41);
            this.DeviceDateTimePicker.Name = "DeviceDateTimePicker";
            this.DeviceDateTimePicker.Size = new System.Drawing.Size(150, 21);
            this.DeviceDateTimePicker.TabIndex = 3;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(27, 123);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(71, 12);
            this.label2.TabIndex = 2;
            this.label2.Text = "DeviceTime:";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(27, 47);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(71, 12);
            this.label1.TabIndex = 1;
            this.label1.Text = "DeviceDate:";
            // 
            // btnSet
            // 
            this.btnSet.Location = new System.Drawing.Point(148, 224);
            this.btnSet.Name = "btnSet";
            this.btnSet.Size = new System.Drawing.Size(75, 23);
            this.btnSet.TabIndex = 9;
            this.btnSet.Text = "SET";
            this.btnSet.UseVisualStyleBackColor = true;
            this.btnSet.Click += new System.EventHandler(this.btnSet_Click);
            // 
            // TimeConfig
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(390, 269);
            this.Controls.Add(this.btnSet);
            this.Controls.Add(this.g_IsupBox);
            this.MaximizeBox = false;
            this.Name = "TimeConfig";
            this.Text = "TimeConfig";
            this.g_IsupBox.ResumeLayout(false);
            this.g_IsupBox.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox g_IsupBox;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox SECONDS;
        private System.Windows.Forms.TextBox MINIUTE;
        private System.Windows.Forms.TextBox HOUR;
        private System.Windows.Forms.DateTimePicker DeviceDateTimePicker;
        private System.Windows.Forms.Button btnSet;
    }
}