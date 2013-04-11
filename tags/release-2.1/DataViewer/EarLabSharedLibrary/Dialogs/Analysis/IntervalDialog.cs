using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace EarLab.Dialogs.Analysis
{
	/// <summary>
	/// Summary description for IntervalDialog.
	/// </summary>
	public class IntervalDialog : System.Windows.Forms.Form
	{
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.Button okButton;
		private System.Windows.Forms.Button cancelButton;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.TextBox bincountTextBox;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.TextBox maxintervalTextBox;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public IntervalDialog()
		{
			InitializeComponent();
			EarLab.Utilities.EnableVisualStyles.EnableControl(this);

			this.okButton.Enabled = false;
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
			this.bincountTextBox = new System.Windows.Forms.TextBox();
			this.label1 = new System.Windows.Forms.Label();
			this.maxintervalTextBox = new System.Windows.Forms.TextBox();
			this.label3 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.groupBox1.SuspendLayout();
			this.SuspendLayout();
			// 
			// okButton
			// 
			this.okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.okButton.Location = new System.Drawing.Point(104, 96);
			this.okButton.Name = "okButton";
			this.okButton.Size = new System.Drawing.Size(40, 23);
			this.okButton.TabIndex = 1;
			this.okButton.Text = "OK";
			// 
			// cancelButton
			// 
			this.cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.cancelButton.Location = new System.Drawing.Point(152, 96);
			this.cancelButton.Name = "cancelButton";
			this.cancelButton.Size = new System.Drawing.Size(56, 23);
			this.cancelButton.TabIndex = 2;
			this.cancelButton.Text = "Cancel";
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.bincountTextBox);
			this.groupBox1.Controls.Add(this.label1);
			this.groupBox1.Controls.Add(this.maxintervalTextBox);
			this.groupBox1.Controls.Add(this.label3);
			this.groupBox1.Controls.Add(this.label2);
			this.groupBox1.Location = new System.Drawing.Point(8, 8);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(200, 80);
			this.groupBox1.TabIndex = 0;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "User Parameters";
			// 
			// bincountTextBox
			// 
			this.bincountTextBox.Location = new System.Drawing.Point(80, 48);
			this.bincountTextBox.Name = "bincountTextBox";
			this.bincountTextBox.Size = new System.Drawing.Size(56, 20);
			this.bincountTextBox.TabIndex = 1;
			this.bincountTextBox.Text = "";
			this.bincountTextBox.TextChanged += new System.EventHandler(this.bincountTextBox_TextChanged);
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(8, 24);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(72, 16);
			this.label1.TabIndex = 1;
			this.label1.Text = "Max Interval:";
			this.label1.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
			// 
			// maxintervalTextBox
			// 
			this.maxintervalTextBox.Location = new System.Drawing.Point(80, 24);
			this.maxintervalTextBox.Name = "maxintervalTextBox";
			this.maxintervalTextBox.Size = new System.Drawing.Size(56, 20);
			this.maxintervalTextBox.TabIndex = 0;
			this.maxintervalTextBox.Text = "";
			this.maxintervalTextBox.TextChanged += new System.EventHandler(this.maxintervalTextBox_TextChanged);
			// 
			// label3
			// 
			this.label3.Location = new System.Drawing.Point(8, 48);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(64, 16);
			this.label3.TabIndex = 4;
			this.label3.Text = "Bin Count:";
			this.label3.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
			// 
			// label2
			// 
			this.label2.Location = new System.Drawing.Point(144, 24);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(48, 16);
			this.label2.TabIndex = 2;
			this.label2.Text = "seconds";
			this.label2.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
			// 
			// IntervalDialog
			// 
			this.AcceptButton = this.okButton;
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.CancelButton = this.cancelButton;
			this.ClientSize = new System.Drawing.Size(218, 128);
			this.ControlBox = false;
			this.Controls.Add(this.groupBox1);
			this.Controls.Add(this.cancelButton);
			this.Controls.Add(this.okButton);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Name = "IntervalDialog";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Interval Analysis Question";
			this.groupBox1.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		private void maxintervalTextBox_TextChanged(object sender, System.EventArgs e)
		{
			double maxInterval;
			if (double.TryParse(this.maxintervalTextBox.Text, System.Globalization.NumberStyles.AllowDecimalPoint, null, out maxInterval))
				this.okButton.Enabled = true;
			else
				this.okButton.Enabled = false;

		}

		private void bincountTextBox_TextChanged(object sender, System.EventArgs e)
		{
			double binCount;
			if (double.TryParse(this.bincountTextBox.Text, System.Globalization.NumberStyles.None, null, out binCount))
			{
				if (binCount > 0)
					this.okButton.Enabled = true;
				else
					this.okButton.Enabled = false;
			}
			else
				this.okButton.Enabled = false;
		}

		public double MaxInterval
		{
			get
			{
				double maxInterval;
				if (double.TryParse(this.maxintervalTextBox.Text, System.Globalization.NumberStyles.AllowDecimalPoint, null, out maxInterval))
				{
					return maxInterval;
				}
				else
					return 0;
			}
		}

		public int BinCount
		{
			get
			{
				double binCount;
				if (double.TryParse(this.bincountTextBox.Text, System.Globalization.NumberStyles.None, null, out binCount))
				{
					return (int)binCount;
				}
				else
					return 0;
			}
		}
	}
}
