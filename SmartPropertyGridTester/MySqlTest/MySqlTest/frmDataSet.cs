using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace MySqlTest
{
    public partial class frmDataSet : Form
    {
        int curDataTypeID;

        public frmDataSet()
        {
            InitializeComponent();
        }

        private void frmDataSet_Load(object sender, EventArgs e)
        {
            // TODO: This line of code loads data into the 'esme_environmentDataSet.datatype' table. You can move, or remove it, as needed.
            this.datatypeTableAdapter.Fill(this.esme_environmentDataSet.datatype);
            // TODO: This line of code loads data into the 'esme_environmentDataSet.dataset' table. You can move, or remove it, as needed.
            // this.datasetTableAdapter.Fill(this.esme_environmentDataSet.dataset);

            // TODO: This line of code loads data into the 'esme_environmentDataSet.datatype' table. You can move, or remove it, as needed.
            this.datatypeTableAdapter.Fill(this.esme_environmentDataSet.datatype);
            this.datatypeTableAdapter.ClearBeforeFill = true;
        }

        public DialogResult ShowDialog(string DataTypeName)
        {
            cboDataType.SelectedIndex = cboDataType.FindStringExact(DataTypeName);
            return this.ShowDialog();
        }

        private void cboDataType_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (cboDataType.SelectedValue == null)
                return;
            curDataTypeID = int.Parse(((DataRowView)(cboDataType.SelectedValue)).Row[0].ToString());
            //this.datasetTableAdapter.FillByDataType(this.esme_environmentDataSet.dataset, curDataTypeID);
            //this.datasetTableAdapter.GetDataByDataType(curDataTypeID);
        }
    }
}