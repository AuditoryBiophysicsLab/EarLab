using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;
using System.Drawing.Imaging;
using System.Threading;

namespace EarLab.Viewers.Layouts
{
	/// <summary>
	/// Summary description for Layout2DColor.
	/// </summary>
	public class Layout2DColor : System.Windows.Forms.UserControl
	{
		private EarLab.Viewers.Panels.PanelAxisNew viewerAxisPanel;
		private EarLab.Viewers.Panels.PanelAxisNew colorbarAxisPanel;
		private EarLab.Viewers.Panels.PanelColorbarNew colorbarPanel;
		private EarLab.Viewers.Panels.Panel2DColor viewerPanel;
		/// <summary> 
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		private double[,] dataArray;
		private System.Windows.Forms.Panel backgroundPanel;
		private Bitmap viewerBitmap;

		public Layout2DColor()
		{
			// This call is required by the Windows.Forms Form Designer.
			InitializeComponent();

			// we set the axes controls to know what controls they are hosting
			this.viewerAxisPanel.TopAxisControl = this.viewerPanel;
			this.viewerAxisPanel.LeftAxisControl = this.viewerPanel;
			this.viewerAxisPanel.BottomAxisControl = this.viewerPanel;
			this.colorbarAxisPanel.RightAxisControl = this.colorbarPanel;
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
			this.colorbarPanel = new EarLab.Viewers.Panels.PanelColorbarNew();
			this.colorbarAxisPanel = new EarLab.Viewers.Panels.PanelAxisNew();
			this.viewerAxisPanel = new EarLab.Viewers.Panels.PanelAxisNew();
			this.viewerPanel = new EarLab.Viewers.Panels.Panel2DColor();
			this.backgroundPanel = new System.Windows.Forms.Panel();
			this.viewerAxisPanel.SuspendLayout();
			this.backgroundPanel.SuspendLayout();
			this.SuspendLayout();
			// 
			// colorbarPanel
			// 
			this.colorbarPanel.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.colorbarPanel.BackColor = System.Drawing.SystemColors.Control;
			this.colorbarPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.colorbarPanel.Cursor = System.Windows.Forms.Cursors.SizeAll;
			this.colorbarPanel.Location = new System.Drawing.Point(478, 24);
			this.colorbarPanel.Name = "colorbarPanel";
			this.colorbarPanel.Size = new System.Drawing.Size(40, 174);
			this.colorbarPanel.TabIndex = 2;
			this.colorbarPanel.CurrentValuesChanged += new EarLab.Viewers.Panels.PanelColorbarNew.CurrentValuesChangedHandler(this.colorbarPanel_CurrentValuesChanged);
			// 
			// colorbarAxisPanel
			// 
			this.colorbarAxisPanel.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.colorbarAxisPanel.BottomAxisEndValue = 0F;
			this.colorbarAxisPanel.BottomAxisLabel = "Axis Label Not Set";
			this.colorbarAxisPanel.BottomAxisLabelShow = true;
			this.colorbarAxisPanel.BottomAxisMajorTickHeight = 3;
			this.colorbarAxisPanel.BottomAxisMajorTickNumbersFormat = "0";
			this.colorbarAxisPanel.BottomAxisMajorTickNumbersShow = true;
			this.colorbarAxisPanel.BottomAxisMajorTickNumbersSpacing = 10;
			this.colorbarAxisPanel.BottomAxisMajorTickOffset = 2;
			this.colorbarAxisPanel.BottomAxisShow = false;
			this.colorbarAxisPanel.BottomAxisStartValue = 0F;
			this.colorbarAxisPanel.Font = new System.Drawing.Font("Arial", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Pixel, ((System.Byte)(0)));
			this.colorbarAxisPanel.LeftAxisEndValue = 0F;
			this.colorbarAxisPanel.LeftAxisLabel = "Axis Label Not Set";
			this.colorbarAxisPanel.LeftAxisLabelShow = true;
			this.colorbarAxisPanel.LeftAxisMajorTickHeight = 3;
			this.colorbarAxisPanel.LeftAxisMajorTickNumbersFormat = "0";
			this.colorbarAxisPanel.LeftAxisMajorTickNumbersShow = true;
			this.colorbarAxisPanel.LeftAxisMajorTickNumbersSpacing = 10;
			this.colorbarAxisPanel.LeftAxisMajorTickOffset = 2;
			this.colorbarAxisPanel.LeftAxisShow = false;
			this.colorbarAxisPanel.LeftAxisStartValue = 0F;
			this.colorbarAxisPanel.Location = new System.Drawing.Point(518, 0);
			this.colorbarAxisPanel.Name = "colorbarAxisPanel";
			this.colorbarAxisPanel.RightAxisEndValue = 0F;
			this.colorbarAxisPanel.RightAxisLabel = "Axis Label Not Set";
			this.colorbarAxisPanel.RightAxisLabelShow = true;
			this.colorbarAxisPanel.RightAxisMajorTickHeight = 3;
			this.colorbarAxisPanel.RightAxisMajorTickNumbersFormat = "0.00e00";
			this.colorbarAxisPanel.RightAxisMajorTickNumbersShow = true;
			this.colorbarAxisPanel.RightAxisMajorTickNumbersSpacing = 10;
			this.colorbarAxisPanel.RightAxisMajorTickOffset = 2;
			this.colorbarAxisPanel.RightAxisShow = true;
			this.colorbarAxisPanel.RightAxisStartValue = 0F;
			this.colorbarAxisPanel.Size = new System.Drawing.Size(64, 230);
			this.colorbarAxisPanel.TabIndex = 1;
			this.colorbarAxisPanel.TopAxisEndValue = 0F;
			this.colorbarAxisPanel.TopAxisLabel = "Axis Label Not Set";
			this.colorbarAxisPanel.TopAxisLabelShow = true;
			this.colorbarAxisPanel.TopAxisMajorTickHeight = 10;
			this.colorbarAxisPanel.TopAxisMajorTickNumbersFormat = "0";
			this.colorbarAxisPanel.TopAxisMajorTickNumbersShow = true;
			this.colorbarAxisPanel.TopAxisMajorTickNumbersSpacing = 10;
			this.colorbarAxisPanel.TopAxisMajorTickOffset = 2;
			this.colorbarAxisPanel.TopAxisShow = false;
			this.colorbarAxisPanel.TopAxisStartValue = 0F;
			// 
			// viewerAxisPanel
			// 
			this.viewerAxisPanel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.viewerAxisPanel.BottomAxisEndValue = 0F;
			this.viewerAxisPanel.BottomAxisLabel = "Axis Label Not Set";
			this.viewerAxisPanel.BottomAxisLabelShow = true;
			this.viewerAxisPanel.BottomAxisMajorTickHeight = 3;
			this.viewerAxisPanel.BottomAxisMajorTickNumbersFormat = "0";
			this.viewerAxisPanel.BottomAxisMajorTickNumbersShow = true;
			this.viewerAxisPanel.BottomAxisMajorTickNumbersSpacing = 10;
			this.viewerAxisPanel.BottomAxisMajorTickOffset = 2;
			this.viewerAxisPanel.BottomAxisShow = true;
			this.viewerAxisPanel.BottomAxisStartValue = 0F;
			this.viewerAxisPanel.Controls.Add(this.viewerPanel);
			this.viewerAxisPanel.Font = new System.Drawing.Font("Arial", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Pixel, ((System.Byte)(0)));
			this.viewerAxisPanel.LeftAxisEndValue = 0F;
			this.viewerAxisPanel.LeftAxisLabel = "Axis Label Not Set";
			this.viewerAxisPanel.LeftAxisLabelShow = true;
			this.viewerAxisPanel.LeftAxisMajorTickHeight = 3;
			this.viewerAxisPanel.LeftAxisMajorTickNumbersFormat = "0.00e00";
			this.viewerAxisPanel.LeftAxisMajorTickNumbersShow = true;
			this.viewerAxisPanel.LeftAxisMajorTickNumbersSpacing = 10;
			this.viewerAxisPanel.LeftAxisMajorTickOffset = 2;
			this.viewerAxisPanel.LeftAxisShow = true;
			this.viewerAxisPanel.LeftAxisStartValue = 0F;
			this.viewerAxisPanel.Location = new System.Drawing.Point(0, 0);
			this.viewerAxisPanel.Name = "viewerAxisPanel";
			this.viewerAxisPanel.RightAxisEndValue = 0F;
			this.viewerAxisPanel.RightAxisLabel = "Axis Label Not Set";
			this.viewerAxisPanel.RightAxisLabelShow = true;
			this.viewerAxisPanel.RightAxisMajorTickHeight = 3;
			this.viewerAxisPanel.RightAxisMajorTickNumbersFormat = "0";
			this.viewerAxisPanel.RightAxisMajorTickNumbersShow = true;
			this.viewerAxisPanel.RightAxisMajorTickNumbersSpacing = 10;
			this.viewerAxisPanel.RightAxisMajorTickOffset = 2;
			this.viewerAxisPanel.RightAxisShow = false;
			this.viewerAxisPanel.RightAxisStartValue = 0F;
			this.viewerAxisPanel.Size = new System.Drawing.Size(510, 230);
			this.viewerAxisPanel.TabIndex = 0;
			this.viewerAxisPanel.TopAxisEndValue = 0F;
			this.viewerAxisPanel.TopAxisLabel = "Axis Label Not Set";
			this.viewerAxisPanel.TopAxisLabelShow = true;
			this.viewerAxisPanel.TopAxisMajorTickHeight = 10;
			this.viewerAxisPanel.TopAxisMajorTickNumbersFormat = "0";
			this.viewerAxisPanel.TopAxisMajorTickNumbersShow = true;
			this.viewerAxisPanel.TopAxisMajorTickNumbersSpacing = 10;
			this.viewerAxisPanel.TopAxisMajorTickOffset = 2;
			this.viewerAxisPanel.TopAxisShow = true;
			this.viewerAxisPanel.TopAxisStartValue = 0F;
			// 
			// viewerPanel
			// 
			this.viewerPanel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.viewerPanel.BackColor = System.Drawing.SystemColors.Control;
			this.viewerPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.viewerPanel.Location = new System.Drawing.Point(64, 24);
			this.viewerPanel.Name = "viewerPanel";
			this.viewerPanel.Size = new System.Drawing.Size(406, 174);
			this.viewerPanel.TabIndex = 0;
			// 
			// backgroundPanel
			// 
			this.backgroundPanel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.backgroundPanel.BackColor = System.Drawing.Color.White;
			this.backgroundPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.backgroundPanel.Controls.Add(this.colorbarPanel);
			this.backgroundPanel.Controls.Add(this.colorbarAxisPanel);
			this.backgroundPanel.Controls.Add(this.viewerAxisPanel);
			this.backgroundPanel.Location = new System.Drawing.Point(0, 0);
			this.backgroundPanel.Name = "backgroundPanel";
			this.backgroundPanel.Size = new System.Drawing.Size(584, 232);
			this.backgroundPanel.TabIndex = 3;
			// 
			// Layout2DColor
			// 
			this.BackColor = System.Drawing.SystemColors.Control;
			this.Controls.Add(this.backgroundPanel);
			this.Name = "Layout2DColor";
			this.Size = new System.Drawing.Size(584, 232);
			this.viewerAxisPanel.ResumeLayout(false);
			this.backgroundPanel.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		#region Bitmap Methods
		public bool View(double [,] dataArray)
		{
			// store the data array and axis internally
			this.dataArray = dataArray;

			// create the bitmap and refresh colors in it
			this.CreateBitmap();
			this.ColorRefresh();

			return true;
		}

		private void CreateBitmap()
		{
			this.viewerBitmap = new Bitmap(this.dataArray.GetLength(0), this.dataArray.GetLength(1), PixelFormat.Format24bppRgb);
		}

		private void ColorRefresh()
		{
			if (this.dataArray != null && this.viewerBitmap != null)
			{
				this.colorbarPanel.ColorRefresh(this.dataArray, ref this.viewerBitmap);
				this.viewerPanel.Bitmap = this.viewerBitmap;

				this.viewerPanel.Invalidate();
			}
		}

		private void ConvertPoint(Point devicePoint, out int xIndex, out int yIndex)
		{
			// convert mouse coordinates into data array indexes
			xIndex = (int)((devicePoint.X/(double)this.viewerPanel.ClientSize.Width)*this.dataArray.GetLength(0));
			yIndex = (int)((devicePoint.Y/(double)this.viewerPanel.ClientSize.Height)*this.dataArray.GetLength(1));

			// we flip the y coordinate because the viewer panel is showing data flipped on Y (ggrrrr)
			yIndex = this.dataArray.GetLength(1)-yIndex-1;
		}

		public double[][] CrosshairData(Point clickPoint)
		{
			int xIndex, yIndex;
			this.ConvertPoint(clickPoint, out xIndex, out yIndex);

			double[][] returnArray = new double[2][];

			returnArray[0] = new double[this.dataArray.GetLength(0)];
			for (int i=0;i<this.dataArray.GetLength(0);i++)
				returnArray[0][i] = this.dataArray[i, yIndex];

			returnArray[1] = new double[this.dataArray.GetLength(1)];
			for (int i=0;i<this.dataArray.GetLength(1);i++)
				returnArray[1][i] = this.dataArray[xIndex, i];

			return returnArray;
		}

		public double[,] BoxData(Point startPoint, Point endPoint)
		{
			int xStart, yStart, xEnd, yEnd;
			this.ConvertPoint(startPoint, out xStart, out yStart);
			this.ConvertPoint(endPoint, out xEnd, out yEnd);

			int width = xEnd-xStart+1;
			int height = yStart-yEnd+1;

			double[,] returnArray = new double[width, height];

			for (int i=0; i<width; i++)
				for (int j=0; j<height; j++)
					returnArray[i,j] = this.dataArray[xStart+i, yEnd+j];

			return returnArray;
		}

		public double PointData(Point mousePoint, out int xIndex, out int yIndex)
		{
			this.ConvertPoint(mousePoint, out xIndex, out yIndex);

			return this.dataArray[xIndex, yIndex];
		}
		#endregion

		#region Colorbar Event and Thread Code
		private void colorbarPanel_CurrentValuesChanged(double minCurrent, double maxCurrent)
		{
			ThreadPool.QueueUserWorkItem(new WaitCallback(ThreadProcedure), null);
			// slow down
			//if (refreshThread != null)
			//	refreshThread.Join(5);

			// we launch a thread to do the refresh, otherwise things get really really sluggish
			//this.refreshThread = new System.Threading.Thread(new System.Threading.ThreadStart(ColorRefresh));
			//refreshThread.Name = "Layout2DColor ColorRefresh Thread";
			//refreshThread.IsBackground = true;
			//refreshThread.Priority = System.Threading.ThreadPriority.BelowNormal;
			//refreshThread.Start();
		}

		void ThreadProcedure(Object stateInfo) { this.ColorRefresh(); }
		#endregion

		#region Properties
		public Point Crosshair
		{
			set { this.viewerPanel.Crosshair = value; }
		}

		public Rectangle Box
		{
			set { this.viewerPanel.Box = value; }
		}

		public Size ArraySize
		{
			get { return new Size(this.dataArray.GetLength(0), this.dataArray.GetLength(1)); }
		}

		public EarLab.Viewers.Panels.Panel2DColor ViewerPanel
		{
			get { return this.viewerPanel; }
		}

		public EarLab.Viewers.Panels.PanelColorbarNew ColorbarPanel
		{
			get { return this.colorbarPanel; }
		}

		public EarLab.Viewers.Panels.PanelAxisNew ViewerAxisPanel
		{
			get { return this.viewerAxisPanel; }
		}

		public EarLab.Viewers.Panels.PanelAxisNew ColorbarAxisPanel
		{
			get { return this.colorbarAxisPanel; }
		}
		#endregion
	}
}
