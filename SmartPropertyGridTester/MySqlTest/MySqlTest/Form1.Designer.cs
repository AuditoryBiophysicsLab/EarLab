namespace MySqlTest
{
    partial class frmDatabaseEditor
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
            this.label1 = new System.Windows.Forms.Label();
            this.cboDataType = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.btnEditDataTypeList = new System.Windows.Forms.Button();
            this.comboBox2 = new System.Windows.Forms.ComboBox();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(57, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Data Type";
            // 
            // cboDataType
            // 
            this.cboDataType.FormattingEnabled = true;
            this.cboDataType.Location = new System.Drawing.Point(15, 26);
            this.cboDataType.Name = "cboDataType";
            this.cboDataType.Size = new System.Drawing.Size(233, 21);
            this.cboDataType.TabIndex = 1;
            this.cboDataType.ValueMember = "idEnvironmentDataType";
            this.cboDataType.SelectedIndexChanged += new System.EventHandler(this.cboDataType_SelectedIndexChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(15, 64);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(35, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "label2";
            // 
            // btnEditDataTypeList
            // 
            this.btnEditDataTypeList.Location = new System.Drawing.Point(254, 26);
            this.btnEditDataTypeList.Name = "btnEditDataTypeList";
            this.btnEditDataTypeList.Size = new System.Drawing.Size(75, 23);
            this.btnEditDataTypeList.TabIndex = 3;
            this.btnEditDataTypeList.Text = "Edit List...";
            this.btnEditDataTypeList.UseVisualStyleBackColor = true;
            this.btnEditDataTypeList.Click += new System.EventHandler(this.btnEditDataTypeList_Click);
            // 
            // comboBox2
            // 
            this.comboBox2.DisplayMember = "idEnvironmentDataSet";
            this.comboBox2.FormattingEnabled = true;
            this.comboBox2.Location = new System.Drawing.Point(15, 80);
            this.comboBox2.Name = "comboBox2";
            this.comboBox2.Size = new System.Drawing.Size(94, 21);
            this.comboBox2.TabIndex = 4;
            this.comboBox2.ValueMember = "idEnvironmentDataSet";
            // 
            // frmDatabaseEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(394, 472);
            this.Controls.Add(this.comboBox2);
            this.Controls.Add(this.btnEditDataTypeList);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.cboDataType);
            this.Controls.Add(this.label1);
            this.Name = "frmDatabaseEditor";
            this.Text = "ESME Environmental Database Editor";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox cboDataType;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button btnEditDataTypeList;
        private System.Windows.Forms.ComboBox comboBox2;
    }
}

