using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;
using EarLab.InheritedControls;
using EarLab.Utilities;

namespace EarLab.Controls
{
	/// <summary>
	/// This class extends the System.Windows.Forms.ScrollBar class to have two theme-enabled autoscroll buttons.
	/// </summary>
	public class ExtendedScrollBar : System.Windows.Forms.UserControl
	{
		private System.ComponentModel.IContainer components;

		private NoKeyPressScrollBar hScrollBar;
		private System.Windows.Forms.PictureBox leftPictureBox;
		private System.Windows.Forms.PictureBox rightPictureBox;

		private string bitmapName;
		private Region leftPlayRegion, rightPlayRegion, stopRegion;
		private ExtendedScrollBarButtonMode buttonMode;
		private ExtendedScrollBarMovieDirection movieDirection;
		private System.Windows.Forms.ImageList stopImageList;
		private System.Windows.Forms.ImageList playImageList;
		private System.Windows.Forms.Timer autoscrollTimer;
		private int scrollAmount;

		private enum ExtendedScrollBarMovieDirection { Left, Right }
		private enum ExtendedScrollBarButtonMode { PlaySymbols, StopSymbols }
		private enum ExtendedScrollBarControlStyle { Normal, WinXP }

		/// <summary>
		/// Create a new ExtendedScrollBar.
		/// </summary>
		public ExtendedScrollBar()
		{
			// This call is required by the Windows.Forms Form Designer.
			InitializeComponent();

			this.hScrollBar.SmallChange = 10;
			this.hScrollBar.LargeChange = 100;

			this.scrollAmount = this.hScrollBar.SmallChange;
			this.autoscrollTimer.Interval = 100;

			this.buttonMode = ExtendedScrollBarButtonMode.PlaySymbols;

			// create left play symbol
			this.leftPlayRegion = new Region();
			this.leftPlayRegion.MakeEmpty();
			this.leftPlayRegion.Union(new Rectangle(1, 4, 1, 1));
			this.leftPlayRegion.Union(new Rectangle(2, 3, 1, 3));
			this.leftPlayRegion.Union(new Rectangle(3, 2, 1, 5));
			this.leftPlayRegion.Union(new Rectangle(4, 1, 1, 7));
			this.leftPlayRegion.Union(new Rectangle(5, 0, 2, 9));

			// create right play symbol
			this.rightPlayRegion = this.leftPlayRegion.Clone();
			System.Drawing.Drawing2D.Matrix matrixTransform = new System.Drawing.Drawing2D.Matrix();
			matrixTransform.RotateAt(45, new Point(4,4));
			matrixTransform.RotateAt(45, new Point(5,5));
			matrixTransform.RotateAt(45, new Point(4,4));
			matrixTransform.RotateAt(45, new Point(4,4));
			this.rightPlayRegion.Transform(matrixTransform);

			// create stop symbol
			this.stopRegion = new Region();
			this.stopRegion.MakeEmpty();
			this.stopRegion.Union(new Rectangle(1, 0, 1, 1));
			this.stopRegion.Union(new Rectangle(7, 0, 1, 1));
			this.stopRegion.Union(new Rectangle(0, 1, 3, 1));
			this.stopRegion.Union(new Rectangle(6, 1, 3, 1));
			this.stopRegion.Union(new Rectangle(1, 2, 3, 1));
			this.stopRegion.Union(new Rectangle(5, 2, 3, 1));
			this.stopRegion.Union(new Rectangle(2, 3, 5, 1));
			this.stopRegion.Union(new Rectangle(3, 4, 3, 1));
			this.stopRegion.Union(new Rectangle(2, 5, 5, 1));
			this.stopRegion.Union(new Rectangle(5, 6, 3, 1));
			this.stopRegion.Union(new Rectangle(1, 6, 3, 1));
			this.stopRegion.Union(new Rectangle(6, 7, 3, 1));
			this.stopRegion.Union(new Rectangle(0, 7, 3, 1));
			this.stopRegion.Union(new Rectangle(7, 8, 1, 1));
			this.stopRegion.Union(new Rectangle(1, 8, 1, 1));

			this.CreateBitmaps();
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
			this.components = new System.ComponentModel.Container();
			this.hScrollBar = new NoKeyPressScrollBar();
			this.leftPictureBox = new System.Windows.Forms.PictureBox();
			this.rightPictureBox = new System.Windows.Forms.PictureBox();
			this.playImageList = new System.Windows.Forms.ImageList(this.components);
			this.stopImageList = new System.Windows.Forms.ImageList(this.components);
			this.autoscrollTimer = new System.Windows.Forms.Timer(this.components);
			this.SuspendLayout();
			// 
			// hScrollBar
			// 
			this.hScrollBar.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.hScrollBar.Location = new System.Drawing.Point(17, 0);
			this.hScrollBar.Name = "hScrollBar";
			this.hScrollBar.Size = new System.Drawing.Size(344, 17);
			this.hScrollBar.TabIndex = 0;
			//this.hScrollBar.ValueChanged += new System.EventHandler(this.hScrollBar_ValueChanged);
			this.hScrollBar.Scroll += new System.Windows.Forms.ScrollEventHandler(this.hScrollBar_Scroll);
			// 
			// leftPictureBox
			// 
			this.leftPictureBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left)));
			this.leftPictureBox.Location = new System.Drawing.Point(0, 0);
			this.leftPictureBox.Name = "leftPictureBox";
			this.leftPictureBox.Size = new System.Drawing.Size(17, 17);
			this.leftPictureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
			this.leftPictureBox.TabIndex = 1;
			this.leftPictureBox.TabStop = false;
			this.leftPictureBox.Click += new System.EventHandler(this.leftPictureBox_Click);
			this.leftPictureBox.EnabledChanged += new System.EventHandler(this.leftPictureBox_EnabledChanged);
			this.leftPictureBox.MouseEnter += new System.EventHandler(this.leftPictureBox_MouseEnter);
			this.leftPictureBox.MouseUp += new System.Windows.Forms.MouseEventHandler(this.leftPictureBox_MouseUp);
			this.leftPictureBox.DoubleClick += new System.EventHandler(this.leftPictureBox_DoubleClick);
			this.leftPictureBox.MouseLeave += new System.EventHandler(this.leftPictureBox_MouseLeave);
			this.leftPictureBox.MouseDown += new System.Windows.Forms.MouseEventHandler(this.leftPictureBox_MouseDown);
			// 
			// rightPictureBox
			// 
			this.rightPictureBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.rightPictureBox.Location = new System.Drawing.Point(361, 0);
			this.rightPictureBox.Name = "rightPictureBox";
			this.rightPictureBox.Size = new System.Drawing.Size(17, 17);
			this.rightPictureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
			this.rightPictureBox.TabIndex = 2;
			this.rightPictureBox.TabStop = false;
			this.rightPictureBox.Click += new System.EventHandler(this.rightPictureBox_Click);
			this.rightPictureBox.EnabledChanged += new System.EventHandler(this.rightPictureBox_EnabledChanged);
			this.rightPictureBox.MouseEnter += new System.EventHandler(this.rightPictureBox_MouseEnter);
			this.rightPictureBox.MouseUp += new System.Windows.Forms.MouseEventHandler(this.rightPictureBox_MouseUp);
			this.rightPictureBox.DoubleClick += new System.EventHandler(this.rightPictureBox_DoubleClick);
			this.rightPictureBox.MouseLeave += new System.EventHandler(this.rightPictureBox_MouseLeave);
			this.rightPictureBox.MouseDown += new System.Windows.Forms.MouseEventHandler(this.rightPictureBox_MouseDown);
			// 
			// playImageList
			// 
			this.playImageList.ColorDepth = System.Windows.Forms.ColorDepth.Depth32Bit;
			this.playImageList.ImageSize = new System.Drawing.Size(17, 17);
			this.playImageList.TransparentColor = System.Drawing.Color.Transparent;
			// 
			// stopImageList
			// 
			this.stopImageList.ColorDepth = System.Windows.Forms.ColorDepth.Depth32Bit;
			this.stopImageList.ImageSize = new System.Drawing.Size(17, 17);
			this.stopImageList.TransparentColor = System.Drawing.Color.Transparent;
			// 
			// autoscrollTimer
			// 
			this.autoscrollTimer.Tick += new System.EventHandler(this.autoscrollTimer_Tick);
			// 
			// ExtendedScrollBar
			// 
			this.Controls.Add(this.rightPictureBox);
			this.Controls.Add(this.leftPictureBox);
			this.Controls.Add(this.hScrollBar);
			this.Name = "ExtendedScrollBar";
			this.Size = new System.Drawing.Size(378, 17);
			this.SystemColorsChanged += new System.EventHandler(this.ExtendedScrollBar_SystemColorsChanged);
			this.EnabledChanged += new System.EventHandler(this.ExtendedScrollBar_EnabledChanged);
			this.ResumeLayout(false);

		}
		#endregion

		#region Methods
		/// <summary>
		/// Create the bitmaps for the autoscroll buttons.
		/// </summary>
		private void CreateBitmaps()
		{
			// clear the lists if they have images in them already
			if (this.playImageList.Images.Count > 0)
				this.playImageList.Images.Clear();
			if (this.stopImageList.Images.Count > 0)
				this.stopImageList.Images.Clear();

			try
			{
				// make sure that the OS can handle xp themes
				if (Environment.OSVersion.Version.Major > 4 && Environment.OSVersion.Version.Minor > 0)
				{
					// we have to declare these seperately, else somehow they get linked and store the same info.
					string themePath = new String('\0', 256);
					string themeColor = new String('\0', 256);
					string themeSize = new String('\0', 256);

					// call the Themes API and get the current theme name, color, and size
					NativeMethods.GetCurrentThemeName(themePath, 256, themeColor, 256, themeSize, 256);

					// trim the trailing '\0' character off of the strings to get the actual resulting info
					themePath = themePath.TrimEnd('\0');
					themeColor = themeColor.TrimEnd('\0');
					themeSize = themeSize.TrimEnd('\0');

					if (themePath.TrimEnd('\0') == "")
					{
						this.CreateBitmapsNormal();
						return;
					}

					// luna style is wierd, "NormalColor" is actually stored as "Blue" in resources, stupid monkeys
					if (themeColor == "NormalColor" && themePath.IndexOf("Luna") >= 0)
						this.bitmapName = "BLUE";
					else
						this.bitmapName = themePath.Substring(themePath.LastIndexOf('\\')+1, themePath.Length-themePath.LastIndexOf('\\')-10);

					// make sure that themes are actually turned on
					if (themePath != "" && themeColor != "")
						this.CreateBitmapsThemed(themePath);
					else
						this.CreateBitmapsNormal();
				}
				else
					this.CreateBitmapsNormal();
			}
			catch
			{
				this.CreateBitmapsNormal();
			}
		}

		private void CreateBitmapsThemed(string themePath)
		{
			// extract the bitmaps we need out of the msstyle files for custom themes, praying that naming conventions are correct
			Bitmap buttonsBitmap, symbolsBitmap;
			System.IntPtr hModule = IntPtr.Zero;
			try
			{
				hModule = NativeMethods.LoadLibraryEx(themePath, System.IntPtr.Zero, NativeMethods.LOAD_LIBRARY_AS_DATAFILE);
				buttonsBitmap = Bitmap.FromResource(hModule, this.bitmapName.ToUpper() + "_SCROLLARROWS_BMP");
				symbolsBitmap = Bitmap.FromResource(hModule, this.bitmapName.ToUpper() + "_SCROLLARROWGLYPHS_BMP");
			}
			catch
			{
				this.CreateBitmapsNormal();
				return;
			}
			finally
			{
				NativeMethods.FreeLibrary(hModule);	// close it lest we do something super bad
			}


			Bitmap tempBitmap, symbolBitmap;

			for (int i=0;i<4;i++)
			{
				tempBitmap = new Bitmap(17, 17, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
				symbolBitmap = new Bitmap(9, 9, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
				Graphics.FromImage(tempBitmap).DrawImageUnscaled(buttonsBitmap, 0, -136-(i*17));

				if (i != 3)
					Graphics.FromImage(symbolBitmap).FillRegion(new SolidBrush(this.FindColor(ref symbolsBitmap, i)), this.leftPlayRegion);
				else
					Graphics.FromImage(symbolBitmap).FillRegion(new SolidBrush(this.FindColor(ref symbolsBitmap, i)), this.leftPlayRegion);
				Graphics.FromImage(tempBitmap).DrawImage(symbolBitmap, 4, 4, 9, 9);
				this.playImageList.Images.Add(tempBitmap);
			}

			for (int i=4;i<8;i++)
			{
				tempBitmap = new Bitmap(17, 17, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
				symbolBitmap = new Bitmap(9, 9, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
				Graphics.FromImage(tempBitmap).DrawImageUnscaled(buttonsBitmap, 0, -136-(i*17));
				if (i != 7)
					Graphics.FromImage(symbolBitmap).FillRegion(new SolidBrush(this.FindColor(ref symbolsBitmap, i)), this.rightPlayRegion);
				else
					Graphics.FromImage(symbolBitmap).FillRegion(new SolidBrush(this.FindColor(ref symbolsBitmap, i)), this.rightPlayRegion);
				Graphics.FromImage(tempBitmap).DrawImage(symbolBitmap, 4, 4, 9, 9);
				this.playImageList.Images.Add(tempBitmap);
			}

			for (int i=0;i<8;i++)
			{
				tempBitmap = new Bitmap(17, 17, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
				symbolBitmap = new Bitmap(9, 9, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
				Graphics.FromImage(tempBitmap).DrawImageUnscaled(buttonsBitmap, 0, -136-(i*17));
				if (i != 3 || i != 7)
					Graphics.FromImage(symbolBitmap).FillRegion(new SolidBrush(this.FindColor(ref symbolsBitmap, i)), this.stopRegion);
				else
					Graphics.FromImage(symbolBitmap).FillRegion(new SolidBrush(this.FindColor(ref symbolsBitmap, i)), this.stopRegion);
				Graphics.FromImage(tempBitmap).DrawImage(symbolBitmap, 4, 4, 9, 9);
				this.stopImageList.Images.Add(tempBitmap);
			}

			this.leftPictureBox.BorderStyle = rightPictureBox.BorderStyle = BorderStyle.None;

			this.leftPictureBox_MouseLeave(this, new System.EventArgs());
			this.rightPictureBox_MouseLeave(this, new System.EventArgs());
		}

		private Color FindColor(ref Bitmap symbolBitmap, int bitmapIndex)
		{
			for (int i=0;i<9;i++)
			{
				for (int j=72+bitmapIndex*9;j<72+bitmapIndex*9+9;j++)
				{
					if (symbolBitmap.GetPixel(i,j).R != Color.Magenta.R
						|| symbolBitmap.GetPixel(i,j).G != Color.Magenta.G
						|| symbolBitmap.GetPixel(i,j).B != Color.Magenta.B)
						return symbolBitmap.GetPixel(i,j);
				}
			}

			return SystemColors.ControlText;
		}

		private void CreateBitmapsNormal()
		{
			Bitmap buttonBitmap, symbolBitmap;

			// draw the left play symbols
			buttonBitmap = new Bitmap(17, 17, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
			symbolBitmap = new Bitmap(9, 9, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
			Graphics.FromImage(buttonBitmap).FillRectangle(new SolidBrush(System.Drawing.SystemColors.Control), this.leftPictureBox.ClientRectangle);
			Graphics.FromImage(symbolBitmap).FillRegion(new SolidBrush(SystemColors.ControlText), this.leftPlayRegion);
			Graphics.FromImage(buttonBitmap).DrawImage(symbolBitmap, 4, 4, 9, 9);
			for (int i=0;i<3;i++)
				this.playImageList.Images.Add(buttonBitmap);
			buttonBitmap = new Bitmap(17, 17, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
			Graphics.FromImage(symbolBitmap).FillRegion(new SolidBrush(SystemColors.GrayText), this.leftPlayRegion);
			Graphics.FromImage(buttonBitmap).DrawImage(symbolBitmap, 4, 4, 9, 9);
			this.playImageList.Images.Add(buttonBitmap);

			// draw the right play symbols
			buttonBitmap = new Bitmap(17, 17, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
			symbolBitmap = new Bitmap(9, 9, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
			Graphics.FromImage(buttonBitmap).FillRectangle(new SolidBrush(System.Drawing.SystemColors.Control), this.rightPictureBox.ClientRectangle);
			Graphics.FromImage(symbolBitmap).FillRegion(new SolidBrush(SystemColors.ControlText), this.rightPlayRegion);
			Graphics.FromImage(buttonBitmap).DrawImage(symbolBitmap, 4, 4, 9, 9);
			for (int i=0;i<3;i++)
				this.playImageList.Images.Add(buttonBitmap);
			buttonBitmap = new Bitmap(17, 17, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
			Graphics.FromImage(symbolBitmap).FillRegion(new SolidBrush(SystemColors.GrayText), this.rightPlayRegion);
			Graphics.FromImage(buttonBitmap).DrawImage(symbolBitmap, 4, 4, 9, 9);
			this.playImageList.Images.Add(buttonBitmap);

			// draw the stop symbols (left and right are the same)
			buttonBitmap = new Bitmap(17, 17, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
			symbolBitmap = new Bitmap(9, 9, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
			Graphics.FromImage(buttonBitmap).FillRectangle(new SolidBrush(System.Drawing.SystemColors.Control), this.leftPictureBox.ClientRectangle);
			Graphics.FromImage(symbolBitmap).FillRegion(new SolidBrush(SystemColors.ControlText), this.stopRegion);
			Graphics.FromImage(buttonBitmap).DrawImage(symbolBitmap, 4, 4, 9, 9);
			for (int i=0;i<3;i++)
				this.stopImageList.Images.Add(buttonBitmap);
			buttonBitmap = new Bitmap(17, 17, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
			Graphics.FromImage(symbolBitmap).FillRegion(new SolidBrush(SystemColors.GrayText), this.stopRegion);
			Graphics.FromImage(buttonBitmap).DrawImage(symbolBitmap, 4, 4, 9, 9);
			this.stopImageList.Images.Add(buttonBitmap);

			// create copies of the left stop buttons for the right stop buttons to use
			for (int i=0; i<4; i++)
				this.stopImageList.Images.Add(this.stopImageList.Images[i]);

			this.leftPictureBox.BorderStyle = rightPictureBox.BorderStyle = BorderStyle.FixedSingle;

			this.leftPictureBox_MouseLeave(this, new System.EventArgs());
			this.rightPictureBox_MouseLeave(this, new System.EventArgs());
		}

		/// <summary>
		/// Handle the button click events of the two autoscroll buttons, and set timers and images appropriately.
		/// </summary>
		private void MovieButtonsClicked()
		{
			if (this.buttonMode == ExtendedScrollBarButtonMode.PlaySymbols)
			{
				this.leftPictureBox.Image = stopImageList.Images[0];
				this.rightPictureBox.Image = stopImageList.Images[4];
				this.buttonMode = ExtendedScrollBarButtonMode.StopSymbols;

				// start the auto-scroll timer
				this.autoscrollTimer.Start();
			}
			else
			{
				this.leftPictureBox.Image = playImageList.Images[0];
				this.rightPictureBox.Image = playImageList.Images[4];
				this.buttonMode = ExtendedScrollBarButtonMode.PlaySymbols;

				// stop the auto-scroll timer if it is still active
				if (this.autoscrollTimer.Enabled)
					this.autoscrollTimer.Stop();
			}
		}
		#endregion

		#region PictureBox Events
		private void rightPictureBox_MouseEnter(object sender, System.EventArgs e)
		{
			if (this.buttonMode == ExtendedScrollBarButtonMode.PlaySymbols)
				this.rightPictureBox.Image = playImageList.Images[5];
			else
				this.rightPictureBox.Image = stopImageList.Images[5];
		}

		private void rightPictureBox_MouseLeave(object sender, System.EventArgs e)
		{
			if (this.buttonMode == ExtendedScrollBarButtonMode.PlaySymbols)
				this.rightPictureBox.Image = playImageList.Images[4];
			else
				this.rightPictureBox.Image = stopImageList.Images[4];
		}

		private void rightPictureBox_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			if (this.buttonMode == ExtendedScrollBarButtonMode.PlaySymbols)
				this.rightPictureBox.Image = playImageList.Images[6];
			else
				this.rightPictureBox.Image = stopImageList.Images[6];
		}

		private void rightPictureBox_MouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			if (this.buttonMode == ExtendedScrollBarButtonMode.PlaySymbols)
				this.rightPictureBox.Image = playImageList.Images[4];
			else
				this.rightPictureBox.Image = stopImageList.Images[4];
		}

		private void rightPictureBox_EnabledChanged(object sender, System.EventArgs e)
		{
			if (rightPictureBox.Enabled)
				this.rightPictureBox.Image = playImageList.Images[4];
			else
				this.rightPictureBox.Image = playImageList.Images[7];
		}

		private void rightPictureBox_Click(object sender, System.EventArgs e)
		{
			this.movieDirection = ExtendedScrollBarMovieDirection.Right;
			this.MovieButtonsClicked();
		}

		private void rightPictureBox_DoubleClick(object sender, System.EventArgs e)
		{
			this.movieDirection = ExtendedScrollBarMovieDirection.Right;
			this.MovieButtonsClicked();
		}

		private void leftPictureBox_MouseEnter(object sender, System.EventArgs e)
		{
			if (this.buttonMode == ExtendedScrollBarButtonMode.PlaySymbols)
				this.leftPictureBox.Image = playImageList.Images[1];
			else
				this.leftPictureBox.Image = stopImageList.Images[1];
		}

		private void leftPictureBox_MouseLeave(object sender, System.EventArgs e)
		{
			if (this.buttonMode == ExtendedScrollBarButtonMode.PlaySymbols)
				this.leftPictureBox.Image = playImageList.Images[0];
			else
				this.leftPictureBox.Image = stopImageList.Images[0];
		}

		private void leftPictureBox_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			if (this.buttonMode == ExtendedScrollBarButtonMode.PlaySymbols)
				this.leftPictureBox.Image = playImageList.Images[2];
			else
				this.leftPictureBox.Image = stopImageList.Images[2];
		}

		private void leftPictureBox_MouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			if (this.buttonMode == ExtendedScrollBarButtonMode.PlaySymbols)
				this.leftPictureBox.Image = playImageList.Images[0];
			else
				this.leftPictureBox.Image = stopImageList.Images[0];
		}

		private void leftPictureBox_EnabledChanged(object sender, System.EventArgs e)
		{
			if (leftPictureBox.Enabled)
				this.leftPictureBox.Image = playImageList.Images[0];
			else
				this.leftPictureBox.Image = playImageList.Images[3];
		}

		private void leftPictureBox_Click(object sender, System.EventArgs e)
		{
			this.movieDirection = ExtendedScrollBarMovieDirection.Left;
			this.MovieButtonsClicked();
		}

		private void leftPictureBox_DoubleClick(object sender, System.EventArgs e)
		{
			this.movieDirection = ExtendedScrollBarMovieDirection.Left;
			this.MovieButtonsClicked();
		}
		#endregion		

		#region ScrollBar Events
		/// <summary>
		/// Event thrown when scrollbar is scrolled using any of the buttons or thumb bar.
		/// </summary>
		public new event System.Windows.Forms.ScrollEventHandler Scroll;
		private void hScrollBar_Scroll(object sender, System.Windows.Forms.ScrollEventArgs e)
		{
			// we check on the scroll type due to a bug in .NET that causes dual scroll events to
			// be triggered for only 1 click of the scroll buttons
			if (e.Type == ScrollEventType.SmallIncrement
				|| e.Type == ScrollEventType.SmallDecrement
				|| e.Type == ScrollEventType.LargeIncrement
				|| e.Type == ScrollEventType.LargeDecrement
				|| e.Type == ScrollEventType.ThumbTrack
				)
			{
				//System.Diagnostics.Debug.WriteLine(e.NewValue);
				if (this.Scroll != null)
				{
					//System.Diagnostics.Debug.WriteLine("Value: " + e.NewValue.ToString());
					//System.Diagnostics.Debug.WriteLine("Min: " + this.hScrollBar.Minimum);
					//System.Diagnostics.Debug.WriteLine("Max: " + this.hScrollBar.Maximum);

					this.Scroll(this, e);
				}
			}
		}

		/// <summary>
		/// Event thrown when scrollbar current position (value) changes.
		/// </summary>
//		public event System.EventHandler ValueChanged;
//		private void hScrollBar_ValueChanged(object sender, System.EventArgs e)
//		{
//			if (this.ValueChanged != null)
//				this.ValueChanged(this, e);
//		}
		#endregion

		#region Timer Event
		/// <summary>
		/// Handles the Tick event of the scroll timer, causing autoscroll to occur.
		/// </summary>
		/// <param name="sender">Sender object</param>
		/// <param name="e">EventArgs of the event</param>
		private void autoscrollTimer_Tick(object sender, System.EventArgs e)
		{
			int newScroll;

			if (this.movieDirection == ExtendedScrollBarMovieDirection.Left)
			{
				newScroll = this.hScrollBar.Value - this.scrollAmount;
				if (newScroll <= this.hScrollBar.Minimum)
				{
					MovieButtonsClicked();
					this.hScrollBar.Value = newScroll = this.hScrollBar.Minimum;
				}
				else
					this.hScrollBar.Value = newScroll;
			}
			else
			{
				newScroll = this.hScrollBar.Value + this.scrollAmount;
				if (newScroll >= this.hScrollBar.Maximum)
				{
					MovieButtonsClicked();
					this.hScrollBar.Value = newScroll = this.hScrollBar.Maximum;
				}
				else
					this.hScrollBar.Value = newScroll;
			}

			// call the scroll event handler, to make sure a scroll event is thrown to any connected delegates
			this.hScrollBar_Scroll(this, new System.Windows.Forms.ScrollEventArgs(ScrollEventType.SmallIncrement, newScroll));
		}
		#endregion

		#region Control Events
		/// <summary>
		/// Handles the SystemColorsChanged event of the ExtendedScrollBar, creating new bitmaps when new theme is activated.
		/// </summary>
		/// <param name="sender">Sender object</param>
		/// <param name="e">EventArgs of the event</param>
		private void ExtendedScrollBar_SystemColorsChanged(object sender, System.EventArgs e)
		{
			this.CreateBitmaps();
		}

		/// <summary>
		/// Handles the EnableChanged event of the ExtendedScrollBar, makind sure to disable autoscroll if needed.
		/// </summary>
		/// <param name="sender">Sender object</param>
		/// <param name="e">EventArgs of the event</param>
		private void ExtendedScrollBar_EnabledChanged(object sender, System.EventArgs e)
		{
			// make sure we stop the scroll timer if it is active
			if (!this.Enabled && this.autoscrollTimer.Enabled)
				this.MovieButtonsClicked();
		}
		#endregion

		#region Control Properties
		/// <summary>
		/// Gets or sets the auto-scrolling state of the extended scrollbar.
		/// </summary>
		public bool AutoScrolling
		{
			get { return this.autoscrollTimer.Enabled; }
			set
			{
				if (value == false && this.autoscrollTimer.Enabled)
					this.MovieButtonsClicked();
                else if (value == true && !this.autoscrollTimer.Enabled)
                {
                    this.MovieButtonsClicked();
                }
			}
		}
		/// <summary>
		/// Gets or sets the current position (value) of the scrollbar.
		/// </summary>
		public int Value
		{
			get { return this.hScrollBar.Value; }
			set { this.hScrollBar.Value = value; }
		}

		/// <summary>
		/// Gets or sets the minimum value of the scrollbar.
		/// </summary>
		public int Minimum
		{
			get { return this.hScrollBar.Minimum; }
			set { this.hScrollBar.Minimum = value; }
		}

		/// <summary>
		/// Gets or sets the maximum value of the srollbar.
		/// </summary>
		public int Maximum
		{
			get { return this.hScrollBar.Maximum; }
			set { this.hScrollBar.Maximum = value; }
		}

		/// <summary>
		/// Gets or sets the value to be added or subtracted from the current scrollbar position (value) when the scrollbox is moved a short distance.
		/// </summary>
		public int SmallChange
		{
			get { return this.hScrollBar.SmallChange; }
			set { this.hScrollBar.SmallChange = value; }
		}

		/// <summary>
		/// Gets or sets the value to be added or subtracted from the current scrollbar position (value) when the scrollbox is moved a large distance.
		/// </summary>
		public int LargeChange
		{
			get { return this.hScrollBar.LargeChange; }
			set { this.hScrollBar.LargeChange = value; }
		}

		/// <summary>
		/// Gets or sets the time interval for the autoscroll function.
		/// </summary>
		public int TimerInterval
		{
			get { return this.autoscrollTimer.Interval; }
			set { this.autoscrollTimer.Interval = value; }
		}

		/// <summary>
		/// Gets or sets the amount (in pixels) that will be scrolled during an auto-scroll step.
		/// </summary>
		public int TimerAmount
		{
			get { return this.scrollAmount; }
			set { this.scrollAmount = Math.Abs(value); }
		}
		#endregion
	}
}
