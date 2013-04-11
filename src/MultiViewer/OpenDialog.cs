using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace MultiViewer
{
	/// <summary>
	/// Summary description for OpenDialog.
	/// </summary>
	public class OpenDialog : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Button ulButton;
		private System.Windows.Forms.OpenFileDialog openFileDialog;
		private System.Windows.Forms.TextBox ulTextBox;
		private System.Windows.Forms.Button okButton;
		private System.Windows.Forms.TextBox urTextBox;
		private System.Windows.Forms.Button urButton;
		private System.Windows.Forms.TextBox llTextBox;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.Label ulLabel;
		private System.Windows.Forms.Label urLabel;
		private System.Windows.Forms.Label llLabel;
		private System.Windows.Forms.Button llButton;
		private System.Windows.Forms.TextBox lrTextBox;
		private System.Windows.Forms.Label lrLabel;
		private System.Windows.Forms.Button lrButton;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public OpenDialog()
		{
			InitializeComponent();
			EarLab.Utilities.EnableVisualStyles.EnableControl(this);
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
			this.ulButton = new System.Windows.Forms.Button();
			this.ulTextBox = new System.Windows.Forms.TextBox();
			this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
			this.okButton = new System.Windows.Forms.Button();
			this.urTextBox = new System.Windows.Forms.TextBox();
			this.urButton = new System.Windows.Forms.Button();
			this.llTextBox = new System.Windows.Forms.TextBox();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.lrButton = new System.Windows.Forms.Button();
			this.lrLabel = new System.Windows.Forms.Label();
			this.lrTextBox = new System.Windows.Forms.TextBox();
			this.llButton = new System.Windows.Forms.Button();
			this.llLabel = new System.Windows.Forms.Label();
			this.urLabel = new System.Windows.Forms.Label();
			this.ulLabel = new System.Windows.Forms.Label();
			this.groupBox1.SuspendLayout();
			this.SuspendLayout();
			// 
			// ulButton
			// 
			this.ulButton.Location = new System.Drawing.Point(392, 40);
			this.ulButton.Name = "ulButton";
			this.ulButton.Size = new System.Drawing.Size(75, 20);
			this.ulButton.TabIndex = 0;
			this.ulButton.Text = "Browse...";
			this.ulButton.Click += new System.EventHandler(this.ulButton_Click);
			// 
			// ulTextBox
			// 
			this.ulTextBox.Location = new System.Drawing.Point(8, 40);
			this.ulTextBox.Name = "ulTextBox";
			this.ulTextBox.ReadOnly = true;
			this.ulTextBox.Size = new System.Drawing.Size(376, 20);
			this.ulTextBox.TabIndex = 1;
			this.ulTextBox.Text = "";
			// 
			// openFileDialog
			// 
			this.openFileDialog.DefaultExt = "metadata";
			this.openFileDialog.Filter = "All EarLab Files|*.metadata;*.index|EarLab Waveform Files (*.metadata)|*.metadata" +
				"|EarLab Spike Files (*.index)|*.index";
			this.openFileDialog.Title = "Select File to Open...";
			// 
			// okButton
			// 
			this.okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.okButton.Location = new System.Drawing.Point(416, 232);
			this.okButton.Name = "okButton";
			this.okButton.Size = new System.Drawing.Size(72, 23);
			this.okButton.TabIndex = 2;
			this.okButton.Text = "OK";
			// 
			// urTextBox
			// 
			this.urTextBox.Location = new System.Drawing.Point(8, 88);
			this.urTextBox.Name = "urTextBox";
			this.urTextBox.ReadOnly = true;
			this.urTextBox.Size = new System.Drawing.Size(376, 20);
			this.urTextBox.TabIndex = 3;
			this.urTextBox.Text = "";
			// 
			// urButton
			// 
			this.urButton.Location = new System.Drawing.Point(392, 88);
			this.urButton.Name = "urButton";
			this.urButton.Size = new System.Drawing.Size(75, 20);
			this.urButton.TabIndex = 4;
			this.urButton.Text = "Browse...";
			this.urButton.Click += new System.EventHandler(this.urButton_Click);
			// 
			// llTextBox
			// 
			this.llTextBox.Location = new System.Drawing.Point(8, 136);
			this.llTextBox.Name = "llTextBox";
			this.llTextBox.ReadOnly = true;
			this.llTextBox.Size = new System.Drawing.Size(376, 20);
			this.llTextBox.TabIndex = 5;
			this.llTextBox.Text = "";
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.lrButton);
			this.groupBox1.Controls.Add(this.lrLabel);
			this.groupBox1.Controls.Add(this.lrTextBox);
			this.groupBox1.Controls.Add(this.llButton);
			this.groupBox1.Controls.Add(this.llLabel);
			this.groupBox1.Controls.Add(this.urLabel);
			this.groupBox1.Controls.Add(this.ulLabel);
			this.groupBox1.Controls.Add(this.llTextBox);
			this.groupBox1.Controls.Add(this.ulTextBox);
			this.groupBox1.Controls.Add(this.urTextBox);
			this.groupBox1.Controls.Add(this.ulButton);
			this.groupBox1.Controls.Add(this.urButton);
			this.groupBox1.Location = new System.Drawing.Point(8, 8);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(480, 216);
			this.groupBox1.TabIndex = 6;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Multi Viewer Files";
			// 
			// lrButton
			// 
			this.lrButton.Location = new System.Drawing.Point(392, 184);
			this.lrButton.Name = "lrButton";
			this.lrButton.Size = new System.Drawing.Size(75, 20);
			this.lrButton.TabIndex = 12;
			this.lrButton.Text = "Browse...";
			this.lrButton.Click += new System.EventHandler(this.lrButton_Click);
			// 
			// lrLabel
			// 
			this.lrLabel.Location = new System.Drawing.Point(8, 168);
			this.lrLabel.Name = "lrLabel";
			this.lrLabel.Size = new System.Drawing.Size(112, 16);
			this.lrLabel.TabIndex = 11;
			this.lrLabel.Text = "Lower Right Viewer:";
			// 
			// lrTextBox
			// 
			this.lrTextBox.Location = new System.Drawing.Point(8, 184);
			this.lrTextBox.Name = "lrTextBox";
			this.lrTextBox.ReadOnly = true;
			this.lrTextBox.Size = new System.Drawing.Size(376, 20);
			this.lrTextBox.TabIndex = 10;
			this.lrTextBox.Text = "";
			// 
			// llButton
			// 
			this.llButton.Location = new System.Drawing.Point(392, 136);
			this.llButton.Name = "llButton";
			this.llButton.Size = new System.Drawing.Size(75, 20);
			this.llButton.TabIndex = 9;
			this.llButton.Text = "Browse...";
			this.llButton.Click += new System.EventHandler(this.llButton_Click);
			// 
			// llLabel
			// 
			this.llLabel.Location = new System.Drawing.Point(8, 120);
			this.llLabel.Name = "llLabel";
			this.llLabel.Size = new System.Drawing.Size(112, 16);
			this.llLabel.TabIndex = 8;
			this.llLabel.Text = "Lower Left Viewer:";
			// 
			// urLabel
			// 
			this.urLabel.Location = new System.Drawing.Point(8, 72);
			this.urLabel.Name = "urLabel";
			this.urLabel.Size = new System.Drawing.Size(112, 16);
			this.urLabel.TabIndex = 7;
			this.urLabel.Text = "Upper Right Viewer:";
			// 
			// ulLabel
			// 
			this.ulLabel.Location = new System.Drawing.Point(8, 24);
			this.ulLabel.Name = "ulLabel";
			this.ulLabel.Size = new System.Drawing.Size(100, 16);
			this.ulLabel.TabIndex = 6;
			this.ulLabel.Text = "Upper Left Viewer:";
			// 
			// OpenDialog
			// 
			this.AcceptButton = this.okButton;
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(498, 262);
			this.Controls.Add(this.groupBox1);
			this.Controls.Add(this.okButton);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "OpenDialog";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "File Open...";
			this.groupBox1.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		private void ulButton_Click(object sender, System.EventArgs e)
		{
			if (this.openFileDialog.ShowDialog(this) == DialogResult.OK)
				this.ulTextBox.Text = this.openFileDialog.FileName;
		}

		private void urButton_Click(object sender, System.EventArgs e)
		{
			if (this.openFileDialog.ShowDialog(this) == DialogResult.OK)
				this.urTextBox.Text = this.openFileDialog.FileName;
		}

		private void llButton_Click(object sender, System.EventArgs e)
		{
			if (this.openFileDialog.ShowDialog(this) == DialogResult.OK)
				this.llTextBox.Text = this.openFileDialog.FileName;
		}

		private void lrButton_Click(object sender, System.EventArgs e)
		{
			if (this.openFileDialog.ShowDialog(this) == DialogResult.OK)
				this.lrTextBox.Text = this.openFileDialog.FileName;
		}

		public string ULFile
		{
			get { return this.ulTextBox.Text; }
			set { this.ulTextBox.Text = value; }
		}

		public string URFile
		{
			get { return this.urTextBox.Text; }
			set { this.urTextBox.Text = value; }
		}

		public string LLFile
		{
			get { return this.llTextBox.Text; }
			set { this.llTextBox.Text = value; }
		}

		public string LRFile
		{
			get { return this.lrTextBox.Text; }
			set { this.lrTextBox.Text = value; }
		}
	}
}
