using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;
using EarLab.ReaderWriters;
using EarLab.ColorBars;

namespace EarLab.Viewers.Panels
{
	/// <summary>
	/// This control inherits from System.Windows.Forms.Panel and can be used to visualize 2-dimensional data from a file.
	/// </summary>
	public class PanelBarWaveform : System.Windows.Forms.Panel
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		private bool normalOrientation = true;

		double minValue = double.MinValue;
		double maxValue = double.MaxValue;
        private Color drawColor = Color.Red;

		private System.Drawing.Bitmap viewerBitmap;
		private double[] dataArray;

		/// <summary>
		/// Creates a new MultiChannel2DViewerPanel.
		/// </summary>
		public PanelBarWaveform()
		{
			// This call is required by the Windows.Forms Form Designer.
			InitializeComponent();

			// We set up double buffering so that there isn't too much flicker (yeah, this is brilliant)
			this.SetStyle(ControlStyles.UserPaint|ControlStyles.AllPaintingInWmPaint|ControlStyles.DoubleBuffer, true); 
			this.UpdateStyles();
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if( components != null )
					components.Dispose();
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
			components = new System.ComponentModel.Container();
		}
		#endregion

		#region Overriden Events Handlers
		/// <summary>
		/// This event handler paints the viewer panel if the status is not Uninitialized.
		/// </summary>
		/// <param name="pe">Graphics object for this control.</param>
		protected override void OnPaint(PaintEventArgs pe)
		{
			// we use this overload of DrawImage because of an edge effect seen when scaling the bitmap to fit the viewer.
			if (this.dataArray != null)
				pe.Graphics.DrawImageUnscaled(this.viewerBitmap, 0, 0);

			// Calling base class OnPaint
			base.OnPaint(pe);
		}

		protected override void OnResize(EventArgs e)
		{
			// if we have resized this panel, we need to recreate bitmap (for sharpness)
			if (this.dataArray != null && this.ClientSize.Height > 0 && this.ClientSize.Width > 0)
			{
				this.CreateBitmap();
				this.Invalidate();
			}

			// Calling base class OnResize
			base.OnResize (e);
		}
		#endregion

		#region Methods
		private void Initialize()
		{
			double[] tempArray = (double[])this.dataArray.Clone();
			Array.Sort(tempArray, 0, tempArray.Length);

			this.minValue = 0;
			this.maxValue = tempArray[tempArray.Length-1];

			//for (int i=0; i<this.dataArray.Length; i++)
			//	this.dataArray[i] -= minValue;
		}

		private void CreateBitmap()
		{
			// we create the correct sized bitmap and draw to it, instead of stretching a data-sized bitmap to fit.  This makes it sharp.
			if (this.normalOrientation)
				this.viewerBitmap = new Bitmap(this.ClientSize.Width, this.ClientSize.Height, System.Drawing.Imaging.PixelFormat.Format24bppRgb);
			else
				this.viewerBitmap = new Bitmap(this.ClientSize.Height, this.ClientSize.Width, System.Drawing.Imaging.PixelFormat.Format24bppRgb);

			double scale;
			int offset;
			double stretch = this.viewerBitmap.Width / ((double)this.dataArray.Length-1);
			if (this.minValue == this.maxValue)
			{
				scale = 1;
				offset = this.viewerBitmap.Height / 1;
			}
			else
			{
				scale = this.viewerBitmap.Height / Math.Abs(maxValue-minValue);
				offset = 0;
			}

			System.Drawing.Graphics bitmapGraphics = Graphics.FromImage(this.viewerBitmap);
			bitmapGraphics.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.HighQualityBicubic;
			bitmapGraphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;
			System.Drawing.Pen drawPen = new System.Drawing.Pen(new SolidBrush(this.drawColor), 2);

			// we draw some nice background to make this thing look cool (white with some grid)
			System.Drawing.Brush backBrush = System.Drawing.SystemBrushes.Control;
			bitmapGraphics.FillRectangle(backBrush, -1, -1, this.viewerBitmap.Width+1, this.viewerBitmap.Height+1);
		
			// draw the actual line graph (yeah, like this is at all accurate...)
			for (int i=0;i<this.dataArray.Length;i++)
				bitmapGraphics.DrawLine(drawPen, (float)(i*stretch), 0,(float)(i*stretch), (float)(this.dataArray[i]*scale+offset));

			if (this.normalOrientation)
				this.viewerBitmap.RotateFlip(RotateFlipType.RotateNoneFlipY);
			else
				this.viewerBitmap.RotateFlip(RotateFlipType.Rotate270FlipX);

			bitmapGraphics.Dispose();
			drawPen.Dispose();
		}

		public void Close()
		{
			if (this.ClientRectangle.Width != 0 && this.ClientRectangle.Height !=0)
			{
				this.dataArray = null;
				this.viewerBitmap = null;
			}

			this.Invalidate();
		}
		#endregion

		#region Properties
		public double[] DataSource
		{
			set
			{
				if (value != null)
				{
					this.dataArray = value;
					this.Initialize();
					this.CreateBitmap();
					this.Invalidate();
				}
				else
					this.Close();
			}
		}

		public bool NormalOrientation
		{
			get { return this.normalOrientation; }
			set { this.normalOrientation = value; }
		}

		public double Min
		{
			get { return this.minValue; }
		}
		
		public double Max
		{
			get { return this.maxValue; }
		}

        public Color DrawColor { get { return this.drawColor; } set { this.drawColor = value; } }
		#endregion
	}
}
