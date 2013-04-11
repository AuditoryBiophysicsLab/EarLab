using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;

namespace ModuleUpdater.Controls
{
	public delegate void NavigateClickedHandler(object sender, string type, object variable);

	/// <summary>
	/// Summary description for ModulesControl.
	/// </summary>
	public class ModulesControl : System.Windows.Forms.UserControl
	{
		/// <summary> 
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private ModuleUpdater.InheritedControls.ColorUpdateViewDeleteDataGrid modulesDataGrid;
		private System.Windows.Forms.DataGridTableStyle modulesStyle;
		private ModuleUpdater.InheritedControls.ColorDataGridLabelColumn nameColumn;
		private System.Windows.Forms.GroupBox modulesGroup;
		private ModuleUpdater.InheritedControls.ColorDataGridLinkColumn listColumn;
		private System.Windows.Forms.LinkLabel addLinkLabel;

		private DataSet modulesDataSet;

		public ModulesControl()
		{
			// This call is required by the Windows.Forms Form Designer.
			InitializeComponent();
			EnableVisualStyles.Enable(this);

			this.SetupDataGrid();
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
			this.modulesDataGrid = new ModuleUpdater.InheritedControls.ColorUpdateViewDeleteDataGrid();
			this.modulesStyle = new System.Windows.Forms.DataGridTableStyle();
			this.nameColumn = new ModuleUpdater.InheritedControls.ColorDataGridLabelColumn();
			this.listColumn = new ModuleUpdater.InheritedControls.ColorDataGridLinkColumn();
			this.modulesGroup = new System.Windows.Forms.GroupBox();
			this.addLinkLabel = new System.Windows.Forms.LinkLabel();
			((System.ComponentModel.ISupportInitialize)(this.modulesDataGrid)).BeginInit();
			this.modulesGroup.SuspendLayout();
			this.SuspendLayout();
			// 
			// modulesDataGrid
			// 
			this.modulesDataGrid.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.modulesDataGrid.CaptionVisible = false;
			this.modulesDataGrid.DataMember = "";
			this.modulesDataGrid.HeaderForeColor = System.Drawing.SystemColors.ControlText;
			this.modulesDataGrid.LinkColor = System.Drawing.SystemColors.ActiveCaption;
			this.modulesDataGrid.Location = new System.Drawing.Point(8, 16);
			this.modulesDataGrid.Name = "modulesDataGrid";
			this.modulesDataGrid.ParentRowsVisible = false;
			this.modulesDataGrid.ReadOnly = true;
			this.modulesDataGrid.RowHeadersVisible = false;
			this.modulesDataGrid.Size = new System.Drawing.Size(648, 232);
			this.modulesDataGrid.TabIndex = 0;
			this.modulesDataGrid.TableStyles.AddRange(new System.Windows.Forms.DataGridTableStyle[] {
																										this.modulesStyle});
			this.modulesDataGrid.LinkLabelClicked += new ModuleUpdater.InheritedControls.ColorUpdateViewDeleteDataGrid.LinkLabelClickedHandler(this.modulesDataGrid_LinkLabelClicked);
			// 
			// modulesStyle
			// 
			this.modulesStyle.DataGrid = this.modulesDataGrid;
			this.modulesStyle.GridColumnStyles.AddRange(new System.Windows.Forms.DataGridColumnStyle[] {
																										   this.nameColumn,
																										   this.listColumn});
			this.modulesStyle.HeaderForeColor = System.Drawing.SystemColors.ControlText;
			this.modulesStyle.LinkColor = System.Drawing.SystemColors.Highlight;
			this.modulesStyle.MappingName = "tblModule";
			this.modulesStyle.ReadOnly = true;
			this.modulesStyle.RowHeadersVisible = false;
			// 
			// nameColumn
			// 
			this.nameColumn.Format = "";
			this.nameColumn.FormatInfo = null;
			this.nameColumn.HeaderText = "Name";
			this.nameColumn.MappingName = "fldName";
			this.nameColumn.ReadOnly = true;
			this.nameColumn.Width = 593;
			// 
			// listColumn
			// 
			this.listColumn.Alignment = System.Windows.Forms.HorizontalAlignment.Center;
			this.listColumn.Format = "List";
			this.listColumn.FormatInfo = null;
			this.listColumn.HeaderText = "List";
			this.listColumn.MappingName = "fldList";
			this.listColumn.NullText = "List";
			this.listColumn.ReadOnly = true;
			this.listColumn.Width = 50;
			// 
			// modulesGroup
			// 
			this.modulesGroup.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.modulesGroup.Controls.Add(this.modulesDataGrid);
			this.modulesGroup.Location = new System.Drawing.Point(0, 0);
			this.modulesGroup.Name = "modulesGroup";
			this.modulesGroup.Size = new System.Drawing.Size(664, 256);
			this.modulesGroup.TabIndex = 1;
			this.modulesGroup.TabStop = false;
			this.modulesGroup.Text = "Modules in EarLab Module Database";
			// 
			// addLinkLabel
			// 
			this.addLinkLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.addLinkLabel.LinkBehavior = System.Windows.Forms.LinkBehavior.HoverUnderline;
			this.addLinkLabel.LinkColor = System.Drawing.SystemColors.ActiveCaption;
			this.addLinkLabel.Location = new System.Drawing.Point(568, 256);
			this.addLinkLabel.Name = "addLinkLabel";
			this.addLinkLabel.Size = new System.Drawing.Size(96, 16);
			this.addLinkLabel.TabIndex = 2;
			this.addLinkLabel.TabStop = true;
			this.addLinkLabel.Text = "Add New Module";
			this.addLinkLabel.TextAlign = System.Drawing.ContentAlignment.TopRight;
			this.addLinkLabel.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.addLinkLabel_LinkClicked);
			// 
			// ModulesControl
			// 
			this.Controls.Add(this.addLinkLabel);
			this.Controls.Add(this.modulesGroup);
			this.Name = "ModulesControl";
			this.Size = new System.Drawing.Size(664, 288);
			((System.ComponentModel.ISupportInitialize)(this.modulesDataGrid)).EndInit();
			this.modulesGroup.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion
		
		public void SetupDataGrid()
		{
			ModuleService.EarDevModuleService moduleService = new ModuleService.EarDevModuleService();
			moduleService.Timeout = 100000;
			modulesDataSet = moduleService.GetModuleList();

			modulesDataSet.Tables["tblModule"].Columns.Add("fldList", typeof(string));
			this.modulesDataGrid.DataSource = modulesDataSet.Tables["tblModule"].DefaultView;
		}
		
		public event NavigateClickedHandler NavigateClicked;
		
		private void modulesDataGrid_LinkLabelClicked(string columnName, System.Data.DataRow row)
		{
			if (columnName == "fldList" && this.NavigateClicked != null)
				this.NavigateClicked(this, "List", row["fldModuleID"]);
		}

		private void addLinkLabel_LinkClicked(object sender, System.Windows.Forms.LinkLabelLinkClickedEventArgs e)
		{
			if (this.NavigateClicked != null)
				this.NavigateClicked(this, "Add", null);
		}

		
	}
}
