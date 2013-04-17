using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.IO;

namespace EarLab
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class SimpleTextViewer : System.Windows.Forms.Form
	{
		private System.Windows.Forms.TextBox textBox1;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public SimpleTextViewer()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();
		}

		public SimpleTextViewer(string FileName)
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			using (StreamReader theReader = new StreamReader(FileName))
				textBox1.Text = theReader.ReadToEnd();
			this.Text += " - " + FileName;

			textBox1.Select(0, 0);
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
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
			this.textBox1 = new System.Windows.Forms.TextBox();
			this.SuspendLayout();
			// 
			// textBox1
			// 
			this.textBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.textBox1.Font = new System.Drawing.Font("Courier New", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.textBox1.Location = new System.Drawing.Point(0, 0);
			this.textBox1.Multiline = true;
			this.textBox1.Name = "textBox1";
			this.textBox1.ScrollBars = System.Windows.Forms.ScrollBars.Both;
			this.textBox1.Size = new System.Drawing.Size(564, 272);
			this.textBox1.TabIndex = 0;
			this.textBox1.Text = "";
			// 
			// SimpleTextViewer
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(560, 266);
			this.Controls.Add(this.textBox1);
			this.Name = "SimpleTextViewer";
			this.Text = "EarLab FileViewer";
			this.ResumeLayout(false);

		}
		#endregion
	}
}
