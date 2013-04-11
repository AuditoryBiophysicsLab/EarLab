using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;

namespace EarLab.Viewers.Layouts
{
	/// <summary>
	/// Summary description for LayoutWaveform.
	/// </summary>
	public class LayoutWaveform : System.Windows.Forms.UserControl
	{
		private System.Windows.Forms.Panel backgroundPanel;
		private EarLab.Viewers.Panels.PanelAxisLimited axisPanel;
		private EarLab.Viewers.Panels.PanelWaveformNew waveformPanel;
		/// <summary> 
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public LayoutWaveform()
		{
			// This call is required by the Windows.Forms Form Designer.
			InitializeComponent();
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

		#region Component Designer generated code
		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.backgroundPanel = new System.Windows.Forms.Panel();
			this.axisPanel = new EarLab.Viewers.Panels.PanelAxisLimited();
			this.waveformPanel = new EarLab.Viewers.Panels.PanelWaveformNew();
			this.backgroundPanel.SuspendLayout();
			this.axisPanel.SuspendLayout();
			this.SuspendLayout();
			// 
			// backgroundPanel
			// 
			this.backgroundPanel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.backgroundPanel.BackColor = System.Drawing.Color.White;
			this.backgroundPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.backgroundPanel.Controls.Add(this.axisPanel);
			this.backgroundPanel.Location = new System.Drawing.Point(0, 0);
			this.backgroundPanel.Name = "backgroundPanel";
			this.backgroundPanel.Size = new System.Drawing.Size(736, 304);
			this.backgroundPanel.TabIndex = 0;
			// 
			// axisPanel
			// 
			this.axisPanel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.axisPanel.Controls.Add(this.waveformPanel);
			this.axisPanel.Font = new System.Drawing.Font("Arial", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Pixel, ((System.Byte)(0)));
			this.axisPanel.Location = new System.Drawing.Point(0, 0);
			this.axisPanel.Name = "axisPanel";
			this.axisPanel.Size = new System.Drawing.Size(734, 302);
			this.axisPanel.TabIndex = 0;
			// 
			// waveformPanel
			// 
			this.waveformPanel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.waveformPanel.BackColor = System.Drawing.SystemColors.Control;
			this.waveformPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.waveformPanel.Location = new System.Drawing.Point(64, 24);
			this.waveformPanel.Name = "waveformPanel";
			this.waveformPanel.NormalOrientation = true;
			this.waveformPanel.Size = new System.Drawing.Size(600, 232);
			this.waveformPanel.TabIndex = 0;
			this.waveformPanel.Resize += new System.EventHandler(this.waveformPanel_Resize);
			// 
			// LayoutWaveform
			// 
			this.Controls.Add(this.backgroundPanel);
			this.Name = "LayoutWaveform";
			this.Size = new System.Drawing.Size(736, 304);
			this.backgroundPanel.ResumeLayout(false);
			this.axisPanel.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion
		
		private void waveformPanel_Resize(object sender, System.EventArgs e)
		{
			// if the waveform panel is resized, the axis will need to move and redraw
			this.axisPanel.Invalidate();
		}
		
		#region Properties
		public double[] DataSource
		{
			set
			{
                if (this.waveformPanel.Width > 0 && this.waveformPanel.Height > 0)
                {
                    // set the waveform panel data
                    this.waveformPanel.DataSource = value;

                    // set the axis values to the proper min/max
                    this.axisPanel.Min = this.waveformPanel.Min;
                    this.axisPanel.Max = this.waveformPanel.Max;

                    // make sure to eitehr hide or display the axis
                    if (value != null)
                        this.axisPanel.ShowAxis = true;
                    else
                        this.axisPanel.ShowAxis = false;
                }
			}
		}

		public string Label
		{
			set { this.axisPanel.Label = value; }
		}

		public bool NormalOrientation
		{
			set
			{
				this.waveformPanel.NormalOrientation = value;
				this.axisPanel.NormalOrientation = value;
			}
		}

		public EarLab.Viewers.Panels.PanelWaveformNew WaveformPanel
		{
			get { return this.waveformPanel; }
		}

		public EarLab.Viewers.Panels.PanelAxisLimited AxisPanel
		{
			get { return this.axisPanel; }
		}
		#endregion
	}
}
