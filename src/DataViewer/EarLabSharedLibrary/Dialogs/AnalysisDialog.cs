using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace EarLab.Dialogs
{
	/// <summary>
	/// Summary description for AnalysisDialog.
	/// </summary>
	public class AnalysisDialog : System.Windows.Forms.Form
	{
		private EarLab.Viewers.Layouts.Layout2DColor layout2DColor;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public AnalysisDialog(double [,] dataArray, double[] axisArray, string[] axisNames)
		{
			// Required for Windows Form Designer support
			InitializeComponent();

			this.layout2DColor.View(dataArray);
			this.layout2DColor.ColorbarPanel.SetMinMax(axisArray[4], axisArray[5], true);

			this.layout2DColor.ViewerAxisPanel.TopAxisLabel = axisNames[0];
			this.layout2DColor.ViewerAxisPanel.LeftAxisLabel = axisNames[1];
			this.layout2DColor.ViewerAxisPanel.LeftAxisStartValue = (float)axisArray[0];
			this.layout2DColor.ViewerAxisPanel.LeftAxisEndValue = (float)axisArray[1];
			this.layout2DColor.ViewerAxisPanel.LeftAxisMajorTickNumbersFormat = "0";
			this.layout2DColor.ViewerAxisPanel.BottomAxisLabel = axisNames[2];
			this.layout2DColor.ViewerAxisPanel.BottomAxisStartValue = (float)axisArray[2];
			this.layout2DColor.ViewerAxisPanel.BottomAxisEndValue = (float)axisArray[3];
			this.layout2DColor.ColorbarAxisPanel.RightAxisLabel = axisNames[3];
			this.layout2DColor.ColorbarAxisPanel.RightAxisStartValue = (float)axisArray[4];
			this.layout2DColor.ColorbarAxisPanel.RightAxisEndValue = (float)axisArray[5];
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
			this.layout2DColor = new EarLab.Viewers.Layouts.Layout2DColor();
			this.SuspendLayout();
			// 
			// layout2DColor
			// 
			this.layout2DColor.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.layout2DColor.Location = new System.Drawing.Point(0, 0);
			this.layout2DColor.Name = "layout2DColor";
			this.layout2DColor.Size = new System.Drawing.Size(520, 262);
			this.layout2DColor.TabIndex = 0;
			// 
			// AnalysisDialog
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(520, 262);
			this.Controls.Add(this.layout2DColor);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.SizableToolWindow;
			this.MinimumSize = new System.Drawing.Size(300, 200);
			this.Name = "AnalysisDialog";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Data Analysis";
			this.ResumeLayout(false);

		}
		#endregion
	}
}
