namespace EarLab
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.udFrameCount = new System.Windows.Forms.NumericUpDown();
            this.label1 = new System.Windows.Forms.Label();
            this.btnRun = new System.Windows.Forms.Button();
            this.btnStop = new System.Windows.Forms.Button();
            this.btnAbort = new System.Windows.Forms.Button();
            this.logDisplay = new System.Windows.Forms.RichTextBox();
            this.statusBar = new System.Windows.Forms.StatusStrip();
            this.sbTextPanel = new System.Windows.Forms.ToolStripStatusLabel();
            this.progressBar = new System.Windows.Forms.ToolStripProgressBar();
            this.txtStatusDisplay = new System.Windows.Forms.TextBox();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.menuFileSave = new System.Windows.Forms.ToolStripMenuItem();
            this.menuFileLoad = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.menuFileOpenDiagram = new System.Windows.Forms.ToolStripMenuItem();
            this.menuFileOpenParameters = new System.Windows.Forms.ToolStripMenuItem();
            this.menuFileSeparator = new System.Windows.Forms.ToolStripSeparator();
            this.menuFileExit = new System.Windows.Forms.ToolStripMenuItem();
            this.modelToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.menuModelChooseDiagramFile = new System.Windows.Forms.ToolStripMenuItem();
            this.menuModelChooseParameterFile = new System.Windows.Forms.ToolStripMenuItem();
            this.menuModelSetInputDirectory = new System.Windows.Forms.ToolStripMenuItem();
            this.menuModelSetOutputDirectory = new System.Windows.Forms.ToolStripMenuItem();
            this.menuModelRun = new System.Windows.Forms.ToolStripMenuItem();
            this.menuEnvironment = new System.Windows.Forms.ToolStripMenuItem();
            this.menuEnvironmentSetModuleDirectory = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.menuHelpDesktopEarLab = new System.Windows.Forms.ToolStripMenuItem();
            this.menuHelpAbout = new System.Windows.Forms.ToolStripMenuItem();
            this.timerCommandlineRun = new System.Windows.Forms.Timer(this.components);
            this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.folderBrowser = new System.Windows.Forms.FolderBrowserDialog();
            this.saveFileDialog = new System.Windows.Forms.SaveFileDialog();
            ((System.ComponentModel.ISupportInitialize)(this.udFrameCount)).BeginInit();
            this.statusBar.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // udFrameCount
            // 
            this.udFrameCount.Location = new System.Drawing.Point(12, 120);
            this.udFrameCount.Maximum = new decimal(new int[] {
            1000000000,
            0,
            0,
            0});
            this.udFrameCount.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.udFrameCount.Name = "udFrameCount";
            this.udFrameCount.Size = new System.Drawing.Size(66, 20);
            this.udFrameCount.TabIndex = 1;
            this.udFrameCount.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.udFrameCount.ValueChanged += new System.EventHandler(this.udFrameCount_ValueChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 104);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(66, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Frame count";
            // 
            // btnRun
            // 
            this.btnRun.Location = new System.Drawing.Point(84, 117);
            this.btnRun.Name = "btnRun";
            this.btnRun.Size = new System.Drawing.Size(75, 23);
            this.btnRun.TabIndex = 3;
            this.btnRun.Text = "Run";
            this.btnRun.UseVisualStyleBackColor = true;
            this.btnRun.Click += new System.EventHandler(this.btnRun_Click);
            // 
            // btnStop
            // 
            this.btnStop.Location = new System.Drawing.Point(165, 117);
            this.btnStop.Name = "btnStop";
            this.btnStop.Size = new System.Drawing.Size(75, 23);
            this.btnStop.TabIndex = 4;
            this.btnStop.Text = "Stop";
            this.btnStop.UseVisualStyleBackColor = true;
            this.btnStop.Click += new System.EventHandler(this.btnStop_Click);
            // 
            // btnAbort
            // 
            this.btnAbort.Location = new System.Drawing.Point(246, 117);
            this.btnAbort.Name = "btnAbort";
            this.btnAbort.Size = new System.Drawing.Size(75, 23);
            this.btnAbort.TabIndex = 5;
            this.btnAbort.Text = "Abort";
            this.btnAbort.UseVisualStyleBackColor = true;
            this.btnAbort.Click += new System.EventHandler(this.btnAbort_Click);
            // 
            // logDisplay
            // 
            this.logDisplay.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.logDisplay.BackColor = System.Drawing.SystemColors.Control;
            this.logDisplay.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.logDisplay.Location = new System.Drawing.Point(12, 146);
            this.logDisplay.Name = "logDisplay";
            this.logDisplay.ReadOnly = true;
            this.logDisplay.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.Vertical;
            this.logDisplay.Size = new System.Drawing.Size(434, 227);
            this.logDisplay.TabIndex = 6;
            this.logDisplay.Text = "";
            this.logDisplay.KeyUp += new System.Windows.Forms.KeyEventHandler(this.logDisplay_KeyUp);
            this.logDisplay.TextChanged += new System.EventHandler(this.logDisplay_TextChanged);
            // 
            // statusBar
            // 
            this.statusBar.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.sbTextPanel,
            this.progressBar});
            this.statusBar.Location = new System.Drawing.Point(0, 386);
            this.statusBar.Name = "statusBar";
            this.statusBar.Size = new System.Drawing.Size(458, 22);
            this.statusBar.TabIndex = 7;
            this.statusBar.Text = "statusStrip1";
            // 
            // sbTextPanel
            // 
            this.sbTextPanel.AutoSize = false;
            this.sbTextPanel.BorderSides = System.Windows.Forms.ToolStripStatusLabelBorderSides.Right;
            this.sbTextPanel.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this.sbTextPanel.Name = "sbTextPanel";
            this.sbTextPanel.Size = new System.Drawing.Size(200, 17);
            this.sbTextPanel.Text = "sbTextPanel";
            // 
            // progressBar
            // 
            this.progressBar.AutoSize = false;
            this.progressBar.Name = "progressBar";
            this.progressBar.Size = new System.Drawing.Size(100, 16);
            this.progressBar.Visible = false;
            // 
            // txtStatusDisplay
            // 
            this.txtStatusDisplay.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txtStatusDisplay.Location = new System.Drawing.Point(15, 28);
            this.txtStatusDisplay.Multiline = true;
            this.txtStatusDisplay.Name = "txtStatusDisplay";
            this.txtStatusDisplay.ReadOnly = true;
            this.txtStatusDisplay.Size = new System.Drawing.Size(434, 73);
            this.txtStatusDisplay.TabIndex = 8;
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.modelToolStripMenuItem,
            this.menuEnvironment,
            this.helpToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(458, 24);
            this.menuStrip1.TabIndex = 9;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menuFileSave,
            this.menuFileLoad,
            this.toolStripMenuItem1,
            this.menuFileOpenDiagram,
            this.menuFileOpenParameters,
            this.menuFileSeparator,
            this.menuFileExit});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(35, 20);
            this.fileToolStripMenuItem.Text = "&File";
            // 
            // menuFileSave
            // 
            this.menuFileSave.Name = "menuFileSave";
            this.menuFileSave.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
            this.menuFileSave.Size = new System.Drawing.Size(209, 22);
            this.menuFileSave.Text = "&Save simulation...";
            this.menuFileSave.Click += new System.EventHandler(this.menuFileSave_Click);
            // 
            // menuFileLoad
            // 
            this.menuFileLoad.Name = "menuFileLoad";
            this.menuFileLoad.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.L)));
            this.menuFileLoad.Size = new System.Drawing.Size(209, 22);
            this.menuFileLoad.Text = "Load simulation...";
            this.menuFileLoad.Click += new System.EventHandler(this.menuFileLoad_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(206, 6);
            // 
            // menuFileOpenDiagram
            // 
            this.menuFileOpenDiagram.Name = "menuFileOpenDiagram";
            this.menuFileOpenDiagram.Size = new System.Drawing.Size(209, 22);
            this.menuFileOpenDiagram.Text = "Open &Diagram File...";
            // 
            // menuFileOpenParameters
            // 
            this.menuFileOpenParameters.Name = "menuFileOpenParameters";
            this.menuFileOpenParameters.Size = new System.Drawing.Size(209, 22);
            this.menuFileOpenParameters.Text = "Open &Parameter File...";
            // 
            // menuFileSeparator
            // 
            this.menuFileSeparator.Name = "menuFileSeparator";
            this.menuFileSeparator.Size = new System.Drawing.Size(206, 6);
            // 
            // menuFileExit
            // 
            this.menuFileExit.Name = "menuFileExit";
            this.menuFileExit.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Alt | System.Windows.Forms.Keys.F4)));
            this.menuFileExit.Size = new System.Drawing.Size(209, 22);
            this.menuFileExit.Text = "E&xit";
            this.menuFileExit.Click += new System.EventHandler(this.menuFileExit_Click);
            // 
            // modelToolStripMenuItem
            // 
            this.modelToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menuModelChooseDiagramFile,
            this.menuModelChooseParameterFile,
            this.menuModelSetInputDirectory,
            this.menuModelSetOutputDirectory,
            this.menuModelRun});
            this.modelToolStripMenuItem.Name = "modelToolStripMenuItem";
            this.modelToolStripMenuItem.Size = new System.Drawing.Size(47, 20);
            this.modelToolStripMenuItem.Text = "&Model";
            // 
            // menuModelChooseDiagramFile
            // 
            this.menuModelChooseDiagramFile.Name = "menuModelChooseDiagramFile";
            this.menuModelChooseDiagramFile.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.D)));
            this.menuModelChooseDiagramFile.Size = new System.Drawing.Size(241, 22);
            this.menuModelChooseDiagramFile.Text = "Choose &Diagram file...";
            this.menuModelChooseDiagramFile.Click += new System.EventHandler(this.menuModelChooseDiagramFile_Click);
            // 
            // menuModelChooseParameterFile
            // 
            this.menuModelChooseParameterFile.Name = "menuModelChooseParameterFile";
            this.menuModelChooseParameterFile.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.P)));
            this.menuModelChooseParameterFile.Size = new System.Drawing.Size(241, 22);
            this.menuModelChooseParameterFile.Text = "Choose &Parameter file...";
            this.menuModelChooseParameterFile.Click += new System.EventHandler(this.menuModelChooseParameterFile_Click);
            // 
            // menuModelSetInputDirectory
            // 
            this.menuModelSetInputDirectory.Name = "menuModelSetInputDirectory";
            this.menuModelSetInputDirectory.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.I)));
            this.menuModelSetInputDirectory.Size = new System.Drawing.Size(241, 22);
            this.menuModelSetInputDirectory.Text = "Set &Input directory...";
            this.menuModelSetInputDirectory.Click += new System.EventHandler(this.menuModelSetInputDirectory_Click);
            // 
            // menuModelSetOutputDirectory
            // 
            this.menuModelSetOutputDirectory.Name = "menuModelSetOutputDirectory";
            this.menuModelSetOutputDirectory.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.O)));
            this.menuModelSetOutputDirectory.Size = new System.Drawing.Size(241, 22);
            this.menuModelSetOutputDirectory.Text = "Set &Output directory...";
            this.menuModelSetOutputDirectory.Click += new System.EventHandler(this.menuModelSetOutputDirectory_Click);
            // 
            // menuModelRun
            // 
            this.menuModelRun.Name = "menuModelRun";
            this.menuModelRun.ShortcutKeys = System.Windows.Forms.Keys.F5;
            this.menuModelRun.Size = new System.Drawing.Size(241, 22);
            this.menuModelRun.Text = "Run Model";
            // 
            // menuEnvironment
            // 
            this.menuEnvironment.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menuEnvironmentSetModuleDirectory});
            this.menuEnvironment.Name = "menuEnvironment";
            this.menuEnvironment.Size = new System.Drawing.Size(79, 20);
            this.menuEnvironment.Text = "&Environment";
            // 
            // menuEnvironmentSetModuleDirectory
            // 
            this.menuEnvironmentSetModuleDirectory.Name = "menuEnvironmentSetModuleDirectory";
            this.menuEnvironmentSetModuleDirectory.Size = new System.Drawing.Size(196, 22);
            this.menuEnvironmentSetModuleDirectory.Text = "Set module directory...";
            this.menuEnvironmentSetModuleDirectory.Click += new System.EventHandler(this.menuEnvironmentSetModuleDirectory_Click);
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menuHelpDesktopEarLab,
            this.menuHelpAbout});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(40, 20);
            this.helpToolStripMenuItem.Text = "&Help";
            // 
            // menuHelpDesktopEarLab
            // 
            this.menuHelpDesktopEarLab.Name = "menuHelpDesktopEarLab";
            this.menuHelpDesktopEarLab.Size = new System.Drawing.Size(204, 22);
            this.menuHelpDesktopEarLab.Text = "Desktop EarLab help...";
            this.menuHelpDesktopEarLab.Click += new System.EventHandler(this.menuHelpDesktopEarLab_Click);
            // 
            // menuHelpAbout
            // 
            this.menuHelpAbout.Name = "menuHelpAbout";
            this.menuHelpAbout.Size = new System.Drawing.Size(204, 22);
            this.menuHelpAbout.Text = "About Desktop EarLab...";
            this.menuHelpAbout.Click += new System.EventHandler(this.menuHelpAbout_Click);
            // 
            // timerCommandlineRun
            // 
            this.timerCommandlineRun.Tick += new System.EventHandler(this.timerCommandlineRun_Tick);
            // 
            // openFileDialog
            // 
            this.openFileDialog.FileName = "openFileDialog";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(458, 408);
            this.Controls.Add(this.txtStatusDisplay);
            this.Controls.Add(this.statusBar);
            this.Controls.Add(this.menuStrip1);
            this.Controls.Add(this.logDisplay);
            this.Controls.Add(this.btnAbort);
            this.Controls.Add(this.btnStop);
            this.Controls.Add(this.btnRun);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.udFrameCount);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip1;
            this.MinimumSize = new System.Drawing.Size(466, 442);
            this.Name = "Form1";
            this.Text = "Desktop EarLab Simulator";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Resize += new System.EventHandler(this.Form1_Resize);
            this.LocationChanged += new System.EventHandler(this.Form1_LocationChanged);
            ((System.ComponentModel.ISupportInitialize)(this.udFrameCount)).EndInit();
            this.statusBar.ResumeLayout(false);
            this.statusBar.PerformLayout();
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.NumericUpDown udFrameCount;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnRun;
        private System.Windows.Forms.Button btnStop;
        private System.Windows.Forms.Button btnAbort;
        private System.Windows.Forms.RichTextBox logDisplay;
        private System.Windows.Forms.StatusStrip statusBar;
        private System.Windows.Forms.ToolStripStatusLabel sbTextPanel;
        private System.Windows.Forms.ToolStripProgressBar progressBar;
        private System.Windows.Forms.TextBox txtStatusDisplay;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem menuFileOpenDiagram;
        private System.Windows.Forms.ToolStripMenuItem menuFileOpenParameters;
        private System.Windows.Forms.ToolStripSeparator menuFileSeparator;
        private System.Windows.Forms.ToolStripMenuItem menuFileExit;
        private System.Windows.Forms.ToolStripMenuItem modelToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem menuModelChooseDiagramFile;
        private System.Windows.Forms.ToolStripMenuItem menuModelChooseParameterFile;
        private System.Windows.Forms.ToolStripMenuItem menuModelSetInputDirectory;
        private System.Windows.Forms.ToolStripMenuItem menuModelSetOutputDirectory;
        private System.Windows.Forms.ToolStripMenuItem menuModelRun;
        private System.Windows.Forms.ToolStripMenuItem menuEnvironment;
        private System.Windows.Forms.ToolStripMenuItem menuEnvironmentSetModuleDirectory;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem menuHelpDesktopEarLab;
        private System.Windows.Forms.ToolStripMenuItem menuHelpAbout;
        private System.Windows.Forms.Timer timerCommandlineRun;
        private System.Windows.Forms.OpenFileDialog openFileDialog;
        private System.Windows.Forms.FolderBrowserDialog folderBrowser;
        private System.Windows.Forms.ToolStripMenuItem menuFileSave;
        private System.Windows.Forms.ToolStripMenuItem menuFileLoad;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
        private System.Windows.Forms.SaveFileDialog saveFileDialog;
    }
}

