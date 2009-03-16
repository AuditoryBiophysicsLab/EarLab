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
        private MySqlConnection connection;
        MySqlDataAdapter myDataAdapter;
        MySqlCommandBuilder myCommandBuilder;
        DataSet myDataSet;
        public frmDataType(MySqlConnection Connection)
        {
            InitializeComponent();
            connection = Connection;
            if (connection.State != ConnectionState.Open)
                connection.Open();
        }

        private void frmDataType_Load(object sender, EventArgs e)
        {
            myDataAdapter = new MySqlDataAdapter();
            myDataAdapter.SelectCommand = new MySqlCommand("Select * From datatype;", connection);
            myCommandBuilder = new MySqlCommandBuilder(myDataAdapter);

            myDataSet = new DataSet();
            myDataAdapter.Fill(myDataSet, "datatype");

            //code to modify data in dataset here
            dataGridView1.AutoGenerateColumns = true;
            dataGridView1.DataSource = myDataSet;
            dataGridView1.DataMember = "datatype";
            dataGridView1.Columns["idDataType"].Visible = false;
            dataGridView1.Columns["Name"].AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill;

        }

        private void dataGridView1_CellValueChanged(object sender, DataGridViewCellEventArgs e)
        {
            //Without the MySqlCommandBuilder this line would fail 
            myDataAdapter.Update(myDataSet, "datatype");
        }

        private void dataGridView1_RowsAdded(object sender, DataGridViewRowsAddedEventArgs e)
        {
        }
    }
}