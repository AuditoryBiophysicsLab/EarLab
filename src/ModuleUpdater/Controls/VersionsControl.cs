using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;

namespace ModuleUpdater.Controls
{
	/// <summary>
	/// Summary description for ModulesControl.
	/// </summary>
	public class VersionsControl : System.Windows.Forms.UserControl
	{
		/// <summary> 
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private System.Windows.Forms.GroupBox versionsGroup;
		private ModuleUpdater.InheritedControls.ColorUpdateViewDeleteDataGrid versionsDataGrid;

		private long moduleID;
		private ModuleUpdater.InheritedControls.ColorDataGridLabelColumn versionColumn;
		private ModuleUpdater.InheritedControls.ColorDataGridLabelColumn releaseColumn;
		private ModuleUpdater.InheritedControls.ColorDataGridLabelColumn contactColumn;
		private ModuleUpdater.InheritedControls.ColorDataGridLinkColumn viewColumn;
		private System.Windows.Forms.LinkLabel backLinkLabel;
		private System.Windows.Forms.DataGridTableStyle versionsStyle;
		private System.Windows.Forms.LinkLabel addLinkLabel;
		private DataSet versionsDataSet;

		public VersionsControl()
		{
			// This call is required by the Windows.Forms Form Designer.
			InitializeComponent();
			EnableVisualStyles.Enable(this);

			this.versionColumn.Width = this.releaseColumn.Width = 100;
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

		#region Component Designer generated code
		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.versionsDataGrid = new ModuleUpdater.InheritedControls.ColorUpdateViewDeleteDataGrid();
			this.versionsStyle = new System.Windows.Forms.DataGridTableStyle();
			this.versionColumn = new ModuleUpdater.InheritedControls.ColorDataGridLabelColumn();
			this.releaseColumn = new ModuleUpdater.InheritedControls.ColorDataGridLabelColumn();
			this.contactColumn = new ModuleUpdater.InheritedControls.ColorDataGridLabelColumn();
			this.viewColumn = new ModuleUpdater.InheritedControls.ColorDataGridLinkColumn();
			this.versionsGroup = new System.Windows.Forms.GroupBox();
			this.backLinkLabel = new System.Windows.Forms.LinkLabel();
			this.addLinkLabel = new System.Windows.Forms.LinkLabel();
			((System.ComponentModel.ISupportInitialize)(this.versionsDataGrid)).BeginInit();
			this.versionsGroup.SuspendLayout();
			this.SuspendLayout();
			// 
			// versionsDataGrid
			// 
			this.versionsDataGrid.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.versionsDataGrid.CaptionVisible = false;
			this.versionsDataGrid.DataMember = "";
			this.versionsDataGrid.HeaderForeColor = System.Drawing.SystemColors.ControlText;
			this.versionsDataGrid.LinkColor = System.Drawing.SystemColors.ActiveCaption;
			this.versionsDataGrid.Location = new System.Drawing.Point(8, 16);
			this.versionsDataGrid.Name = "versionsDataGrid";
			this.versionsDataGrid.ParentRowsVisible = false;
			this.versionsDataGrid.ReadOnly = true;
			this.versionsDataGrid.RowHeadersVisible = false;
			this.versionsDataGrid.Size = new System.Drawing.Size(648, 232);
			this.versionsDataGrid.TabIndex = 0;
			this.versionsDataGrid.TableStyles.AddRange(new System.Windows.Forms.DataGridTableStyle[] {
																										 this.versionsStyle});
			this.versionsDataGrid.LinkLabelClicked += new ModuleUpdater.InheritedControls.ColorUpdateViewDeleteDataGrid.LinkLabelClickedHandler(this.modulesDataGrid_LinkLabelClicked);
			// 
			// versionsStyle
			// 
			this.versionsStyle.DataGrid = this.versionsDataGrid;
			this.versionsStyle.GridColumnStyles.AddRange(new System.Windows.Forms.DataGridColumnStyle[] {
																											this.versionColumn,
																											this.releaseColumn,
																											this.contactColumn,
																											this.viewColumn});
			this.versionsStyle.HeaderForeColor = System.Drawing.SystemColors.ControlText;
			this.versionsStyle.LinkColor = System.Drawing.SystemColors.Highlight;
			this.versionsStyle.MappingName = "tblVersion";
			this.versionsStyle.PreferredColumnWidth = 100;
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
			this.versionColumn.Width = 143;
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
			this.contactColumn.Width = 320;
			// 
			// viewColumn
			// 
			this.viewColumn.Alignment = System.Windows.Forms.HorizontalAlignment.Center;
			this.viewColumn.Format = "View";
			this.viewColumn.FormatInfo = null;
			this.viewColumn.HeaderText = "View Info";
			this.viewColumn.MappingName = "fldView";
			this.viewColumn.NullText = "View";
			this.viewColumn.ReadOnly = true;
			this.viewColumn.Width = 80;
			// 
			// versionsGroup
			// 
			this.versionsGroup.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.versionsGroup.Controls.Add(this.versionsDataGrid);
			this.versionsGroup.Location = new System.Drawing.Point(0, 0);
			this.versionsGroup.Name = "versionsGroup";
			this.versionsGroup.Size = new System.Drawing.Size(664, 256);
			this.versionsGroup.TabIndex = 1;
			this.versionsGroup.TabStop = false;
			this.versionsGroup.Text = "Versions in EarLab Module Database";
			// 
			// backLinkLabel
			// 
			this.backLinkLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.backLinkLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.backLinkLabel.LinkBehavior = System.Windows.Forms.LinkBehavior.HoverUnderline;
			this.backLinkLabel.LinkColor = System.Drawing.SystemColors.ActiveCaption;
			this.backLinkLabel.Location = new System.Drawing.Point(0, 264);
			this.backLinkLabel.Name = "backLinkLabel";
			this.backLinkLabel.Size = new System.Drawing.Size(72, 16);
			this.backLinkLabel.TabIndex = 3;
			this.backLinkLabel.TabStop = true;
			this.backLinkLabel.Text = "< back";
			this.backLinkLabel.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
			this.backLinkLabel.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.backLinkLabel_LinkClicked);
			// 
			// addLinkLabel
			// 
			this.addLinkLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.addLinkLabel.LinkBehavior = System.Windows.Forms.LinkBehavior.HoverUnderline;
			this.addLinkLabel.LinkColor = System.Drawing.SystemColors.ActiveCaption;
			this.addLinkLabel.Location = new System.Drawing.Point(568, 256);
			this.addLinkLabel.Name = "addLinkLabel";
			this.addLinkLabel.Size = new System.Drawing.Size(96, 16);
			this.addLinkLabel.TabIndex = 4;
			this.addLinkLabel.TabStop = true;
			this.addLinkLabel.Text = "Add New Version";
			this.addLinkLabel.TextAlign = System.Drawing.ContentAlignment.TopRight;
			this.addLinkLabel.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.addLinkLabel_LinkClicked);
			// 
			// VersionsControl
			// 
			this.Controls.Add(this.addLinkLabel);
			this.Controls.Add(this.backLinkLabel);
			this.Controls.Add(this.versionsGroup);
			this.Name = "VersionsControl";
			this.Size = new System.Drawing.Size(664, 288);
			((System.ComponentModel.ISupportInitialize)(this.versionsDataGrid)).EndInit();
			this.versionsGroup.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		public long ModuleID
		{
			get { return this.moduleID; }
			set
			{
				if (value != 0)
				{
					this.moduleID = value;
					this.SetupControl();
				}
			}
		}

		private void SetupControl()
		{
			ModuleService.EarDevModuleService moduleService = new ModuleService.EarDevModuleService();
			this.versionsDataSet = moduleService.GetVersionList(this.moduleID);

			this.versionsDataSet.Tables["tblVersion"].Columns.Add("fldView", typeof(string));
			this.versionsDataGrid.DataSource = this.versionsDataSet.Tables["tblVersion"].DefaultView;

			this.versionsGroup.Text = "Versions in '" + this.versionsDataSet.Tables["tblModule"].Rows[0]["fldName"].ToString() + "' Module";
		}

		public event NavigateClickedHandler NavigateClicked;
		
		private void modulesDataGrid_LinkLabelClicked(string columnName, System.Data.DataRow row)
		{
			if (columnName == "fldView" && this.NavigateClicked != null)
				this.NavigateClicked(this, "View", row["fldVersionID"]);
		}

		private void backLinkLabel_LinkClicked(object sender, System.Windows.Forms.LinkLabelLinkClickedEventArgs e)
		{
			if (this.NavigateClicked != null)
				this.NavigateClicked(this, "Back", null);
		}

		private void addLinkLabel_LinkClicked(object sender, System.Windows.Forms.LinkLabelLinkClickedEventArgs e)
		{
			Dialogs.VersionAddDialog addDialog = new Dialogs.VersionAddDialog(ref this.versionsDataSet);

			if (addDialog.ShowDialog(this) == DialogResult.OK && this.NavigateClicked != null)
				this.NavigateClicked(this, "Add", addDialog.versionID);
		}

	}
}
