using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.IO;
using EarLab.Viewers;
using EarLab.ReaderWriters;

namespace EarLab.DataViewer
{
	/// <summary>
	/// Summary description for MainForm.
	/// </summary>
	public class MainForm : System.Windows.Forms.Form
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		private System.Windows.Forms.MainMenu mainMenu;
		private System.Windows.Forms.MenuItem fileMenuItem;
		private System.Windows.Forms.MenuItem openMenuItem;
		private System.Windows.Forms.MenuItem seperatorMenuItem1;
		private System.Windows.Forms.MenuItem exitMenuItem;
		private System.Windows.Forms.OpenFileDialog openFileDialog;
		private System.Windows.Forms.MenuItem closeMenuItem;
		private System.Windows.Forms.MenuItem aboutMenuItem;
		private System.Windows.Forms.MenuItem dataviewerMenuItem;
		private System.Windows.Forms.MenuItem earlabMenuItem;
		private System.Windows.Forms.PictureBox pictureBox;
		private System.Windows.Forms.MenuItem helpMenuItem;
		private System.Windows.Forms.MenuItem helpIndexMenuItem;
		private System.Windows.Forms.MenuItem menuItem3;

		private EarLab.Viewers.Layouts.ILayout viewerControl;

		//private EarLab.Viewers.Layouts.LayoutFullScrollable viewerControl;

		public MainForm(string[] args)
		{
			// Required for Windows Form Designer support
			InitializeComponent();

			if (args.Length == 1 && args[0] != "")
				this.OpenFile(args[0]);
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(MainForm));
			this.mainMenu = new System.Windows.Forms.MainMenu();
			this.fileMenuItem = new System.Windows.Forms.MenuItem();
			this.openMenuItem = new System.Windows.Forms.MenuItem();
			this.closeMenuItem = new System.Windows.Forms.MenuItem();
			this.seperatorMenuItem1 = new System.Windows.Forms.MenuItem();
			this.exitMenuItem = new System.Windows.Forms.MenuItem();
			this.aboutMenuItem = new System.Windows.Forms.MenuItem();
			this.helpMenuItem = new System.Windows.Forms.MenuItem();
			this.helpIndexMenuItem = new System.Windows.Forms.MenuItem();
			this.menuItem3 = new System.Windows.Forms.MenuItem();
			this.dataviewerMenuItem = new System.Windows.Forms.MenuItem();
			this.earlabMenuItem = new System.Windows.Forms.MenuItem();
			this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
			this.pictureBox = new System.Windows.Forms.PictureBox();
			this.SuspendLayout();
			// 
			// mainMenu
			// 
			this.mainMenu.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					 this.fileMenuItem,
																					 this.aboutMenuItem});
			// 
			// fileMenuItem
			// 
			this.fileMenuItem.Index = 0;
			this.fileMenuItem.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																						 this.openMenuItem,
																						 this.closeMenuItem,
																						 this.seperatorMenuItem1,
																						 this.exitMenuItem});
			this.fileMenuItem.Text = "&File";
			// 
			// openMenuItem
			// 
			this.openMenuItem.Index = 0;
			this.openMenuItem.Text = "&Open...";
			this.openMenuItem.Click += new System.EventHandler(this.openMenuItem_Click);
			// 
			// closeMenuItem
			// 
			this.closeMenuItem.Enabled = false;
			this.closeMenuItem.Index = 1;
			this.closeMenuItem.Text = "&Close";
			this.closeMenuItem.Click += new System.EventHandler(this.closeMenuItem_Click);
			// 
			// seperatorMenuItem1
			// 
			this.seperatorMenuItem1.Index = 2;
			this.seperatorMenuItem1.Text = "-";
			// 
			// exitMenuItem
			// 
			this.exitMenuItem.Index = 3;
			this.exitMenuItem.Text = "E&xit";
			this.exitMenuItem.Click += new System.EventHandler(this.exitMenuItem_Click);
			// 
			// aboutMenuItem
			// 
			this.aboutMenuItem.Index = 1;
			this.aboutMenuItem.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																						  this.helpMenuItem,
																						  this.helpIndexMenuItem,
																						  this.menuItem3,
																						  this.dataviewerMenuItem,
																						  this.earlabMenuItem});
			this.aboutMenuItem.MergeOrder = 2;
			this.aboutMenuItem.ShowShortcut = false;
			this.aboutMenuItem.Text = "&Help";
			// 
			// helpMenuItem
			// 
			this.helpMenuItem.Index = 0;
			this.helpMenuItem.Text = "Show Help...";
			this.helpMenuItem.Click += new System.EventHandler(this.helpMenuItem_Click);
			// 
			// helpIndexMenuItem
			// 
			this.helpIndexMenuItem.Index = 1;
			this.helpIndexMenuItem.Text = "Show Help Index...";
			this.helpIndexMenuItem.Click += new System.EventHandler(this.helpIndexMenuItem_Click);
			// 
			// menuItem3
			// 
			this.menuItem3.Index = 2;
			this.menuItem3.Text = "-";
			// 
			// dataviewerMenuItem
			// 
			this.dataviewerMenuItem.Index = 3;
			this.dataviewerMenuItem.Text = "About DataViewer Application...";
			this.dataviewerMenuItem.Click += new System.EventHandler(this.dataviewerMenuItem_Click);
			// 
			// earlabMenuItem
			// 
			this.earlabMenuItem.Index = 4;
			this.earlabMenuItem.Text = "About EarLab at Boston University...";
			this.earlabMenuItem.Click += new System.EventHandler(this.earlabMenuItem_Click);
			// 
			// openFileDialog
			// 
			this.openFileDialog.Filter = "EarLab Files|*.index;*.metadata|EarLab Metadata XML File (*.metadata)|*.metadata|" +
				"EarLab XML Spike Metadata File (*.index)|*.index";
			this.openFileDialog.Title = "File Open...";
			// 
			// pictureBox
			// 
			this.pictureBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.pictureBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.pictureBox.Image = ((System.Drawing.Image)(resources.GetObject("pictureBox.Image")));
			this.pictureBox.Location = new System.Drawing.Point(0, 0);
			this.pictureBox.Name = "pictureBox";
			this.pictureBox.Size = new System.Drawing.Size(712, 401);
			this.pictureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
			this.pictureBox.TabIndex = 0;
			this.pictureBox.TabStop = false;
			// 
			// MainForm
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(712, 401);
			this.Controls.Add(this.pictureBox);
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Menu = this.mainMenu;
			this.MinimumSize = new System.Drawing.Size(600, 400);
			this.Name = "MainForm";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "EarLab DataViewer";
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main(string[] args) 
		{
			EarLab.Utilities.EnableVisualStyles.EnableApplication();
			Application.Run(new MainForm(args));
		}

		private void OpenFile(string fileName)
		{
			// let the screen update, so it isn't garbled
			Application.DoEvents();

			if (this.viewerControl != null)
			{
				this.viewerControl.Close();
				this.Controls.Remove((Control)this.viewerControl);
				((Control)this.viewerControl).Dispose();
				this.pictureBox.Visible = true;

				// we need this to allow the computer some time to display the picture box
				Application.DoEvents();
			}

			if (fileName.EndsWith(".metadata"))
				viewerControl = new EarLab.Viewers.Layouts.LayoutFullScrollable();
			else
				viewerControl = new EarLab.Viewers.Layouts.LayoutScatterFullScrollable();

			viewerControl.CriticalError += new EarLab.Viewers.Layouts.CriticalErrorHandler(viewerControl_CriticalError);
			this.Controls.Add((Control)viewerControl);
			((Control)viewerControl).Location = new Point(2,2);
			((Control)viewerControl).Size = new Size(this.ClientRectangle.Width-4, this.ClientRectangle.Height-4);
			((Control)viewerControl).Anchor = (AnchorStyles.Top | AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right);

			string returnString = viewerControl.Read(fileName);
			if (returnString == "0")
			{
				viewerControl.MainMenu = this.mainMenu;

				this.closeMenuItem.Enabled = true;
				this.pictureBox.Visible = false;
			}
			else
			{
				MessageBox.Show(this, returnString, "File Open Error", MessageBoxButtons.OK, MessageBoxIcon.Stop); 
				this.closeMenuItem_Click(this, new System.EventArgs());
			}
		}

		#region Menu Events
		private void openMenuItem_Click(object sender, System.EventArgs e)
		{
			if (this.openFileDialog.ShowDialog() == DialogResult.OK
				&& this.openFileDialog.FileName.EndsWith(".metadata") || this.openFileDialog.FileName.EndsWith(".index"))
				this.OpenFile(this.openFileDialog.FileName);
		}

		private void closeMenuItem_Click(object sender, System.EventArgs e)
		{
			this.viewerControl.Close();
			this.Controls.Remove((Control)this.viewerControl);
			((Control)this.viewerControl).Dispose();

			this.closeMenuItem.Enabled = false;
			this.pictureBox.Visible = true;
			this.Text = "EarLab DataViewer";
		}

		private void exitMenuItem_Click(object sender, System.EventArgs e)
		{
			Application.Exit();
		}
		#endregion

		private void dataviewerMenuItem_Click(object sender, System.EventArgs e)
		{
			DataViewerDialog dataViewerDialog = new DataViewerDialog();
			dataViewerDialog.ShowDialog(this);
		}

		private void earlabMenuItem_Click(object sender, System.EventArgs e)
		{
			EarLabDialog earlabDialog = new EarLabDialog();
			earlabDialog.ShowDialog(this);
		}

		private void viewerControl_CriticalError(string message)
		{
			MessageBox.Show(this, message, "File Open Error", MessageBoxButtons.OK, MessageBoxIcon.Stop);
			this.closeMenuItem_Click(this, new System.EventArgs());
		}

		private void helpMenuItem_Click(object sender, System.EventArgs e)
		{
			Help.ShowHelp(this, Path.Combine(Application.StartupPath, "DataViewer.chm"));
		}

		private void helpIndexMenuItem_Click(object sender, System.EventArgs e)
		{
			Help.ShowHelpIndex(this, Path.Combine(Application.StartupPath, "DataViewer.chm"));
		}
	}
}
