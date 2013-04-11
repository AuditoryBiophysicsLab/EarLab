using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace EarLab
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(string [] args)
        {
            try
            {
                Application.EnableVisualStyles();
            }
            catch (Exception e)
            {
                MessageBox.Show("Exception caught during call to Application.EnableVisualStyles: " + e.ToString());
            }
            try
            {
                Application.SetCompatibleTextRenderingDefault(false);
            }
            catch (Exception e)
            {
                MessageBox.Show("Exception caught during call to Application.SetCompatibleTextRenderingDefault: " + e.ToString());
            }
            try
            {
                Application.Run(new Form1(args));
            }
            catch (Exception e)
            {
                MessageBox.Show("Exception caught during call to Application.Run: " + e.ToString());
            }
        }
    }
}