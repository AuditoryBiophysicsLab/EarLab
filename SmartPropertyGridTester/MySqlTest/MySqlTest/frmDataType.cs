using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace MySqlTest
{
    public partial class frmDataType : Form
    {
        public frmDataType()
        {
            InitializeComponent();
        }

        private void frmDataType_Load(object sender, EventArgs e)
        {
            // TODO: This line of code loads data into the 'esme_environmentDataSet.datatype' table. You can move, or remove it, as needed.
            this.datatypeTableAdapter.Fill(this.esme_environmentDataSet.datatype);
        }

        private void btnDone_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void dataGridView1_CellValueChanged(object sender, DataGridViewCellEventArgs e)
        {
            UpdateDataset();
        }

        private void frmDataType_Activated(object sender, EventArgs e)
        {
            this.datatypeTableAdapter.GetData();
        }

        private void dataGridView1_RowsAdded(object sender, DataGridViewRowsAddedEventArgs e)
        {
            UpdateDataset();
        }

        private void dataGridView1_RowsRemoved(object sender, DataGridViewRowsRemovedEventArgs e)
        {
            UpdateDataset();
        }

        private void UpdateDataset()
        {
            try
            {
                this.Validate();
                this.datatypeBindingSource.EndEdit();
                this.datatypeTableAdapter.Update(this.esme_environmentDataSet.datatype);
            }
            catch (System.Exception ex)
            {
                MessageBox.Show("Update failed: " + ex.ToString());
            }
        }
    }
}