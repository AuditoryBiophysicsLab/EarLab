namespace MySqlTest
{
    partial class frmDataSet
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
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.label2 = new System.Windows.Forms.Label();
            this.btnDone = new System.Windows.Forms.Button();
            this.cboDataType = new System.Windows.Forms.ComboBox();
            this.datatypeBindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.esme_environmentDataSet = new MySqlTest.esme_environmentDataSet();
            this.datatypeTableAdapter = new MySqlTest.esme_environmentDataSetTableAdapters.datatypeTableAdapter();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.datatypeBindingSource)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.esme_environmentDataSet)).BeginInit();
            this.SuspendLayout();
            // 
            // dataGridView1
            // 
            this.dataGridView1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Location = new System.Drawing.Point(13, 78);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.Size = new System.Drawing.Size(279, 212);
            this.dataGridView1.TabIndex = 4;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 62);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(225, 13);
            this.label2.TabIndex = 5;
            this.label2.Text = "Available data set(s) for the selected data type";
            // 
            // btnDone
            // 
            this.btnDone.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnDone.Location = new System.Drawing.Point(217, 296);
            this.btnDone.Name = "btnDone";
            this.btnDone.Size = new System.Drawing.Size(75, 23);
            this.btnDone.TabIndex = 6;
            this.btnDone.Text = "Done";
            this.btnDone.UseVisualStyleBackColor = true;
            // 
            // cboDataType
            // 
            this.cboDataType.DataSource = this.datatypeBindingSource;
            this.cboDataType.DisplayMember = "Name";
            this.cboDataType.FormattingEnabled = true;
            this.cboDataType.Location = new System.Drawing.Point(12, 28);
            this.cboDataType.Name = "cboDataType";
            this.cboDataType.Size = new System.Drawing.Size(279, 21);
            this.cboDataType.TabIndex = 7;
            this.cboDataType.ValueMember = "idDataType";
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
            // datatypeTableAdapter
            // 
            this.datatypeTableAdapter.ClearBeforeFill = true;
            // 
            // frmDataSet
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(304, 331);
            this.Controls.Add(this.cboDataType);
            this.Controls.Add(this.btnDone);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.dataGridView1);
            this.Name = "frmDataSet";
            this.Text = "frmDataSet";
            this.Load += new System.EventHandler(this.frmDataSet_Load);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.datatypeBindingSource)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.esme_environmentDataSet)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button btnDone;
        private System.Windows.Forms.ComboBox cboDataType;
        private esme_environmentDataSet esme_environmentDataSet;
        private System.Windows.Forms.BindingSource datatypeBindingSource;
        private MySqlTest.esme_environmentDataSetTableAdapters.datatypeTableAdapter datatypeTableAdapter;
    }
}