using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;
using System.IO;
using System.Drawing.Imaging;

namespace EarLab.Viewers.Panels
{
	/// <summary>
	/// Summary description for PanelColorbarNew.
	/// </summary>
	public class PanelColorbarNew : System.Windows.Forms.Panel
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		private double minTotal, minCurrent, maxTotal, maxCurrent;
		private Color[] colorsArray;
		private Bitmap colorsBitmap;
		private System.Windows.Forms.ContextMenu contextMenu;
		private System.Windows.Forms.OpenFileDialog openFileDialog;

		private Point clickPoint = Point.Empty;

		public PanelColorbarNew()
		{
			// This call is required by the Windows.Forms Form Designer.
			InitializeComponent();

			// We set up double buffering so that there isn't too much flicker
			this.SetStyle(ControlStyles.UserPaint|ControlStyles.AllPaintingInWmPaint|ControlStyles.DoubleBuffer, true); 
			this.UpdateStyles();

			// load the internal (embedded) default colorbar with some values
			this.minTotal = this.minCurrent = 0;
			this.maxTotal = this.maxCurrent = 512;
			this.LoadInternalColorbar("Default");

			// create the internal context menu
			this.CreateContextMenu();
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
			this.contextMenu = new System.Windows.Forms.ContextMenu();
			this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
			// 
			// openFileDialog
			// 
			this.openFileDialog.DefaultExt = "txt";
			this.openFileDialog.Filter = "Colorbar files (*.txt)|*.txt";
			this.openFileDialog.Title = "Load Colorbar File...";
			// 
			// PanelColorbarNew
			// 
			this.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.ContextMenu = this.contextMenu;
			this.Cursor = System.Windows.Forms.Cursors.SizeAll;
			this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.PanelColorbarNew_MouseUp);
			this.DoubleClick += new System.EventHandler(this.PanelColorbarNew_DoubleClick);
			this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.PanelColorbarNew_MouseMove);
			this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.PanelColorbarNew_MouseDown);

		}
		#endregion

		#region Overriden Methods
		protected override void OnPaint(PaintEventArgs pe)
		{
			if (this.colorsBitmap != null)
			{
				// find the start and height of the current colorbar settings (if shifted or shrunk)
				int heightDraw = (int)Math.Round((this.ClientSize.Height/(float)(this.maxTotal-this.minTotal))*(this.maxCurrent-this.minCurrent));
				int startDraw = (int)Math.Round((this.ClientSize.Height/(this.maxTotal -this.minTotal))*(this.maxTotal-this.maxCurrent));

				// draw the bitmap to the panel, shifted and shrunk as needed
				pe.Graphics.DrawImage(this.colorsBitmap, new Rectangle(0, startDraw, this.ClientSize.Width, heightDraw), new Rectangle(0, 0, this.colorsBitmap.Width-1, this.colorsBitmap.Height-1), GraphicsUnit.Pixel);
			}

			// Calling the base class OnPaint
			base.OnPaint(pe);
		}

		protected override void OnResize(EventArgs e)
		{
			// we need to invalidate entire colorbar, since we need to redraw  colorbar
			this.Invalidate();

			// Calling the base class OnResize
			base.OnResize (e);
		}
		#endregion

		#region Methods
		private void LoadExternalColorbar()
		{
			if (this.openFileDialog.ShowDialog(this) == DialogResult.OK)
			{

				Color[] tempArray = EarLab.ReaderWriters.ReaderWriterASCIIColorbar.Read(this.openFileDialog.FileName);
				if (tempArray != null)
				{
					this.colorsArray = tempArray;
					this.CreateBitmap();

					if (this.CurrentValuesChanged != null)
						this.CurrentValuesChanged(this.minCurrent, this.maxCurrent);
				}
			}
		}

		private void LoadInternalColorbar(string colorbarName)
		{
			Color[] tempArray = EarLab.ReaderWriters.ReaderWriterASCIIColorbar.ReadInternal(colorbarName);
			if (tempArray != null)
			{
				this.colorsArray = tempArray;
				this.CreateBitmap();

				if (this.CurrentValuesChanged != null)
					this.CurrentValuesChanged(this.minCurrent, this.maxCurrent);
			}
		}

		private void CreateBitmap()
		{
			// create a new color bitmap image
			this.colorsBitmap = new Bitmap(this.ClientSize.Width, this.colorsArray.Length, System.Drawing.Imaging.PixelFormat.Format24bppRgb);

			// draw to the color bitmap the colors in the internal array (we draw colors upside down for some reason)
			Graphics bitmapGraphics = Graphics.FromImage(this.colorsBitmap);
			for (int i=0; i<this.colorsArray.Length; i++)
				bitmapGraphics.DrawLine(new Pen(this.colorsArray[this.colorsArray.Length-1-i], 1), -1, i, this.ClientSize.Width, i);

			// refresh the panel to show the new bitmap
			this.Refresh();
		}

		public void ColorRefresh(double[,] dataArray, ref Bitmap viewerBitmap)
		{
			int index;

			Bitmap tempBitmap = (Bitmap)viewerBitmap.Clone();
			BitmapData bitmapData = tempBitmap.LockBits(new Rectangle(0, 0, tempBitmap.Width-1, tempBitmap.Height-1), ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb); 
			int bitmapStride = bitmapData.Stride; 
			System.IntPtr bitmapScan0 = bitmapData.Scan0;

			// create some local variables that may be sent into the function later
			int renderWidth = tempBitmap.Width;
			int renderHeight = tempBitmap.Height;
			int renderStart = 0;

			// create some local variables to make our loop as efficient as possible
			double colorRatio = (this.maxCurrent - this.minCurrent) / (double)this.colorsArray.Length;
			int renderHeightEnd = tempBitmap.Height-1;
			int colorsArrayLengthEnd = this.colorsArray.Length-1;

			unsafe 
			{ 
				byte* p = (byte*)bitmapScan0.ToPointer();
				long bitmapOffset = bitmapStride - (renderWidth * 3);
				p += renderStart * 3;

				for (int i=renderHeightEnd; i>=0; i--)
				{
					for(int j=renderStart; j<renderWidth; j++)
					{
						// get the index into the color array (select proper color based on data value
						index = Math.Min(colorsArrayLengthEnd, (int)Math.Max(0d, (dataArray[j, i] - this.minCurrent) / colorRatio));

						// set the red, green, and blue portions of the pixel (we need to do this backwards due to bitmap format)
						p[0] = (byte)this.colorsArray[index].B;
						p[1] = (byte)this.colorsArray[index].G;
						p[2] = (byte)this.colorsArray[index].R;
						p += 3;	// increment our pointer since we just set 3 bytes
					}
					p += bitmapOffset;
				}
			}

			tempBitmap.UnlockBits(bitmapData);
			viewerBitmap = (Bitmap)tempBitmap.Clone();
			tempBitmap.Dispose();
		}

		/*public void ColorRefresh(double[,] dataArray, ref Bitmap viewerBitmap)
		{
			int index;
			
			Bitmap tempBitmap = (Bitmap)viewerBitmap.Clone();

			// create some local variables to make our loop as efficient as possible
			double colorRatio = (this.maxCurrent - this.minCurrent) / (double)this.colorsArray.Length;
			int colorsArrayLengthEnd = this.colorsArray.Length-1;
			int viewerBitmapHeightEnd = tempBitmap.Height-1;
			int viewerBitmapHeight = tempBitmap.Height;
			int viewerBitmapWidth = tempBitmap.Width;

			for (int i=0; i<viewerBitmapWidth; i++)
			{
				// seems data is upside down so we draw bitmap flipped (silly silly)
				int k = viewerBitmapHeightEnd;
				for(int j=0; j<viewerBitmapHeight; j++)
				{
					// get the index into the color array (select proper color based on data value
					index = Math.Min(colorsArrayLengthEnd, (int)Math.Max(0d, (dataArray[i,j] - this.minCurrent) / colorRatio));
					tempBitmap.SetPixel(i, k, this.colorsArray[index]);
					k--;
				}
			}
			
			viewerBitmap = (Bitmap)tempBitmap.Clone();
			tempBitmap.Dispose();
		}*/

		public void SetMinMax(double minValue, double maxValue, bool alignCurrent)
		{
			this.minTotal = minValue;
			this.maxTotal = maxValue;

			// user might want us to align the min max and current values to each other
			if (alignCurrent)
			{
				// set the current min and current max to match the totals
				this.minCurrent = this.minTotal;
				this.maxCurrent = this.maxTotal;
			}
			else
			{
				// make sure current min and current max fall within min and max
				if (this.minTotal > this.minCurrent)
					this.minCurrent = this.minTotal;
				if (this.maxTotal < this.maxCurrent)
					this.maxCurrent = this.maxTotal;
			}

			this.Invalidate();

			// kick up a CurrentValuesChangedEvent
			if (this.CurrentValuesChanged != null)
				this.CurrentValuesChanged(this.minCurrent, this.maxCurrent);
		}
		#endregion

		#region Mouse Events
		private void PanelColorbarNew_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			// store the click to shrink/shift the colorbar
			if (e.Button == MouseButtons.Left && e.Clicks == 1)
				this.clickPoint = new Point(e.X, e.Y);
		}

		private void PanelColorbarNew_MouseMove(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			if (e.Button == MouseButtons.Left)
			{
				Point lastPoint = new Point(e.X, e.Y);

				if (this.clickPoint != lastPoint)
				{
					// create factors for the shrink/shift of the colorbar
					float shrinkFactor = (clickPoint.X-lastPoint.X)/2f;
					int shiftFactor = lastPoint.Y-clickPoint.Y;

					// find the potentially new current min and current max based on shrink and shift
					double width = this.maxTotal-this.minTotal;
					double tempMaxCurrent = Math.Min(this.maxCurrent - shrinkFactor/(this.ClientSize.Height/width) - shiftFactor/(this.ClientSize.Height/width), this.maxTotal);
					double tempMinCurrent = Math.Max(this.minCurrent + shrinkFactor/(this.ClientSize.Height/width) - shiftFactor/(this.ClientSize.Height/width), this.minTotal);

					// make sure that the new values will be within neccessary bounds
					if ((tempMaxCurrent != this.maxCurrent || tempMinCurrent != this.minCurrent)
						&& tempMaxCurrent > this.minTotal
						&& tempMinCurrent < this.maxTotal
						&& Math.Floor((this.ClientSize.Height/width)*(tempMaxCurrent-tempMinCurrent)) >= 20)
					{
						// store the new values
						if (tempMinCurrent >= this.minTotal)
							this.minCurrent = tempMinCurrent;
						if (tempMaxCurrent <= this.maxTotal)
							this.maxCurrent = tempMaxCurrent;

						// refresh the panel to show changes
						this.Invalidate();

						// kick up a CurrentValuesChangedEvent
						if (this.CurrentValuesChanged != null)
							this.CurrentValuesChanged(this.minCurrent, this.maxCurrent);
					}
				}

				// store the new position for the next time around
				this.clickPoint = lastPoint;
			}
		}

		private void PanelColorbarNew_MouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
		{
		
		}

		private void PanelColorbarNew_DoubleClick(object sender, System.EventArgs e)
		{
			this.SetMinMax(this.minTotal, this.maxTotal, true);
		}
		#endregion

		#region Context Menu Code
		private void CreateContextMenu()
		{
			string[] manifestNames = System.Reflection.Assembly.GetExecutingAssembly().GetManifestResourceNames();
			Array.Sort(manifestNames);
			Array.Reverse(manifestNames);
			foreach (string resource in manifestNames)
			{
				if (resource.StartsWith("EarLab.ColorBars."))
					this.contextMenu.MenuItems.Add(0, new MenuItem("Load " + resource.Replace("EarLab.ColorBars.", "").Replace(".txt",""),  new System.EventHandler(contextMenuLoad_Clicked)));
			}
			this.contextMenu.MenuItems.Add(new MenuItem("-"));
			this.contextMenu.MenuItems.Add(new MenuItem("Load Colorbar file...", new System.EventHandler(contextMenuLoad_Clicked)));
		}

		private void contextMenuLoad_Clicked(object sender, System.EventArgs e)
		{
			if (((MenuItem)sender).Text != "Load Colorbar file...")
				this.LoadInternalColorbar(((MenuItem)sender).Text.Substring(5));
			else
				this.LoadExternalColorbar();
		}
		#endregion

		#region Delegates and Events
		public delegate void CurrentValuesChangedHandler(double minCurrent, double maxCurrent);
		public event CurrentValuesChangedHandler CurrentValuesChanged;
		#endregion
	}
}
