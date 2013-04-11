using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using EarLab.Utilities;

namespace EarLab.DataViewer
{
	/// <summary>
	/// Summary description for DataViewerDialog.
	/// </summary>
	public class DataViewerDialog : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Button closeButton;
		private System.Windows.Forms.GroupBox applicationGroup;
		private System.Windows.Forms.Label nameLabel;
		private System.Windows.Forms.Label versionLabel;
		private System.Windows.Forms.Label authorsLabel;
		private System.Windows.Forms.Label authors2Label;
		private System.Windows.Forms.Label name2Label;
		private System.Windows.Forms.Label version2Label;
		private System.Windows.Forms.GroupBox fundingGroup;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.PictureBox viewerPictureBox;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public DataViewerDialog()
		{
			// Required for Windows Form Designer support
			InitializeComponent();
			EnableVisualStyles.EnableControl(this);

			this.name2Label.Text = Application.ProductName;
			this.version2Label.Text = Application.ProductVersion;
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
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(DataViewerDialog));
			this.closeButton = new System.Windows.Forms.Button();
			this.viewerPictureBox = new System.Windows.Forms.PictureBox();
			this.applicationGroup = new System.Windows.Forms.GroupBox();
			this.version2Label = new System.Windows.Forms.Label();
			this.name2Label = new System.Windows.Forms.Label();
			this.authors2Label = new System.Windows.Forms.Label();
			this.authorsLabel = new System.Windows.Forms.Label();
			this.versionLabel = new System.Windows.Forms.Label();
			this.nameLabel = new System.Windows.Forms.Label();
			this.fundingGroup = new System.Windows.Forms.GroupBox();
			this.label1 = new System.Windows.Forms.Label();
			this.applicationGroup.SuspendLayout();
			this.fundingGroup.SuspendLayout();
			this.SuspendLayout();
			// 
			// closeButton
			// 
			this.closeButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.closeButton.Location = new System.Drawing.Point(392, 272);
			this.closeButton.Name = "closeButton";
			this.closeButton.Size = new System.Drawing.Size(72, 23);
			this.closeButton.TabIndex = 2;
			this.closeButton.Text = "Close";
			// 
			// viewerPictureBox
			// 
			this.viewerPictureBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.viewerPictureBox.Image = ((System.Drawing.Image)(resources.GetObject("viewerPictureBox.Image")));
			this.viewerPictureBox.Location = new System.Drawing.Point(8, 8);
			this.viewerPictureBox.Name = "viewerPictureBox";
			this.viewerPictureBox.Size = new System.Drawing.Size(456, 80);
			this.viewerPictureBox.TabIndex = 3;
			this.viewerPictureBox.TabStop = false;
			// 
			// applicationGroup
			// 
			this.applicationGroup.Controls.Add(this.version2Label);
			this.applicationGroup.Controls.Add(this.name2Label);
			this.applicationGroup.Controls.Add(this.authors2Label);
			this.applicationGroup.Controls.Add(this.authorsLabel);
			this.applicationGroup.Controls.Add(this.versionLabel);
			this.applicationGroup.Controls.Add(this.nameLabel);
			this.applicationGroup.Location = new System.Drawing.Point(8, 96);
			this.applicationGroup.Name = "applicationGroup";
			this.applicationGroup.Size = new System.Drawing.Size(456, 96);
			this.applicationGroup.TabIndex = 4;
			this.applicationGroup.TabStop = false;
			this.applicationGroup.Text = "Application Info";
			// 
			// version2Label
			// 
			this.version2Label.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.version2Label.Location = new System.Drawing.Point(80, 48);
			this.version2Label.Name = "version2Label";
			this.version2Label.Size = new System.Drawing.Size(360, 16);
			this.version2Label.TabIndex = 5;
			// 
			// name2Label
			// 
			this.name2Label.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.name2Label.Location = new System.Drawing.Point(80, 24);
			this.name2Label.Name = "name2Label";
			this.name2Label.Size = new System.Drawing.Size(360, 16);
			this.name2Label.TabIndex = 4;
			// 
			// authors2Label
			// 
			this.authors2Label.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.authors2Label.Location = new System.Drawing.Point(80, 72);
			this.authors2Label.Name = "authors2Label";
			this.authors2Label.Size = new System.Drawing.Size(360, 16);
			this.authors2Label.TabIndex = 3;
			this.authors2Label.Text = "V. Vajda, analysis code by D. Anderson, S. Deligeorges";
			// 
			// authorsLabel
			// 
			this.authorsLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.authorsLabel.Location = new System.Drawing.Point(16, 72);
			this.authorsLabel.Name = "authorsLabel";
			this.authorsLabel.Size = new System.Drawing.Size(56, 16);
			this.authorsLabel.TabIndex = 2;
			this.authorsLabel.Text = "Authors:";
			// 
			// versionLabel
			// 
			this.versionLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.versionLabel.Location = new System.Drawing.Point(16, 48);
			this.versionLabel.Name = "versionLabel";
			this.versionLabel.Size = new System.Drawing.Size(56, 16);
			this.versionLabel.TabIndex = 1;
			this.versionLabel.Text = "Version:";
			// 
			// nameLabel
			// 
			this.nameLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.nameLabel.Location = new System.Drawing.Point(16, 24);
			this.nameLabel.Name = "nameLabel";
			this.nameLabel.Size = new System.Drawing.Size(48, 16);
			this.nameLabel.TabIndex = 0;
			this.nameLabel.Text = "Name:";
			// 
			// fundingGroup
			// 
			this.fundingGroup.Controls.Add(this.label1);
			this.fundingGroup.Location = new System.Drawing.Point(8, 200);
			this.fundingGroup.Name = "fundingGroup";
			this.fundingGroup.Size = new System.Drawing.Size(456, 64);
			this.fundingGroup.TabIndex = 5;
			this.fundingGroup.TabStop = false;
			this.fundingGroup.Text = "Funding Info";
			// 
			// label1
			// 
			this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.label1.Location = new System.Drawing.Point(8, 24);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(440, 24);
			this.label1.TabIndex = 0;
			this.label1.Text = "Funded through the Human Brain Project program by NIDCD and NIMH";
			this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// DataViewerDialog
			// 
			this.AcceptButton = this.closeButton;
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.CancelButton = this.closeButton;
			this.ClientSize = new System.Drawing.Size(474, 304);
			this.Controls.Add(this.fundingGroup);
			this.Controls.Add(this.applicationGroup);
			this.Controls.Add(this.viewerPictureBox);
			this.Controls.Add(this.closeButton);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "DataViewerDialog";
			this.ShowInTaskbar = false;
			this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "About EarLab DataViewer";
			this.applicationGroup.ResumeLayout(false);
			this.fundingGroup.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion
	}
}
