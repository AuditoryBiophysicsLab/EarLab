using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;

namespace ModuleUpdater
{
	/// <summary>
	/// This class inherits from TextBox and creates a textbox which can only accept letters, numbers, and the delete key
	/// </summary>
	public class AlphaNumeric : System.Windows.Forms.TextBox
	{
		public AlphaNumeric()
		{
		}

		protected override void OnKeyPress(KeyPressEventArgs e) 
		{
			// if the key pressed is a letter, digit, or the delete key we mark it as unhandled, and pass it down
			if (char.IsLetterOrDigit(e.KeyChar) || e.KeyChar == (char)8) 
				e.Handled = false;
			else
				e.Handled = true;

			// we need to call the base class OnKeyPress to make sure that everything is handled correctly
			base.OnKeyPress(e);
		} 
	}
}
