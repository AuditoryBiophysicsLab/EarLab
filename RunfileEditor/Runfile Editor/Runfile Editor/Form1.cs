using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using VisualHint.SmartPropertyGrid;
using System.IO;
using System.Text.RegularExpressions;


namespace RunfileEditor
{
    public partial class frmContainer : Form
    {

        //Data Members ==========================================================================================///
        #region Data members
        private string theRunfile;
        private string err;
        private XmlDocument RunfileDocument;
        private EarlabRunfile RunfileObject;
        #endregion

        //The Error View Specific Data Members ==================================================================///
        #region ErrorViewList Data members

        //[Remove]
        //private System.Collections.Specialized.StringCollection folderCol;

        //[Remove]
        //Not quite sure about these two...
        //private System.Windows.Forms.ImageList ilLarge;
        //private System.Windows.Forms.ImageList ilSmall;

        //This is the list view
        private System.Windows.Forms.ListView listViewErrors;

        //[Remove]
        //This is the label that goes on top of the errors
        //private System.Windows.Forms.Label lblCurrentPath;

        #endregion

        public frmContainer()
        {
            //Main Form
            InitializeComponent();

            //Error View List on Form
            InitializeErrorView();

            //Instatiate the Runfile Object Value here and connect to an 'on change property"
            //1.) So a runfile Object is always around. 
            RunfileObject = new EarlabRunfile();

        }

        //The Buttons on the GUI ================================================================================///
        #region The list of all the buttons on the GUI
        
        private void SaveXMLFile_Click(object sender, EventArgs e)
        {
            SaveXmlFile();
        }

        private void SendFileXMLToEFI_Click(object sender, EventArgs e)
        {

            XMLtoEFI();

        }

        //This button launches the Desktop Earlab appears if the error count on Verification Errors == 0.
        private void button1_Click(object sender, EventArgs e)
        {

            MessageBox.Show(
            "This would launch Desktop Earlab.",
            "About Menu", MessageBoxButtons.OK, MessageBoxIcon.Information);
         
        }

        private void Exit_Click(object sender, EventArgs e)
        {
            ExitRoutine();
        }

        private void OpenFile_Click(object sender, EventArgs e)
        {
            OpenXMLFile();
        }
        #endregion

        //====Menu Items ========================================================================================///
        #region Menu Items
        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExitRoutine();
        }


        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show(
            "This would be the about information.",
            "About Menu", MessageBoxButtons.OK, MessageBoxIcon.Information);

        }

        private void onlineHelpToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show(
            "This would be the online help information at the Earlab site.",
            "Help Menu", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        private void model1ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show(
            "This would be the model call.",
            "Model stand in", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        [STAThread]
        private void openSavedRunfileToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenXMLFile();
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveXmlFile();
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveXmlFile();
        }

        private void EarlabHelpToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show(
            "This would be the help information.",
            "Help Menu", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        #endregion

        // Some GUI elements on the Form ========================================================================///
        #region Some GUI elements on the Form

        /// <summary>
        /// This builds a tab for the Abstract information in the
        /// Runfile Information tab
        /// 
        /// </summary>
        public void Abstract_ModuleTab()
        {
            ////2.) Add Module Grid to Tab
            TabPage tabPageA = new TabPage("Model Abstract");

            //3.) Tab location and size settings
            tabPageA.Location = new System.Drawing.Point(4, 22);
            tabPageA.Padding = new System.Windows.Forms.Padding(3);
            tabPageA.Size = new System.Drawing.Size(831, 392);
            tabPageA.TabIndex = 1;
            tabPageA.UseVisualStyleBackColor = true;

            ////items
            //tabPageA.
            //textBox1 = new System.Windows.Forms.TextBox();
            //tabPageA.pictureBox2 = new System.Windows.Forms.PictureBox();
            //tabPageA.label5 = new System.Windows.Forms.Label();
            //tabPageA.label4 = new System.Windows.Forms.Label();




            //// 
            //// label5
            ////

            //tabPageA.label5.AutoSize = true;
            //tabPageA.label5.Location = new System.Drawing.Point(579, 42);
            //tabPageA.label5.Name = "label5";
            //tabPageA.label5.Size = new System.Drawing.Size(35, 13);
            //tabPageA.label5.TabIndex = 8;
            //tabPageA.label5.Text = "label5";
            //// 
            //// label4
            //// 
            //tabPageA.label4.AutoSize = true;
            //tabPageA.label4.Location = new System.Drawing.Point(579, 16);
            //tabPageA.label4.Name = "label4";
            //tabPageA.label4.Size = new System.Drawing.Size(35, 13);
            //tabPageA.label4.TabIndex = 7;
            //tabPageA.label4.Text = "label4";
            //// 
            //// textBox1
            //// 
            //this.textBox1.Location = new System.Drawing.Point(579, 102);
            //this.textBox1.Multiline = true;
            //this.textBox1.Name = "textBox1";
            //this.textBox1.Size = new System.Drawing.Size(359, 345);
            //this.textBox1.TabIndex = 1;

            //// 
            //// pictureBox2
            //// 
            //tabPageA.pictureBox2.Location = new System.Drawing.Point(23, 16);
            //tabPageA.pictureBox2.Name = "pictureBox2";
            //tabPageA.pictureBox2.Size = new System.Drawing.Size(524, 437);
            //tabPageA.pictureBox2.TabIndex = 4;
            //tabPageA.pictureBox2.TabStop = false;
            //tabPageA.tabPage2.ResumeLayout(false);
            //tabPageA.tabPage2.PerformLayout();
            //((System.ComponentModel.ISupportInitialize)(tabPageA.pictureBox2)).EndInit();







            //tabPageA.Controls.Add(tabPageA.label5);
            //tabPageA.Controls.Add(this.label4);
            //tabPageA.Controls.Add(this.textBox1);
            //.Controls.Add(this.pictureBox2);
            //tabPageA.Name = "Model Abstract";
            ////    this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            ////    this.tabPage2.Size = new System.Drawing.Size(1016, 486);
            ////    this.tabPage2.TabIndex = 1;
            ////    this.tabPage2.Text = "tabPage2";
            ////    this.tabPage2.UseVisualStyleBackColor = true;


            ////(?)
            ////Need to add this to the form for this to work right
            ////TabControl tabControl1 = new System.Windows.Forms.TabControl();
            tabControl1.TabPages.Add(tabPageA);
        }

        /// <summary>
        /// On click this sends the user to the Abstract tab
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void pictureBox1_Click(object sender, EventArgs e)
        {
            //on click go to tab two -- the abstract tab
            if (RunfileObject != null)
            {
                tabControl1.SelectedTab = tabControl1.TabPages[1];
            }
        }

        
        private void InitializeErrorView()
        {
            CreateHeadersAndFillListView();

        }

        /// <summary>
        /// Labels the columns
        /// initiaizes
        /// </summary>
        private void CreateHeadersAndFillListView()
        {
            ColumnHeader colHead;
            colHead = new ColumnHeader();
            colHead.Text = "Error Path";
            colHead.Width = 300;
            this.listViewErrors.Columns.Add(colHead);

            colHead = new ColumnHeader();
            colHead.Text = "Warning Level";
            colHead.Width = 100;
            this.listViewErrors.Columns.Add(colHead);

            colHead = new ColumnHeader();
            colHead.Text = "Warning Message";
            colHead.Width = 500;
            this.listViewErrors.Columns.Add(colHead);
        }

        private void ListClear()
        {
            listViewErrors.Clear();

        }

        private void ProductionListView(EarlabRunfile RunfileObject)
        {
            try
            {
                ListViewItem lvi;
                ListViewItem.ListViewSubItem lvsi;

                //Adds the label above the error list
                //this.lblCurrentPath.Text = root + "    (Double click to display the path name)";

                //this.lblCurrentPath.Text = " Errors Present in the Runfile Document";

                if (RunfileObject != null)
                {

                    //this.listViewErrors.Items.Clear();
                    //this.listViewErrors.BeginUpdate();
                    this.listViewErrors.Clear();
                    InitializeErrorView();

                    foreach (VerificationError error1 in RunfileObject.VErrorCollection)
                    {

                        lvi = new ListViewItem();
                        lvi.Text = error1.FullErrorPath;
                        //lvi.ImageIndex = 0;

                        lvsi = new ListViewItem.ListViewSubItem();
                        lvsi.Text = error1.Severity;
                        lvi.SubItems.Add(lvsi);

                        lvsi = new ListViewItem.ListViewSubItem();
                        lvsi.Text = error1.Message;
                        lvi.SubItems.Add(lvsi);

                        this.listViewErrors.Items.Add(lvi);
                    }

                }

                this.listViewErrors.EndUpdate();
            }
            catch (System.Exception err)
            {
                MessageBox.Show("Error: " + err.Message);
            }

            this.listViewErrors.View = View.Details;

        }


        #endregion


        //Functionality of the GUI elements =====================================================================///
        #region Functionality of the GUI elements
        [STAThread]
        private void SaveXmlFile()
        {

            try
            {

                //1.) Use RunfileObject Function
                XmlDocument NewXmlRunfile = new XmlDocument();
                string ValidRunfileName;

                //2.) Give user options on saving
                //.Earlab file cannot be over-written

                if (RunfileObject == null)
                {
                    //some feedback
                    MessageBox.Show(
                        "Some Feedback.",
                        "cancel button clicked", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                //[Change]
                //else if (RunfileObject.HasChanged == false)
                //{

                //    //some feedback
                //    MessageBox.Show(
                //        "You haven't changed anything in Runfile",
                //        "Some Feedback.",
                //         MessageBoxButtons.OK, MessageBoxIcon.Information);

                //}

                else
                {
                    NewXmlRunfile = RunfileObject.RunfileXMLCreate();

                    //Create form inherited bit.
                    SaveFileDialog saveFileDialog1 = new SaveFileDialog();
                    saveFileDialog1.Title = "Specify Destination Filename";

                    //Set items to the last used directory
                    saveFileDialog1.InitialDirectory =
                        System.Environment.GetFolderPath(Environment.SpecialFolder.Personal);
                    saveFileDialog1.Filter = "XML|*.xml|Earlab|*.Earlab";


                    //not sure about this part.
                    saveFileDialog1.FilterIndex = 1;
                    saveFileDialog1.OverwritePrompt = true;


                    if (saveFileDialog1.ShowDialog() == DialogResult.Cancel)
                    {

                        ValidRunfileName = null;

                        //some feedback
                        MessageBox.Show(
                            "Some Feedback.",
                            "cancel button clicked", MessageBoxButtons.OK, MessageBoxIcon.Information);
                        //return null;

                    }
                    else
                    {
                        ValidRunfileName = saveFileDialog1.FileName;

                        NewXmlRunfile.Save(ValidRunfileName);

                        //some feedback    
                        MessageBox.Show(
                       "Some Feedback.",
                       "Some Feedback", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }

                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);

            }

        }

        [STAThread]
        private void OpenXMLFile()
        {

            try
            {
                //XmlDocument doc = new XmlDocument();
                RunfileDocument = new XmlDocument();


                string FileNameTemp;
                //1.) Open File
                //a.) Open file
                //b.) Check file against xsd schema
                //c.) check to ensure it isn't null

                //Create form inherited bit.
                OpenFileDialog openFileDialog1 = new OpenFileDialog();
                openFileDialog1.Title = "Please open a saved Run File";

                //Set items
                //openFileDialog1.InitialDirectory =
                //    System.Environment.GetFolderPath(Environment.SpecialFolder.Personal);


                //.xml ToDo: own "file format" .Earlab hehe
                openFileDialog1.Filter = "XML|*.xml|Earlab|*.Earlab";


                //Open Dialogue box to get a Runfile
                //Some error handling here
                if (openFileDialog1.ShowDialog() == DialogResult.Cancel)
                {
                    MessageBox.Show("cancel button clicked");
                    FileNameTemp = null;
                }
                else
                {
                    FileNameTemp = openFileDialog1.FileName;
                }

                if (FileNameTemp != null)
                {
                    theRunfile = FileNameTemp;
                    //2.) Initize and Load XML Document
                    //RunfileDocument = new XmlDocument();
                    RunfileDocument.Load(theRunfile);
                }

                //2.9) Is this a valid runfile document?



                ConsoleValidator.ValidateXml("your.xml", "your.xsd");


                //3.) Create a RunfileObject
                //This should have entire Runfile in testRunfile
                RunfileObject = new EarlabRunfile(RunfileDocument);

                //attach efi call to any change
                foreach (EarlabModule eM in RunfileObject.EarlabModules)
                {
                    foreach (EarlabParameter eP in eM.EarlabParameters)
                    {

                        eP.DataChanged += SomethingChanged;
                    }

                }

                //3.5) Clear tabs -- probably want an error check here
                //Do you really want to load a new Runfile?
                ModuleClearTabs();

                //3.75) Clear List View
                this.listViewErrors.Clear();

                //4.) Create the Grids on the Form
                //Now For Each Module in Runfile build a sample grid
                //Using the tabcreation class

                //[abstract info]
                //Abstract_ModuleTab();

                //AbstractTabPageClass AModuleTab = new AbstractTabPageClass(RunfileObject, FileNameTemp);
                //tabControl1.TabPages.Add(new AbstractTabPageClass(RunfileObject, FileNameTemp));

                //Load the Picture //RunfileObject.RunfileInformation.RunfileInformationImageLocation;
                string tempPath = Path.GetFullPath(FileNameTemp);
                tempPath = Regex.Replace(tempPath, Path.GetFileName(FileNameTemp), "");
                this.pictureBox1.ImageLocation = Path.Combine(tempPath, RunfileObject.RunfileInformation.RunfileInformationImageLocation); 

                //this part works, module works, ModuleTab doesnt'
                foreach (EarlabModule elModule in RunfileObject.EarlabModules)
                {
                    //create a new tab on the form
                    ModuleTab(elModule);
                }


                //Some Feedback
                MessageBox.Show("Runfile Document Opened Successfully!");

                //[Problem]
               //(EarlabParameterInteger)RunfileObject.EarlabModules[1].EarlabParameters[3]. += Intparameter_ValueChanged;

            }

            //[Change]
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
                err = ex.StackTrace;

                //lastly make the recording to the errorlog
                //Going to have to make a "catch flag" to log errors
                {   
                    //!!/write to error pop-up
                    errorLog currentLog = new errorLog(err);
                    currentLog.PathName = Application.StartupPath;
                    currentLog.WriteErrorLog();

                    RunfileEditor.frmChild child = new RunfileEditor.frmChild(this);
                    //Show the form
                    child.Show(); //--> pops it from start.
                    child.BringToFront();//brings it to front

                    //[error]
                    //child.txtErrorMsgs.Text = currentLog.ReadErrorLog();
                    //Error Logging works --- need to work on windows type display.
                    //textBox1.Text = currentLog.ReadErrorLog();

                    //errorBox errorForm = new errorBox();
                    //errorForm.ShowDialog();
                    //!!/write to error pop-up
                }


            }


        }

        private void ExitRoutine()
        {

            if (
                MessageBox.Show("Do You Really Want to Quit?", "Exit", MessageBoxButtons.OKCancel) == DialogResult.OK
               )


            //confirms that the user wants to quit the app
            {

                Application.Exit();
            }

        }

        private void XMLtoEFI()
        {
            if (RunfileObject != null)
            {
                //int TabLocation;
                //Find Tab.
                //TabLocation = tabControl1.SelectedIndex;


                //EFI works.
                RunfileObject.EFI_Run();


                //0.) clear tabs
                ModuleClearTabs();

                //1.) Repaint Modules
                //[abstract info]
                //Abstract_ModuleTab();

                ////Load the Picture //RunfileObject.RunfileInformation.RunfileInformationImageLocation;
                //string tempPath = Path.GetFullPath(FileNameTemp);
                //tempPath = Regex.Replace(tempPath, Path.GetFileName(FileNameTemp), "");
                //this.pictureBox1.ImageLocation = Path.Combine(tempPath, RunfileObject.RunfileInformation.RunfileInformationImageLocation);

                //this part works, module works, ModuleTab doesnt'
                foreach (EarlabModule elModule in RunfileObject.EarlabModules)
                {
                    //create a new tab on the form
                    ModuleTab(elModule);
                }


                //2.) Repaint List View
                //Make dummy Errors Show up in List view --
                ProductionListView(RunfileObject);
                //Make Errors from the EFI show up in list view.
                //ProductionListView(RunfileObject);

                //Launch desktop earlab//
                //Just counts the errors.
                if (RunfileObject.VErrorCollection.Count == 0)
                {
                    button_create_if_no_errors();

                }


                //Put you back on original tab location.
               




            }
            else
            {
                MessageBox.Show(
                "This occurs b/c you don't have a runfile object yet",
                "Run File to Earlab", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        #endregion

        private void SomethingChanged(object Sender, EventArgs e)
        {
            int TabLocation = tabControl1.SelectedIndex;

            //take where the focus is, return focus.
            XMLtoEFI();

            tabControl1.SelectedIndex = TabLocation;

        }



        private void SchemaCheck(XmlDocument Document)
        {
            
            //The ValidationHandler event handler method is defined.
            //The XSD schema is being parsed using XmlTextReader class.
            //The parsed schema is added to the schema collection using XmlSchemaCollection class.
            //The schema collection is associated with XmlValidatingReader class.
            //The event handler method is associated with XmlValidatingReader class.
            //XmlValidatingReader class validates the XML document using the namespace URI specified when the schema was added to the collection.

                        



        }
    
    }



   

    



}

