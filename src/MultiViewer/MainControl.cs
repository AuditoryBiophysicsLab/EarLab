using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;

namespace MultiViewer
{
	/// <summary>
	/// Summary description for MainControl.
	/// </summary>
	public class MainControl : System.Windows.Forms.UserControl
	{
		/// <summary> 
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		private EarLab.Controls.ExtendedScrollBar extendedScrollBar;
		private EarLab.Viewers.Layouts.ILayoutMulti ulLayout;
		private EarLab.Viewers.Layouts.ILayoutMulti urLayout;
		private EarLab.Viewers.Layouts.ILayoutMulti llLayout;
		private EarLab.Viewers.Layouts.ILayoutMulti lrLayout;

		string ulFile = "";
		string urFile = "";
		string llFile = "";
		string lrFile = "";

		double scrollStep = .0001;

		public MainControl()
		{
			// This call is required by the Windows.Forms Form Designer.
			InitializeComponent();

			this.extendedScrollBar.Minimum = 0;
			this.extendedScrollBar.Maximum = 1;
			this.extendedScrollBar.SmallChange = 1;
			this.extendedScrollBar.LargeChange = 10;
			this.extendedScrollBar.TimerInterval = 100;
			this.extendedScrollBar.TimerAmount = 10;

			this.extendedScrollBar.Enabled = false;

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
			this.SuspendLayout();
			// 
			// extendedScrollBar
			// 
			this.extendedScrollBar.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.extendedScrollBar.AutoScrolling = false;
			this.extendedScrollBar.LargeChange = 100;
			this.extendedScrollBar.Location = new System.Drawing.Point(8, 368);
			this.extendedScrollBar.Maximum = 100;
			this.extendedScrollBar.Minimum = 0;
			this.extendedScrollBar.Name = "extendedScrollBar";
			this.extendedScrollBar.Size = new System.Drawing.Size(840, 17);
			this.extendedScrollBar.SmallChange = 10;
			this.extendedScrollBar.TabIndex = 0;
			this.extendedScrollBar.TimerAmount = 10;
			this.extendedScrollBar.TimerInterval = 100;
			this.extendedScrollBar.Value = 0;
			this.extendedScrollBar.Scroll += new System.Windows.Forms.ScrollEventHandler(this.extendedScrollBar_Scroll);
			// 
			// MainControl
			// 
			this.Controls.Add(this.extendedScrollBar);
			this.Name = "MainControl";
			this.Size = new System.Drawing.Size(856, 392);
			this.Resize += new System.EventHandler(this.MainControl_Resize);
			this.ResumeLayout(false);

		}
		#endregion

		private void MainControl_Resize(object sender, System.EventArgs e)
		{
			this.SetupLayout();
			this.SetupScrollBar();
		}

		private void SetupLayout()
		{
			Control ulLayout = (Control)this.ulLayout;
			Control urLayout = (Control)this.urLayout;
			Control llLayout = (Control)this.llLayout;
			Control lrLayout = (Control)this.lrLayout;

			if (this.ulLayout != null)
			{
				if (this.urLayout != null)
				{
					if (this.llLayout != null || this.lrLayout != null)
					{
						ulLayout.Location = new Point(8,0);
						ulLayout.Width = this.ClientSize.Width/2 - 12;
						ulLayout.Height = this.ClientSize.Height/2 - 4 - 14;
					}
					else
					{
						ulLayout.Location = new Point(8,0);
						ulLayout.Width = this.ClientSize.Width/2 - 12;
						ulLayout.Height = this.ClientSize.Height - 24;
					}
				}
				else
				{
					if (this.llLayout != null || this.lrLayout != null)
					{
						ulLayout.Location = new Point(8,0);
						ulLayout.Width = this.ClientSize.Width - 16;
						ulLayout.Height = this.ClientSize.Height/2 - 4 - 14;
					}
					else
					{
						ulLayout.Location = new Point(8,0);
						ulLayout.Width = this.ClientSize.Width - 16;
						ulLayout.Height = this.ClientSize.Height - 24;
					}
				}

				if (this.ulLayout.ScrollStep != 0)
					this.scrollStep = this.ulLayout.ScrollStep;
				else
					this.ulLayout.ScrollStep = this.scrollStep*.001;
			}

			if (this.urLayout != null)
			{
				if (this.ulLayout != null)
				{
					if (this.llLayout != null || this.lrLayout != null)
					{
						urLayout.Location = new Point(this.ClientSize.Width / 2 + 4, 0);
						urLayout.Width = this.ClientSize.Width/2 - 12;
						urLayout.Height = this.ClientSize.Height/2 - 4 - 14;
					}
					else
					{
						urLayout.Location = new Point(this.ClientSize.Width / 2 + 4, 0);
						urLayout.Width = this.ClientSize.Width/2 - 12;
						urLayout.Height = this.ClientSize.Height - 24;
					}
				}
				else
				{
					if (this.llLayout != null || this.lrLayout != null)
					{
						urLayout.Location = new Point(8, 0);
						urLayout.Width = this.ClientSize.Width - 16;
						urLayout.Height = this.ClientSize.Height/2 - 4 - 14;
					}
					else
					{
						urLayout.Location = new Point(8, 0);
						urLayout.Width = this.ClientSize.Width - 16;
						urLayout.Height = this.ClientSize.Height - 24;
					}
				}

				if (this.urLayout.ScrollStep != 0)
					this.scrollStep = this.urLayout.ScrollStep;
				else
					this.urLayout.ScrollStep = this.scrollStep*.001;
			}

			if (this.llLayout != null)
			{
				if (this.lrLayout != null)
				{
					if (this.ulLayout != null || this.urLayout != null)
					{
						llLayout.Location = new Point(8, this.ClientSize.Height/2 + 4 - 14);
						llLayout.Width = this.ClientSize.Width/2 - 12;
						llLayout.Height = this.ClientSize.Height/2 - 4 - 14;
					}
					else
					{
						llLayout.Location = new Point(8, 0);
						llLayout.Width = this.ClientSize.Width/2 - 12;
						llLayout.Height = this.ClientSize.Height - 24;
					}
				}
				else
				{
					if (this.ulLayout != null || this.urLayout != null)
					{
						llLayout.Location = new Point(8, this.ClientSize.Height/2 + 4 - 14);
						llLayout.Width = this.ClientSize.Width - 16;
						llLayout.Height = this.ClientSize.Height/2 - 4 - 14;
					}
					else
					{
						llLayout.Location = new Point(8, 0);
						llLayout.Width = this.ClientSize.Width - 16;
						llLayout.Height = this.ClientSize.Height - 24;
					}
				}

				if (this.llLayout.ScrollStep != 0)
					this.scrollStep = this.llLayout.ScrollStep;
				else
					this.llLayout.ScrollStep = this.scrollStep*.001;
			}

			if (this.lrLayout != null)
			{
				if (this.llLayout != null)
				{
					if (this.ulLayout != null || this.urLayout != null)
					{
						lrLayout.Location = new Point(this.ClientSize.Width / 2 + 4, this.ClientSize.Height/2 + 4 - 14);
						lrLayout.Width = this.ClientSize.Width/2 - 12;
						lrLayout.Height = this.ClientSize.Height/2 - 4 - 14;
					}
					else
					{
						lrLayout.Location = new Point(this.ClientSize.Width / 2 + 4, 0);
						lrLayout.Width = this.ClientSize.Width/2 - 12;
						lrLayout.Height = this.ClientSize.Height - 24;
					}
				}
				else
				{
					if (this.ulLayout != null || this.urLayout != null)
					{
						lrLayout.Location = new Point(8, this.ClientSize.Height/2 + 4 - 14);
						lrLayout.Width = this.ClientSize.Width - 16;
						lrLayout.Height = this.ClientSize.Height/2 - 4 - 14;
					}
					else
					{
						lrLayout.Location = new Point(8, 0);
						lrLayout.Width = this.ClientSize.Width - 16;
						lrLayout.Height = this.ClientSize.Height - 24;
					}
				}

				if (this.lrLayout.ScrollStep != 0)
					this.scrollStep = this.lrLayout.ScrollStep;
				else
					this.lrLayout.ScrollStep = this.scrollStep*.001;
			}


			//if (this.ulLayout != null)
			//{
			//	ulLayout.Location = new Point(8,0);
			//	ulLayout.Width = this.ClientSize.Width/2 - 12;
			//	ulLayout.Height = this.ClientSize.Height/2 - 4 - 14;
			//}

			//if (this.urLayout != null)
			//{
			//	urLayout.Location = new Point(this.ClientSize.Width / 2 + 4, 0);
			//	urLayout.Width = this.ClientSize.Width/2 - 12;
			//	urLayout.Height = this.ClientSize.Height/2 - 4 - 14;
			//}

			//if (this.llLayout != null)
			//{
			//	llLayout.Location = new Point(8, this.ClientSize.Height/2 + 4 - 14);
			//	llLayout.Width = this.ClientSize.Width/2 - 12;
			//	llLayout.Height = this.ClientSize.Height/2 - 4 - 14;
			//}

			//if (this.lrLayout != null)
			//{
			//	lrLayout.Location = new Point(this.ClientSize.Width / 2 + 4, this.ClientSize.Height/2 + 4 - 14);
			//	lrLayout.Width = this.ClientSize.Width/2 - 12;
			//	lrLayout.Height = this.ClientSize.Height/2 - 4 - 14;
			//}
		}

		private void SetupScrollBar()
		{
			int oldValue = this.extendedScrollBar.Value;
			this.extendedScrollBar.Value = 0;
			this.extendedScrollBar.Maximum = 1;
			this.extendedScrollBar.Enabled = false;

			if (this.ulLayout != null && this.ulLayout.ScrollMax > this.extendedScrollBar.Maximum)
				this.extendedScrollBar.Maximum = this.ulLayout.ScrollMax;
			if (this.urLayout != null && this.urLayout.ScrollMax > this.extendedScrollBar.Maximum)
				this.extendedScrollBar.Maximum = this.urLayout.ScrollMax;
			if (this.llLayout != null && this.llLayout.ScrollMax > this.extendedScrollBar.Maximum)
				this.extendedScrollBar.Maximum = this.llLayout.ScrollMax;
			if (this.lrLayout != null && this.lrLayout.ScrollMax > this.extendedScrollBar.Maximum)
				this.extendedScrollBar.Maximum = this.lrLayout.ScrollMax;

			if (this.extendedScrollBar.Maximum > 1)
				this.extendedScrollBar.Enabled = true;
			if (oldValue <= this.extendedScrollBar.Maximum)
			{
				this.extendedScrollBar.Value = oldValue;
				this.extendedScrollBar_Scroll(this, new ScrollEventArgs(ScrollEventType.ThumbPosition, oldValue));
			}
			else
				this.extendedScrollBar_Scroll(this, new ScrollEventArgs(ScrollEventType.ThumbPosition, 0));
		}

		private void OpenFile(ref EarLab.Viewers.Layouts.ILayoutMulti layout, string fileName)
		{
			if (fileName == "")
			{
				this.Controls.Remove((Control)layout);
				layout = null;
				this.SetupLayout();
				this.SetupScrollBar();
				return;
			}

			if (layout != null)
			{
				this.Controls.Remove((Control)layout);
				layout = null;
			}

			if (fileName.EndsWith(".metadata"))
				layout = new EarLab.Viewers.Layouts.LayoutWaveformFullMulti();
			else
				layout = new EarLab.Viewers.Layouts.LayoutScatterFullMulti();

			this.Controls.Add((Control)layout);
			EarLab.Utilities.EnableVisualStyles.EnableControl((Control)layout);

			string returnString = layout.Read(fileName);
			this.SetupLayout();

			if (layout.ScrollMax > this.extendedScrollBar.Maximum)
				this.extendedScrollBar.Maximum = layout.ScrollMax;
			if (this.extendedScrollBar.Maximum > 1)
				this.extendedScrollBar.Enabled = true;
		}

		private void extendedScrollBar_Scroll(object sender, System.Windows.Forms.ScrollEventArgs e)
		{
			bool downsampleBool = false;
			if ((this.ulLayout != null && this.ulLayout.Type != 1) || (this.urLayout != null && this.urLayout.Type != 1)
				&& (this.llLayout != null && this.llLayout.Type != 1) || (this.lrLayout != null && this.lrLayout.Type != 1))
				downsampleBool = true;
		
			if (this.ulLayout != null)
			{
				if (this.ulLayout.Type == 1 && downsampleBool)
					this.ulLayout.Scroll = (int)Math.Floor(e.NewValue*.001);
				else
					this.ulLayout.Scroll = e.NewValue;
			}
			if (this.urLayout != null)
			{
				if (this.urLayout.Type == 1 && downsampleBool)
					this.urLayout.Scroll = (int)Math.Floor(e.NewValue*.001);
				else
					this.urLayout.Scroll = e.NewValue;
			}
			if (this.llLayout != null)
			{
				if (this.llLayout.Type == 1 && downsampleBool)
					this.llLayout.Scroll = (int)Math.Floor(e.NewValue*.001);
				else
					this.llLayout.Scroll = e.NewValue;
			}
			if (this.lrLayout != null)
			{
				if (this.lrLayout.Type == 1 && downsampleBool)
					this.lrLayout.Scroll = (int)Math.Floor(e.NewValue*.001);
				else
					this.lrLayout.Scroll = e.NewValue;
			}
		}
	
		public string ULFile
		{
			set
			{
				this.ulFile = value;
				this.OpenFile(ref this.ulLayout, value);
			}
			get { return this.ulFile; }
		}

		public string URFile
		{
			set
			{
				this.urFile = value;
				this.OpenFile(ref this.urLayout, value);
			}
			get { return this.urFile; }
		}

		public string LLFile
		{
			set
			{
				this.llFile = value;
				this.OpenFile(ref this.llLayout, value);
			}
			get { return this.llFile; }
		}

		public string LRFile
		{
			set
			{
				this.lrFile = value;
				this.OpenFile(ref this.lrLayout, value);
			}
			get { return this.lrFile; }
		}
	}
}
