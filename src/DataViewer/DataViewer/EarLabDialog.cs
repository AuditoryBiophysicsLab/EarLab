using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using EarLab.Utilities;

namespace EarLab.DataViewer
{
	/// <summary>
	/// Summary description for EarLabDialog.
	/// </summary>
	public class EarLabDialog : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Button closeButton;
		private System.Windows.Forms.LinkLabel earlabLinkLabel;
		private System.Windows.Forms.RichTextBox aboutTextBox;
		private System.Windows.Forms.PictureBox pictureBox;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public EarLabDialog()
		{
			// Required for Windows Form Designer support
			InitializeComponent();
			EnableVisualStyles.EnableControl(this);

			this.aboutTextBox.Text = "EarLab is a virtual laboratory that is intended to allow scientists from a wide range of backgrounds to perform experiments on large-scale computational models of the mammalian auditory pathways. The models in EarLab are intended to serve as repository of current knowledge about auditory function and can be used to bridge the gap between information gathered from anatomical and physiological experiments in laboratory animals and data gathered from less direct physiological and behavioral experiments in humans. The models are also intended to give insight into changes associated with disease and into the effects of theraputic interventions.\n\nThe EarLab models are based on interchangeable building blocks or modules each of which represents a different component of the auditory system or sound-source, data-acquisition and data-visualization systems. The modules are designed to run in a distributed heterogeneous computing environment and to be configured at run time. The physiological modules are designed to be species independent with species dependent parameters loaded from a parameter database at run time.  The current system includes modules that represent sound sources, propagation between the source and the tympanic membrane, and the subthalamic auditory pathways.";
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
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(EarLabDialog));
			this.closeButton = new System.Windows.Forms.Button();
			this.earlabLinkLabel = new System.Windows.Forms.LinkLabel();
			this.aboutTextBox = new System.Windows.Forms.RichTextBox();
			this.pictureBox = new System.Windows.Forms.PictureBox();
			this.SuspendLayout();
			// 
			// closeButton
			// 
			this.closeButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.closeButton.Location = new System.Drawing.Point(392, 272);
			this.closeButton.Name = "closeButton";
			this.closeButton.Size = new System.Drawing.Size(72, 23);
			this.closeButton.TabIndex = 1;
			this.closeButton.Text = "Close";
			// 
			// earlabLinkLabel
			// 
			this.earlabLinkLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.earlabLinkLabel.LinkArea = new System.Windows.Forms.LinkArea(43, 21);
			this.earlabLinkLabel.LinkBehavior = System.Windows.Forms.LinkBehavior.HoverUnderline;
			this.earlabLinkLabel.Location = new System.Drawing.Point(8, 272);
			this.earlabLinkLabel.Name = "earlabLinkLabel";
			this.earlabLinkLabel.Size = new System.Drawing.Size(352, 16);
			this.earlabLinkLabel.TabIndex = 3;
			this.earlabLinkLabel.TabStop = true;
			this.earlabLinkLabel.Text = "To learn more, visit the EarLab website at http://earlab.bu.edu/.";
			this.earlabLinkLabel.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.earlabLinkLabel_LinkClicked);
			// 
			// aboutTextBox
			// 
			this.aboutTextBox.Location = new System.Drawing.Point(8, 96);
			this.aboutTextBox.Name = "aboutTextBox";
			this.aboutTextBox.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.ForcedVertical;
			this.aboutTextBox.Size = new System.Drawing.Size(456, 168);
			this.aboutTextBox.TabIndex = 4;
			this.aboutTextBox.Text = "";
			// 
			// pictureBox
			// 
			this.pictureBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.pictureBox.Image = ((System.Drawing.Image)(resources.GetObject("pictureBox.Image")));
			this.pictureBox.Location = new System.Drawing.Point(8, 8);
			this.pictureBox.Name = "pictureBox";
			this.pictureBox.Size = new System.Drawing.Size(456, 80);
			this.pictureBox.TabIndex = 5;
			this.pictureBox.TabStop = false;
			// 
			// EarLabDialog
			// 
			this.AcceptButton = this.closeButton;
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.CancelButton = this.closeButton;
			this.ClientSize = new System.Drawing.Size(474, 304);
			this.Controls.Add(this.pictureBox);
			this.Controls.Add(this.aboutTextBox);
			this.Controls.Add(this.earlabLinkLabel);
			this.Controls.Add(this.closeButton);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "EarLabDialog";
			this.ShowInTaskbar = false;
			this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "About EarLab at BU";
			this.ResumeLayout(false);

		}
		#endregion

		private void earlabLinkLabel_LinkClicked(object sender, System.Windows.Forms.LinkLabelLinkClickedEventArgs e)
		{
			try
			{
				System.Diagnostics.Process.Start("http://earlab.bu.edu/");
			}
			catch
			{
				return;
			}
		}

	}
}
