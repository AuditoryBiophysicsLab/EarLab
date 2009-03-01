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
    public partial class frmDatabaseEditor : Form
    {
        //MySqlConnection SqlConnection;
        //MySqlDataAdapter DataAdapter;
        //DataSet dataset = new DataSet();

        public frmDatabaseEditor()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            //SqlConnection = new MySqlConnection("Database=esme_environment;Data Source=nikon.bu.edu;User ID=da;Password=AmySophie17");
            //DataAdapter = new MySqlDataAdapter("SELECT * from `esme_environment`.`datatype`", SqlConnection);
            //DataAdapter.Fill(dataset);
            //cboDataType.DataSource = dataset;
            //cboDataType.Items.Add()
        }

        private void btnEditDataTypeList_Click(object sender, EventArgs e)
        {

        }

        private void cboDataType_SelectedIndexChanged(object sender, EventArgs e)
        {

        }
    }
}