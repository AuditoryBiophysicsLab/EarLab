using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;

namespace ModuleUpdater.InheritedControls
{
	/// <summary>
	/// This class inherits from TextBox and creates a textbox which can only accept numbers.  The delete key can be used to delete numbers.
	/// </summary>
	public class WholeNumberTextBox : System.Windows.Forms.TextBox
	{
		public WholeNumberTextBox()
		{
			if (this.Text == "")
				this.Text = "0";
		}

		protected override void OnKeyPress(KeyPressEventArgs e) 
		{
			// if the key pressed is a digit or the delete key we mark it as unhandled, and pass it down
			if(char.IsDigit(e.KeyChar) || e.KeyChar == (char)8)
				e.Handled = false;
			else
				e.Handled = true;

			// we need to call the base class OnKeyPress to make sure that everything is handled correctly
			base.OnKeyPress(e);
		} 

		protected override void OnTextChanged(EventArgs e)
		{
			if (this.Text == "")
			{
				this.Text = "0";
				this.SelectionStart = 0;
				this.SelectionLength = 1;
			}

			base.OnTextChanged(e);

		}
	}
}
