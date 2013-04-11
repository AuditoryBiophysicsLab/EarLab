using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;

namespace ModuleUpdater
{
	/// <summary>
	/// This class inherits from TextBox and creates a textbox which can only accept letters.  The delete key can be used to delete letters.
	/// </summary>
	public class NameTextBox : System.Windows.Forms.TextBox
	{
		public NameTextBox()
		{
		}

		protected override void OnKeyPress(KeyPressEventArgs e) 
		{
			// if the key pressed is a digit or the delete key we mark it as unhandled, and pass it down
			if(char.IsLetter(e.KeyChar) || e.KeyChar == (char)8 || e.KeyChar == (char)39 || e.KeyChar == (char)45
				|| e.KeyChar == (char)46 || e.KeyChar == (char)32) 
				e.Handled = false;
			else
				e.Handled = true;

			// we need to call the base class OnKeyPress to make sure that everything is handled correctly
			base.OnKeyPress(e);
		} 
	}
}
