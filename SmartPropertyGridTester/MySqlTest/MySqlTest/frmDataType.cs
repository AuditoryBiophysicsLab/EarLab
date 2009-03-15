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
        private bool IsActive = false;

        public frmDataType()
        {
            InitializeComponent();
        }

        private void frmDataType_Load(object sender, EventArgs e)
        {
            // TODO: This line of code loads data into the 'esme_environmentDataSet.datatype' table. You can move, or remove it, as needed.
        }

        private void btnDone_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void dataGridView1_CellValueChanged(object sender, DataGridViewCellEventArgs e)
        {
            if (IsActive)
            {
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

        private void frmDataType_Leave(object sender, EventArgs e)
        {
            this.IsActive = false;
        }

        private void frmDataType_Activated(object sender, EventArgs e)
        {
            this.IsActive = true;
        }

        private void UpdateDatabase()
        {
            try
            {
                this.Validate();
                this.datatypeBindingSource.EndEdit();
            }
            catch (System.Exception ex)
            {
                MessageBox.Show("Update failed: " + ex.ToString());
            }
        }
    }
}