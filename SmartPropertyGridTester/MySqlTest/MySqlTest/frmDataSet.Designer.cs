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
            this.gridControl1 = new DevExpress.XtraGrid.GridControl();
            this.datasetBindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.esmeenvironmentDataSetBindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.esme_environmentDataSet = new MySqlTest.esme_environmentDataSet();
            this.gridView1 = new DevExpress.XtraGrid.Views.Grid.GridView();
            this.colidDataSet = new DevExpress.XtraGrid.Columns.GridColumn();
            this.colidDataType = new DevExpress.XtraGrid.Columns.GridColumn();
            this.colName = new DevExpress.XtraGrid.Columns.GridColumn();
            this.datasetTableAdapter = new MySqlTest.esme_environmentDataSetTableAdapters.datasetTableAdapter();
            ((System.ComponentModel.ISupportInitialize)(this.gridControl1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.datasetBindingSource)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.esmeenvironmentDataSetBindingSource)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.esme_environmentDataSet)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.gridView1)).BeginInit();
            this.SuspendLayout();
            // 
            // gridControl1
            // 
            this.gridControl1.DataSource = this.datasetBindingSource;
            this.gridControl1.Location = new System.Drawing.Point(13, 13);
            this.gridControl1.MainView = this.gridView1;
            this.gridControl1.Name = "gridControl1";
            this.gridControl1.Size = new System.Drawing.Size(400, 200);
            this.gridControl1.TabIndex = 0;
            this.gridControl1.ViewCollection.AddRange(new DevExpress.XtraGrid.Views.Base.BaseView[] {
            this.gridView1});
            // 
            // datasetBindingSource
            // 
            this.datasetBindingSource.DataMember = "dataset";
            this.datasetBindingSource.DataSource = this.esmeenvironmentDataSetBindingSource;
            // 
            // esmeenvironmentDataSetBindingSource
            // 
            this.esmeenvironmentDataSetBindingSource.DataSource = this.esme_environmentDataSet;
            this.esmeenvironmentDataSetBindingSource.Position = 0;
            // 
            // esme_environmentDataSet
            // 
            this.esme_environmentDataSet.DataSetName = "esme_environmentDataSet";
            this.esme_environmentDataSet.SchemaSerializationMode = System.Data.SchemaSerializationMode.IncludeSchema;
            // 
            // gridView1
            // 
            this.gridView1.Columns.AddRange(new DevExpress.XtraGrid.Columns.GridColumn[] {
            this.colidDataSet,
            this.colidDataType,
            this.colName});
            this.gridView1.GridControl = this.gridControl1;
            this.gridView1.Name = "gridView1";
            this.gridView1.OptionsCustomization.AllowColumnMoving = false;
            this.gridView1.OptionsCustomization.AllowColumnResizing = false;
            this.gridView1.OptionsCustomization.AllowFilter = false;
            this.gridView1.OptionsCustomization.AllowGroup = false;
            this.gridView1.OptionsCustomization.AllowSort = false;
            this.gridView1.OptionsMenu.EnableColumnMenu = false;
            this.gridView1.OptionsMenu.EnableFooterMenu = false;
            this.gridView1.OptionsMenu.EnableGroupPanelMenu = false;
            this.gridView1.OptionsView.ShowGroupPanel = false;
            // 
            // colidDataSet
            // 
            this.colidDataSet.Caption = "idDataSet";
            this.colidDataSet.FieldName = "idDataSet";
            this.colidDataSet.Name = "colidDataSet";
            // 
            // colidDataType
            // 
            this.colidDataType.Caption = "idDataType";
            this.colidDataType.FieldName = "idDataType";
            this.colidDataType.Name = "colidDataType";
            // 
            // colName
            // 
            this.colName.Caption = "Data Set";
            this.colName.FieldName = "Name";
            this.colName.Name = "colName";
            this.colName.Visible = true;
            this.colName.VisibleIndex = 0;
            // 
            // datasetTableAdapter
            // 
            this.datasetTableAdapter.ClearBeforeFill = true;
            // 
            // frmDataSet
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(421, 266);
            this.Controls.Add(this.gridControl1);
            this.Name = "frmDataSet";
            this.Text = "frmDataSet";
            this.Load += new System.EventHandler(this.frmDataSet_Load);
            ((System.ComponentModel.ISupportInitialize)(this.gridControl1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.datasetBindingSource)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.esmeenvironmentDataSetBindingSource)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.esme_environmentDataSet)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.gridView1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private DevExpress.XtraGrid.GridControl gridControl1;
        private DevExpress.XtraGrid.Views.Grid.GridView gridView1;
        private System.Windows.Forms.BindingSource esmeenvironmentDataSetBindingSource;
        private esme_environmentDataSet esme_environmentDataSet;
        private System.Windows.Forms.BindingSource datasetBindingSource;
        private MySqlTest.esme_environmentDataSetTableAdapters.datasetTableAdapter datasetTableAdapter;
        private DevExpress.XtraGrid.Columns.GridColumn colidDataSet;
        private DevExpress.XtraGrid.Columns.GridColumn colidDataType;
        private DevExpress.XtraGrid.Columns.GridColumn colName;
    }
}