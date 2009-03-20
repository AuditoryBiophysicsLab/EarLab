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
    public partial class frmDataType2 : Form
    {
        public frmDataType2(MySqlConnection connection)
        {
            InitializeComponent();
            this.datatypeTableAdapter.Connection = connection;
        }

        private void frmDataType2_Load(object sender, EventArgs e)
        {
            // TODO: This line of code loads data into the 'esme_environmentDataSet.datatype' table. You can move, or remove it, as needed.
            this.datatypeTableAdapter.Fill(this.esme_environmentDataSet.datatype);
        }

        private void dataGridView1_CellValueChanged(object sender, DataGridViewCellEventArgs e)
        {
            dataGridView1.EndEdit();
            this.datatypeTableAdapter.Update(this.esme_environmentDataSet.datatype);
        }

        private void dataGridView1_RowsAdded(object sender, DataGridViewRowsAddedEventArgs e)
        {
            DataRow dr = this.esme_environmentDataSet.datatype.NewRow();
            dr["Name"] = "";
        }
    }
}