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
            this.idDataSetDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.idDataTypeDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.nameDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.datasetBindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.esme_environmentDataSet = new MySqlTest.esme_environmentDataSet();
            this.label2 = new System.Windows.Forms.Label();
            this.btnDone = new System.Windows.Forms.Button();
            this.datasetTableAdapter = new MySqlTest.esme_environmentDataSetTableAdapters.datasetTableAdapter();
            this.DataType = new CustomControls.SimpleBoundDropdown();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.datasetBindingSource)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.esme_environmentDataSet)).BeginInit();
            this.SuspendLayout();
            // 
            // dataGridView1
            // 
            this.dataGridView1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.dataGridView1.AutoGenerateColumns = false;
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.idDataSetDataGridViewTextBoxColumn,
            this.idDataTypeDataGridViewTextBoxColumn,
            this.nameDataGridViewTextBoxColumn});
            this.dataGridView1.DataSource = this.datasetBindingSource;
            this.dataGridView1.Location = new System.Drawing.Point(13, 78);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.Size = new System.Drawing.Size(279, 212);
            this.dataGridView1.TabIndex = 4;
            this.dataGridView1.CellValueChanged += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView1_CellValueChanged);
            this.dataGridView1.RowsRemoved += new System.Windows.Forms.DataGridViewRowsRemovedEventHandler(this.dataGridView1_RowsRemoved);
            // 
            // idDataSetDataGridViewTextBoxColumn
            // 
            this.idDataSetDataGridViewTextBoxColumn.DataPropertyName = "idDataSet";
            this.idDataSetDataGridViewTextBoxColumn.HeaderText = "idDataSet";
            this.idDataSetDataGridViewTextBoxColumn.Name = "idDataSetDataGridViewTextBoxColumn";
            this.idDataSetDataGridViewTextBoxColumn.Visible = false;
            // 
            // idDataTypeDataGridViewTextBoxColumn
            // 
            this.idDataTypeDataGridViewTextBoxColumn.DataPropertyName = "idDataType";
            this.idDataTypeDataGridViewTextBoxColumn.HeaderText = "idDataType";
            this.idDataTypeDataGridViewTextBoxColumn.Name = "idDataTypeDataGridViewTextBoxColumn";
            this.idDataTypeDataGridViewTextBoxColumn.Visible = false;
            // 
            // nameDataGridViewTextBoxColumn
            // 
            this.nameDataGridViewTextBoxColumn.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.nameDataGridViewTextBoxColumn.DataPropertyName = "Name";
            this.nameDataGridViewTextBoxColumn.HeaderText = "Data Set";
            this.nameDataGridViewTextBoxColumn.Name = "nameDataGridViewTextBoxColumn";
            // 
            // datasetBindingSource
            // 
            this.datasetBindingSource.DataMember = "dataset";
            this.datasetBindingSource.DataSource = this.esme_environmentDataSet;
            // 
            // esme_environmentDataSet
            // 
            this.esme_environmentDataSet.DataSetName = "esme_environmentDataSet";
            this.esme_environmentDataSet.SchemaSerializationMode = System.Data.SchemaSerializationMode.IncludeSchema;
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
            // datasetTableAdapter
            // 
            this.datasetTableAdapter.ClearBeforeFill = true;
            // 
            // DataType
            // 
            this.DataType.BackColor = System.Drawing.SystemColors.Control;
            this.DataType.Caption = "Data Type";
            this.DataType.Location = new System.Drawing.Point(13, 13);
            this.DataType.MySqlConnection = null;
            this.DataType.Name = "DataType";
            this.DataType.Size = new System.Drawing.Size(279, 37);
            this.DataType.TabIndex = 7;
            this.DataType.SelectedValueChanged += new System.EventHandler(this.DataType_SelectedValueChanged);
            this.DataType.Leave += new System.EventHandler(this.DataType_Leave);
            // 
            // frmDataSet
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(304, 331);
            this.Controls.Add(this.DataType);
            this.Controls.Add(this.btnDone);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.dataGridView1);
            this.Name = "frmDataSet";
            this.Text = "frmDataSet";
            this.Load += new System.EventHandler(this.frmDataSet_Load);
            this.Activated += new System.EventHandler(this.frmDataSet_Activated);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.datasetBindingSource)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.esme_environmentDataSet)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button btnDone;
        private CustomControls.SimpleBoundDropdown DataType;
        private esme_environmentDataSet esme_environmentDataSet;
        private System.Windows.Forms.BindingSource datasetBindingSource;
        private MySqlTest.esme_environmentDataSetTableAdapters.datasetTableAdapter datasetTableAdapter;
        private System.Windows.Forms.DataGridViewTextBoxColumn idDataSetDataGridViewTextBoxColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn idDataTypeDataGridViewTextBoxColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn nameDataGridViewTextBoxColumn;
    }
}