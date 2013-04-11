using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace ModuleUpdater.Dialogs
{
	/// <summary>
	/// Summary description for ClassificationDialog.
	/// </summary>
	public class ClassificationDialog : System.Windows.Forms.Form
	{
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.Button cancelButton;
		private System.Windows.Forms.Button okButton;
		private System.Windows.Forms.ComboBox structureComboBox;
		private System.Windows.Forms.Label structureLabel;
		private System.Windows.Forms.Label substructureLabel;
		private System.Windows.Forms.ComboBox substructureComboBox;
		private System.Windows.Forms.Label celltypeLabel;
		private System.Windows.Forms.ComboBox celltypeComboBox;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		private DataSet versionDataSet;
		private DataRow classificationRow;
		private bool addMode;

		public ClassificationDialog(bool addMode, ref DataRow classificationRow, ref DataSet versionDataSet)
		{
			// Required for Windows Form Designer support
			InitializeComponent();
			EnableVisualStyles.Enable(this);

			this.addMode = addMode;
			this.classificationRow = classificationRow;
			this.versionDataSet = versionDataSet;

			this.structureComboBox.DataSource = versionDataSet.Tables["tblStructure"];
			this.substructureComboBox.DataSource = versionDataSet.Tables["tblSubstructure"];
			this.celltypeComboBox.DataSource = versionDataSet.Tables["tblCellType"];

			if (addMode)
			{
				this.okButton.Text = "Add";
				this.Text = "Add New Classification";

				this.substructureComboBox.SelectedIndex = 0;
			}
			else
			{
				this.okButton.Text = "Update";
				this.Text = "Update Classification";

				this.structureComboBox.SelectedValue = long.Parse(this.classificationRow["fldStructureID"].ToString());

				if (this.classificationRow["fldSubstructureID"].ToString() != "")
					this.substructureComboBox.SelectedValue = long.Parse(this.classificationRow["fldSubstructureID"].ToString());
				else
					this.substructureComboBox.SelectedIndex = 0;

				if (classificationRow["fldCellTypeID"].ToString() != "")
					this.celltypeComboBox.SelectedValue = long.Parse(this.classificationRow["fldCellTypeID"].ToString());
				else
					this.celltypeComboBox.SelectedIndex = 0;
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
			this.cancelButton = new System.Windows.Forms.Button();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.structureComboBox = new System.Windows.Forms.ComboBox();
			this.structureLabel = new System.Windows.Forms.Label();
			this.substructureLabel = new System.Windows.Forms.Label();
			this.substructureComboBox = new System.Windows.Forms.ComboBox();
			this.celltypeLabel = new System.Windows.Forms.Label();
			this.celltypeComboBox = new System.Windows.Forms.ComboBox();
			this.okButton = new System.Windows.Forms.Button();
			this.groupBox1.SuspendLayout();
			this.SuspendLayout();
			// 
			// cancelButton
			// 
			this.cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.cancelButton.Location = new System.Drawing.Point(368, 128);
			this.cancelButton.Name = "cancelButton";
			this.cancelButton.Size = new System.Drawing.Size(72, 24);
			this.cancelButton.TabIndex = 0;
			this.cancelButton.Text = "Cancel";
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.celltypeComboBox);
			this.groupBox1.Controls.Add(this.celltypeLabel);
			this.groupBox1.Controls.Add(this.substructureComboBox);
			this.groupBox1.Controls.Add(this.substructureLabel);
			this.groupBox1.Controls.Add(this.structureComboBox);
			this.groupBox1.Controls.Add(this.structureLabel);
			this.groupBox1.Location = new System.Drawing.Point(8, 8);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(432, 112);
			this.groupBox1.TabIndex = 1;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Classification Information";
			// 
			// structureComboBox
			// 
			this.structureComboBox.DisplayMember = "fldStructureName";
			this.structureComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.structureComboBox.Location = new System.Drawing.Point(88, 24);
			this.structureComboBox.Name = "structureComboBox";
			this.structureComboBox.Size = new System.Drawing.Size(328, 21);
			this.structureComboBox.TabIndex = 0;
			this.structureComboBox.ValueMember = "fldStructureID";
			this.structureComboBox.SelectedIndexChanged += new System.EventHandler(this.structureComboBox_SelectedIndexChanged);
			// 
			// structureLabel
			// 
			this.structureLabel.Location = new System.Drawing.Point(16, 24);
			this.structureLabel.Name = "structureLabel";
			this.structureLabel.Size = new System.Drawing.Size(56, 16);
			this.structureLabel.TabIndex = 1;
			this.structureLabel.Text = "Structure:";
			this.structureLabel.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
			// 
			// substructureLabel
			// 
			this.substructureLabel.Location = new System.Drawing.Point(16, 48);
			this.substructureLabel.Name = "substructureLabel";
			this.substructureLabel.Size = new System.Drawing.Size(72, 16);
			this.substructureLabel.TabIndex = 2;
			this.substructureLabel.Text = "Substructure:";
			// 
			// substructureComboBox
			// 
			this.substructureComboBox.DisplayMember = "fldSubstructureName";
			this.substructureComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.substructureComboBox.Location = new System.Drawing.Point(88, 48);
			this.substructureComboBox.Name = "substructureComboBox";
			this.substructureComboBox.Size = new System.Drawing.Size(328, 21);
			this.substructureComboBox.TabIndex = 3;
			this.substructureComboBox.ValueMember = "fldSubstructureID";
			this.substructureComboBox.SelectedIndexChanged += new System.EventHandler(this.substructureComboBox_SelectedIndexChanged);
			// 
			// celltypeLabel
			// 
			this.celltypeLabel.Location = new System.Drawing.Point(16, 72);
			this.celltypeLabel.Name = "celltypeLabel";
			this.celltypeLabel.Size = new System.Drawing.Size(56, 16);
			this.celltypeLabel.TabIndex = 4;
			this.celltypeLabel.Text = "Cell Type:";
			// 
			// celltypeComboBox
			// 
			this.celltypeComboBox.DisplayMember = "fldCellTypeName";
			this.celltypeComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.celltypeComboBox.Location = new System.Drawing.Point(88, 72);
			this.celltypeComboBox.Name = "celltypeComboBox";
			this.celltypeComboBox.Size = new System.Drawing.Size(328, 21);
			this.celltypeComboBox.TabIndex = 5;
			this.celltypeComboBox.ValueMember = "fldCellTypeID";
			this.celltypeComboBox.SelectedIndexChanged += new System.EventHandler(this.celltypeComboBox_SelectedIndexChanged);
			// 
			// okButton
			// 
			this.okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.okButton.Location = new System.Drawing.Point(280, 128);
			this.okButton.Name = "okButton";
			this.okButton.Size = new System.Drawing.Size(75, 24);
			this.okButton.TabIndex = 2;
			this.okButton.Text = "OK";
			this.okButton.Click += new System.EventHandler(this.okButton_Click);
			// 
			// ClassificationDialog
			// 
			this.AcceptButton = this.okButton;
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.CancelButton = this.cancelButton;
			this.ClientSize = new System.Drawing.Size(450, 160);
			this.Controls.Add(this.okButton);
			this.Controls.Add(this.groupBox1);
			this.Controls.Add(this.cancelButton);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "ClassificationDialog";
			this.ShowInTaskbar = false;
			this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "ClassificationDialog";
			this.TopMost = true;
			this.groupBox1.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		private void DoCheck()
		{
			if (this.addMode)
			{
				if (this.structureComboBox.SelectedIndex != 0)
					this.okButton.Enabled = true;
				else
					this.okButton.Enabled = false;
			}
			else
			{
				bool minimumMet = false;
				bool structureOn = false;
				bool substructureOn = false;
				bool celltypeOn = false;

				if (this.structureComboBox.SelectedIndex != 0)
					minimumMet = true;

				if (((DataRowView)this.structureComboBox.SelectedItem).Row["fldStructureID"].ToString() != this.classificationRow["fldStructureID"].ToString())
					structureOn = true;

				if (this.classificationRow["fldSubstructureID"].ToString() == "")
				{
					if (this.substructureComboBox.SelectedIndex != 0)
						substructureOn = true;
				}
				else if (((DataRowView)this.substructureComboBox.SelectedItem).Row["fldSubstructureID"].ToString() != this.classificationRow["fldSubstructureID"].ToString())
					substructureOn = true;

				if (this.classificationRow["fldCellTypeID"].ToString() == "")
				{
					if (this.celltypeComboBox.SelectedIndex != 0)
						celltypeOn = true;
				}
				else if (((DataRowView)this.celltypeComboBox.SelectedItem).Row["fldCellTypeID"].ToString() != this.classificationRow["fldCellTypeID"].ToString())
					celltypeOn = true;

				if (minimumMet && (structureOn || substructureOn || celltypeOn))
					this.okButton.Enabled = true;
				else
					this.okButton.Enabled = false;
			}
		}
		
		private void structureComboBox_SelectedIndexChanged(object sender, System.EventArgs e)
		{
			this.versionDataSet.Tables["tblSubstructure"].DefaultView.RowFilter = "fldSubstructureStructureID = " + this.structureComboBox.SelectedValue + " OR fldSubstructureID = 0";
			this.versionDataSet.Tables["tblCellType"].DefaultView.RowFilter = "fldCellTypeSubstructureID = " + this.substructureComboBox.SelectedValue + " OR fldCellTypeID = 0";
			this.substructureComboBox.SelectedIndex = 0;

			this.DoCheck();
		}

		private void substructureComboBox_SelectedIndexChanged(object sender, System.EventArgs e)
		{
			this.versionDataSet.Tables["tblCellType"].DefaultView.RowFilter = "fldCellTypeSubstructureID = " + this.substructureComboBox.SelectedValue + " OR fldCellTypeID = 0";
			this.celltypeComboBox.SelectedIndex = 0;

			this.DoCheck();
		}

		private void celltypeComboBox_SelectedIndexChanged(object sender, System.EventArgs e)
		{
			this.DoCheck();
		}

		private void okButton_Click(object sender, System.EventArgs e)
		{
			this.classificationRow["fldVersionID"] = this.versionDataSet.Tables["tblVersion"].Rows[0]["fldVersionID"];
			this.classificationRow["fldStructureID"] = ((DataRowView)this.structureComboBox.SelectedItem).Row["fldStructureID"];
			this.classificationRow["fldStructure"] = ((DataRowView)this.structureComboBox.SelectedItem).Row["fldStructureName"].ToString();

			if (this.substructureComboBox.SelectedIndex != 0)
			{
				this.classificationRow["fldSubstructureID"] = ((DataRowView)this.substructureComboBox.SelectedItem).Row["fldSubstructureID"];
				this.classificationRow["fldSubstructure"] = ((DataRowView)this.substructureComboBox.SelectedItem).Row["fldSubstructureName"].ToString();
			}
			else
				this.classificationRow["fldSubstructureID"] = this.classificationRow["fldSubstructure"] = DBNull.Value;

			if (this.celltypeComboBox.SelectedIndex != 0)
			{
				this.classificationRow["fldCellTypeID"] = ((DataRowView)this.celltypeComboBox.SelectedItem).Row["fldCellTypeID"];
				this.classificationRow["fldCellType"] = ((DataRowView)this.celltypeComboBox.SelectedItem).Row["fldCellTypeName"].ToString();
			}
			else
				this.classificationRow["fldCellTypeID"] = this.classificationRow["fldCellType"] = DBNull.Value;
		}
	}
}
