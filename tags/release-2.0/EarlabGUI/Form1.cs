using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using Microsoft.Win32;
using System.Threading;
using System.IO;

namespace Earlab
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private RegistryKey mKey;
		private RegistryString mModuleDirectory;
		private RegistryString mOutputDirectory;
		private RegistryString mDiagramFile;
		private RegistryString mParameterFile;
		private RegistryInt mFrameCount;
		private RegistryFormWindowState mWindowState;
		private RegistryPoint mWindowLocation;
		private RegistrySize mWindowSize;

		private Thread mExecutionThread;
		private bool mStopModel, mRunning, mStoppedCleanly;
		private LogCallback mLogCallback;

		private static Form1 mMainForm;

		private System.Windows.Forms.MainMenu mainMenu1;
		private System.Windows.Forms.MenuItem menuItem1;
		private System.Windows.Forms.MenuItem menuFileExit;
		private System.Windows.Forms.MenuItem menuModelChooseDiagramFile;
		private System.Windows.Forms.MenuItem menuModelChooseParameterFile;
		private System.Windows.Forms.MenuItem menuItem6;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.MenuItem menuModelRun;
		private System.Windows.Forms.MenuItem menuItem2;
		private System.Windows.Forms.MenuItem menuEnvironmentSetModuleDirectory;
		private System.Windows.Forms.MenuItem menuEnvironmentSetOutputDirectory;
		private System.Windows.Forms.OpenFileDialog openFileDialog;
		private System.Windows.Forms.TextBox txtStatusDisplay;
		private System.Windows.Forms.NumericUpDown udFrameCount;
		private System.Windows.Forms.StatusBar statusBar;
		private System.Windows.Forms.StatusBarPanel sbTextPanel;
		private System.Windows.Forms.Button btnRun;
		private System.Windows.Forms.Button btnStop;
		private System.Windows.Forms.RichTextBox logDisplay;
		private System.Windows.Forms.Timer timer;
		private System.Windows.Forms.Button btnAbort;
		private System.Windows.Forms.FolderBrowserDialog folderBrowser;
		private System.Windows.Forms.ProgressBar progressBar;
		private System.Windows.Forms.MenuItem menuHelp;
		private System.Windows.Forms.MenuItem menuHelpAbout;
		private System.ComponentModel.IContainer components;

		public Form1()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//

			mLogCallback = new LogCallback(LogCallback);
			mKey = Registry.CurrentUser.CreateSubKey(@"Software\Earlab\DesktopEarlabGUI");
			mModuleDirectory = new RegistryString(mKey, "ModuleDirectory", null);
			mOutputDirectory = new RegistryString(mKey, "OutputDirectory", null);
			mDiagramFile = new RegistryString(mKey, "RunParameterFile", null);
			mParameterFile = new RegistryString(mKey, "ModuleParameterFile", null);
			mFrameCount = new RegistryInt(mKey, "FrameCount", 0);
			udFrameCount.Value = mFrameCount.Value;

			mWindowState = new RegistryFormWindowState(mKey, "WindowState", FormWindowState.Normal);
			mWindowLocation = new RegistryPoint(mKey, "WindowLocation", this.Location);
			mWindowSize = new RegistrySize(mKey, "WindowSize", this.MinimumSize);
			this.Location = mWindowLocation.Value;
			this.Size = mWindowSize.Value;
			this.WindowState = mWindowState.Value;

			udFrameCount.Focus();

			UpdateStatusDisplay();
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
			this.components = new System.ComponentModel.Container();
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(Form1));
			this.mainMenu1 = new System.Windows.Forms.MainMenu();
			this.menuItem1 = new System.Windows.Forms.MenuItem();
			this.menuFileExit = new System.Windows.Forms.MenuItem();
			this.menuItem6 = new System.Windows.Forms.MenuItem();
			this.menuModelChooseDiagramFile = new System.Windows.Forms.MenuItem();
			this.menuModelChooseParameterFile = new System.Windows.Forms.MenuItem();
			this.menuModelRun = new System.Windows.Forms.MenuItem();
			this.menuItem2 = new System.Windows.Forms.MenuItem();
			this.menuEnvironmentSetModuleDirectory = new System.Windows.Forms.MenuItem();
			this.menuEnvironmentSetOutputDirectory = new System.Windows.Forms.MenuItem();
			this.label3 = new System.Windows.Forms.Label();
			this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
			this.txtStatusDisplay = new System.Windows.Forms.TextBox();
			this.udFrameCount = new System.Windows.Forms.NumericUpDown();
			this.statusBar = new System.Windows.Forms.StatusBar();
			this.sbTextPanel = new System.Windows.Forms.StatusBarPanel();
			this.btnRun = new System.Windows.Forms.Button();
			this.btnStop = new System.Windows.Forms.Button();
			this.logDisplay = new System.Windows.Forms.RichTextBox();
			this.timer = new System.Windows.Forms.Timer(this.components);
			this.btnAbort = new System.Windows.Forms.Button();
			this.folderBrowser = new System.Windows.Forms.FolderBrowserDialog();
			this.progressBar = new System.Windows.Forms.ProgressBar();
			this.menuHelp = new System.Windows.Forms.MenuItem();
			this.menuHelpAbout = new System.Windows.Forms.MenuItem();
			((System.ComponentModel.ISupportInitialize)(this.udFrameCount)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.sbTextPanel)).BeginInit();
			this.SuspendLayout();
			// 
			// mainMenu1
			// 
			this.mainMenu1.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					  this.menuItem1,
																					  this.menuItem6,
																					  this.menuItem2,
																					  this.menuHelp});
			// 
			// menuItem1
			// 
			this.menuItem1.Index = 0;
			this.menuItem1.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					  this.menuFileExit});
			this.menuItem1.Text = "&File";
			// 
			// menuFileExit
			// 
			this.menuFileExit.Index = 0;
			this.menuFileExit.Shortcut = System.Windows.Forms.Shortcut.AltF4;
			this.menuFileExit.Text = "E&xit";
			this.menuFileExit.Click += new System.EventHandler(this.menuFileExit_Click);
			// 
			// menuItem6
			// 
			this.menuItem6.Index = 1;
			this.menuItem6.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					  this.menuModelChooseDiagramFile,
																					  this.menuModelChooseParameterFile,
																					  this.menuModelRun});
			this.menuItem6.Text = "&Model";
			// 
			// menuModelChooseDiagramFile
			// 
			this.menuModelChooseDiagramFile.Index = 0;
			this.menuModelChooseDiagramFile.Shortcut = System.Windows.Forms.Shortcut.CtrlD;
			this.menuModelChooseDiagramFile.Text = "Choose Diagram File...";
			this.menuModelChooseDiagramFile.Click += new System.EventHandler(this.menuModelChooseDiagramFile_Click);
			// 
			// menuModelChooseParameterFile
			// 
			this.menuModelChooseParameterFile.Index = 1;
			this.menuModelChooseParameterFile.Shortcut = System.Windows.Forms.Shortcut.CtrlP;
			this.menuModelChooseParameterFile.Text = "Choose Parameter File...";
			this.menuModelChooseParameterFile.Click += new System.EventHandler(this.menuModelChooseParameterFile_Click);
			// 
			// menuModelRun
			// 
			this.menuModelRun.Index = 2;
			this.menuModelRun.Shortcut = System.Windows.Forms.Shortcut.F5;
			this.menuModelRun.Text = "Run Model...";
			// 
			// menuItem2
			// 
			this.menuItem2.Index = 2;
			this.menuItem2.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					  this.menuEnvironmentSetModuleDirectory,
																					  this.menuEnvironmentSetOutputDirectory});
			this.menuItem2.Text = "Environment";
			// 
			// menuEnvironmentSetModuleDirectory
			// 
			this.menuEnvironmentSetModuleDirectory.Index = 0;
			this.menuEnvironmentSetModuleDirectory.Text = "Set Module Directory...";
			this.menuEnvironmentSetModuleDirectory.Click += new System.EventHandler(this.menuEnvironmentSetModuleDirectory_Click);
			// 
			// menuEnvironmentSetOutputDirectory
			// 
			this.menuEnvironmentSetOutputDirectory.Index = 1;
			this.menuEnvironmentSetOutputDirectory.Text = "Set Output Directory...";
			this.menuEnvironmentSetOutputDirectory.Click += new System.EventHandler(this.menuEnvironmentSetOutputDirectory_Click);
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(8, 88);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(68, 16);
			this.label3.TabIndex = 7;
			this.label3.Text = "Frame count";
			// 
			// txtStatusDisplay
			// 
			this.txtStatusDisplay.AcceptsReturn = true;
			this.txtStatusDisplay.AcceptsTab = true;
			this.txtStatusDisplay.BackColor = System.Drawing.SystemColors.Control;
			this.txtStatusDisplay.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.txtStatusDisplay.Location = new System.Drawing.Point(8, 8);
			this.txtStatusDisplay.Multiline = true;
			this.txtStatusDisplay.Name = "txtStatusDisplay";
			this.txtStatusDisplay.ReadOnly = true;
			this.txtStatusDisplay.Size = new System.Drawing.Size(536, 72);
			this.txtStatusDisplay.TabIndex = 14;
			this.txtStatusDisplay.TabStop = false;
			this.txtStatusDisplay.Text = "Status Display";
			// 
			// udFrameCount
			// 
			this.udFrameCount.Location = new System.Drawing.Point(8, 104);
			this.udFrameCount.Maximum = new System.Decimal(new int[] {
																		 1000000,
																		 0,
																		 0,
																		 0});
			this.udFrameCount.Name = "udFrameCount";
			this.udFrameCount.Size = new System.Drawing.Size(72, 20);
			this.udFrameCount.TabIndex = 1;
			this.udFrameCount.ValueChanged += new System.EventHandler(this.udFrameCount_ValueChanged);
			// 
			// statusBar
			// 
			this.statusBar.Location = new System.Drawing.Point(0, 325);
			this.statusBar.Name = "statusBar";
			this.statusBar.Panels.AddRange(new System.Windows.Forms.StatusBarPanel[] {
																						 this.sbTextPanel});
			this.statusBar.ShowPanels = true;
			this.statusBar.Size = new System.Drawing.Size(552, 22);
			this.statusBar.TabIndex = 16;
			// 
			// sbTextPanel
			// 
			this.sbTextPanel.Alignment = System.Windows.Forms.HorizontalAlignment.Center;
			this.sbTextPanel.Text = "Not Ready";
			this.sbTextPanel.Width = 200;
			// 
			// btnRun
			// 
			this.btnRun.Enabled = false;
			this.btnRun.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.btnRun.Location = new System.Drawing.Point(104, 104);
			this.btnRun.Name = "btnRun";
			this.btnRun.TabIndex = 2;
			this.btnRun.Text = "Run";
			this.btnRun.Click += new System.EventHandler(this.btnRun_Click);
			// 
			// btnStop
			// 
			this.btnStop.Enabled = false;
			this.btnStop.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.btnStop.Location = new System.Drawing.Point(192, 104);
			this.btnStop.Name = "btnStop";
			this.btnStop.TabIndex = 3;
			this.btnStop.Text = "Stop";
			this.btnStop.Click += new System.EventHandler(this.btnStop_Click);
			// 
			// logDisplay
			// 
			this.logDisplay.AcceptsTab = true;
			this.logDisplay.Location = new System.Drawing.Point(8, 128);
			this.logDisplay.Name = "logDisplay";
			this.logDisplay.ReadOnly = true;
			this.logDisplay.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.Vertical;
			this.logDisplay.Size = new System.Drawing.Size(536, 168);
			this.logDisplay.TabIndex = 19;
			this.logDisplay.TabStop = false;
			this.logDisplay.Text = "";
			this.logDisplay.TextChanged += new System.EventHandler(this.logDisplay_TextChanged);
			// 
			// timer
			// 
			this.timer.Tick += new System.EventHandler(this.timer_Tick);
			// 
			// btnAbort
			// 
			this.btnAbort.Enabled = false;
			this.btnAbort.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.btnAbort.Location = new System.Drawing.Point(280, 104);
			this.btnAbort.Name = "btnAbort";
			this.btnAbort.TabIndex = 20;
			this.btnAbort.Text = "Abort";
			this.btnAbort.Click += new System.EventHandler(this.btnAbort_Click);
			// 
			// progressBar
			// 
			this.progressBar.Location = new System.Drawing.Point(204, 320);
			this.progressBar.Name = "progressBar";
			this.progressBar.Size = new System.Drawing.Size(328, 16);
			this.progressBar.TabIndex = 21;
			// 
			// menuHelp
			// 
			this.menuHelp.Index = 3;
			this.menuHelp.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					 this.menuHelpAbout});
			this.menuHelp.Text = "Help";
			// 
			// menuHelpAbout
			// 
			this.menuHelpAbout.Index = 0;
			this.menuHelpAbout.Text = "About Desktop Earlab...";
			this.menuHelpAbout.Click += new System.EventHandler(this.menuHelpAbout_Click);
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.BackColor = System.Drawing.SystemColors.Control;
			this.ClientSize = new System.Drawing.Size(552, 347);
			this.Controls.Add(this.progressBar);
			this.Controls.Add(this.btnAbort);
			this.Controls.Add(this.logDisplay);
			this.Controls.Add(this.btnStop);
			this.Controls.Add(this.btnRun);
			this.Controls.Add(this.udFrameCount);
			this.Controls.Add(this.txtStatusDisplay);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.statusBar);
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Menu = this.mainMenu1;
			this.MinimumSize = new System.Drawing.Size(448, 381);
			this.Name = "Form1";
			this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
			this.Text = "Desktop Earlab";
			this.Resize += new System.EventHandler(this.Form1_Resize);
			this.Closing += new System.ComponentModel.CancelEventHandler(this.Form1_Closing);
			this.Load += new System.EventHandler(this.Form1_Load);
			this.LocationChanged += new System.EventHandler(this.Form1_LocationChanged);
			((System.ComponentModel.ISupportInitialize)(this.udFrameCount)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.sbTextPanel)).EndInit();
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.EnableVisualStyles();
			mMainForm = new Form1();
			Application.Run(mMainForm);
		}

		static void LogCallback(String Message)
		{
			mMainForm.Log(Message);
		}

		private void menuEnvironmentSetModuleDirectory_Click(object sender, System.EventArgs e)
		{
			folderBrowser.Reset();
			//folderBrowser.RootFolder = Environment.SpecialFolder.Desktop;
			if (mModuleDirectory != null)
				folderBrowser.SelectedPath = mModuleDirectory.Value;
			folderBrowser.ShowNewFolderButton = false;
			folderBrowser.Description = "Choose the directory that contains the Earlab modules";
			if (folderBrowser.ShowDialog() == DialogResult.OK)
			{
				mModuleDirectory.Value = folderBrowser.SelectedPath;
				UpdateStatusDisplay();
			}
		}

		private void menuEnvironmentSetOutputDirectory_Click(object sender, System.EventArgs e)
		{
			folderBrowser.Reset();
			if (mModuleDirectory != null)
				folderBrowser.SelectedPath = mOutputDirectory.Value;
			folderBrowser.ShowNewFolderButton = true;
			folderBrowser.Description = "Choose the directory that will hold your output files";
			if (folderBrowser.ShowDialog() == DialogResult.OK)
			{
				mOutputDirectory.Value = folderBrowser.SelectedPath;
				UpdateStatusDisplay();
			}
		}

		private void menuFileExit_Click(object sender, System.EventArgs e)
		{
			StopModelAndWait();
			Application.Exit();
		}

		private void menuModelChooseDiagramFile_Click(object sender, System.EventArgs e)
		{
			openFileDialog.FileName = mDiagramFile.Value;
			openFileDialog.Title = "Choose Diagram file";

			openFileDialog.CheckFileExists = true;
			openFileDialog.CheckPathExists = true;
			openFileDialog.Filter = "Diagram files (*.diagram)|*.diagram|All files (*.*)|*.*";
			openFileDialog.FilterIndex = 0;
			openFileDialog.Multiselect = false;
			openFileDialog.RestoreDirectory = true;
			openFileDialog.ValidateNames = true;
			if (openFileDialog.ShowDialog() == DialogResult.OK)
			{
				mDiagramFile.Value = openFileDialog.FileName;
				UpdateStatusDisplay();
			}
		}

		private void menuModelChooseParameterFile_Click(object sender, System.EventArgs e)
		{
			openFileDialog.FileName = mParameterFile.Value;
			openFileDialog.Title = "Choose Parameter file";

			openFileDialog.CheckFileExists = true;
			openFileDialog.CheckPathExists = true;
			openFileDialog.Filter = "Parameter files (*.parameters)|*.parameters|All files (*.*)|*.*";
			openFileDialog.FilterIndex = 0;
			openFileDialog.Multiselect = false;
			openFileDialog.RestoreDirectory = true;
			openFileDialog.ValidateNames = true;
			if (openFileDialog.ShowDialog() == DialogResult.OK)
			{
				mParameterFile.Value = openFileDialog.FileName;
				UpdateStatusDisplay();
			}		
		}

		private void UpdateStatusDisplay()
		{
			string [] MyLines = new string[5];
			bool SimOK = true;
			int LineCount = 0;

			if (mDiagramFile.Value != null)
				MyLines[LineCount++] = "Diagram file is \"" + mDiagramFile.Value + "\"";
			else
			{
				MyLines[LineCount++] = "Diagram file is not set";
				SimOK = false;
			}
		
			if (mParameterFile.Value != null)
				MyLines[LineCount++] = "Parameter file is \"" + mParameterFile.Value + "\"";
			else
			{
				MyLines[LineCount++] = "Parameter file is not set";
				SimOK = false;
			}

			if (mOutputDirectory.Value != null)
				MyLines[LineCount++] = "Output Directory is \"" + mOutputDirectory.Value + "\"";
			else
			{
				MyLines[LineCount++] = "Output Directory is not set";
				SimOK = false;
			}

			if (mModuleDirectory.Value != null)
				MyLines[LineCount++] = "Module Directory is \"" + mModuleDirectory.Value + "\"";
			else
			{
				MyLines[LineCount++] = "Module Directory is not set";
				SimOK = false;
			}

			if (mFrameCount.Value != 0)
				MyLines[LineCount++] = "Simulation will run for " + mFrameCount.Value + " frames";
			else
			{
				MyLines[LineCount++] = "Frame count not set";
				SimOK = false;
			}

			txtStatusDisplay.Lines = MyLines;
			if (SimOK)
			{
				sbTextPanel.Text = "Ready";
				btnRun.Enabled = true;
				txtStatusDisplay.ForeColor = SystemColors.ControlText;
			}
			else
			{
				btnRun.Enabled = false;
				sbTextPanel.Text = "Not Ready";
				txtStatusDisplay.ForeColor = Color.Red;
			}
			txtStatusDisplay.SelectionLength = 0;
			udFrameCount.Focus();
		}

		private void udFrameCount_ValueChanged(object sender, System.EventArgs e)
		{
			mFrameCount.Value = (int)udFrameCount.Value;
			UpdateStatusDisplay();
		}

		private void btnRun_Click(object sender, System.EventArgs e)
		{
			mExecutionThread = new Thread(new ThreadStart(RunModel));
			mExecutionThread.IsBackground = true;
			mExecutionThread.Name = "Model Execution Thread";
			mStopModel = false;
			progressBar.Value = 0;
			progressBar.Visible = true;
			progressBar.Minimum = 0;
			progressBar.Maximum = mFrameCount.Value + 1;
			//mExecutionThread.Priority = ThreadPriority.BelowNormal;
			mExecutionThread.Start();
			timer.Enabled = true;
		}
		
		private void btnStop_Click(object sender, System.EventArgs e)
		{
			btnAbort.Enabled = true;
			StopModel();
		}

		private void RunModel()
		{
			int i;
			DesktopEarlabDLL theDLL = new DesktopEarlabDLL();

			mStoppedCleanly = false;
			btnRun.Enabled = false;
			btnStop.Enabled = true;
			udFrameCount.Enabled = false;
			progressBar.Visible = true;

			ClearLog();

			mRunning = false;
			sbTextPanel.Text = "Running";
			Log("Starting simulation");

			theDLL.SetLogCallback(mLogCallback);

			Log("Module directory: \"" + mModuleDirectory.Value + "\"");
			if (theDLL.SetModuleDirectory(mModuleDirectory.Value) == 0)
			{
				Log("Error setting module directory");
				return;
			}

			Log("Model configuration: \"" + mDiagramFile.Value + "\"");
			if (theDLL.LoadModelConfigFile(mDiagramFile.Value, 1000.0f) == 0)
			{
				Log("Error loading model config file");
				return;
			}
			
			Log("Module parameters: \"" + mParameterFile.Value + "\"");
			if (theDLL.LoadModuleParameters(mParameterFile.Value) == 0)
			{
				Log("Error loading module parameter file");
				return;
			}

			Log("Setting current directory to output directory (\"" + mOutputDirectory.Value + "\")");
			theDLL.SetOutputDirectory(mOutputDirectory.Value);

			Log("Starting modules");
			if (theDLL.StartModules() == 0)
			{
				Log("Error starting modules");
				return;
			}

			for (i = 0; i < mFrameCount.Value; i++)
			{
				mRunning = true;
				if (mStopModel)
					break;
				progressBar.Value = i;

				sbTextPanel.Text = "Processing frame " + (i + 1) + " of " + mFrameCount.Value;

				if (i == 0)
					Log("Starting frame " + (i + 1) + " of " + mFrameCount.Value);
				try
				{
					if (theDLL.AdvanceModules() == 0)
					{
						Log("Error processing frame " + i + " of " + mFrameCount.Value);
						return;
					}
				}
				catch (Exception e)
				{
					Log("Caught exception: " + e.ToString());
				}
				Application.DoEvents();
				Thread.Sleep(100);
				theDLL.SetLogCallback(null);
			}

			theDLL.SetLogCallback(mLogCallback);

			sbTextPanel.Text = "Stopping";
			Log("Stopping modules");
			if (theDLL.StopModules() == 0)
			{
				Log("Error stopping modules");
				return;
			}

			Log("Unloading modules");
			if (theDLL.UnloadModel() == 0)
			{
				Log("Error unloading model");
				return;
			}

			btnRun.Enabled = true;
			btnStop.Enabled = false;
			btnAbort.Enabled = false;
			udFrameCount.Enabled = true;
			progressBar.Visible = false;
			UpdateStatusDisplay();
			udFrameCount.Focus();
			mStoppedCleanly = true;
			mRunning = false;
		}

		void ClearLog()
		{
			logDisplay.Text = "";
		}

		void Log(string Message)
		{
			logDisplay.AppendText(DateTime.Now.ToString() + " " + Message + "\r\n");
		}

		private void logDisplay_TextChanged(object sender, System.EventArgs e)
		{
			logDisplay.Focus();
			logDisplay.SelectionStart = logDisplay.TextLength;
			//logDisplay.SelectionLength = 0;
			logDisplay.ScrollToCaret();
		}

		private void Form1_Resize(object sender, System.EventArgs e)
		{
			if (this.WindowState == FormWindowState.Normal)
			{
				mWindowSize.Value = this.Size;
				txtStatusDisplay.Width = this.ClientSize.Width - 16;
				logDisplay.Width = this.ClientSize.Width - 16;
				logDisplay.Height = (this.ClientSize.Height - 24) - logDisplay.Top;
				progressBar.Top = statusBar.Top + ((statusBar.Height - progressBar.Height) / 2);
				progressBar.Width = this.Width - progressBar.Left - 30;
			}
			mWindowState.Value = this.WindowState;
		}

		private void Form1_LocationChanged(object sender, System.EventArgs e)
		{
			if (this.WindowState == FormWindowState.Normal)
				mWindowLocation.Value = this.Location;
			mWindowState.Value = this.WindowState;
		}

		private void StopModel()
		{
			sbTextPanel.Text = "Stopping (User request)";
			Log("User requested stop");
			if ((mExecutionThread != null) && mExecutionThread.IsAlive && mRunning)
				mStopModel = true;
		} // private void StopModel()

		private void StopModelAndWait()
		{
			int WaitTimeout = 3000; // Wait 30 seconds for model to stop
			StopModel();
			while ((mExecutionThread != null) && mExecutionThread.IsAlive && mRunning && (WaitTimeout > 0))
			{
				Thread.Sleep(10);
				Application.DoEvents();
				WaitTimeout--;
			}
			if (WaitTimeout <= 0)
				AbortModel();
		} // private void StopModel()

		private void AbortModel()
		{
			if (mExecutionThread.IsAlive)
			{
				Log("User requested abort");
				mExecutionThread.Abort();
				mStoppedCleanly = false;
				sbTextPanel.Text = "Aborted";
			} // if (mExecutionThread.IsAlive)
			btnAbort.Enabled = false;
		}

		private void Form1_Closing(object sender, System.ComponentModel.CancelEventArgs e)
		{
			StopModelAndWait();
		}

		private void timer_Tick(object sender, System.EventArgs e)
		{
			if (!mExecutionThread.IsAlive)
			{
				if (!mStoppedCleanly)
					Log("Model failed");
				else
					Log("Model stopped");
				timer.Enabled = false;
				btnRun.Enabled = true;
				btnStop.Enabled = false;
				btnAbort.Enabled = false;
				UpdateStatusDisplay();
				progressBar.Visible = false;
				progressBar.Value = 0;
			}
		}

		private void btnAbort_Click(object sender, System.EventArgs e)
		{
			AbortModel();
		}

		private void Form1_Load(object sender, System.EventArgs e)
		{
			progressBar.Visible = false;
		}

		private void menuHelpAbout_Click(object sender, System.EventArgs e)
		{
			Form About = new EarlabGUI.About();

			About.Show();
		}
	}
}
