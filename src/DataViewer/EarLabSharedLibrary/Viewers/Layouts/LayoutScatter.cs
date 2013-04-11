using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;
using EarLab.ReaderWriters;

namespace EarLab.Viewers.Layouts
{
	/// <summary>
	/// Summary description for LayoutScatter.
	/// </summary>
	public class LayoutScatter : System.Windows.Forms.UserControl
	{
		public struct BoxDataType
		{
			public int cellIDStart;
			public int cellIDEnd;
			public double timeStart;
			public double timeEnd;
			public ReaderWriterBinarySpikes.SpikeItem[] spikeArray; 
		}

		/// <summary> 
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		private EarLab.Viewers.Panels.Panel2DColor viewerPanel;
		private EarLab.Viewers.Panels.PanelAxisNew axisPanel;
		private System.Windows.Forms.Panel backgroundPanel;

		private ReaderWriterBinarySpikes.SpikeItem[] spikesArray;
		private double offsetTime, pixelTime;
		private int cellCount;

        private Color scatterColor = Color.Black;

		private Bitmap viewerBitmap;

		public LayoutScatter()
		{
			// This call is required by the Windows.Forms Form Designer.
			InitializeComponent();

			// we set the axes controls to know what controls they are hosting
			this.axisPanel.TopAxisShow = true;
			this.axisPanel.TopAxisControl = this.viewerPanel;
			this.axisPanel.LeftAxisShow = true;
			this.axisPanel.LeftAxisControl = this.viewerPanel;
			this.axisPanel.BottomAxisShow = true;
			this.axisPanel.BottomAxisControl = this.viewerPanel;

			//this.viewerBitmap = new Bitmap(1, 1, System.Drawing.Imaging.PixelFormat.Format24bppRgb);
			//this.bitmapGraphics = Graphics.FromImage(this.viewerBitmap);
			//this.bitmapGraphics.Clear(System.Drawing.SystemColors.Control);
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
			this.axisPanel = new EarLab.Viewers.Panels.PanelAxisNew();
			this.viewerPanel = new EarLab.Viewers.Panels.Panel2DColor();
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
			this.backgroundPanel.Size = new System.Drawing.Size(640, 216);
			this.backgroundPanel.TabIndex = 0;
			// 
			// axisPanel
			// 
			this.axisPanel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.axisPanel.BottomAxisEndValue = 0F;
			this.axisPanel.BottomAxisLabel = "Axis Label Not Set";
			this.axisPanel.BottomAxisLabelShow = true;
			this.axisPanel.BottomAxisMajorTickHeight = 3;
			this.axisPanel.BottomAxisMajorTickNumbersFormat = "0";
			this.axisPanel.BottomAxisMajorTickNumbersShow = true;
			this.axisPanel.BottomAxisMajorTickNumbersSpacing = 10;
			this.axisPanel.BottomAxisMajorTickOffset = 2;
			this.axisPanel.BottomAxisShow = false;
			this.axisPanel.BottomAxisStartValue = 0F;
			this.axisPanel.Controls.Add(this.viewerPanel);
			this.axisPanel.Font = new System.Drawing.Font("Arial", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Pixel, ((System.Byte)(0)));
			this.axisPanel.LeftAxisEndValue = 0F;
			this.axisPanel.LeftAxisLabel = "Axis Label Not Set";
			this.axisPanel.LeftAxisLabelShow = true;
			this.axisPanel.LeftAxisMajorTickHeight = 3;
			this.axisPanel.LeftAxisMajorTickNumbersFormat = "0";
			this.axisPanel.LeftAxisMajorTickNumbersShow = true;
			this.axisPanel.LeftAxisMajorTickNumbersSpacing = 10;
			this.axisPanel.LeftAxisMajorTickOffset = 2;
			this.axisPanel.LeftAxisShow = false;
			this.axisPanel.LeftAxisStartValue = 0F;
			this.axisPanel.Location = new System.Drawing.Point(0, 0);
			this.axisPanel.Name = "axisPanel";
			this.axisPanel.RightAxisEndValue = 0F;
			this.axisPanel.RightAxisLabel = "Axis Label Not Set";
			this.axisPanel.RightAxisLabelShow = true;
			this.axisPanel.RightAxisMajorTickHeight = 3;
			this.axisPanel.RightAxisMajorTickNumbersFormat = "0";
			this.axisPanel.RightAxisMajorTickNumbersShow = true;
			this.axisPanel.RightAxisMajorTickNumbersSpacing = 10;
			this.axisPanel.RightAxisMajorTickOffset = 2;
			this.axisPanel.RightAxisShow = false;
			this.axisPanel.RightAxisStartValue = 0F;
			this.axisPanel.Size = new System.Drawing.Size(638, 214);
			this.axisPanel.TabIndex = 0;
			this.axisPanel.TopAxisEndValue = 0F;
			this.axisPanel.TopAxisLabel = "Axis Label Not Set";
			this.axisPanel.TopAxisLabelShow = true;
			this.axisPanel.TopAxisMajorTickHeight = 3;
			this.axisPanel.TopAxisMajorTickNumbersFormat = "0";
			this.axisPanel.TopAxisMajorTickNumbersShow = true;
			this.axisPanel.TopAxisMajorTickNumbersSpacing = 10;
			this.axisPanel.TopAxisMajorTickOffset = 2;
			this.axisPanel.TopAxisShow = false;
			this.axisPanel.TopAxisStartValue = 0F;
			// 
			// viewerPanel
			// 
			this.viewerPanel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.viewerPanel.BackColor = System.Drawing.SystemColors.Control;
			this.viewerPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.viewerPanel.Location = new System.Drawing.Point(48, 24);
			this.viewerPanel.Name = "viewerPanel";
			this.viewerPanel.Size = new System.Drawing.Size(544, 160);
			this.viewerPanel.TabIndex = 0;
			// 
			// LayoutScatter
			// 
			this.Controls.Add(this.backgroundPanel);
			this.Name = "LayoutScatter";
			this.Size = new System.Drawing.Size(640, 216);
			this.backgroundPanel.ResumeLayout(false);
			this.axisPanel.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		#region Methods
		public bool View(ReaderWriterBinarySpikes.SpikeItem[] spikesArray, double offsetTime, double pixelTime, int cellCount)
		{
			this.spikesArray = spikesArray;
			this.offsetTime = offsetTime;
			this.pixelTime = pixelTime;
			this.cellCount = cellCount;

			this.axisPanel.BottomAxisStartValue = (float)this.offsetTime;
			this.axisPanel.BottomAxisEndValue = (float)(this.offsetTime + this.viewerPanel.ClientSize.Width*this.pixelTime);
			this.axisPanel.Invalidate();

			this.CreateBitmap();
			this.ViewerRefresh();

			return true;
		}

		public void SetupAxis(string leftAxisName, string leftAxisFormat, float leftAxisMin, float leftAxisMax, string bottomAxisName, string bottomAxisFormat)
		{
			this.axisPanel.LeftAxisLabel = leftAxisName;
			this.axisPanel.LeftAxisMajorTickNumbersFormat = leftAxisFormat;
			this.axisPanel.LeftAxisStartValue = leftAxisMin;
			this.axisPanel.LeftAxisEndValue = leftAxisMax;

			this.axisPanel.BottomAxisLabel = bottomAxisName;
			this.axisPanel.BottomAxisMajorTickNumbersFormat = bottomAxisFormat;
		}

		private void CreateBitmap()
		{
			if (this.viewerBitmap == null || this.viewerBitmap.Width != this.viewerPanel.ClientSize.Width || this.viewerBitmap.Height != this.viewerPanel.ClientSize.Height)
			{
				this.viewerBitmap = new Bitmap(this.viewerPanel.ClientSize.Width, this.viewerPanel.ClientSize.Height, System.Drawing.Imaging.PixelFormat.Format24bppRgb);
				//this.bitmapGraphics = Graphics.FromImage(this.viewerBitmap);
			}

			Graphics.FromImage(this.viewerBitmap).Clear(System.Drawing.SystemColors.Control);
			//this.bitmapGraphics.FillRectangle(new SolidBrush(SystemColors.Control), 0, 0, this.viewerBitmap.Width-1, this.viewerBitmap.Height-1);
			
			int x, y;
			foreach (ReaderWriterBinarySpikes.SpikeItem item in this.spikesArray)
			{
				x = (int)Math.Max(0, Math.Min(((item.time-this.offsetTime)/this.pixelTime)-1, this.viewerPanel.ClientSize.Width-1));
				y = (int)Math.Max(0, Math.Min((this.viewerPanel.ClientSize.Height-1-this.viewerPanel.ClientSize.Height*(item.cellID+1)/this.cellCount), this.viewerPanel.ClientSize.Height-1));
				this.viewerBitmap.SetPixel(x, y, this.scatterColor);
			}

			//this.viewerBitmap.RotateFlip(RotateFlipType.RotateNoneFlipY);
		}

		public int CellID(int yCoordinate)
		{
			return this.cellCount-1-(int)Math.Round((yCoordinate/(double)this.viewerPanel.Height)*this.cellCount, 0);
		}

		public double Time(int xCoordinate)
		{
			return this.offsetTime+xCoordinate*this.pixelTime;
		}

		private void ViewerRefresh()
		{
			this.viewerPanel.Bitmap = this.viewerBitmap;
			this.viewerPanel.Invalidate();
		}
		#endregion
		
		#region Properties
		public Rectangle Box
		{
			set { this.viewerPanel.Box = value; }
		}

		public BoxDataType BoxData
		{
			get
			{
				BoxDataType boxData = new BoxDataType();
				boxData.cellIDStart = this.CellID(this.viewerPanel.Box.Bottom);
				boxData.cellIDEnd = this.CellID(this.viewerPanel.Box.Top);
				boxData.timeStart = this.Time(this.viewerPanel.Box.Left);
				boxData.timeEnd = this.Time(this.viewerPanel.Box.Right);
				boxData.spikeArray = this.spikesArray;

				return boxData;
			}
		}

		public EarLab.Viewers.Panels.Panel2DColor ViewerPanel
		{
			get { return this.viewerPanel; }
		}

		public EarLab.Viewers.Panels.PanelAxisNew AxisPanel
		{
			get { return this.axisPanel; }
		}
		#endregion
	}
}
