using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace ModuleUpdater.Dialogs
{
	/// <summary>
	/// Summary description for ParameterDialog.
	/// </summary>
	public class ParameterDialog : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Button cancelButton;
		private System.Windows.Forms.Button okButton;
		private System.Windows.Forms.TextBox nameTextBox;
		private System.Windows.Forms.Label nameLabel;
		private System.Windows.Forms.Label descriptionLabel;
		private System.Windows.Forms.TextBox descriptionTextBox;
		private System.Windows.Forms.Label typeLabel;
		private System.Windows.Forms.ComboBox typeComboBox;
		private System.Windows.Forms.TextBox minTextBox;
		private System.Windows.Forms.Label minLabel;
		private System.Windows.Forms.TextBox maxTextBox;
		private System.Windows.Forms.Label maxLabel;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		private bool addMode;
		private DataRow parameterRow;
		private System.Windows.Forms.GroupBox requiredGroupBox;
		private System.Windows.Forms.GroupBox optionalGroupBox;
		private DataTable parameterTypeTable;

		public ParameterDialog(bool addMode, ref DataRow parameterRow, DataTable parameterTypeTable)
		{
			// Required for Windows Form Designer support
			InitializeComponent();
			EnableVisualStyles.Enable(this);

			this.addMode = addMode;
			this.parameterRow = parameterRow;
			this.parameterTypeTable = parameterTypeTable;
			this.typeComboBox.DataSource = this.parameterTypeTable;

			if (this.addMode)
			{
				this.okButton.Text = "Add";
				this.Text = "Add New Parameter";
				this.typeComboBox.SelectedIndex = 0;
			}
			else
			{
				this.okButton.Text = "Update";
				this.Text = "Update Parameter";
				this.nameTextBox.Text = this.parameterRow["fldName"].ToString();
				this.descriptionTextBox.Text = this.parameterRow["fldDescription"].ToString();

				this.typeComboBox.SelectedValue = long.Parse(this.parameterRow["fldParameterTypeID"].ToString());

				if (this.optionalGroupBox.Enabled)
				{
					this.minTextBox.Text = this.parameterRow["fldMinimum"].ToString();
					this.maxTextBox.Text = this.parameterRow["fldMaximum"].ToString();
				}
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
			this.requiredGroupBox = new System.Windows.Forms.GroupBox();
			this.typeComboBox = new System.Windows.Forms.ComboBox();
			this.typeLabel = new System.Windows.Forms.Label();
			this.descriptionTextBox = new System.Windows.Forms.TextBox();
			this.descriptionLabel = new System.Windows.Forms.Label();
			this.nameLabel = new System.Windows.Forms.Label();
			this.nameTextBox = new System.Windows.Forms.TextBox();
			this.optionalGroupBox = new System.Windows.Forms.GroupBox();
			this.maxLabel = new System.Windows.Forms.Label();
			this.maxTextBox = new System.Windows.Forms.TextBox();
			this.minLabel = new System.Windows.Forms.Label();
			this.minTextBox = new System.Windows.Forms.TextBox();
			this.cancelButton = new System.Windows.Forms.Button();
			this.okButton = new System.Windows.Forms.Button();
			this.requiredGroupBox.SuspendLayout();
			this.optionalGroupBox.SuspendLayout();
			this.SuspendLayout();
			// 
			// requiredGroupBox
			// 
			this.requiredGroupBox.Controls.Add(this.typeComboBox);
			this.requiredGroupBox.Controls.Add(this.typeLabel);
			this.requiredGroupBox.Controls.Add(this.descriptionTextBox);
			this.requiredGroupBox.Controls.Add(this.descriptionLabel);
			this.requiredGroupBox.Controls.Add(this.nameLabel);
			this.requiredGroupBox.Controls.Add(this.nameTextBox);
			this.requiredGroupBox.Location = new System.Drawing.Point(8, 8);
			this.requiredGroupBox.Name = "requiredGroupBox";
			this.requiredGroupBox.Size = new System.Drawing.Size(480, 136);
			this.requiredGroupBox.TabIndex = 0;
			this.requiredGroupBox.TabStop = false;
			this.requiredGroupBox.Text = "Required Information";
			// 
			// typeComboBox
			// 
			this.typeComboBox.DisplayMember = "fldItem";
			this.typeComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.typeComboBox.Location = new System.Drawing.Point(80, 100);
			this.typeComboBox.Name = "typeComboBox";
			this.typeComboBox.Size = new System.Drawing.Size(184, 21);
			this.typeComboBox.TabIndex = 5;
			this.typeComboBox.ValueMember = "fldParameterTypeID";
			this.typeComboBox.SelectedIndexChanged += new System.EventHandler(this.typeComboBox_SelectedIndexChanged);
			// 
			// typeLabel
			// 
			this.typeLabel.Location = new System.Drawing.Point(16, 100);
			this.typeLabel.Name = "typeLabel";
			this.typeLabel.Size = new System.Drawing.Size(32, 16);
			this.typeLabel.TabIndex = 4;
			this.typeLabel.Text = "Type:";
			this.typeLabel.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
			// 
			// descriptionTextBox
			// 
			this.descriptionTextBox.Location = new System.Drawing.Point(80, 48);
			this.descriptionTextBox.MaxLength = 65536;
			this.descriptionTextBox.Multiline = true;
			this.descriptionTextBox.Name = "descriptionTextBox";
			this.descriptionTextBox.Size = new System.Drawing.Size(384, 48);
			this.descriptionTextBox.TabIndex = 3;
			this.descriptionTextBox.Text = "";
			this.descriptionTextBox.TextChanged += new System.EventHandler(this.Changed_DoCheck);
			// 
			// descriptionLabel
			// 
			this.descriptionLabel.Location = new System.Drawing.Point(16, 48);
			this.descriptionLabel.Name = "descriptionLabel";
			this.descriptionLabel.Size = new System.Drawing.Size(64, 16);
			this.descriptionLabel.TabIndex = 2;
			this.descriptionLabel.Text = "Description:";
			this.descriptionLabel.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
			// 
			// nameLabel
			// 
			this.nameLabel.Location = new System.Drawing.Point(16, 24);
			this.nameLabel.Name = "nameLabel";
			this.nameLabel.Size = new System.Drawing.Size(40, 16);
			this.nameLabel.TabIndex = 1;
			this.nameLabel.Text = "Name:";
			this.nameLabel.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
			// 
			// nameTextBox
			// 
			this.nameTextBox.Location = new System.Drawing.Point(80, 24);
			this.nameTextBox.MaxLength = 255;
			this.nameTextBox.Name = "nameTextBox";
			this.nameTextBox.Size = new System.Drawing.Size(384, 20);
			this.nameTextBox.TabIndex = 0;
			this.nameTextBox.Text = "";
			this.nameTextBox.TextChanged += new System.EventHandler(this.Changed_DoCheck);
			// 
			// optionalGroupBox
			// 
			this.optionalGroupBox.Controls.Add(this.maxLabel);
			this.optionalGroupBox.Controls.Add(this.maxTextBox);
			this.optionalGroupBox.Controls.Add(this.minLabel);
			this.optionalGroupBox.Controls.Add(this.minTextBox);
			this.optionalGroupBox.Location = new System.Drawing.Point(8, 152);
			this.optionalGroupBox.Name = "optionalGroupBox";
			this.optionalGroupBox.Size = new System.Drawing.Size(480, 80);
			this.optionalGroupBox.TabIndex = 1;
			this.optionalGroupBox.TabStop = false;
			this.optionalGroupBox.Text = "Variable Type Information";
			// 
			// maxLabel
			// 
			this.maxLabel.Location = new System.Drawing.Point(248, 24);
			this.maxLabel.Name = "maxLabel";
			this.maxLabel.Size = new System.Drawing.Size(100, 16);
			this.maxLabel.TabIndex = 3;
			this.maxLabel.Text = "Maximum Value:";
			// 
			// maxTextBox
			// 
			this.maxTextBox.Location = new System.Drawing.Point(248, 40);
			this.maxTextBox.MaxLength = 255;
			this.maxTextBox.Name = "maxTextBox";
			this.maxTextBox.Size = new System.Drawing.Size(216, 20);
			this.maxTextBox.TabIndex = 2;
			this.maxTextBox.Text = "";
			this.maxTextBox.TextChanged += new System.EventHandler(this.Changed_DoCheck);
			// 
			// minLabel
			// 
			this.minLabel.Location = new System.Drawing.Point(16, 24);
			this.minLabel.Name = "minLabel";
			this.minLabel.Size = new System.Drawing.Size(100, 16);
			this.minLabel.TabIndex = 1;
			this.minLabel.Text = "Minimum Value:";
			// 
			// minTextBox
			// 
			this.minTextBox.Location = new System.Drawing.Point(16, 40);
			this.minTextBox.MaxLength = 255;
			this.minTextBox.Name = "minTextBox";
			this.minTextBox.Size = new System.Drawing.Size(216, 20);
			this.minTextBox.TabIndex = 0;
			this.minTextBox.Text = "";
			this.minTextBox.TextChanged += new System.EventHandler(this.Changed_DoCheck);
			// 
			// cancelButton
			// 
			this.cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.cancelButton.Location = new System.Drawing.Point(408, 240);
			this.cancelButton.Name = "cancelButton";
			this.cancelButton.Size = new System.Drawing.Size(80, 24);
			this.cancelButton.TabIndex = 2;
			this.cancelButton.Text = "Cancel";
			// 
			// okButton
			// 
			this.okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.okButton.Location = new System.Drawing.Point(320, 240);
			this.okButton.Name = "okButton";
			this.okButton.Size = new System.Drawing.Size(80, 24);
			this.okButton.TabIndex = 3;
			this.okButton.Text = "OK";
			this.okButton.Click += new System.EventHandler(this.okButton_Click);
			// 
			// ParameterDialog
			// 
			this.AcceptButton = this.okButton;
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.CancelButton = this.cancelButton;
			this.ClientSize = new System.Drawing.Size(496, 272);
			this.Controls.Add(this.okButton);
			this.Controls.Add(this.cancelButton);
			this.Controls.Add(this.optionalGroupBox);
			this.Controls.Add(this.requiredGroupBox);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "ParameterDialog";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "ParameterDialog";
			this.requiredGroupBox.ResumeLayout(false);
			this.optionalGroupBox.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		private void DoCheck()
		{
			this.okButton.Enabled = false;

			if (addMode)
			{
				if (this.nameTextBox.Text != ""
					&& this.descriptionTextBox.Text != "")
				{
					if (this.optionalGroupBox.Enabled && this.minTextBox.Text != "" && this.maxTextBox.Text != "")
						this.okButton.Enabled = true;
					else if (!this.optionalGroupBox.Enabled)
						this.okButton.Enabled = true;
				}
			}
			else
			{
				if (this.nameTextBox.Text != this.parameterRow["fldName"].ToString()
					|| this.descriptionTextBox.Text != this.parameterRow["fldDescription"].ToString()
					|| this.typeComboBox.SelectedValue.ToString() != this.parameterRow["fldParameterTypeID"].ToString()
					|| this.minTextBox.Text != this.parameterRow["fldMinimum"].ToString()
					|| this.maxTextBox.Text != this.parameterRow["fldMaximum"].ToString())
				{
					if (this.nameTextBox.Text != "" && this.descriptionTextBox.Text != "")
					{
						if (this.optionalGroupBox.Enabled && this.minTextBox.Text != "" && this.maxTextBox.Text != "")
							this.okButton.Enabled = true;
						else if (!this.optionalGroupBox.Enabled)
							this.okButton.Enabled = true;
					}
				}
			}
		}
		
		private void typeComboBox_SelectedIndexChanged(object sender, System.EventArgs e)
		{
			if (((DataRowView)this.typeComboBox.SelectedItem).Row["fldItem"].ToString() == "string"
				|| ((DataRowView)this.typeComboBox.SelectedItem).Row["fldItem"].ToString() == "bool"
				|| ((DataRowView)this.typeComboBox.SelectedItem).Row["fldItem"].ToString() == "int vector"
				|| ((DataRowView)this.typeComboBox.SelectedItem).Row["fldItem"].ToString() == "float vector")
			{
				this.minTextBox.Text = this.maxTextBox.Text = "";
				this.optionalGroupBox.Enabled = false;
			}
			else
				this.optionalGroupBox.Enabled = true;

			this.DoCheck();
		}

		private void Changed_DoCheck(object sender, System.EventArgs e)
		{
			this.DoCheck();
		}

		private void okButton_Click(object sender, System.EventArgs e)
		{
			this.parameterRow["fldName"] = this.nameTextBox.Text;
			this.parameterRow["fldDescription"] = this.descriptionTextBox.Text;
			this.parameterRow["fldParameterTypeID"] = this.typeComboBox.SelectedValue;
			this.parameterRow["fldParameterType"] = ((DataRowView)this.typeComboBox.SelectedItem).Row["fldItem"].ToString();
			
			if (this.optionalGroupBox.Enabled) {
				this.parameterRow["fldMinimum"] = this.minTextBox.Text;
				this.parameterRow["fldMaximum"] = this.maxTextBox.Text; }
			else
				this.parameterRow["fldMinimum"] = this.parameterRow["fldMaximum"] = DBNull.Value;
		}
	}
}
