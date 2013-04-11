using System;
using System.IO;
using System.Windows.Forms;
using Microsoft.Win32;

namespace ConsoleApplication1
{
	/// <summary>
	/// Summary description for Class1.
	/// </summary>
	class Class1
	{
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main(string[] args)
		{
			//
			// TODO: Add code to start application here
			//

			for(int i = 0; i < args.Length; i++)
			{
				string pathdir = "";
				string tempstring = "";
				RegistryKey myKey = null;

				switch(args[i])
				{
					case "-i":
						pathdir = args[i+1];
						
						if( pathdir.EndsWith("\\") )
							pathdir.Remove(pathdir.Length-1, 1);

						if( Directory.Exists(pathdir) )
						{
							myKey = Registry.LocalMachine.OpenSubKey("System\\CurrentControlSet\\Control\\Session Manager\\Environment", true);
							if( myKey.GetValue("Path").ToString().IndexOf(pathdir + "\\;") < 0 && myKey.GetValue("Path").ToString().IndexOf(pathdir + "\\") < 0 && myKey.GetValue("Path").ToString().IndexOf(pathdir) < 0 )
							{	
								tempstring = myKey.GetValue("Path").ToString();
								if( !tempstring.EndsWith(";") )
									tempstring += ";";

								myKey.SetValue("Path", tempstring + pathdir + ";");

								myKey.Flush();
								myKey.Close();
								
							}
						
						}
						break;
					case "-u":
						pathdir = args[i+1];
						
						if( pathdir.EndsWith("\\") )
							pathdir.Remove(pathdir.Length-1, 1);

						myKey = Registry.LocalMachine.OpenSubKey("System\\CurrentControlSet\\Control\\Session Manager\\Environment", true);
						
						tempstring = myKey.GetValue("Path").ToString();
						
						tempstring = tempstring.Replace(pathdir + "\\;", "");
						tempstring = tempstring.Replace(pathdir + "\\", "");
						tempstring = tempstring.Replace(pathdir, "");
						
						myKey.SetValue("Path", tempstring);

						myKey.Flush();
						myKey.Close();
							
						break;
					case "-deletefolders":
						pathdir = args[i+1];
						
						if(Directory.Exists(pathdir))
						{
							if( MessageBox.Show("Remove EarLab Data folder(s) (" + pathdir + ") ?", "Remove Folder(s)", MessageBoxButtons.YesNo, MessageBoxIcon.Question, MessageBoxDefaultButton.Button2) == DialogResult.Yes )
								Directory.Delete(pathdir, true);
						}
						break;
					case "-removedataviewer":
						if( MessageBox.Show("Remove EarLab DataViewer?", "Remove DataViewer", MessageBoxButtons.YesNo, MessageBoxIcon.Question, MessageBoxDefaultButton.Button2) == DialogResult.Yes )
							System.Diagnostics.Process.Start("msiexec.exe", "/uninstall " + args[i+1] + " /quiet");
						break;
				}

			}
			
		}
	}
}
