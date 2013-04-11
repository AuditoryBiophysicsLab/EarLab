using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;

namespace EarLab.InheritedControls
{
	/// <summary>
	/// Summary description for NoKeyPressScrollBar.
	/// </summary>
	public class NoKeyPressScrollBar : System.Windows.Forms.HScrollBar
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public NoKeyPressScrollBar()
		{
			// This call is required by the Windows.Forms Form Designer.
			InitializeComponent();
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if( components != null )
					components.Dispose();
			}
			base.Dispose( disposing );
		}

		#region Component Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			components = new System.ComponentModel.Container();
		}
		#endregion

		protected override void OnPaint(PaintEventArgs pe)
		{
			// Calling the base class OnPaint
			base.OnPaint(pe);
		}

		protected override void OnKeyPress(KeyPressEventArgs e)
		{
			e.Handled = true;

			// Calling the base class OnKeyPress
			base.OnKeyPress (e);
		}

	}
}
