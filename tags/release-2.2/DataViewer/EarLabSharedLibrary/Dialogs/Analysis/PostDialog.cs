using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace EarLab.Dialogs.Analysis
{
	/// <summary>
	/// Summary description for PostDialog.
	/// </summary>
	public class PostDialog : System.Windows.Forms.Form
	{
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.Button okButton;
		private System.Windows.Forms.Button cancelButton;
		private System.Windows.Forms.TextBox binwidthTextBox;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;

		private double maxBinWidth_Seconds;
		private System.Windows.Forms.Label lblLimits;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public PostDialog(double MaxBinWidth_Seconds)
		{
			InitializeComponent();
			EarLab.Utilities.EnableVisualStyles.EnableControl(this);

			this.okButton.Enabled = false;
			maxBinWidth_Seconds = MaxBinWidth_Seconds;
			lblLimits.Text = "(0.0001 - " + maxBinWidth_Seconds.ToString("0.####") + ") seconds";
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
			this.okButton = new System.Windows.Forms.Button();
			this.cancelButton = new System.Windows.Forms.Button();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.label2 = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			this.binwidthTextBox = new System.Windows.Forms.TextBox();
			this.lblLimits = new System.Windows.Forms.Label();
			this.groupBox1.SuspendLayout();
			this.SuspendLayout();
			// 
			// okButton
			// 
			this.okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.okButton.Location = new System.Drawing.Point(112, 88);
			this.okButton.Name = "okButton";
			this.okButton.Size = new System.Drawing.Size(40, 23);
			this.okButton.TabIndex = 1;
			this.okButton.Text = "OK";
			// 
			// cancelButton
			// 
			this.cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.cancelButton.Location = new System.Drawing.Point(160, 88);
			this.cancelButton.Name = "cancelButton";
			this.cancelButton.Size = new System.Drawing.Size(56, 23);
			this.cancelButton.TabIndex = 2;
			this.cancelButton.Text = "Cancel";
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.lblLimits);
			this.groupBox1.Controls.Add(this.label2);
			this.groupBox1.Controls.Add(this.label1);
			this.groupBox1.Controls.Add(this.binwidthTextBox);
			this.groupBox1.Location = new System.Drawing.Point(8, 8);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(208, 72);
			this.groupBox1.TabIndex = 0;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "User Parameters";
			// 
			// label2
			// 
			this.label2.Location = new System.Drawing.Point(152, 24);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(48, 16);
			this.label2.TabIndex = 2;
			this.label2.Text = "seconds";
			this.label2.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(8, 24);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(56, 16);
			this.label1.TabIndex = 1;
			this.label1.Text = "Bin Width:";
			this.label1.TextAlign = System.Drawing.ContentAlignment.BottomRight;
			// 
			// binwidthTextBox
			// 
			this.binwidthTextBox.Location = new System.Drawing.Point(72, 24);
			this.binwidthTextBox.Name = "binwidthTextBox";
			this.binwidthTextBox.Size = new System.Drawing.Size(72, 20);
			this.binwidthTextBox.TabIndex = 0;
			this.binwidthTextBox.Text = "";
			this.binwidthTextBox.TextChanged += new System.EventHandler(this.binwidthTextBox_TextChanged);
			// 
			// lblLimits
			// 
			this.lblLimits.Location = new System.Drawing.Point(8, 48);
			this.lblLimits.Name = "lblLimits";
			this.lblLimits.Size = new System.Drawing.Size(192, 16);
			this.lblLimits.TabIndex = 3;
			this.lblLimits.Text = "Limits";
			this.lblLimits.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
			this.lblLimits.Click += new System.EventHandler(this.lblLimits_Click);
			// 
			// PostDialog
			// 
			this.AcceptButton = this.okButton;
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.CancelButton = this.cancelButton;
			this.ClientSize = new System.Drawing.Size(226, 120);
			this.ControlBox = false;
			this.Controls.Add(this.groupBox1);
			this.Controls.Add(this.cancelButton);
			this.Controls.Add(this.okButton);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Name = "PostDialog";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Post Analysis Question";
			this.groupBox1.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		private void binwidthTextBox_TextChanged(object sender, System.EventArgs e)
		{
			double binWidth;
			if (double.TryParse(this.binwidthTextBox.Text, System.Globalization.NumberStyles.AllowDecimalPoint, null, out binWidth))
			{
				if (BinWidth <= maxBinWidth_Seconds)
					this.okButton.Enabled = true;
				else
				{
					this.okButton.Enabled = false;
					MessageBox.Show(this, "The specified bin width must be less than the analysis window size (" + maxBinWidth_Seconds.ToString("#.###") + " seconds).", "Parameter error", MessageBoxButtons.OK, MessageBoxIcon.Error);
				}
			}
		}

		private void lblLimits_Click(object sender, System.EventArgs e)
		{
		
		}

		public double BinWidth
		{
			get
			{
				double binWidth;
				if (double.TryParse(this.binwidthTextBox.Text, System.Globalization.NumberStyles.AllowDecimalPoint, null, out binWidth))
				{
					return binWidth;
				}
				else
					return 0;
			}
		}
	}
}
