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
	/// This control inherits from System.Windows.Forms.Panel and can be used to visualize 2-dimensional data.
	/// </summary>
	public class Panel2DColor : System.Windows.Forms.Panel
	{
		private System.ComponentModel.IContainer components;

		private System.Drawing.Bitmap viewerBitmap;
		private System.Windows.Forms.ToolTip toolTip;
		private System.Drawing.Pen drawPen = new System.Drawing.Pen(System.Drawing.Brushes.Red, 1);
		private Point crosshairPoint = Point.Empty;
		private Rectangle boxRectangle = Rectangle.Empty;

		/// <summary>
		/// Creates a new MultiChannel2DViewerPanel.
		/// </summary>
		public Panel2DColor()
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
			this.components = new System.ComponentModel.Container();
			this.toolTip = new System.Windows.Forms.ToolTip(this.components);

		}
		#endregion

		#region Overriden Events Handlers (OnPaint, OnResize)
		/// <summary>
		/// This event handler paints the viewer panel if the status is not Uninitialized.
		/// </summary>
		/// <param name="pe">Graphics object for this control.</param>
		protected override void OnPaint(PaintEventArgs pe)
		{
			// we use this overload of DrawImage because of an edge effect seen when scaling the bitmap to fit the viewer.
			if (this.viewerBitmap != null)
			{
				Bitmap tempBitmap = (Bitmap)this.viewerBitmap.Clone();

				pe.Graphics.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.HighQualityBicubic;
				pe.Graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;

				pe.Graphics.DrawImage(tempBitmap, new Rectangle(0, 0, this.ClientSize.Width, this.ClientSize.Height), new Rectangle(0, 0, tempBitmap.Width-1, tempBitmap.Height-1), GraphicsUnit.Pixel);

				if (this.crosshairPoint != Point.Empty)
				{
					pe.Graphics.DrawLine(this.drawPen, this.crosshairPoint.X, 0, this.crosshairPoint.X, this.Height);
					pe.Graphics.DrawLine(this.drawPen, 0, this.crosshairPoint.Y, this.Width, this.crosshairPoint.Y);
				}
				if (this.boxRectangle != Rectangle.Empty)
					pe.Graphics.DrawRectangle(this.drawPen, this.boxRectangle);

				tempBitmap.Dispose();
			}

			// Calling base class OnPaint
			base.OnPaint(pe);
		}

		protected override void OnResize(EventArgs e)
		{
			this.Invalidate();

			// Calling base class OnResize
			base.OnResize (e);
		}


		#endregion

		#region Properties
		public Bitmap Bitmap
		{
			set { this.viewerBitmap = value; }
		}

		public Rectangle Box
		{
			set
			{
				this.boxRectangle = value;
				this.Invalidate();
			}
			get
			{
				return this.boxRectangle;
			}
		}

		public Point Crosshair
		{
			set
			{
				this.crosshairPoint = value;
				this.Invalidate();
			}
		}

		public ToolTip ToolTip
		{
			get { return this.toolTip; }
		}
		#endregion
	}
}
