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
    public partial class SimpleBoundDropdown : ComboBox
    {
        #region Private data
        private MySqlConnection mDB;
        ArrayList mList = new ArrayList();
        private string mTable, mDisplayColumn, mValueColumn, mForeignKey;
        private int mForeignKeyValue;
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

        #region Public Properties
        [BrowsableAttribute(true),
        CategoryAttribute("Data"),
        DescriptionAttribute("The name of the database table used to populate the control.")]
        public string Table { get { return mTable; } set { mTable = value; } }

        [BrowsableAttribute(true),
        CategoryAttribute("Data"),
        DescriptionAttribute("The name of the column in the specified Table used for display.")]
        public string DisplayColumn { get { return mDisplayColumn; } set { mDisplayColumn = value; } }

        [BrowsableAttribute(true),
        CategoryAttribute("Data"),
        DescriptionAttribute("The name of the column in the specified Table used as the Key value. This value is returned by the KeyChanged event.")]
        public string ValueColumn { get { return mValueColumn; } set { mValueColumn = value; } }

        [BrowsableAttribute(true),
        CategoryAttribute("Data"),
        DescriptionAttribute("The name of the column in the specified Table used as the Foreign Key value.  Data is selected from the specified Table based on the value of this property.")]
        public string ForeignKey { get { return mForeignKey; } set { mForeignKey = value; } }

        [BrowsableAttribute(true),
        CategoryAttribute("Data"),
        DescriptionAttribute("The value of the foreign key used to populate the control.  This value is used to restrict the selection of data from the specified Table")]
        public int ForeignKeyValue { get { return mForeignKeyValue; } set { mForeignKeyValue = value; } }

        [BrowsableAttribute(false)]
        public MySqlConnection MySqlConnection { set { mDB = value; } get { return mDB; } }

        [BrowsableAttribute(false)]
        public ConnectionState State { get { return mDB.State; } }

        [BrowsableAttribute(false)]
        public new string DisplayMember { set { } }

        [BrowsableAttribute(false)]
        public new string ValueMember { set { } }
        #endregion

        #region Public methods
        public void Fill()
        {
            MySqlCommand command;
            MySqlDataReader reader;
            ArrayList newList = new ArrayList();

            if ((mTable == null) || (mDisplayColumn == null) || (mValueColumn == null))
                return;

            if (mDB == null)
                return;

            if (mDB.State != ConnectionState.Open)
                mDB.Open();

            if (mForeignKey == null)
                command = new MySqlCommand("SELECT " + mDisplayColumn + ", " + mValueColumn + " FROM " + mTable + ";", mDB);
            else
                command = new MySqlCommand("SELECT " + mDisplayColumn + ", " + mValueColumn + " FROM " + mTable + " WHERE " + mForeignKey + " = " + ForeignKeyValue.ToString() + ";", mDB);

            reader = command.ExecuteReader();
            if (!reader.HasRows)
            {
                reader.Close();
                mDB.Close();
                DataSource = null;
                Items.Clear();
                this.Enabled = false;
                mList = null;
                return;
            }

            this.Enabled = true;
            while (reader.Read())
                newList.Add(new ComboData(reader.GetString(mDisplayColumn), reader.GetInt32(mValueColumn), ForeignKeyValue));
            reader.Close();
            mDB.Close();

            DataSource = newList;
            DisplayMember = "Name";
            ValueMember = "Key";
            mList = newList;
        }

        public void Find(int Key)
        {
            if (mList == null)
                return;
            for (int i = 0; i < mList.Count; i++)
            {
                ComboData curItem = (ComboData)mList[i];
                if (curItem.Key == Key)
                {
                    SelectedIndex = i;
                    return;
                }
            }
        }
        #endregion

        #region Event Handlers
        [CategoryAttribute("Data"),
        DescriptionAttribute("Occurs when the selected value changes, and provides the new key value associated with the selected value.")]
        public event EventHandler<KeyChangedEventArgs> KeyChanged;
        protected void OnKeyChanged(KeyChangedEventArgs NewKey)
        {
            // Make a temporary copy of the event to avoid possibility of
            // a race condition if the last subscriber unsubscribes
            // immediately after the null check and before the event is raised.
            EventHandler<KeyChangedEventArgs> temp = KeyChanged;
            if (temp != null)
                temp(this, NewKey);
        }

        protected override void OnSelectedValueChanged(EventArgs e)
        {
            base.OnSelectedValueChanged(e);
            if (SelectedValue is CustomControls.ComboData)
                OnKeyChanged(new KeyChangedEventArgs((int)((CustomControls.ComboData)(SelectedValue)).Key));
            else if (SelectedValue is int)
                OnKeyChanged(new KeyChangedEventArgs((int)SelectedValue));
        }
        #endregion
    }

    public class KeyChangedEventArgs : EventArgs
    {
        public int newKey;

        public KeyChangedEventArgs(int NewKey)
        {
            this.NewKey = NewKey;
        }

        public int NewKey { get { return newKey; } set { newKey = value; } }
    }

    internal class ComboData
    {
        private string mName;
        private int mKey;
        private int mForeignKey;

        public ComboData(string Name, int Key, int ForeignKey)
        {
            mName = Name;
            mKey = Key;
            mForeignKey = ForeignKey;
        }

        public string Name { get { return mName; } }
        public int Key { get { return mKey; } }
        public int ForeignKey { get { return mForeignKey; } }
        public override string ToString() { return mName; }
    }
}
