namespace MySqlTest
{
    partial class frmDataType2
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
            this.esme_environmentDataSet = new MySqlTest.esme_environmentDataSet();
            this.datatypeBindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.datatypeTableAdapter = new MySqlTest.esme_environmentDataSetTableAdapters.datatypeTableAdapter();
            this.idDataTypeDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.nameDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.esme_environmentDataSet)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.datatypeBindingSource)).BeginInit();
            this.SuspendLayout();
            // 
            // dataGridView1
            // 
            this.dataGridView1.AutoGenerateColumns = false;
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.idDataTypeDataGridViewTextBoxColumn,
            this.nameDataGridViewTextBoxColumn});
            this.dataGridView1.DataSource = this.datatypeBindingSource;
            this.dataGridView1.Location = new System.Drawing.Point(13, 13);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.Size = new System.Drawing.Size(267, 160);
            this.dataGridView1.TabIndex = 0;
            this.dataGridView1.CellValueChanged += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView1_CellValueChanged);
            // 
            // esme_environmentDataSet
            // 
            this.esme_environmentDataSet.DataSetName = "esme_environmentDataSet";
            this.esme_environmentDataSet.SchemaSerializationMode = System.Data.SchemaSerializationMode.IncludeSchema;
            // 
            // datatypeBindingSource
            // 
            this.datatypeBindingSource.DataMember = "datatype";
            this.datatypeBindingSource.DataSource = this.esme_environmentDataSet;
            // 
            // datatypeTableAdapter
            // 
            this.datatypeTableAdapter.ClearBeforeFill = true;
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
            this.nameDataGridViewTextBoxColumn.HeaderText = "Name";
            this.nameDataGridViewTextBoxColumn.Name = "nameDataGridViewTextBoxColumn";
            // 
            // frmDataType2
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(292, 266);
            this.Controls.Add(this.dataGridView1);
            this.Name = "frmDataType2";
            this.Text = "frmDataType2";
            this.Load += new System.EventHandler(this.frmDataType2_Load);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.esme_environmentDataSet)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.datatypeBindingSource)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.DataGridView dataGridView1;
        private esme_environmentDataSet esme_environmentDataSet;
        private System.Windows.Forms.BindingSource datatypeBindingSource;
        private MySqlTest.esme_environmentDataSetTableAdapters.datatypeTableAdapter datatypeTableAdapter;
        private System.Windows.Forms.DataGridViewTextBoxColumn idDataTypeDataGridViewTextBoxColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn nameDataGridViewTextBoxColumn;
    }
}