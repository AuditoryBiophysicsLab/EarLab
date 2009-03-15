using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using MySql.Data.MySqlClient;

namespace MySqlTest
{
    public partial class frmDataSet : Form
    {
        private int mKey;
        private bool IsActive;

        public frmDataSet(MySqlConnection Connection)
        {
            InitializeComponent();
            DataType.MySqlConnection = Connection;
            DataType.Table = "DataType";
            DataType.DisplayColumnName = "Name";
            DataType.ValueColumnName = "idDataType";
            DataType.Fill();
        }

        public DialogResult ShowDialog(int Key)
        {
            mKey = Key;
            DataType.Find(mKey);
            return this.ShowDialog();
        }

        private void frmDataSet_Load(object sender, EventArgs e)
        {
            this.datasetTableAdapter.FillByDataType(this.esme_environmentDataSet.dataset, mKey);
        }

        private void UpdateDatabase()
        {
            try
            {
                this.Validate();
                this.datasetBindingSource.EndEdit();
                this.datasetTableAdapter.Update(this.esme_environmentDataSet.dataset);
            }
            catch (System.Exception ex)
            {
                MessageBox.Show("Update failed: " + ex.ToString());
            }
        }

        private void DataType_SelectedValueChanged(object sender, EventArgs e)
        {
            mKey = DataType.SelectedValue;
            this.datasetTableAdapter.FillByDataType(this.esme_environmentDataSet.dataset, mKey);
            dataGridView1.Columns["idDataSetDataGridViewTextBoxColumn"].Visible = false;
        }

        private void DataType_Leave(object sender, EventArgs e)
        {
            this.IsActive = false;
        }

        private void frmDataSet_Activated(object sender, EventArgs e)
        {
            this.datasetTableAdapter.GetDataByDataType(mKey);
            this.IsActive = true;
        }

        private void dataGridView1_CellValueChanged(object sender, DataGridViewCellEventArgs e)
        {
            if (IsActive)
            {
                dataGridView1.SelectedRows[0].Cells["idDataSetDataGridViewTextBoxColumn"].Value = mKey;
                UpdateDatabase();
            }
        }

        private void dataGridView1_RowsRemoved(object sender, DataGridViewRowsRemovedEventArgs e)
        {
            if (IsActive)
            {
                UpdateDatabase();
            }
        }
    }
}