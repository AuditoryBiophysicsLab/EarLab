using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace EarLab.Dialogs
{
	/// <summary>
	/// Summary description for AnalysisQuestionDialog.
	/// </summary>
	public class AnalysisQuestionDialog : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Button cancelButton;
		private System.Windows.Forms.Button okButton;
		private System.Windows.Forms.GroupBox groupBox;
		private System.Windows.Forms.TextBox questionTextBox;
		private System.Windows.Forms.Label questionLabel;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		private double parameterValue;

		public AnalysisQuestionDialog()
		{
			// Required for Windows Form Designer support
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

		public string Question
		{
			set { this.questionLabel.Text = value; }
		}

		public double Value
		{
			get { return this.parameterValue; }
			set { this.questionTextBox.Text = value.ToString(); }
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.groupBox = new System.Windows.Forms.GroupBox();
			this.questionLabel = new System.Windows.Forms.Label();
			this.questionTextBox = new System.Windows.Forms.TextBox();
			this.cancelButton = new System.Windows.Forms.Button();
			this.okButton = new System.Windows.Forms.Button();
			this.groupBox.SuspendLayout();
			this.SuspendLayout();
			// 
			// groupBox
			// 
			this.groupBox.Controls.Add(this.questionLabel);
			this.groupBox.Controls.Add(this.questionTextBox);
			this.groupBox.Location = new System.Drawing.Point(8, 8);
			this.groupBox.Name = "groupBox";
			this.groupBox.Size = new System.Drawing.Size(376, 88);
			this.groupBox.TabIndex = 0;
			this.groupBox.TabStop = false;
			this.groupBox.Text = "Analysis Parameter";
			// 
			// questionLabel
			// 
			this.questionLabel.Location = new System.Drawing.Point(8, 16);
			this.questionLabel.Name = "questionLabel";
			this.questionLabel.Size = new System.Drawing.Size(360, 40);
			this.questionLabel.TabIndex = 1;
			this.questionLabel.Text = "Question";
			// 
			// questionTextBox
			// 
			this.questionTextBox.Location = new System.Drawing.Point(144, 56);
			this.questionTextBox.MaxLength = 9;
			this.questionTextBox.Name = "questionTextBox";
			this.questionTextBox.TabIndex = 0;
			this.questionTextBox.Text = "";
			this.questionTextBox.TextChanged += new System.EventHandler(this.questionTextBox_TextChanged);
			// 
			// cancelButton
			// 
			this.cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.cancelButton.Location = new System.Drawing.Point(312, 104);
			this.cancelButton.Name = "cancelButton";
			this.cancelButton.Size = new System.Drawing.Size(72, 23);
			this.cancelButton.TabIndex = 1;
			this.cancelButton.Text = "Cancel";
			// 
			// okButton
			// 
			this.okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.okButton.Location = new System.Drawing.Point(232, 104);
			this.okButton.Name = "okButton";
			this.okButton.Size = new System.Drawing.Size(72, 23);
			this.okButton.TabIndex = 2;
			this.okButton.Text = "OK";
			// 
			// AnalysisQuestionDialog
			// 
			this.AcceptButton = this.okButton;
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.CancelButton = this.cancelButton;
			this.ClientSize = new System.Drawing.Size(394, 136);
			this.Controls.Add(this.okButton);
			this.Controls.Add(this.cancelButton);
			this.Controls.Add(this.groupBox);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "AnalysisQuestionDialog";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "AnalysisQuestionDialog";
			this.groupBox.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		private void questionTextBox_TextChanged(object sender, System.EventArgs e)
		{
			if (double.TryParse(this.questionTextBox.Text, System.Globalization.NumberStyles.Number, null, out this.parameterValue))
			{
				if (this.parameterValue > 0)
					this.okButton.Enabled = true;
				else
					this.okButton.Enabled = false;
			}
			else
				this.okButton.Enabled = false;
		}
	}
}
