using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;
using EarLab.ReaderWriters;
using EarLab.Utilities;

namespace EarLab.Viewers.Layouts
{
	/// <summary>
	/// Summary description for LayoutScatterFullScrollable.
	/// </summary>
	public class LayoutScatterFullScrollable : System.Windows.Forms.UserControl, ILayout
	{
		/// <summary> 
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		private ReaderWriterBinarySpikes readerWriterBinarySpikes;
		private System.Windows.Forms.ContextMenu contextMenu;
		private System.Windows.Forms.MenuItem optionsMenuItem;
		private System.Windows.Forms.MenuItem zoomMenuItem;
		private System.Windows.Forms.MenuItem zoomInMenuItem;
		private System.Windows.Forms.MenuItem zoomOutMenuItem;
		private System.Windows.Forms.MenuItem seperatorMenuItem;
		private System.Windows.Forms.MenuItem zoomLevelsMenuItem;
		private System.Windows.Forms.MenuItem tipMenuItem;
		private System.Windows.Forms.MenuItem seperator1MenuItem;
		private System.Windows.Forms.MenuItem saveMenuItem;

		private MainMenu mainMenu;
		private bool toolTipShow = false;

		private double startTime, endTime;
		private float pixelTime = .0001f;
		private int zoomLevel = 1;
		private int zoomMax = 10;
		private float zoomTime = .0001f;

		private ReaderWriterBinarySpikes.SpikeItem[] spikesArray;
		private EarLab.Controls.ExtendedScrollBar extendedScrollBar;

		private Point clickStart = Point.Empty;
		private Point clickEnd = Point.Empty;
		private Point tempStart = Point.Empty;
		private EarLab.Viewers.Layouts.LayoutScatterFull layoutScatterFull;
		private System.Windows.Forms.SaveFileDialog saveFileDialog;
		private Point tempEnd = Point.Empty;

		public LayoutScatterFullScrollable()
		{
			// This call is required by the Windows.Forms Form Designer.
			InitializeComponent();
			EarLab.Utilities.EnableVisualStyles.EnableControl(this);

			this.layoutScatterFull.LayoutScatter.ViewerPanel.MouseMove += new MouseEventHandler(ViewerPanel_MouseMove);

			// add the analysis menu to the main menu
			this.optionsMenuItem.MenuItems.Add(1, this.layoutScatterFull.AnalysisMenu);
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
			this.extendedScrollBar = new EarLab.Controls.ExtendedScrollBar();
			this.contextMenu = new System.Windows.Forms.ContextMenu();
			this.optionsMenuItem = new System.Windows.Forms.MenuItem();
			this.zoomMenuItem = new System.Windows.Forms.MenuItem();
			this.zoomInMenuItem = new System.Windows.Forms.MenuItem();
			this.zoomOutMenuItem = new System.Windows.Forms.MenuItem();
			this.seperatorMenuItem = new System.Windows.Forms.MenuItem();
			this.zoomLevelsMenuItem = new System.Windows.Forms.MenuItem();
			this.tipMenuItem = new System.Windows.Forms.MenuItem();
			this.seperator1MenuItem = new System.Windows.Forms.MenuItem();
			this.saveMenuItem = new System.Windows.Forms.MenuItem();
			this.layoutScatterFull = new EarLab.Viewers.Layouts.LayoutScatterFull();
			this.saveFileDialog = new System.Windows.Forms.SaveFileDialog();
			this.SuspendLayout();
			// 
			// extendedScrollBar
			// 
			this.extendedScrollBar.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.extendedScrollBar.AutoScrolling = false;
			this.extendedScrollBar.LargeChange = 100;
			this.extendedScrollBar.Location = new System.Drawing.Point(124, 232);
			this.extendedScrollBar.Maximum = 100;
			this.extendedScrollBar.Minimum = 0;
			this.extendedScrollBar.Name = "extendedScrollBar";
			this.extendedScrollBar.Size = new System.Drawing.Size(486, 17);
			this.extendedScrollBar.SmallChange = 10;
			this.extendedScrollBar.TabIndex = 1;
			this.extendedScrollBar.TimerAmount = 10;
			this.extendedScrollBar.TimerInterval = 100;
			this.extendedScrollBar.Value = 0;
			this.extendedScrollBar.Scroll += new System.Windows.Forms.ScrollEventHandler(this.extendedScrollBar_Scroll);
			// 
			// contextMenu
			// 
			this.contextMenu.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																						this.optionsMenuItem});
			// 
			// optionsMenuItem
			// 
			this.optionsMenuItem.Index = 0;
			this.optionsMenuItem.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																							this.zoomMenuItem,
																							this.tipMenuItem,
																							this.seperator1MenuItem,
																							this.saveMenuItem});
			this.optionsMenuItem.Text = "&Options";
			// 
			// zoomMenuItem
			// 
			this.zoomMenuItem.Index = 0;
			this.zoomMenuItem.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																						 this.zoomInMenuItem,
																						 this.zoomOutMenuItem,
																						 this.seperatorMenuItem,
																						 this.zoomLevelsMenuItem});
			this.zoomMenuItem.Text = "&Zoom";
			// 
			// zoomInMenuItem
			// 
			this.zoomInMenuItem.Enabled = false;
			this.zoomInMenuItem.Index = 0;
			this.zoomInMenuItem.Shortcut = System.Windows.Forms.Shortcut.F1;
			this.zoomInMenuItem.Text = "Zoom &In";
			this.zoomInMenuItem.Click += new System.EventHandler(this.zoomInMenuItem_Click);
			// 
			// zoomOutMenuItem
			// 
			this.zoomOutMenuItem.Index = 1;
			this.zoomOutMenuItem.Shortcut = System.Windows.Forms.Shortcut.F2;
			this.zoomOutMenuItem.Text = "Zoom &Out";
			this.zoomOutMenuItem.Click += new System.EventHandler(this.zoomOutMenuItem_Click);
			// 
			// seperatorMenuItem
			// 
			this.seperatorMenuItem.Index = 2;
			this.seperatorMenuItem.Text = "-";
			// 
			// zoomLevelsMenuItem
			// 
			this.zoomLevelsMenuItem.Index = 3;
			this.zoomLevelsMenuItem.Shortcut = System.Windows.Forms.Shortcut.F3;
			this.zoomLevelsMenuItem.Text = "&Zoom Levels";
			// 
			// tipMenuItem
			// 
			this.tipMenuItem.Index = 1;
			this.tipMenuItem.Shortcut = System.Windows.Forms.Shortcut.CtrlT;
			this.tipMenuItem.Text = "&Show Tool Tip";
			this.tipMenuItem.Click += new System.EventHandler(this.tipMenuItem_Click);
			// 
			// seperator1MenuItem
			// 
			this.seperator1MenuItem.Index = 2;
			this.seperator1MenuItem.Text = "-";
			// 
			// saveMenuItem
			// 
			this.saveMenuItem.Index = 3;
			this.saveMenuItem.Text = "Save &Image...";
			this.saveMenuItem.Click += new System.EventHandler(this.saveMenuItem_Click);
			// 
			// layoutScatterFull
			// 
			this.layoutScatterFull.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.layoutScatterFull.Location = new System.Drawing.Point(0, 0);
			this.layoutScatterFull.Name = "layoutScatterFull";
			this.layoutScatterFull.Size = new System.Drawing.Size(656, 224);
			this.layoutScatterFull.TabIndex = 2;
			// 
			// saveFileDialog
			// 
			this.saveFileDialog.Filter = "BMP Windows Bitmap (*.bmp)|*.bmp|JPEG Joint Photographic Experts Group (*.jpg)|*." +
				"jpg|GIF Graphics Interchange Format (*.gif)|*.gif|PNG Portable Network Graphics " +
				"(*.png)|*.png|TIFF Tag Image File Format (*.tiff)|*.tiff";
			this.saveFileDialog.Title = "Save File...";
			// 
			// LayoutScatterFullScrollable
			// 
			this.Controls.Add(this.layoutScatterFull);
			this.Controls.Add(this.extendedScrollBar);
			this.Name = "LayoutScatterFullScrollable";
			this.Size = new System.Drawing.Size(656, 256);
			this.ResumeLayout(false);

		}
		#endregion


		protected override void OnResize(EventArgs e)
		{
			base.OnResize (e);

			if (this.readerWriterBinarySpikes != null && this.ClientSize.Width > 0 && this.ClientSize.Height > 0)
			{
				this.SetupZoom();
				if (this.zoomLevel > this.zoomMax)
				{
					this.zoomLevel = this.zoomMax;
					this.pixelTime = (float)Math.Round(this.zoomTime*this.zoomLevel, 4);
				}

				if (this.zoomLevel == this.zoomMax)
				{
					this.zoomOutMenuItem.Enabled = false;
					this.zoomInMenuItem.Enabled = true;
				}
				else if (this.zoomLevel == 1)
				{
					this.zoomOutMenuItem.Enabled = true;
					this.zoomInMenuItem.Enabled = false;
				}
				else
				{
					this.zoomOutMenuItem.Enabled = true;
					this.zoomInMenuItem.Enabled = true;
				}

				this.extendedScrollBar.Minimum = 0;
				this.extendedScrollBar.Maximum = (int)Math.Max(0, Math.Round(this.readerWriterBinarySpikes.TimeMaximum/this.pixelTime)-this.layoutScatterFull.LayoutScatter.ViewerPanel.ClientSize.Width);
				
				if (this.extendedScrollBar.Minimum != this.extendedScrollBar.Maximum)
					this.extendedScrollBar.Enabled = true;
				else
					this.extendedScrollBar.Enabled = false;

				this.RefreshAll();
			}
		}

		#region Methods
		public string Read(string fileName)
		{
			// create a new instance of the binary file reader
			if (this.readerWriterBinarySpikes != null)
				this.readerWriterBinarySpikes.Close();
			this.readerWriterBinarySpikes = new ReaderWriterBinarySpikes();
			
			// open the file for reading
			string returnString = this.readerWriterBinarySpikes.Read(fileName);
			if (returnString != "0")
				return returnString;

			// set the viewer top axis label to the file name
			string[] fileParts = fileName.Split('\\');
			this.layoutScatterFull.LayoutScatter.AxisPanel.TopAxisLabel = fileParts[fileParts.Length-1];
			this.layoutScatterFull.LayoutScatter.AxisPanel.TopAxisLabelShow = true;

			this.SetupElements();

			return "0";
		}

		private void SetupElements()
		{
			this.SetupZoom();
			this.zoomLevel = this.zoomMax;
			this.zoomInMenuItem.Enabled = true;
			this.zoomOutMenuItem.Enabled = false;

			this.pixelTime = (float)Math.Round(this.zoomTime*this.zoomLevel, 4);

			this.layoutScatterFull.VerticalSplitter.SplitterMoved += new SplitterEventHandler(VerticalSplitter_SplitterMoved);

			this.startTime = (double)0;
			
			this.extendedScrollBar.Minimum = this.extendedScrollBar.Maximum = this.extendedScrollBar.Value = 0;
			this.extendedScrollBar.Enabled = false;

			//this.extendedScrollBar.Maximum = Math.Max(0, (int)Math.Round(this.readerWriterBinarySpikes.TimeMaximum/this.pixelTime)-this.layoutScatterFull.LayoutScatter.ViewerPanel.ClientSize.Width);
			//this.extendedScrollBar.Value = 0;

			this.layoutScatterFull.SetupAxis("Cell ID", "0", 0, this.readerWriterBinarySpikes.CellCount-1, this.readerWriterBinarySpikes.TimeUnits, "0.000");

			this.RefreshAll();
		}

		private void SetupZoom()
		{
			this.zoomMax = (int)Math.Ceiling(this.readerWriterBinarySpikes.TimeMaximum / (this.layoutScatterFull.LayoutScatter.ViewerPanel.ClientSize.Width*this.zoomTime));
		}

		public void Close()
		{
			if (this.readerWriterBinarySpikes != null)
			{
				this.readerWriterBinarySpikes.Close();
				this.readerWriterBinarySpikes = null;
			}

			// remove the options menu from the main menu (if we added it)
			if (this.mainMenu != null)
				this.mainMenu.MenuItems.Remove(this.optionsMenuItem);
		}

		private void RefreshAll()
		{
			this.endTime = this.startTime + this.layoutScatterFull.LayoutScatter.ViewerPanel.ClientSize.Width*this.pixelTime;
			this.readerWriterBinarySpikes.GetData(this.startTime, this.endTime, out this.spikesArray);
			
			this.layoutScatterFull.View(this.spikesArray, this.startTime, this.endTime, this.pixelTime, this.readerWriterBinarySpikes.CellCount);
		}

		private void Zoom()
		{
			double centerTime = this.endTime - this.startTime;
			this.startTime = (double)Math.Max(0, (int)(centerTime - this.pixelTime*this.layoutScatterFull.LayoutScatter.ViewerPanel.ClientSize.Width/2f));

			this.extendedScrollBar.Maximum = (int)Math.Max(0, Math.Round(this.readerWriterBinarySpikes.TimeMaximum/this.pixelTime)-this.layoutScatterFull.LayoutScatter.ViewerPanel.ClientSize.Width);
			this.extendedScrollBar.Value = (int)(this.startTime / this.pixelTime);

			if (this.extendedScrollBar.Minimum != this.extendedScrollBar.Maximum)
				this.extendedScrollBar.Enabled = true;
			else
				this.extendedScrollBar.Enabled = false;
			
			this.RefreshAll();
		}
		#endregion

		#region Zoom Events
		private void zoomInMenuItem_Click(object sender, System.EventArgs e)
		{
			if (this.zoomLevel > 1)
			{
				this.zoomLevel--;
				this.pixelTime = (float)Math.Round(this.zoomTime*this.zoomLevel, 4);
				this.zoomOutMenuItem.Enabled = true;
				this.Zoom();
			}

			if (this.zoomLevel == 1)
				this.zoomInMenuItem.Enabled = false;
		}

		private void zoomOutMenuItem_Click(object sender, System.EventArgs e)
		{
			if (this.zoomLevel < this.zoomMax)
			{
				this.zoomLevel++;
				this.pixelTime = (float)Math.Round(this.zoomLevel*this.zoomTime, 4);
				this.zoomInMenuItem.Enabled = true;
				this.Zoom();
			}

			if (this.zoomLevel == this.zoomMax)
				this.zoomOutMenuItem.Enabled = false;
		}
		#endregion

		#region Scrollbar and Splitter Events
		private void extendedScrollBar_Scroll(object sender, System.Windows.Forms.ScrollEventArgs e)
		{
			this.startTime = e.NewValue*this.pixelTime;
			this.RefreshAll();
		}

		private void VerticalSplitter_SplitterMoved(object sender, SplitterEventArgs e)
		{
			if (this.layoutScatterFull.LayoutScatter.ViewerPanel.ClientSize.Width > 0)
			{
				this.RefreshAll();

				this.extendedScrollBar.Anchor = AnchorStyles.None;
				this.extendedScrollBar.Left = this.layoutScatterFull.LayoutScatter.Left+this.layoutScatterFull.LayoutScatter.ViewerPanel.Left;
				this.extendedScrollBar.Width = this.layoutScatterFull.LayoutScatter.ViewerPanel.Width;
				this.extendedScrollBar.Anchor = AnchorStyles.Left | AnchorStyles.Right | AnchorStyles.Bottom;
			}
		}
		#endregion

		public event EarLab.Viewers.Layouts.CriticalErrorHandler CriticalError;

		#region Mouse Events
		private void viewerPanel_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			/*if (e.Button == MouseButtons.Left && e.Clicks == 1)
			{
				// clear out any old crosshair or box that we may have
				this.viewerPanel.BoxRectangle = Rectangle.Empty;

				// store the new click point
				this.clickStart = new Point(e.X, e.Y);
			}*/
		}

		private void ViewerPanel_MouseMove(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			if (e.Button == MouseButtons.None && this.toolTipShow)
			{
				ToolTip toolTip = this.layoutScatterFull.LayoutScatter.ViewerPanel.ToolTip;
				Point mousePoint = new Point(e.X, e.Y);
				String tipString;

				tipString = "CellID: " + this.layoutScatterFull.LayoutScatter.CellID(e.Y).ToString() + "\nTime: " + this.layoutScatterFull.LayoutScatter.Time(e.X).ToString("0.000");
				//tipString = "CellID: " + (this.readerWriterBinarySpikes.CellCount-1-Math.Round((e.Y/(double)this.layoutScatterFull.LayoutScatter.ViewerPanel.Height)*this.readerWriterBinarySpikes.CellCount, 0)).ToString()
				//	+ "\nTime: " + (this.startTime+e.X*this.pixelTime).ToString("0.000");
				toolTip.SetToolTip((Control)this.layoutScatterFull.LayoutScatter.ViewerPanel, tipString);
				toolTip.Active = true;
			}
			else
				this.layoutScatterFull.LayoutScatter.ViewerPanel.ToolTip.Active = false;

			/*
			if (e.Button == MouseButtons.Left)
			{
				// store the new click point
				this.clickEnd = new Point(e.X, e.Y);

				// figure out which is upper left point
				this.tempStart = new Point(Math.Min(this.clickStart.X, this.clickEnd.X), Math.Min(this.clickStart.Y, this.clickEnd.Y));
				this.tempEnd = new Point(Math.Max(this.clickStart.X, this.clickEnd.X), Math.Max(this.clickStart.Y, this.clickEnd.Y));
				
				// fix point within viewer panel size
				this.tempStart = new Point(Math.Max(tempStart.X, 0), Math.Max(tempStart.Y, 0));
				this.tempEnd = new Point(Math.Min(tempEnd.X, this.viewerPanel.ClientRectangle.Right-1), Math.Min(tempEnd.Y, this.viewerPanel.ClientRectangle.Bottom-1));

				// tell viewer to draw box if box drawing is enabled
				this.viewerPanel.BoxRectangle = new Rectangle(this.tempStart, new Size(this.tempEnd.X-this.tempStart.X, this.tempEnd.Y-this.tempStart.Y));
			}
			*/
		}

		private void viewerPanel_MouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			/*
			if (e.Button == MouseButtons.Left && e.Clicks == 1)
			{
				if (Math.Abs(this.clickStart.X - e.X) > 2 && Math.Abs(this.clickStart.Y - e.Y) > 2)
				{
					this.viewerPanel.BoxRectangle = new Rectangle(this.tempStart, new Size(this.tempEnd.X-this.tempStart.X, this.tempEnd.Y-this.tempStart.Y));
				}
			}
			else if (e.Button == MouseButtons.Right && e.Clicks == 1)
			{
				this.viewerPanel.BoxRectangle = Rectangle.Empty;
			}
			*/
		}
		#endregion

		#region Menu Events
		private void tipMenuItem_Click(object sender, System.EventArgs e)
		{
			if (this.tipMenuItem.Checked)
				this.tipMenuItem.Checked = this.toolTipShow = false;
			else
				this.tipMenuItem.Checked = this.toolTipShow = true;
		}

		private void saveMenuItem_Click(object sender, System.EventArgs e)
		{
			if (this.saveFileDialog.ShowDialog() == DialogResult.OK)
			{
				Application.DoEvents();

				Graphics layoutGraphics = this.layoutScatterFull.CreateGraphics();
				Bitmap layoutBitmap = new Bitmap(this.layoutScatterFull.ClientSize.Width, this.layoutScatterFull.ClientSize.Height, layoutGraphics);
				Graphics bitmapGraphics = Graphics.FromImage(layoutBitmap);
				IntPtr layoutDeviceContext = layoutGraphics.GetHdc();
				IntPtr bitmapDeviceContext = bitmapGraphics.GetHdc();
				NativeMethods.BitBlt(bitmapDeviceContext, 0, 0, this.layoutScatterFull.ClientSize.Width, this.layoutScatterFull.ClientSize.Height, layoutDeviceContext, 0, 0, 13369376);
				layoutGraphics.ReleaseHdc(layoutDeviceContext);
				bitmapGraphics.ReleaseHdc(bitmapDeviceContext);

				layoutGraphics.Dispose();
				bitmapGraphics.Dispose();

				System.Drawing.Imaging.ImageFormat imageFormat;
				switch (saveFileDialog.FilterIndex)
				{
					case 2:
						imageFormat = System.Drawing.Imaging.ImageFormat.Jpeg;
						break;
					case 3:
						imageFormat = System.Drawing.Imaging.ImageFormat.Gif;
						break;
					case 4:
						imageFormat = System.Drawing.Imaging.ImageFormat.Png;
						break;
					case 5:
						imageFormat = System.Drawing.Imaging.ImageFormat.Tiff;
						break;
					default:
						imageFormat = System.Drawing.Imaging.ImageFormat.Bmp;
						break;
				}

				layoutBitmap.Save(saveFileDialog.FileName, imageFormat);
			}
		}
		#endregion

		#region Properties
		public MainMenu MainMenu
		{
			set
			{
				this.mainMenu = value;
				this.mainMenu.MenuItems.Add(1, this.optionsMenuItem);
			}
		}
		#endregion

		
	}
}
