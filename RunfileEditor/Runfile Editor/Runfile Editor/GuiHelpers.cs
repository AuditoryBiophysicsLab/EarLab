using System;
using System.Collections.Generic;
using System.Data;
using System.Text;
using System.Xml;
using System.Windows.Forms;
using VisualHint.SmartPropertyGrid;

/// <summary>
/// This class
/// handles all the functions from the 
/// </summary>
/// 
namespace MyTestApp
{
    public class GuiFile
    {
        //data members given basically static methods???
        //Perhaps theRunfileName gets returned or?
        //public string theRunfileName;

        public GuiFile()
        {
            //
            // TODO: Add constructor logic here
            //
        }

        public static string OpenFile()
        {
            //
            string theValidRunfile;


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
                theValidRunfile = null;
                return null;
            }
            //store document into the xml object.
            theValidRunfile = openFileDialog1.FileName;
            //Error handling here

            //return

            return theValidRunfile;

        } // The user of this function has to handle null values

        public static void OpenModelFile()
        {

        }

        //Need to make sure we are not overwriting a model
        //That comes with the package
        public static void SaveFile()
        {

            string theValidRunfile;

            //Create form inherited bit.
            SaveFileDialog saveFileDialog1 = new SaveFileDialog();
            saveFileDialog1.Title = "Specify Destination Filename";

            //Set items
            saveFileDialog1.InitialDirectory =
                System.Environment.GetFolderPath(Environment.SpecialFolder.Personal);

            saveFileDialog1.Filter = "XML|*.xml";

            //not sure about this part.
            //saveFileDialog1.FilterIndex = 1;
            //saveFileDialog1.OverwritePrompt = true;


            //
            if (saveFileDialog1.ShowDialog() == DialogResult.Cancel)
            {
                MessageBox.Show("cancel button clicked");
                theValidRunfile = null;
                //return null;
                
            }
            else
            {
                theValidRunfile = saveFileDialog1.FileName;

            }

        }

        //Validating methods
        public static bool ValidateRunfile(/*Runfile Xml*/)
        {
            //Use XSD scheme to validate file
            return true;
        }
        public static bool ValidateModuleFile(/* module xml */)
        {
            //Use XSD scheme to validate file

            return true;
        }

        public static void HandleStream()
        {


        }

        public static void HandleURL()
        {



        }

        public static void HandleString()
        {


        }

    }




}