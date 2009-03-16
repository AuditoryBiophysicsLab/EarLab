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
    public partial class frmDataType : Form
    {
        private MySqlConnection conn;
        public frmDataType(MySqlConnection connection)
        {
            InitializeComponent();
            conn = connection;
        }

        private void frmDataType_Load(object sender, EventArgs e)
        {
            // TODO: This line of code loads data into the 'esme_environmentDataSet.datatype' table. You can move, or remove it, as needed.
            this.datatypeTableAdapter.Connection = conn;
            this.datatypeTableAdapter.Fill(this.esme_environmentDataSet.datatype);

        }

        private void dataGridView1_CellValueChanged(object sender, DataGridViewCellEventArgs e)
        {
            esme_environmentDataSet.Tables["datatype"].AcceptChanges();
        }
    }
}