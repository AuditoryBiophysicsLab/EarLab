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
            this.btnEditDataTypeList = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.cboDataSet = new MySqlTest.cboDropdown();
            this.cboDataType = new MySqlTest.cboDropdown();
            this.environmentalDatabaseReader1 = new MySqlTest.EnvironmentalDatabaseReader();
            this.SuspendLayout();
            // 
            // btnEditDataTypeList
            // 
            this.btnEditDataTypeList.Location = new System.Drawing.Point(251, 24);
            this.btnEditDataTypeList.Name = "btnEditDataTypeList";
            this.btnEditDataTypeList.Size = new System.Drawing.Size(95, 23);
            this.btnEditDataTypeList.TabIndex = 3;
            this.btnEditDataTypeList.Text = "Edit Types...";
            this.btnEditDataTypeList.UseVisualStyleBackColor = true;
            this.btnEditDataTypeList.Click += new System.EventHandler(this.btnEditDataTypeList_Click);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(251, 78);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(95, 23);
            this.button1.TabIndex = 7;
            this.button1.Text = "Edit Datasets...";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(255, 136);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(95, 23);
            this.button2.TabIndex = 8;
            this.button2.Text = "Edit Subsets...";
            this.button2.UseVisualStyleBackColor = true;
            // 
            // cboDataSet
            // 
            this.cboDataSet.BackColor = System.Drawing.SystemColors.Control;
            this.cboDataSet.Caption = "Data Set";
            this.cboDataSet.Location = new System.Drawing.Point(12, 63);
            this.cboDataSet.MySqlConnection = null;
            this.cboDataSet.Name = "cboDataSet";
            this.cboDataSet.Size = new System.Drawing.Size(233, 40);
            this.cboDataSet.TabIndex = 12;
            // 
            // cboDataType
            // 
            this.cboDataType.BackColor = System.Drawing.SystemColors.Control;
            this.cboDataType.Caption = "Data Type";
            this.cboDataType.Location = new System.Drawing.Point(12, 9);
            this.cboDataType.MySqlConnection = null;
            this.cboDataType.Name = "cboDataType";
            this.cboDataType.Size = new System.Drawing.Size(233, 40);
            this.cboDataType.TabIndex = 10;
            // 
            // environmentalDatabaseReader1
            // 
            this.environmentalDatabaseReader1.AllowDrop = true;
            this.environmentalDatabaseReader1.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.environmentalDatabaseReader1.Location = new System.Drawing.Point(159, 310);
            this.environmentalDatabaseReader1.Name = "environmentalDatabaseReader1";
            this.environmentalDatabaseReader1.Size = new System.Drawing.Size(223, 150);
            this.environmentalDatabaseReader1.TabIndex = 9;
            // 
            // frmDatabaseEditor
            // 
            this.AllowDrop = true;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(394, 472);
            this.Controls.Add(this.cboDataSet);
            this.Controls.Add(this.cboDataType);
            this.Controls.Add(this.environmentalDatabaseReader1);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.btnEditDataTypeList);
            this.Name = "frmDatabaseEditor";
            this.Text = "ESME Environmental Database Editor";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btnEditDataTypeList;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
        private EnvironmentalDatabaseReader environmentalDatabaseReader1;
        private cboDropdown cboDataType;
        private cboDropdown cboDataSet;
    }
}

