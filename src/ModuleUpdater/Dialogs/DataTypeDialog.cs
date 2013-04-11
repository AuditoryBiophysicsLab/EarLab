using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace ModuleUpdater.Dialogs
{
	/// <summary>
	/// Summary description for DataTypeDialog.
	/// </summary>
	public class DataTypeDialog : System.Windows.Forms.Form
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		private bool addMode;
		private DataRow datatypeRow;
		private System.Windows.Forms.Label nameLabel;
		private System.Windows.Forms.TextBox nameTextBox;
		private System.Windows.Forms.Label typeLabel;
		private System.Windows.Forms.ComboBox directionComboBox;
		private System.Windows.Forms.ComboBox typeComboBox;
		private System.Windows.Forms.Label directionLabel;
		private System.Windows.Forms.Button cancelButton;
		private System.Windows.Forms.Button okButton;
		private System.Windows.Forms.GroupBox infoGroupBox;
		private System.Windows.Forms.TextBox descriptionTextBox;
		private System.Windows.Forms.Label descriptionLabel;
		private ModuleUpdater.InheritedControls.WholeNumberTextBox indexTextBox;
		private System.Windows.Forms.Label indexLabel;
		private DataSet versionDataSet;

		public DataTypeDialog(bool addMode, ref DataRow datatypeRow, ref DataSet versionDataSet)
		{
			// Required for Windows Form Designer support
			InitializeComponent();
			EnableVisualStyles.Enable(this);

			this.addMode = addMode;
			this.datatypeRow = datatypeRow;
			this.versionDataSet = versionDataSet;

			this.typeComboBox.DataSource = this.versionDataSet.Tables["tblDataType"];
			this.directionComboBox.DataSource = this.versionDataSet.Tables["tblDirection"];

			if (this.addMode)
			{
				this.okButton.Text = "Add";
				this.Text = "Add New Data Type";
				this.typeComboBox.SelectedIndex = 0;
				this.directionComboBox.SelectedIndex = 0;
				this.indexTextBox.Text = "1";
			}
			else
			{
				this.okButton.Text = "Update";
				this.Text = "Update Data Type";

				this.nameTextBox.Text = this.datatypeRow["fldName"].ToString();
				this.descriptionTextBox.Text = this.datatypeRow["fldDescription"].ToString();
				this.typeComboBox.SelectedValue = long.Parse(this.datatypeRow["fldDataTypeID"].ToString());
				this.directionComboBox.SelectedValue = long.Parse(this.datatypeRow["fldDirectionID"].ToString());
				this.indexTextBox.Text = this.datatypeRow["fldIndex"].ToString();
			}
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.infoGroupBox = new System.Windows.Forms.GroupBox();
			this.indexLabel = new System.Windows.Forms.Label();
			this.indexTextBox = new ModuleUpdater.InheritedControls.WholeNumberTextBox();
			this.descriptionLabel = new System.Windows.Forms.Label();
			this.descriptionTextBox = new System.Windows.Forms.TextBox();
			this.directionLabel = new System.Windows.Forms.Label();
			this.typeComboBox = new System.Windows.Forms.ComboBox();
			this.directionComboBox = new System.Windows.Forms.ComboBox();
			this.typeLabel = new System.Windows.Forms.Label();
			this.nameTextBox = new System.Windows.Forms.TextBox();
			this.nameLabel = new System.Windows.Forms.Label();
			this.cancelButton = new System.Windows.Forms.Button();
			this.okButton = new System.Windows.Forms.Button();
			this.infoGroupBox.SuspendLayout();
			this.SuspendLayout();
			// 
			// infoGroupBox
			// 
			this.infoGroupBox.Controls.Add(this.indexLabel);
			this.infoGroupBox.Controls.Add(this.indexTextBox);
			this.infoGroupBox.Controls.Add(this.descriptionLabel);
			this.infoGroupBox.Controls.Add(this.descriptionTextBox);
			this.infoGroupBox.Controls.Add(this.directionLabel);
			this.infoGroupBox.Controls.Add(this.typeComboBox);
			this.infoGroupBox.Controls.Add(this.directionComboBox);
			this.infoGroupBox.Controls.Add(this.typeLabel);
			this.infoGroupBox.Controls.Add(this.nameTextBox);
			this.infoGroupBox.Controls.Add(this.nameLabel);
			this.infoGroupBox.Location = new System.Drawing.Point(8, 8);
			this.infoGroupBox.Name = "infoGroupBox";
			this.infoGroupBox.Size = new System.Drawing.Size(456, 168);
			this.infoGroupBox.TabIndex = 0;
			this.infoGroupBox.TabStop = false;
			this.infoGroupBox.Text = "Data Type Information";
			// 
			// indexLabel
			// 
			this.indexLabel.Location = new System.Drawing.Point(408, 120);
			this.indexLabel.Name = "indexLabel";
			this.indexLabel.Size = new System.Drawing.Size(40, 16);
			this.indexLabel.TabIndex = 11;
			this.indexLabel.Text = "Index:";
			this.indexLabel.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
			// 
			// indexTextBox
			// 
			this.indexTextBox.Location = new System.Drawing.Point(408, 136);
			this.indexTextBox.Name = "indexTextBox";
			this.indexTextBox.Size = new System.Drawing.Size(32, 20);
			this.indexTextBox.TabIndex = 10;
			this.indexTextBox.Text = "0";
			this.indexTextBox.TextChanged += new System.EventHandler(this.Changed_DoCheck);
			// 
			// descriptionLabel
			// 
			this.descriptionLabel.Location = new System.Drawing.Point(16, 48);
			this.descriptionLabel.Name = "descriptionLabel";
			this.descriptionLabel.Size = new System.Drawing.Size(40, 16);
			this.descriptionLabel.TabIndex = 9;
			this.descriptionLabel.Text = "Desc.:";
			this.descriptionLabel.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
			// 
			// descriptionTextBox
			// 
			this.descriptionTextBox.Location = new System.Drawing.Point(56, 48);
			this.descriptionTextBox.Multiline = true;
			this.descriptionTextBox.Name = "descriptionTextBox";
			this.descriptionTextBox.Size = new System.Drawing.Size(384, 64);
			this.descriptionTextBox.TabIndex = 2;
			this.descriptionTextBox.Text = "";
			this.descriptionTextBox.TextChanged += new System.EventHandler(this.Changed_DoCheck);
			// 
			// directionLabel
			// 
			this.directionLabel.Location = new System.Drawing.Point(312, 120);
			this.directionLabel.Name = "directionLabel";
			this.directionLabel.Size = new System.Drawing.Size(56, 16);
			this.directionLabel.TabIndex = 7;
			this.directionLabel.Text = "Direction:";
			this.directionLabel.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
			// 
			// typeComboBox
			// 
			this.typeComboBox.DisplayMember = "fldItem";
			this.typeComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.typeComboBox.Location = new System.Drawing.Point(56, 136);
			this.typeComboBox.Name = "typeComboBox";
			this.typeComboBox.Size = new System.Drawing.Size(248, 21);
			this.typeComboBox.TabIndex = 3;
			this.typeComboBox.ValueMember = "fldDataTypeID";
			this.typeComboBox.SelectedIndexChanged += new System.EventHandler(this.Changed_DoCheck);
			// 
			// directionComboBox
			// 
			this.directionComboBox.DisplayMember = "fldItem";
			this.directionComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.directionComboBox.Location = new System.Drawing.Point(312, 136);
			this.directionComboBox.Name = "directionComboBox";
			this.directionComboBox.Size = new System.Drawing.Size(88, 21);
			this.directionComboBox.TabIndex = 4;
			this.directionComboBox.ValueMember = "fldDirectionID";
			this.directionComboBox.SelectedIndexChanged += new System.EventHandler(this.Changed_DoCheck);
			// 
			// typeLabel
			// 
			this.typeLabel.Location = new System.Drawing.Point(56, 120);
			this.typeLabel.Name = "typeLabel";
			this.typeLabel.Size = new System.Drawing.Size(40, 16);
			this.typeLabel.TabIndex = 4;
			this.typeLabel.Text = "Type:";
			this.typeLabel.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
			// 
			// nameTextBox
			// 
			this.nameTextBox.Location = new System.Drawing.Point(56, 24);
			this.nameTextBox.Name = "nameTextBox";
			this.nameTextBox.Size = new System.Drawing.Size(384, 20);
			this.nameTextBox.TabIndex = 1;
			this.nameTextBox.Text = "";
			this.nameTextBox.TextChanged += new System.EventHandler(this.Changed_DoCheck);
			// 
			// nameLabel
			// 
			this.nameLabel.Location = new System.Drawing.Point(16, 24);
			this.nameLabel.Name = "nameLabel";
			this.nameLabel.Size = new System.Drawing.Size(40, 16);
			this.nameLabel.TabIndex = 0;
			this.nameLabel.Text = "Name:";
			this.nameLabel.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
			// 
			// cancelButton
			// 
			this.cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.cancelButton.Location = new System.Drawing.Point(392, 184);
			this.cancelButton.Name = "cancelButton";
			this.cancelButton.Size = new System.Drawing.Size(72, 24);
			this.cancelButton.TabIndex = 6;
			this.cancelButton.Text = "Cancel";
			// 
			// okButton
			// 
			this.okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.okButton.Location = new System.Drawing.Point(312, 184);
			this.okButton.Name = "okButton";
			this.okButton.Size = new System.Drawing.Size(72, 24);
			this.okButton.TabIndex = 5;
			this.okButton.Text = "OK";
			this.okButton.Click += new System.EventHandler(this.okButton_Click);
			// 
			// DataTypeDialog
			// 
			this.AcceptButton = this.okButton;
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.CancelButton = this.cancelButton;
			this.ClientSize = new System.Drawing.Size(474, 216);
			this.Controls.Add(this.okButton);
			this.Controls.Add(this.cancelButton);
			this.Controls.Add(this.infoGroupBox);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "DataTypeDialog";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "DataTypeDialog";
			this.infoGroupBox.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		private void DoCheck()
		{
			this.okButton.Enabled = false;

			if (this.nameTextBox.Text != "" && this.descriptionTextBox.Text != "" && this.indexTextBox.Text != "0")
			{
				if (this.addMode)
					this.okButton.Enabled = true;
				else if (this.nameTextBox.Text != this.datatypeRow["fldName"].ToString()
					|| this.descriptionTextBox.Text != this.datatypeRow["fldDescription"].ToString()
					|| this.typeComboBox.SelectedValue.ToString() != this.datatypeRow["fldDataTypeID"].ToString()
					|| this.directionComboBox.SelectedValue.ToString() != this.datatypeRow["fldDirectionID"].ToString()
					|| this.indexTextBox.Text != this.datatypeRow["fldIndex"].ToString())
					this.okButton.Enabled = true;
			}
		}
		
		private void Changed_DoCheck(object sender, System.EventArgs e)
		{
			this.DoCheck();
		}

		private void okButton_Click(object sender, System.EventArgs e)
		{
			this.datatypeRow["fldVersionID"] = this.versionDataSet.Tables["tblVersion"].Rows[0]["fldVersionID"];
			this.datatypeRow["fldDataTypeID"] = this.typeComboBox.SelectedValue;
			this.datatypeRow["fldDirectionID"] = this.directionComboBox.SelectedValue;
			this.datatypeRow["fldName"] = this.nameTextBox.Text;

			if (this.descriptionTextBox.Text != "")
				this.datatypeRow["fldDescription"] = this.descriptionTextBox.Text;
			else
				this.datatypeRow["fldDescription"] = DBNull.Value;

			this.datatypeRow["fldDataType"] = ((DataRowView)this.typeComboBox.SelectedItem).Row["fldItem"].ToString();
			this.datatypeRow["fldDirection"] = ((DataRowView)this.directionComboBox.SelectedItem).Row["fldItem"].ToString();
			if (this.indexTextBox.Text != "")
				this.datatypeRow["fldIndex"] = int.Parse(this.indexTextBox.Text);
			else
				this.datatypeRow["fldIndex"] = DBNull.Value;
		}
	}
}
