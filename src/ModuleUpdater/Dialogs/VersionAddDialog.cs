using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace ModuleUpdater.Dialogs
{
	/// <summary>
	/// Summary description for VersionAddDialog.
	/// </summary>
	public class VersionAddDialog : System.Windows.Forms.Form
	{
		private ModuleUpdater.InheritedControls.ColorFixedDataGrid versionsDataGrid;
		private System.Windows.Forms.GroupBox datagridGroup;
		private System.Windows.Forms.Button okButton;
		private System.Windows.Forms.Button cancelButton;
		private System.Windows.Forms.DataGridTableStyle versionsStyle;
		private ModuleUpdater.InheritedControls.ColorDataGridSelectColumn versionColumn;
		private ModuleUpdater.InheritedControls.ColorDataGridSelectColumn releaseColumn;
		private ModuleUpdater.InheritedControls.ColorDataGridSelectColumn contactColumn;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public long versionID;

		public VersionAddDialog(ref DataSet versionsDataSet)
		{
			// Required for Windows Form Designer support
			InitializeComponent();

			this.versionsDataGrid.DataSource = versionsDataSet.Tables["tblVersion"].DefaultView;
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
			this.versionsDataGrid = new ModuleUpdater.InheritedControls.ColorFixedDataGrid();
			this.versionsStyle = new System.Windows.Forms.DataGridTableStyle();
			this.versionColumn = new ModuleUpdater.InheritedControls.ColorDataGridSelectColumn();
			this.releaseColumn = new ModuleUpdater.InheritedControls.ColorDataGridSelectColumn();
			this.contactColumn = new ModuleUpdater.InheritedControls.ColorDataGridSelectColumn();
			this.datagridGroup = new System.Windows.Forms.GroupBox();
			this.okButton = new System.Windows.Forms.Button();
			this.cancelButton = new System.Windows.Forms.Button();
			((System.ComponentModel.ISupportInitialize)(this.versionsDataGrid)).BeginInit();
			this.datagridGroup.SuspendLayout();
			this.SuspendLayout();
			// 
			// versionsDataGrid
			// 
			this.versionsDataGrid.CaptionVisible = false;
			this.versionsDataGrid.DataMember = "";
			this.versionsDataGrid.HeaderForeColor = System.Drawing.SystemColors.ControlText;
			this.versionsDataGrid.Location = new System.Drawing.Point(8, 16);
			this.versionsDataGrid.Name = "versionsDataGrid";
			this.versionsDataGrid.ReadOnly = true;
			this.versionsDataGrid.RowHeadersVisible = false;
			this.versionsDataGrid.Size = new System.Drawing.Size(600, 112);
			this.versionsDataGrid.TabIndex = 0;
			this.versionsDataGrid.TableStyles.AddRange(new System.Windows.Forms.DataGridTableStyle[] {
																										 this.versionsStyle});
			// 
			// versionsStyle
			// 
			this.versionsStyle.DataGrid = this.versionsDataGrid;
			this.versionsStyle.GridColumnStyles.AddRange(new System.Windows.Forms.DataGridColumnStyle[] {
																											this.versionColumn,
																											this.releaseColumn,
																											this.contactColumn});
			this.versionsStyle.HeaderForeColor = System.Drawing.SystemColors.ControlText;
			this.versionsStyle.MappingName = "tblVersion";
			this.versionsStyle.ReadOnly = true;
			this.versionsStyle.RowHeadersVisible = false;
			// 
			// versionColumn
			// 
			this.versionColumn.Alignment = System.Windows.Forms.HorizontalAlignment.Center;
			this.versionColumn.Format = "";
			this.versionColumn.FormatInfo = null;
			this.versionColumn.HeaderText = "Version Number";
			this.versionColumn.MappingName = "fldVersionNumber";
			this.versionColumn.ReadOnly = true;
			this.versionColumn.Width = 200;
			// 
			// releaseColumn
			// 
			this.releaseColumn.Alignment = System.Windows.Forms.HorizontalAlignment.Center;
			this.releaseColumn.Format = "";
			this.releaseColumn.FormatInfo = null;
			this.releaseColumn.HeaderText = "Release Date";
			this.releaseColumn.MappingName = "fldReleaseDate";
			this.releaseColumn.ReadOnly = true;
			this.releaseColumn.Width = 75;
			// 
			// contactColumn
			// 
			this.contactColumn.Alignment = System.Windows.Forms.HorizontalAlignment.Center;
			this.contactColumn.Format = "";
			this.contactColumn.FormatInfo = null;
			this.contactColumn.HeaderText = "Contact";
			this.contactColumn.MappingName = "fldContactName";
			this.contactColumn.ReadOnly = true;
			this.contactColumn.Width = 300;
			// 
			// datagridGroup
			// 
			this.datagridGroup.Controls.Add(this.versionsDataGrid);
			this.datagridGroup.Location = new System.Drawing.Point(8, 8);
			this.datagridGroup.Name = "datagridGroup";
			this.datagridGroup.Size = new System.Drawing.Size(616, 136);
			this.datagridGroup.TabIndex = 1;
			this.datagridGroup.TabStop = false;
			this.datagridGroup.Text = "Module Versions";
			// 
			// okButton
			// 
			this.okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.okButton.Location = new System.Drawing.Point(464, 152);
			this.okButton.Name = "okButton";
			this.okButton.TabIndex = 2;
			this.okButton.Text = "OK";
			this.okButton.Click += new System.EventHandler(this.okButton_Click);
			// 
			// cancelButton
			// 
			this.cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.cancelButton.Location = new System.Drawing.Point(552, 152);
			this.cancelButton.Name = "cancelButton";
			this.cancelButton.Size = new System.Drawing.Size(72, 23);
			this.cancelButton.TabIndex = 3;
			this.cancelButton.Text = "Cancel";
			// 
			// VersionAddDialog
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(632, 182);
			this.Controls.Add(this.cancelButton);
			this.Controls.Add(this.okButton);
			this.Controls.Add(this.datagridGroup);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "VersionAddDialog";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Add Module Version";
			((System.ComponentModel.ISupportInitialize)(this.versionsDataGrid)).EndInit();
			this.datagridGroup.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		private void okButton_Click(object sender, System.EventArgs e)
		{
			this.versionID = long.Parse(((DataRowView)versionsDataGrid.BindingContext[versionsDataGrid.DataSource, versionsDataGrid.DataMember].Current).Row["fldVersionID"].ToString());
		}
	}
}
