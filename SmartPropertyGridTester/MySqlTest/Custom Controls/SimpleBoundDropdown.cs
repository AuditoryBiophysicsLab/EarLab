using System;
using System.Collections.Generic;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using MySql.Data.MySqlClient;

namespace CustomControls
{
    public partial class SimpleBoundDropdown : UserControl
    {
        #region Private data
        private MySqlConnection mDB;
        private bool mDatasetIsValid = false;
        ArrayList mList = new ArrayList();
        #endregion

        #region Constructors
        public SimpleBoundDropdown()
        {
            InitializeComponent();
        }

        public SimpleBoundDropdown(MySqlConnection SqlConnection)
        {
            InitializeComponent();
            mDB = SqlConnection;
        }
        #endregion

        #region Events
        public event EventHandler SelectedValueChanged;
        #endregion

        #region Public Properties
        
        public string Table, DisplayColumnName, ValueColumnName, ForeignKeyName;
        public int ForeignKeyValue;

        public MySqlConnection MySqlConnection { set { mDB = value; } get { return mDB; } }

        public ConnectionState State { get { return mDB.State; } }
        public string Caption
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
        #endregion

        #region Public methods
        public void Fill()
        {
            MySqlCommand command;
            MySqlDataReader reader;
            ArrayList newList = new ArrayList();

            if ((Table == null) || (DisplayColumnName == null) || (ValueColumnName == null))
                return;

            if (mDB == null)
                return;

            if (mDB.State != ConnectionState.Open)
                mDB.Open();

            if (ForeignKeyName == null)
                command = new MySqlCommand("SELECT " + DisplayColumnName + ",  " + ValueColumnName + " FROM " + Table + ";", mDB);
            else
                command = new MySqlCommand("SELECT " + DisplayColumnName + ",  " + ValueColumnName + " FROM " + Table + " WHERE " + ForeignKeyName + " = " + ForeignKeyValue.ToString() + ";", mDB);

            reader = command.ExecuteReader();
            if (!reader.HasRows)
            {
                reader.Close();
                mDB.Close();
                mDatasetIsValid = false;
                cboCombo.DataSource = null;
                cboCombo.Items.Clear();
                cboCombo.Enabled = false;
                mList = null;
                return;
            }

            cboCombo.Enabled = true;
            while (reader.Read())
                newList.Add(new ComboData(reader.GetString(DisplayColumnName), reader.GetInt32(ValueColumnName)));
            reader.Close();
            mDB.Close();

            mDatasetIsValid = true;
            cboCombo.DataSource = newList;
            cboCombo.DisplayMember = "Display";
            cboCombo.ValueMember = "Value";
            mList = newList;
        }

        public void Find(int Key)
        {
            if (mList == null)
                return;
            for (int i = 0; i < mList.Count; i++)
            {
                ComboData curItem = (ComboData)mList[i];
                if (curItem.Value == Key)
                {
                    cboCombo.SelectedIndex = i;
                    return;
                }
            }
        }
        #endregion

        #region Private methods
        private void cboCombo_SelectedValueChanged(object sender, EventArgs e)
        {
            if ((SelectedValueChanged != null) && (mDatasetIsValid))
                SelectedValueChanged(sender, e);
        }
        #endregion
    }

    internal class ComboData
    {
        private string mDisplay;
        private int mValue;

        public ComboData(string Display, int Value)
        {
            mDisplay = Display;
            mValue = Value;
        }

        public string Display { get { return mDisplay; } }
        public int Value { get { return mValue; } }
        public override string ToString() { return mDisplay; }
    }
}
