using System;
using System.Collections.Generic;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using MySql.Data.MySqlClient;

namespace MySqlTest
{
    public partial class cboDropdown : UserControl
    {
        private MySqlConnection mDB = new MySqlConnection();
        private string mHostname, mUsername, mPassword;
        private int mPort = -100;
        public string Table, DisplayColumnName, ValueColumnName, ForeignKeyName;
        public int ForeignKeyValue;

        public cboDropdown()
        {
            InitializeComponent();
            this.lblText.Text = "Text";
        }

        public cboDropdown(string Hostname, string Username, string Password)
        {
            InitializeComponent();
            mHostname = Hostname;
            mUsername = Username;
            mPassword = Password;
            ConnectToDatabase();
        }

        public cboDropdown(string Hostname, int Port, string Username, string Password)
        {
            InitializeComponent();
            mHostname = Hostname;
            mPort = Port;
            mUsername = Username;
            mPassword = Password;
            ConnectToDatabase();
        }

        public string Server { set { mHostname = value; } }
        public string Username { set { mUsername = value;} }
        public string Password { set { mPassword = value;} }
        public int Port { set { mPort = value;} }
        public override string Text
        {
            set { lblText.Text = value; }
            get { return lblText.Text; }
        }
        public int SelectedValue 
        { 
            get 
            {
                if (cboCombo.SelectedValue is int)
                    return (int)cboCombo.SelectedValue;
                if (cboCombo.SelectedValue is DataRowView)
                    return (int)(((DataRowView)(cboCombo.SelectedValue)).Row[1]);
                return -1;
            } 
        }

        public void Fill()
        {
            cboCombo.Items.Clear();

            if ((Table == null) || (DisplayColumnName == null) || (ValueColumnName == null))
                return;

            MySqlCommand command;

            if (ForeignKeyName == null)
                command = new MySqlCommand("SELECT " + DisplayColumnName + ",  " + ValueColumnName + " FROM " + Table + ";", mDB);
            else
                command = new MySqlCommand("SELECT " + DisplayColumnName + ",  " + ValueColumnName + " FROM " + Table + " WHERE " + ForeignKeyName + " = " + ForeignKeyValue.ToString() + ";", mDB);
#if false
            MySqlDataAdapter data = new MySqlDataAdapter(command);
            DataSet ds = new DataSet();
            data.Fill(ds);
            cboCombo.DataSource = ds.Tables[0];
            cboCombo.DisplayMember = DisplayColumnName;
            cboCombo.ValueMember = ValueColumnName;
#else
            ArrayList myList = new ArrayList();
            MySqlDataReader reader = command.ExecuteReader();
            if (!reader.HasRows)
                return;

            while (reader.Read())
            {
                myList.Add(new ComboData(reader.GetString(DisplayColumnName), reader.GetInt32(ValueColumnName)));
            }
            cboCombo.DataSource = myList;
            cboCombo.DisplayMember = "Display";
            cboCombo.ValueMember = "Value";
#endif
        }
        public event EventHandler SelectedValueChanged;

        public void Open()
        {
            TryConnect();
        }

        public ConnectionState State { get { return mDB.State; } }

        public cboDropdown(MySqlConnection SqlConnection)
        {
            InitializeComponent();

            mDB = SqlConnection;

            if (mDB.State != ConnectionState.Open)
            {
                mDB.Open();
                if (mDB.State != ConnectionState.Open)
                    throw new Exception("Supplied MySqlConnection is not connected to any database");
            }
        }

        private bool TryConnect()
        {
            try
            {
                if ((mHostname != null) && (mUsername != null) && (mPassword != null))
                    ConnectToDatabase();
                return true;
            }
            catch (Exception)
            {
            }
            return false;
        }

        private void ConnectToDatabase()
        {
            string ConnectString;
            
            if (mPort != -100)
                ConnectString = "Database=esme_environment;Server=" + mHostname + ";Port=" + mPort + ";User Id=" + mUsername + ";Password=" + mPassword;
            else
                ConnectString = "Database=esme_environment;Server=" + mHostname + ";User Id=" + mUsername + ";Password=" + mPassword;
            mDB = new MySqlConnection(ConnectString);
            mDB.Open();
            if (mDB.State != ConnectionState.Open)
            {
                if (mDB.State != ConnectionState.Open)
                    throw new Exception("Cannot connect to database with supplied credentials");
            }
        }

        private void cboCombo_SelectedValueChanged(object sender, EventArgs e)
        {
            if (SelectedValueChanged != null)
                SelectedValueChanged(sender, e);
        }
    }

    public class ComboData
    {
        public string Display;
        public int Value;

        public ComboData(string Display, int Value)
        {
            this.Display = Display;
            this.Value = Value;
        }
    }
}
