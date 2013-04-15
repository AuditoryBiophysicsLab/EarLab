using System.ComponentModel;
using System.Windows.Forms;
using System.Collections;
using System.Diagnostics;
using System.Reflection;
using Microsoft.Win32;
using System.Drawing;
using System.IO;
using System;

namespace EarLab
{
	/// <summary>
	/// Summary description for TreeView.
	/// </summary>
	public class TreeView : System.Windows.Forms.Form
	{
		private System.Windows.Forms.TreeView treeView1;
		private System.ComponentModel.IContainer components;
		private System.Windows.Forms.MainMenu mainMenu1;
		private System.Windows.Forms.MenuItem menuItem1;
		private System.Windows.Forms.ContextMenu runContextMenu;
		private System.Windows.Forms.MenuItem contextMenuRun;

		private RunFile theRunFile;
		private TreeNode SelectedNode, HoverNode;
        static NodeInfo lastNodeInfo;
        private System.Windows.Forms.MenuItem contextMenuOpenSimulation;
		private System.Windows.Forms.MenuItem menuBrowseViewFolder;
		private System.Windows.Forms.Timer timer1;
		private System.Windows.Forms.OpenFileDialog openFileDialog1;
		private System.Windows.Forms.ToolTip toolTip1;
		private System.Windows.Forms.MenuItem menuFileOpenRunfile;
		private System.Windows.Forms.MenuItem menuItem3;
		private System.Windows.Forms.MenuItem menuFileExit;
		private string RunFileName;
		private RegistryKey theKey, mApplicationKey;
		private RegistryString theRunfileName;
		private RegistryBool mEnableSuperuserMode;

		private const int RunCommand = 0;
		private const int Description = 1;
		private const int DiagramFile = 2;
		private const int ParameterFile = 3;
		private const int InputDirectory = 4;
		private const int OutputDirectory = 5;

		private const int CochleaIcon = 0;
		private const int CochleaIconSelected = 0;
		private const int QuestionIcon = 1;
		private const int QuestionIconSelected = 1;
		private const int TextIcon = 2;
		private const int TextIconSelected = 2;
		private const int FolderIcon = 3;
		private const int FolderIconSelected = 3;
		private const int PlayIcon = 4;
		private System.Windows.Forms.MenuItem menuBrowseDeleteFolder;
		private System.Windows.Forms.ContextMenu browseOutputContext;
		private System.Windows.Forms.ContextMenu browseInputContext;
		private System.Windows.Forms.MenuItem menuItem2;
		private const int PlayIconSelected = 4;
		private System.Windows.Forms.MenuItem menuHelp;
		private System.Windows.Forms.MenuItem menuHelpFile;
		private System.Windows.Forms.MenuItem menuHelpAbout;
		Process p;
        private bool bCopyCompleted;

		public TreeView(string RunFileName)
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			RegistryInit();
			RunFile = RunFileName;
		}

		public TreeView()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			RegistryInit();
		}

		private void RegistryInit()
		{
            try
            {
                theKey = null;
                theRunfileName = null;
                mApplicationKey = null;
                mEnableSuperuserMode = null;
                theKey = Registry.CurrentUser.CreateSubKey(@"Software\EarLab");
                theRunfileName = new RegistryString(theKey, "RunfileName", Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments), @"EarLab\runfile.xml"));
                mApplicationKey = Registry.CurrentUser.CreateSubKey(@"Software\EarLab\EarLab GUI");
                mEnableSuperuserMode = new RegistryBool(mApplicationKey, "SuperuserMode", false);
            }
            catch
            {
            }
            if ((theRunfileName == null) || (theRunfileName.Value == null) || (theRunfileName.Value.Length == 0))
                RunFileName = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments), @"EarLab\runfile.xml");
            else
                RunFileName = theRunfileName.Value;

            if (!File.Exists(RunFileName))
            {
                xDirectory Copier = new xDirectory();
                string source = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles), @"EarLab\Models");
                string dest = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments), @"EarLab");

                if (!Directory.Exists(source))
                {
                    MessageBox.Show("Cannot locate common sample experiment source in \"" + source + "\".  Exiting.");
                    Application.Exit();
                    return;
                }
                if (!Directory.Exists(dest))
                    Directory.CreateDirectory(dest);

                Copier.Source = new DirectoryInfo(source);
                Copier.Destination = new DirectoryInfo(dest);
                Copier.Overwrite = true;
                Copier.FolderFilter = "*";
                Copier.FileFilters.Add("*.*");
                bCopyCompleted = false;
                Copier.StartCopy();
                Copier.CopyComplete += new CopyCompleteEventHandler(Copier_CopyComplete);
                while (!bCopyCompleted)
                    System.Threading.Thread.Sleep(100);
            }
            if (!File.Exists(RunFileName))
            {
                MessageBox.Show("Required ExperimentManager run file \"" + RunFile + "\" not found.  It may be necessary to reinstall EarLab before continuing.");
                Application.Exit();
                return;
            }
            RunFile = RunFileName;
        }

        void Copier_CopyComplete(object sender, CopyCompleteEventArgs e)
        {
            bCopyCompleted = true;
        }

		public string RunFile
		{
			get {return RunFileName;}
			set
			{
				RunFileName = value;
				OpenRunFile();
			}
		}

		private void OpenRunFile()
		{
			NodeInfo theInfo;

			if (!File.Exists(RunFileName))
				return;

			theRunfileName.Value = RunFileName;

			theRunFile = new RunFile(RunFileName);
			treeView1.BeginUpdate();
			treeView1.Nodes.Clear();

			ImageList myImageList = new ImageList();
			Stream resourceStream;
			
			timer1.Enabled = true;

			resourceStream = Assembly.GetExecutingAssembly().GetManifestResourceStream("EarLab.Cochlea.bmp");
			myImageList.Images.Add(Image.FromStream(resourceStream));
			resourceStream = Assembly.GetExecutingAssembly().GetManifestResourceStream("EarLab.QuestionMark.bmp");
			myImageList.Images.Add(Image.FromStream(resourceStream));
			resourceStream = Assembly.GetExecutingAssembly().GetManifestResourceStream("EarLab.TextDocument.bmp");
			myImageList.Images.Add(Image.FromStream(resourceStream));
			resourceStream = Assembly.GetExecutingAssembly().GetManifestResourceStream("EarLab.Folder.bmp");
			myImageList.Images.Add(Image.FromStream(resourceStream));
			resourceStream = Assembly.GetExecutingAssembly().GetManifestResourceStream("EarLab.PlayIcon.bmp");
			myImageList.Images.Add(Image.FromStream(resourceStream));
			treeView1.ImageList = myImageList;
#if DEBUG
            var executablePath = Path.GetDirectoryName(Assembly.GetEntryAssembly().Location);
#else
            var executablePath = Path.GetDirectoryName(Assembly.GetEntryAssembly().Location);
#endif
            for (int CurRun = 0; CurRun < theRunFile.Run.Length; CurRun++)
			{
				treeView1.Nodes.Insert(CurRun, new TreeNode(theRunFile.Run[CurRun].Name));
				treeView1.Nodes[CurRun].ImageIndex = CochleaIcon;
				treeView1.Nodes[CurRun].SelectedImageIndex = CochleaIconSelected;
				theInfo = new NodeInfo();
				theInfo.ToolTipType = ToolTipType.Static;
				theInfo.NodeInfoType = NodeInfoType.ContextMenu;
				theInfo.ContextMenu = runContextMenu;
				theInfo.ToolTip = "Right click for options";
				theInfo.RunRecord = theRunFile.Run[CurRun];
				theInfo.ProgramName = Path.Combine(executablePath, "Simulator.exe");
				treeView1.Nodes[CurRun].Tag = theInfo;

				treeView1.Nodes[CurRun].Nodes.Insert(RunCommand, new TreeNode("Double click to run this simulation"));
				treeView1.Nodes[CurRun].Nodes[RunCommand].ForeColor = Color.Black;
				treeView1.Nodes[CurRun].Nodes[RunCommand].ImageIndex = PlayIcon;
				treeView1.Nodes[CurRun].Nodes[RunCommand].SelectedImageIndex = PlayIconSelected;
				theInfo = new NodeInfo();
				theInfo.ToolTipType = ToolTipType.Static;
				theInfo.NodeInfoType = NodeInfoType.RunSimulation;
				theInfo.ToolTip = "Double click to run the simulation";
				theInfo.RunRecord = theRunFile.Run[CurRun];
                theInfo.ProgramName = Path.Combine(executablePath, "Simulator.exe");
                treeView1.Nodes[CurRun].Nodes[RunCommand].Tag = theInfo;

				treeView1.Nodes[CurRun].Nodes.Insert(Description, new TreeNode("Description: " + theRunFile.Run[CurRun].Description));
				treeView1.Nodes[CurRun].Nodes[Description].ForeColor = Color.MediumSlateBlue;
				treeView1.Nodes[CurRun].Nodes[Description].ImageIndex = QuestionIcon;
				treeView1.Nodes[CurRun].Nodes[Description].SelectedImageIndex = QuestionIconSelected;
				treeView1.Nodes[CurRun].Nodes[Description].NodeFont = new Font(treeView1.Font, FontStyle.Italic);
				treeView1.Nodes[CurRun].Nodes[Description].Tag = null;

				treeView1.Nodes[CurRun].Nodes.Insert(DiagramFile, new TreeNode("Diagram File: " + theRunFile.Run[CurRun].DiagramFileName));
				treeView1.Nodes[CurRun].Nodes[DiagramFile].ImageIndex = TextIcon;
				treeView1.Nodes[CurRun].Nodes[DiagramFile].SelectedImageIndex = TextIconSelected;
				theInfo = new NodeInfo();
				theInfo.ToolTipType = ToolTipType.FilenameDependent;
				theInfo.NodeInfoType = NodeInfoType.TextViewer;
				theInfo.ToolTip = "Double click to open the file";
				theInfo.ProgramName = "notepad.exe";
				theInfo.FileName = theRunFile.Run[CurRun].DiagramFileName;
				treeView1.Nodes[CurRun].Nodes[DiagramFile].Tag = theInfo;
				
				treeView1.Nodes[CurRun].Nodes.Insert(ParameterFile, new TreeNode("Parameter File: " + theRunFile.Run[CurRun].ParameterFileName));
				treeView1.Nodes[CurRun].Nodes[ParameterFile].ImageIndex = TextIcon;
				treeView1.Nodes[CurRun].Nodes[ParameterFile].SelectedImageIndex = TextIconSelected;
				theInfo = new NodeInfo();
				theInfo.NodeInfoType = NodeInfoType.TextViewer;
				theInfo.ToolTipType = ToolTipType.FilenameDependent;
				theInfo.ToolTip = "Double click to open the file";
				theInfo.ProgramName = "notepad.exe";
				theInfo.FileName = theRunFile.Run[CurRun].ParameterFileName;
				treeView1.Nodes[CurRun].Nodes[ParameterFile].Tag = theInfo;
				
				treeView1.Nodes[CurRun].Nodes.Insert(InputDirectory, new TreeNode("Input Directory: " + theRunFile.Run[CurRun].InputDirectoryPath));
				treeView1.Nodes[CurRun].Nodes[InputDirectory].ImageIndex = FolderIcon;
				treeView1.Nodes[CurRun].Nodes[InputDirectory].SelectedImageIndex = FolderIconSelected;
				theInfo = new NodeInfo();
				theInfo.NodeInfoType = NodeInfoType.ContextMenu;
				theInfo.ToolTip = "Right click for more options";
				theInfo.ToolTipType = ToolTipType.Static;
				theInfo.ContextMenu = browseInputContext;
				theInfo.ProgramName = "explorer.exe";
				theInfo.FileName = theRunFile.Run[CurRun].InputDirectoryPath;
				treeView1.Nodes[CurRun].Nodes[InputDirectory].Tag = theInfo;
				
				treeView1.Nodes[CurRun].Nodes.Insert(OutputDirectory, new TreeNode("Output Directory: " + theRunFile.Run[CurRun].OutputDirectoryPath));
				treeView1.Nodes[CurRun].Nodes[OutputDirectory].ImageIndex = FolderIcon;
				treeView1.Nodes[CurRun].Nodes[OutputDirectory].SelectedImageIndex = FolderIconSelected;
				theInfo = new NodeInfo();
				theInfo.NodeInfoType = NodeInfoType.ContextMenu;
				theInfo.ToolTip = "Right click for more options";
				theInfo.ToolTipType = ToolTipType.Static;
				theInfo.ContextMenu = browseOutputContext;
				theInfo.ProgramName = "explorer.exe";
				theInfo.FileName = theRunFile.Run[CurRun].OutputDirectoryPath;
				treeView1.Nodes[CurRun].Nodes[OutputDirectory].Tag = theInfo;

				for (int InputFile = 0; InputFile < theRunFile.Run[CurRun].SourceFileNames.Length; InputFile++)
				{
					treeView1.Nodes[CurRun].Nodes[InputDirectory].Nodes.Insert(InputFile, new TreeNode(theRunFile.Run[CurRun].SourceFileNames[InputFile]));
					treeView1.Nodes[CurRun].Nodes[InputDirectory].Nodes[InputFile].ImageIndex = TextIcon;
					treeView1.Nodes[CurRun].Nodes[InputDirectory].Nodes[InputFile].SelectedImageIndex = TextIconSelected;
					theInfo = new NodeInfo();
					theInfo.ToolTipType = ToolTipType.FilenameDependent;
					theInfo.NodeInfoType = NodeInfoType.RunProgram;
					theInfo.ToolTip = "Double click to open the file";
					theInfo.ProgramName = "explorer.exe";
					theInfo.FileName = Path.Combine(theRunFile.Run[CurRun].InputDirectoryPath, theRunFile.Run[CurRun].SourceFileNames[InputFile]);
					if (Path.GetExtension(theInfo.FileName) == ".wav")
					{
						theInfo.ProgramName = "wmplayer.exe";
					}
					treeView1.Nodes[CurRun].Nodes[InputDirectory].Nodes[InputFile].Tag = theInfo;

					if (File.Exists(((NodeInfo)(treeView1.Nodes[CurRun].Nodes[InputDirectory].Nodes[InputFile].Tag)).FileName))
						treeView1.Nodes[CurRun].Nodes[InputDirectory].Nodes[InputFile].ForeColor = Color.Black;
					else
						treeView1.Nodes[CurRun].Nodes[InputDirectory].Nodes[InputFile].ForeColor = Color.LightGray;
				}
				for (int OutputFile = 0; OutputFile < theRunFile.Run[CurRun].OutputFileNames.Length; OutputFile++)
				{
					treeView1.Nodes[CurRun].Nodes[OutputDirectory].Nodes.Insert(OutputFile, new TreeNode(theRunFile.Run[CurRun].OutputFileNames[OutputFile]));
					treeView1.Nodes[CurRun].Nodes[OutputDirectory].Nodes[OutputFile].ImageIndex = TextIcon;
					treeView1.Nodes[CurRun].Nodes[OutputDirectory].Nodes[OutputFile].SelectedImageIndex = TextIconSelected;
					treeView1.Nodes[CurRun].Nodes[OutputDirectory].Nodes[OutputFile].Tag = Path.Combine(theRunFile.Run[CurRun].OutputDirectoryPath, theRunFile.Run[CurRun].OutputFileNames[OutputFile]);
					theInfo = new NodeInfo();
					theInfo.ToolTipType = ToolTipType.FilenameDependent;
					theInfo.ToolTip = "Double click to open the file";
					if ((Path.GetExtension(theRunFile.Run[CurRun].OutputFileNames[OutputFile]) == ".metadata") ||
						(Path.GetExtension(theRunFile.Run[CurRun].OutputFileNames[OutputFile]) == ".index"))
					{
						theInfo.NodeInfoType = NodeInfoType.RunProgram;
						theInfo.ProgramName = @"C:\Program Files\EarLab\DataViewer\DataViewer.exe";
					}
					else
					{
						theInfo.NodeInfoType = NodeInfoType.TextViewer;
						theInfo.ProgramName = @"notepad.exe";
					}
					theInfo.FileName = Path.Combine(theRunFile.Run[CurRun].OutputDirectoryPath, theRunFile.Run[CurRun].OutputFileNames[OutputFile]);
					treeView1.Nodes[CurRun].Nodes[OutputDirectory].Nodes[OutputFile].Tag = theInfo;
					if (File.Exists(((NodeInfo)(treeView1.Nodes[CurRun].Nodes[OutputDirectory].Nodes[OutputFile].Tag)).FileName))
						treeView1.Nodes[CurRun].Nodes[OutputDirectory].Nodes[OutputFile].ForeColor = Color.Black;
					else
						treeView1.Nodes[CurRun].Nodes[OutputDirectory].Nodes[OutputFile].ForeColor = Color.LightGray;
				}
			}
			treeView1.EndUpdate();
			treeView1.Location = new Point(0, 0);
			treeView1.Size = this.ClientSize;
			toolTip1.SetToolTip(treeView1, "Test Tip");
		}

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main(string [] args) 
		{

			Application.EnableVisualStyles();
			if (args.Length == 0)
				Application.Run(new TreeView());
			else
				Application.Run(new TreeView(args[0]));
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

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
            this.components = new System.ComponentModel.Container();
            this.treeView1 = new System.Windows.Forms.TreeView();
            this.runContextMenu = new System.Windows.Forms.ContextMenu();
            this.contextMenuRun = new System.Windows.Forms.MenuItem();
            this.contextMenuOpenSimulation = new System.Windows.Forms.MenuItem();
            this.mainMenu1 = new System.Windows.Forms.MainMenu(this.components);
            this.menuItem1 = new System.Windows.Forms.MenuItem();
            this.menuFileOpenRunfile = new System.Windows.Forms.MenuItem();
            this.menuItem3 = new System.Windows.Forms.MenuItem();
            this.menuFileExit = new System.Windows.Forms.MenuItem();
            this.menuHelp = new System.Windows.Forms.MenuItem();
            this.menuHelpFile = new System.Windows.Forms.MenuItem();
            this.menuHelpAbout = new System.Windows.Forms.MenuItem();
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this.browseOutputContext = new System.Windows.Forms.ContextMenu();
            this.menuBrowseViewFolder = new System.Windows.Forms.MenuItem();
            this.menuBrowseDeleteFolder = new System.Windows.Forms.MenuItem();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.browseInputContext = new System.Windows.Forms.ContextMenu();
            this.menuItem2 = new System.Windows.Forms.MenuItem();
            this.SuspendLayout();
            // 
            // treeView1
            // 
            this.treeView1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.treeView1.Location = new System.Drawing.Point(0, 0);
            this.treeView1.Name = "treeView1";
            this.treeView1.Size = new System.Drawing.Size(504, 307);
            this.treeView1.TabIndex = 0;
            this.treeView1.BeforeExpand += new System.Windows.Forms.TreeViewCancelEventHandler(this.treeView1_BeforeExpand);
            this.treeView1.DoubleClick += new System.EventHandler(this.treeView1_DoubleClick);
            this.treeView1.MouseMove += new System.Windows.Forms.MouseEventHandler(this.treeView1_MouseMove);
            this.treeView1.MouseDown += new System.Windows.Forms.MouseEventHandler(this.treeView1_MouseDown);
            // 
            // runContextMenu
            // 
            this.runContextMenu.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.contextMenuRun,
            this.contextMenuOpenSimulation});
            // 
            // contextMenuRun
            // 
            this.contextMenuRun.Index = 0;
            this.contextMenuRun.Text = "Run Simulation...";
            this.contextMenuRun.Click += new System.EventHandler(this.contextMenuRun_Click);
            // 
            // contextMenuOpenSimulation
            // 
            this.contextMenuOpenSimulation.Index = 1;
            this.contextMenuOpenSimulation.Text = "Open Simulation...";
            this.contextMenuOpenSimulation.Click += new System.EventHandler(this.contextMenuOpenSimulation_Click);
            // 
            // mainMenu1
            // 
            this.mainMenu1.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.menuItem1,
            this.menuHelp});
            // 
            // menuItem1
            // 
            this.menuItem1.Index = 0;
            this.menuItem1.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.menuFileOpenRunfile,
            this.menuItem3,
            this.menuFileExit});
            this.menuItem1.Text = "&File";
            // 
            // menuFileOpenRunfile
            // 
            this.menuFileOpenRunfile.Index = 0;
            this.menuFileOpenRunfile.Shortcut = System.Windows.Forms.Shortcut.CtrlO;
            this.menuFileOpenRunfile.Text = "&Open Run File...";
            this.menuFileOpenRunfile.Click += new System.EventHandler(this.menuFileOpenRunfile_Click);
            // 
            // menuItem3
            // 
            this.menuItem3.Index = 1;
            this.menuItem3.Text = "-";
            // 
            // menuFileExit
            // 
            this.menuFileExit.Index = 2;
            this.menuFileExit.Shortcut = System.Windows.Forms.Shortcut.AltF4;
            this.menuFileExit.Text = "E&xit";
            this.menuFileExit.Click += new System.EventHandler(this.menuFileExit_Click);
            // 
            // menuHelp
            // 
            this.menuHelp.Index = 1;
            this.menuHelp.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.menuHelpFile,
            this.menuHelpAbout});
            this.menuHelp.Text = "Help";
            // 
            // menuHelpFile
            // 
            this.menuHelpFile.Index = 0;
            this.menuHelpFile.Shortcut = System.Windows.Forms.Shortcut.F1;
            this.menuHelpFile.Text = "ExperimentManager Help...";
            this.menuHelpFile.Click += new System.EventHandler(this.menuHelpFile_Click);
            // 
            // menuHelpAbout
            // 
            this.menuHelpAbout.Index = 1;
            this.menuHelpAbout.Text = "About EarLab ExperimentManager...";
            this.menuHelpAbout.Click += new System.EventHandler(this.menuHelpAbout_Click);
            // 
            // toolTip1
            // 
            this.toolTip1.Popup += new System.Windows.Forms.PopupEventHandler(this.toolTip1_Popup);
            // 
            // browseOutputContext
            // 
            this.browseOutputContext.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.menuBrowseViewFolder,
            this.menuBrowseDeleteFolder});
            // 
            // menuBrowseViewFolder
            // 
            this.menuBrowseViewFolder.Index = 0;
            this.menuBrowseViewFolder.Text = "View Folder...";
            this.menuBrowseViewFolder.Click += new System.EventHandler(this.menuBrowseViewFolder_Click);
            // 
            // menuBrowseDeleteFolder
            // 
            this.menuBrowseDeleteFolder.Index = 1;
            this.menuBrowseDeleteFolder.Text = "Delete Folder...";
            this.menuBrowseDeleteFolder.Click += new System.EventHandler(this.menuBrowseDeleteFolder_Click);
            // 
            // timer1
            // 
            this.timer1.Interval = 1000;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.Filter = "XML run files (*.xml)|*.xml|All files (*.*)|*.*";
            // 
            // browseInputContext
            // 
            this.browseInputContext.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.menuItem2});
            // 
            // menuItem2
            // 
            this.menuItem2.Index = 0;
            this.menuItem2.Text = "View Folder...";
            this.menuItem2.Click += new System.EventHandler(this.menuBrowseViewFolder_Click);
            // 
            // TreeView
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.ClientSize = new System.Drawing.Size(504, 313);
            this.Controls.Add(this.treeView1);
            this.Menu = this.mainMenu1;
            this.Name = "TreeView";
            this.Text = "EarLab ExperimentManager";
            this.ResumeLayout(false);

		}
		#endregion

		private void treeView1_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			NodeInfo curInfo;

            System.Diagnostics.Debug.WriteLine("MouseDown");
			switch (e.Button)
			{
				case MouseButtons.Left:
					SelectedNode = treeView1.GetNodeAt(e.X, e.Y);
					break;
				case MouseButtons.Right:
					SelectedNode = treeView1.GetNodeAt(e.X, e.Y);
					if (SelectedNode != null)
					{
						if (SelectedNode.Tag != null)
						{
							curInfo = (NodeInfo)SelectedNode.Tag;
							if (curInfo.ContextMenu != null)
								curInfo.ContextMenu.Show(treeView1, new Point(e.X + 10, e.Y));
						}
					}
					break;
				default:
					break;
			}
		}

		private void treeView1_DoubleClick(object sender, System.EventArgs e)
		{
			NodeInfo curInfo;
			RunRecord CurRunRecord;

            System.Diagnostics.Debug.WriteLine("DoubleClick");
            if (SelectedNode != null)
			{
				if (SelectedNode.Parent != null)
				{
					if (SelectedNode.Tag != null)
					{
						curInfo = (NodeInfo)SelectedNode.Tag;
						switch (curInfo.NodeInfoType)
						{
							case NodeInfoType.RunProgram:
                                try
                                {
                                    if (File.Exists(curInfo.FileName))
                                        System.Diagnostics.Process.Start(curInfo.ProgramName, "\"" + curInfo.FileName + "\"");
                                    else
                                        MessageBox.Show("File does not exist.  Perhaps you need to run the simulation?", "File not found", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                                }
                                catch (Exception ex)
                                {
                                    MessageBox.Show("Error performing requested operation: " + ex.InnerException.ToString());
                                }
								break;
							case NodeInfoType.RunSimulation:
								curInfo = (NodeInfo)SelectedNode.Tag;
								CurRunRecord = curInfo.RunRecord;
								p = System.Diagnostics.Process.Start(curInfo.ProgramName, "-n " + CurRunRecord.FrameCount + " -i \"" + CurRunRecord.InputDirectoryPath + "\" -o \"" + CurRunRecord.OutputDirectoryPath + "\" \"" + CurRunRecord.DiagramFileName + "\" \"" + CurRunRecord.ParameterFileName + "\"");
								p.Exited += new EventHandler(Simulation_Exited);
								break;
							case NodeInfoType.TextViewer:
								if (mEnableSuperuserMode.Value)
								{
									if (File.Exists(curInfo.FileName))
										System.Diagnostics.Process.Start(curInfo.ProgramName, "\"" + curInfo.FileName + "\"");
									else
										MessageBox.Show("File does not exist.  Perhaps you need to run the simulation?", "File not found", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
								}
								else
								{
									if (File.Exists(curInfo.FileName))
										new SimpleTextViewer(curInfo.FileName).Show();
									else
										MessageBox.Show("File does not exist.  Perhaps you need to run the simulation?", "File not found", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
								}
								break;
							default:
								break;
						}
					}
				}
			}
		}

		private void Simulation_Exited(object sender, EventArgs e)
		{
			UpdateFileStatus();
		}

		private void treeView1_MouseMove(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			NodeInfo curInfo;

			HoverNode = treeView1.GetNodeAt(e.X, e.Y);
			if (HoverNode != null)
			{
				curInfo = (NodeInfo)HoverNode.Tag;
                if ((lastNodeInfo == null) || (lastNodeInfo != curInfo))
                {
                    lastNodeInfo = curInfo;
                    if ((curInfo != null) && (curInfo.ToolTip != null) && (curInfo.ToolTip != ""))
                    {
                        if (curInfo.ToolTipType == ToolTipType.FilenameDependent)
                        {
                            if (File.Exists(curInfo.FileName))
                                toolTip1.SetToolTip(treeView1, curInfo.ToolTip);
                            else
                                toolTip1.SetToolTip(treeView1, "File not found");
                        }
                        else
                        {
                            toolTip1.SetToolTip(treeView1, curInfo.ToolTip);
                        }
                        toolTip1.Active = true;
                        return;
                    }
                }
			}
			//toolTip1.Active = false;			
		}

		private void contextMenuRun_Click(object sender, System.EventArgs e)
		{
			NodeInfo curInfo;
			RunRecord curRunRecord;

			if (SelectedNode.Tag != null)
			{
				curInfo = (NodeInfo)SelectedNode.Tag;
				curRunRecord = curInfo.RunRecord;
				System.Diagnostics.Process.Start(curInfo.ProgramName, "-n " + curRunRecord.FrameCount + " -i \"" + curRunRecord.InputDirectoryPath + "\" -o \"" + curRunRecord.OutputDirectoryPath + "\" \"" + curRunRecord.DiagramFileName + "\" \"" + curRunRecord.ParameterFileName + "\"");
			}
		}

		private void contextMenuOpenSimulation_Click(object sender, System.EventArgs e)
		{
			RunRecord CurRunRecord;
			NodeInfo curInfo;

			curInfo = (NodeInfo)SelectedNode.Tag;
			CurRunRecord = curInfo.RunRecord;
			System.Diagnostics.Process.Start(curInfo.ProgramName, "-l -n " + CurRunRecord.FrameCount + " -i \"" + CurRunRecord.InputDirectoryPath + "\" -o \"" + CurRunRecord.OutputDirectoryPath + "\" \"" + CurRunRecord.DiagramFileName + "\" \"" + CurRunRecord.ParameterFileName + "\"");
		}

		private void UpdateFileStatus()
		{
			NodeInfo curInfo;
			for (int CurRun = 0; CurRun < treeView1.Nodes.Count; CurRun++)
			{
				for (int InputFile = 0; InputFile < theRunFile.Run[CurRun].SourceFileNames.Length; InputFile++)
				{
					curInfo = (NodeInfo)treeView1.Nodes[CurRun].Nodes[InputDirectory].Nodes[InputFile].Tag;
					if (File.Exists(curInfo.FileName))
						treeView1.Nodes[CurRun].Nodes[InputDirectory].Nodes[InputFile].ForeColor = Color.Black;
					else
						treeView1.Nodes[CurRun].Nodes[InputDirectory].Nodes[InputFile].ForeColor = Color.LightGray;
				}
				for (int OutputFile = 0; OutputFile < theRunFile.Run[CurRun].OutputFileNames.Length; OutputFile++)
				{
					curInfo = (NodeInfo)treeView1.Nodes[CurRun].Nodes[OutputDirectory].Nodes[OutputFile].Tag;
					if (File.Exists(curInfo.FileName))
					{
						//treeView1.Nodes[CurRun].Nodes[OutputDirectory].Nodes[OutputFile].IsVisible = true;
						treeView1.Nodes[CurRun].Nodes[OutputDirectory].Nodes[OutputFile].ForeColor = Color.Black;
					}
					else
					{
						//treeView1.Nodes[CurRun].Nodes[OutputDirectory].Nodes[OutputFile].IsVisible = false;
						treeView1.Nodes[CurRun].Nodes[OutputDirectory].Nodes[OutputFile].ForeColor = Color.LightGray;
					}
				}
			}
		}

		private void timer1_Tick(object sender, System.EventArgs e)
		{
			UpdateFileStatus();
		}

		private void treeView1_BeforeExpand(object sender, System.Windows.Forms.TreeViewCancelEventArgs e)
		{
			NodeInfo curInfo = (NodeInfo)e.Node.Tag;
			for (int Subnodes = 0; Subnodes < e.Node.Nodes.Count; Subnodes++)
			{
				curInfo = (NodeInfo)e.Node.Nodes[Subnodes].Tag;
				if ((curInfo != null) && (curInfo.ToolTipType == ToolTipType.FilenameDependent))
				{
					if (File.Exists(curInfo.FileName))
						e.Node.Nodes[Subnodes].ForeColor = Color.Black;
					else
						e.Node.Nodes[Subnodes].ForeColor = Color.LightGray;
				}
			}
		}

		private void menuFileExit_Click(object sender, System.EventArgs e)
		{
			Application.Exit();
		}

		private void menuFileOpenRunfile_Click(object sender, System.EventArgs e)
		{
			if (openFileDialog1.ShowDialog() == DialogResult.OK)
			{
				RunFile = openFileDialog1.FileName;
			}
		}

		private void menuBrowseViewFolder_Click(object sender, System.EventArgs e)
		{
			NodeInfo curInfo;
			RunRecord curRunRecord;

			if (SelectedNode.Tag != null)
			{
				curInfo = (NodeInfo)SelectedNode.Tag;
				curRunRecord = curInfo.RunRecord;
				System.Diagnostics.Process.Start(curInfo.ProgramName, curInfo.FileName);
			}
		}

		private void menuBrowseDeleteFolder_Click(object sender, System.EventArgs e)
		{
			NodeInfo curInfo;
			RunRecord curRunRecord;

			if (SelectedNode.Tag != null)
			{
				if (MessageBox.Show("Really delete this output folder?  You will have to run the simulation again to re-create the files.", "Confirm folder deletion", MessageBoxButtons.YesNo, MessageBoxIcon.Warning, MessageBoxDefaultButton.Button2) == DialogResult.Yes)
				{
					curInfo = (NodeInfo)SelectedNode.Tag;
					curRunRecord = curInfo.RunRecord;
					Directory.Delete(curInfo.FileName, true);
					Directory.CreateDirectory(curInfo.FileName);
					UpdateFileStatus();
				}
			}
		}

		private void menuHelpFile_Click(object sender, System.EventArgs e)
		{
			string HelpPath;

			HelpPath = Path.GetDirectoryName(Application.ExecutablePath);
			HelpPath = Path.Combine(HelpPath, "DesktopEarLab.chm");
			Help.ShowHelp(this, HelpPath);
		}

		private void menuHelpAbout_Click(object sender, System.EventArgs e)
		{
			About AboutForm = new About();

			AboutForm.ShowDialog(this);
		}

        private void toolTip1_Popup(object sender, PopupEventArgs e)
        {

        }
	}
}
