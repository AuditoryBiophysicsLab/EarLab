using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace MySqlTest
{
    public partial class EnvironmentalDatabaseReader : UserControl
    {
        public EnvironmentDataType EnvironmentDataType;
        // Todo: Create a private member for your reader class here

        public EnvironmentalDatabaseReader()
        {
            InitializeComponent();
        }

        public event EnvironmentDataReadEventHandler EnvironmentDataRead;

        protected virtual void OnEnvironmentDataRead(EnvironmentDataReadEventArgs e)
        {
            if (EnvironmentDataRead != null)
                EnvironmentDataRead(this, e);
        }

        private void EnvironmentalDatabaseReader_DragDrop(object sender, DragEventArgs e)
        {
            // Extract the data from the DataObject-Container into a string list
            string[] FileList = (string[])e.Data.GetData(DataFormats.FileDrop, false);

            // Do something with the data...
            foreach (string curFile in FileList)
            {
                switch (EnvironmentDataType)
                {
                    case EnvironmentDataType.Bathymetry:
                        // ToDo: Ahmad: Call your bathymetry reader function here with each file
                        //BathymetryFileReader(curFile);
                        break;
                    case EnvironmentDataType.Salinity:
                    case EnvironmentDataType.Temperature:
                    case EnvironmentDataType.SalinityStandardDeviation:
                    case EnvironmentDataType.TemperatureStandardDeviation:
                        // ToDo: Ahmad: Call your water colummn reader function here with each file
                        //EnvironmentFileReader(curFile);
                        break;
                    default:
                        break;
                }
            }
        }

        private void EnvironmentalDatabaseReader_DragEnter(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
                e.Effect = DragDropEffects.Copy; // Okay
            else
                e.Effect = DragDropEffects.None; // Unknown data, ignore it
        }
    }

    public delegate void EnvironmentDataReadEventHandler(object sender, EnvironmentDataReadEventArgs e);

    public enum EnvironmentDataType
    {
        Bathymetry,
        Temperature,
        Salinity,
        TemperatureStandardDeviation,
        SalinityStandardDeviation
    }

    public class EnvironmentDataReadEventArgs : EventArgs
    {
        // ToDo: Ahmad: put a public member with your data type here
        public string SourceFileName;
    }

}
