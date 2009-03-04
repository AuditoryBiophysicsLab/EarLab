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
            this.components = new System.ComponentModel.Container();
            this.label1 = new System.Windows.Forms.Label();
            this.cboDataType = new System.Windows.Forms.ComboBox();
            this.datatypeBindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.esme_environmentDataSet = new MySqlTest.esme_environmentDataSet();
            this.label2 = new System.Windows.Forms.Label();
            this.btnEditDataTypeList = new System.Windows.Forms.Button();
            this.comboBox2 = new System.Windows.Forms.ComboBox();
            this.datasetBindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.datatypeTableAdapter = new MySqlTest.esme_environmentDataSetTableAdapters.datatypeTableAdapter();
            this.datasetTableAdapter = new MySqlTest.esme_environmentDataSetTableAdapters.datasetTableAdapter();
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.datasubsetBindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.label3 = new System.Windows.Forms.Label();
            this.datasubsetTableAdapter = new MySqlTest.esme_environmentDataSetTableAdapters.datasubsetTableAdapter();
            ((System.ComponentModel.ISupportInitialize)(this.datatypeBindingSource)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.esme_environmentDataSet)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.datasetBindingSource)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.datasubsetBindingSource)).BeginInit();
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
            this.cboDataType.DataSource = this.datatypeBindingSource;
            this.cboDataType.DisplayMember = "Name";
            this.cboDataType.FormattingEnabled = true;
            this.cboDataType.Location = new System.Drawing.Point(15, 26);
            this.cboDataType.Name = "cboDataType";
            this.cboDataType.Size = new System.Drawing.Size(233, 21);
            this.cboDataType.TabIndex = 1;
            this.cboDataType.ValueMember = "idEnvironmentDataType";
            this.cboDataType.SelectedIndexChanged += new System.EventHandler(this.cboDataType_SelectedIndexChanged);
            // 
            // datatypeBindingSource
            // 
            this.datatypeBindingSource.DataMember = "datatype";
            this.datatypeBindingSource.DataSource = this.esme_environmentDataSet;
            // 
            // esme_environmentDataSet
            // 
            this.esme_environmentDataSet.DataSetName = "esme_environmentDataSet";
            this.esme_environmentDataSet.SchemaSerializationMode = System.Data.SchemaSerializationMode.IncludeSchema;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(15, 64);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(49, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "Data Set";
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
            this.comboBox2.DataSource = this.datasetBindingSource;
            this.comboBox2.DisplayMember = "Name";
            this.comboBox2.FormattingEnabled = true;
            this.comboBox2.Location = new System.Drawing.Point(15, 80);
            this.comboBox2.Name = "comboBox2";
            this.comboBox2.Size = new System.Drawing.Size(233, 21);
            this.comboBox2.TabIndex = 4;
            this.comboBox2.ValueMember = "idEnvironmentDataSet";
            // 
            // datasetBindingSource
            // 
            this.datasetBindingSource.DataMember = "dataset";
            this.datasetBindingSource.DataSource = this.esme_environmentDataSet;
            // 
            // datatypeTableAdapter
            // 
            this.datatypeTableAdapter.ClearBeforeFill = true;
            // 
            // datasetTableAdapter
            // 
            this.datasetTableAdapter.ClearBeforeFill = true;
            // 
            // comboBox1
            // 
            this.comboBox1.DataSource = this.datasubsetBindingSource;
            this.comboBox1.DisplayMember = "Name";
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Location = new System.Drawing.Point(15, 136);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(233, 21);
            this.comboBox1.TabIndex = 6;
            this.comboBox1.ValueMember = "idEnvironmentDataSet";
            // 
            // datasubsetBindingSource
            // 
            this.datasubsetBindingSource.DataMember = "datasubset";
            this.datasubsetBindingSource.DataSource = this.esme_environmentDataSet;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(15, 120);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(66, 13);
            this.label3.TabIndex = 5;
            this.label3.Text = "Data Subset";
            // 
            // datasubsetTableAdapter
            // 
            this.datasubsetTableAdapter.ClearBeforeFill = true;
            // 
            // frmDatabaseEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(394, 472);
            this.Controls.Add(this.comboBox1);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.comboBox2);
            this.Controls.Add(this.btnEditDataTypeList);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.cboDataType);
            this.Controls.Add(this.label1);
            this.Name = "frmDatabaseEditor";
            this.Text = "ESME Environmental Database Editor";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.datatypeBindingSource)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.esme_environmentDataSet)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.datasetBindingSource)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.datasubsetBindingSource)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox cboDataType;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button btnEditDataTypeList;
        private System.Windows.Forms.ComboBox comboBox2;
        private esme_environmentDataSet esme_environmentDataSet;
        private System.Windows.Forms.BindingSource datatypeBindingSource;
        private MySqlTest.esme_environmentDataSetTableAdapters.datatypeTableAdapter datatypeTableAdapter;
        private System.Windows.Forms.BindingSource datasetBindingSource;
        private MySqlTest.esme_environmentDataSetTableAdapters.datasetTableAdapter datasetTableAdapter;
        private System.Windows.Forms.ComboBox comboBox1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.BindingSource datasubsetBindingSource;
        private MySqlTest.esme_environmentDataSetTableAdapters.datasubsetTableAdapter datasubsetTableAdapter;
    }
}

