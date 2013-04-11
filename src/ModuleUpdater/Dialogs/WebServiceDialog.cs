using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace ModuleUpdater.Dialogs
{
	/// <summary>
	/// Summary description for WebServiceDialog.
	/// </summary>
	public class WebServiceDialog : System.Windows.Forms.Form
	{
		private ModuleUpdater.Controls.WebServiceControl webServiceControl;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public WebServiceDialog()
		{
			// Required for Windows Form Designer support
			InitializeComponent();
			EnableVisualStyles.Enable(this);
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
			this.webServiceControl = new ModuleUpdater.Controls.WebServiceControl();
			this.SuspendLayout();
			// 
			// webServiceControl
			// 
			this.webServiceControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.webServiceControl.Location = new System.Drawing.Point(8, 8);
			this.webServiceControl.Name = "webServiceControl";
			this.webServiceControl.Size = new System.Drawing.Size(648, 368);
			this.webServiceControl.TabIndex = 0;
			// 
			// WebServiceDialog
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(664, 382);
			this.Controls.Add(this.webServiceControl);
			this.MinimumSize = new System.Drawing.Size(650, 410);
			this.Name = "WebServiceDialog";
			this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Show;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Module Database Updater";
			this.ResumeLayout(false);

		}
		#endregion
	}
}
