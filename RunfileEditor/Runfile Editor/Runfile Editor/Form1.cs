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


namespace RunfileEditor
{
    public partial class frmContainer : Form
    {

        #region Data members
        private string theRunfile;
        private string err;
        private XmlDocument RunfileDocument;
        private EarlabRunfile RunfileObject;


        #endregion


        public frmContainer()
        {
            InitializeComponent();
            //Create a new instance of the child form.
            
            //RunfileEditor.frmChild child = new RunfileEditor.frmChild(this);
            //Show the form
            //child.Show(); --> pops it from start.
            
        }

        private void SaveXMLFile_Click(object sender, EventArgs e)
        {
            SaveXmlFile();

        }


        private void SendFileXMLToEFI_Click(object sender, EventArgs e)
        {
            //This is the part that needs to be updated from the Verification.dll
            MessageBox.Show(
            "This would send the Runfile to Earlab.",
            "Run File to Earlab", MessageBoxButtons.OK, MessageBoxIcon.Information);

            //Send to EFI
            //1.) Write the RunfileObject to an XML string
                    
            //2.) Send the Runfile XML string to the EFI and Receive XML Errors back
            //3.) if xml has errors
                //4.) RunfileObject.EFIUpdate(string VerificationErrors) to update spg
                //     Update Runfile Object
                //     Update display automatically
                //     Use older code to use the path to update necessary RunfileObjects
            //X.) else
                //6.) Allow desktop earlab to run.


            ///////to do this
            //a.) RunfileObject --
                //i.) str. severity, (already here) message on each parameter item.
                //ii.)

            //Do some operations on the Runfile Object ....

            //Clear the old errors

            //Discard any old verification information



        }

        private void Exit_Click(object sender, EventArgs e)
        {
            ExitRoutine();
        }

        ////////////////////////Some other ways to input the Runfile
        [STAThread]
        private void OpenFile_Click(object sender, EventArgs e)
        {
            OpenXMLFile();
        }

        //end stuff not using yet

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExitRoutine();
        }

        //Need to fix these guys ---
        #region
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
        #endregion

        [STAThread]
        private void openSavedRunfileToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenXMLFile2();
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

        private void menuStrip1_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {

        }

        [STAThread]
        private void SaveXmlFile()
        {

            try
            {

                //1.) Use RunfileObject Function
                XmlDocument NewXmlRunfile = new XmlDocument();
                string theValidRunfile;

                //2.) Give user options on saving
                //.Earlab file cannot be over-written

                //3.) Save Runfile

                //Need to error check to ensure we have an object.

                if (RunfileObject == null)
                {
                    //some feedback
                    MessageBox.Show(
                        "You don't have an active Runfile",
                        "Some Feedback.",
                         MessageBoxButtons.OK, MessageBoxIcon.Information);

                }
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
                    //saveFileDialog1.FilterIndex = 1;
                    //saveFileDialog1.OverwritePrompt = true;


                    //
                    if (saveFileDialog1.ShowDialog() == DialogResult.Cancel)
                    {

                        theValidRunfile = null;

                        //some feedback
                        MessageBox.Show(
                            "Some Feedback.",
                            "cancel button clicked", MessageBoxButtons.OK, MessageBoxIcon.Information);
                        //return null;

                    }

                    else
                    {

                        theValidRunfile = saveFileDialog1.FileName;

                        NewXmlRunfile.Save(theValidRunfile);
                        //save("c:\pavel.xml")

                        //some feedback    
                        MessageBox.Show(
                       "Some Feedback.",
                       "Some Feedback", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }

                    //SaveFile()
                    //4.) How do you want to save this new document?
                    //Old file name, newfile name. -- This is in GUI Helper class
                    //NewXmlRunfile.Save("tester.xml");

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

                //if (radioButton1.Checked)
                //{
                //    //Some Checking
                //    RunfileDocument.Load(textBox1.Text);
                //}
                //if (radioButton2.Checked)
                //{
                //    //Some Checking
                //    FileStream stream = new FileStream(textBox1.Text, FileMode.Open);
                //    RunfileDocument.Load(stream);
                //    stream.Close();
                //}
                //if (radioButton3.Checked)
                //{
                //    //Some Checking
                //    RunfileDocument.LoadXml(textBox1.Text);
                //}

                //if button 4
                if (radioButton4.Checked)
                {
                    string FileNameTemp;
                    //1.) Open File
                    //a.) Open file
                    //b.) Check file against xsd schema
                    //c.) check to ensure it isn't null

                    //Create form inherited bit.
                    OpenFileDialog openFileDialog1 = new OpenFileDialog();
                    openFileDialog1.Title = "Please open a saved Run File";

                    //Set items
                    openFileDialog1.InitialDirectory =
                        System.Environment.GetFolderPath(Environment.SpecialFolder.Personal);


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


                    //Keep GUI elements on main form 
                    //Leave that stuff in the GUI
                    //--- OK you can use settings to remember the last directory you were in.

                    //FileNameTemp = GuiFile.OpenFile();

                    if (FileNameTemp != null)
                    {
                        theRunfile = FileNameTemp;
                        //2.) Initize and Load XML Document
                        //RunfileDocument = new XmlDocument();
                        RunfileDocument.Load(theRunfile);
                    }


                }// end button 4

                //3.) Create a RunfileObject
                //This should have entire Runfile in testRunfile
                RunfileObject = new EarlabRunfile(RunfileDocument);

                //3.5) Clear tabs -- probably want an error check here
                //Do you really want to load a new Runfile?
                ModuleClearTabs();

                //4.) Create the Grids on the Form
                //Now For Each Module in Runfile build a sample grid
                //Using the tabcreation class

                //Change the image?????????????????
                //string thedamnimage = RunfileObject.RunfileInformation.RunfileInformationImageLocation.ToString();

                //this.pictureBox1.ImageLocation = thedamnimage;

                //this part works, module works, ModuleTab doesnt'
                foreach (EarlabModule elModule in RunfileObject.EarlabModules)
                {
                    //create a new tab on the form
                    ModuleTab(elModule);
                    //ModuleTab();
                }
                //set information fro the user
                //Abstract data, model image etc...
                //pictureBox1.Image = new Bitmap(RunfileObject.RunfileInformation.RunfileModelImageLocation);

                //Change the image in the picture box
                //this.pictureBox1.ImageLocation = "image.jpg";

                //Some Feedback
                MessageBox.Show("Runfile Document Opened Successfully!");
            }


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

        [STAThread]
        private void OpenXMLFile2()
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
                    openFileDialog1.InitialDirectory =
                        System.Environment.GetFolderPath(Environment.SpecialFolder.Personal);


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


                    //Keep GUI elements on main form 
                    //Leave that stuff in the GUI
                    //--- OK you can use settings to remember the last directory you were in.

                    //FileNameTemp = GuiFile.OpenFile();

                    if (FileNameTemp != null)
                    {
                        theRunfile = FileNameTemp;
                        //2.) Initize and Load XML Document
                        //RunfileDocument = new XmlDocument();
                        RunfileDocument.Load(theRunfile);
                    }



                //3.) Create a RunfileObject
                //This should have entire Runfile in testRunfile
                RunfileObject = new EarlabRunfile(RunfileDocument);

                //3.5) Clear tabs -- probably want an error check here
                //Do you really want to load a new Runfile?
                ModuleClearTabs();

                //4.) Create the Grids on the Form
                //Now For Each Module in Runfile build a sample grid
                //Using the tabcreation class

                //Change the image?????????????????
                //string thedamnimage = RunfileObject.RunfileInformation.RunfileInformationImageLocation.ToString();

                //string thedamnimage = "C:\\Documents and Settings\\Jon\\My Documents\\Visual Studio 2005\\Projects\\Ear Lab Project\\Earlab-SPG-Tester-JLW\\WindowsApplication4\\resources\\image.jpg";

                //this.pictureBox1.ImageLocation ;
                //= thedamnimage;
                //this part works, module works, ModuleTab doesnt'
                foreach (EarlabModule elModule in RunfileObject.EarlabModules)
                {
                    //create a new tab on the form
                    ModuleTab(elModule);
                    //ModuleTab();
                }

                //Change the image in the picture box
                //this.pictureBox1.ImageLocation = "image.jpg";

                //Some Feedback
                MessageBox.Show("Runfile Document Opened Successfully!");
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
            //some stuff here
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

        private void label2_Click(object sender, EventArgs e)
        {

        }




    }





}

