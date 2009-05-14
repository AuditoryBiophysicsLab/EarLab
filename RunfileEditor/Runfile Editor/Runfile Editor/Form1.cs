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
    public partial class Form1 : Form
    {

        #region Data members
        private string theRunFile;
        private XmlDocument RunFileDocument;
        private EarLabRunFile RunFileObject;


        #endregion


        public Form1()
        {
            InitializeComponent();
        }




        private void SaveXMLFile_Click(object sender, EventArgs e)
        {
            SaveXmlFile();

        }


        private void SendFileXMLToEFI_Click(object sender, EventArgs e)
        {
            MessageBox.Show(
            "This would send the RunFile to EarLab.",
            "Run File to Earlab", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        private void Exit_Click(object sender, EventArgs e)
        {
            ExitRoutine();
        }

        ////////////////////////Some other ways to input the runfile

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


        private void openSavedRunFileToolStripMenuItem_Click(object sender, EventArgs e)
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

        private void earLabHelpToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show(
            "This would be the help information.",
            "Help Menu", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        private void menuStrip1_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {

        }

        private void SaveXmlFile()
        {

            try
            {

                //1.) Use RunFileObject Function
                XmlDocument NewXmlRunFile = new XmlDocument();
                string theValidRunFile;

                //2.) Give user options on saving
                //.earlab file cannot be over-written

                //3.) Save Runfile

                //Need to error check to ensure we have an object.

                if (RunFileObject == null)
                {
                    //some feedback
                    MessageBox.Show(
                        "You don't have an active RunFile",
                        "Some Feedback.",
                         MessageBoxButtons.OK, MessageBoxIcon.Information);

                }
                else
                {

                    NewXmlRunFile = RunFileObject.RunFileXMLCreate();

                    //Create form inherited bit.
                    SaveFileDialog saveFileDialog1 = new SaveFileDialog();
                    saveFileDialog1.Title = "Specify Destination Filename";

                    //Set items to the last used directory
                    saveFileDialog1.InitialDirectory =
                        System.Environment.GetFolderPath(Environment.SpecialFolder.Personal);
                    saveFileDialog1.Filter = "XML|*.xml|EARLAB|*.earlab";


                    //not sure about this part.
                    //saveFileDialog1.FilterIndex = 1;
                    //saveFileDialog1.OverwritePrompt = true;


                    //
                    if (saveFileDialog1.ShowDialog() == DialogResult.Cancel)
                    {

                        theValidRunFile = null;

                        //some feedback
                        MessageBox.Show(
                            "Some Feedback.",
                            "cancel button clicked", MessageBoxButtons.OK, MessageBoxIcon.Information);
                        //return null;

                    }

                    else
                    {

                        theValidRunFile = saveFileDialog1.FileName;

                        NewXmlRunFile.Save(theValidRunFile);
                        //save("c:\pavel.xml")

                        //some feedback    
                        MessageBox.Show(
                       "Some Feedback.",
                       "Some Feedback", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }

                    //SaveFile()
                    //4.) How do you want to save this new document?
                    //Old file name, newfile name. -- This is in GUI Helper class
                    //NewXmlRunFile.Save("tester.xml");

                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

        }


        private void OpenXMLFile()
        {

            try
            {
                //XmlDocument doc = new XmlDocument();
                RunFileDocument = new XmlDocument();

                if (radioButton1.Checked)
                {
                    //Some Checking
                    RunFileDocument.Load(textBox1.Text);
                }
                if (radioButton2.Checked)
                {
                    //Some Checking
                    FileStream stream = new FileStream(textBox1.Text, FileMode.Open);
                    RunFileDocument.Load(stream);
                    stream.Close();
                }
                if (radioButton3.Checked)
                {
                    //Some Checking
                    RunFileDocument.LoadXml(textBox1.Text);
                }

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


                    //.xml ToDo: own "file format" .earlab hehe
                    openFileDialog1.Filter = "XML|*.xml|Earlab|*.earlab";


                    //Open Dialogue box to get a runFile
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
                        theRunFile = FileNameTemp;
                        //2.) Initize and Load XML Document
                        //RunFileDocument = new XmlDocument();
                        RunFileDocument.Load(theRunFile);
                    }


                }// end button 4

                //3.) Create a RunFileObject
                //This should have entire runfile in testRunfile
                RunFileObject = new EarLabRunFile(RunFileDocument);

                //3.5) Clear tabs -- probably want an error check here
                //Do you really want to load a new runfile?
                ModuleClearTabs();

                //4.) Create the Grids on the Form
                //Now For Each Module in RunFile build a sample grid
                //Using the tabcreation class

                //Change the image?????????????????
                string thedamnimage = RunFileObject.RunFileInformation.RunFileInformationImageLocation.ToString();

                //this.pictureBox1.ImageLocation = thedamnimage;

                //this part works, module works, ModuleTab doesnt'
                foreach (EarLabModule elModule in RunFileObject.EarLabModules)
                {
                    //create a new tab on the form
                    ModuleTab(elModule);
                    //ModuleTab();
                }

                //Change the image in the picture box
                //this.pictureBox1.ImageLocation = "image.jpg";

                //Some Feedback
                MessageBox.Show("RunFile Document Opened Successfully!");
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

        }


        private void OpenXMLFile2()
        {

            try
            {
                    //XmlDocument doc = new XmlDocument();
                    RunFileDocument = new XmlDocument();

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


                    //.xml ToDo: own "file format" .earlab hehe
                    openFileDialog1.Filter = "XML|*.xml|EARLAB|*.earlab";


                    //Open Dialogue box to get a runFile
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
                        theRunFile = FileNameTemp;
                        //2.) Initize and Load XML Document
                        //RunFileDocument = new XmlDocument();
                        RunFileDocument.Load(theRunFile);
                    }



                //3.) Create a RunFileObject
                //This should have entire runfile in testRunfile
                RunFileObject = new EarLabRunFile(RunFileDocument);

                //3.5) Clear tabs -- probably want an error check here
                //Do you really want to load a new runfile?
                ModuleClearTabs();

                //4.) Create the Grids on the Form
                //Now For Each Module in RunFile build a sample grid
                //Using the tabcreation class

                //Change the image?????????????????
                //string thedamnimage = RunFileObject.RunFileInformation.RunFileInformationImageLocation.ToString();

                //string thedamnimage = "C:\\Documents and Settings\\Jon\\My Documents\\Visual Studio 2005\\Projects\\Ear Lab Project\\EarLab-SPG-Tester-JLW\\WindowsApplication4\\resources\\image.jpg";

                //this.pictureBox1.ImageLocation ;
                //= thedamnimage;
                //this part works, module works, ModuleTab doesnt'
                foreach (EarLabModule elModule in RunFileObject.EarLabModules)
                {
                    //create a new tab on the form
                    ModuleTab(elModule);
                    //ModuleTab();
                }

                //Change the image in the picture box
                //this.pictureBox1.ImageLocation = "image.jpg";

                //Some Feedback
                MessageBox.Show("RunFile Document Opened Successfully!");
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
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




    }





}

