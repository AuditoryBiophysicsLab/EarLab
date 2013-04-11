using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;
using EarLab.ReaderWriters;
using EarLab.ColorBars;
using System.Threading;

namespace EarLab.Viewers.Panels
{
	/// <summary>
	/// This control inherits from System.Windows.Forms.Panel and can be used to visualize 2-dimensional data from a file.
	/// </summary>
	public class PanelScatter : System.Windows.Forms.Panel
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		private System.Drawing.Bitmap viewerBitmap;
		private ReaderWriterBinarySpikes.SpikeItem[] spikesArray;

		private double pixelTime;
		private int cellCount;
		private double offsetTime;

		private Rectangle boxRectangle = Rectangle.Empty;
		private Pen drawPen = new Pen(Color.Blue, 1f);

		/// <summary>
		/// Creates a new MultiChannel2DViewerPanel.
		/// </summary>
		public PanelScatter()
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
		protected override void OnPaint(PaintEventArgs pe)
		{
			// we use this overload of DrawImage because of an edge effect seen when scaling the bitmap to fit the viewer.
			if (this.spikesArray != null)
			{
				pe.Graphics.DrawImageUnscaled(this.viewerBitmap, 0, 0);

				if (this.boxRectangle != Rectangle.Empty)
					pe.Graphics.DrawRectangle(this.drawPen, this.boxRectangle);
			}

			// Calling base class OnPaint
			base.OnPaint(pe);
		}
		#endregion

		#region Methods
		private void CreateBitmap()
		{
			if (this.ClientRectangle.Width != 0 && this.ClientRectangle.Height !=0)
			{
				this.viewerBitmap = new Bitmap(this.ClientSize.Width, this.ClientSize.Height, System.Drawing.Imaging.PixelFormat.Format24bppRgb);
				this.viewerBitmap.MakeTransparent(System.Drawing.Color.Black);

				foreach (ReaderWriterBinarySpikes.SpikeItem item in this.spikesArray)
				{
					int x = (int)Math.Max(0, Math.Min(((item.time-this.offsetTime)/this.pixelTime)-1, this.ClientSize.Width-1));
					int y = (int)Math.Max(0, Math.Min((this.ClientSize.Height*(item.cellID+1)/this.cellCount), this.ClientSize.Height-1));
					this.viewerBitmap.SetPixel(x, y, System.Drawing.Color.Red);
				}

				this.viewerBitmap.RotateFlip(RotateFlipType.RotateNoneFlipY);
			}
		}

		public void Close()
		{
			this.spikesArray = null;
			this.viewerBitmap = null;

			this.Invalidate();
		}
		#endregion

		#region Properties
		public ReaderWriterBinarySpikes.SpikeItem[] DataSource
		{
			set
			{
				if (value != null)
				{
					this.spikesArray = value;
					this.CreateBitmap();
					this.Invalidate();
				}
				else
					this.Close();
			}
		}

		public double PixeTime
		{
			set
			{
				this.pixelTime = value;
			}
		}

		public int CellCount
		{
			set
			{
				this.cellCount = value;
			}
		}

		public double OffsetTime
		{
			set
			{
				this.offsetTime = value;
			}
		}
		public Rectangle BoxRectangle
		{
			set
			{
				this.boxRectangle = value;
				this.Invalidate();
			}
		}
		#endregion
	}
}
