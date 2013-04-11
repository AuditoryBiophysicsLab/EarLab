using System;
using System.Windows.Forms;

namespace EarLab.Viewers.Layouts
{
	/// <summary>
	/// Summary description for ILayout.
	/// </summary>
	public interface ILayout
	{
		string Read(string fileName);
		void Close();

		MainMenu MainMenu
		{
			set;
		}
		
		event CriticalErrorHandler CriticalError;
	}

	public delegate void CriticalErrorHandler(string message);
}
