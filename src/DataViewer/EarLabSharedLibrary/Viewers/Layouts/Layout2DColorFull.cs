using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;
using EarLab.Analysis;

namespace EarLab.Viewers.Layouts
{
	/// <summary>
	/// Summary description for Layout2DColorFull.
	/// </summary>
	public class Layout2DColorFull : System.Windows.Forms.UserControl
	{
		private EarLab.Viewers.Layouts.Layout2DColor layout2DColor;
		private System.Windows.Forms.Splitter splitterVertical;
		private System.Windows.Forms.Splitter splitterHorizontal;
		private EarLab.Viewers.Layouts.LayoutWaveform waveformX;
		private EarLab.Viewers.Layouts.LayoutWaveform waveformY;
		/// <summary> 
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		private MenuItem analysisMenuItem;
		private bool crosshairEnabled = true;
		private bool boxEnabled = false;
		private bool waveformsActive = false;

		private Point clickStart = Point.Empty;
		private Point clickEnd = Point.Empty;
		private Point tempStart = Point.Empty;
		private Point tempEnd = Point.Empty;

		public Layout2DColorFull()
		{
			// This call is required by the Windows.Forms Form Designer.
			InitializeComponent();

			// we size up the x waveform panel to fit the viewer panel nicely
			this.waveformX.WaveformPanel.Anchor = AnchorStyles.None;
			this.waveformX.WaveformPanel.Location = new Point(this.layout2DColor.ViewerPanel.Left, 8);
			this.waveformX.WaveformPanel.Width = this.layout2DColor.ViewerPanel.Width;
			this.waveformX.WaveformPanel.Height = this.waveformX.Height - 16;
			this.waveformX.WaveformPanel.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Bottom | AnchorStyles.Right;
			this.waveformX.NormalOrientation = true;

			// we size up the y waveform panel to fit the viewer panel nicely
			this.waveformY.WaveformPanel.Anchor = AnchorStyles.None;
			this.waveformY.WaveformPanel.Location = new Point(8, this.layout2DColor.ViewerPanel.Top+this.layout2DColor.Top);
			this.waveformY.WaveformPanel.Width = this.waveformY.Width - 16;
			this.waveformY.WaveformPanel.Height = this.layout2DColor.ViewerPanel.Height;
			this.waveformY.WaveformPanel.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Bottom | AnchorStyles.Right;
			this.waveformY.NormalOrientation = false;

			// attach to viewer panel mouse events for crosshairs and box
			this.layout2DColor.ViewerPanel.MouseDown += new MouseEventHandler(ViewerPanel_MouseDown);
			this.layout2DColor.ViewerPanel.MouseMove += new MouseEventHandler(ViewerPanel_MouseMove);
			this.layout2DColor.ViewerPanel.MouseUp += new MouseEventHandler(ViewerPanel_MouseUp);
			this.layout2DColor.ViewerPanel.Resize += new EventHandler(ViewerPanel_Resize);

			// setup the analysis menu
			analysisMenuItem = new MenuItem("&Analysis");
			string[] menus = AnalysisTools.GetAnalysisMenus();
			foreach(string menu in menus)
				analysisMenuItem.MenuItems.Add(menu, new System.EventHandler(AnalysisMenuItem_Click));
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
			this.waveformX = new EarLab.Viewers.Layouts.LayoutWaveform();
			this.waveformY = new EarLab.Viewers.Layouts.LayoutWaveform();
			this.layout2DColor = new EarLab.Viewers.Layouts.Layout2DColor();
			this.SuspendLayout();
			// 
			// splitterVertical
			// 
			this.splitterVertical.Location = new System.Drawing.Point(72, 0);
			this.splitterVertical.MinExtra = 550;
			this.splitterVertical.MinSize = 72;
			this.splitterVertical.Name = "splitterVertical";
			this.splitterVertical.Size = new System.Drawing.Size(3, 352);
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
			this.waveformY.Size = new System.Drawing.Size(72, 352);
			this.waveformY.TabIndex = 6;
			// 
			// layout2DColor
			// 
			this.layout2DColor.BackColor = System.Drawing.Color.White;
			this.layout2DColor.Dock = System.Windows.Forms.DockStyle.Fill;
			this.layout2DColor.Location = new System.Drawing.Point(75, 75);
			this.layout2DColor.Name = "layout2DColor";
			this.layout2DColor.Size = new System.Drawing.Size(589, 277);
			this.layout2DColor.TabIndex = 7;
			// 
			// Layout2DColorFull
			// 
			this.Controls.Add(this.layout2DColor);
			this.Controls.Add(this.splitterHorizontal);
			this.Controls.Add(this.waveformX);
			this.Controls.Add(this.splitterVertical);
			this.Controls.Add(this.waveformY);
			this.Name = "Layout2DColorFull";
			this.Size = new System.Drawing.Size(664, 352);
			this.ResumeLayout(false);

		}
		#endregion

		#region Methods
		public void View(double[,] dataArray)
		{
			this.layout2DColor.View(dataArray);

			if (this.waveformsActive)
				this.WaveformsShow();
		}

		private void WaveformsShow()
		{
			double[][] waveformData = this.layout2DColor.CrosshairData(this.clickStart);
			this.waveformsActive = true;
			this.waveformX.DataSource = waveformData[0];
			this.waveformY.DataSource = waveformData[1];
		}

		private void BoxShow()
		{
			//boxArray = this.layout2DColor.BoxData(this.tempStart, this.tempEnd);
			this.analysisMenuItem.Enabled = true;
		}

		public void CrosshairHide()
		{
			if (this.crosshairEnabled)
			{
				this.layout2DColor.Crosshair = Point.Empty;
				this.waveformsActive = false;
				this.waveformX.DataSource = null;
				this.waveformY.DataSource = null;
			}
		}

		public void BoxHide()
		{
			if (this.boxEnabled)
				this.layout2DColor.Box = Rectangle.Empty;

			if (this.analysisMenuItem.Enabled)
				this.analysisMenuItem.Enabled = false;
		}
		#endregion

		#region Mouse Events
		private void ViewerPanel_MouseDown(object sender, MouseEventArgs e)
		{
			if (e.Button == MouseButtons.Left && e.Clicks == 1 && (this.boxEnabled || this.crosshairEnabled))
			{
				// clear out any old crosshair or box that we may have
				this.CrosshairHide();
				this.BoxHide();

				// store the new click point
				this.clickStart = new Point(e.X, e.Y);
			}
		}

		private void ViewerPanel_MouseMove(object sender, MouseEventArgs e)
		{
			if (e.Button == MouseButtons.Left && (this.boxEnabled || this.crosshairEnabled))
			{
				// store the new click point
				this.clickEnd = new Point(e.X, e.Y);

				// figure out which is upper left point
				this.tempStart = new Point(Math.Min(this.clickStart.X, this.clickEnd.X), Math.Min(this.clickStart.Y, this.clickEnd.Y));
				this.tempEnd = new Point(Math.Max(this.clickStart.X, this.clickEnd.X), Math.Max(this.clickStart.Y, this.clickEnd.Y));
				
				// fix point within viewer panel size
				this.tempStart = new Point(Math.Max(tempStart.X, 0), Math.Max(tempStart.Y, 0));
				this.tempEnd = new Point(Math.Min(tempEnd.X, this.layout2DColor.ViewerPanel.ClientRectangle.Right-1), Math.Min(tempEnd.Y, this.layout2DColor.ViewerPanel.ClientRectangle.Bottom-1));

				// tell viewer to draw box if box drawing is enabled
				if (this.boxEnabled)
					this.layout2DColor.Box = new Rectangle(this.tempStart, new Size(this.tempEnd.X-this.tempStart.X, this.tempEnd.Y-this.tempStart.Y));
			}
		}

		private void ViewerPanel_MouseUp(object sender, MouseEventArgs e)
		{
			if (e.Button == MouseButtons.Left && e.Clicks == 1 && (this.boxEnabled || this.crosshairEnabled))
			{
				if (Math.Abs(this.clickStart.X - e.X) < 2 && Math.Abs(this.clickStart.Y - e.Y) < 2)
				{
					// tell viewer to draw crosshair if drawing is enabled
					if (this.crosshairEnabled)
					{
						this.layout2DColor.Crosshair = this.clickStart;
						this.WaveformsShow();
					}
				}
				else
				{
					// tell viewer to draw box if box drawing is enabled
					if (this.boxEnabled)
					{
						this.layout2DColor.Box = new Rectangle(this.tempStart, new Size(this.tempEnd.X-this.tempStart.X, this.tempEnd.Y-this.tempStart.Y));
						this.BoxShow();
					}
				}
			}
			else if (e.Button == MouseButtons.Right && e.Clicks == 1)
			{
				this.CrosshairHide();
				this.BoxHide();
			}
		}
		#endregion

		#region Splitter and Panel Resize Events
		private void splitterHorizontal_SplitterMoved(object sender, System.Windows.Forms.SplitterEventArgs e)
		{
			// we size up the y waveform panel to fit the viewer panel nicely
			this.waveformY.WaveformPanel.Anchor = AnchorStyles.None;
			this.waveformY.WaveformPanel.Location = new Point(8, this.layout2DColor.ViewerPanel.Top+this.layout2DColor.Top);
			this.waveformY.WaveformPanel.Width = this.waveformY.Width - 16;
			this.waveformY.WaveformPanel.Height = this.layout2DColor.ViewerPanel.Height;
			this.waveformY.WaveformPanel.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Bottom | AnchorStyles.Right;
		}

		private void ViewerPanel_Resize(object sender, EventArgs e)
		{
			if (this.clickStart.X > this.layout2DColor.ViewerPanel.ClientSize.Width-1 || this.clickStart.Y > this.layout2DColor.ViewerPanel.ClientSize.Height-1)
			{
				this.CrosshairHide();
				this.BoxHide();
			}
			else if (this.waveformsActive)
				this.WaveformsShow();
		}
		#endregion

		private void AnalysisMenuItem_Click(object sender, System.EventArgs e)
		{
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
				double sampleRate = 1 / ((Math.Abs(this.layout2DColor.ViewerAxisPanel.BottomAxisEndValue-this.layout2DColor.ViewerAxisPanel.BottomAxisStartValue)/(double)this.layout2DColor.ArraySize.Width) / 1000);

				double[,] analysisArray = EarLab.Analysis.AnalysisTools.Analyze((int)analysisType, this.layout2DColor.BoxData(this.tempStart, this.tempEnd), sampleRate, analysisQuestionDialog.Value, out axisArray[4], out axisArray[5], out axisNames[3], out axisArray[2], out axisArray[3], out axisNames[2], out axisArray[0], out axisArray[1], out axisNames[1], out axisNames[0]);

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

		public bool CrosshairEnabled
		{
			set { this.crosshairEnabled = value; }
		}

		public EarLab.Viewers.Layouts.Layout2DColor Layout2DColor
		{
			get { return this.layout2DColor; }
		}

		public EarLab.Viewers.Layouts.LayoutWaveform WaveformX
		{
			get { return this.waveformX; }
		}

		public EarLab.Viewers.Layouts.LayoutWaveform WaveformY
		{
			get { return this.waveformY; }
		}
		#endregion
	}
}
