using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;
using System.IO;
using EarLab.ReaderWriters;
using EarLab.Utilities;

namespace EarLab.Viewers.Layouts
{
	/// <summary>
	/// Summary description for LayoutFullScrollable.
	/// </summary>
	public class LayoutFullScrollable : System.Windows.Forms.UserControl, ILayout
	{
		private EarLab.Viewers.Layouts.Layout2DColorFull layout2DColorFull;
		private EarLab.Controls.ExtendedScrollBar extendedScrollBar;
		private System.Windows.Forms.ContextMenu contextMenu;
		private System.Windows.Forms.MenuItem zoomInMenuItem;
		private System.Windows.Forms.MenuItem zoomOutMenuItem;
		private System.Windows.Forms.MenuItem zoomMenuItem;
		private System.Windows.Forms.MenuItem zoomLevelsMenuItem;
		private System.Windows.Forms.MenuItem seperatorMenuItem;
		private System.Windows.Forms.MenuItem optionsMenuItem;
		private System.Windows.Forms.MenuItem colorbarMenuItem;
		private System.Windows.Forms.MenuItem tipMenuItem;
		private System.Windows.Forms.MenuItem seperator1MenuItem;
		private System.Windows.Forms.MenuItem seperator2MenuItem;
		private System.Windows.Forms.MenuItem saveMenuItem;
		private System.Windows.Forms.SaveFileDialog saveFileDialog;
		/// <summary> 
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		private double[,] dataArray;
		private ReaderWriterBinary readerWriterBinary;
		private MainMenu mainMenu;
		private bool toolTipShow = false;
		private double[] dimensionSteps;

		private DataTable fileTable;
        private BackgroundWorker backgroundWorker;
		private int fileIndex = 0;

		public LayoutFullScrollable()
		{
			// This call is required by the Windows.Forms Form Designer.
			InitializeComponent();
			EarLab.Utilities.EnableVisualStyles.EnableControl(this);

			// sign up to mouse move event of the viewer panel so we can display tooltip
			this.layout2DColorFull.Layout2DColor.ViewerPanel.MouseMove += new MouseEventHandler(ViewerPanel_MouseMove);

			// add the colorbar context menu into this context menu
			ContextMenu colorbarContextMenu = this.layout2DColorFull.Layout2DColor.ColorbarPanel.ContextMenu;
			MenuItem[] colorbarMenuItems = new MenuItem[colorbarContextMenu.MenuItems.Count];
			for (int i=0; i<colorbarMenuItems.Length; i++)
				colorbarMenuItems[i] = colorbarContextMenu.MenuItems[i].CloneMenu();
			this.colorbarMenuItem.MenuItems.AddRange(colorbarMenuItems);

			// add the analysis menu to the main menu
			this.optionsMenuItem.MenuItems.Add(4, this.layout2DColorFull.AnalysisMenu);

            this.backgroundWorker.DoWork += new DoWorkEventHandler(backgroundWorker_DoWork);
            this.backgroundWorker.RunWorkerCompleted += new RunWorkerCompletedEventHandler(backgroundWorker_RunWorkerCompleted);
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(LayoutFullScrollable));
            this.layout2DColorFull = new EarLab.Viewers.Layouts.Layout2DColorFull();
            this.extendedScrollBar = new EarLab.Controls.ExtendedScrollBar();
            this.contextMenu = new System.Windows.Forms.ContextMenu();
            this.optionsMenuItem = new System.Windows.Forms.MenuItem();
            this.zoomMenuItem = new System.Windows.Forms.MenuItem();
            this.zoomInMenuItem = new System.Windows.Forms.MenuItem();
            this.zoomOutMenuItem = new System.Windows.Forms.MenuItem();
            this.seperatorMenuItem = new System.Windows.Forms.MenuItem();
            this.zoomLevelsMenuItem = new System.Windows.Forms.MenuItem();
            this.colorbarMenuItem = new System.Windows.Forms.MenuItem();
            this.tipMenuItem = new System.Windows.Forms.MenuItem();
            this.seperator1MenuItem = new System.Windows.Forms.MenuItem();
            this.seperator2MenuItem = new System.Windows.Forms.MenuItem();
            this.saveMenuItem = new System.Windows.Forms.MenuItem();
            this.saveFileDialog = new System.Windows.Forms.SaveFileDialog();
            this.backgroundWorker = new System.ComponentModel.BackgroundWorker();
            this.SuspendLayout();
            // 
            // layout2DColorFull
            // 
            this.layout2DColorFull.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.layout2DColorFull.Location = new System.Drawing.Point(0, 0);
            this.layout2DColorFull.Name = "layout2DColorFull";
            this.layout2DColorFull.Size = new System.Drawing.Size(664, 352);
            this.layout2DColorFull.TabIndex = 0;
            // 
            // extendedScrollBar
            // 
            this.extendedScrollBar.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.extendedScrollBar.AutoScrolling = false;
            this.extendedScrollBar.LargeChange = 100;
            this.extendedScrollBar.Location = new System.Drawing.Point(140, 356);
            this.extendedScrollBar.Maximum = 100;
            this.extendedScrollBar.Minimum = 0;
            this.extendedScrollBar.Name = "extendedScrollBar";
            this.extendedScrollBar.Size = new System.Drawing.Size(412, 17);
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
            this.colorbarMenuItem,
            this.tipMenuItem,
            this.seperator1MenuItem,
            this.seperator2MenuItem,
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
            // colorbarMenuItem
            // 
            this.colorbarMenuItem.Index = 1;
            this.colorbarMenuItem.Text = "&Colorbar";
            // 
            // tipMenuItem
            // 
            this.tipMenuItem.Index = 2;
            this.tipMenuItem.Shortcut = System.Windows.Forms.Shortcut.CtrlT;
            this.tipMenuItem.Text = "&Show Tool Tip";
            this.tipMenuItem.Click += new System.EventHandler(this.tipMenuItem_Click);
            // 
            // seperator1MenuItem
            // 
            this.seperator1MenuItem.Index = 3;
            this.seperator1MenuItem.Text = "-";
            // 
            // seperator2MenuItem
            // 
            this.seperator2MenuItem.Index = 4;
            this.seperator2MenuItem.Text = "-";
            // 
            // saveMenuItem
            // 
            this.saveMenuItem.Index = 5;
            this.saveMenuItem.Text = "Save &Image...";
            this.saveMenuItem.Click += new System.EventHandler(this.saveMenuItem_Click);
            // 
            // saveFileDialog
            // 
            this.saveFileDialog.Filter = resources.GetString("saveFileDialog.Filter");
            this.saveFileDialog.Title = "Save File...";
            // 
            // backgroundWorker
            // 
            this.backgroundWorker.WorkerSupportsCancellation = true;
            // 
            // LayoutFullScrollable
            // 
            this.Controls.Add(this.extendedScrollBar);
            this.Controls.Add(this.layout2DColorFull);
            this.Name = "LayoutFullScrollable";
            this.Size = new System.Drawing.Size(664, 376);
            this.ResumeLayout(false);

		}
		#endregion

		#region Methods
		public string Read(string fileName)
		{
			if (!this.GetRelatedFiles(fileName))
				return "One of the downsampled files does not match expected format.";

			return this.SetupFile(fileName);
		}

		public void Close()
		{
			if (this.readerWriterBinary != null)
			{
				this.readerWriterBinary.Close();
				this.readerWriterBinary = null;
			}

			// remove the options menu from the main menu (if we added it)
			if (this.mainMenu != null)
				this.mainMenu.MenuItems.Remove(this.optionsMenuItem);
		}

		private string SetupFile(string fileName)
		{
			// create a new instance of the binary file reader
			if (this.readerWriterBinary != null)
				this.readerWriterBinary.Close();
			this.readerWriterBinary = new ReaderWriterBinary();
			
			// open the file for reading
			string returnString = this.readerWriterBinary.Read(fileName);
			if (returnString != "0")
				return returnString;
			
			// set the viewer top axis label to the file name
			string[] fileParts = fileName.Split('\\');
			this.layout2DColorFull.Layout2DColor.ViewerAxisPanel.TopAxisLabel = fileParts[fileParts.Length-1];

			this.SetupElements();

			return "0";
		}

		private bool GetRelatedFiles(string fileName)
		{
			FileInfo selectedFile = new FileInfo(fileName);
		
			this.fileTable = new DataTable("FileTable");
			this.fileTable.Columns.Add("FileName", typeof(string));
			this.fileTable.Columns.Add("ZoomLevel", typeof(int));

			this.fileTable.Rows.Add(new object[] {selectedFile.FullName, 0});
			this.fileIndex = 0;

			// get the file name and split it up on '.' character
			string[] filenameArray = selectedFile.Name.Split('.');

			if (filenameArray.Length == 3)
			{
				string[] fileNames = Directory.GetFiles(selectedFile.Directory.FullName, filenameArray[0] + ".*.metadata");

				if (fileNames.Length > 1)
				{
					this.fileTable.Clear();
					int[] zoomLevels = new int[fileNames.Length];

					// figure out zoom levels and store them
					for (int i=0; i<fileNames.Length; i++)
					{
						filenameArray = fileNames[i].Split('.');
						try
						{
							zoomLevels[i] = int.Parse(filenameArray[filenameArray.Length-2]);
						}
						catch
						{
							MessageBox.Show(this.ParentForm, "One of the parameter file names was in an incorrect format.", "Incorrect Format", MessageBoxButtons.OK, MessageBoxIcon.Error);
							return false;
						}
					}

					Array.Sort(zoomLevels, fileNames);
					Array.Sort(zoomLevels);

					for (int i=0; i<fileNames.Length; i++)
					{
						if (fileNames[i] == selectedFile.FullName)
						{
							this.fileIndex = i;
							break;
						}
					}

					// create the file table
					for (int i=0; i<fileNames.Length; i++)
						if (File.Exists(fileNames[i].Replace(".metadata", ".binary")))
							this.fileTable.Rows.Add(new object[] {fileNames[i], zoomLevels[i]});

					// create the zoom levels for zoom menu
					foreach(DataRow row in this.fileTable.Rows)
						this.zoomLevelsMenuItem.MenuItems.Add("Level " + row["ZoomLevel"].ToString(), new EventHandler(ZoomLevelMenuItem_Clicked));
					this.ZoomMenuUpdate();

					return true;
				}
				else
					return true;
			}
			else
				return true;
		}

		private void SetupElements()
		{
			// make sure we don't show zoom menu if it is a 3D file, but enable box selection
			if (this.readerWriterBinary.Dimensions.Length == 3)
			{
				this.zoomMenuItem.Enabled = false;
				this.layout2DColorFull.Layout2DColor.ViewerAxisPanel.TopAxisLabel += ", 0.00 " + this.readerWriterBinary.Dimensions[2].Name;
			}
			else
			{
				this.layout2DColorFull.BoxEnabled = true;
			}

			// create a new data array of the correct size, fill it with data, and send it to the viewer panel
			if (this.readerWriterBinary.Dimensions.Length ==2)
				dataArray = new double[this.layout2DColorFull.Layout2DColor.ViewerPanel.ClientSize.Width, this.readerWriterBinary.Dimensions[0].Size];
			else
				dataArray = new double[this.readerWriterBinary.Dimensions[1].Size, this.readerWriterBinary.Dimensions[0].Size];

			// get initial data from the file, send it to the viewer
			this.readerWriterBinary.GetData(ref dataArray, 0);
			this.layout2DColorFull.View(dataArray);

			// if we don't have a 3D viewer, we need to respond to a resize event by making a bigger array (due to our scroll)
			if (this.readerWriterBinary.Dimensions.Length == 2)
			{
				this.layout2DColorFull.Layout2DColor.Resize -= new EventHandler(Layout2DColor_Resize);
				this.layout2DColorFull.Layout2DColor.Resize += new EventHandler(Layout2DColor_Resize);
			}

			// calculate step sizes for the different dimensions
			dimensionSteps = new double[this.readerWriterBinary.Dimensions.Length];
			for (int i=0;i<dimensionSteps.Length;i++)
				dimensionSteps[i] = (double)(Math.Abs(this.readerWriterBinary.Dimensions[i].End-this.readerWriterBinary.Dimensions[i].Start)/(double)this.readerWriterBinary.Dimensions[i].Size);

			// setup the viewer's axis to show correct values
			this.layout2DColorFull.Layout2DColor.ViewerAxisPanel.LeftAxisLabel = this.readerWriterBinary.Dimensions[0].Name;
			if (this.readerWriterBinary.Dimensions[0].ElementArray == null)
				this.layout2DColorFull.Layout2DColor.ViewerAxisPanel.LeftAxisMajorTickNumbersFormat = "0";
			else
				this.layout2DColorFull.Layout2DColor.ViewerAxisPanel.LeftAxisMajorTickNumbersFormat = "0.00e00";
			this.layout2DColorFull.Layout2DColor.ViewerAxisPanel.LeftAxisStartValue = (float)this.readerWriterBinary.Dimensions[0].Start;
			this.layout2DColorFull.Layout2DColor.ViewerAxisPanel.LeftAxisEndValue = (float)this.readerWriterBinary.Dimensions[0].End;
			this.layout2DColorFull.Layout2DColor.ViewerAxisPanel.LeftAxisValueArray = this.readerWriterBinary.Dimensions[0].ElementArray;
			this.layout2DColorFull.Layout2DColor.ViewerAxisPanel.BottomAxisLabel = this.readerWriterBinary.Dimensions[1].Name;
			this.layout2DColorFull.Layout2DColor.ViewerAxisPanel.BottomAxisMajorTickNumbersFormat = "0.00";	
			this.layout2DColorFull.Layout2DColor.ViewerAxisPanel.BottomAxisStartValue = (float)this.readerWriterBinary.Dimensions[1].Start;

			// we set up the bottom axis and scrollbar to have enough values
			if (this.readerWriterBinary.Dimensions.Length == 2)
			{
				this.layout2DColorFull.Layout2DColor.ViewerAxisPanel.BottomAxisEndValue = (float)(this.readerWriterBinary.Dimensions[1].Start+this.layout2DColorFull.Layout2DColor.ViewerPanel.ClientSize.Width*this.dimensionSteps[1]);
				if (this.readerWriterBinary.Dimensions[1].Size - 1 <= this.layout2DColorFull.Layout2DColor.ViewerPanel.ClientSize.Width)
				{
					this.extendedScrollBar.Value = 0;
					this.extendedScrollBar.Maximum = 1;
					this.extendedScrollBar.Enabled = false;
				}
				else
				{
					this.extendedScrollBar.Enabled = true;
					this.extendedScrollBar.Maximum = (int)this.readerWriterBinary.Dimensions[1].Size - 1 - this.layout2DColorFull.Layout2DColor.ViewerPanel.ClientSize.Width;
				}
				
				//this.extendedScrollBar.Maximum = (int)this.readerWriterBinary.Dimensions[1].Size - 1 - this.layout2DColorFull.Layout2DColor.ViewerPanel.ClientSize.Width;
			}
			else
			{
				this.extendedScrollBar.TimerAmount = 1;
				this.extendedScrollBar.SmallChange = 1;
				this.extendedScrollBar.LargeChange = 10;
				this.layout2DColorFull.Layout2DColor.ViewerAxisPanel.BottomAxisEndValue = (float)this.readerWriterBinary.Dimensions[1].End;
				this.layout2DColorFull.Layout2DColor.ViewerAxisPanel.BottomAxisValueArray = this.readerWriterBinary.Dimensions[1].ElementArray;
				this.extendedScrollBar.Maximum = (int)this.readerWriterBinary.Dimensions[2].Size;
			}

			// setup the colorbar to have the correct values, and set up its axis
			this.layout2DColorFull.Layout2DColor.ColorbarPanel.SetMinMax(this.readerWriterBinary.Min, this.readerWriterBinary.Max, true);
			this.layout2DColorFull.Layout2DColor.ColorbarAxisPanel.RightAxisLabel = this.readerWriterBinary.Units;
			this.layout2DColorFull.Layout2DColor.ColorbarAxisPanel.RightAxisStartValue = (float)this.readerWriterBinary.Min;
			this.layout2DColorFull.Layout2DColor.ColorbarAxisPanel.RightAxisEndValue = (float)this.readerWriterBinary.Max;
			this.layout2DColorFull.Layout2DColor.ColorbarAxisPanel.Invalidate();
		}

		private void RefreshViewer(int scrollPosition)
		{
            if (!this.backgroundWorker.IsBusy)
                this.backgroundWorker.RunWorkerAsync(scrollPosition);
        }
        
        void backgroundWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            int scrollPosition = (int)e.Argument;

			// get new data and send it to the viewer panel
			this.readerWriterBinary.GetData(ref dataArray, scrollPosition);
			this.layout2DColorFull.View(dataArray);

            e.Result = scrollPosition;
        }

        void backgroundWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
           int scrollPosition = (int)e.Result;

            // we set up the scrollbar to have enough values
            if (this.readerWriterBinary.Dimensions.Length == 2)
            {
                // set new bottom axis values and refresh the axis
                this.layout2DColorFull.Layout2DColor.ViewerAxisPanel.BottomAxisStartValue = (float)(this.readerWriterBinary.Dimensions[1].Start + scrollPosition * this.dimensionSteps[1]);
                this.layout2DColorFull.Layout2DColor.ViewerAxisPanel.BottomAxisEndValue = (float)(this.readerWriterBinary.Dimensions[1].Start + (scrollPosition + this.layout2DColorFull.Layout2DColor.ViewerPanel.ClientSize.Width) * this.dimensionSteps[1]);
                if (this.readerWriterBinary.Dimensions[1].Size - 1 <= this.layout2DColorFull.Layout2DColor.ViewerPanel.ClientSize.Width)
                {
                    this.extendedScrollBar.Value = 0;
                    this.extendedScrollBar.Maximum = 1;
                    this.extendedScrollBar.Enabled = false;
                }
                else
                {
                    this.extendedScrollBar.Enabled = true;
                    this.extendedScrollBar.Maximum = (int)this.readerWriterBinary.Dimensions[1].Size - 1 - this.layout2DColorFull.Layout2DColor.ViewerPanel.ClientSize.Width;
                }
            }
            else
                this.layout2DColorFull.Layout2DColor.ViewerAxisPanel.TopAxisLabel = this.layout2DColorFull.Layout2DColor.ViewerAxisPanel.TopAxisLabel.Split(',')[0] + ", " + (this.readerWriterBinary.Dimensions[2].Start + this.dimensionSteps[2] * scrollPosition).ToString("0.00") + " " + this.readerWriterBinary.Dimensions[2].Name;

            // we need to redraw the axis so that the bottom values change as needed
            this.layout2DColorFull.Layout2DColor.ViewerAxisPanel.Invalidate();
		}
		#endregion

		#region Scrollbar and Resize Events
		private void extendedScrollBar_Scroll(object sender, System.Windows.Forms.ScrollEventArgs e)
		{
			this.RefreshViewer(e.NewValue);
		}

		private void Layout2DColor_Resize(object sender, EventArgs e)
		{
			if (this.layout2DColorFull.Layout2DColor.ViewerPanel.ClientSize.Width > 0)
			{
				this.dataArray = new double[this.layout2DColorFull.Layout2DColor.ViewerPanel.ClientSize.Width, this.readerWriterBinary.Dimensions[0].Size];
				this.RefreshViewer(this.extendedScrollBar.Value);
			}
		}
		#endregion

		#region Zoom Code and Events
		private void ZoomMenuUpdate()
		{
			// check the zoom level menu items as needed
			foreach (MenuItem item in zoomLevelsMenuItem.MenuItems)
				item.Checked = false;
			this.zoomLevelsMenuItem.MenuItems[this.fileIndex].Checked = true;

			// enable or disable the zoom in and zoom out menu items as needed
			this.zoomInMenuItem.Enabled = this.zoomOutMenuItem.Enabled = true;
			if (this.zoomLevelsMenuItem.MenuItems[0].Checked)
				this.zoomInMenuItem.Enabled = false;
			if (this.zoomLevelsMenuItem.MenuItems[this.zoomLevelsMenuItem.MenuItems.Count-1].Checked)
				this.zoomOutMenuItem.Enabled = false;
		}

        private void ZoomSet(int zoomIndex)
        {
            this.fileIndex = zoomIndex;

            bool autoScrolling = this.extendedScrollBar.AutoScrolling;
            if (autoScrolling)
                this.extendedScrollBar.AutoScrolling = false;

            this.backgroundWorker.CancelAsync();
            System.Threading.Thread.Sleep(10);
            Application.DoEvents();
                
            this.ZoomFunction();
            if (autoScrolling)
                this.extendedScrollBar.AutoScrolling = true;
        }

        private void ZoomFunction()
        {
			int oldScroll = this.extendedScrollBar.Value;
			int oldMax = this.extendedScrollBar.Maximum;
			
			this.ZoomMenuUpdate();

			// try to use downsampled file, and if there is a problem, fail
			string returnValue = this.SetupFile((string)this.fileTable.Rows[this.fileIndex]["FileName"]);
			if (returnValue != "0" && this.CriticalError != null)
			{
				this.CriticalError(returnValue);
				return;
			}

			this.extendedScrollBar.Value = Math.Max(0, Math.Min(this.extendedScrollBar.Maximum, (int)(oldScroll/(float)oldMax*this.extendedScrollBar.Maximum)));
			this.RefreshViewer(this.extendedScrollBar.Value);
		}

		private void zoomInMenuItem_Click(object sender, System.EventArgs e)
		{
			this.ZoomSet(this.fileIndex-1);
		}

		private void zoomOutMenuItem_Click(object sender, System.EventArgs e)
		{
			this.ZoomSet(this.fileIndex+1);
		}

		private void ZoomLevelMenuItem_Clicked(object sender, System.EventArgs e)
		{
			if (!((MenuItem)sender).Checked)
				this.ZoomSet(this.zoomLevelsMenuItem.MenuItems.IndexOf((MenuItem)sender));
		}

		// event and delegate for scroll (see above for use)
		public event CriticalErrorHandler CriticalError;
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

		#region Tool Tip Code
		private void ViewerPanel_MouseMove(object sender, MouseEventArgs e)
		{
			ToolTip toolTip = this.layout2DColorFull.Layout2DColor.ViewerPanel.ToolTip;
			Point mousePoint = new Point(e.X, e.Y);
			String tipString;

			if (e.Button == MouseButtons.None && this.toolTipShow)
			{
				EarLab.ReaderWriters.ReaderWriterBinary.Dimension[] dimensions = this.readerWriterBinary.Dimensions;
				int xIndex, yIndex;
				double xValue, yValue, zValue=0.0, dValue;

				dValue = this.layout2DColorFull.Layout2DColor.PointData(mousePoint, out xIndex, out yIndex);

				if (this.readerWriterBinary.Dimensions[0].ElementArray != null)
					yValue = this.readerWriterBinary.Dimensions[0].ElementArray[yIndex];
				else
					yValue = this.readerWriterBinary.Dimensions[0].Start + this.dimensionSteps[0]*yIndex;

				if (this.readerWriterBinary.Dimensions.Length == 2)
					xValue = this.readerWriterBinary.Dimensions[1].Start + this.dimensionSteps[1]*(xIndex+this.extendedScrollBar.Value);
				else
				{
					if (this.readerWriterBinary.Dimensions[1].ElementArray != null)
						xValue = this.readerWriterBinary.Dimensions[1].ElementArray[xIndex];
					else
						xValue = this.readerWriterBinary.Dimensions[1].Start + this.dimensionSteps[1]*xIndex;
					zValue = this.readerWriterBinary.Dimensions[2].Start + this.dimensionSteps[2]*this.extendedScrollBar.Value;
				}

				tipString = "   Location: " + xValue.ToString(this.layout2DColorFull.Layout2DColor.ViewerAxisPanel.BottomAxisMajorTickNumbersFormat) + ", ";
				tipString += yValue.ToString(this.layout2DColorFull.Layout2DColor.ViewerAxisPanel.LeftAxisMajorTickNumbersFormat);
				if (this.readerWriterBinary.Dimensions.Length == 3)
					tipString += ", " + zValue.ToString("0.00");
				tipString += "\nData Value: " + dValue.ToString("0.00e00");

				toolTip.SetToolTip((Control)this.layout2DColorFull.Layout2DColor.ViewerPanel, tipString);
				toolTip.Active = true;
			}
			else
			{
				// hide the tool tip (it might have been showing
				toolTip.Active = false;
			}
		}

		private void tipMenuItem_Click(object sender, System.EventArgs e)
		{
			if (this.tipMenuItem.Checked)
				this.tipMenuItem.Checked = this.toolTipShow = false;
			else
				this.tipMenuItem.Checked = this.toolTipShow = true;
		}
		#endregion

		#region Save Image Code
		private void saveMenuItem_Click(object sender, System.EventArgs e)
		{
			if (this.saveFileDialog.ShowDialog() == DialogResult.OK)
			{
				Application.DoEvents();

				Graphics layoutGraphics = this.layout2DColorFull.CreateGraphics();
				Bitmap layoutBitmap = new Bitmap(this.layout2DColorFull.ClientSize.Width, this.layout2DColorFull.ClientSize.Height, layoutGraphics);
				Graphics bitmapGraphics = Graphics.FromImage(layoutBitmap);
				IntPtr layoutDeviceContext = layoutGraphics.GetHdc();
				IntPtr bitmapDeviceContext = bitmapGraphics.GetHdc();
				NativeMethods.BitBlt(bitmapDeviceContext, 0, 0, this.layout2DColorFull.ClientSize.Width, this.layout2DColorFull.ClientSize.Height, layoutDeviceContext, 0, 0, 13369376);
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
	}
}
