using System;
using System.Drawing;
using System.Windows.Forms;

namespace ModuleUpdater
{
	/// <summary>
	/// Summary description for EnableVisualStyles.
	/// </summary>
	public class EnableVisualStyles
	{
		public static void Enable(Control parent)
		{
			// we check to make sure that the Operating System is Windows XP or greater
			if (Environment.OSVersion.Version.Major > 4 & Environment.OSVersion.Version.Minor > 0)
				EnableControl(parent);
		}

		private static void EnableControl(Control parent)
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

				// Set contained controls FlatStyle, too
				EnableControl(control);
			}
		}
	}
}
