namespace DeleteFace
{
    partial class DeleteFace
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(DeleteFace));
            this.panel1 = new System.Windows.Forms.Panel();
            this.label1 = new System.Windows.Forms.Label();
            this.btnLogin = new System.Windows.Forms.Button();
            this.comboBoxdDel = new System.Windows.Forms.ComboBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.textBoxReaderNo = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.checkBoxDelAll = new System.Windows.Forms.CheckBox();
            this.textBoxCardNo = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.checkedListBoxCardReader = new System.Windows.Forms.CheckedListBox();
            this.btnDel = new System.Windows.Forms.Button();
            this.panel1.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("panel1.BackgroundImage")));
            this.panel1.Controls.Add(this.label1);
            this.panel1.Location = new System.Drawing.Point(0, -2);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(572, 95);
            this.panel1.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.label1.AutoSize = true;
            this.label1.BackColor = System.Drawing.Color.Transparent;
            this.label1.Font = new System.Drawing.Font("Consolas", 26.25F);
            this.label1.ForeColor = System.Drawing.Color.White;
            this.label1.Location = new System.Drawing.Point(161, 23);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(208, 41);
            this.label1.TabIndex = 0;
            this.label1.Text = "DeleteFace";
            // 
            // btnLogin
            // 
            this.btnLogin.Location = new System.Drawing.Point(3, 113);
            this.btnLogin.Name = "btnLogin";
            this.btnLogin.Size = new System.Drawing.Size(62, 29);
            this.btnLogin.TabIndex = 1;
            this.btnLogin.Text = "Login";
            this.btnLogin.UseVisualStyleBackColor = true;
            this.btnLogin.Click += new System.EventHandler(this.btnLogin_Click);
            // 
            // comboBoxdDel
            // 
            this.comboBoxdDel.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxdDel.FormattingEnabled = true;
            this.comboBoxdDel.Items.AddRange(new object[] {
            "Delete ByCard",
            "Delete ByCardReader"});
            this.comboBoxdDel.Location = new System.Drawing.Point(16, 21);
            this.comboBoxdDel.Name = "comboBoxdDel";
            this.comboBoxdDel.Size = new System.Drawing.Size(162, 22);
            this.comboBoxdDel.TabIndex = 2;
            this.comboBoxdDel.SelectedIndexChanged += new System.EventHandler(this.comboBoxdDel_SelectedIndexChanged);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.textBoxReaderNo);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.checkBoxDelAll);
            this.groupBox1.Controls.Add(this.textBoxCardNo);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.checkedListBoxCardReader);
            this.groupBox1.Controls.Add(this.btnDel);
            this.groupBox1.Controls.Add(this.comboBoxdDel);
            this.groupBox1.Location = new System.Drawing.Point(68, 99);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(504, 297);
            this.groupBox1.TabIndex = 3;
            this.groupBox1.TabStop = false;
            // 
            // textBoxReaderNo
            // 
            this.textBoxReaderNo.Location = new System.Drawing.Point(91, 138);
            this.textBoxReaderNo.Name = "textBoxReaderNo";
            this.textBoxReaderNo.Size = new System.Drawing.Size(112, 22);
            this.textBoxReaderNo.TabIndex = 9;
            this.textBoxReaderNo.Text = "1";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(15, 141);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(70, 14);
            this.label3.TabIndex = 8;
            this.label3.Text = "ReaderNo:";
            // 
            // checkBoxDelAll
            // 
            this.checkBoxDelAll.AutoSize = true;
            this.checkBoxDelAll.Location = new System.Drawing.Point(18, 102);
            this.checkBoxDelAll.Name = "checkBoxDelAll";
            this.checkBoxDelAll.Size = new System.Drawing.Size(117, 18);
            this.checkBoxDelAll.TabIndex = 7;
            this.checkBoxDelAll.Text = "DeleteAllFace";
            this.checkBoxDelAll.UseVisualStyleBackColor = true;
            // 
            // textBoxCardNo
            // 
            this.textBoxCardNo.Location = new System.Drawing.Point(77, 61);
            this.textBoxCardNo.Name = "textBoxCardNo";
            this.textBoxCardNo.Size = new System.Drawing.Size(126, 22);
            this.textBoxCardNo.TabIndex = 6;
            this.textBoxCardNo.Text = "1000";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(15, 64);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(56, 14);
            this.label2.TabIndex = 5;
            this.label2.Text = "CardNo:";
            // 
            // checkedListBoxCardReader
            // 
            this.checkedListBoxCardReader.FormattingEnabled = true;
            this.checkedListBoxCardReader.Location = new System.Drawing.Point(230, 21);
            this.checkedListBoxCardReader.Name = "checkedListBoxCardReader";
            this.checkedListBoxCardReader.Size = new System.Drawing.Size(162, 191);
            this.checkedListBoxCardReader.TabIndex = 4;
            // 
            // btnDel
            // 
            this.btnDel.Location = new System.Drawing.Point(16, 181);
            this.btnDel.Name = "btnDel";
            this.btnDel.Size = new System.Drawing.Size(75, 23);
            this.btnDel.TabIndex = 3;
            this.btnDel.Text = "Delete";
            this.btnDel.UseVisualStyleBackColor = true;
            this.btnDel.Click += new System.EventHandler(this.btnDel_Click);
            // 
            // DeleteFace
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 14F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(574, 408);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.btnLogin);
            this.Controls.Add(this.panel1);
            this.Font = new System.Drawing.Font("Consolas", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "DeleteFace";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "DeleteFace";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.DeleteFace_FormClosing);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnLogin;
        private System.Windows.Forms.ComboBox comboBoxdDel;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button btnDel;
        private System.Windows.Forms.CheckedListBox checkedListBoxCardReader;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textBoxCardNo;
        private System.Windows.Forms.CheckBox checkBoxDelAll;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox textBoxReaderNo;
    }
}