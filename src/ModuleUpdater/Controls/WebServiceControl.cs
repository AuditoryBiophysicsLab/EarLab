using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;

namespace ModuleUpdater.Controls
{
	/// <summary>
	/// Summary description for WebServiceControl.
	/// </summary>
	public class WebServiceControl : System.Windows.Forms.UserControl
	{
		private ModuleUpdater.Controls.ModulesControl modulesControl;
		private ModuleUpdater.Controls.VersionsControl versionsControl;
		private ModuleUpdater.Controls.VersionControl versionControl;

		//private ModuleUpdater.Controls.VersionControl versionControl;
		/// <summary> 
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public WebServiceControl()
		{
			// This call is required by the Windows.Forms Form Designer.
			InitializeComponent();
			EnableVisualStyles.Enable(this);
			this.modulesControl.BringToFront();
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
			this.modulesControl = new ModuleUpdater.Controls.ModulesControl();
			this.versionsControl = new ModuleUpdater.Controls.VersionsControl();
			this.SuspendLayout();
			// 
			// modulesControl
			// 
			this.modulesControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.modulesControl.Location = new System.Drawing.Point(0, 0);
			this.modulesControl.Name = "modulesControl";
			this.modulesControl.Size = new System.Drawing.Size(648, 368);
			this.modulesControl.TabIndex = 0;
			this.modulesControl.NavigateClicked += new ModuleUpdater.Controls.NavigateClickedHandler(this.modulesControl_NavigateClicked);
			// 
			// versionsControl
			// 
			this.versionsControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.versionsControl.Location = new System.Drawing.Point(0, 0);
			this.versionsControl.ModuleID = ((long)(0));
			this.versionsControl.Name = "versionsControl";
			this.versionsControl.Size = new System.Drawing.Size(648, 368);
			this.versionsControl.TabIndex = 1;
			this.versionsControl.NavigateClicked += new ModuleUpdater.Controls.NavigateClickedHandler(this.versionsControl_NavigateClicked);
			// 
			// WebServiceControl
			// 
			this.Controls.Add(this.modulesControl);
			this.Controls.Add(this.versionsControl);
			this.Name = "WebServiceControl";
			this.Size = new System.Drawing.Size(648, 368);
			this.ResumeLayout(false);

		}
		#endregion

		private void modulesControl_NavigateClicked(object sender, string type, object variable)
		{
			this.Cursor = Cursors.WaitCursor;
			if (type == "List")
			{
				this.versionsControl.BringToFront();
				this.versionsControl.ModuleID = long.Parse(variable.ToString());
			}
			else if (type == "Add")
			{
				this.versionControl = new VersionControl(true, 0);
				this.versionControl.Location = new System.Drawing.Point(0, 0);
				this.versionControl.Name = "versionControl";
				this.versionControl.Size = new System.Drawing.Size(this.ClientSize.Width, this.ClientSize.Height);
				this.versionControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
					| System.Windows.Forms.AnchorStyles.Left) 
					| System.Windows.Forms.AnchorStyles.Right)));
				this.versionControl.TabIndex = 2;
				this.versionControl.NavigateClicked += new ModuleUpdater.Controls.NavigateClickedHandler(this.versionControl_NavigateClicked);

				this.Controls.Add(this.versionControl);
				this.versionControl.BringToFront();
			}
			this.Cursor = Cursors.Default;
		}

		private void versionsControl_NavigateClicked(object sender, string type, object variable)
		{
			this.Cursor = Cursors.WaitCursor;
			if (type == "Back")
			{
				this.modulesControl.SetupDataGrid();
				this.modulesControl.BringToFront();
			}
			else if (type == "View" || type == "Add")
			{
				this.versionControl = null;

				if (type == "Add")
					this.versionControl = new VersionControl(true, long.Parse(variable.ToString()));
				else
					this.versionControl = new VersionControl(false, long.Parse(variable.ToString()));

				this.versionControl.Location = new System.Drawing.Point(0, 0);
				this.versionControl.Name = "versionControl";
				this.versionControl.Size = new System.Drawing.Size(this.ClientSize.Width, this.ClientSize.Height);
				this.versionControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
					| System.Windows.Forms.AnchorStyles.Left) 
					| System.Windows.Forms.AnchorStyles.Right)));
				this.versionControl.TabIndex = 2;
				this.versionControl.NavigateClicked += new ModuleUpdater.Controls.NavigateClickedHandler(this.versionControl_NavigateClicked);

				this.Controls.Add(this.versionControl);
				this.versionControl.BringToFront();
			}
			this.Cursor = Cursors.Default;
		}

		private void versionControl_NavigateClicked(object sender, string type, object variable)
		{
			this.Cursor = Cursors.WaitCursor;
			if (type == "Back")
			{
				if (this.versionControl.Status == VersionControl.VersionControlStatus.AddModule)
				{
					this.modulesControl.SetupDataGrid();
					this.modulesControl.BringToFront();
				}
				else
				{
					this.versionsControl.ModuleID = long.Parse(variable.ToString());
					this.versionsControl.BringToFront();
				}
			}
			this.Cursor = Cursors.Default;
		}
	}
}
