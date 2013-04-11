using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;
using EarLab.Analysis;
using EarLab.ReaderWriters;

namespace EarLab.Viewers.Layouts
{
	/// <summary>
	/// Summary description for LayoutScatterFull.
	/// </summary>
	public class LayoutScatterFull : System.Windows.Forms.UserControl
	{
		/// <summary> 
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private EarLab.Viewers.Layouts.LayoutScatter layoutScatter;
		private System.Windows.Forms.Splitter splitterVertical;
		private System.Windows.Forms.Splitter splitterHorizontal;
		private EarLab.Viewers.Layouts.LayoutBarWaveform waveformX;
		private EarLab.Viewers.Layouts.LayoutBarWaveform waveformY;
		private System.Windows.Forms.MenuItem analysisMenuItem;
		private System.Windows.Forms.MenuItem postMenuItem;
		private System.Windows.Forms.MenuItem periodMenuItem;
		private System.Windows.Forms.MenuItem intervalMenuItem;
		private System.Windows.Forms.ContextMenu analysisContextMenu;

		private bool boxEnabled = true;
		private Point clickStart = Point.Empty;
		private Point clickEnd = Point.Empty;
		private Point tempStart = Point.Empty;
        private MenuItem colorMenuItem;
		private Point tempEnd = Point.Empty;

		public LayoutScatterFull()
		{
			// This call is required by the Windows.Forms Form Designer.
			InitializeComponent();

			// we size up the x waveform panel to fit the viewer panel nicely
			this.waveformX.WaveformPanel.Anchor = AnchorStyles.None;
			this.waveformX.WaveformPanel.Location = new Point(this.layoutScatter.ViewerPanel.Left, 8);
			this.waveformX.WaveformPanel.Width = this.layoutScatter.ViewerPanel.Width;
			this.waveformX.WaveformPanel.Height = this.waveformX.Height - 16;
			this.waveformX.WaveformPanel.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Bottom | AnchorStyles.Right;
			this.waveformX.NormalOrientation = true;

			// we size up the y waveform panel to fit the viewer panel nicely
			this.waveformY.WaveformPanel.Anchor = AnchorStyles.None;
			this.waveformY.WaveformPanel.Location = new Point(8, this.layoutScatter.ViewerPanel.Top+this.layoutScatter.Top);
			this.waveformY.WaveformPanel.Width = this.waveformY.Width - 16;
			this.waveformY.WaveformPanel.Height = this.layoutScatter.ViewerPanel.Height;
			this.waveformY.WaveformPanel.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Bottom | AnchorStyles.Right;
			this.waveformY.NormalOrientation = false;

			// attach to viewer panel mouse events for crosshairs and box
			this.layoutScatter.ViewerPanel.MouseDown += new MouseEventHandler(ViewerPanel_MouseDown);
			this.layoutScatter.ViewerPanel.MouseMove += new MouseEventHandler(ViewerPanel_MouseMove);
			this.layoutScatter.ViewerPanel.MouseUp += new MouseEventHandler(ViewerPanel_MouseUp);
			this.layoutScatter.ViewerPanel.Resize += new EventHandler(ViewerPanel_Resize);

			this.analysisMenuItem.Enabled = false;
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
            this.splitterVertical = new System.Windows.Forms.Splitter();
            this.splitterHorizontal = new System.Windows.Forms.Splitter();
            this.analysisContextMenu = new System.Windows.Forms.ContextMenu();
            this.analysisMenuItem = new System.Windows.Forms.MenuItem();
            this.postMenuItem = new System.Windows.Forms.MenuItem();
            this.periodMenuItem = new System.Windows.Forms.MenuItem();
            this.intervalMenuItem = new System.Windows.Forms.MenuItem();
            this.layoutScatter = new EarLab.Viewers.Layouts.LayoutScatter();
            this.waveformX = new EarLab.Viewers.Layouts.LayoutBarWaveform();
            this.waveformY = new EarLab.Viewers.Layouts.LayoutBarWaveform();
            this.colorMenuItem = new System.Windows.Forms.MenuItem();
            this.SuspendLayout();
            // 
            // splitterVertical
            // 
            this.splitterVertical.Location = new System.Drawing.Point(72, 0);
            this.splitterVertical.MinExtra = 550;
            this.splitterVertical.MinSize = 72;
            this.splitterVertical.Name = "splitterVertical";
            this.splitterVertical.Size = new System.Drawing.Size(3, 331);
            this.splitterVertical.TabIndex = 1;
            this.splitterVertical.TabStop = false;
            // 
            // splitterHorizontal
            // 
            this.splitterHorizontal.Dock = System.Windows.Forms.DockStyle.Top;
            this.splitterHorizontal.Location = new System.Drawing.Point(75, 72);
            this.splitterHorizontal.MinExtra = 275;
            this.splitterHorizontal.MinSize = 72;
            this.splitterHorizontal.Name = "splitterHorizontal";
            this.splitterHorizontal.Size = new System.Drawing.Size(589, 3);
            this.splitterHorizontal.TabIndex = 3;
            this.splitterHorizontal.TabStop = false;
            this.splitterHorizontal.SplitterMoved += new System.Windows.Forms.SplitterEventHandler(this.splitterHorizontal_SplitterMoved);
            // 
            // analysisContextMenu
            // 
            this.analysisContextMenu.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.analysisMenuItem,
            this.colorMenuItem});
            // 
            // analysisMenuItem
            // 
            this.analysisMenuItem.Index = 0;
            this.analysisMenuItem.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.postMenuItem,
            this.periodMenuItem,
            this.intervalMenuItem});
            this.analysisMenuItem.Text = "Analysis";
            // 
            // postMenuItem
            // 
            this.postMenuItem.Index = 0;
            this.postMenuItem.Text = "Post Stimulus...";
            this.postMenuItem.Click += new System.EventHandler(this.postMenuItem_Click);
            // 
            // periodMenuItem
            // 
            this.periodMenuItem.Index = 1;
            this.periodMenuItem.Text = "Period...";
            this.periodMenuItem.Click += new System.EventHandler(this.periodMenuItem_Click);
            // 
            // intervalMenuItem
            // 
            this.intervalMenuItem.Index = 2;
            this.intervalMenuItem.Text = "Interval...";
            this.intervalMenuItem.Click += new System.EventHandler(this.intervalMenuItem_Click);
            // 
            // layoutScatter
            // 
            this.layoutScatter.BackColor = System.Drawing.Color.White;
            this.layoutScatter.Dock = System.Windows.Forms.DockStyle.Fill;
            this.layoutScatter.Location = new System.Drawing.Point(75, 75);
            this.layoutScatter.Name = "layoutScatter";
            this.layoutScatter.Size = new System.Drawing.Size(589, 256);
            this.layoutScatter.TabIndex = 7;
            // 
            // waveformX
            // 
            this.waveformX.Dock = System.Windows.Forms.DockStyle.Top;
            this.waveformX.Location = new System.Drawing.Point(75, 0);
            this.waveformX.Name = "waveformX";
            this.waveformX.Size = new System.Drawing.Size(589, 72);
            this.waveformX.TabIndex = 5;
            // 
            // waveformY
            // 
            this.waveformY.Dock = System.Windows.Forms.DockStyle.Left;
            this.waveformY.Location = new System.Drawing.Point(0, 0);
            this.waveformY.Name = "waveformY";
            this.waveformY.Size = new System.Drawing.Size(72, 331);
            this.waveformY.TabIndex = 6;
            // 
            // colorMenuItem
            // 
            this.colorMenuItem.Index = 1;
            this.colorMenuItem.Text = "Plot Color...";
            // 
            // LayoutScatterFull
            // 
            this.Controls.Add(this.layoutScatter);
            this.Controls.Add(this.splitterHorizontal);
            this.Controls.Add(this.waveformX);
            this.Controls.Add(this.splitterVertical);
            this.Controls.Add(this.waveformY);
            this.Name = "LayoutScatterFull";
            this.Size = new System.Drawing.Size(664, 331);
            this.ResumeLayout(false);

		}
		#endregion

		#region Methods
		public void View(ReaderWriterBinarySpikes.SpikeItem[] spikesArray, double startTime, double endTime, double pixelTime, int cellCount)
		{
            this.waveformX.WaveformPanel.DrawColor = this.waveformY.WaveformPanel.DrawColor = Color.Black;

			this.layoutScatter.View(spikesArray, startTime, pixelTime, cellCount);
			this.WaveformsShow(spikesArray, startTime, endTime, cellCount);
		}

		public void SetupAxis(string leftAxisName, string leftAxisFormat, float leftAxisMin, float leftAxisMax, string bottomAxisName, string bottomAxisFormat)
		{
			this.layoutScatter.SetupAxis(leftAxisName, leftAxisFormat, leftAxisMin, leftAxisMax, bottomAxisName, bottomAxisFormat);
		}

		private void WaveformsShow(ReaderWriterBinarySpikes.SpikeItem[] spikesArray, double startTime, double endTime, int cellCount)
		{
			double[][] waveForms = EarLab.Analysis.SpikeAnalysis.BinSpikes(spikesArray, startTime, endTime, cellCount, this.layoutScatter.ViewerPanel.ClientSize.Height,  this.layoutScatter.ViewerPanel.ClientSize.Width);

			this.waveformX.DataSource = waveForms[1];
			this.waveformY.DataSource = waveForms[0];
		}

		private void BoxShow()
		{
			//boxArray = this.layoutScatter.BoxData(this.tempStart, this.tempEnd);
			this.analysisMenuItem.Enabled = true;
		}

		public void BoxHide()
		{
			if (this.boxEnabled)
				this.layoutScatter.Box = Rectangle.Empty;

			if (this.analysisMenuItem.Enabled)
				this.analysisMenuItem.Enabled = false;
		}
		#endregion

		#region Mouse Events
		private void ViewerPanel_MouseDown(object sender, MouseEventArgs e)
		{
			if (e.Button == MouseButtons.Left && e.Clicks == 1 && this.boxEnabled)
			{
				// clear out any box that we may have
				this.BoxHide();

				// store the new click point
				this.clickStart = new Point(e.X, e.Y);
			}
		}

		private void ViewerPanel_MouseMove(object sender, MouseEventArgs e)
		{
			if (e.Button == MouseButtons.Left && this.boxEnabled)
			{
				// store the new click point
				this.clickEnd = new Point(e.X, e.Y);

				// figure out which is upper left point
				this.tempStart = new Point(Math.Min(this.clickStart.X, this.clickEnd.X), Math.Min(this.clickStart.Y, this.clickEnd.Y));
				this.tempEnd = new Point(Math.Max(this.clickStart.X, this.clickEnd.X), Math.Max(this.clickStart.Y, this.clickEnd.Y));
				
				// fix point within viewer panel size
				this.tempStart = new Point(Math.Max(tempStart.X, 0), Math.Max(tempStart.Y, 0));
				this.tempEnd = new Point(Math.Min(tempEnd.X, this.layoutScatter.ViewerPanel.ClientRectangle.Right-1), Math.Min(tempEnd.Y, this.layoutScatter.ViewerPanel.ClientRectangle.Bottom-1));

				// tell viewer to draw box if box drawing is enabled
				if (this.boxEnabled)
					this.layoutScatter.Box = new Rectangle(this.tempStart, new Size(this.tempEnd.X-this.tempStart.X, this.tempEnd.Y-this.tempStart.Y));
			}
		}

		private void ViewerPanel_MouseUp(object sender, MouseEventArgs e)
		{
			if (e.Button == MouseButtons.Left && e.Clicks == 1 && this.boxEnabled)
			{
				if (Math.Abs(this.clickStart.X - e.X) > 2 && Math.Abs(this.clickStart.Y - e.Y) > 2)
				{
					// tell viewer to draw box if box drawing is enabled
					if (this.boxEnabled)
					{
						this.layoutScatter.Box = new Rectangle(this.tempStart, new Size(this.tempEnd.X-this.tempStart.X, this.tempEnd.Y-this.tempStart.Y));
						this.BoxShow();
					}
				}
			}
			else if (e.Button == MouseButtons.Right && e.Clicks == 1)
				this.BoxHide();
		}
		#endregion

		#region Splitter and Panel Resize Events
		private void splitterHorizontal_SplitterMoved(object sender, System.Windows.Forms.SplitterEventArgs e)
		{
			// we size up the y waveform panel to fit the viewer panel nicely
			this.waveformY.WaveformPanel.Anchor = AnchorStyles.None;
			this.waveformY.WaveformPanel.Location = new Point(8, this.layoutScatter.ViewerPanel.Top+this.layoutScatter.Top);
			this.waveformY.WaveformPanel.Width = this.waveformY.Width - 16;
			this.waveformY.WaveformPanel.Height = this.layoutScatter.ViewerPanel.Height;
			this.waveformY.WaveformPanel.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Bottom | AnchorStyles.Right;
		}

		private void ViewerPanel_Resize(object sender, EventArgs e)
		{
			if (this.clickStart.X > this.layoutScatter.ViewerPanel.ClientSize.Width-1 || this.clickStart.Y > this.layoutScatter.ViewerPanel.ClientSize.Height-1)
				this.BoxHide();

			//this.WaveformsShow();
		}
		#endregion

		private void AnalysisMenuItem_Click(object sender, System.EventArgs e)
		{
			/*
			AnalysisType analysisType = (AnalysisType)(((MenuItem)sender).Index);
			double defaultValue;

			string analysisQuestion = AnalysisTools.GetAnalysisMenuQuestion((int)analysisType, out defaultValue);
			EarLab.Dialogs.AnalysisQuestionDialog analysisQuestionDialog = new EarLab.Dialogs.AnalysisQuestionDialog();
			analysisQuestionDialog.Question = analysisQuestion;
			analysisQuestionDialog.Value = defaultValue;

			if (analysisQuestion == "" || analysisQuestionDialog.ShowDialog(this) == DialogResult.OK)
			{
				double[] axisArray = new double[6];
				string[] axisNames = new string[4];
				double sampleRate = 1 / ((Math.Abs(this.layoutScatter.AxisPanel.BottomAxisEndValue-this.layoutScatter.AxisPanel.BottomAxisStartValue)/(double)this.layoutScatter.ArraySize.Width) / 1000);

				double[,] analysisArray = EarLab.Analysis.AnalysisTools.Analyze((int)analysisType, this.layoutScatter.BoxData(this.tempStart, this.tempEnd), sampleRate, analysisQuestionDialog.Value, out axisArray[4], out axisArray[5], out axisNames[3], out axisArray[2], out axisArray[3], out axisNames[2], out axisArray[0], out axisArray[1], out axisNames[1], out axisNames[0]);

				EarLab.Dialogs.AnalysisDialogNew analysisDialog = new EarLab.Dialogs.AnalysisDialogNew(analysisArray, axisArray, axisNames);
				analysisDialog.ShowDialog(this);
			}
			*/
		}

		private void GetMinMax(double[,] analysisArray, out double min, out double max)
		{
			min = double.MaxValue;
			max = 0;

			for (int i=0; i<analysisArray.GetLength(0); i++)
			{
				for (int j=0; j<analysisArray.GetLength(1); j++)
				{
					if (analysisArray[i,j] > max)
						max = analysisArray[i,j];
					if (analysisArray[i,j] < min)
						min = analysisArray[i,j];
				}
			}
		}

		private void postMenuItem_Click(object sender, System.EventArgs e)
		{
			EarLab.Dialogs.Analysis.PostDialog postDialog = new EarLab.Dialogs.Analysis.PostDialog(this.layoutScatter.BoxData.timeEnd - this.layoutScatter.BoxData.timeStart);

			if (postDialog.ShowDialog(this) == DialogResult.OK)
			{
				LayoutScatter.BoxDataType boxData = this.layoutScatter.BoxData;
				double[,] analysisArray = Analysis.SpikeAnalysis.PostStimulusAnalysis(boxData.spikeArray, postDialog.BinWidth, boxData.timeStart, boxData.timeEnd, boxData.cellIDStart, boxData.cellIDEnd);
			
				double[] axisArray = new double[6];
				axisArray[0] = boxData.cellIDStart;
				axisArray[1] = boxData.cellIDEnd;
				axisArray[2] = boxData.timeStart;
				axisArray[3] = boxData.timeEnd;
				
				double min, max;
				this.GetMinMax(analysisArray, out min, out max);
				axisArray[4] = min;
				axisArray[5] = max;

				string[] axisNames = new string[4];
				axisNames[0] = "Post Stimulus Analysis";

				EarLab.Dialogs.AnalysisDialogNew analysisDialog = new EarLab.Dialogs.AnalysisDialogNew(analysisArray, axisArray, axisNames);
				analysisDialog.ShowDialog(this);
			}
		}

		private void periodMenuItem_Click(object sender, System.EventArgs e)
		{
			EarLab.Dialogs.Analysis.PeriodDialog periodDialog = new EarLab.Dialogs.Analysis.PeriodDialog();

			if (periodDialog.ShowDialog(this) == DialogResult.OK)
			{
				LayoutScatter.BoxDataType boxData = this.layoutScatter.BoxData;
				double[,] analysisArray = Analysis.SpikeAnalysis.PeriodAnalysis(boxData.spikeArray, periodDialog.Period, periodDialog.BinCount, boxData.timeStart, boxData.timeEnd, boxData.cellIDStart, boxData.cellIDEnd);
			
				double[] axisArray = new double[6];
				axisArray[0] = boxData.cellIDStart;
				axisArray[1] = boxData.cellIDEnd;
				axisArray[2] = 0.0;
				axisArray[3] = 1.0;
				
				double min, max;
				this.GetMinMax(analysisArray, out min, out max);
				axisArray[4] = min;
				axisArray[5] = max;

				string[] axisNames = new string[4];
				axisNames[0] = "Period Analysis";
				axisNames[1] = "Cell ID";
				axisNames[2] = "Phase (Periods)";
				axisNames[3] = "Bin count";

				EarLab.Dialogs.AnalysisDialogNew analysisDialog = new EarLab.Dialogs.AnalysisDialogNew(analysisArray, axisArray, axisNames);
				analysisDialog.ShowDialog(this);
			}
		}

		private void intervalMenuItem_Click(object sender, System.EventArgs e)
		{
			EarLab.Dialogs.Analysis.IntervalDialog intervalDialog = new EarLab.Dialogs.Analysis.IntervalDialog();

			if (intervalDialog.ShowDialog(this) == DialogResult.OK)
			{
				LayoutScatter.BoxDataType boxData = this.layoutScatter.BoxData;
				double[,] analysisArray = Analysis.SpikeAnalysis.IntervalAnalysis(boxData.spikeArray, intervalDialog.MaxInterval, intervalDialog.BinCount, boxData.timeStart, boxData.timeEnd, boxData.cellIDStart, boxData.cellIDEnd);
			
				double[] axisArray = new double[6];
				axisArray[0] = boxData.cellIDStart;
				axisArray[1] = boxData.cellIDEnd;
				axisArray[2] = 0.0;
				axisArray[3] = intervalDialog.MaxInterval;
				
				double min, max;
				this.GetMinMax(analysisArray, out min, out max);
				axisArray[4] = min;
				axisArray[5] = max;

				string[] axisNames = new string[4];
				axisNames[0] = "Interval Analysis";
				axisNames[1] = "Cell ID";
				axisNames[2] = "Interval (Sec)";
				axisNames[3] = "Bin count";

				EarLab.Dialogs.AnalysisDialogNew analysisDialog = new EarLab.Dialogs.AnalysisDialogNew(analysisArray, axisArray, axisNames);
				analysisDialog.ShowDialog(this);
			}
		}

		#region Properties
		public MenuItem AnalysisMenu
		{
			get { return this.analysisMenuItem; }
		}
		public bool BoxEnabled
		{
			set { this.boxEnabled = value; }
		}

		public EarLab.Viewers.Layouts.LayoutScatter LayoutScatter
		{
			get { return this.layoutScatter; }
		}

		public EarLab.Viewers.Layouts.LayoutBarWaveform WaveformX
		{
			get { return this.waveformX; }
		}

		public EarLab.Viewers.Layouts.LayoutBarWaveform WaveformY
		{
			get { return this.waveformY; }
		}

		public Splitter VerticalSplitter
		{
			get { return this.splitterVertical; }
		}
		#endregion
	}
}
