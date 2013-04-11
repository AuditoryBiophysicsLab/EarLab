using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace MultiViewer
{
	/// <summary>
	/// Summary description for CloseDialog.
	/// </summary>
	public class CloseDialog : System.Windows.Forms.Form
	{
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.CheckBox ulCheckBox;
		private System.Windows.Forms.CheckBox urCheckBox;
		private System.Windows.Forms.CheckBox llCheckBox;
		private System.Windows.Forms.CheckBox lrCheckBox;
		private System.Windows.Forms.Button okButton;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public CloseDialog(bool ulBool, bool urBool, bool llBool, bool lrBool)
		{
			InitializeComponent();
			EarLab.Utilities.EnableVisualStyles.EnableControl(this);

			this.ulCheckBox.Enabled = this.urCheckBox.Enabled =
				this.llCheckBox.Enabled = this.lrCheckBox.Enabled = false;

			if (ulBool)
				this.ulCheckBox.Enabled = true;
			if (urBool)
				this.urCheckBox.Enabled = true;
			if (llBool)
				this.llCheckBox.Enabled = true;
			if (lrBool)
				this.lrCheckBox.Enabled = true;
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
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.ulCheckBox = new System.Windows.Forms.CheckBox();
			this.urCheckBox = new System.Windows.Forms.CheckBox();
			this.llCheckBox = new System.Windows.Forms.CheckBox();
			this.lrCheckBox = new System.Windows.Forms.CheckBox();
			this.okButton = new System.Windows.Forms.Button();
			this.groupBox1.SuspendLayout();
			this.SuspendLayout();
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.llCheckBox);
			this.groupBox1.Controls.Add(this.ulCheckBox);
			this.groupBox1.Controls.Add(this.urCheckBox);
			this.groupBox1.Controls.Add(this.lrCheckBox);
			this.groupBox1.Location = new System.Drawing.Point(8, 8);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(272, 96);
			this.groupBox1.TabIndex = 0;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "groupBox1";
			// 
			// ulCheckBox
			// 
			this.ulCheckBox.Location = new System.Drawing.Point(16, 24);
			this.ulCheckBox.Name = "ulCheckBox";
			this.ulCheckBox.Size = new System.Drawing.Size(120, 24);
			this.ulCheckBox.TabIndex = 0;
			this.ulCheckBox.Text = "Upper Left Viewer";
			// 
			// urCheckBox
			// 
			this.urCheckBox.Location = new System.Drawing.Point(144, 24);
			this.urCheckBox.Name = "urCheckBox";
			this.urCheckBox.Size = new System.Drawing.Size(126, 24);
			this.urCheckBox.TabIndex = 1;
			this.urCheckBox.Text = "Upper Right Viewer";
			// 
			// llCheckBox
			// 
			this.llCheckBox.Location = new System.Drawing.Point(16, 56);
			this.llCheckBox.Name = "llCheckBox";
			this.llCheckBox.Size = new System.Drawing.Size(120, 24);
			this.llCheckBox.TabIndex = 2;
			this.llCheckBox.Text = "Lower Left Viewer";
			// 
			// lrCheckBox
			// 
			this.lrCheckBox.Location = new System.Drawing.Point(144, 56);
			this.lrCheckBox.Name = "lrCheckBox";
			this.lrCheckBox.Size = new System.Drawing.Size(126, 24);
			this.lrCheckBox.TabIndex = 3;
			this.lrCheckBox.Text = "Lower Right Viewer";
			// 
			// okButton
			// 
			this.okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.okButton.Location = new System.Drawing.Point(208, 112);
			this.okButton.Name = "okButton";
			this.okButton.Size = new System.Drawing.Size(72, 23);
			this.okButton.TabIndex = 1;
			this.okButton.Text = "OK";
			// 
			// CloseDialog
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(290, 144);
			this.Controls.Add(this.okButton);
			this.Controls.Add(this.groupBox1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "CloseDialog";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Close File...";
			this.groupBox1.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		public bool ULChecked
		{
			get
			{
				if (this.ulCheckBox.Checked)
					return true;
				else
					return false;
			}
		}

		public bool URChecked
		{
			get
			{
				if (this.urCheckBox.Checked)
					return true;
				else
					return false;
			}
		}

		public bool LLChecked
		{
			get
			{
				if (this.llCheckBox.Checked)
					return true;
				else
					return false;
			}
		}

		public bool LRChecked
		{
			get
			{
				if (this.lrCheckBox.Checked)
					return true;
				else
					return false;
			}
		}
	}
}
