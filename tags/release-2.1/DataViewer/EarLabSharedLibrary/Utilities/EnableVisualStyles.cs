using System;
using System.Drawing;
using System.Windows.Forms;

namespace EarLab.Utilities
{
	/// <summary>
	/// Class used to enable visual (XP) styles in C#.NET applications
	/// </summary>
	public class EnableVisualStyles
	{
		/// <summary>
		/// Enable visual (XP) styles on the given control, and recursively all the controls it contains.
		/// </summary>
		/// <param name="parent">Control to begin recursively enabling visual (XP) styles.</param>
		public static void EnableControl(Control parent)
		{
			// we check to make sure that the Operating System is Windows XP or greater
			if (Environment.OSVersion.Version.Major > 4 & Environment.OSVersion.Version.Minor > 0)
				Enable(parent);
		}

		/// <summary>
		/// Enable visual (XP) styles on the the main Application, if possible.
		/// </summary>
		public static void EnableApplication()
		{
			// we check to make sure that the Operating System is Windows XP or greater enable XP themes
			if (Environment.OSVersion.Version.Major > 4 & Environment.OSVersion.Version.Minor > 0)
				Application.EnableVisualStyles();
			Application.DoEvents();
		}

		private static void Enable(Control parent)
		{
			foreach(Control control in parent.Controls) 
			{
				// Only these controls have a FlatStyle property
				ButtonBase button = control as ButtonBase;
				GroupBox group = control as GroupBox;
				//Label label = control as Label;
				TabPage tab = control as TabPage;

				if (button != null)
					button.FlatStyle = FlatStyle.System;
				else if (group != null)
					group.FlatStyle = FlatStyle.System;
				//else if (label != null)
				//	label.FlatStyle = FlatStyle.System;
				else if (tab != null)
					tab.BackColor = SystemColors.ControlLightLight;

				// Set contained controls FlatStyle.System, too
				Enable(control);
			}
		}
	}
}
