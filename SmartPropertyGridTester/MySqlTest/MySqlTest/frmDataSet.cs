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

        public frmDataSet(MySqlConnection connection)
        {
            InitializeComponent();
            this.datasetTableAdapter.Connection = connection;
        }

        private void frmDataSet_Load(object sender, EventArgs e)
        {
            // TODO: This line of code loads data into the 'esme_environmentDataSet.dataset' table. You can move, or remove it, as needed.
            this.datasetTableAdapter.Fill(this.esme_environmentDataSet.dataset);

        }
    }
}