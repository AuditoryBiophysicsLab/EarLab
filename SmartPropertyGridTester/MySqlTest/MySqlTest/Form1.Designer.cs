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
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.esmeDataSet = new MySqlTest.esmeDataSet();
            this.environmentdatatypeBindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.environmentdatatypeTableAdapter = new MySqlTest.esmeDataSetTableAdapters.environmentdatatypeTableAdapter();
            this.label2 = new System.Windows.Forms.Label();
            this.btnEditDataTypeList = new System.Windows.Forms.Button();
            this.comboBox2 = new System.Windows.Forms.ComboBox();
            this.environmentdatasetBindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.environmentdatasetTableAdapter = new MySqlTest.esmeDataSetTableAdapters.environmentdatasetTableAdapter();
            ((System.ComponentModel.ISupportInitialize)(this.esmeDataSet)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.environmentdatatypeBindingSource)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.environmentdatasetBindingSource)).BeginInit();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 13);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(57, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Data Type";
            // 
            // comboBox1
            // 
            this.comboBox1.DataSource = this.environmentdatatypeBindingSource;
            this.comboBox1.DisplayMember = "DataTypeName";
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Location = new System.Drawing.Point(16, 30);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(94, 21);
            this.comboBox1.TabIndex = 1;
            this.comboBox1.ValueMember = "idEnvironmentDataType";
            // 
            // esmeDataSet
            // 
            this.esmeDataSet.DataSetName = "esmeDataSet";
            this.esmeDataSet.SchemaSerializationMode = System.Data.SchemaSerializationMode.IncludeSchema;
            // 
            // environmentdatatypeBindingSource
            // 
            this.environmentdatatypeBindingSource.DataMember = "environmentdatatype";
            this.environmentdatatypeBindingSource.DataSource = this.esmeDataSet;
            // 
            // environmentdatatypeTableAdapter
            // 
            this.environmentdatatypeTableAdapter.ClearBeforeFill = true;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(16, 68);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(35, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "label2";
            // 
            // btnEditDataTypeList
            // 
            this.btnEditDataTypeList.Location = new System.Drawing.Point(117, 30);
            this.btnEditDataTypeList.Name = "btnEditDataTypeList";
            this.btnEditDataTypeList.Size = new System.Drawing.Size(75, 23);
            this.btnEditDataTypeList.TabIndex = 3;
            this.btnEditDataTypeList.Text = "Edit List...";
            this.btnEditDataTypeList.UseVisualStyleBackColor = true;
            this.btnEditDataTypeList.Click += new System.EventHandler(this.btnEditDataTypeList_Click);
            // 
            // comboBox2
            // 
            this.comboBox2.DataSource = this.environmentdatasetBindingSource;
            this.comboBox2.DisplayMember = "DataSetName";
            this.comboBox2.FormattingEnabled = true;
            this.comboBox2.Location = new System.Drawing.Point(16, 84);
            this.comboBox2.Name = "comboBox2";
            this.comboBox2.Size = new System.Drawing.Size(94, 21);
            this.comboBox2.TabIndex = 4;
            this.comboBox2.ValueMember = "idEnvironmentDataSet";
            // 
            // environmentdatasetBindingSource
            // 
            this.environmentdatasetBindingSource.DataMember = "environmentdataset";
            this.environmentdatasetBindingSource.DataSource = this.esmeDataSet;
            // 
            // environmentdatasetTableAdapter
            // 
            this.environmentdatasetTableAdapter.ClearBeforeFill = true;
            // 
            // frmDatabaseEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(337, 266);
            this.Controls.Add(this.comboBox2);
            this.Controls.Add(this.btnEditDataTypeList);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.comboBox1);
            this.Controls.Add(this.label1);
            this.Name = "frmDatabaseEditor";
            this.Text = "ESME Environmental Database Editor";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.esmeDataSet)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.environmentdatatypeBindingSource)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.environmentdatasetBindingSource)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox comboBox1;
        private esmeDataSet esmeDataSet;
        private System.Windows.Forms.BindingSource environmentdatatypeBindingSource;
        private MySqlTest.esmeDataSetTableAdapters.environmentdatatypeTableAdapter environmentdatatypeTableAdapter;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button btnEditDataTypeList;
        private System.Windows.Forms.ComboBox comboBox2;
        private System.Windows.Forms.BindingSource environmentdatasetBindingSource;
        private MySqlTest.esmeDataSetTableAdapters.environmentdatasetTableAdapter environmentdatasetTableAdapter;

    }
}

