using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Reflection;
using System.Windows.Forms;
using System.Data;
using Microsoft.Win32;
using System.Threading;
using System.IO;
using System.Diagnostics;
using System.Xml;

namespace EarLab
{
    public partial class Form1 : Form
    {
        private RegistryKey mModuleKey;
        private RegistryKey mApplicationKey;
        private RegistryString mModuleDirectory;
        private RegistryString mInputDirectory;
        private RegistryString mOutputDirectory;
        private RegistryString mDiagramFile;
        private RegistryString mParameterFile;
        private RegistryInt mFrameCount;
        private RegistryFormWindowState mWindowState;
        private RegistryPoint mWindowLocation;
        private RegistrySize mWindowSize;
        private RegistryBool mEnableSuperuserMode;

        private Process mExecutionProcess;
        private Thread mExecutionThread;
        private bool mRunning, RunningFromCommandLineInput, LoadAndWait;
        private LogCallback mLogCallback;

        private static Form1 mMainForm;

        public Form1(string [] args)
        {
            try
            {
                mMainForm = this;
                mModuleKey = Registry.CurrentUser.CreateSubKey(@"Software\EarLab");
                mApplicationKey = Registry.CurrentUser.CreateSubKey(@"Software\EarLab\EarLab GUI");
                mLogCallback = new LogCallback(LogCallback);
                mModuleDirectory = new RegistryString(mModuleKey, "ModulesPath", Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles), @"EarLab\Modules"));
                if (!Directory.Exists(mModuleDirectory.Value))
                {
                    mModuleDirectory.Value = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles), @"EarLab\Modules");
                    if (!Directory.Exists(mModuleDirectory.Value)) MessageBox.Show(@"Module directory not found", @"Simulator error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
                mInputDirectory = new RegistryString(mApplicationKey, "InputDirectory", System.Environment.ExpandEnvironmentVariables("%USERPROFILE%") + @"\My Documents\EarLab\Signals");
                mOutputDirectory = new RegistryString(mApplicationKey, "OutputDirectory", System.Environment.ExpandEnvironmentVariables("%USERPROFILE%") + @"\My Documents\EarLab\Output");
                mDiagramFile = new RegistryString(mApplicationKey, "RunParameterFile", null);
                mParameterFile = new RegistryString(mApplicationKey, "ModuleParameterFile", null);
                mFrameCount = new RegistryInt(mApplicationKey, "FrameCount", 1);
#if DEBUG
                mEnableSuperuserMode = new RegistryBool(mApplicationKey, "SuperuserMode", true);
#else
                mEnableSuperuserMode = new RegistryBool(mApplicationKey, "SuperuserMode", false);
#endif
                mWindowState = new RegistryFormWindowState(mApplicationKey, "WindowState", FormWindowState.Normal);
                mWindowLocation = new RegistryPoint(mApplicationKey, "WindowLocation", this.Location);
                mWindowSize = new RegistrySize(mApplicationKey, "WindowSize", this.MinimumSize);
            }
            catch (Exception e)
            {
                MessageBox.Show("Exception caught while creating application registry keys: " + e.ToString());
            }

            try
            {
                InitializeComponent();
            }
            catch (Exception e)
            {
                MessageBox.Show("Exception caught during call to InitializeComponent: " + e.ToString());
            }

            udFrameCount.Value = mFrameCount.Value;

            this.Location = mWindowLocation.Value;
            this.Size = mWindowSize.Value;
            this.WindowState = mWindowState.Value;
            CheckForIllegalCrossThreadCalls = false;

            if (mEnableSuperuserMode.Value)
            {
                menuEnvironment.Enabled = true;
                menuEnvironment.Visible = true;
                menuFileOpenDiagram.Enabled = true;
                menuFileOpenDiagram.Visible = true;
                menuFileOpenParameters.Enabled = true;
                menuFileOpenParameters.Visible = true;
                menuFileSeparator.Enabled = true;
                menuFileSeparator.Visible = true;
            }
            else
            {
                menuEnvironment.Enabled = false;
                menuEnvironment.Visible = false;
                menuFileOpenDiagram.Enabled = false;
                menuFileOpenDiagram.Visible = false;
                menuFileOpenParameters.Enabled = false;
                menuFileOpenParameters.Visible = false;
                menuFileSeparator.Enabled = false;
                menuFileSeparator.Visible = false;
            }

            if ((args != null) && (args.Length > 0))
            {
                if (args.Length == 1)
                {
                    // This should be a file name for us to open and parse
                    ReadSimulationFile(args[0]);
                }
                else
                {
                    bool dashN = false;	// -n <FrameCount>
                    bool dashI = false;	// -i <InputDirectory>
                    bool dashO = false;	// -o <OutputDirectory>
                    bool DiagramFileSpecified = false;
                    bool ParameterFileSpecified = false;
                    RunningFromCommandLineInput = false;
                    try
                    {
                        for (int i = 0; i < args.Length; i++)
                        {
                            switch (args[i])
                            {
                                case "-n":
                                    try
                                    {
                                        FrameCount = int.Parse(args[++i]);
                                        dashN = true;
                                    }
                                    catch (FormatException)
                                    {
                                        throw new ApplicationException("Frame count (-n) must be a non-zero positive integer.  \"" + args[i] + "\" is invalid");
                                    }
                                    break;
                                case "-m":
                                    ModuleDirectory = args[++i];
                                    break;
                                case "-i":
                                    InputDirectory = args[++i];
                                    dashI = true;
                                    break;
                                case "-o":
                                    OutputDirectory = args[++i];
                                    dashO = true;
                                    break;
                                case "-l":
                                    LoadAndWait = true;
                                    break;
                                default:
                                    if (!DiagramFileSpecified)
                                    {
                                        DiagramFile = args[i];
                                        DiagramFileSpecified = true;
                                    }
                                    else
                                    {
                                        if (!ParameterFileSpecified)
                                        {
                                            ParameterFile = args[i];
                                            ParameterFileSpecified = true;
                                        }
                                        else
                                        {
                                            throw new ApplicationException("Too many parameters specified");
                                        }
                                    }
                                    break;
                            } // switch
                        } // for
                        if (dashN && dashI && dashO && DiagramFileSpecified && ParameterFileSpecified)
                            RunningFromCommandLineInput = true;
                    } // try
                    catch (ApplicationException e)
                    {
                        string Message;
                        Message = e.Message + "\n";
                        Message += "Usage: EarLabGUI -n <NumFrames> [-m <ModuleDirectory>] [-i <InputDirectory>] [-o <OutputDirectory>]\n";
                        Message += "                                        <DiagramFile> <ParameterFile>\n";
                        Message += "\n";
                        Message += "Where: <NumFrames> is a positive integer which is the number of frames over\n";
                        Message += "                   which the simulation will be run\n";
                        Message += "       <ModuleDirectory> is a directory path that contains the module executables\n";
                        Message += "                       that will be used for this simulation.  If this option is not provided,\n";
                        Message += "                       the module directory defaults to the directory into which EarLab was installed.\n";
                        Message += "       <InputDirectory> is a directory path that contains the input files for the model.\n";
                        Message += "       <OutputDirectory> is a directory path that will contain the output files produced by the model.\n";
                        Message += "       <DiagramFile> is a file that describes an Earlab model\n";
                        Message += "       <ParameterFile> is a parameter file that contains parameter definitions\n";
                        Message += "                       for all modules specified by the <DiagramFile>.\n";
                        System.Windows.Forms.MessageBox.Show(Message);
                    }
                }
            }
            else
                RunningFromCommandLineInput = false;

            if (LoadAndWait)
                RunningFromCommandLineInput = false;

            udFrameCount.Focus();

            UpdateStatusDisplay();
            if (RunningFromCommandLineInput)
                timerCommandlineRun.Enabled = true;
        }

        static void LogCallback(String Message)
        {
            mMainForm.Log(Message);
        }

        void Log(string Message)
        {
            logDisplay.AppendText(DateTime.Now.ToString() + " " + Message + "\r\n");
        }

        void ClearLog()
        {
            logDisplay.Text = "";
        }

        private void RunModel()
        {
            string CurLine;
            string[] Fields;
            int Frame = 0;
            string Delims = " ";
            char[] FieldDelims = Delims.ToCharArray();
            bool Once = true;

            btnRun.Enabled = false;
            btnStop.Enabled = true;
            udFrameCount.Enabled = false;
            menuModelRun.Enabled = false;

            mRunning = false;
            sbTextPanel.Text = "Running";
            Log("Starting simulation");
            Log("Current path is: " + Directory.GetCurrentDirectory());

            Log("Module directory: \"" + ModuleDirectory + "\"");
            Log("Input directory: \"" + InputDirectory + "\"");
            Log("Output directory: \"" + OutputDirectory + "\"");
            Log("Model configuration: \"" + DiagramFile + "\"");
            Log("Module parameters: \"" + ParameterFile + "\"");
            Log("Beginning simulation\n");
            while (true)
            {
                CurLine = mExecutionProcess.StandardOutput.ReadLine();
                if ((CurLine != null) && (CurLine.IndexOf("Error") > 0))
                    Log(CurLine);
                else
                {
                    if ((CurLine != null) && (CurLine.StartsWith("  Starting Frame ")))
                    {
                        Fields = CurLine.Split(FieldDelims);
                        Frame = Int32.Parse(Fields[Fields.Length - 1]);
                        progressBar.Value = Frame;
                        sbTextPanel.Text = "Running: Frame " + Frame.ToString() + "/" + mFrameCount.Value.ToString();
                    }
                }
                if (Frame < 2)
                {
                    Log(CurLine);
                }
                else
                {
                    if (Once)
                    {
                        Once = false;
                        Log("Simulation running, please wait");
                    }
                }
                if (mExecutionProcess.HasExited)
                    break;
            }
            progressBar.Value = progressBar.Maximum;
            progressBar.Visible = false;
            if (mExecutionProcess.ExitCode != 0)
                Log("Simulation completed with errors");
            else
                Log("Simulation complete");
            Log("Ready");

            btnRun.Enabled = true;
            btnStop.Enabled = false;
            btnAbort.Enabled = false;
            udFrameCount.Enabled = true;
            UpdateStatusDisplay();
            udFrameCount.Focus();
            mRunning = false;
            menuModelRun.Enabled = true;
            if (RunningFromCommandLineInput)
                Application.Exit();
        }

        public int FrameCount
        {
            set
            {
                if (value <= 0)
                    throw new FormatException("Frame count must be non-zero positive integer");
                mFrameCount.Value = value;
                udFrameCount.Value = mFrameCount.Value;
                UpdateStatusDisplay();
            }
            get { return mFrameCount.Value; }
        }

        public string ModuleDirectory
        {
            set
            {
                if (!Directory.Exists(value))
                    throw new ApplicationException("Specified module directory \"" + value + "\" does not exist");
                mModuleDirectory.Value = value;
                UpdateStatusDisplay();
            }
            get { return mModuleDirectory.Value; }
        }

        public string InputDirectory
        {
            set
            {
                if (!Directory.Exists(value))
                    throw new ApplicationException("Specified input directory \"" + value + "\" does not exist");
                mInputDirectory.Value = value;
                UpdateStatusDisplay();
            }
            get { return mInputDirectory.Value; }
        }

        public string OutputDirectory
        {
            set
            {
                mOutputDirectory.Value = value;
                UpdateStatusDisplay();
            }
            get { return mOutputDirectory.Value; }
        }

        public string DiagramFile
        {
            set
            {
                if (!File.Exists(value))
                    throw new ApplicationException("Specified diagram file \"" + value + "\" does not exist");
                mDiagramFile.Value = value;
                UpdateStatusDisplay();
            }
            get { return mDiagramFile.Value; }
        }

        public string ParameterFile
        {
            set
            {
                if (!File.Exists(value))
                    throw new ApplicationException("Specified parameter file \"" + value + "\" does not exist");
                mParameterFile.Value = value;
                UpdateStatusDisplay();
            }
            get { return mParameterFile.Value; }
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

			if (mInputDirectory.Value != null)
				MyLines[LineCount++] = "Input Directory is \"" + mInputDirectory.Value + "\"";
			else
			{
				MyLines[LineCount++] = "Input Directory is not set";
				SimOK = false;
			}

			if (mOutputDirectory.Value != null)
				MyLines[LineCount++] = "Output Directory is \"" + mOutputDirectory.Value + "\"";
			else
			{
				MyLines[LineCount++] = "Output Directory is not set";
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

        private void udFrameCount_ValueChanged(object sender, EventArgs e)
        {
            mFrameCount.Value = (int)udFrameCount.Value;
            UpdateStatusDisplay();
        }

        private void DoRun()
        {
            string CurPath;

            CurPath = Path.Combine(Path.GetDirectoryName(Application.ExecutablePath), "DesktopEarlab.exe");
            if (File.Exists(CurPath))
            {
                mExecutionProcess = new Process();
                mExecutionProcess.StartInfo.WorkingDirectory = Path.GetDirectoryName(Application.ExecutablePath); 
            }
            else
            {
                CurPath = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles), @"EarLab\Desktop EarLab\DesktopEarlab.exe");
                if (File.Exists(CurPath))
                {
                    mExecutionProcess = new Process();
                    mExecutionProcess.StartInfo.WorkingDirectory = Path.GetDirectoryName(CurPath);
                }
                else
                {
                    MessageBox.Show("Could not find necessary simulation environment file \"DesktopEarlab.exe\".  Please reinstall the Desktop EarLab environment and try again.", "Error running simulation", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }
            }
            mExecutionProcess.StartInfo.FileName = CurPath;
            mExecutionProcess.StartInfo.Arguments = "-n " + FrameCount.ToString() + " -m \"" + ModuleDirectory + "\" -i \"" + InputDirectory + "\" -o \"" + OutputDirectory + "\" \"" + DiagramFile + "\" \"" + ParameterFile + "\"";

            ClearLog();
            if (mEnableSuperuserMode.Value)
                Log("Running command: \"" + CurPath + "\" " + mExecutionProcess.StartInfo.Arguments);
            mExecutionProcess.StartInfo.RedirectStandardInput = true;
            mExecutionProcess.StartInfo.RedirectStandardOutput = true;
            mExecutionProcess.StartInfo.UseShellExecute = false;
            mExecutionProcess.StartInfo.CreateNoWindow = true;
            mExecutionProcess.Start();
            mExecutionThread = new Thread(new ThreadStart(RunModel));
            mExecutionThread.IsBackground = true;
            mExecutionThread.Name = "Model Execution Thread";
            progressBar.Value = 0;
            progressBar.Visible = true;
            progressBar.Minimum = 0;
            progressBar.Maximum = mFrameCount.Value + 1;
            mExecutionThread.Priority = ThreadPriority.BelowNormal;
            mExecutionThread.Start();
        }

        private void btnRun_Click(object sender, EventArgs e)
        {
            DoRun();
        }

        private void btnStop_Click(object sender, EventArgs e)
        {
            btnAbort.Enabled = true;
            StopModel();
        }

        private void btnAbort_Click(object sender, EventArgs e)
        {
            AbortModel();
        }

        private void StopModel()
        {
            sbTextPanel.Text = "Stopping (User request)";
            if ((mExecutionProcess != null) && (!mExecutionProcess.HasExited))
            {
                mExecutionProcess.StandardInput.Write("\003");
                mExecutionProcess.CloseMainWindow();
                mExecutionProcess.Kill();
                Log("User requested stop");
            }
        }

        private void logDisplay_TextChanged(object sender, EventArgs e)
        {
            logDisplay.Focus();
            logDisplay.SelectionStart = logDisplay.TextLength;
            logDisplay.ScrollToCaret();
        }

        private void Form1_LocationChanged(object sender, EventArgs e)
        {
            if (this.WindowState == FormWindowState.Normal)
                mWindowLocation.Value = this.Location;
            mWindowState.Value = this.WindowState;
        }

        private void Form1_Resize(object sender, EventArgs e)
        {
            if (this.WindowState == FormWindowState.Normal)
            {
                mWindowSize.Value = this.Size;
                txtStatusDisplay.Width = this.ClientSize.Width - 16;
                logDisplay.Width = this.ClientSize.Width - 16;
                logDisplay.Height = (this.ClientSize.Height - 24) - logDisplay.Top;
                progressBar.Width = statusBar.Width - 220;

            }
            mWindowState.Value = this.WindowState;
        } // private void Form1_Resize

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
                sbTextPanel.Text = "Aborted";
            } // if (mExecutionThread.IsAlive)
            btnAbort.Enabled = false;
        }

        private void menuFileExit_Click(object sender, EventArgs e)
        {
            StopModelAndWait();
            Application.Exit();
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            StopModelAndWait();
        }

        private void menuModelChooseDiagramFile_Click(object sender, EventArgs e)
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

        private void menuModelChooseParameterFile_Click(object sender, EventArgs e)
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

        private void menuModelSetInputDirectory_Click(object sender, EventArgs e)
        {
            folderBrowser.Reset();
            if (mModuleDirectory != null)
                folderBrowser.SelectedPath = mInputDirectory.Value;
            folderBrowser.ShowNewFolderButton = false;
            folderBrowser.Description = "Choose the directory that holds your input files";
            if (folderBrowser.ShowDialog() == DialogResult.OK)
            {
                mInputDirectory.Value = folderBrowser.SelectedPath;
                UpdateStatusDisplay();
            }
        }

        private void menuModelSetOutputDirectory_Click(object sender, EventArgs e)
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

        private void menuEnvironmentSetModuleDirectory_Click(object sender, EventArgs e)
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

        private void menuHelpAbout_Click(object sender, EventArgs e)
        {
            Form About = new About();

            About.ShowDialog(this);
        }

        private void menuHelpDesktopEarLab_Click(object sender, EventArgs e)
        {
            string HelpPath;

            HelpPath = Path.GetDirectoryName(Application.ExecutablePath);
            HelpPath = Path.Combine(HelpPath, "DesktopEarLab.chm");
            Help.ShowHelp(this, HelpPath);
        }

        private void ReadSimulationFile(string FileName)
        {
            XmlDocument XMLDoc = new XmlDocument();
            NameTable nt = new NameTable();
            object DiagramFile = nt.Add("DiagramFileName");
            object ParameterFile = nt.Add("ParameterFileName");
            object InputDirectory = nt.Add("InputDirectoryPath");
            object OutputDirectory = nt.Add("OutputDirectoryPath");
            object FrameCount = nt.Add("FrameCount");

            // Create a reader that uses the NameTable.
            XmlReaderSettings settings = new XmlReaderSettings();
            settings.NameTable = nt;
            settings.ConformanceLevel = ConformanceLevel.Fragment;
            using (StreamReader StreamReader = new StreamReader(FileName))
            {
                // Create the XmlParserContext.
                XmlParserContext context = new XmlParserContext(nt, null, null, XmlSpace.None);
                XmlReader reader = XmlReader.Create(StreamReader, settings, context);

                while (reader.Read())
                {
                    if (reader.NodeType == XmlNodeType.Element)
                    {
                        // Cache the local name to prevent multiple calls to the LocalName property.
                        object localname = reader.LocalName;

                        if (DiagramFile == localname)
                            this.DiagramFile = reader.ReadString();
                        if (ParameterFile == localname)
                            this.ParameterFile = reader.ReadString();
                        if (InputDirectory == localname)
                            this.InputDirectory = reader.ReadString();
                        if (OutputDirectory == localname)
                            this.OutputDirectory = reader.ReadString();
                        if (FrameCount == localname)
                            this.FrameCount = int.Parse(reader.ReadString());
                    }
                }  // End While
                // Close the reader.
                reader.Close();
            } // end Using

        }

        private void WriteSimulationFile(string FileName)
        {
            using (StreamWriter theFile = new StreamWriter(FileName))
            {
                string XML;

                XML = "<Run name=\"\" description=\"\">\n";
                XML += "  <DiagramFileName>" + DiagramFile + "</DiagramFileName>\n";
                XML += "  <ParameterFileName>" + ParameterFile + "</ParameterFileName>\n";
                XML += "  <InputDirectoryPath>" + InputDirectory + "</InputDirectoryPath>\n";
                XML += "  <OutputDirectoryPath>" + OutputDirectory + "</OutputDirectoryPath>\n";
                XML += "  <FrameCount>" + FrameCount + "</FrameCount>\n";
                XML += "</Run>\n";
                theFile.Write(XML);
                theFile.Close();
            }
        }

        private void menuFileSave_Click(object sender, EventArgs e)
        {
            saveFileDialog.FileName = mParameterFile.Value;
            saveFileDialog.Title = "Save EarLab simulation file";

            saveFileDialog.FileName = "";
            saveFileDialog.CheckFileExists = false;
            saveFileDialog.CheckPathExists = true;
            saveFileDialog.Filter = "EarLab simulation files (*.earlab)|*.earlab|All files (*.*)|*.*";
            saveFileDialog.FilterIndex = 0;
            saveFileDialog.RestoreDirectory = true;
            saveFileDialog.ValidateNames = true;
            if (saveFileDialog.ShowDialog() == DialogResult.OK)
            {
                WriteSimulationFile(saveFileDialog.FileName);
            }
        }

        private void menuFileLoad_Click(object sender, EventArgs e)
        {
            openFileDialog.FileName = mParameterFile.Value;
            openFileDialog.Title = "Choose EarLab simulation file";

            openFileDialog.FileName = "";
            openFileDialog.CheckFileExists = true;
            openFileDialog.CheckPathExists = true;
            openFileDialog.Filter = "EarLab simulation files (*.earlab)|*.earlab|All files (*.*)|*.*";
            openFileDialog.FilterIndex = 0;
            openFileDialog.Multiselect = false;
            openFileDialog.RestoreDirectory = true;
            openFileDialog.ValidateNames = true;
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                ReadSimulationFile(openFileDialog.FileName);
            }
        }

        private void timerCommandlineRun_Tick(object sender, EventArgs e)
        {
            timerCommandlineRun.Enabled = false;
            DoRun();
        }

        private void logDisplay_KeyUp(object sender, KeyEventArgs e)
        {
            if ((e.KeyCode == Keys.C) && (e.Control))
                Clipboard.SetText(logDisplay.SelectedText);
        }
    } // public partial class Form1
}