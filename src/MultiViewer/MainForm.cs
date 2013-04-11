using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace MultiViewer
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class MainForm : System.Windows.Forms.Form
	{
		private System.Windows.Forms.MainMenu mainMenu;
		private System.Windows.Forms.MenuItem fileMenuItem;
		private System.Windows.Forms.MenuItem openMenuItem;
		private System.Windows.Forms.MenuItem closeMenuItem;
		private System.Windows.Forms.MenuItem closeallMenuItem;
		private System.Windows.Forms.MenuItem exitMenuItem;
		private System.Windows.Forms.MenuItem seperator1MenuItem;
		private MultiViewer.MainControl mainControl;
		private System.Windows.Forms.PictureBox pictureBox;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public MainForm()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();
			EarLab.Utilities.EnableVisualStyles.EnableControl(this);

			this.CheckPictureBox();
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
			this.closeallMenuItem = new System.Windows.Forms.MenuItem();
			this.seperator1MenuItem = new System.Windows.Forms.MenuItem();
			this.exitMenuItem = new System.Windows.Forms.MenuItem();
			this.mainControl = new MultiViewer.MainControl();
			this.pictureBox = new System.Windows.Forms.PictureBox();
			this.SuspendLayout();
			// 
			// mainMenu
			// 
			this.mainMenu.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					 this.fileMenuItem});
			// 
			// fileMenuItem
			// 
			this.fileMenuItem.Index = 0;
			this.fileMenuItem.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																						 this.openMenuItem,
																						 this.closeMenuItem,
																						 this.closeallMenuItem,
																						 this.seperator1MenuItem,
																						 this.exitMenuItem});
			this.fileMenuItem.Text = "File";
			// 
			// openMenuItem
			// 
			this.openMenuItem.Index = 0;
			this.openMenuItem.Text = "Open...";
			this.openMenuItem.Click += new System.EventHandler(this.openMenuItem_Click);
			// 
			// closeMenuItem
			// 
			this.closeMenuItem.Index = 1;
			this.closeMenuItem.Text = "Close...";
			this.closeMenuItem.Click += new System.EventHandler(this.closeMenuItem_Click);
			// 
			// closeallMenuItem
			// 
			this.closeallMenuItem.Index = 2;
			this.closeallMenuItem.Text = "Close All";
			this.closeallMenuItem.Click += new System.EventHandler(this.closeallMenuItem_Click);
			// 
			// seperator1MenuItem
			// 
			this.seperator1MenuItem.Index = 3;
			this.seperator1MenuItem.Text = "-";
			// 
			// exitMenuItem
			// 
			this.exitMenuItem.Index = 4;
			this.exitMenuItem.Text = "Exit";
			this.exitMenuItem.Click += new System.EventHandler(this.exitMenuItem_Click);
			// 
			// mainControl
			// 
			this.mainControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.mainControl.LLFile = "";
			this.mainControl.Location = new System.Drawing.Point(0, 0);
			this.mainControl.LRFile = "";
			this.mainControl.Name = "mainControl";
			this.mainControl.Size = new System.Drawing.Size(848, 400);
			this.mainControl.TabIndex = 0;
			this.mainControl.ULFile = "";
			this.mainControl.URFile = "";
			// 
			// pictureBox
			// 
			this.pictureBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.pictureBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.pictureBox.Image = ((System.Drawing.Image)(resources.GetObject("pictureBox.Image")));
			this.pictureBox.Location = new System.Drawing.Point(8, 8);
			this.pictureBox.Name = "pictureBox";
			this.pictureBox.Size = new System.Drawing.Size(832, 360);
			this.pictureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
			this.pictureBox.TabIndex = 1;
			this.pictureBox.TabStop = false;
			// 
			// MainForm
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(848, 401);
			this.Controls.Add(this.pictureBox);
			this.Controls.Add(this.mainControl);
			this.Menu = this.mainMenu;
			this.Name = "MainForm";
			this.Text = "EarLab MultiViewer";
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			EarLab.Utilities.EnableVisualStyles.EnableApplication();
			Application.Run(new MainForm());
		}

		private void exitMenuItem_Click(object sender, System.EventArgs e)
		{
			Application.Exit();
		}

		private void openMenuItem_Click(object sender, System.EventArgs e)
		{
			OpenDialog openDialog = new OpenDialog();
			
			openDialog.ULFile = this.mainControl.ULFile;
			openDialog.URFile = this.mainControl.URFile;
			openDialog.LLFile = this.mainControl.LLFile;
			openDialog.LRFile = this.mainControl.LRFile;

			if (openDialog.ShowDialog(this) == DialogResult.OK)
			{
				if (this.mainControl.ULFile != openDialog.ULFile)
					this.mainControl.ULFile = openDialog.ULFile;
				if (this.mainControl.URFile != openDialog.URFile)
					this.mainControl.URFile = openDialog.URFile;
				if (this.mainControl.LLFile != openDialog.LLFile)
					this.mainControl.LLFile = openDialog.LLFile;
				if (this.mainControl.LRFile != openDialog.LRFile)
					this.mainControl.LRFile = openDialog.LRFile;
			}

			this.CheckPictureBox();
		}

		private void closeallMenuItem_Click(object sender, System.EventArgs e)
		{
			this.mainControl.ULFile = "";
			this.mainControl.URFile = "";
			this.mainControl.LLFile = "";
			this.mainControl.LRFile = "";

			this.CheckPictureBox();
		}

		private void CheckPictureBox()
		{
			if (this.mainControl.ULFile == "" && this.mainControl.URFile == ""
				&& this.mainControl.LLFile == "" && this.mainControl.LRFile == "")
			{
				this.pictureBox.Visible = true;
				this.pictureBox.BringToFront();
				this.closeMenuItem.Enabled = false;
				this.closeallMenuItem.Enabled = false;
			}
			else
			{
				this.pictureBox.Visible = false;
				this.pictureBox.SendToBack();
				this.closeMenuItem.Enabled = true;
				this.closeallMenuItem.Enabled= true;
			}
		}

		private void closeMenuItem_Click(object sender, System.EventArgs e)
		{
			bool ulBool, urBool, llBool, lrBool;
			ulBool = urBool = llBool = lrBool = false;

			if (this.mainControl.ULFile != "")
				ulBool = true;
			if (this.mainControl.URFile != "")
				urBool = true;
			if (this.mainControl.LLFile != "")
				llBool = true;
			if (this.mainControl.LRFile != "")
				lrBool = true;

			CloseDialog closeDialog = new CloseDialog(ulBool, urBool, llBool, lrBool);

			if (closeDialog.ShowDialog(this) == DialogResult.OK)
			{
				if (closeDialog.ULChecked)
					this.mainControl.ULFile = "";
				if (closeDialog.URChecked)
					this.mainControl.URFile = "";
				if (closeDialog.LLChecked)
					this.mainControl.LLFile = "";
				if (closeDialog.LRChecked)
					this.mainControl.LRFile = "";

				this.CheckPictureBox();
			}
		}
	}
}
